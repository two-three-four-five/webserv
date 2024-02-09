/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:46:15 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/08 13:57:55 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebservConfig.hpp"

using namespace Hafserv;

WebservConfig::WebservConfig() : directives(), event(), http() {}

WebservConfig::WebservConfig(const WebservConfig &other)
	: directives(other.directives), event(other.event), http(other.http)
{
}

WebservConfig::WebservConfig(const ConfigFile &configFile) throw(ParseError) : directives(), event(), http()
{
	ConfigFile main = configFile.getSubBlocks().at(0);

	directives = main.getDirectives();
	std::string key, value;
	ConfigFile::directives_t::const_iterator it = main.getDirectives().begin();
	for (; it != main.getDirectives().end(); it++)
	{
		key = (*it).first;
		value = (*it).second;
		if (key == "user")
			;
		else if (key == "worker_processes")
			;
		else if (key == "error_log")
			;
		else if (key == "pid")
			;
		else if (key == "worker_rlimit_nofile")
			;
		else
			throw ParseError("unexpected main context directive: " + key);
	}

	ConfigFile subBlock;
	for (size_t i = 0; i < main.getSubBlocks().size(); i++)
	{
		subBlock = main.getSubBlocks().at(i);
		if (subBlock.getName() == "events")
			event = EventConfig(subBlock);
		else if (subBlock.getName() == "http")
			http = HttpConfig(subBlock);
		else
			throw ParseError("unexpected block directive: " + subBlock.getName());
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