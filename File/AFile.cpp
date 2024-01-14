/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFile.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:46:33 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 13:51:20 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AFile.hpp"

using namespace Hafserv;

AFile::AFile() : code(NOT_EXIST), name("") {}

AFile::AFile(const AFile &other) : code(other.code), name(other.name) {}

AFile::AFile(const std::string &filename) : code(NOT_EXIST), name(filename)
{
	if (!isExist(name))
		code = NOT_EXIST;
	else if (!isReadable(name))
		code = PERMISSION_DENIED;
	else if (isDirectory(name))
		code = IS_A_DIRECTORY;
	else if (isRegularFile(name))
		code = REGULAR_FILE;
	else
		code = UNKNOWN_ERROR;
}

AFile &AFile::operator=(const AFile &other)
{
	if (this != &other)
	{
		code = other.code;
		name = other.name;
	}
	return *this;
}

AFile::~AFile() {}

bool AFile::isExist(const std::string &filename)
{
	if (filename.empty())
		return true;
	else
		return access(filename.c_str(), F_OK) == 0;
}

bool AFile::isDirectory(const std::string &filename)
{
	int fd = open(filename.c_str(), O_DIRECTORY);
	close(fd);

	return fd >= 0;
}

bool AFile::isReadable(const std::string &filename)
{
	return isExist(filename) && (access(filename.c_str(), R_OK) == 0);
}

bool AFile::isRegularFile(const std::string &filename)
{
	return isExist(filename) && !isDirectory(filename) && isReadable(filename);
}
