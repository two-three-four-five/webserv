/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directory.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:05:07 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/09 20:01:14 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File/Directory.hpp"

using namespace Hafserv;

Directory::Directory() : File(), contents() {}

Directory::Directory(const Directory &other) : File(other.code, other.name), contents(other.contents) {} // File(other)

Directory::Directory(const std::string &filename) : File(filename), contents()
{
	if (code != File::DIRECTORY)
		return;

	DIR *dir = opendir(filename.c_str());
	if (dir == NULL)
	{
		code = File::UNKNOWN_ERROR;
		return;
	}
	for (struct dirent *dp = readdir(dir); dp != NULL; dp = readdir(dir))
	{
		std::string curr = "./" + filename + "/" + std::string(dp->d_name);
		File file = File(curr.c_str());
		contents.push_back(std::make_pair(file.getCode(), dp->d_name));
	}

	closedir(dir);
}

Directory &Directory::operator=(const Directory &other)
{
	if (this != &other)
	{
		code = other.code;
		name = other.name;
		contents = other.contents;
	}
	return *this;
}

Directory::~Directory() {}

const std::vector<std::pair<int, std::string> > Directory::getContents() const { return contents; }

std::ostream &operator<<(std::ostream &os, const Directory &file)
{
	os << "code: " << file.getCode() << std::endl;
	os << "name: " << file.getName() << std::endl;
	if (file.getCode() == File::DIRECTORY)
	{
		std::cout << "count: " << file.getContents().size() << std::endl;
		for (size_t i = 0; i < file.getContents().size(); i++)
			os << file.getContents()[i].first << "\t" << file.getContents()[i].second << std::endl;
		return os;
	}
	else
		std::cout << "not a directory. error code : " << file.getCode();
	return os;
}
