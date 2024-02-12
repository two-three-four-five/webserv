/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:58:14 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/12 13:03:07 by gyoon            ###   ########.fr       */
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
		else if (!eventsSimpleDirectives.count(key))
			throw DisallowDirectiveError(key);

		if (key == "worker_connections")
		{
			if (util::string::stoi(value).first)
				workerConnections = util::string::stoi(value).second;
			else
				throw ParseError("stoi failed: " + value);
		}
	}

	for (size_t i = 0; i < block.getSubBlocks().size(); i++)
	{
		const ConfigFile &subBlock = block.getSubBlocks().at(i);
		const std::string &subBlockName = subBlock.getBlockDirective();

		if (allSimpleDirectives.count(subBlockName))
			throw NoSemicolonError(subBlockName);
		else if (!allBlockDirectives.count(subBlockName))
			throw UnknownDirectiveError(subBlockName);
		else if (!serverBlockDirectives.count(subBlockName))
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
