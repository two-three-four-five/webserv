#include "Connection.hpp"

#include <string>
#include <sys/socket.h>

using namespace Hafserv;

Connection::Connection(int socket, unsigned short port) : socket(socket), port(port) {}

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

		request.parse(static_cast<std::string>(readBuf));
		if (request.getParseStatus() >= Body && request.getTargetServer() == NULL)
		{
			request.setTargetServer(Webserv::findTargetServer(port, request));
			std::cout << request.getTargetServer() << std::endl;
			request.setTargetLocation();
		}
		if (request.getParseStatus() == End)
		{
			request.printRequest();
			Hafserv::Response response(request);
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
