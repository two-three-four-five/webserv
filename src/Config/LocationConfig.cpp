/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:08:09 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/05 07:49:30 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

using namespace Hafserv;

LocationConfig::LocationConfig() : AHttpConfigModule(), modifier(), pattern(), alias(), proxyPass() {}

LocationConfig::LocationConfig(const LocationConfig &other)
	: AHttpConfigModule(other.core),modifier(other.modifier), pattern(other.pattern), alias(other.alias), proxyPass(other.proxyPass)
{
}

LocationConfig::LocationConfig(const ConfigFile &block, const HttpConfigCore &core)
	: AHttpConfigModule(core), modifier(), pattern(), alias(), proxyPass()
{
	modifier = block.parameters.at(0);
	if (modifier != "=" && modifier != "$" && modifier != "^")
		throw ParseError("unexpected location parameter: " + modifier);
	pattern = block.parameters.at(1);

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
		modifier = other.modifier;
		pattern = other.pattern;
		alias = other.alias;
		proxyPass = other.proxyPass;
	}
	return *this;
}

LocationConfig::~LocationConfig() {}

const std::string &LocationConfig::getModifier() const { return modifier; }

const std::string &LocationConfig::getPattern() const { return pattern; }

const std::string &LocationConfig::getAlias() const { return alias; }

const std::string &LocationConfig::getProxyPass() const { return proxyPass; }

void LocationConfig::setModifier(const std::string &modifier) { this->modifier = modifier; }

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
	os << "\tmodifier: " << conf.getModifier() << std::endl;
	os << "\tpattern: " << conf.getPattern() << std::endl;
	os << "\talias: " << conf.getAlias() << std::endl;
	os << "\tproxy_pass: " << conf.getProxyPass();

	return os;
}
