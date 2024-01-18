/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigTest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:01:41 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/18 16:02:55 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: test filename" << std::endl;
		return 1;
	}
	Hafserv::ConfigFile configFile = Hafserv::ConfigFile(argv[1]);
	configFile.print();
	return 0;
}