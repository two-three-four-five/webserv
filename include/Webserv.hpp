#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Request.hpp"
#include "Server.hpp"
#include <map>
#include <vector>

namespace Hafserv
{

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

	bool inServSocks(int serv_sock);
	void closeServSocks();

private:
	int kq;

	std::map<int, std::string> statusCodeMap;

	std::map<unsigned short, int> portToServSock;
	std::map<int, unsigned short> servSockToPort;
	std::map<int, unsigned short> sockToPort;
	std::vector<Server *> servers;

	std::map<int, Hafserv::Request> Requests;
};

} // namespace Hafserv

#endif
