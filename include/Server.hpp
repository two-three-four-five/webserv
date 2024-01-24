#ifndef SERVER_HPP
#define SERVER_HPP

#include "Request.hpp"
#include <string>
#include <vector>

#define BUF_SIZE 512
#define MAX_EVENTS 50

namespace Hafserv
{
class Server
{
public:
	Server(int port);
	~Server();

	std::vector<int> &getPorts();
	std::vector<std::string> &getServerNames();
	/*
		string -> Request
		parseRequest
		makeResponse
		sendResponse
	*/
private:
	std::vector<int> ports;
	std::vector<std::string> serverNames;
};
} // namespace Hafserv

#endif
