/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:08:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 01:44:14 by gyoon            ###   ########.fr       */
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

struct Config
{
	typedef std::vector<std::string> parameters_t;
	typedef std::multimap<std::string, std::string> directives_t;

	std::string name;
	std::vector<std::string> parameters;
	std::multimap<std::string, std::string> directives;
	std::vector<Config> subBlocks;

	void print() const;
};

class ConfigParser
{
public:
	static Config parse(const std::string &filename);

private:
	ConfigParser();

	static const std::string meta;
};

} // namespace Hafserv

#endif