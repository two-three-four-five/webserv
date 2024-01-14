#include "Request.hpp"
#include "utils.hpp"
#include <iostream>

namespace ft
{
Request::Request() : status(CREATED), errCode(200) {}

int Request::parse(const std::string &request)
{
	if (status == CREATED)
		parseStartLine(request);
	else if (status == HEADER)
		parseLine(request);
	else if (status == BODY)
		parseBody(request);
	return (status);
}

void Request::parseStartLine(const std::string &request)
{
	std::istringstream requestStream(request);
	std::string httpVersion;

	// GET /index.html HTTP/1.1
	requestStream >> method >> requestTarget >> httpVersion;
	// if (!requestStream.eof())
	// 	throw new httpException(400); //throw 400 bad request

	if (httpVersion.find("HTTP/") != 0)
		throw httpException(400); // throw 400 bad request
	else
	{
		std::istringstream iss(httpVersion.substr(5));
		double ver;

		iss >> ver;
		if (iss.fail() || !iss.eof())
			throw httpException(400); // throw 400 bad request

		if (httpVersion.substr(5) != "1.1")
			throw httpException(505); // throw 505 HTTP version not supported
	}
	if (requestTarget == "/")
		requestTarget = "/index.html";
	status = HEADER;
}

void Request::parseLine(const std::string &fieldLine)
{
	if (fieldLine == "\r\n")
	{
		std::cout << "Header field end" << std::endl;
		if (method == "GET")
			status = TRAILER;
		else
			status = BODY;
		return;
	}
	std::istringstream iss(fieldLine);
	std::string key, value;

	std::getline(iss, key, ':');
	if (isSpaceIncluded(key))
		; // error
	while (std::getline(iss >> std::ws, value, ','))
	{
		if (value.empty())
			; // error
		if (value[value.length() - 1] == '\n')
			value = value.substr(0, value.length() - 2);
		message[key].push_back(value);
	}
}

void Request::parseBody(const std::string &line)
{
	if (line == "\r\n")
	{
		std::cout << "Body end" << std::endl;
		std::ostringstream oss;
		for (std::vector<std::string>::iterator it = message["body"].begin(); it != message["body"].end(); it++)
			oss << *it;
		message["body"].clear();
		message["body"].push_back(oss.str());
		status = TRAILER;
		std::cout << message["body"].at(0);
		return;
	}
	message["body"].push_back(line);
}

void Request::printMessage()
{
	for (std::map<std::string, std::vector<std::string> >::iterator it = message.begin(); it != message.end(); it++)
	{
		std::cout << it->first << ": ";
		for (std::vector<std::string>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); vecIt++)
		{
			std::cout << *vecIt << ". ";
		}
		std::cout << std::endl;
	}
}

std::map<std::string, std::vector<std::string> > &Request::getMessage() { return (message); }

} // namespace ft
