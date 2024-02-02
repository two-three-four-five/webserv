#include "Request.hpp"
#include "Parse.hpp"
#include "util/string.hpp"
#include <iostream>

using namespace Hafserv;

Request::Request()
	: parseStatus(Created), statusCode(200), targetServer(NULL), targetLocationConfig(NULL), bodyLength(0)
{
	startTime = time(NULL);
}

int Request::parse(const std::string &request)
{
	if (parseStatus == Created)
		parseStartLine(request);
	else if (parseStatus == Header)
		parseHeaders(request);
	else if (parseStatus == Body)
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
		parseStatus = End;
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
			parseStatus = End;
			return;
		} // 400 Bad Request

		if (httpVersion.substr(5) != "1.1")
		{
			statusCode = 505;
			parseStatus = End;
			return;
		} // 505 HTTP version not supported
	}
	if (method == "HEAD")
		parseStatus = End;
	else
		parseStatus = Header;
}

void Request::parseHeaders(const std::string &fieldLine)
{
	if (fieldLine == "\r\n")
	{
		// std::cout << "Header field end" << std::endl;
		if (method == "GET")
			parseStatus = End;
		else
		{
			parseStatus = Body;
			startTime = time(NULL);
		}
		return;
	}
	std::istringstream iss(fieldLine);
	std::string key, value;

	std::getline(iss, key, ':');
	if (util::string::hasSpace(key))
	{
		statusCode = 400;
		parseStatus = End;
		return;
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
}

void Request::parseBody(const std::string &line)
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
		return;
	}
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

void Hafserv::Request::setTargetLocation()
{
	int depth = -1;
	const std::vector<LocationConfig> &locations = targetServer->getServerConfig().getLocations();
	std::vector<LocationConfig>::const_iterator selectedIt = locations.end();

	for (std::vector<LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); it++)
	{
		// deep route first
		const std::string &pattern = (*it).getPattern();
		if (requestTarget.find(pattern) == 0)
		{
			int currDepth = pattern == "/" ? 0 : std::count(pattern.begin(), pattern.end(), '/');
			if (depth < currDepth)
			{
				depth = currDepth;
				selectedIt = it;
			}
		}
	}
	std::cout << "SELECTED LOCATION" << std::endl << selectedIt->getPattern() << std::endl;

	// root / is always presents in httpConfigCore
	if (selectedIt != locations.end())
	{
		targetLocationConfig = &(*selectedIt);

		const std::string &selectedPattern = selectedIt->getPattern();
		const std::string &selectedAlias = selectedIt->getAlias();
		if (!selectedAlias.empty())
		{
			if (selectedPattern.back() == '/')
				targetLocation = selectedAlias + requestTarget.substr(selectedPattern.length() - 1);
			else
				targetLocation = selectedAlias + requestTarget.substr(selectedPattern.length());
		}
		else
			targetLocation = selectedIt->getHttpConfigCore().getRoot() + requestTarget;
		if (targetLocation.back() == '/')
		{
			std::vector<std::string> indexes = selectedIt->getHttpConfigCore().getIndexes();
			std::string defaultTargetLocation = targetLocation + indexes[0];
			for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
			{
				File index(targetLocation + *it);
				if (index.getCode() == File::REGULAR_FILE)
				{
					targetLocation += *it;
					return;
				}
			}
			targetLocation = defaultTargetLocation;
		}
	}
}

const int Hafserv::Request::getParseStatus() const { return parseStatus; }

const HeaderMultiMap &Request::getHeaders() const { return headers; }

const Server *Hafserv::Request::getTargetServer() const { return targetServer; }

void Hafserv::Request::setTargetServer(Server *server) { targetServer = server; }

const time_t &Hafserv::Request::getStartTime() const { return startTime; }

const LocationConfig *Request::getTargetLocationConfig() const { return targetLocationConfig; }
