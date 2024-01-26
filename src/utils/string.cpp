/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 15:47:09 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/20 23:06:49 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util/string.hpp"

using namespace Hafserv;

std::pair<bool, int> util::string::stoi(const std::string &str)
{
	std::pair<bool, int> ret = std::make_pair(false, 0);
	std::stringstream ss(str);
	ss >> ret.second;
	ret.first = !ss.fail() && !ss.bad() && ss.eof();
	return ret;
}

std::vector<std::string> util::string::split(const std::string &str, char delimiter)
{
	std::vector<std::string> vec;
	std::stringstream ss(str);
	std::string section;
	while (std::getline(ss, section, delimiter))
	{
		std::stringstream sector(section);
		std::string trimmed;
		sector >> trimmed;
		if (trimmed.size())
			vec.push_back(trimmed);
	}
	return vec;
}

bool util::string::hasSpace(const std::string &str)
{
	for (int i = 0; i < str.length(); i++)
		if (std::isspace(str[i]))
			return true;
	return false;
};

std::string util::string::toLower(const std::string &str)
{
	std::string ret = str;
	for (int i = 0; i < str.length(); i++)
		ret[i] = std::tolower(str[i]);
	return ret;
}
