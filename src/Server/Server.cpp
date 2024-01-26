#include "Server.hpp"
#include "Response.hpp"

namespace Hafserv
{

Server::Server(ServerConfig serverConfig) : serverConfig(serverConfig) {}

Server::~Server() {}

const std::vector<unsigned short> &Server::getPorts() const { return serverConfig.getPorts(); }

const std::vector<std::string> &Server::getNames() const { return serverConfig.getNames(); }

const ServerConfig &Server::getServerConfig() const { return serverConfig; }

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::Server &server)
{
	os << "Server Info" << std::endl;
	os << server.getServerConfig();
	return os;
}
