/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:46:00 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/18 17:20:27 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

using namespace Hafserv;

ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig &other) {}

ServerConfig::ServerConfig(const ConfigFile &block) {}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
	}
	return *this;
}

ServerConfig::~ServerConfig() {}
