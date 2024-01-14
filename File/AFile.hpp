/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AFile.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:44:36 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 13:45:16 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AFILE_HPP
#define AFILE_HPP

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

namespace Hafserv
{
class AFile
{
public:
	enum FileCode
	{
		UNKNOWN_ERROR = -1,
		NOT_EXIST = 0,
		REGULAR_FILE = 1,
		IS_A_DIRECTORY = 2,
		PERMISSION_DENIED = 3,
	};

	AFile();
	AFile(const AFile &other);
	AFile(const std::string &filename);
	AFile &operator=(const AFile &other);
	virtual ~AFile();

	static bool isExist(const std::string &filename);
	static bool isDirectory(const std::string &filename);
	static bool isReadable(const std::string &filename);
	static bool isRegularFile(const std::string &filename);

	virtual void printProperty() const = 0;
	virtual void print() const = 0;

protected:
	int code;
	std::string name;
};

} // namespace Hafserv

#endif