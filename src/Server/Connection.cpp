#include "Http/Connection.hpp"

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
	statusCode = request.readRequest(fd);
	// std::cout << "sc" << request.getParseStatus() << std::endl;
	if (request.getParseStatus() == End)
	{
		request.printRequest();
		std::cout << "parseEnd" << std::endl;
		targetServer = Webserv::getInstance().findTargetServer(port, request);
		targetResource = configureTargetResource(request.getRequestTarget().getTargetURI());
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
	if (statusCode)
	{
		buildErrorResponse(statusCode);
	}
	else
	{
		std::cout << std::endl << "TARGET\n" << targetResource << std::endl;
		std::string method = request.getMethod();

		response.setStatusLine("HTTP/1.1 200 OK");
		response.addToHeaders("Server", "Hafserv/1.0.0");

		if (method == "POST" && targetLocationConfig.getCgiPath().length() > 0)
		{
			buildCGIResponse(targetLocationConfig.getCgiPath());
		}
		else if (method == "GET")
		{
			File targetFile(targetResource);
			if (targetFile.getCode() == File::DIRECTORY)
				build301Response("http://" + request.getHeaders().find("host")->second +
								 request.getRequestTarget().getTargetURI() + "/");
			else if (targetFile.getCode() == File::REGULAR_FILE || targetLocationConfig.getProxyPass().length() != 0)
				buildGetResponse();
			else if (targetFile.getCode() != File::REGULAR_FILE)
				buildErrorResponse(404);
		}
		else if (method == "HEAD")
			buildErrorResponse(405);
		else if (method == "POST")
		{
			if (request.getRequestTarget().getTargetURI() == "/")
				buildErrorResponse(405);
			else
				buildCGIResponse(targetResource);
		}
		else if (method == "DELETE")
		{
			File targetFile(targetResource);
			if (targetFile.getCode() != File::REGULAR_FILE)
				buildErrorResponse(404);
			else
				buildDeleteResponse();
		}
	}
	response.setResponseBuffer();
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

void Connection::buildCGIResponse(const std::string &scriptPath)
{
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

	fcntl(outward_fd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(outward_fd[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(inward_fd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	fcntl(inward_fd[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);

	int pid = fork();
	if (pid == 0)
	{
		close(outward_fd[0]);
		close(inward_fd[1]);
		dup2(outward_fd[1], STDOUT_FILENO);
		dup2(inward_fd[0], STDIN_FILENO);

		struct stat fileStat;
		File target(scriptPath);
		if (target.getCode() == File::REGULAR_FILE && stat(scriptPath.c_str(), &fileStat) == 0 &&
			(fileStat.st_mode & S_IXUSR))
		{
			execve(argv[0], argv, envp);
		}
		else
		{
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
		char buffer[BUFFER_SIZE + 1];
		ssize_t bytes_read;
		std::ostringstream output;

		int kq = kqueue();
		if (kq == -1)
		{
			std::cerr << "kqueue() error" << std::endl;
			exit(1);
		}
		int status;

		struct kevent event;
		EV_SET(&event, outward_fd[0], EVFILT_READ, EV_ADD, 0, 0, NULL);
		kevent(kq, &event, 1, NULL, 0, NULL);
		EV_SET(&event, inward_fd[1], EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		kevent(kq, &event, 1, NULL, 0, NULL);
		struct kevent evList[2];
		struct timespec timeout;
		bzero(&timeout, sizeof(struct timespec));
		bool isWriteEnd = false;
		const char *wrBuffer = request.getBody().c_str();
		size_t wrBytes = request.getBody().length();
		size_t written = 0;
		size_t readen = 0;
		int events;
		while (!waitpid(pid, &status, WNOHANG))
		{
			events = kevent(kq, NULL, 0, evList, 2, &timeout);
			if (events == -1)
			{
				std::cerr << "kevent() error" << std::endl;
				continue;
			}
			for (int i = 0; i < events; i++)
			{
				if (evList[i].filter == EVFILT_WRITE)
				{
					int bytesToWrite = wrBytes - written;
					if (bytesToWrite > BUFFER_SIZE)
						bytesToWrite = BUFFER_SIZE;
					int ret = write(evList[i].ident, wrBuffer + written, bytesToWrite);
					if (ret > 0)
					{
						written += ret;
					}
					else if (ret == 0)
					{
						EV_SET(&event, evList[i].ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
						kevent(kq, &event, 1, NULL, 0, NULL);
					}
				}
				else if (evList[i].filter == EVFILT_READ)
				{
					bytes_read = read(outward_fd[0], buffer, BUFFER_SIZE);
					if (bytes_read == 0)
					{
						EV_SET(&event, evList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
						kevent(kq, &event, 1, NULL, 0, NULL);
					}
					if (bytes_read > 0)
					{
						output.write(buffer, bytes_read);
						readen += bytes_read;
						// std::cout << "read: " << readen << ", " << bytes_read << std::endl;
					}
					if (bytes_read < 0)
					{
						perror("HWY");
					}
				}
			}
		}

		// TODO: waitpid child process
		// int status;
		// waitpid(pid, &status, 0);

		close(inward_fd[1]);
		close(outward_fd[0]);

		std::string returned = output.str();

		std::cout << returned.length() << std::endl;
		returned = returned.substr(returned.find('\n') + 1);
		returned = returned.substr(returned.find('\n') + 1);
		returned = returned.substr(returned.find('\n') + 1);

		response.setStatusLine("HTTP/1.1 200 OK");
		response.addToHeaders("Content-Type", "text/html; charset=utf-8");
		response.addToHeaders("Content-Length", util::string::itos(returned.length()));

		response.setBody(returned);
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
}

const Request &Connection::getRequest() const { return request; }

const Response &Connection::getResponse() const { return response; }

const Server *Connection::getTargetServer() const { return targetServer; }

const time_t &Connection::getStartTime() const { return startTime; }

const LocationConfig &Connection::getTargetLocationConfig() const { return targetLocationConfig; }
