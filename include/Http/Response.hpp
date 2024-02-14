/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:58:45 by jinhchoi          #+#    #+#             */
/*   Updated: 2024/02/14 14:34:02 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Http/Request.hpp"
#include <string>
#include <vector>

namespace Hafserv
{
class Response
{
public:
	enum ResponseState
	{
		Created,
		BuildingCGI,
		Ready,
		Sending,
		End
	};
	Response();
	Response(const Response &other);
	Response &operator=(const Response &rhs);
	~Response();

	void addToHeaders(std::string key, std::string value);
	void makeBody(const LocationConfig &targetLocationConfig, const std::string &requestTarget);

	void setBody(const std::string &bodyString);
	void setStatusLine(std::string statusLine);

	void setResponseBuffer();
	std::string getResponse();

	const ResponseState getResponseState() const;
	void setResponseState(ResponseState rs);
	const int getWrittenBytes() const;
	const int getResponseBytes() const;

	void send(int fd);

private:
	ResponseState responseState;

	std::string statusLine;
	HeaderMultiMap headers;
	std::string body;

	std::string responseBuffer;
	size_t writtenBytes;
	size_t responseBytes;
};

} // namespace Hafserv

#endif
