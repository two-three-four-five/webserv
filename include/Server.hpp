/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:58:56 by jinhchoi          #+#    #+#             */
/*   Updated: 2024/02/16 19:46:55 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config/ServerConfig.hpp"
#include <set>
#include <string>

namespace Hafserv
{
class Server
{
public:
	Server(ServerConfig serverConfig);
	~Server();

	const std::set<unsigned short> &getPorts() const;
	const std::set<std::string> &getNames() const;
	const ServerConfig &getServerConfig() const;
	/*
		string -> Request
		parseRequest
		makeResponse
		sendResponse
	*/
private:
	ServerConfig serverConfig;
};
} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::Server &server);

#endif
