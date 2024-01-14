/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegularFile.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 13:45:44 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 13:46:09 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REGULARFILE_HPP
#define REGULARFILE_HPP

#include "AFile.hpp"

namespace Hafserv
{
class RegularFile : public AFile
{
public:
	RegularFile();
	RegularFile(const RegularFile &other);
	RegularFile(const std::string &filename);
	RegularFile &operator=(const RegularFile &other);
	virtual ~RegularFile();

	virtual void printProperty() const;
	virtual void print() const;

protected:
	std::string extension;
	std::vector<std::string> contents;
};

} // namespace Hafserv

#endif