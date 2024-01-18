/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigTest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:01:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/18 22:45:42 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include "ConfigFile.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: test filename" << std::endl;
		return 1;
	}
	Hafserv::ConfigFile configFile = Hafserv::ConfigFile(argv[1]);

	Hafserv::ServerConfig sconf0 = Hafserv::ServerConfig(configFile.subBlocks.at(0).subBlocks.at(0));
	Hafserv::ServerConfig sconf1 = Hafserv::ServerConfig(configFile.subBlocks.at(0).subBlocks.at(1));

	sconf0.print();
	sconf1.print();

	return 0;
}