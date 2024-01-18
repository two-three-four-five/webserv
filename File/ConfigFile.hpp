/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:08:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/18 13:38:23 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "RegularFile.hpp"
#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <vector>

namespace Hafserv
{
class ConfigFile // TODO: inherits public RegularFile in the future
{
public:
	typedef std::vector<std::string> parameters_t;
	typedef std::multimap<std::string, std::string> directives_t;
	typedef std::vector<ConfigFile> subblocks_t;

	ConfigFile();
	ConfigFile(const ConfigFile &other);
	ConfigFile(const std::string &filename);
	ConfigFile &operator=(const ConfigFile &other);
	virtual ~ConfigFile() throw();

	void print() const;

private:
	static const std::string meta;

	// TODO: error code
	std::string name;
	std::vector<std::string> parameters;
	std::multimap<std::string, std::string> directives;
	std::vector<ConfigFile> subBlocks;
};

} // namespace Hafserv

#endif