#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
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

	void buildResponseFromRequest();
	std::string getTargetLocation(const std::string &requestTarget);

	void buildGetResponse(const std::string &targetLocation);
	void build301Response(const std::string &redirectTarget);
	void build400Response();
	void build405Response();
	void build413Response();
	void build404Response();
	void build505Response();

	void callCGI(const std::string &scriptPath);
	char **makeEnvp();

	void makeBody(const std::string &requestTarget);
	std::string getResponse();

	void addToHeaders(std::string key, std::string value);

private:
	std::string statusLine;
	HeaderMultiMap headers;
	std::string body;

	std::vector<LocationConfig>::const_iterator targetLocationConfig;

	Request &request;
	friend class Request;
};

} // namespace Hafserv

#endif
