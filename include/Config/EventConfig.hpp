/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:58:12 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/18 15:08:53 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTCONFIG_HPP
#define EVENTCONFIG_HPP

#include "Config/AConfig.hpp"
#include "Config/ConfigException.hpp"
#include "File/ConfigFile.hpp"
#include "util/string.hpp"

namespace Hafserv
{
class EventConfig : public AConfig
{
public:
	EventConfig();
	EventConfig(const EventConfig &other);
	EventConfig(const ConfigFile &block) throw(std::logic_error);
	EventConfig &operator=(const EventConfig &other);
	~EventConfig();

	int getWorkerConnections() const;

private:
	int workerConnections;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::EventConfig &conf);

#endif
