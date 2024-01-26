#include <iostream>

#include "Webserv.hpp"
#include "WebservConfig.hpp"

using namespace Hafserv;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	try
	{
		ConfigFile configFile = ConfigFile(argv[1]);
		WebservConfig config = WebservConfig(configFile);
		Webserv webserv;

		for (std::vector<ServerConfig>::const_iterator it = config.getHttpConfig().getServers().begin();
			 it != config.getHttpConfig().getServers().end(); it++)
		{
			Server *server = new Server(*it);
			webserv.addServer(server);
		}

		webserv.runWebserv();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
