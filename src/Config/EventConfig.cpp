/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:58:14 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/11 20:35:28 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/EventConfig.hpp"

using namespace Hafserv;

EventConfig::EventConfig() : AConfig(), workerConnections() {}

EventConfig::EventConfig(const EventConfig &other) : AConfig(other), workerConnections(other.workerConnections) {}

EventConfig::EventConfig(const ConfigFile &block) throw(ParseError) : AConfig(), workerConnections()
{
	ConfigFile::directives_t::const_iterator it = block.getDirectives().begin();
	for (; it != block.getDirectives().end(); it++)
	{
		const std::string &key = (*it).first;
		const std::string &value = (*it).second;
		if (allBlockDirectives.count(key))
			throw NoBraceError(key);
		else if (!allSimpleDirectives.count(key))
			throw UnknownDirectiveError(key);
		else if (key == "worker_connections")
		{
			if (util::string::stoi(value).first)
				workerConnections = util::string::stoi(value).second;
			else
				throw ParseError("stoi failed: " + value);
		}
		else
			throw DisallowDirectiveError(key);
	}

	if (block.getSubBlocks().size())
	{
		const std::string &subBlockName = block.getSubBlocks().front().getBlockDirective();
		throw DisallowDirectiveError(subBlockName);
	}
}

EventConfig &EventConfig::operator=(const EventConfig &other)
{
	if (this != &other)
	{
		AConfig::operator=(other);
		workerConnections = other.workerConnections;
	}
	return *this;
}

EventConfig::~EventConfig() {}

int EventConfig::getWorkerConnections() const { return workerConnections; }

std::ostream &operator<<(std::ostream &os, const EventConfig &conf)
{
	os << "[EventConfig]" << std::endl;

	os << "workerConnections: " << conf.getWorkerConnections() << std::endl;
	return os;
}
