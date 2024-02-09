/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegularFile.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:46:48 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/09 17:42:35 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RegularFile.hpp"
#include <sstream>

using namespace Hafserv;

RegularFile::RegularFile() : File(), extension(""), contents() {}

RegularFile::RegularFile(const RegularFile &other)
	: File(other.code, other.name), extension(other.extension), contents(other.contents)
{
}

RegularFile::RegularFile(const std::string &filename) : File(filename), extension(""), contents()
{
	if (code != REGULAR_FILE)
		return;

	if (filename.rfind('.') != std::string::npos)
		extension = filename.substr(filename.rfind('.') + 1, filename.size() - filename.rfind('.'));

	std::ifstream fs;
	fs.open(filename.c_str(), std::fstream::binary);
	if (!fs.is_open())
	{
		code = UNKNOWN_ERROR;
		return;
	}

	std::stringstream ss;
	ss << fs.rdbuf();
	contents = ss.str();

	fs.close();
}

RegularFile &RegularFile::operator=(const RegularFile &other)
{
	if (this != &other)
	{
		File::operator=(other);
		extension = other.extension;
		contents = other.contents;
	}
	return *this;
}

RegularFile::~RegularFile() {}

const std::string &RegularFile::getContents() const { return contents; }

size_t RegularFile::getContentsSize() const { return contents.length(); }

const std::string &RegularFile::getExtension() const { return extension; }

std::ostream &operator<<(std::ostream &os, const RegularFile &file)
{
	os << "code: " << file.getCode() << std::endl;
	os << "name: " << file.getName() << std::endl;
	if (file.getCode() == File::REGULAR_FILE)
	{
		std::cout << "Extension: " << file.getExtension() << std::endl;
		std::cout << "Contents-length: " << file.getContentsSize() << std::endl;
		std::cout << file.getContents();
	}
	else
		std::cout << "not a regular file. error code : " << file.getCode();

	return os;
}
