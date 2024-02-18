/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:46:19 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/18 15:12:56 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVCONFIG_HPP
#define WEBSERVCONFIG_HPP

#include "Config/AConfig.hpp"
#include "Config/ConfigException.hpp"
#include "Config/EventConfig.hpp"
#include "Config/HttpConfig.hpp"
#include "File/ConfigFile.hpp"
#include <string>

namespace Hafserv
{
class WebservConfig : public AConfig
{
public:
	WebservConfig();
	WebservConfig(const WebservConfig &other);
	WebservConfig(const ConfigFile &configFile) throw(std::logic_error);
	WebservConfig &operator=(const WebservConfig &other);
	~WebservConfig();

	const HttpConfig &getHttpConfig() const;
	const EventConfig &getEventConfig() const;

private:
	EventConfig event;
	HttpConfig http;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::WebservConfig &conf);

#endif
