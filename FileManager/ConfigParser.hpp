/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:08:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/11 20:25:47 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "FileManager.hpp"
#include <algorithm>
#include <list>
#include <map>
#include <string>

/*
 * directives can belong to some level of contexts.
 * contexts and directive are represented as directive_t
 *
 * directives can have 0 or more parameters.
 * parameters are saved in config_t as values.
 *
 * because there can be various parameters,
 * config_t is designed with multimap.
 */

typedef std::vector<std::string> directive_t;
typedef std::string parameter_t;
typedef std::multimap<directive_t, parameter_t> config_t;

struct ConfigFile
{
	config_t config;
};

class ConfigParser
{
public:
	ConfigParser();
	ConfigParser(const ConfigParser &other);
	ConfigParser &operator=(const ConfigParser &other);
	~ConfigParser();

	static void printConfig(config_t config);
	static config_t parse(std::string filename);

private:
	static const std::string meta;

	static config_t parseSingleFile(std::string filename);
};

#endif