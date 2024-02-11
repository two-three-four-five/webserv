/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:47:01 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/11 19:17:03 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACONFIG_HPP
#define ACONFIG_HPP

#include <set>
#include <string>

namespace Hafserv
{
class AConfig
{
public:
	AConfig();
	AConfig(const AConfig &other);
	AConfig &operator=(const AConfig &other);
	~AConfig();

protected:
	// static std::vector<std::string> directives;
	static std::set<std::string> simpleDirectives;
	static std::set<std::string> blockDirectives;

private:
	// void initDirectives();
	void initSimpleDirectives();
	void initBlockDirectives();
};
} // namespace Hafserv

#endif