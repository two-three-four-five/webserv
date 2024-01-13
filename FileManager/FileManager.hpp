/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:50:04 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 01:43:54 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

#include <dirent.h>

namespace Hafserv
{

struct File
{
	bool isValid; // -> int errno like not exist, no permission ...
	std::string name;
};

struct RegularFile : public File
{
	std::string extention;
	std::vector<std::string> contents;

	void printProperty() const;
	void print() const;
};

struct Directory : public File
{
	DIR *info;
};

class FileManager
{
public:
	static bool isExist(const std::string &filename);
	static bool isDirectory(const std::string &filename);
	static bool isReadable(const std::string &filename);
	static bool isRegularFile(const std::string &filename);

	static RegularFile readRegularFile(const std::string &filename) throw(); // throw if not regular file

private:
	FileManager();
};

} // namespace Hafserv

#endif