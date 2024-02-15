#include "Http/Response.hpp"
#include "File/File.hpp"
#include "Webserv.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace Hafserv
{

Response::Response() : responseState(Created), writtenBytes(0), responseBytes(0) {}

Response::Response(const Response &other)
	: responseState(other.responseState), writtenBytes(0), responseBytes(0), statusLine(other.statusLine),
	  headers(other.headers), body(other.body)
{
}

Response &Response::operator=(const Response &rhs)
{
	if (this != &rhs)
	{
		responseState = rhs.responseState;
		responseBuffer = rhs.responseBuffer;
		writtenBytes = rhs.writtenBytes;
		responseBytes = rhs.responseBytes;
		statusLine = rhs.statusLine;
		headers = rhs.headers;
		body = rhs.body;
	}
	return *this;
}

Response::~Response() {}

void Response::makeBody(const LocationConfig &targetLocationConfig, const std::string &targetLocation)
{
	const std::multimap<std::string, std::string> &typeMap = targetLocationConfig.getTypes();
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
	body = targetFile.getContents();

	std::ostringstream contentLengthOss;
	contentLengthOss << body.length();
	addToHeaders("Content-Length", contentLengthOss.str());
}

void Response::setStatusLine(std::string statusLine) { this->statusLine = statusLine; }

void Response::setBody(const std::string &bodyString)
{
	body = bodyString;
	addToHeaders("Content-Length", util::string::itos(body.length()));
}

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

const Response::ResponseState Response::getResponseState() const { return responseState; }

void Response::setResponseState(ResponseState rs) { responseState = rs; }

void Response::setResponseBuffer()
{
	std::ostringstream oss;

	oss << statusLine << CRLF;
	for (HeaderMultiMap::iterator it = headers.begin(); it != headers.end(); it++)
		oss << it->first << ": " << it->second << CRLF;
	oss << CRLF;
	oss << body;
	responseBuffer = oss.str();
	responseBytes = responseBuffer.length();
	writtenBytes = 0;
	responseState = Ready;
}

void Response::addToHeaders(std::string key, std::string value) { headers.insert(std::make_pair(key, value)); }

const int Response::getWrittenBytes() const { return writtenBytes; }

const int Response::getResponseBytes() const { return responseBytes; }

void Response::send(int fd)
{
	const char *wrBuffer = responseBuffer.c_str();
	size_t bytesToWrite = std::min(responseBytes - writtenBytes, (unsigned long)BUFFER_SIZE);
	int ret = write(fd, wrBuffer + writtenBytes, bytesToWrite);
	if (ret != -1)
	{
		// std::cout << "sent: " << std::endl << std::string(wrBuffer + writtenBytes, ret);
		writtenBytes += ret;
	}
	if (writtenBytes == responseBytes)
	{
		// std::cout << "sendEnd" << std::endl;
		responseState = End;
		close(fd);
	}
}

} // namespace Hafserv
