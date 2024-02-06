/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:07:53 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/06 16:51:42 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include "AHttpConfigModule.hpp"
#include "ConfigException.hpp"
#include "ConfigFile.hpp"
#include <iostream>
#include <string>

namespace Hafserv
{
class LocationConfig : public AHttpConfigModule
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
	virtual bool isCoreDirective(const std::string &directive);

private:
	std::string modifier;
	std::string pattern;
	std::string alias;
	std::string proxyPass;
	std::string cgiPath;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::LocationConfig &conf);

#endif
