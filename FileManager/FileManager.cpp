/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:07:37 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/13 12:42:26 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileManager.hpp"

using namespace Hafserv;

FileManager::FileManager() {}

bool FileManager::isExist(std::string filename)
{
	if (filename.empty())
		return true;
	else
		return access(filename.c_str(), F_OK) == 0;
}

bool FileManager::isDirectory(std::string filename)
{
	int fd = open(filename.c_str(), O_DIRECTORY);
	close(fd);

	return fd >= 0;
}

bool FileManager::isReadable(std::string filename)
{
	return isExist(filename) && (access(filename.c_str(), R_OK) == 0);
}

bool FileManager::isFile(std::string filename)
{
	return isExist(filename) && !isDirectory(filename) && isReadable(filename);
}

File FileManager::openFile(std::string filename)
{
	File file = File();
	file.name = filename;
	if (filename.rfind('.') != std::string::npos)
		file.extention = filename.substr(filename.rfind('.') + 1, filename.size() - filename.rfind('.'));

	std::fstream fs;
	fs.open(filename.c_str(), std::fstream::in);
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
