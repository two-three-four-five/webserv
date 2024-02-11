/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:19:48 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/11 19:34:18 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGEXCEPTION_HPP
#define CONFIGEXCEPTION_HPP

#include <stdexcept>
#include <string>

class NoBraceError : public std::logic_error
{
public:
	NoBraceError(const std::string &directive) : std::logic_error("directive \"" + directive + "\" has no opening {"){};
	virtual ~NoBraceError() throw(){};
};

class NoSemicolonError : public std::logic_error
{
public:
	NoSemicolonError(const std::string &directive)
		: std::logic_error("directive \"" + directive + "\"  is not terminated by \";\""){};
	virtual ~NoSemicolonError() throw(){};
};

class UnknownDirectiveError : public std::logic_error
{
public:
	UnknownDirectiveError(const std::string &directive) : std::logic_error("unknown directive \"" + directive + "\""){};
	virtual ~UnknownDirectiveError() throw(){};
};

class DuplicateDirectiveError : public std::logic_error
{
public:
	DuplicateDirectiveError(const std::string &directive) : std::logic_error("\"" + directive + "\" is duplicated"){};
	DuplicateDirectiveError(const std::string &d1, const std::string &d2)
		: std::logic_error("\"" + d1 + "\" is duplicated, \"" + d2 + "\" directive was specified earlier"){};
	virtual ~DuplicateDirectiveError() throw(){};
};

class DisallowDirectiveError : public std::logic_error
{
public:
	DisallowDirectiveError(const std::string &directive)
		: std::logic_error("\"" + directive + "\" directive is not allowed here"){};
	virtual ~DisallowDirectiveError() throw(){};
};

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