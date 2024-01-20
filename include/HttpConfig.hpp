/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:46:19 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/20 13:22:20 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONFIG_HPP
#define HTTPCONFIG_HPP

#include "AHttpConfigModule.hpp"
#include "ConfigFile.hpp"
#include "ServerConfig.hpp"
#include <string>
#include <vector>

namespace Hafserv
{
class HttpConfig : public AHttpConfigModule
{
public:
	HttpConfig();
	HttpConfig(const HttpConfig &other);
	HttpConfig(const ConfigFile &block);
	HttpConfig &operator=(const HttpConfig &other);
	~HttpConfig();

	const ConfigFile::directives_t &getDirectives() const;
	const std::vector<ServerConfig> &getServers() const;

private:
	ConfigFile::directives_t directives;
	std::vector<ServerConfig> servers;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::HttpConfig &conf);

#endif