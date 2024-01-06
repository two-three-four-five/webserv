#include "Server.hpp"

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <unistd.h>
#include <fcntl.h>

Server::Server()
{
}

Server::Server(int port) : port(port)
{
}

Server::~Server()
{
}

void Server::startServer()
{
	int serv_sock;
	struct sockaddr_in serv_adr;
	int str_len;
	char buf[BUF_SIZE];

	struct kevent event_list[MAX_EVENTS];
	struct kevent event;
	int ret;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(port);

	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
	{
		std::cerr << "bind() error" << std::endl;
		exit(1);
	}
	if (listen(serv_sock, 5) == -1)
	{
		std::cerr << "listen() error" << std::endl;
		exit(1);
	}

	kq = kqueue();
	if (kq == -1)
	{
		std::cerr << "kqueue() error" << std::endl;
		exit(1);
	}
	EV_SET(&event, serv_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
	ret = kevent(kq, &event, 1, NULL, 0, NULL);

	while (1)
	{
		// 연결된 후 입력이 오랫동안 없는 경우 생각하기
		int events = kevent(kq, NULL, 0, event_list, MAX_EVENTS, NULL);
		if (events == -1)
		{
			std::cerr << "kevent() error" << std::endl;
			break;
		}

		for (int i=0; i < events; i++)
		{
			if (event_list[i].ident == serv_sock)
			{
				connectClient(serv_sock);
			}
			else
			{
				// str_len = read(event_list[i].ident, buf, BUF_SIZE);
				str_len = recv(event_list[i].ident, buf, BUF_SIZE, MSG_PEEK);
				if (str_len == 0)
				{
					disconnectClient(event_list[i].ident);
				}
				else
				{
					std::string temp = buf;
					int idx = temp.find('\n');
					str_len = recv(event_list[i].ident, buf, idx + 1, MSG_DONTWAIT);
					std::string line = buf;

					ft::Request &request = (*Requests.find(event_list[i].ident)).second;
					try
					{
						request.parse(line);
					}
					catch(const ft::Request::httpException& e)
					{
						std::cerr << e.errCode << std::endl;
						// client에게 오류메시지 보내기
						disconnectClient(event_list[i].ident);
						continue;
					}

						//respoen
					request.printFields();
					std::cout<< std::endl;

					// write(event_list[i].ident, buf, str_len);
				}
			}
		}
	}
	close(serv_sock);
	close(kq);
}

void Server::connectClient(int serv_sock)
{
	int clnt_sock;
	struct sockaddr_in clnt_adr;
	socklen_t adr_sz;
	struct kevent event;

	adr_sz = sizeof(clnt_adr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
	EV_SET(&event, clnt_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
	kevent(kq, &event, 1, NULL, 0, NULL);

	std::pair<int, ft::Request> p;
	p.first = clnt_sock;
	Requests.insert(p);

	std::cout << "connected client: " << clnt_sock << std::endl;
}

void Server::disconnectClient(int socketfd)
{
	struct kevent event;
	EV_SET(&event, socketfd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	kevent(kq, &event, 1, NULL, 0, NULL);
	close(socketfd);

	Requests.erase(socketfd);

	std::cout << "closed client: " << socketfd << std::endl;
}
