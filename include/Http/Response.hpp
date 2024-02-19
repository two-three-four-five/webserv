/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:58:45 by jinhchoi          #+#    #+#             */
/*   Updated: 2024/02/19 14:13:18 by gyoon            ###   ########.fr       */
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
    void removeHeaders(std::string key);
    void makeBody(const LocationConfig &targetLocationConfig,
                  const std::string &requestTarget);
    void makeErrorBody(int statusCode);
    std::string generateDate();

    void setBody(const std::string &bodyString);
    void setStatusLine(std::string statusLine);

    void setResponseBuffer();

    ResponseState getResponseState() const;
    void setResponseState(ResponseState rs);
    int getWrittenBytes() const;
    int getResponseBytes() const;

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
