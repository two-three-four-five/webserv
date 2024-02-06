#include "Webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <algorithm>
#include <arpa/inet.h>
#include <iostream>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace Hafserv;

Webserv &Webserv::getInstance()
{
	static Webserv webserv;
	return webserv;
}

Webserv::Webserv()
{
	initWebserv();
	statusCodeMap[200] = "OK";
	statusCodeMap[201] = "Created";
	statusCodeMap[202] = "Accepted";
	statusCodeMap[204] = "No Content";
	statusCodeMap[400] = "Bad Request";
	statusCodeMap[403] = "Forbidden";
	statusCodeMap[404] = "Not Found";
	statusCodeMap[500] = "Internal Server Error";
	statusCodeMap[501] = "Not Implemented";
	statusCodeMap[505] = "HTTP Version Not Supported";
}

Webserv::~Webserv()
{
	for (std::vector<Server *>::iterator it = servers.begin(); it != servers.end(); it++)
		delete *it;
}

void Webserv::addServer(Server *server)
{
	servers.push_back(server);

	for (std::vector<unsigned short>::const_iterator it = server->getPorts().begin(); it != server->getPorts().end();
		 it++)
		openPort(*it);
}

void Hafserv::Webserv::openPort(unsigned short port)
{
	if (portToServSock.find(port) == portToServSock.end())
	{
		struct sockaddr_in serv_adr;
		struct kevent event;
		int socketFd;

		socketFd = socket(PF_INET, SOCK_STREAM, 0);
		memset(&serv_adr, 0, sizeof(serv_adr));
		serv_adr.sin_family = AF_INET;
		serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_adr.sin_port = htons(port);

		if (bind(socketFd, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		{
			std::cerr << "bind() error" << std::endl;
			exit(1);
		}
		if (listen(socketFd, 5) == -1)
		{
			std::cerr << "listen() error" << std::endl;
			exit(1);
		}

		int ret;
		EV_SET(&event, socketFd, EVFILT_READ, EV_ADD, 0, 0, NULL);
		ret = kevent(kq, &event, 1, NULL, 0, NULL);

		portToServSock[port] = socketFd;
		servSockToPort[socketFd] = port;
	}
}

void Webserv::initWebserv()
{
	kq = kqueue();
	if (kq == -1)
	{
		std::cerr << "kqueue() error" << std::endl;
		exit(1);
	}
}

void Webserv::runWebserv()
{
	struct kevent event_list[MAX_EVENTS];

	while (1)
	{
		// 연결된 후 입력이 오랫동안 없는 경우 생각하기
		struct timespec timeout;
		bzero(&timeout, sizeof(struct timespec));
		int events = kevent(kq, NULL, 0, event_list, MAX_EVENTS, &timeout);
		if (events == -1)
		{
			std::cerr << "kevent() error" << std::endl;
			continue;
		}

		for (int i = 0; i < events; i++)
		{
			if (inServSocks(event_list[i].ident))
			{
				connectClient(event_list[i].ident);
			}
			else if (event_list[i].filter == EVFILT_READ)
			{
				Connection &conn = Connections.find(event_list[i].ident)->second;
				if (!conn.readRequest(event_list->ident))
					disconnectClient(event_list[i].ident);
			}
		}
		checkTimeout();
	}
	closeServSocks();
	close(kq);
}

void Webserv::connectClient(int serv_sock)
{
	int clnt_sock;
	struct sockaddr_in clnt_adr;
	socklen_t adr_sz;
	struct kevent event;

	adr_sz = sizeof(clnt_adr);
	clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
	fcntl(clnt_sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	EV_SET(&event, clnt_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(kq, &event, 1, NULL, 0, NULL);
	EV_SET(&event, clnt_sock, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	kevent(kq, &event, 1, NULL, 0, NULL);

	std::pair<int, Hafserv::Request> p;
	p.first = clnt_sock;
	Requests.insert(p);

	int port = servSockToPort.find(serv_sock)->second;
	sockToPort[clnt_sock] = port;

	Connections.insert(std::make_pair(clnt_sock, Connection(clnt_sock, sockToPort.find(clnt_sock)->second)));

	std::cout << "connected client: " << clnt_sock << std::endl;
}

void Webserv::disconnectClient(int socketfd)
{
	struct kevent event;
	EV_SET(&event, socketfd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	kevent(kq, &event, 1, NULL, 0, NULL);
	EV_SET(&event, socketfd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	kevent(kq, &event, 1, NULL, 0, NULL);
	close(socketfd);

	Requests.erase(socketfd);
	Connections.erase(socketfd);

	std::map<int, unsigned short>::iterator socketToPortIt = sockToPort.find(socketfd);
	if (socketToPortIt != sockToPort.end())
		sockToPort.erase(socketToPortIt);

	std::cout << "closed client: " << socketfd << std::endl;
}

Server *Hafserv::Webserv::findTargetServer(unsigned short port, const Request &request)
{
	Server *defaultServer = NULL;
	std::string host = request.getHeaders().find("host")->second;

	std::string hostName = host;
	if (host.find(':') != std::string::npos)
		hostName = host.substr(0, host.find(':'));

	for (std::vector<Server *>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::vector<std::string> serverNames = (*it)->getNames();
		std::vector<unsigned short> ports = (*it)->getPorts();
		std::vector<std::string>::iterator findNameIt = std::find(serverNames.begin(), serverNames.end(), hostName);
		std::vector<unsigned short>::iterator findPortIt = std::find(ports.begin(), ports.end(), port);

		if (defaultServer == NULL && findPortIt != ports.end())
			defaultServer = *it;
		if (findNameIt != serverNames.end() && findPortIt != ports.end())
			return *it;
	}
	return defaultServer;
}

void Hafserv::Webserv::checkTimeout()
{
	time_t now = time(NULL);
	std::vector<int> timeoutSockets;
	for (RequestMap::iterator it = Requests.begin(); it != Requests.end(); it++)
	{
		const Server *server = it->second.getTargetServer();
		const LocationConfig *targetConfig = it->second.getTargetLocationConfig();
		if (it->second.getParseStatus() < Body)
		{
			int headerTimeout = 60;
			if (server != NULL)
				headerTimeout = server->getServerConfig().getHttpConfigCore().getTimeout().clientHeader;
			if (targetConfig != NULL)
				headerTimeout = targetConfig->getHttpConfigCore().getTimeout().clientHeader;
			if (now - it->second.getStartTime() > headerTimeout)
			{
				timeoutSockets.push_back(it->first);
			}
		}
		else if (it->second.getParseStatus() == Body)
		{
			int bodyTimeout = 60;
			if (targetConfig != NULL)
				bodyTimeout = targetConfig->getHttpConfigCore().getTimeout().clientBody;
			if (now - it->second.getStartTime() > bodyTimeout)
			{
				timeoutSockets.push_back(it->first);
			}
		}
	}
	for (std::vector<int>::iterator it = timeoutSockets.begin(); it != timeoutSockets.end(); it++)
		disconnectClient(*it);
}

bool Webserv::inServSocks(int serv_sock) { return servSockToPort.find(serv_sock) != servSockToPort.end(); }

void Webserv::closeServSocks()
{
	for (std::map<int, unsigned short>::iterator it = servSockToPort.begin(); it != servSockToPort.end(); it++)
	{
		close((*it).first);
	}
}
