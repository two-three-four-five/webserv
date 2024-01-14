/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegularFile.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:46:48 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 13:50:55 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RegularFile.hpp"

using namespace Hafserv;

RegularFile::RegularFile() : AFile(), extension("") {}

RegularFile::RegularFile(const RegularFile &other)
	: AFile(other.name), extension(other.extension), contents(std::vector<std::string>(other.contents))
{
}

RegularFile::RegularFile(const std::string &filename) : AFile(filename), extension("")
{
	if (code != REGULAR_FILE)
		return;

	if (filename.rfind('.') != std::string::npos)
		extension = filename.substr(filename.rfind('.') + 1, filename.size() - filename.rfind('.'));

	std::fstream fs;
	fs.open(filename.c_str(), std::fstream::in);
	if (!fs.is_open())
	{
		code = UNKNOWN_ERROR;
		return;
	}

	std::string line;
	while (std::getline(fs, line))
		contents.push_back(line);

	fs.close();
}

RegularFile &RegularFile::operator=(const RegularFile &other)
{
	if (this != &other)
	{
		code = other.code;
		name = other.name;
		extension = other.extension;
		contents = std::vector<std::string>(other.contents);
	}
	return *this;
}

RegularFile::~RegularFile() {}

void RegularFile::printProperty() const
{
	std::cout << "Filename: " << name << std::endl;
	if (code == REGULAR_FILE)
	{
		std::cout << "Extension: " << extension << std::endl;
		std::cout << "Contents-length: " << contents.size() << std::endl;
	}
	else
		std::cout << "not a regular file. error code : " << code << std::endl;
}

void RegularFile::print() const
{
	for (size_t i = 0; i < contents.size(); i++)
		std::cout << contents[i] << std::endl;
}
