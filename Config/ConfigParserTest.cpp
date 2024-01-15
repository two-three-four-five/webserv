/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParserTest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 15:01:10 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 00:56:03 by gyoon            ###   ########.fr       */
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
	Hafserv::Config config = Hafserv::ConfigParser::parse(argv[1]);
	config.print();
	return 0;
}