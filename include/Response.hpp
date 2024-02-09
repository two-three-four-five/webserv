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
	Response(const Response &other);
	Response &operator=(Response &rhs);
	~Response();

	void addToHeaders(std::string key, std::string value);
	void makeBody(const LocationConfig &targetLocationConfig, const std::string &requestTarget);

	void setStatusLine(std::string statusLine);
	std::string getResponse();

private:
	std::string statusLine;
	HeaderMultiMap headers;
	std::string body;
};

} // namespace Hafserv

#endif
