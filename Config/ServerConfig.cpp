/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:46:00 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/18 22:44:58 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

using namespace Hafserv;

ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig &other) {}

#include <sstream>

ServerConfig::ServerConfig(const ConfigFile &block)
{
	ConfigFile::directives_t::const_iterator it = block.directives.begin();
	for (; it != block.directives.end(); it++)
	{
		std::string key = (*it).first;
		std::string value = (*it).second;
		if (key == "listen")
		{
			std::stringstream ss(value);
			unsigned short us;
			ss >> us;
			ports.push_back(us);
		}
		else if (key == "server_name")
		{
			names.push_back(value);
		}
	}
	for (size_t i = 0; i < block.subBlocks.size(); i++)
	{
		if (block.subBlocks.at(i).name == "location")
		{
			locations.push_back(LocationConfig(block.subBlocks.at(i)));
		}
	}
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
	}
	return *this;
}

ServerConfig::~ServerConfig() {}

const std::vector<std::string> &ServerConfig::getNames() const { return names; }

const std::vector<unsigned short> &ServerConfig::getPorts() const { return ports; }

const std::vector<LocationConfig> &ServerConfig::getLocations() const { return locations; }

void ServerConfig::print() const
{
	std::cout << "names : ";
	for (size_t i = 0; i < names.size(); i++)
		std::cout << names.at(i) << " ";
	std::cout << std::endl;
	std::cout << "ports : ";
	for (size_t i = 0; i < ports.size(); i++)
		std::cout << static_cast<int>(ports.at(i)) << " ";
	std::cout << std::endl;
	std::cout << "locations : ";
	for (size_t i = 0; i < locations.size(); i++)
		std::cout << locations.at(i).getPattern() << " ";
	std::cout << std::endl;
}
