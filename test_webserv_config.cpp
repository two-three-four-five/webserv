/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservConfigTest.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:01:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 22:21:09 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebservConfig.hpp"
#include "ConfigFile.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: test filename" << std::endl;
		return 1;
	}
	Hafserv::ConfigFile configFile = Hafserv::ConfigFile(argv[1]);
	Hafserv::WebservConfig config = Hafserv::WebservConfig(configFile);

	std::cout << config << std::endl;

	return 0;
}