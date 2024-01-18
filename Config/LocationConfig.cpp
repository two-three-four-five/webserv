/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:08:09 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/18 22:42:31 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

using namespace Hafserv;

LocationConfig::LocationConfig() : pattern(""), root("") {}

LocationConfig::LocationConfig(const LocationConfig &other) : pattern(other.pattern), root(other.root) {}

LocationConfig::LocationConfig(const ConfigFile &block)
{
	pattern = block.parameters.at(0);
	ConfigFile::directives_t::const_iterator it = block.directives.begin();
	for (; it != block.directives.end(); it++)
	{
		if ((*it).first == "root")
			root = (*it).second;
	}
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
	if (this != &other)
	{
		pattern = other.pattern;
		root = other.root;
	}
	return *this;
}

LocationConfig::~LocationConfig() {}

// std::string &LocationConfig::getPattern() { return pattern; }

// std::string &LocationConfig::getRoot() { return root; }

const std::string &LocationConfig::getPattern() const { return pattern; }

const std::string &LocationConfig::getRoot() const { return root; }

void LocationConfig::setPattern(const std::string &pattern) { this->pattern = pattern; }

void LocationConfig::setRoot(const std::string &root) { this->root = root; }

bool LocationConfig::isMatching(const std::string &url)
{
	if (pattern.find(url) == 0)
		return true;
	else
		return false;
}
