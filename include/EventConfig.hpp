/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:58:12 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 16:11:29 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTCONFIG_HPP
#define EVENTCONFIG_HPP

#include "ConfigFile.hpp"

namespace Hafserv
{
class EventConfig
{
public:
	EventConfig();
	EventConfig(const EventConfig &other);
	EventConfig(const ConfigFile &block);
	EventConfig &operator=(const EventConfig &other);
	~EventConfig();

	const ConfigFile::directives_t &getDirectives() const;

private:
	ConfigFile::directives_t directives;
};

} // namespace Hafserv

#endif