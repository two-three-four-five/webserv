/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:47:01 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/11 21:23:36 by gyoon            ###   ########.fr       */
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
	static std::set<std::string> allSimpleDirectives;
	static std::set<std::string> mainSimpleDirectives;
	static std::set<std::string> eventsSimpleDirectives;
	static std::set<std::string> httpSimpleDirectives;
	static std::set<std::string> serverSimpleDirectives;
	static std::set<std::string> locationSimpleDirectives;
	static std::set<std::string> coreSimpleDirectives;

	static std::set<std::string> allBlockDirectives;
	static std::set<std::string> mainBlockDirectives;
	static std::set<std::string> eventsBlockDirectives;
	static std::set<std::string> httpBlockDirectives;
	static std::set<std::string> serverBlockDirectives;
	static std::set<std::string> locationBlockDirectives;
	static std::set<std::string> coreBlockDirectives;

private:
	void initAllSimpleDirectives();
	void initMainSimpleDirectives();
	void initEventsSimpleDirectives();
	void initHttpSimpleDirectives();
	void initServerSimpleDirectives();
	void initLocationSimpleDirectives();
	void initCoreSimpleDirectives();

	void initAllBlockDirectives();
	void initMainBlockDirectives();
	void initEventsBlockDirectives();
	void initHttpBlockDirectives();
	void initServerBlockDirectives();
	void initLocationBlockDirectives();
	void initCoreBlockDirectives();
};
} // namespace Hafserv

#endif