/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:50:04 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/12 21:10:55 by gyoon            ###   ########.fr       */
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

struct File
{
	bool isValid;

	std::string name;
	std::string extention;
	std::vector<std::string> contents;
};

class FileManager
{
public:
	FileManager();
	FileManager(const FileManager &other);
	~FileManager();
	FileManager &operator=(const FileManager &other);

	static bool isExist(std::string filename);
	static bool isDirectory(std::string filename);
	static bool isReadable(std::string filename);
	static bool isFile(std::string filename);
	static File openFile(std::string filename);
	static void printFileInfo(File file);
	static void printFileContents(File file);

private:
};

#endif