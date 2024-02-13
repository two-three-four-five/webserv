/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:44:36 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/13 16:01:27 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
#define FILE_HPP

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define F_ERR_MSG_OK "file is okay"
#define F_ERR_MSG_NOT_EXIST "file does not exist."
#define F_ERR_MSG_NO_PERMISSION "file has no permission"
#define F_ERR_MSG_TYPE_NOT_MATCHING "file type is not matching"
#define F_ERR_MSG_UNKNOWN "file unknown error"

namespace Hafserv
{
class File
{
public:
	enum ErrorCode
	{
		FILE_OK = 0,
		FILE_NOT_EXIST = 1,
		FILE_NO_PERMISSION = 2,
		FILE_TYPE_NOT_MATCHING = 3,
		FILE_UNKNOWN = 4,
	};

	File();
	File(const File &other);
	File(const std::string &name);
	File &operator=(const File &other);
	virtual ~File();

	const bool exist() const;
	const bool error() const;
	const bool valid() const;

	const unsigned short getErrorCode() const;
	const std::string getErrorMsg() const;
	const std::string &getName() const;
	const struct stat &getFileStat() const;
	const mode_t &getFileMode() const;
	const off_t &getFileSize() const;

	const bool isDirectory() const;
	const bool isRegularFile() const;
	const bool isSocket() const;

	const bool isReadable() const;
	const bool isWritable() const;
	const bool isExecutable() const;

protected:
	unsigned short errorCode;
	std::string name;
	struct stat fileStat;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::File &file);

#endif
