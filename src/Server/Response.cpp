#include "Response.hpp"
#include "File.hpp"
#include "Webserv.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace Hafserv
{

Response::Response() {}

Response::Response(const Response &other) : statusLine(other.statusLine), headers(other.headers), body(other.body) {}

Response &Response::operator=(Response &rhs)
{
	if (this != &rhs)
	{
		statusLine = rhs.statusLine;
		headers = rhs.headers;
		body = rhs.body;
	}
	return *this;
}

Response::~Response() {}

void Response::makeBody(const LocationConfig &targetLocationConfig, const std::string &targetLocation)
{
	const std::multimap<std::string, std::string> &typeMap = targetLocationConfig.getHttpConfigCore().getTypes();
	std::multimap<std::string, std::string>::const_iterator typeIt =
		typeMap.find(targetLocation.substr(targetLocation.rfind('.') + 1));
	if (typeIt != typeMap.end())
	{
		std::string contentType = typeIt->second;
		if (typeIt->second == "text/html" || typeIt->second == "text/css" || typeIt->second == "text/xml")
			contentType += ";charset=UTF-8";
		addToHeaders("Content-Type", contentType);
	}
	else
		addToHeaders("Content-Type", "application/octet-stream");

	RegularFile targetFile(targetLocation);
	body = targetFile.getRawContents();

	std::ostringstream contentLengthOss;
	contentLengthOss << body.length();
	addToHeaders("Content-Length", contentLengthOss.str());
}

void Response::setStatusLine(std::string statusLine) { this->statusLine = statusLine; }

std::string Response::getResponse()
{
	std::string response;

	response = statusLine + "\r\n";
	for (HeaderMultiMap::iterator it = headers.begin(); it != headers.end(); it++)
		response += it->first + ": " + it->second + "\r\n";
	response += "\r\n";
	response += body;
	return response;
}

void Response::addToHeaders(std::string key, std::string value) { headers.insert(std::make_pair(key, value)); }

} // namespace Hafserv
