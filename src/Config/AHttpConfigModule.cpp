/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpConfigModule.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:56:11 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/09 19:55:23 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/AHttpConfigModule.hpp"

using namespace Hafserv;

AHttpConfigModule::AHttpConfigModule() : core() {}

AHttpConfigModule::AHttpConfigModule(const AHttpConfigModule &other) : core(other.core) {}

AHttpConfigModule::AHttpConfigModule(const HttpConfigCore &core) : core(core) {}

AHttpConfigModule &AHttpConfigModule::operator=(const AHttpConfigModule &other)
{
	if (this != &other)
	{
		core = other.core;
	}
	return *this;
}

AHttpConfigModule::~AHttpConfigModule() {}

const HttpConfigCore &AHttpConfigModule::getHttpConfigCore() const { return core; }

void AHttpConfigModule::setHttpConfigCore(const HttpConfigCore &core) { this->core = core; }

void AHttpConfigModule::setHttpConfigCore(const ConfigFile::directives_t &directives)
{
	std::string key, value;
	std::vector<std::string> params;
	ConfigFile::directives_t::const_iterator it = directives.begin();
	bool hasRoot = false;
	for (; it != directives.end(); it++)
	{
		key = (*it).first;
		value = (*it).second;
		size_t numToken = util::string::split(value, ' ').size();
		if (key == "root")
		{
			if (hasRoot)
				throw ParseError("\"root\" directive is duplicate");
			if (numToken != 1)
				throw ParseError("invalid number of arguments in \"root\" directive");
			hasRoot = true;
			core.setRoot(value);
		}
		else if (key == "index")
		{
			params = util::string::split(value, ' ');
			for (size_t i = 0; i < params.size(); i++)
				core.addIndex(params[i]);
		}
		else if (key == "client_header_timeout")
		{
			if (numToken != 1)
				throw ParseError("invalid number of arguments in \"client_header_timeout\" directive");
			if (!util::string::stoi(value).first)
				throw ParseError("\"client_header_timeout\" directive invalid value");

			core.setClientHeaderTimeout(util::string::stoi(value).second);
		}
		else if (key == "client_body_timeout")
		{
			if (numToken != 1)
				throw ParseError("invalid number of arguments in \"client_body_timeout\" directive");
			if (!util::string::stoi(value).first)
				throw ParseError("\"client_body_timeout\" directive invalid value");
			core.setClientBodyTimeout(util::string::stoi(value).second);
		}
		else if (key == "keepalive_timeout")
		{
			if (numToken != 1)
				throw ParseError("invalid number of arguments in \"keepalive_timeout\" directive");
			if (!util::string::stoi(value).first)
				throw ParseError("\"keepalive_timeout\" directive invalid value");
			core.setKeepAliveTimeout(util::string::stoi(value).second);
		}
		else if (key == "send_timeout")
		{
			if (numToken != 1)
				throw ParseError("invalid number of arguments in \"send_timeout\" directive");
			if (!util::string::stoi(value).first)
				throw ParseError("\"send_timeout\" directive invalid value");
			core.setSendTimeout(util::string::stoi(value).second);
		}
		else if (key == "error_page")
		{
			// what if error_page option duplicate?
			params = util::string::split(value, ' ');
			if (params.size() == 1)
				; // error
			for (size_t i = 0; i < params.size() - 1; i++)
			{
				if (util::string::stoi(params[i]).first && 300 <= util::string::stoi(params[i]).second &&
					util::string::stoi(params[i]).second < 600)
					core.addErrorPage(util::string::stoi(params[i]).second, params.back());
				else
					; // error
			}
		}
		else
			;
	}
}

void AHttpConfigModule::setHttpConfigCore(const ConfigFile::subblocks_t &subBlocks)
{
	std::string subBlockName, type, extension;
	for (size_t i = 0; i < subBlocks.size(); i++)
	{
		subBlockName = subBlocks.at(i).getBlockDirective();
		if (subBlockName == "types")
		{
			ConfigFile::directives_t::const_iterator it = subBlocks.at(i).getDirectives().begin();
			for (; it != subBlocks.at(i).getDirectives().end(); it++)
			{
				type = (*it).first;
				extension = (*it).second;
				std::vector<std::string> extensions = util::string::split(extension, ' ');
				for (size_t j = 0; j < extensions.size(); j++)
					core.addType(extensions.at(j), type);
			}
		}
	}
}

bool AHttpConfigModule::isCoreDirective(const std::string &directive)
{
	if (directive == "root" || directive == "index" || directive == "client_header_timeout" ||
		directive == "client_body_timeout" || directive == "keepalive_timeout" || directive == "send_timeout" ||
		directive == "error_page")
		return true;
	else
		return false;
}
