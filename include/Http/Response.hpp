/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:58:45 by jinhchoi          #+#    #+#             */
/*   Updated: 2024/02/09 21:18:29 by jinhchoi         ###   ########.fr       */
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
	Response();
	Response(const Response &other);
	Response &operator=(Response &rhs);
	~Response();

	void addToHeaders(std::string key, std::string value);
	void makeBody(const LocationConfig &targetLocationConfig, const std::string &requestTarget);

	void setBody(const std::string &bodyString);
	void setStatusLine(std::string statusLine);
	std::string getResponse();

private:
	std::string statusLine;
	HeaderMultiMap headers;
	std::string body;
};

} // namespace Hafserv

#endif
