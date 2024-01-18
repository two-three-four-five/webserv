/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:08:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/18 16:42:31 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

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

	std::string name;
	std::vector<std::string> parameters;
	std::multimap<std::string, std::string> directives;
	std::vector<ConfigFile> subBlocks;

private:
	static const std::string meta;

	// TODO: error code
};

} // namespace Hafserv

#endif