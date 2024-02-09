/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:46:00 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/09 15:05:47 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

using namespace Hafserv;

ServerConfig::ServerConfig() : AHttpConfigModule(), names(), ports(), locations(3) {}

ServerConfig::ServerConfig(const ServerConfig &other)
	: AHttpConfigModule(other.core), names(other.names), ports(other.ports), locations(other.locations)
{
}

ServerConfig::ServerConfig(const ConfigFile &block, const HttpConfigCore &core)
	: AHttpConfigModule(core), names(), ports(), locations(3)
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
		// locations.push_back(LocationConfig(block.getSubBlocks().at(i), core));
		if (block.getSubBlocks().at(i).getName() == "location")
		{
			LocationConfig conf = LocationConfig(block.getSubBlocks().at(i), core);
			if (conf.getModifier() == "=")
				locations[0].push_back(conf);
			else if (conf.getModifier() == "$")
				locations[1].push_back(conf);
			else if (conf.getModifier() == "^")
				locations[2].push_back(conf);
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

const std::vector<std::vector<LocationConfig> > &ServerConfig::getLocations() const { return locations; }

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
		for (size_t j = 0; j < conf.getLocations()[i].size(); j++)
			os << conf.getLocations()[i][j] << std::endl;
	return os;
}
