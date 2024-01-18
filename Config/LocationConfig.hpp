/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:07:53 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/18 17:19:32 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include "Configfile.hpp"
#include <string>

namespace Hafserv
{
class LocationConfig
{
public:
	LocationConfig();
	LocationConfig(const LocationConfig &other);
	LocationConfig(const ConfigFile &block);
	LocationConfig &operator=(const LocationConfig &other);
	~LocationConfig();

	bool isMatching(const std::string &url);

	std::string pattern;
	std::string root;

private:
	// NOT NEEDED : std::string regExpr;
};

} // namespace Hafserv

#endif