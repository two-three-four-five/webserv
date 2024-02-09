/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:08:09 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/10 00:12:27 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/LocationConfig.hpp"

using namespace Hafserv;

LocationConfig::LocationConfig() : AHttpConfigModule(), modifier(), pattern(), alias(), proxyPass(), cgiPath() {}

LocationConfig::LocationConfig(const LocationConfig &other)
	: AHttpConfigModule(other.core), modifier(other.modifier), pattern(other.pattern), alias(other.alias),
	  proxyPass(other.proxyPass), cgiPath(other.cgiPath)
{
}

LocationConfig::LocationConfig(const ConfigFile &block, const HttpConfigCore &core)
	: AHttpConfigModule(core), modifier(), pattern(), alias(), proxyPass(), cgiPath()
{
	/* SAMPLE LOCATION BLOCK
	 * location modifier(=, $, ^) pattern {
			# root path; 	// can exist with no alias
			alias path; 	// can exist with no root
			proxy_path URL;
			cgi_path path;
	   }
	 */
	if (block.getParameters().size() != 2)
		throw ParseError("location parameter should be two.");
	modifier = block.getParameters().at(0);
	if (modifier != "=" && modifier != "$" && modifier != "^")
		throw ParseError("unexpected location parameter: " + modifier);
	pattern = block.getParameters().at(1);

	this->setHttpConfigCore(block.getDirectives());
	this->setHttpConfigCore(block.getSubBlocks());

	ConfigFile::directives_t::const_iterator it = block.getDirectives().begin();
	bool hasAlias = false, hasRoot = false, hasProxyPass = false, hasCgiPath = false;
	for (; it != block.getDirectives().end(); it++)
	{
		std::string key = (*it).first;
		std::string value = (*it).second;
		size_t numToken = util::string::split(value, ' ').size();
		if (key == "alias")
		{
			if (hasAlias)
				throw ParseError("\"alias\" directive is duplicate");
			if (hasRoot)
				throw ParseError("\"alias\" directive is duplicate, \"root\" directive was specified earlier");
			if (numToken != 1)
				throw ParseError("invalid number of arguments in \"alias\" directive");

			hasAlias = true;
			this->alias = value;
		}
		else if (key == "proxy_pass")
		{
			if (hasProxyPass)
				throw ParseError("\"proxy_pass\" directive is duplicate");
			if (false) // TODO: check if proxy_pass is valid URL
				;

			hasProxyPass = true;
			this->proxyPass = value;
		}
		else if (key == "cgi_path")
		{
			if (hasCgiPath)
				throw ParseError("\"cgi_path\" directive is duplicate");
			if (numToken != 1)
				throw ParseError("invalid number of arguments in \"cgi_path\" directive");
			if (File(value).getCode() != File::REGULAR_FILE)
				throw ParseError("invalid path for cgi file " + value);

			hasCgiPath = true;
			this->cgiPath = value;
		}
		else if (key == "root")
		{
			if (hasAlias)
				throw ParseError("\"root\" directive is duplicate, \"alias\" directive was specified earlier");
		}
		else if (!isCoreDirective(key))
		{
			throw ParseError("unknown directive \"" + key + "\"");
		}
		else
			;
	}
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
	if (this != &other)
	{
		core = other.core;
		modifier = other.modifier;
		pattern = other.pattern;
		alias = other.alias;
		proxyPass = other.proxyPass;
		cgiPath = other.cgiPath;
	}
	return *this;
}

LocationConfig::~LocationConfig() {}

const std::string &LocationConfig::getModifier() const { return modifier; }

const std::string &LocationConfig::getPattern() const { return pattern; }

const std::string &LocationConfig::getAlias() const { return alias; }

const std::string &LocationConfig::getProxyPass() const { return proxyPass; }

const std::string &LocationConfig::getCgiPath() const { return cgiPath; }

void LocationConfig::setModifier(const std::string &modifier) { this->modifier = modifier; }

void LocationConfig::setPattern(const std::string &pattern) { this->pattern = pattern; }

void LocationConfig::setAlias(const std::string &alias) { this->alias = alias; }

void LocationConfig::setProxyPass(const std::string &proxyPass) { this->proxyPass = proxyPass; }

void LocationConfig::setCgiPath(const std::string &cgiPath) { this->cgiPath = cgiPath; }

bool LocationConfig::isMatching(const std::string &url)
{
	if (pattern.find(url) == 0)
		return true;
	else
		return false;
}

bool LocationConfig::isCoreDirective(const std::string &directive)
{
	if (directive == "root" || directive == "index" || directive == "client_body_timeout" ||
		directive == "keepalive_timeout" || directive == "send_timeout" || directive == "error_page" ||
		directive == "allow_methods")
		return true;
	else
		return false;
}

std::ostream &operator<<(std::ostream &os, const LocationConfig &conf)
{
	os << "[LocationConfig]" << std::endl;
	os << conf.getHttpConfigCore();
	os << "\tmodifier: " << conf.getModifier() << std::endl;
	os << "\tpattern: " << conf.getPattern() << std::endl;
	if (conf.getAlias().length())
		os << "\talias: " << conf.getAlias() << std::endl;
	else
		os << "\tno alias" << std::endl;
	if (conf.getProxyPass().length())
		os << "\tproxy_pass: " << conf.getProxyPass() << std::endl;
	else
		os << "\tno proxy_pass" << std::endl;
	if (conf.getCgiPath().length())
		os << "\tcgi_path: " << conf.getCgiPath();
	else
		os << "\tno cgi_pass";
	return os;
}
