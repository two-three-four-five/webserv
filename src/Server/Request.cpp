#include "Request.hpp"
#include "Parse.hpp"
#include "util/string.hpp"
#include <iostream>

using namespace Hafserv;

Request::Request() : parseStatus(Created), bodyLength(0) {}

int Request::parse(const std::string &request)
{
	if (parseStatus == Created)
		return parseStartLine(request);
	else if (parseStatus == Header)
		return parseHeaders(request);
	else if (parseStatus == Body)
		return parseBody(request);
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

int Request::parseBody(const std::string &line)
{
	// https://www.rfc-editor.org/rfc/rfc9112.html#name-message-body-length
	// have to deal with transfer-encoding & content-length
	// content-length presents
	static bool inBoundary = false;

	if (headers.find("content-length") != headers.end())
	{
	}

	body.push_back(line);

	for (int i = 0; i < body.size(); i++)
		bodyLength += body[i].length();

	std::vector<std::string> ContentType = parseContentType(headers.find("content-type")->second);
	if (ContentType.size() == 4 && line.compare("--" + ContentType[3] + "\r\n") == 0)
		inBoundary = true;

	if ((inBoundary == false && (line == "\r\n" || line == "\n")) ||
		ContentType.size() == 4 && line.compare("--" + ContentType[3] + "--\r\n") == 0)
	{
		// std::cout << "Body end" << std::endl;
		std::ostringstream oss;
		for (std::vector<std::string>::iterator it = body.begin(); it != body.end(); it++)
			oss << *it;
		body.clear();
		body.push_back(oss.str());
		// std::cout << body[0];
		parseStatus = End;
		return 0;
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
		ss << "\r\n" << body[0] << "\r\n";
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
	for (int i = 0; i < body.size(); i++)
	{
		std::cout << "body[" << i << "]:" << std::endl << body[i] << std::endl;
	}
}

const int Hafserv::Request::getParseStatus() const { return parseStatus; }

const std::string Hafserv::Request::getRequestTarget() const { return requestTarget; }

const HeaderMultiMap &Request::getHeaders() const { return headers; }

const std::string &Hafserv::Request::getMethod() const { return method; }

const std::string &Hafserv::Request::getBody() const { return body[0]; }
