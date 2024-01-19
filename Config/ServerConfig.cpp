/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:46:00 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 17:13:00 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

using namespace Hafserv;

ServerConfig::ServerConfig() : names(), ports(), locations() {}

ServerConfig::ServerConfig(const ServerConfig &other)
	: names(other.names), ports(other.ports), locations(other.locations)
{
}

ServerConfig::ServerConfig(const ConfigFile &block) : names(), ports(), locations()
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
			locations.push_back(LocationConfig(block.subBlocks.at(i), core));
		}
	}
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
		names = other.names;
		ports = other.ports;
		locations = other.locations;
	}
	return *this;
}

ServerConfig::~ServerConfig() {}

const std::vector<std::string> &ServerConfig::getNames() const { return names; }

const std::vector<unsigned short> &ServerConfig::getPorts() const { return ports; }

const std::vector<LocationConfig> &ServerConfig::getLocations() const { return locations; }

std::ostream &operator<<(std::ostream &os, const ServerConfig &conf)
{
	os << "[ServerConfig]" << std::endl;

	os << "\tnames: ";
	for (size_t i = 0; i < conf.getNames().size(); i++)
		os << conf.getNames().at(i) << " ";
	os << std::endl;

	os << "\tports: ";
	for (size_t i = 0; i < conf.getPorts().size(); i++)
		os << static_cast<int>(conf.getPorts().at(i)) << " ";
	os << std::endl;

	os << "\t locations:" << std::endl;
	for (size_t i = 0; i < conf.getLocations().size(); i++)
		os << conf.getLocations().at(i) << std::endl;
	return os;
}
