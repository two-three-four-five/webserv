/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:58:14 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 16:11:42 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EventConfig.hpp"

using namespace Hafserv;

EventConfig::EventConfig() : directives() {}

EventConfig::EventConfig(const EventConfig &other) : directives(other.directives) {}

EventConfig::EventConfig(const ConfigFile &block) : directives(block.directives) {}

EventConfig &EventConfig::operator=(const EventConfig &other)
{
	if (this != &other)
	{
		directives = other.directives;
	}
	return *this;
}

EventConfig::~EventConfig() {}

const ConfigFile::directives_t &EventConfig::getDirectives() const { return directives; }
