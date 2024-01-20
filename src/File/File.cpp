/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:46:33 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/20 21:00:20 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File.hpp"

using namespace Hafserv;

File::File() : code(NOT_EXIST), name("") {}

File::File(const File &other) : code(other.code), name(other.name) {}

File::File(const std::string &filename) : code(NOT_EXIST), name(filename)
{
	if (!isExist(name))
		code = NOT_EXIST;
	else if (!isReadable(name))
		code = PERMISSION_DENIED;
	else if (isDirectory(name))
		code = DIRECTORY;
	else if (isRegularFile(name))
		code = REGULAR_FILE;
	else
		code = UNKNOWN_ERROR;
}

File::File(int code, const std::string &name) : code(code), name(name) {}

File &File::operator=(const File &other)
{
	if (this != &other)
	{
		code = other.code;
		name = other.name;
	}
	return *this;
}

File::~File() {}

bool File::isExist(const std::string &filename)
{
	if (filename.empty())
		return true;
	else
		return access(filename.c_str(), F_OK) == 0;
}

bool File::isDirectory(const std::string &filename)
{
	int fd = open(filename.c_str(), O_DIRECTORY);
	close(fd);

	return fd >= 0;
}

bool File::isReadable(const std::string &filename)
{
	return isExist(filename) && (access(filename.c_str(), R_OK) == 0);
}

bool File::isRegularFile(const std::string &filename)
{
	return isExist(filename) && !isDirectory(filename) && isReadable(filename);
}

int File::getCode() const { return code; }

const std::string &File::getName() const { return name; }

std::ostream &operator<<(std::ostream &os, const File &file)
{
	os << "code: " << file.getCode() << std::endl;
	os << "name: " << file.getName();
	return os;
}