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
	int str_len;
	char peekBuf[BUF_SIZE + 1];
	char readBuf[BUF_SIZE + 1];
	struct kevent event_list[MAX_EVENTS];

	while (1)
	{
		// 연결된 후 입력이 오랫동안 없는 경우 생각하기
		int events = kevent(kq, NULL, 0, event_list, MAX_EVENTS, NULL);
		if (events == -1)
		{
			std::cerr << "kevent() error" << std::endl;
			break;
		}

		for (int i = 0; i < events; i++)
		{
			if (inServSocks(event_list[i].ident))
			{
				connectClient(event_list[i].ident);
			}
			else if (event_list[i].filter == EVFILT_READ)
			{
				memset(peekBuf, 0, sizeof(peekBuf));
				str_len = recv(event_list[i].ident, peekBuf, BUF_SIZE, MSG_PEEK | MSG_DONTWAIT);

				if (str_len == 0)
				{
					disconnectClient(event_list[i].ident);
				}
				else
				{
					int idx = static_cast<std::string>(peekBuf).find('\n');
					memset(readBuf, 0, sizeof(readBuf));
					read(event_list[i].ident, readBuf, idx + 1);
					Request &request = Requests.find(event_list[i].ident)->second;
					try
					{
						// 일단 TRAILER로 해놓음. 추후에 PARSE_END로 바꾸어야함
						request.parse(static_cast<std::string>(readBuf));

						if (request.getParseStatus() >= BODY && request.getTargetServer() == NULL)
						{
							request.setTargetServer(
								findTargetServer(sockToPort.find(event_list[i].ident)->second, request));
						}
						if (request.getParseStatus() == PARSE_END)
						{
							Hafserv::Response response(request);
							std::string responseString = response.getResponse();
							std::cout << "<-------response------->" << std::endl << responseString;
							send(event_list[i].ident, responseString.c_str(), responseString.length(), 0);
							disconnectClient(event_list[i].ident);
						}
					}
					catch (const std::exception &e)
					{
						std::cerr << e.what() << std::endl;
						// client에게 오류메시지 보내기
						disconnectClient(event_list[i].ident);
						continue;
					}
					// write(event_list[i].ident, buf, str_len);
				}
			}
		}
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
	EV_SET(&event, clnt_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(kq, &event, 1, NULL, 0, NULL);
	EV_SET(&event, clnt_sock, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	kevent(kq, &event, 1, NULL, 0, NULL);

	std::pair<int, Hafserv::Request> p;
	p.first = clnt_sock;
	Requests.insert(p);

	int port = servSockToPort.find(serv_sock)->second;
	sockToPort[clnt_sock] = port;

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

	std::map<int, unsigned short>::iterator socketToPortIt = sockToPort.find(socketfd);
	if (socketToPortIt != sockToPort.end())
		sockToPort.erase(socketToPortIt);

	std::cout << "closed client: " << socketfd << std::endl;
}

Server *Hafserv::Webserv::findTargetServer(unsigned short port, const Request &request)
{
	Server *defaultServer = NULL;
	std::string host = request.getFields().find("host")->second;

	std::string hostName = host;
	if (host.find(':') != std::string::npos)
		hostName = host.substr(0, host.find(':'));

	for (std::vector<Server *>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::vector<std::string> serverNames = (*it)->getNames();
		std::vector<unsigned short> ports = (*it)->getPorts();
		std::vector<std::string>::iterator findNameIt = find(serverNames.begin(), serverNames.end(), hostName);
		std::vector<unsigned short>::iterator findPortIt = find(ports.begin(), ports.end(), port);

		if (defaultServer == NULL && findPortIt != ports.end())
			defaultServer = *it;
		if (findNameIt != serverNames.end() && findPortIt != ports.end())
			return *it;
	}
	return defaultServer;
}

bool Webserv::inServSocks(int serv_sock) { return servSockToPort.find(serv_sock) != servSockToPort.end(); }

void Webserv::closeServSocks()
{
	for (std::map<int, unsigned short>::iterator it = servSockToPort.begin(); it != servSockToPort.end(); it++)
	{
		close((*it).first);
	}
}
