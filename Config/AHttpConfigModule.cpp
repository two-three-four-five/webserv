/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpConfigModule.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:56:11 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/20 17:06:37 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AHttpConfigModule.hpp"

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
	ConfigFile::directives_t::const_iterator it = directives.begin();
	for (; it != directives.end(); it++)
	{
		std::string key = (*it).first;
		std::string value = (*it).second;
		if (key == "root")
			core.setRoot(value);
		else if (key == "index")
			;
		else if (key == "client_header_timeout")
		{
			if (util::string::stoi(value).first)
				core.setClientHeaderTimeout(util::string::stoi(value).second);
		}
		else if (key == "client_body_timeout")
		{
			if (util::string::stoi(value).first)
				core.setClientBodyTimeout(util::string::stoi(value).second);
		}
		else if (key == "keepalive_timeout")
		{
			if (util::string::stoi(value).first)
				core.setKeepAliveTimeout(util::string::stoi(value).second);
		}
		else if (key == "send_timeout")
		{
			if (util::string::stoi(value).first)
				core.setSendTimeout(util::string::stoi(value).second);
		}
		else if (key == "error_pages")
			;
		else
			;
	}
}
