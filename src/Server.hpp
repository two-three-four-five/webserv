#ifndef SERVER_HPP
#define SERVER_HPP

#include "Request.hpp"
#include <map>
#include <string>

#define BUF_SIZE 512
#define MAX_EVENTS 50

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
	std::string callCGI(const std::string &scriptPath);
	std::string makeResponse(std::map<std::string, std::vector<std::string> > &message);
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
