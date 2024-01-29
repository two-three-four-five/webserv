/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:07:53 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/29 19:32:28 by gyoon            ###   ########.fr       */
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
	void setPattern(const std::string &pattern);
	void setAlias(const std::string &alias);
	bool isMatching(const std::string &url);

private:
	std::string pattern;
	std::string alias;
	// NOT NEEDED : std::string regExpr;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::LocationConfig &conf);

#endif