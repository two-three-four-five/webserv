/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directory.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:05:19 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/09 19:57:43 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include "File/File.hpp"
#include "dirent.h"
#include <iostream>
#include <utility>
#include <vector>

namespace Hafserv
{
class Directory : public File
{
public:
	Directory();
	Directory(const Directory &other);
	Directory(const std::string &filename);
	Directory &operator=(const Directory &other);
	virtual ~Directory();

	const std::vector<std::pair<int, std::string> > getContents() const;

	void printContents() const;

private:
	std::vector<std::pair<int, std::string> > contents;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::Directory &file);

#endif
