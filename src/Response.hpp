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

	std::string makeGetResponse(const Request &request);
	std::string makeErrorResponse(std::string statusCode);

	std::string makeBody(const std::string &requestTarget);
	std::string &getResponse();

	std::string callCGI(const std::string &scriptPath);
	char **makeEnvp();

	bool isFileExists(const std::string &filename);

private:
	static std::map<std::string, std::string> statusCodeMap;
	std::string statusCode;
	std::string response;
	std::string boundary;
	Request &request;
	friend class Request;
};

} // namespace Hafserv

#endif
