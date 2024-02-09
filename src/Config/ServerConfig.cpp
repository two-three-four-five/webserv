/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:46:00 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/08 13:57:13 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

using namespace Hafserv;

ServerConfig::ServerConfig() : AHttpConfigModule(), names(), ports(), locations() {}

ServerConfig::ServerConfig(const ServerConfig &other)
	: AHttpConfigModule(other.core), names(other.names), ports(other.ports), locations(other.locations)
{
}

ServerConfig::ServerConfig(const ConfigFile &block, const HttpConfigCore &core)
	: AHttpConfigModule(core), names(), ports(), locations()
{
	this->setHttpConfigCore(block.getDirectives());
	this->setHttpConfigCore(block.getSubBlocks());

	ConfigFile::directives_t::const_iterator it = block.getDirectives().begin();
	for (; it != block.getDirectives().end(); it++)
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
	for (size_t i = 0; i < block.getSubBlocks().size(); i++)
	{
		if (block.getSubBlocks().at(i).getName() == "location")
		{
			locations.push_back(LocationConfig(block.getSubBlocks().at(i), core));
		}
	}
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
		core = other.core;
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

	os << conf.getHttpConfigCore();

	os << "\tnames: ";
	for (size_t i = 0; i < conf.getNames().size(); i++)
		os << conf.getNames().at(i) << " ";
	os << std::endl;

	os << "\tports: ";
	for (size_t i = 0; i < conf.getPorts().size(); i++)
		os << static_cast<int>(conf.getPorts().at(i)) << " ";
	os << std::endl;

	for (size_t i = 0; i < conf.getLocations().size(); i++)
		os << conf.getLocations().at(i) << std::endl;
	return os;
}
