#include <iostream>

#include "Response.hpp"
#include "Server.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		exit(EXIT_FAILURE);
	}
	ft::Server *myServer = new ft::Server(atoi(argv[1]));
	// myServer->startServer();
	ft::Response response(argv[1]);
	std::cout << response.toString();
	return 0;
}
