/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:58:14 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/22 22:21:18 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventConfig.hpp"

using namespace Hafserv;

EventConfig::EventConfig() : workerConnections() {}

EventConfig::EventConfig(const EventConfig &other) : workerConnections(other.workerConnections) {}

EventConfig::EventConfig(const ConfigFile &block) throw(ParseError) : workerConnections()
{
	std::string key, value;
	ConfigFile::directives_t::const_iterator it = block.directives.begin();
	for (; it != block.directives.end(); it++)
	{
		key = (*it).first;
		value = (*it).second;
		if (key == "worker_connections")
		{
			if (util::string::stoi(value).first)
				workerConnections = util::string::stoi(value).second;
			else
				throw ParseError("stoi failed: " + value);
		}
		else
			throw ParseError("unexpected block context directive: " + key);
	}
}

EventConfig &EventConfig::operator=(const EventConfig &other)
{
	if (this != &other)
	{
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