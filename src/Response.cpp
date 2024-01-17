#include "Response.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>

namespace ft
{
Response::Response(const Request &request)
{
	// /* temp headers */
	// headers.push_back("server: NWS");
	// headers.push_back("date: Sat, 06 Jan 2024 11:54:12 GMT");
	// headers.push_back("content-type: text/html");
	// headers.push_back("location: http://www.naver.com/");
	// headers.push_back("vary: Accept-Encoding,User-Agent");
	// headers.push_back("referrer-policy:unsafe-url");
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
		// callCGI();
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
	if (request.fields.find("Content-Type") == request.fields.end())
		oss << "Content-Type: text/html\n";
	else
		oss << "Content-Type: " << request.fields.at("Content-Type")[0] << "\n";
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
	char *argv[] = {(char *)scriptPath.c_str(), nullptr};
	char *envp[] = {nullptr};

	// char *argv[] = makeArgv();
	// char *envp[] = makeEnvp();
	int fd[2];

	pipe(fd);
	if (fork() == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(scriptPath.c_str(), argv, envp);
		perror("execve");
	}
	else
	{
		char buffer[4096];
		ssize_t bytes_read;
		std::ostringstream output;

		close(fd[1]);
		while ((bytes_read = read(fd[0], buffer, sizeof(buffer))) > 0)
			output.write(buffer, bytes_read);
		close(fd[0]);
		return (output.str());
	}
	return (NULL);
}

std::string Response::setBoundary(std::map<std::string, std::vector<std::string> > &message)
{
	std::string contentType = message["Content-Type"].front();
	std::string str;
	std::istringstream iss(contentType);
	// Content-Type: multipart/form-data; boundary=----WebKitFormBoundarymPT9dBQpTyN8gwce.

	std::getline(iss >> std::ws, str, ';');
	if (iss.eof())
		boundary = "\r\n";
	else
	{
		std::getline(iss >> std::ws, str, ' ');
	}
}

std::string &Response::getResponse() { return (response); }

} // namespace ft
