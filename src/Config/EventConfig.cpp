/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:58:14 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/22 18:38:27 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventConfig.hpp"

using namespace Hafserv;

EventConfig::EventConfig() : workerConnections() {}

EventConfig::EventConfig(const EventConfig &other) : workerConnections(other.workerConnections) {}

EventConfig::EventConfig(const ConfigFile &block) : workerConnections()
{
	ConfigFile::directives_t::const_iterator it = block.directives.begin();
	for (; it != block.directives.end(); it++)
		if ((*it).first == "worker_connections")
			if (util::string::stoi((*it).second).first)
				workerConnections = util::string::stoi((*it).second).second;
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