#include "Http/Connection.hpp"

#include "File/Directory.hpp"
#include "File/File.hpp"
#include <string>
#include <sys/event.h>
#include <sys/socket.h>

using namespace Hafserv;

Connection::Connection(int socket, unsigned short port) : socket(socket), port(port), targetServer(NULL), statusCode(0)
{
	startTime = time(NULL);
}

Connection::Connection(const Connection &other)
	: request(other.request), response(other.response), socket(other.socket), port(other.port),
	  statusCode(other.statusCode), startTime(other.startTime), targetServer(other.targetServer),
	  targetLocationConfig(other.targetLocationConfig), targetResource(other.targetResource)
{
}

Connection &Connection::operator=(Connection &rhs)
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
	// std::cout << "reading" << std::endl;
	statusCode = request.readRequest(fd);
	// std::cout << "parseStatus : " << request.getParseStatus() << " statusCode : " << statusCode
	// 		  << " bodyLength : " << request.getBodyLength() << std::endl;
	if (request.getParseStatus() == End)
	{
		// request.printRequest();
		targetServer = Webserv::getInstance().findTargetServer(port, request);
		targetResource = configureTargetResource(request.getRequestTarget().getTargetURI());
		if (targetLocationConfig.getClientMaxBodySize() < getRequest().getBodyLength())
		{
			statusCode = 413;
		}
		buildResponseFromRequest();
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
			targetLocationConfig = *it;
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
			targetLocationConfig = *it;
			return ("cgi-bin" + requestTarget);
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
			tempTargetResource = selectedIt->getRoot() + requestTarget;
		if (tempTargetResource.back() == '/')
		{
			if (selectedIt->getAutoIndex())
			{
				return tempTargetResource;
			}
			else
			{
				std::vector<std::string> indexes = selectedIt->getIndexes();
				std::string defaultTargetResource;
				if (indexes.size() == 0)
					defaultTargetResource = tempTargetResource + "index.html";
				else
					defaultTargetResource = tempTargetResource + indexes[0];
				for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
				{
					std::cout << "d " << tempTargetResource + *it << std::endl;
					if (RegularFile(tempTargetResource + *it).valid())
					{
						return tempTargetResource + *it;
					}
				}
				tempTargetResource = defaultTargetResource;
			}
		}
	}
	return tempTargetResource;
}

void Connection::buildResponseFromRequest()
{
	// std::cout << "statusCode : " << statusCode << std::endl;
	if (statusCode)
	{
		buildErrorResponse(statusCode);
		response.setResponseBuffer();
	}
	else
	{
		std::cout << std::endl << "TARGET\n" << targetResource << std::endl;
		std::string method = request.getMethod();

		response.setStatusLine("HTTP/1.1 200 OK");
		response.addToHeaders("Server", "Hafserv/1.0.0");

		if (!targetLocationConfig.isAllowedMethod(method))
		{
			buildErrorResponse(405);
			response.setResponseBuffer();
			return;
		}
		if (method == "GET")
		{
			File targetFile(targetResource);
			if (targetFile.isDirectory() && targetLocationConfig.getAutoIndex() && targetResource.back() == '/')
				buildDirectoryResponse();
			else if (targetFile.isDirectory())
				build301Response("http://" + request.getHeaders().find("host")->second +
								 request.getRequestTarget().getTargetURI() + "/");
			else if (targetFile.isRegularFile() || targetLocationConfig.getProxyPass().length() != 0)
				buildGetResponse();
			else if (!targetFile.isRegularFile())
				buildErrorResponse(404);
			response.setResponseBuffer();
		}
		else if (method == "POST")
		{
			if (targetLocationConfig.getCgiPath().size())
			{
				std::cout << "----------------------------" << std::endl;
				buildCGIResponse(targetLocationConfig.getCgiPath());
				return;
			}
			else if (request.getRequestTarget().getTargetURI() == "/")
				buildErrorResponse(405);
			else
			{
				File targetFile(targetResource);

				if (!(targetFile.isRegularFile() && targetFile.isExecutable()))
				{
					statusCode = 200;
					buildErrorResponse(statusCode);
				}
				else
					buildCGIResponse(targetResource);
			}
			response.setResponseBuffer();
		}
		else if (method == "DELETE")
		{
			File targetFile(targetResource);
			if (!targetFile.isRegularFile())
				buildErrorResponse(404);
			else
				buildDeleteResponse();
			response.setResponseBuffer();
		}
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

void Hafserv::Connection::buildDirectoryResponse()
{
	Directory dir(targetResource);
	response.setBody(dir.toHtml());
	response.addToHeaders("Content-Type", "text/html; charset=utf-8");
}

void Connection::buildErrorResponse(int statusCode)
{
	std::string startLine = "HTTP/1.1 " + util::string::itos(statusCode) + " " +
							Webserv::getInstance().getStatusCodeMap().find(statusCode)->second;
	response.setStatusLine(startLine);
	// 제일 잘맞는 LocationConfig == targetLocationConfig
	std::map<int, std::string> errorPages = targetLocationConfig.getErrorPages();
	// found error page
	std::map<int, std::string>::iterator targetIt = errorPages.find(statusCode);
	// targetResource 실제 파일
	std::string targetResource;
	if (targetIt == errorPages.end())
		targetResource = "error/" + util::string::itos(statusCode) + ".html";
	else
		targetResource = configureTargetResource(targetIt->second);
	response.makeBody(targetLocationConfig, targetResource);
	if (statusCode == 413)
		response.removeHeaders("Content-Length");
}

void Connection::build301Response(const std::string &redirectTarget)
{
	std::string startLine = "HTTP/1.1 301 Moved Permanently";
	response.setStatusLine(startLine);
	response.addToHeaders("Location", redirectTarget);
	std::map<int, std::string> errorPages = targetLocationConfig.getErrorPages();
	std::map<int, std::string>::iterator targetIt = errorPages.find(301);
	std::string targetResource;
	if (targetIt == errorPages.end())
		targetResource = "error/301.html";
	else
		targetResource = configureTargetResource(targetIt->second);
	response.makeBody(targetLocationConfig, targetResource);
}

void Connection::buildCGIResponse(const std::string &scriptPath)
{
	/* 예시
	// std::string home_path = getenv("HOME");
	// std::string scriptPath = home_path + "/cgi-bin/my_cgi.py";
	// std::string queryString = "first=1&second=2";
	*/
	char *argv[] = {(char *)scriptPath.c_str(), NULL};
	char **envp = makeEnvp();

	File target(scriptPath);

	if (!(target.isRegularFile() && target.isExecutable()))
	{
		statusCode = 200;
		buildErrorResponse(statusCode);
		return;
	}
	int outward_fd[2];
	int inward_fd[2];

	pipe(outward_fd);
	pipe(inward_fd);

	fcntl(outward_fd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(outward_fd[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(inward_fd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(inward_fd[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	cgiPID = fork();
	if (cgiPID == 0)
	{
		close(outward_fd[0]);
		close(inward_fd[1]);
		dup2(outward_fd[1], STDOUT_FILENO);
		dup2(inward_fd[0], STDIN_FILENO);

		if (target.isRegularFile() && target.isExecutable())
		{
			execve(argv[0], argv, envp);
		}
		else
		{
			std::cerr << scriptPath << std::endl;
			std::cerr << "No authorization" << std::endl;
			exit(1);
		}
		perror(scriptPath.c_str());
		exit(1);
	}
	else
	{
		close(outward_fd[1]);
		close(inward_fd[0]);

		wrBytes = request.getBody().length();
		written = 0;
		response.setResponseState(Response::BuildingCGI);
		readen = 0;

		readPipe = outward_fd[0];
		writePipe = inward_fd[1];
		Webserv::getInstance().addCGIEvent(socket, outward_fd[0], inward_fd[1]);
	}
}

void Connection::writeToCGI(int fd)
{
	const char *wrBuffer = request.getBody().c_str();
	wrBytes = request.getBody().length();
	int bytesToWrite = wrBytes - written;
	if (bytesToWrite > BUFFER_SIZE)
		bytesToWrite = BUFFER_SIZE;
	if (!bytesToWrite)
		return;
	int ret = write(fd, wrBuffer + written, bytesToWrite);
	if (ret > 0)
	{
		// std::cout << "written: " << written << ", " << ret << std::endl;
		written += ret;
	}
}

void Connection::readFromCGI(int fd)
{
	ssize_t bytes_read;
	char buffer[BUFFER_SIZE + 1];

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read > 0)
	{
		CGIOutput.write(buffer, bytes_read);
		readen += bytes_read;
		// std::cout << "read: " << readen << ", " << bytes_read << std::endl;
	}
	int status;
	if (waitpid(cgiPID, &status, WNOHANG) > 0)
	{
		std::cout << "pipeEND" << std::endl;
		Webserv::getInstance().deleteCGIEvent(readPipe, writePipe);
		// response.setBody(CGIOutput.str());

		std::string returned = CGIOutput.str();
		std::string header;
		while (true)
		{
			header = returned.substr(0, returned.find('\r'));
			returned = returned.substr(returned.find('\n') + 1);

			if (!header.size())
				break;
			std::string key = header.substr(0, header.find(':'));
			std::string value = header.substr(header.find(':') + 2);
			if (key == "Status")
				response.setStatusLine("HTTP/1.1 " + value);
			else
				response.addToHeaders(key, value);
		}
		response.setBody(returned);
		response.setResponseBuffer();
	}
}

char **Connection::makeEnvp()
{
	// https://datatracker.ietf.org/doc/html/rfc3875#section-4.1
	std::vector<std::string> envVec;
	envVec.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envVec.push_back("PATH_INFO=/");
	std::string requestMethod("REQUEST_METHOD=");
	requestMethod += request.getMethod();
	envVec.push_back(requestMethod);

	HeaderMultiMap::const_iterator it = request.getHeaders().begin();
	for (; it != request.getHeaders().end(); it++)
	{
		std::string key = (*it).first;
		std::string value = (*it).second;
		for (size_t i = 0; i < key.size(); i++)
		{
			key[i] = toupper(key[i]);
			if (key[i] == '-')
				key[i] = '_';
		}
		// std::cout << "envkey=" << key << std::endl;
		// std::cout << "envvalue=" << value << std::endl;
		if (key.find("X_") == 0)
			key = "HTTP_" + key;
		envVec.push_back(key + "=" + value);
	}

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

void Connection::sendResponse() { response.send(socket); }

void Connection::reset()
{
	startTime = time(NULL);
	statusCode = 0;
	targetServer = NULL;
	targetResource = "";
	targetLocationConfig = LocationConfig();
	request = Request();
	response = Response();
	CGIOutput.str("");
}

const int &Connection::getStatusCode() const { return statusCode; }

const Request &Connection::getRequest() const { return request; }

const Response &Connection::getResponse() const { return response; }

const Server *Connection::getTargetServer() const { return targetServer; }

const time_t &Connection::getStartTime() const { return startTime; }

const LocationConfig &Connection::getTargetLocationConfig() const { return targetLocationConfig; }
