#include "Request.hpp"
#include "Parse.hpp"
#include "util/string.hpp"
#include <iostream>

using namespace Hafserv;

Request::Request() : parseStatus(CREATED), statusCode(200), inBoundary(false) {}

int Request::parse(const std::string &request)
{
	if (parseStatus == CREATED)
		parseStartLine(request);
	else if (parseStatus == HEADER)
		parseFieldLine(request);
	else if (parseStatus == BODY)
		parseBody(request);
	return (parseStatus);
}

void Request::parseStartLine(const std::string &request)
{
	std::istringstream requestStream(request);
	std::string httpVersion;

	// GET /index.html HTTP/1.1
	std::getline(requestStream, method, ' ');
	std::getline(requestStream, requestTarget, ' ');
	std::getline(requestStream, httpVersion, '\r');

	if (httpVersion.find("HTTP/") != 0)
	{
		statusCode = 400;
		parseStatus = PARSE_END;
		return;
	} // 400 Bad Request
	else
	{
		std::istringstream iss(httpVersion.substr(5));
		double ver;

		iss >> ver;
		if (iss.fail() || !iss.eof())
		{
			statusCode = 400;
			parseStatus = PARSE_END;
			return;
		} // 400 Bad Request

		if (httpVersion.substr(5) != "1.1")
		{
			statusCode = 505;
			parseStatus = PARSE_END;
			return;
		} // 505 HTTP version not supported
	}
	if (requestTarget == "/")
		requestTarget = "/index.html"; // from Config
	parseStatus = HEADER;
}

void Request::parseFieldLine(const std::string &fieldLine)
{
	if (fieldLine == "\r\n")
	{
		// std::cout << "Header field end" << std::endl;
		if (method == "GET")
			parseStatus = PARSE_END;
		else
			parseStatus = BODY;
		return;
	}
	std::istringstream iss(fieldLine);
	std::string key, value;

	std::getline(iss, key, ':');
	if (util::string::hasSpace(key))
	{
		statusCode = 400;
		parseStatus = PARSE_END;
		return;
	} // 400 Bad Request
	while (std::getline(iss >> std::ws, value, ','))
	{
		if (value.empty())
			; // error ( NOT ERROR MAYBE )
		if (value[value.length() - 1] == '\n')
			value = value.substr(0, value.length() - 2);
		key = util::string::toLower(key);
		fields[key] = value;
	}
}

void Request::parseBody(const std::string &line)
{
	// https://www.rfc-editor.org/rfc/rfc9112.html#name-message-body-length
	// have to deal with transfer-encoding & content-length
	// content-length presents
	if (fields.find("content-length") != fields.end())
	{
	}

	body.push_back(line);

	std::vector<std::string> ContentType = parseContentType(fields["content-type"]);
	if (ContentType.size() == 4 && line.compare("--" + ContentType[3] + "\r\n") == 0)
		inBoundary = true;

	if ((inBoundary == false && line == "\r\n") ||
		ContentType.size() == 4 && line.compare("--" + ContentType[3] + "--\r\n") == 0)
	{
		// std::cout << "Body end" << std::endl;
		std::ostringstream oss;
		for (std::vector<std::string>::iterator it = body.begin(); it != body.end(); it++)
			oss << *it;
		body.clear();
		body.push_back(oss.str());
		// std::cout << body[0];
		parseStatus = PARSE_END;
		return;
	}
}

std::string Request::getRawRequest()
{
	std::stringstream ss;

	ss << method << " " << requestTarget << " HTTP/1.1\r\n";
	for (std::map<std::string, std::string>::iterator it = fields.begin(); it != fields.end(); it++)
		ss << (*it).first << ": " << (*it).second << "\r\n";
	if (method == "POST")
		ss << "\r\n" << body[0] << "\r\n";
	return ss.str();
}

void Request::printRequest()
{
	std::cout << "<-------request------->" << std::endl;
	std::cout << method << " " << requestTarget << " HTTP/1.1\r\n";
	for (std::map<std::string, std::string>::iterator it = fields.begin(); it != fields.end(); it++)
		std::cout << (*it).first << ": " << (*it).second << "\r\n";
	if (method == "POST")
		std::cout << "\r\n" << body[0] << "\r\n";
	std::cout << "<-----request end----->" << std::endl;
}

void Request::printBody()
{
	for (int i = 0; i < body.size(); i++)
	{
		std::cout << "body[" << i << "]:" << std::endl << body[i] << std::endl;
	}
}

// void Request::printMessage()
// {
// 	for (std::map<std::string, std::vector<std::string> >::iterator it = message.begin(); it != message.end(); it++)
// 	{
// 		std::cout << it->first << ": ";
// 		for (std::vector<std::string>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); vecIt++)
// 		{
// 			std::cout << *vecIt << ". ";
// 		}
// 		std::cout << std::endl;
// 	}
// }

// std::map<std::string, std::vector<std::string> > &Request::getMessage() { return (message); }
