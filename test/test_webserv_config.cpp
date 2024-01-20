/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_webserv_config.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:01:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/20 18:55:15 by gyoon            ###   ########.fr       */
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
	Hafserv::ConfigFile configFile = Hafserv::ConfigFile(argv[1]);
	Hafserv::WebservConfig config = Hafserv::WebservConfig(configFile);

	std::cout << configFile << std::endl;
	std::cout << config << std::endl;

	return 0;
}