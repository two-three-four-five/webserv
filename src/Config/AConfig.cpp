/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AConfig.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:46:49 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/11 19:43:53 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/AConfig.hpp"

using namespace Hafserv;

std::set<std::string> AConfig::simpleDirectives;
std::set<std::string> AConfig::blockDirectives;

AConfig::AConfig()
{
	if (simpleDirectives.empty())
		initSimpleDirectives();
	if (blockDirectives.empty())
		initBlockDirectives();
}

AConfig::AConfig(const AConfig &other) {}

AConfig &AConfig::operator=(const AConfig &other)
{
	if (this != &other)
	{
	}
	return *this;
}

AConfig::~AConfig() {}

// void initDirectives() {}

void AConfig::initSimpleDirectives()
{
	/*		WebservConfig		*/
	simpleDirectives.insert("user");
	simpleDirectives.insert("worker_processes");
	simpleDirectives.insert("error_log");
	simpleDirectives.insert("pid");
	simpleDirectives.insert("worker_rlimit_nofile");

	/*		EventConfig			*/
	simpleDirectives.insert("worker_connections");

	/*		HttpConfig			*/
	simpleDirectives.insert("default_type");
	simpleDirectives.insert("sendfile");

	/*		ServerConfig		*/
	simpleDirectives.insert("listen");
	simpleDirectives.insert("server_name");

	/*		LocationConfig		*/
	simpleDirectives.insert("alias");
	simpleDirectives.insert("proxy_pass");
	simpleDirectives.insert("cgi_path");

	/*		HttpConfigCore		*/
	simpleDirectives.insert("root");
	simpleDirectives.insert("index");
	simpleDirectives.insert("client_header_timeout");
	simpleDirectives.insert("client_body_timeout");
	simpleDirectives.insert("keepalive_timeout");
	simpleDirectives.insert("send_timeout");
	simpleDirectives.insert("error_page");
	simpleDirectives.insert("allow_methods");
}

void AConfig::initBlockDirectives()
{
	blockDirectives.insert("events");
	blockDirectives.insert("http");
	blockDirectives.insert("server");
	blockDirectives.insert("location");
	blockDirectives.insert("types");
}
