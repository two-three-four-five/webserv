#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

class Server
{
public:
	Server();
	~Server();

	void startServer();
private:
	int port;
	std::string serverName;
};



#endif
