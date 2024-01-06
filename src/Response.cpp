#include "Response.hpp"

#include <iostream>

namespace ft
{
Response::Response()
{
}

Response::Response(std::string statusCode) : statusCode(statusCode)
{
	/* temp headers */
	headers.push_back("server: NWS");
	headers.push_back("date: Sat, 06 Jan 2024 11:54:12 GMT");
	headers.push_back("content-type: text/html");
	headers.push_back("location: http://www.naver.com/");
	headers.push_back("vary: Accept-Encoding,User-Agent");
	headers.push_back("referrer-policy:unsafe-url");
}

Response::~Response()
{
}

std::string Response::toString()
{
	std::string response;

	response = "HTTP/1.1 " + statusCode + "\r\n"; // reason-phrase
	if (headers.size() > 0)
	{
		for (std::vector<std::string>::iterator it = headers.begin();
			 it != headers.end(); it++)
			response += *it + "\r\n";
		response += "\r\n";
	}
	if (body.length() > 0)
		response += body;
	if (trailers.size() > 0)
	{
		for (std::vector<std::string>::iterator it = trailers.begin();
			 it != trailers.end(); it++)
			response += *it + "\r\n";
		response += "\r\n";
	}
	return response;
}

} // namespace ft
