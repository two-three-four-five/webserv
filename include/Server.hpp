/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:58:56 by jinhchoi          #+#    #+#             */
/*   Updated: 2024/02/09 19:58:56 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config/ServerConfig.hpp"
#include <string>
#include <vector>

namespace Hafserv
{
class Server
{
public:
	Server(ServerConfig serverConfig);
	~Server();

	const std::vector<unsigned short> &getPorts() const;
	const std::vector<std::string> &getNames() const;
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
