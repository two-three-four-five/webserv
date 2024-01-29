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
	void build301Response(std::string redirectTarget);
	void build405Response();
	void buildErrorResponse(int statusCode);

	void callCGI(const std::string &scriptPath);
	void makeBody(const std::string &requestTarget);
	std::string getResponse();
	char **makeEnvp();

	bool isFileExists(const std::string &filename);

private:
	std::string statusLine;
	std::vector<std::string> fields;
	std::string body;

	Request &request;
	friend class Request;
};

} // namespace Hafserv

#endif
