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
	void configureTargetResource();

	void buildResponseFromRequest();

	const Request &getRequest() const;
	const Server *getTargetServer() const;
	const time_t &getStartTime() const;
	const LocationConfig *getTargetLocationConfig() const;

private:
	Request request;
	Response response;
	int socket;
	unsigned short port;

	int statusCode;
	time_t startTime;
	Server *targetServer;
	const LocationConfig *targetLocationConfig;
	std::string targetResource;
};

} // namespace Hafserv
#endif
