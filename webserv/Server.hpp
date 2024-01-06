#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <map>
#include "Request.hpp"

#define BUF_SIZE 512
#define MAX_EVENTS 50

class Server
{
public:
	Server();
	Server(int port);
	~Server();

	void startServer();
	void connectClient(int serv_sock);
	void disconnectClient(int socketfd);
	/*
		string -> Request
		parseRequest
		makeResponse
		sendResponse
	*/
private:
	int port;
	int kq;
	std::string serverName;
	std::map<int, ft::Request> Requests;
};



#endif
