/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:46:00 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/11 21:58:10 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/ServerConfig.hpp"

using namespace Hafserv;

ServerConfig::ServerConfig() : AConfig(), AHttpConfigModule(), names(), ports(), locations(3) {}

ServerConfig::ServerConfig(const ServerConfig &other)
	: AConfig(other), AHttpConfigModule(other.core), names(other.names), ports(other.ports), locations(other.locations)
{
}

ServerConfig::ServerConfig(const ConfigFile &block, const HttpConfigCore &core)
	: AConfig(), AHttpConfigModule(core), names(), ports(), locations(3)
{
	this->setHttpConfigCore(block.getDirectives());
	this->setHttpConfigCore(block.getSubBlocks());

	ConfigFile::directives_t::const_iterator it = block.getDirectives().begin();
	for (; it != block.getDirectives().end(); it++)
	{
		const std::string &key = (*it).first;
		const std::string &value = (*it).second;

		if (allBlockDirectives.count(key))
			throw NoBraceError(key);
		else if (!allSimpleDirectives.count(key))
			throw UnknownDirectiveError(key);
		else if (key == "listen")
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
		else if (!isCoreDirective(key))
			throw DisallowDirectiveError(key);
		else
			;
	}
	for (size_t i = 0; i < block.getSubBlocks().size(); i++)
	{
		const ConfigFile &subBlock = block.getSubBlocks().at(i);
		const std::string &subBlockName = subBlock.getBlockDirective();

		if (allSimpleDirectives.count(subBlockName))
			throw NoSemicolonError(subBlockName);
		else if (!allBlockDirectives.count(subBlockName))
			throw UnknownDirectiveError(subBlockName);
		else if (block.getSubBlocks().at(i).getBlockDirective() == "location")
		{
			LocationConfig conf = LocationConfig(block.getSubBlocks().at(i), core);
			if (conf.getModifier() == "=")
				locations[0].push_back(conf);
			else if (conf.getModifier() == "$")
				locations[1].push_back(conf);
			else if (conf.getModifier() == "^")
				locations[2].push_back(conf);
		}
		else
			throw DisallowDirectiveError(subBlockName);
	}
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
		AConfig::operator=(other);
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
