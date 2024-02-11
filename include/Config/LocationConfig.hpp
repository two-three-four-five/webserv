/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:07:53 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/11 20:56:11 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include "Config/AConfig.hpp"
#include "Config/AHttpConfigModule.hpp"
#include "Config/ConfigException.hpp"
#include "File/ConfigFile.hpp"
#include <iostream>
#include <string>

namespace Hafserv
{
class LocationConfig : public AConfig, public AHttpConfigModule
{
public:
	LocationConfig();
	LocationConfig(const LocationConfig &other);
	LocationConfig(const ConfigFile &block, const HttpConfigCore &core);
	LocationConfig &operator=(const LocationConfig &other);
	~LocationConfig();

	const std::string &getModifier() const;
	const std::string &getPattern() const;
	const std::string &getAlias() const;
	const std::string &getProxyPass() const;
	const std::string &getCgiPath() const;

	void setModifier(const std::string &modifier);
	void setPattern(const std::string &pattern);
	void setAlias(const std::string &alias);
	void setProxyPass(const std::string &proxyPass);
	void setCgiPath(const std::string &cgiPath);

	bool isMatching(const std::string &url);

protected:
	virtual void initSimpleDirectives();
	virtual void initBlockDirectives();

private:
	std::string modifier;
	std::string pattern; // TODO: change pattern into URI
	std::string alias;
	std::string proxyPass;
	std::string cgiPath;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::LocationConfig &conf);

#endif
