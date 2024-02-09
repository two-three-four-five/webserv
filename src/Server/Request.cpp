#include "Http/Request.hpp"
#include "util/Parse.hpp"
#include "util/string.hpp"
#include <iostream>

using namespace Hafserv;

Request::Request() : parseStatus(Created), bodyLength(0) {}

// Request::Request(const Request &other) {}
// Request::Request &operator=(const Request &rhs) {}
// Request::~Request() {}

int Request::parse(std::string request)
{
	if (parseStatus == Created)
		return parseStartLine(request);
	else if (parseStatus == Header)
		return parseHeaders(request);
	else if (parseStatus == Body)
		return (this->*parseBody)(request);
	return 0;
}

int Request::parseStartLine(const std::string &request)
{
	std::istringstream requestStream(request);
	std::string httpVersion;

	// GET /index.html HTTP/1.1
	std::getline(requestStream, method, ' ');
	std::getline(requestStream, requestTarget, ' ');
	std::getline(requestStream, httpVersion, '\r');

	if (httpVersion.find("HTTP/") != 0)
	{
		parseStatus = End;
		return 400;
	} // 400 Bad Request
	else
	{
		std::istringstream iss(httpVersion.substr(5));
		double ver;

		iss >> ver;
		if (iss.fail() || !iss.eof())
		{
			parseStatus = End;
			return 400;
		} // 400 Bad Request

		if (httpVersion.substr(5) != "1.1")
		{
			parseStatus = End;
			return 505;
		} // 505 HTTP version not supported
	}
	if (method == "HEAD")
		parseStatus = End;
	else
		parseStatus = Header;
	return 0;
}

int Request::parseHeaders(const std::string &fieldLine)
{
	if (fieldLine == "\r\n")
	{
		// std::cout << "Header field end" << std::endl;
		if (method == "GET")
			parseStatus = End;
		else
			parseStatus = Body;
		return 0;
	}
	std::istringstream iss(fieldLine);
	std::string key, value;

	std::getline(iss, key, ':');
	if (util::string::hasSpace(key))
	{
		parseStatus = End;
		return 400;
	} // 400 Bad Request
	while (std::getline(iss >> std::ws, value, ','))
	{
		if (value.empty())
			; // error ( NOT ERROR MAYBE )
		if (value[value.length() - 1] == '\n')
			value = value.substr(0, value.length() - 2);
		key = util::string::toLower(key);
		headers.insert(std::make_pair(key, value));
	}
	return 0;
}

void Request::checkHeaderField()
{
	// contentLength
	std::multimap<std::string, std::string>::iterator contentLengthIt = headers.find("content-length");
	if (contentLengthIt != headers.end())
	{
		contentLength = stoi(contentLengthIt->second);
		// parseBody = parseByContentLength;
		parseBody = &Request::parseByContentLength;
	}

	// Transfer-Encoding
	std::multimap<std::string, std::string>::iterator tranferEncodingIt = headers.find("transfer-encoding");
	if (tranferEncodingIt != headers.end())
	{
		std::vector<std::string> transferEncoding = parseTransferEncoding(tranferEncodingIt->second);
		std::vector<std::string>::iterator it = std::find(transferEncoding.begin(), transferEncoding.end(), "chunked");
		if (it + 1 == transferEncoding.end())
			parseBody = &Request::parseByTransferEncoding;

		else if (it != transferEncoding.end() || contentLength == -1)
			; // chunked가 있는데 마지막에 있는게 아니거나 | tranferEncoding이 있는데 contentLength도 있으면 에러
	}

	// Boundary
	std::multimap<std::string, std::string>::iterator contentTypeIt = headers.find("content-type");
	if (contentTypeIt != headers.end())
	{
		std::vector<std::string> contentType = parseContentType(contentTypeIt->second);
		std::vector<std::string>::iterator it = std::find(contentType.begin(), contentType.end(), "boundary");
		if (it != contentType.end())
		{
			boundary = *(it + 1);
			parseBody = &Request::parseByBoundary;
		}
	}
}

int Request::parseByContentLength(std::string &line)
{
	bodyVec.push_back(line);
	bodyLength += line.length();
	if (line == "\r\n")
	{
		std::ostringstream oss;
		for (std::vector<std::string>::iterator it = bodyVec.begin(); it != bodyVec.end(); it++)
			oss << *it;
		body = oss.str();
		parseStatus = End;
	}
	return 0;
}

int Request::parseByBoundary(std::string &line)
{
	// 계속 읽다가 Boundary--일때면 멈춤
	bodyVec.push_back(line);
	bodyLength += line.length();
	if (line.substr(2, line.length() - 6) == boundary)
	{
		std::ostringstream oss;
		for (std::vector<std::string>::iterator it = bodyVec.begin(); it != bodyVec.end(); it++)
			oss << *it;
		body = oss.str();
		parseStatus = End;
	}
	return 0;
}

int Request::parseByTransferEncoding(std::string &line)
{
	static int chunkSize;
	std::istringstream iss(line);

	if (!chunkSize)
	{
		chunkSize = std::stoi(readHex(line), NULL, 16) + 2;
		if (chunkSize == 2)
		{
			std::ostringstream oss;
			for (std::vector<std::string>::iterator it = bodyVec.begin(); it != bodyVec.end(); it++)
				oss << *it;
			body = oss.str();
			parseStatus = End;
		}
	}
	else
	{
		int lineLength = line.length();

		bodyLength += lineLength;
		chunkSize -= lineLength;
		bodyVec.push_back(line);
	}
	return 0;
}

std::string Request::getRawRequest()
{
	std::stringstream ss;

	ss << method << " " << requestTarget << " HTTP/1.1\r\n";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
		ss << it->first << ": " << it->second << "\r\n";
	if (method == "POST")
		ss << "\r\n" << body << "\r\n";
	return ss.str();
}

void Request::printRequest()
{
	std::cout << "<-------request------->" << std::endl;
	std::cout << method << " " << requestTarget << " HTTP/1.1\r\n";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
		std::cout << it->first << ": " << it->second << "\r\n";
	// if (method == "POST")
	// 	std::cout << "\r\n" << body[0] << "\r\n";
	std::cout << "<-----request end----->" << std::endl;
}

void Request::printBody()
{
	for (int i = 0; i < bodyVec.size(); i++)
	{
		std::cout << "body[" << i << "]:" << std::endl << bodyVec[i] << std::endl;
	}
}

const int Hafserv::Request::getParseStatus() const { return parseStatus; }

const std::string Hafserv::Request::getRequestTarget() const { return requestTarget; }

const HeaderMultiMap &Request::getHeaders() const { return headers; }

const std::string &Hafserv::Request::getMethod() const { return method; }

const std::string &Hafserv::Request::getBody() const { return body; }
