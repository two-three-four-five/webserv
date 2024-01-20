/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 15:47:09 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/20 18:25:15 by gyoon            ###   ########.fr       */
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
