/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:58:13 by jinhchoi          #+#    #+#             */
/*   Updated: 2024/02/12 22:00:49 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Http/Request.hpp"
#include "Http/Response.hpp"
#include "Webserv.hpp"
#include <sys/stat.h>

namespace Hafserv
{

#define BUFFER_SIZE 100000

class Webserv;

class Connection
{
public:
	Connection(int socket, unsigned short port);
	Connection(const Connection &other);
	Connection &operator=(Connection &rhs);
	~Connection();

	bool readRequest(int fd);
	std::string configureTargetResource(std::string requestTarget);

	void buildResponseFromRequest();
	void buildGetResponse();
	void buildDeleteResponse();
	void buildErrorResponse(int statusCode);
	void build301Response(const std::string &redirectTarget);

	void buildCGIResponse(const std::string &scriptPath);
	char **makeEnvp();

	const Request &getRequest() const;
	const Server *getTargetServer() const;
	const time_t &getStartTime() const;
	const LocationConfig &getTargetLocationConfig() const;

private:
	std::string buffer;

	Request request;
	Response response;
	int socket;
	unsigned short port;

	int statusCode;
	time_t startTime;
	Server *targetServer;
	LocationConfig targetLocationConfig;
	std::string targetResource;
};

} // namespace Hafserv
#endif
