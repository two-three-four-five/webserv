/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParserTest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:01:10 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/10 15:04:29 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: test filename" << std::endl;
		return 1;
	}
	ConfigFile configFile = ConfigParser::parse(argv[1]);
	config_t::iterator it = configFile.config.begin();
	for (; it != configFile.config.end(); it++)
	{
		std::cout << "key: ";
		for (size_t i = 0; i < (*it).first.size(); i++)
			std::cout << (*it).first.at(i) << " ";
		std::cout << "value: " << (*it).second << std::endl;
	}

	return 0;
}