/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileTest.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 21:30:38 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 13:47:26 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RegularFile.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: test filename" << std::endl;
		return 1;
	}

	Hafserv::RegularFile file = Hafserv::RegularFile(argv[1]);
	file.printProperty();
	file.print();

	return 0;
}