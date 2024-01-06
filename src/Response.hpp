#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <vector>

namespace ft
{
class Response
{
public:
	Response();
	Response(std::string statusCode);
	~Response();

	std::string toString();

private:
	std::string statusCode;
	std::vector<std::string> headers;
	std::string body;
	std::vector<std::string> trailers;
};

} // namespace ft

#endif
