/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileManagerTest.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 21:30:38 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/09 10:27:11 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileManager.hpp"
#include <iostream>
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: test filename" << std::endl;
        return 1;
    }

    File file = FileManager::openFile(argv[1]);
    FileManager::printFileInfo(file);
    return 0;
}