/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:08:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/13 19:20:48 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "FileManager.hpp"
#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <vector>

namespace Hafserv
{

typedef std::string directive_t;
typedef std::string parameter_t;
// typedef std::multimap<directive_t, parameter_t> config_t;

struct Config
{
public:
	directive_t name;
	std::vector<parameter_t> options;

	std::multimap<directive_t, parameter_t> values;
	std::vector<Config> subBlocks;
};

class ConfigParser
{
public:
	static void printConfig(Config config);
	static Config parse(std::string filename);

private:
	ConfigParser();

	static const std::string meta;
};

} // namespace Hafserv

#endif