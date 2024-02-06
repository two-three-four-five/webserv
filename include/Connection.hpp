#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Webserv.hpp"

namespace Hafserv
{

#define BUFFER_SIZE 1024

class Webserv;

class Connection
{
public:
	Connection(int socket, unsigned short port);
	~Connection();

	bool readRequest(int fd);

	Request request;

private:
	Webserv *webserv;
	Response *response;
	int socket;
	unsigned short port;

	int statusCode;
};

} // namespace Hafserv
#endif
