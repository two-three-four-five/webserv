/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfig.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:46:15 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 16:12:52 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConfig.hpp"

using namespace Hafserv;

HttpConfig::HttpConfig() : directives(), servers() {}

HttpConfig::HttpConfig(const HttpConfig &other) : directives(other.directives), servers(other.servers) {}

HttpConfig::HttpConfig(const ConfigFile &block) : directives(block.directives), servers()
{
	for (size_t i = 0; i < block.subBlocks.size(); i++)
	{
		ConfigFile subBlock = block.subBlocks.at(i);
		if (subBlock.name == "server")
			servers.push_back(ServerConfig(subBlock));
	}
}

HttpConfig &HttpConfig::operator=(const HttpConfig &other)
{
	if (this != &other)
	{
	}
	return *this;
}

HttpConfig::~HttpConfig() {}

const ConfigFile::directives_t &HttpConfig::getDirectives() const { return directives; }

const std::vector<ServerConfig> &HttpConfig::getServers() const { return servers; }
