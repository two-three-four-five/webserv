/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:07:53 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/18 22:42:43 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include "ConfigFile.hpp"
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

	// std::string &getPattern();
	// std::string &getRoot();
	const std::string &getPattern() const;
	const std::string &getRoot() const;
	void setPattern(const std::string &pattern);
	void setRoot(const std::string &root);

	bool isMatching(const std::string &url);

private:
	std::string pattern;
	std::string root;
	// NOT NEEDED : std::string regExpr;
};

} // namespace Hafserv

#endif