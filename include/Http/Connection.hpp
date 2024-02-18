/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukim2 <jukim2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:58:13 by jinhchoi          #+#    #+#             */
/*   Updated: 2024/02/18 22:28:12 by jukim2           ###   ########.fr       */
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

	void readRequest(int fd);
	std::string configureTargetResource(std::string requestTarget);
	std::string getTargetResource(std::string &requestTarget);

	void buildResponseFromRequest();
	void buildGetResponse();
	void buildDirectoryResponse();
	void buildDeleteResponse();
	void buildErrorResponse(int statusCode);
	void build301Response(const std::string &redirectTarget);

	std::string getCGIExecutable();
	void buildCGIResponse(const std::string &scriptPath);
	void writeToCGI(int fd);
	void readFromCGI(int fd, bool eof);
	char **makeEnvp();

	void sendResponse();
	void reset();

	const int &getStatusCode() const;
	const Request &getRequest() const;
	const Response &getResponse() const;
	const Server *getTargetServer() const;
	const time_t &getStartTime() const;
	const LocationConfig &getTargetLocationConfig() const;
	const int getReadPipe() const;
	const int getWritePipe() const;

private:
	Request request;
	Response response;
	int socket;
	unsigned short port;

	int statusCode;
	time_t startTime;
	Server *targetServer;
	LocationConfig targetLocationConfig;
	std::string targetResource;

	pid_t cgiPID;
	int readPipe;
	int writePipe;
	size_t wrBytes;
	size_t written;
	std::stringstream CGIOutput;
	size_t readen;
	bool end;
};

} // namespace Hafserv
#endif
