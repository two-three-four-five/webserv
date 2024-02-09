/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_webserv_config.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:01:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/09 14:45:31 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"
#include "WebservConfig.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: test filename" << std::endl;
		return 1;
	}

	try
	{
		Hafserv::ConfigFile configFile = Hafserv::ConfigFile(argv[1]);
		std::cout << configFile << std::endl;
		configFile.include();
		std::cout << "-----------------------------------" << std::endl;
		std::cout << configFile << std::endl;
		Hafserv::WebservConfig config = Hafserv::WebservConfig(configFile);
		std::cout << config << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return 0;
}