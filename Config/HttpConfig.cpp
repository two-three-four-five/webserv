/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:46:15 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 22:31:08 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConfig.hpp"

using namespace Hafserv;

HttpConfig::HttpConfig() : directives(), servers() {}

HttpConfig::HttpConfig(const HttpConfig &other) : directives(other.directives), servers(other.servers) {}

HttpConfig::HttpConfig(const ConfigFile &block, const HttpConfigCore &core)
	: AHttpConfigModule(core), directives(block.directives), servers()
{
	for (size_t i = 0; i < block.subBlocks.size(); i++)
	{
		ConfigFile subBlock = block.subBlocks.at(i);
		if (subBlock.name == "server")
			servers.push_back(ServerConfig(subBlock, core));
	}
}

HttpConfig &HttpConfig::operator=(const HttpConfig &other)
{
	if (this != &other)
	{
		directives = other.directives;
		servers = other.servers;
	}
	return *this;
}

HttpConfig::~HttpConfig() {}

const ConfigFile::directives_t &HttpConfig::getDirectives() const { return directives; }

const std::vector<ServerConfig> &HttpConfig::getServers() const { return servers; }

std::ostream &operator<<(std::ostream &os, const HttpConfig &conf)
{
	os << "[HttpConfig]" << std::endl;

	os << "\tdirectives" << std::endl;
	ConfigFile::directives_t::const_iterator it = conf.getDirectives().begin();
	for (; it != conf.getDirectives().end(); it++)
		os << "\t\t" << (*it).first << ": " << (*it).second << std::endl;

	os << "\tservers:";
	for (size_t i = 0; i < conf.getServers().size(); i++)
		os << conf.getServers().at(i) << std::endl;
	return os;
}