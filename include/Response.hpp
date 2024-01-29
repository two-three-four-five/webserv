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
	std::string getTargetLocation(const std::string &requestTarget);

	void buildGetResponse(const std::string &targetLocation);
	void build301Response(const std::string &redirectTarget);
	void build405Response();
	void build404Response();

	void callCGI(const std::string &scriptPath);
	void makeBody(const std::string &requestTarget);
	std::string getResponse();
	char **makeEnvp();

private:
	std::string statusLine;
	std::vector<std::string> fields;
	std::string body;

	std::vector<LocationConfig>::const_iterator targetLocationConfig;

	Request &request;
	friend class Request;
};

} // namespace Hafserv

#endif
