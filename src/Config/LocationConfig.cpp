/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:08:09 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/04 13:40:19 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

using namespace Hafserv;

LocationConfig::LocationConfig() : AHttpConfigModule(), pattern(), alias(), proxyPass() {}

LocationConfig::LocationConfig(const LocationConfig &other)
	: AHttpConfigModule(other.core), pattern(other.pattern), alias(other.alias), proxyPass(other.proxyPass)
{
}

LocationConfig::LocationConfig(const ConfigFile &block, const HttpConfigCore &core)
	: AHttpConfigModule(core), pattern(), alias(), proxyPass()
{
	pattern = block.parameters.at(0);

	this->setHttpConfigCore(block.directives);
	this->setHttpConfigCore(block.subBlocks);

	ConfigFile::directives_t::const_iterator it = block.directives.begin();
	for (; it != block.directives.end(); it++)
	{
		std::string key = (*it).first;
		std::string value = (*it).second;
		if (key == "alias")
			this->alias = value;
		else if (key == "proxy_pass")
			this->proxyPass = value;
	}
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
	if (this != &other)
	{
		core = other.core;
		pattern = other.pattern;
		alias = other.alias;
		proxyPass = other.proxyPass;
	}
	return *this;
}

LocationConfig::~LocationConfig() {}

const std::string &LocationConfig::getPattern() const { return pattern; }

const std::string &LocationConfig::getAlias() const { return alias; }

const std::string &LocationConfig::getProxyPass() const { return proxyPass; }

void LocationConfig::setPattern(const std::string &pattern) { this->pattern = pattern; }

void LocationConfig::setAlias(const std::string &alias) { this->alias = alias; }

void LocationConfig::setProxyPass(const std::string &proxyPass) { this->proxyPass = proxyPass; }

bool LocationConfig::isMatching(const std::string &url)
{
	if (pattern.find(url) == 0)
		return true;
	else
		return false;
}

std::ostream &operator<<(std::ostream &os, const LocationConfig &conf)
{
	os << "[LocationConfig]" << std::endl;
	os << conf.getHttpConfigCore();
	os << "\tpattern: " << conf.getPattern() << std::endl;
	os << "\talias: " << conf.getAlias() << std::endl;
	os << "\tproxy_pass: " << conf.getProxyPass();

	return os;
}
