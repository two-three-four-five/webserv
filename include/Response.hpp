#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <Request.hpp>
#include <string>
#include <vector>

namespace Hafserv
{
class Response
{
public:
	Response();
	Response(Request &request);
	~Response();
	std::string callCGI(const std::string &scriptPath);
	std::string makeGetResponse(const Request &request);
	std::string makeBody(const std::string &requestTarget);
	std::string &getResponse();
	// std::string setBoundary(std::map<std::string, std::vector<std::string> > &message);
	char **makeEnvp();

private:
	std::string statusCode;
	std::string response;
	std::string boundary;
	Request &request;
	friend class Request;
};

} // namespace Hafserv

#endif
