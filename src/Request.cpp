#include "Request.hpp"
#include "utils.hpp"
#include <iostream>

namespace ft
{
Request::Request() : status(CREATED) { status = HEADER; }
// 임시로 시작부분바꿈

int Request::parse(const std::string &request)
{
	if (status == CREATED)
		parseStartLine(request);
	else if (status == HEADER)
		parseFields(request);
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

	status = HEADER;
}

void Request::parseLine(const std::string &fieldLine)
{
	std::istringstream iss(fieldLine);
	std::string key, value;

	std::getline(iss, key, ":");
	std::getline(iss >> std::ws, value);
	if (isSpaceIncluded(key) || value.empty())
		; // error
	message[key].push_back(value);
}

void Request::parseFields(const std::string &request)
{
	std::istringstream requestStream(request);
	std::string line;
	std::string key, value;

	while (requestStream.good())
	{
		std::getline(requestStream, line);
		// if (line == "\n" || line == "")
		// {
		// 	status = PARSE_END;
		// 	return;
		// }
		std::istringstream lineStream(line);
		std::getline(lineStream, key, ':');			// field name
		std::getline(lineStream >> std::ws, value); // field value
		// field name과 ':' 사이에 띄어쓰기가 있는지 || 라인에 :가 없어서
		// value가 비었는지
		if (isSpaceIncluded(key) || value.empty())
			continue; // throw error
		message[key].push_back(value);
		// std::cout << "1 " << key << " " << value << std::endl;
		// std::cout << "2 " << message[key][0] << std::endl;
		// key = "";
		// value = "";
	}
}

void Request::printMessage()
{
	int i = 0;
	for (std::map<std::string, std::vector<std::string> >::iterator it = message.begin(); it != message.end(); it++)
	{
		std::cout << it->first << ": " << std::flush;
		for (std::vector<std::string>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); vecIt++)
		{
			std::cout << *vecIt << ", " << std::flush;
		}
		// std::cout << std::endl;
	}
};

} // namespace ft
