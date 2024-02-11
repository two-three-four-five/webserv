/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfig.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 22:46:19 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/11 20:44:06 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONFIG_HPP
#define HTTPCONFIG_HPP

#include "Config/AConfig.hpp"
#include "Config/AHttpConfigModule.hpp"
#include "Config/ConfigException.hpp"
#include "Config/ServerConfig.hpp"
#include "File/ConfigFile.hpp"
#include <string>
#include <vector>

namespace Hafserv
{
class HttpConfig : public AConfig, public AHttpConfigModule
{
public:
	HttpConfig();
	HttpConfig(const HttpConfig &other);
	HttpConfig(const ConfigFile &block) throw(ParseError);
	HttpConfig &operator=(const HttpConfig &other);
	~HttpConfig();

	const ConfigFile::directives_t &getDirectives() const;
	const std::vector<ServerConfig> &getServers() const;

protected:
	virtual void initSimpleDirectives();
	virtual void initBlockDirectives();

private:
	ConfigFile::directives_t directives;
	std::vector<ServerConfig> servers;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::HttpConfig &conf);

#endif
