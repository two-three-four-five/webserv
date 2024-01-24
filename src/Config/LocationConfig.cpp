/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:08:09 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/23 15:00:58 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

using namespace Hafserv;

LocationConfig::LocationConfig() : AHttpConfigModule(), pattern() {}

LocationConfig::LocationConfig(const LocationConfig &other) : AHttpConfigModule(other.core), pattern(other.pattern) {}

LocationConfig::LocationConfig(const ConfigFile &block, const HttpConfigCore &core) : AHttpConfigModule(core), pattern()
{
	pattern = block.parameters.at(0);

	this->setHttpConfigCore(block.directives);
	this->setHttpConfigCore(block.subBlocks);

	ConfigFile::directives_t::const_iterator it = block.directives.begin();
	for (; it != block.directives.end(); it++)
	{
	}
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
	if (this != &other)
	{
		core = other.core;
		pattern = other.pattern;
	}
	return *this;
}

LocationConfig::~LocationConfig() {}

const std::string &LocationConfig::getPattern() const { return pattern; }

void LocationConfig::setPattern(const std::string &pattern) { this->pattern = pattern; }

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
	os << "\tpattern: " << conf.getPattern();
	return os;
}
