/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfigCore.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:26:35 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/12 13:02:03 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONFIGCORE_HPP
#define HTTPCONFIGCORE_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace Hafserv
{
class HttpConfigCore
{
public:
	struct Timeout
	{
		int clientHeader;
		int clientBody;
		int keepAlive;
		int send;

		Timeout();
	};

	HttpConfigCore();
	HttpConfigCore(const HttpConfigCore &other);
	HttpConfigCore &operator=(const HttpConfigCore &other);
	~HttpConfigCore();

	const std::string &getRoot() const;
	const std::vector<std::string> &getIndexes() const;
	const Timeout &getTimeout() const;
	const std::map<int, std::string> &getErrorPages() const;
	const std::string &getDefaultType() const;
	const std::multimap<std::string, std::string> &getTypes() const;
	const std::vector<std::string> &getAllowMethods() const;

	void setRoot(const std::string &root);
	void setIndexes(const std::vector<std::string> &indexes);
	void setTimeouts(const Timeout &timeouts);
	void setClientHeaderTimeout(int timeout);
	void setClientBodyTimeout(int timeout);
	void setKeepAliveTimeout(int timeout);
	void setSendTimeout(int timeout);
	void setErrorPages(const std::map<int, std::string> &errorPages);
	void setDefaultType(const std::string &defaultType);
	void setTypes(const std::multimap<std::string, std::string> &types);
	void setAllowMethods(const std::vector<std::string> allowMethods);

	void addIndex(const std::string &index);
	void addErrorPage(int errorCode, const std::string &uri);
	void addType(const std::string &type, const std::string &extension);
	void addAllowMethod(const std::string &method);

	std::string root;
	std::vector<std::string> indexes;
	Timeout timeouts;
	std::map<int, std::string> errorPages; // value must be between 300 and 599
	std::string defaultType;
	std::multimap<std::string, std::string> types;
	std::vector<std::string> allowMethods;

private:
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::HttpConfigCore &conf);
std::ostream &operator<<(std::ostream &os, const Hafserv::HttpConfigCore::Timeout &timeouts);

#endif