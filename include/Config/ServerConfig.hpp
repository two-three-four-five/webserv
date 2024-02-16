/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:46:10 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/16 17:14:01 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Config/AConfig.hpp"
#include "Config/AHttpConfigCore.hpp"
#include "Config/LocationConfig.hpp"
#include "File/ConfigFile.hpp"
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace Hafserv
{
class ServerConfig : public AConfig, public AHttpConfigCore
{
public:
	ServerConfig();
	ServerConfig(const ServerConfig &other);
	ServerConfig(const ConfigFile &block, const AHttpConfigCore &core);
	ServerConfig &operator=(const ServerConfig &other);
	~ServerConfig();

	const std::set<std::string> &getNames() const;
	const std::set<unsigned short> &getPorts() const;
	const std::vector<std::vector<LocationConfig> > &getLocations() const;

	const bool hasName(const std::string &name) const;
	const bool hasPort(const unsigned short port) const;

private:
	std::set<std::string> names;
	std::set<unsigned short> ports;
	std::vector<std::vector<LocationConfig> > locations;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::ServerConfig &conf);

#endif
