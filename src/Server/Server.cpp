#include "Server.hpp"
#include "Response.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

namespace Hafserv
{

Server::Server(int port) { ports.push_back(port); }

Server::~Server() {}

std::vector<int> &Server::getPorts() { return ports; }

std::vector<std::string> &Server::getServerNames() { return serverNames; }

} // namespace Hafserv
