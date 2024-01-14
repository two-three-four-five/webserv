/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:08:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 16:53:40 by gyoon            ###   ########.fr       */
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
class Config
{
public:
	typedef std::vector<std::string> parameters_t;
	typedef std::multimap<std::string, std::string> directives_t;
	typedef std::vector<Config> subblocks_t;

	Config();
	Config(const Config &other);
	Config(const std::string &filename);
	Config &operator=(const Config &other);
	virtual ~Config() throw();

	void print() const;

private:
	static const std::string meta;

	// TODO: error code
	std::string name;
	std::vector<std::string> parameters;
	std::multimap<std::string, std::string> directives;
	std::vector<Config> subBlocks;
};

} // namespace Hafserv

#endif