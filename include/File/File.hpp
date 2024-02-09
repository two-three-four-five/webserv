/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:44:36 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/09 18:00:39 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
#define FILE_HPP

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

namespace Hafserv
{
class File
{
public:
	enum FileCode
	{
		DIRECTORY = 2,
		REGULAR_FILE = 1,

		NOT_EXIST = 0,
		PERMISSION_DENIED = -1,
		UNKNOWN_ERROR = -100,
	};

	File();
	File(const File &other);
	File(const std::string &filename);
	File(int code, const std::string &name);
	File &operator=(const File &other);
	virtual ~File();

	static bool isExist(const std::string &filename);
	static bool isDirectory(const std::string &filename);
	static bool isReadable(const std::string &filename);
	static bool isRegularFile(const std::string &filename);

	int getCode() const;
	const std::string &getName() const;

protected:
	int code;
	std::string name;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::File &file);

#endif
