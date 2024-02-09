/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:19:48 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/08 13:49:48 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGEXCEPTION_HPP
#define CONFIGEXCEPTION_HPP

#include <stdexcept>
#include <string>

class ParseError : public std::logic_error
{
public:
	ParseError(const std::string &msg) : std::logic_error(msg){};
	virtual ~ParseError() throw(){};
};

class IncludeError : public std::logic_error
{
public:
	IncludeError(const std::string &msg) : std::logic_error(msg){};
	virtual ~IncludeError() throw(){};
};

#endif