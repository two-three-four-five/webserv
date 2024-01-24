#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "Server.hpp"
#include <vector>

namespace Hafserv
{

class Webserv
{
public:
	Webserv();
	~Webserv();

	void addServer(Server *server);
	void initServer();
	void runWebserv();
	void connectClient(int serv_sock);
	void disconnectClient(int socketfd);
	bool inServSocks(int serv_sock);

	void closeServSocks();

private:
	int kq;
	std::vector<int> serv_socks;
	std::vector<Server *> servers;
	std::map<int, Hafserv::Request> Requests;
};

} // namespace Hafserv

#endif
