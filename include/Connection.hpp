#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Webserv.hpp"
#include <sys/stat.h>

namespace Hafserv
{

#define BUFFER_SIZE 50000

class Webserv;

class Connection
{
public:
	Connection(int socket, unsigned short port);
	Connection(const Connection &other);
	Connection &operator=(Connection &rhs);
	~Connection();

	bool readRequest(int fd);
	std::string configureTargetResource(std::string requestTarget);

	void buildResponseFromRequest();
	void buildGetResponse();
	void buildDeleteResponse();
	void buildErrorResponse(int statusCode);
	void build301Response(const std::string &redirectTarget);

	void callCGI(const std::string &scriptPath);
	char **makeEnvp();

	const Request &getRequest() const;
	const Server *getTargetServer() const;
	const time_t &getStartTime() const;
	const LocationConfig &getTargetLocationConfig() const;

private:
	Request request;
	Response response;
	int socket;
	unsigned short port;

	int statusCode;
	time_t startTime;
	Server *targetServer;
	LocationConfig targetLocationConfig;
	std::string targetResource;
};

} // namespace Hafserv
#endif
