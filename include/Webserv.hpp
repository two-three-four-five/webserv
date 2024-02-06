#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Connection.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include <map>
#include <vector>

#define BUF_SIZE 512
#define MAX_EVENTS 50

namespace Hafserv
{

typedef std::map<int, Hafserv::Request> RequestMap;

class Connection;

class Webserv
{
public:
	Webserv();
	~Webserv();

	void initWebserv();

	void addServer(Server *server);
	void openPort(unsigned short port);

	void runWebserv();
	void connectClient(int serv_sock);
	void disconnectClient(int socketfd);

	Server *findTargetServer(unsigned short port, const Request &request);

	void checkTimeout();

	bool inServSocks(int serv_sock);
	void closeServSocks();

private:
	int kq;

	std::map<int, std::string> statusCodeMap;

	std::map<unsigned short, int> portToServSock;
	std::map<int, unsigned short> servSockToPort;
	std::map<int, unsigned short> sockToPort;
	std::vector<Server *> servers;

	RequestMap Requests;
	std::map<int, Connection> Connections;
};

} // namespace Hafserv

#endif
