/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegularFile.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:46:48 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/20 21:00:33 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RegularFile.hpp"

using namespace Hafserv;

RegularFile::RegularFile() : File(), extension(""), contents() {}

RegularFile::RegularFile(const RegularFile &other)
	: File(other.code, other.name), extension(other.extension), contents(std::vector<std::string>(other.contents))
{
}

RegularFile::RegularFile(const std::string &filename) : File(filename), extension(""), contents()
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

const std::vector<std::string> &RegularFile::getContents() const { return contents; }

size_t RegularFile::getContentsSize() const { return contents.size(); }

const std::string &RegularFile::getExtension() const { return extension; }

const std::string &RegularFile::getline(size_t lineNum) const { return contents.at(lineNum); }

std::ostream &operator<<(std::ostream &os, const RegularFile &file)
{
	os << "code: " << file.getCode() << std::endl;
	os << "name: " << file.getName() << std::endl;
	if (file.getCode() == File::REGULAR_FILE)
	{
		std::cout << "Extension: " << file.getExtension() << std::endl;
		std::cout << "Contents-length: " << file.getContentsSize() << std::endl;
		for (size_t i = 0; i < file.getContentsSize(); i++)
			std::cout << file.getContents()[i] << std::endl;
	}
	else
		std::cout << "not a regular file. error code : " << file.getCode();

	return os;
}