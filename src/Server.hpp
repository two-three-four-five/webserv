#ifndef SERVER_HPP
#define SERVER_HPP

#include "Request.hpp"
#include <map>
#include <string>

#define BUF_SIZE 512
#define MAX_EVENTS 50
#define PARSE_END 5

namespace ft
{
class Server
{
public:
	Server();
	Server(int port);
	~Server();

	void initServer();
	void startServer();
	void connectClient(int serv_sock);
	void disconnectClient(int socketfd);
	std::string callCGI(const std::string &scriptPath, const std::string &queryString);
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
	std::map<int, ft::Request> Requests;
};
} // namespace ft

#endif
