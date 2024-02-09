#include "Connection.hpp"

#include "File.hpp"
#include <string>
#include <sys/socket.h>

using namespace Hafserv;

Connection::Connection(int socket, unsigned short port) : socket(socket), port(port), targetServer(NULL)
{
	startTime = time(NULL);
}

Hafserv::Connection::Connection(const Connection &other)
	: request(other.request), response(other.response), socket(other.socket), port(other.port),
	  statusCode(other.statusCode), startTime(other.startTime), targetServer(other.targetServer),
	  targetLocationConfig(other.targetLocationConfig), targetResource(other.targetResource)
{
}

Connection &Hafserv::Connection::operator=(Connection &rhs)
{
	if (this != &rhs)
	{
		request = rhs.request;
		response = rhs.response;
		socket = rhs.socket;
		port = rhs.port;
		statusCode = rhs.statusCode;
		startTime = rhs.startTime;
		targetServer = rhs.targetServer;
		targetLocationConfig = rhs.targetLocationConfig;
		targetResource = rhs.targetResource;
	}
	return *this;
}

Connection::~Connection() {}

bool Connection::readRequest(int fd)
{
	int str_len;
	char peekBuf[BUFFER_SIZE + 1];
	char readBuf[BUFFER_SIZE + 1];

	bzero(peekBuf, BUFFER_SIZE + 1);
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
		bzero(readBuf, BUFFER_SIZE + 1);
		str_len = read(fd, readBuf, idx + 1);
		readBuf[str_len] = 0;

		statusCode = request.parse(static_cast<std::string>(readBuf));
		if (request.getParseStatus() >= Body && targetServer == NULL)
		{
			targetServer = Webserv::getInstance().findTargetServer(port, request);
			targetResource = configureTargetResource(request.getRequestTarget());
		}
		if (request.getParseStatus() == End)
		{
			request.printRequest();
			// Response response(request);
			// std::string responseString = response.getResponse();
			buildResponseFromRequest();
			std::string responseString = response.getResponse();
			std::cout << "<-------response------->" << std::endl << responseString;
			std::cout << "<-----response end----->" << std::endl;
			// send(fd, responseString.c_str(), responseString.length(), 0);
			write(fd, responseString.c_str(), responseString.length());
			return false;
		}
	}
	return true;
}

std::string Connection::configureTargetResource(std::string requestTarget)
{
	int depth = -1;
	// =
	const std::vector<LocationConfig> &equalLocations = targetServer->getServerConfig().getLocations().at(0);
	std::vector<LocationConfig>::const_iterator selectedIt = equalLocations.end();

	for (std::vector<LocationConfig>::const_iterator it = equalLocations.begin(); it != equalLocations.end(); it++)
	{
		const std::string &pattern = it->getPattern();
		if (requestTarget == pattern)
		{
			return (requestTarget);
		}
	}
	// $
	const std::vector<LocationConfig> &endLocations = targetServer->getServerConfig().getLocations().at(1);
	selectedIt = endLocations.end();

	for (std::vector<LocationConfig>::const_iterator it = endLocations.begin(); it != endLocations.end(); it++)
	{
		const std::string &pattern = it->getPattern();
		if (requestTarget.rfind(pattern) == requestTarget.length() - pattern.length())
		{
			return (requestTarget);
		}
	}

	// ^
	const std::vector<LocationConfig> &locations = targetServer->getServerConfig().getLocations().at(2);
	selectedIt = locations.end();

	for (std::vector<LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); it++)
	{
		// deep route first
		const std::string &pattern = it->getPattern();
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

	std::string tempTargetResource;
	// root / is always presents in httpConfigCore
	if (selectedIt != locations.end())
	{
		targetLocationConfig = *selectedIt;

		const std::string &selectedPattern = selectedIt->getPattern();
		const std::string &selectedAlias = selectedIt->getAlias();
		if (!selectedAlias.empty())
		{
			if (selectedPattern.back() == '/')
				tempTargetResource = selectedAlias + requestTarget.substr(selectedPattern.length() - 1);
			else
				tempTargetResource = selectedAlias + requestTarget.substr(selectedPattern.length());
		}
		else
			tempTargetResource = selectedIt->getHttpConfigCore().getRoot() + requestTarget;
		if (tempTargetResource.back() == '/')
		{
			std::vector<std::string> indexes = selectedIt->getHttpConfigCore().getIndexes();
			std::string defaultTargetResource;
			if (indexes.size() == 0)
				defaultTargetResource = tempTargetResource + "index.html";
			else
				defaultTargetResource = tempTargetResource + indexes[0];
			for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
			{
				File index(tempTargetResource + *it);
				if (index.getCode() == File::REGULAR_FILE)
				{
					return tempTargetResource + *it;
				}
			}
			tempTargetResource = defaultTargetResource;
		}
	}
	return tempTargetResource;
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
			build301Response("http://" + request.getHeaders().find("host")->second + request.getRequestTarget() + "/");
		else if (targetFile.getCode() == File::REGULAR_FILE || targetLocationConfig.getProxyPass().length() != 0)
			buildGetResponse();
		else if (targetFile.getCode() != File::REGULAR_FILE)
			buildErrorResponse(404);
	}
	else if (method == "HEAD")
		buildErrorResponse(405);
	else if (method == "POST")
	{
		if (request.getRequestTarget() == "/")
			buildErrorResponse(405);
		else
			callCGI(targetResource);
	}
	else if (method == "DELETE")
	{
		if (targetFile.getCode() != File::REGULAR_FILE)
			buildErrorResponse(404);
		else
			buildDeleteResponse();
	}
}

void Connection::buildDeleteResponse()
{
	response.setStatusLine(std::string("HTTP/1.1 200 OK"));
	response.makeBody(targetLocationConfig, targetResource);
	std::remove(targetResource.c_str());
}

void Connection::buildGetResponse()
{
	if (request.getHeaders().find("host") == request.getHeaders().end())
		buildErrorResponse(400);
	else if (targetLocationConfig.getProxyPass().length() != 0)
		build301Response(targetLocationConfig.getProxyPass());
	else
	{
		response.setStatusLine(std::string("HTTP/1.1 200 OK"));
		response.makeBody(targetLocationConfig, targetResource);
	}
}

void Connection::buildErrorResponse(int statusCode)
{
	std::string startLine = "HTTP/1.1 " + util::string::itos(statusCode) + " " +
							Webserv::getInstance().getStatusCodeMap().find(statusCode)->second;
	response.setStatusLine(startLine);
	std::map<int, std::string> errorPages = targetLocationConfig.getHttpConfigCore().getErrorPages();
	std::map<int, std::string>::iterator targetIt = errorPages.find(statusCode);
	std::string targetResource;
	if (targetIt == errorPages.end())
		targetResource = "error/" + util::string::itos(statusCode) + ".html";
	else
		targetResource = configureTargetResource(targetIt->second);
}

void Connection::build301Response(const std::string &redirectTarget)
{
	std::string startLine = "HTTP/1.1 301 Moved Permanently";
	response.setStatusLine(startLine);
	response.addToHeaders("Location", redirectTarget);
	std::map<int, std::string> errorPages = targetLocationConfig.getHttpConfigCore().getErrorPages();
	std::map<int, std::string>::iterator targetIt = errorPages.find(301);
	std::string targetResource;
	if (targetIt == errorPages.end())
		targetResource = "error/301.html";
	else
		targetResource = configureTargetResource(targetIt->second);
}

void Hafserv::Connection::callCGI(const std::string &scriptPath)
{
	std::string body;
	/* 예시
	// std::string home_path = getenv("HOME");
	// std::string scriptPath = home_path + "/cgi-bin/my_cgi.py";
	// std::string queryString = "first=1&second=2";
	*/
	char *argv[] = {(char *)scriptPath.c_str(), NULL};
	char **envp = makeEnvp();

	int outward_fd[2];
	int inward_fd[2];

	pipe(outward_fd);
	pipe(inward_fd);
	int pid = fork();
	if (pid == 0)
	{
		close(outward_fd[0]);
		close(inward_fd[1]);
		dup2(outward_fd[1], STDOUT_FILENO);
		dup2(inward_fd[0], STDIN_FILENO);
		close(outward_fd[1]);
		close(inward_fd[0]);

		struct stat fileStat;
		File target(scriptPath);
		if (target.getCode() == File::REGULAR_FILE && stat(scriptPath.c_str(), &fileStat) == 0 &&
			(fileStat.st_mode & S_IXUSR))
		{
			execve(scriptPath.c_str(), argv, envp);
		}
		else
		{
			std::cerr << "No authorization" << std::endl;
			exit(1);
		}
		perror(scriptPath.c_str());
	}
	else
	{
		char buffer[4096];
		ssize_t bytes_read;
		std::ostringstream output;

		close(outward_fd[1]);
		close(inward_fd[0]);
		write(inward_fd[1], request.getBody().c_str(), request.getBody().length());
		int status;
		while (!waitpid(pid, &status, WNOHANG) && (bytes_read = read(outward_fd[0], buffer, sizeof(buffer))) > 0)
			output.write(buffer, bytes_read);
		close(outward_fd[0]);
		close(inward_fd[1]);
		body = output.str();
	}
}

char **Hafserv::Connection::makeEnvp()
{
	// https://datatracker.ietf.org/doc/html/rfc3875#section-4.1
	std::vector<std::string> envVec;
	envVec.push_back("SERVER_PROTOCOL=HTTP/1.1");
	std::string requestMethod("REQUEST_METHOD=");
	requestMethod += request.getMethod();
	envVec.push_back(requestMethod);
	if (request.getHeaders().find("content-type") != request.getHeaders().end())
	{
		std::string contentType("CONTENT_TYPE=");
		contentType += request.getHeaders().find("content-type")->second;
		envVec.push_back(contentType);
	};
	std::stringstream ss;
	ss << request.getBody().length();
	std::string contentLength("CONTENT_LENGTH=");
	contentLength += ss.str();
	envVec.push_back(contentLength);
	char **envp = new char *[envVec.size() + 1];
	for (size_t i = 0; i < envVec.size(); i++)
	{
		const char *envString = envVec[i].c_str();
		char *env = new char[envVec[i].length() + 1];
		std::strcpy(env, envString);
		envp[i] = env;
	}
	envp[envVec.size()] = NULL;
	return envp;
}

const Request &Connection::getRequest() const { return request; }

const Server *Connection::getTargetServer() const { return targetServer; }

const time_t &Connection::getStartTime() const { return startTime; }

const LocationConfig &Connection::getTargetLocationConfig() const { return targetLocationConfig; }
