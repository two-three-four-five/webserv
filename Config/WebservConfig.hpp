/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:46:19 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 16:51:45 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVCONFIG_HPP
#define WEBSERVCONFIG_HPP

#include "ConfigFile.hpp"
#include "EventConfig.hpp"
#include "HttpConfig.hpp"
#include <string>

namespace Hafserv
{
class WebservConfig
{
public:
	WebservConfig();
	WebservConfig(const WebservConfig &other);
	WebservConfig(const ConfigFile &block);
	WebservConfig &operator=(const WebservConfig &other);
	~WebservConfig();

	const ConfigFile::directives_t &getDirectives() const;
	const HttpConfig &getHttpConfig() const;
	const EventConfig &getEventConfig() const;

private:
	ConfigFile::directives_t directives;
	EventConfig event;
	HttpConfig http;
};

} // namespace Hafserv

#endif