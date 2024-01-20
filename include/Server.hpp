#ifndef SERVER_HPP
#define SERVER_HPP

#include "Request.hpp"
#include <map>
#include <string>

#define BUF_SIZE 512
#define MAX_EVENTS 50

namespace Hafserv
{
class Server
{
public:
	Server();
	Server(int port);
	~Server();

	int getPort();
	int getServSock();
	void setServSock(int serv_sock);
	/*
		string -> Request
		parseRequest
		makeResponse
		sendResponse
	*/
private:
	int port;
	int kq;
	int serv_sock;
	std::string serverName;
	std::map<int, Hafserv::Request> Requests;
};
} // namespace Hafserv

#endif
