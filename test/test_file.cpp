/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_file.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 21:30:38 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/13 16:06:27 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File/Directory.hpp"
#include "File/RegularFile.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: test filename" << std::endl;
		return 1;
	}

	Hafserv::RegularFile file = Hafserv::RegularFile(argv[1]);
	std::cout << file << std::endl;

	std::cout << std::endl;

	Hafserv::Directory dir = Hafserv::Directory(argv[1]);
	std::cout << dir << std::endl;
	return 0;
}
