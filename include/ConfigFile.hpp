/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:08:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/09 18:03:40 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include "ConfigException.hpp"
#include "RegularFile.hpp"
#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <vector>

namespace Hafserv
{
class ConfigFile : public RegularFile
{
public:
	typedef std::vector<std::string> parameters_t;
	typedef std::multimap<std::string, std::string> directives_t;
	typedef std::vector<ConfigFile> subblocks_t;

	ConfigFile();
	ConfigFile(const ConfigFile &other);
	ConfigFile(const RegularFile &other);
	ConfigFile(const std::string &filename);
	ConfigFile &operator=(const ConfigFile &other);
	virtual ~ConfigFile() throw();

	const std::string &getBlockDirective() const;
	const parameters_t &getParameters() const;
	const directives_t &getDirectives() const;
	const subblocks_t &getSubBlocks() const;

	void include() throw(IncludeError);

private:
	static const std::string meta;

	std::vector<std::string> lines;

	std::string blockDirective;
	std::vector<std::string> parameters;
	std::multimap<std::string, std::string> directives;
	std::vector<ConfigFile> subBlocks;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::ConfigFile &configFile);

#endif
