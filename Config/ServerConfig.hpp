/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:46:10 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/18 22:42:47 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "ConfigFile.hpp"
#include "LocationConfig.hpp"
#include <string>
#include <vector>

namespace Hafserv
{
class ServerConfig
{
public:
	ServerConfig();
	ServerConfig(const ServerConfig &other);
	ServerConfig(const ConfigFile &block);
	ServerConfig &operator=(const ServerConfig &other);
	~ServerConfig();

	const std::vector<std::string> &getNames() const;
	const std::vector<unsigned short> &getPorts() const;
	const std::vector<LocationConfig> &getLocations() const;

	void print() const;

private:
	std::vector<std::string> names;
	std::vector<unsigned short> ports;
	std::vector<LocationConfig> locations;
};

} // namespace Hafserv

#endif