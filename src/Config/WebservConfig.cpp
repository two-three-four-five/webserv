/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:46:15 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/11 19:29:06 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/WebservConfig.hpp"

using namespace Hafserv;

WebservConfig::WebservConfig() : AConfig(), directives(), event(), http() {}

WebservConfig::WebservConfig(const WebservConfig &other)
	: AConfig(other), directives(other.directives), event(other.event), http(other.http)
{
}

WebservConfig::WebservConfig(const ConfigFile &configFile) throw(ParseError) : AConfig(), directives(), event(), http()
{
	ConfigFile main = configFile.getSubBlocks().at(0);

	directives = main.getDirectives();
	ConfigFile::directives_t::const_iterator it = main.getDirectives().begin();
	for (; it != main.getDirectives().end(); it++)
	{
		const std::string &key = (*it).first;
		const std::string &value = (*it).second;

		if (blockDirectives.count(key))
			throw NoBraceError(key);
		else if (!simpleDirectives.count(key))
			throw UnknownDirectiveError(key);
		else if (key == "user")
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
			throw DisallowDirectiveError(key);
	}

	bool hasEvent = false, hasHttp = false;
	for (size_t i = 0; i < main.getSubBlocks().size(); i++)
	{
		const ConfigFile &subBlock = main.getSubBlocks().at(i);
		const std::string &subBlockName = subBlock.getBlockDirective();

		if (simpleDirectives.count(subBlockName))
			throw NoSemicolonError(subBlockName);
		else if (!blockDirectives.count(subBlockName))
			throw UnknownDirectiveError(subBlockName);
		else if (subBlockName == "events")
		{
			if (hasEvent)
				throw DuplicateDirectiveError("events");
			hasEvent = true;
			event = EventConfig(subBlock);
		}
		else if (subBlockName == "http")
		{
			if (hasHttp)
				throw DuplicateDirectiveError("http");
			hasHttp = true;
			http = HttpConfig(subBlock);
		}
		else
			throw DisallowDirectiveError(subBlockName);
	}
}

WebservConfig &WebservConfig::operator=(const WebservConfig &other)
{
	if (this != &other)
	{
		AConfig::operator=(other);
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
