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
	Response(Request &request);
	~Response();

	void buildResponseFromRequest();
	std::string getTargetLocation();

	void buildGetResponse(std::string targetLocation);
	void build404Response();

	void callCGI(const std::string &scriptPath);
	void makeBody(const std::string &requestTarget);
	std::string getResponse();
	char **makeEnvp();

private:
	std::string statusLine;
	std::vector<std::string> fields;
	std::string body;

	Request &request;
	friend class Request;
};

} // namespace Hafserv

#endif
