/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:46:15 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/22 18:38:10 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebservConfig.hpp"

using namespace Hafserv;

WebservConfig::WebservConfig() : directives(), event(), http() {}

WebservConfig::WebservConfig(const WebservConfig &other)
	: directives(other.directives), event(other.event), http(other.http)
{
}

WebservConfig::WebservConfig(const ConfigFile &configFile) : directives(), event(), http()
{
	ConfigFile main = configFile.subBlocks.at(0);
	directives = main.directives;
	for (size_t i = 0; i < main.subBlocks.size(); i++)
	{
		ConfigFile subBlock = main.subBlocks.at(i);
		if (subBlock.name == "events")
			event = EventConfig(subBlock);
		else if (subBlock.name == "http")
			http = HttpConfig(subBlock);
		else
			;
	}
}

WebservConfig &WebservConfig::operator=(const WebservConfig &other)
{
	if (this != &other)
	{
		directives = other.directives;
		event = other.event;
		http = other.http;
	}
	return *this;
}

WebservConfig::~WebservConfig() {}

const ConfigFile::directives_t &WebservConfig::getDirectives() const { return directives; }

const HttpConfig &WebservConfig::getHttpConfig() const { return http; }

const EventConfig &WebservConfig::getEventConfig() const { return event; }

std::ostream &operator<<(std::ostream &os, const WebservConfig &conf)
{
	os << "[WebservConfig]" << std::endl;

	ConfigFile::directives_t::const_iterator it = conf.getDirectives().begin();
	for (; it != conf.getDirectives().end(); it++)
		os << "\t[" << (*it).first << "] : [" << (*it).second << "]" << std::endl;

	os << std::endl;
	os << conf.getEventConfig();
	os << std::endl;
	os << conf.getHttpConfig();
	return os;
}