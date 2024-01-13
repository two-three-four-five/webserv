/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:07:37 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 01:44:51 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FileManager.hpp"

using namespace Hafserv;

void RegularFile::printProperty() const
{
	std::cout << "Filename: " << name << std::endl;
	std::cout << "Extension: " << extention << std::endl;
	std::cout << "Contents-length: " << contents.size() << std::endl;
}

void RegularFile::print() const
{
	for (size_t i = 0; i < contents.size(); i++)
		std::cout << contents[i] << std::endl;
}

FileManager::FileManager() {}

bool FileManager::isExist(const std::string &filename)
{
	if (filename.empty())
		return true;
	else
		return access(filename.c_str(), F_OK) == 0;
}

bool FileManager::isDirectory(const std::string &filename)
{
	int fd = open(filename.c_str(), O_DIRECTORY);
	close(fd);

	return fd >= 0;
}

bool FileManager::isReadable(const std::string &filename)
{
	return isExist(filename) && (access(filename.c_str(), R_OK) == 0);
}

bool FileManager::isRegularFile(const std::string &filename)
{
	return isExist(filename) && !isDirectory(filename) && isReadable(filename);
}

RegularFile FileManager::readRegularFile(const std::string &filename) throw()
{
	RegularFile file = RegularFile();
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