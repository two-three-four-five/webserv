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

Server::Server() {}

Server::Server(int port) : port(port) {}

Server::~Server() {}

int Server::getPort() { return port; }

int Server::getServSock() { return serv_sock; }

void Server::setServSock(int serv_sock) { this->serv_sock = serv_sock; }

} // namespace Hafserv
