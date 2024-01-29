#include "Response.hpp"
#include "File.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace Hafserv
{

Response::Response(Request &request) : request(request)
{
	request.printRequest();

	fields.push_back("Server: Hafserv/1.0.0");

	if (request.statusCode != 200)
	{
		buildErrorResponse(request.statusCode);
	}
	else
	{
		statusLine = "HTTP/1.1 200 OK";
		buildResponseFromRequest();
	}
}

Response::~Response() {}

void Response::buildResponseFromRequest()
{
	std::string targetLocation = getTargetLocation();
	std::cout << std::endl << "TARGET\n" << targetLocation << std::endl;
	File targetFile(targetLocation);

	if (request.method == "GET" && targetFile.getCode() == File::DIRECTORY)
		build301Response(targetLocation);
	else if (request.method == "GET" && targetFile.getCode() == File::REGULAR_FILE)
		buildGetResponse(targetLocation);
	else if (request.method == "GET" && targetFile.getCode() != File::REGULAR_FILE)
		buildErrorResponse(404);
	else if (request.method == "POST")
		build405Response();
	// callCGI("./cgi-bin" + request.requestTarget);
}

std::string Response::getTargetLocation()
{
	int depth = -1;
	const std::vector<LocationConfig> &locations = request.getTargetServer()->getServerConfig().getLocations();
	std::vector<LocationConfig>::const_iterator selectedIt = locations.end();

	for (std::vector<LocationConfig>::const_iterator it = locations.begin(); it != locations.end(); it++)
	{
		// deep route first
		const std::string &pattern = (*it).getPattern();
		if (request.requestTarget.find(pattern) == 0)
		{
			int currDepth = pattern == "/" ? 0 : std::count(pattern.begin(), pattern.end(), '/');
			if (depth < currDepth)
			{
				depth = currDepth;
				selectedIt = it;
			}
		}
	}
	std::cout << selectedIt->getPattern() << std::endl;

	std::string targetLocation;
	// root / is always presents in httpConfigCore
	if (selectedIt != locations.end())
	{
		targetLocation = (*selectedIt).getHttpConfigCore().getRoot() + request.requestTarget;
		if (targetLocation.back() == '/')
		{
			std::vector<std::string> indexes = (*selectedIt).getHttpConfigCore().getIndexes();
			for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
			{
				File index(targetLocation + *it);
				if (index.getCode() == File::REGULAR_FILE)
					return targetLocation + *it;
			}
		}
	}
	return targetLocation;
}

void Response::buildGetResponse(std::string targetLocation) { makeBody(targetLocation); }

void Response::build301Response(std::string redirectTarget)
{
	statusLine = "HTTP/1.1 301 Moved Permanently";
	fields.push_back("Location: http://" + request.fields.find("host")->second + request.requestTarget + "/");
	makeBody("error/301.html");
}

void Response::build405Response()
{
	statusLine = "HTTP/1.1 405 Not Allowed";
	makeBody("error/405.html");
}

void Response::buildErrorResponse(int statusCode)
{
	std::ostringstream oss;
	oss << "HTTP/1.1 " << statusCode;
	statusLine = oss.str();
	makeBody(request.getTargetServer()->getServerConfig().getHttpConfigCore().getErrorPages().find(statusCode)->second);
}

void Response::makeBody(const std::string &targetLocation)
{
	const std::multimap<std::string, std::string> &typeMap =
		request.getTargetServer()->getServerConfig().getHttpConfigCore().getTypes();
	std::multimap<std::string, std::string>::const_iterator typeIt =
		typeMap.find(targetLocation.substr(targetLocation.rfind('.') + 1));
	if (typeIt != typeMap.end())
		fields.push_back("Content-Type: " + typeIt->second);
	else
		fields.push_back("Content-Type: application/octet-stream");

	std::ostringstream oss;
	std::ifstream file(targetLocation);

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			oss << line << std::endl;
		}
		file.close();
	}
	body = oss.str();

	std::ostringstream contentLengthOss;
	contentLengthOss << "Content-Length: " << body.length();
	fields.push_back(contentLengthOss.str());
}

void Response::callCGI(const std::string &scriptPath)
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
	int pid = fork();
	if (pid == 0)
	{
		close(outward_fd[0]);
		close(inward_fd[1]);
		dup2(outward_fd[1], STDOUT_FILENO);
		dup2(inward_fd[0], STDIN_FILENO);
		close(outward_fd[1]);
		close(inward_fd[0]);
		execve(scriptPath.c_str(), argv, envp);
		perror(scriptPath.c_str());
	}
	else
	{
		char buffer[4096];
		ssize_t bytes_read;
		std::ostringstream output;

		close(outward_fd[1]);
		close(inward_fd[0]);
		write(inward_fd[1], request.body[0].c_str(), request.body[0].length());
		int status;
		while (!waitpid(pid, &status, WNOHANG) && (bytes_read = read(outward_fd[0], buffer, sizeof(buffer))) > 0)
			output.write(buffer, bytes_read);
		close(outward_fd[0]);
		close(inward_fd[1]);
		body = output.str();
	}
}

char **Response::makeEnvp()
{
	// https://datatracker.ietf.org/doc/html/rfc3875#section-4.1
	std::vector<std::string> envVec;
	std::string requestMethod("REQUEST_METHOD=");
	requestMethod += request.method;
	envVec.push_back(requestMethod);
	if (request.fields.find("content-type") != request.fields.end())
	{
		std::string contentType("CONTENT_TYPE=");
		contentType += request.fields.find("content-type")->second;
		envVec.push_back(contentType);
	};
	std::stringstream ss;
	ss << request.body[0].length();
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

std::string Response::getResponse()
{
	std::string response;

	response = statusLine + "\r\n";
	for (std::vector<std::string>::iterator it = fields.begin(); it != fields.end(); it++)
		response += (*it) + "\r\n";
	response += "\r\n";
	response += body;
	return response;
}

} // namespace Hafserv
