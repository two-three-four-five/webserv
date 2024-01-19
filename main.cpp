#include <iostream>

#include "Webserv.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		exit(EXIT_FAILURE);
	}
	Hafserv::Webserv webserv;
	Hafserv::Server *myServer = new Hafserv::Server(atoi(argv[1]));
	// Hafserv::Server *myServer2 = new Hafserv::Server(atoi(argv[1]) + 1);

	webserv.addServer(myServer);
	// webserv.addServer(myServer2);
	webserv.runWebserv();
	return 0;
}
