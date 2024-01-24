#ifndef WEBSERV_HPP
#define WEBSERV_HPP

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

	bool inServSocks(int serv_sock);
	void closeServSocks();

private:
	int kq;

	std::map<unsigned short, int> portToSocket;
	std::map<int, unsigned short> socketToPort;
	std::vector<Server *> servers;

	std::map<int, Hafserv::Request> Requests;
};

} // namespace Hafserv

#endif
