/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:58:12 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/22 22:20:37 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTCONFIG_HPP
#define EVENTCONFIG_HPP

#include "ConfigException.hpp"
#include "ConfigFile.hpp"
#include "util/string.hpp"

namespace Hafserv
{
class EventConfig
{
public:
	EventConfig();
	EventConfig(const EventConfig &other);
	EventConfig(const ConfigFile &block) throw(ParseError);
	EventConfig &operator=(const EventConfig &other);
	~EventConfig();

	int getWorkerConnections() const;

private:
	int workerConnections;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::EventConfig &conf);

#endif