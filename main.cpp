#include <iostream>
#include "Request.hpp"
#include "webserv/Server.hpp"

int main(int argc, char *argv[])
{
    std::string requestStr = "Host: www.test101.com\r\nAccept: image/gif, image/jpeg, */*\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nUser-Agent: Mozilla/4.0\nContent-Length: 35";
    ft::Request request;

	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		exit(EXIT_FAILURE);
	}
	Server *myServer = new Server(atoi(argv[1]));
	myServer->startServer();
	return 0;

    request.parseFields(requestStr);
    request.printFields();
    return 0;
}
