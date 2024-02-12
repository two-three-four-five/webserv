/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpConfigModule.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:56:11 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/12 13:25:29 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/AHttpConfigModule.hpp"

using namespace Hafserv;

AHttpConfigModule::AHttpConfigModule() : core() { core.addAllowMethod("GET"); }

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
	std::vector<std::string> params;
	size_t numToken;
	bool hasRoot = false;
	ConfigFile::directives_t::const_iterator it = directives.begin();
	for (; it != directives.end(); it++)
	{
		const std::string &key = (*it).first;
		const std::string &value = (*it).second;

		params = util::string::split(value, ' ');
		numToken = params.size();
		if (key == "root")
		{
			if (hasRoot)
				throw DuplicateDirectiveError(key);
			else if (numToken != 1)
				throw InvalidNumberArgumentError(key);

			core.setRoot(value);
			hasRoot = true;
		}
		else if (key == "index")
		{
			std::vector<std::string> newIndexes;
			for (size_t i = 0; i < params.size(); i++)
				newIndexes.push_back(params[i]);
			core.setIndexes(newIndexes);
		}
		else if (key == "client_header_timeout")
		{
			if (numToken != 1)
				throw InvalidNumberArgumentError(key);
			else if (!util::string::stoi(value).first)
				throw InvalidArgumentError(key);

			core.setClientHeaderTimeout(util::string::stoi(value).second);
		}
		else if (key == "client_body_timeout")
		{
			if (numToken != 1)
				throw InvalidNumberArgumentError(key);
			else if (!util::string::stoi(value).first)
				throw InvalidArgumentError(key);
			core.setClientBodyTimeout(util::string::stoi(value).second);
		}
		else if (key == "keepalive_timeout")
		{
			if (numToken != 1)
				throw InvalidNumberArgumentError(key);
			else if (!util::string::stoi(value).first)
				throw InvalidArgumentError(key);
			core.setKeepAliveTimeout(util::string::stoi(value).second);
		}
		else if (key == "send_timeout")
		{
			if (numToken != 1)
				throw InvalidNumberArgumentError(key);
			else if (!util::string::stoi(value).first)
				throw InvalidArgumentError(key);
			core.setSendTimeout(util::string::stoi(value).second);
		}
		else if (key == "error_page")
		{
			// what if error_page option duplicate?
			if (params.size() == 1)
				throw InvalidArgumentError(key, value);
			for (size_t i = 0; i < params.size() - 1; i++)
			{
				if (util::string::stoi(params[i]).first && 300 <= util::string::stoi(params[i]).second &&
					util::string::stoi(params[i]).second < 600)
					core.addErrorPage(util::string::stoi(params[i]).second, params.back());
				else
					; // error
			}
		}
		else if (key == "allow_methods")
		{
			for (size_t i = 0; i < numToken; i++)
			{
				if (params[i] != "GET" && params[i] != "POST" && params[i] != "DELETE")
					throw InvalidArgumentError(key, value);
				core.addAllowMethod(params[i]);
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