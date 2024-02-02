#ifndef SERVER_HPP
#define SERVER_HPP

#include "ServerConfig.hpp"
#include <string>
#include <vector>

namespace Hafserv
{
class Server
{
public:
	Server(ServerConfig serverConfig);
	~Server();

	const std::vector<unsigned short> &getPorts() const;
	const std::vector<std::string> &getNames() const;
	const ServerConfig &getServerConfig() const;
	/*
		string -> Request
		parseRequest
		makeResponse
		sendResponse
	*/
private:
	ServerConfig serverConfig;
};
} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::Server &server);

#endif
