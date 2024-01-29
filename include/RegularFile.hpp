/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegularFile.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:45:44 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/29 23:10:20 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REGULARFILE_HPP
#define REGULARFILE_HPP

#include "File.hpp"

namespace Hafserv
{
class RegularFile : public File
{
public:
	RegularFile();
	RegularFile(const RegularFile &other);
	RegularFile(const std::string &filename);
	RegularFile &operator=(const RegularFile &other);
	virtual ~RegularFile();

	const std::vector<std::string> &getContents() const;
	const std::string getRawContents() const;
	size_t getContentsSize() const;
	const std::string &getExtension() const;
	const std::string &getline(size_t lineNum) const;

protected:
	std::string extension;
	std::vector<std::string> contents;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::RegularFile &file);

#endif
