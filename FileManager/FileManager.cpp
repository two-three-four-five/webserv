/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:07:37 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/09 10:25:24 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileManager.hpp"

FileManager::FileManager() {}

FileManager::FileManager(const FileManager &other) {}

FileManager::~FileManager() {}

FileManager &FileManager::operator=(const FileManager &other)
{
    if (this != &other)
        ;
    return *this;
}

File FileManager::openFile(std::string filename)
{
    File file = File();
    file.name = filename;
    if (filename.rfind('.') != std::string::npos)
        file.extention = filename.substr(filename.rfind('.'), filename.size() - filename.rfind('.'));

    std::fstream fs;
    fs.open(filename, std::fstream::in);
    if (!fs.is_open())
        return file;
    // TODO: check that file is directory

    std::string line;
    while (std::getline(fs, line))
        file.contents.push_back(line);

    fs.close();
    file.isValid = true;
    return file;
}

void FileManager::printFileInfo(File file)
{
    std::cout << "Filename: " << file.name << std::endl;
    std::cout << "Extension: " << file.extention << std::endl;
    std::cout << "Contents-length: " << file.contents.size() << std::endl;

    std::cout << "Contents:" << std::endl;
    printFileContents(file);
}

void FileManager::printFileContents(File file)
{
    for (size_t i = 0; i < file.contents.size(); i++)
        std::cout << file.contents[i] << std::endl;
}
