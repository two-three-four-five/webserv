#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <Request.hpp>
#include <string>
#include <vector>

namespace ft
{
class Response
{
public:
	Response();
	Response(const Request &request);
	~Response();
	std::string callCGI(const std::string &scriptPath);
	std::string makeGetResponse(const Request &request);
	std::string makeBody(const std::string &requestTarget);
	std::string &getResponse();

private:
	std::string statusCode;
	std::string response;
	friend class Request;
};

} // namespace ft

#endif
