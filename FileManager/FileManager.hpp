/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:50:04 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 01:32:36 by gyoon            ###   ########.fr       */
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
	static bool isExist(std::string filename);
	static bool isDirectory(std::string filename);
	static bool isReadable(std::string filename);
	static bool isFile(std::string filename);

	static RegularFile openFile(std::string filename);
	static void printFileInfo(RegularFile file);
	static void printFileContents(RegularFile file);

private:
	FileManager();
};

} // namespace Hafserv

#endif