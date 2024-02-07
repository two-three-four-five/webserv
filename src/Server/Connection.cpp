#include "Connection.hpp"

#include "File.hpp"
#include <string>
#include <sys/socket.h>

using namespace Hafserv;

Connection::Connection(int socket, unsigned short port) : socket(socket), port(port), targetServer(NULL)
{
	startTime = time(NULL);
}

Connection::~Connection() {}

bool Connection::readRequest(int fd)
{
	int str_len;
	char peekBuf[BUFFER_SIZE + 1];
	char readBuf[BUFFER_SIZE + 1];

	bzero(peekBuf, BUFFER_SIZE);
	str_len = recv(fd, peekBuf, BUFFER_SIZE, MSG_PEEK | MSG_DONTWAIT);
	// peekBuf[str_len] = 0;

	if (str_len == 0)
	{
		std::cout << "here" << std::endl;
		return false;
	}
	else
	{
		int idx = static_cast<std::string>(peekBuf).find('\n');
		if (request.getParseStatus() == Body && idx == std::string::npos)
		{
			idx = BUFFER_SIZE - 1;
		}
		else if (idx == std::string::npos)
			return true;
		str_len = read(fd, readBuf, idx + 1);
		readBuf[str_len] = 0;

		statusCode = request.parse(static_cast<std::string>(readBuf));
		if (request.getParseStatus() >= Body && targetServer == NULL)
		{
			targetServer = Webserv::getInstance().findTargetServer(port, request);
			configureTargetResource();
		}
		if (request.getParseStatus() == End)
		{
			request.printRequest();
			Response response(request);
			std::string responseString = response.getResponse();
			// std::cout << "<-------response------->" << std::endl << responseString;
			// std::cout << "<-----response end----->" << std::endl;
			// send(fd, responseString.c_str(), responseString.length(), 0);
			write(fd, responseString.c_str(), responseString.length());
			return false;
		}
	}
	return true;
}

void Connection::configureTargetResource()
{
	int depth = -1;
	const std::vector<LocationConfig> &locations = targetServer->getServerConfig().getLocations();
	std::vector<LocationConfig>::const_iterator selectedIt = locations.end();

	for (std::vector<LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); it++)
	{
		// deep route first
		const std::string &pattern = (*it).getPattern();
		if (request.getRequestTarget().find(pattern) == 0)
		{
			int currDepth = pattern == "/" ? 0 : std::count(pattern.begin(), pattern.end(), '/');
			if (depth < currDepth)
			{
				depth = currDepth;
				selectedIt = it;
			}
		}
	}

	// root / is always presents in httpConfigCore
	if (selectedIt != locations.end())
	{
		targetLocationConfig = &(*selectedIt);

		const std::string &selectedPattern = selectedIt->getPattern();
		const std::string &selectedAlias = selectedIt->getAlias();
		if (!selectedAlias.empty())
		{
			if (selectedPattern.back() == '/')
				targetResource = selectedAlias + request.getRequestTarget().substr(selectedPattern.length() - 1);
			else
				targetResource = selectedAlias + request.getRequestTarget().substr(selectedPattern.length());
		}
		else
			targetResource = selectedIt->getHttpConfigCore().getRoot() + request.getRequestTarget();
		if (targetResource.back() == '/')
		{
			std::vector<std::string> indexes = selectedIt->getHttpConfigCore().getIndexes();
			std::string defaultTargetResource;
			if (indexes.size() == 0)
				defaultTargetResource = targetResource + "index.html";
			else
				defaultTargetResource = targetResource + indexes[0];
			for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
			{
				File index(targetResource + *it);
				if (index.getCode() == File::REGULAR_FILE)
				{
					targetResource += *it;
					return;
				}
			}
			targetResource = defaultTargetResource;
		}
	}
}

void Connection::buildResponseFromRequest()
{
	std::cout << std::endl << "TARGET\n" << targetResource << std::endl;
	File targetFile(targetResource);
	std::string method = request.getMethod();

	response.addToHeaders("Server", "Hafserv/1.0.0");

	if (method == "GET")
	{
		if (targetFile.getCode() == File::DIRECTORY)
			response.build301Response("http://" + request.getHeaders().find("host")->second +
									  request.getRequestTarget() + "/");
		else if (targetFile.getCode() == File::REGULAR_FILE || targetLocationConfig->getProxyPass().length() != 0)
			response.buildGetResponse(targetResource);
		else if (targetFile.getCode() != File::REGULAR_FILE)
			response.build404Response();
	}
	else if (method == "HEAD")
		response.build405Response();
	else if (method == "POST")
	{
		if (request.getRequestTarget() == "/")
			response.build405Response();
		else
			response.callCGI(targetResource);
	}
}

const Request &Connection::getRequest() const { return request; }

const Server *Connection::getTargetServer() const { return targetServer; }

const time_t &Connection::getStartTime() const { return startTime; }

const LocationConfig *Connection::getTargetLocationConfig() const { return targetLocationConfig; }
