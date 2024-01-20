#include "Response.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>

namespace Hafserv
{
Response::Response(Request &request) : request(request)
{
	// /* temp headers */
	// headers.push_back("server: NWS");
	// headers.push_back("date: Sat, 06 Jan 2024 11:54:12 GMT");
	// headers.push_back("content-type: text/html");
	// headers.push_back("location: http://www.naver.com/");
	// headers.push_back("vary: Accept-Encoding,User-Agent");
	// headers.push_back("referrer-policy:unsafe-url");

	request.printRequest();

	if (request.statusCode != 200)
	{
		// generate error response
	}
	if (request.method == "GET")
	{
		response = makeGetResponse(request);
	}
	else if (request.method == "POST")
	{
		response = callCGI("./cgi-bin" + request.requestTarget);
	}
}

Response::~Response() {}

std::string Response::makeGetResponse(const Request &request)
{
	/*
	Server: nginx/1.25.3
	Date: Sat, 13 Jan 2024 07:13:54 GMT
	Content-Type: text/html
	Content-Length: 615
	Last-Modified: Tue, 24 Oct 2023 13:46:52 GMT
	Connection: keep-alive
	ETag: "6537cacc-267"
	Accept-Ranges: bytes
	*/
	std::ostringstream oss;
	std::string body = makeBody(request.requestTarget);

	oss << "HTTP/1.1 200 OK\n";
	if (request.fields.find("content-type") == request.fields.end())
		oss << "Content-Type: text/html\n";
	else
		oss << "Content-Type: " << request.fields.at("content-type")[0] << "\n";
	oss << "Content-Length: " << body.length() << "\n\n";
	oss << body;

	return (oss.str());
}

std::string Response::makeBody(const std::string &requestTarget)
{
	std::ostringstream oss;
	std::ifstream file("www" + requestTarget);

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			oss << line << std::endl;
		}
		file.close();
	}
	return (oss.str());
}

std::string Response::callCGI(const std::string &scriptPath)
{
	/* 예시
	// std::string home_path = getenv("HOME");
	// std::string scriptPath = home_path + "/cgi-bin/my_cgi.py";
	// std::string queryString = "first=1&second=2";
	*/
	char *argv[] = {(char *)scriptPath.c_str(), NULL};
	// char *envp[] = {nullptr};

	// char *argv[] = makeArgv();
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

		output << "HTTP/1.1 200 OK\n";
		close(outward_fd[1]);
		close(inward_fd[0]);
		write(inward_fd[1], request.body[0].c_str(), request.body[0].length());
		int status;
		while (!waitpid(pid, &status, WNOHANG) && (bytes_read = read(outward_fd[0], buffer, sizeof(buffer))) > 0)
			output.write(buffer, bytes_read);
		close(outward_fd[0]);
		close(inward_fd[1]);
		return (output.str());
	}
	return (NULL);
}

// std::string Response::setBoundary(std::map<std::string, std::vector<std::string> > &message)
// {
// 	std::string contentType = message["Content-Type"].front();
// 	std::string str;
// 	std::istringstream iss(contentType);
// 	// Content-Type: multipart/form-data; boundary=----WebKitFormBoundarymPT9dBQpTyN8gwce.

// 	std::getline(iss >> std::ws, str, ';');
// 	if (iss.eof())
// 		boundary = "\r\n";
// 	else
// 	{
// 		std::getline(iss >> std::ws, str, ' ');
// 	}
// }

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
		contentType += (*request.fields.find("content-type")).second;
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

std::string &Response::getResponse() { return (response); }

} // namespace Hafserv
