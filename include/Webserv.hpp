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

class Connection;

typedef std::map<int, Hafserv::Request> RequestMap;
typedef std::map<int, Connection> ConnectionMap;

class Webserv
{
public:
	Webserv();
	~Webserv();

	static void initWebserv();

	static void addServer(Server *server);
	static void openPort(unsigned short port);

	static void runWebserv();
	static void connectClient(int serv_sock);
	static void disconnectClient(int socketfd);

	static Server *findTargetServer(unsigned short port, const Request &request);

	static void checkTimeout();

	static bool inServSocks(int serv_sock);
	static void closeServSocks();

private:
	static int kq;

	static std::map<int, std::string> statusCodeMap;

	static std::map<unsigned short, int> portToServSock;
	static std::map<int, unsigned short> servSockToPort;
	static std::map<int, unsigned short> sockToPort;
	static std::vector<Server *> servers;

	static RequestMap Requests;
	static ConnectionMap Connections;
};

} // namespace Hafserv

#endif
