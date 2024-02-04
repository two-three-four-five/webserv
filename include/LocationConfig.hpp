/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:07:53 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/03 16:14:23 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include "AHttpConfigModule.hpp"
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

	const std::string &getPattern() const;
	const std::string &getAlias() const;
	const std::string &getProxyPass() const;
	void setPattern(const std::string &pattern);
	void setAlias(const std::string &alias);
	void setProxyPass(const std::string &proxyPass);
	bool isMatching(const std::string &url);

private:
	std::string pattern;
	std::string alias;
	std::string proxyPass;
	// NOT NEEDED : std::string regExpr;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::LocationConfig &conf);

#endif
