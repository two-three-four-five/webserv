/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:46:15 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/11 22:45:32 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/HttpConfig.hpp"

using namespace Hafserv;

HttpConfig::HttpConfig() : AConfig(), AHttpConfigModule(), directives(), servers() {}

HttpConfig::HttpConfig(const HttpConfig &other)
	: AConfig(other), AHttpConfigModule(other.core), directives(other.directives), servers(other.servers)
{
}

HttpConfig::HttpConfig(const ConfigFile &block) throw(ParseError)
	: AConfig(), AHttpConfigModule(), directives(block.getDirectives()), servers()
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
		else if (!httpSimpleDirectives.count(key))
			throw DisallowDirectiveError(key);

		if (key == "default_type")
			;
		else if (key == "sendfile")
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
		else if (!httpBlockDirectives.count(subBlockName))
			throw DisallowDirectiveError(subBlockName);

		if (subBlockName == "server")
			servers.push_back(ServerConfig(block.getSubBlocks().at(i), core));
		else if (subBlockName == "types")
			;
	}
}

HttpConfig &HttpConfig::operator=(const HttpConfig &other)
{
	if (this != &other)
	{
		AConfig::operator=(other);
		core = other.core;
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

	os << conf.getHttpConfigCore();

	ConfigFile::directives_t::const_iterator it = conf.getDirectives().begin();
	for (; it != conf.getDirectives().end(); it++)
		os << "\t[" << (*it).first << "] : [" << (*it).second << "]" << std::endl;

	for (size_t i = 0; i < conf.getServers().size(); i++)
		os << conf.getServers().at(i) << std::endl;
	return os;
}
