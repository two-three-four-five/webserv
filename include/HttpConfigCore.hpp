/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfigCore.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:26:35 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/22 22:41:33 by gyoon            ###   ########.fr       */
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
	const std::multimap<std::string, std::string> &getTypes() const;

	void setRoot(const std::string &root);
	void setIndexes(const std::vector<std::string> &indexes);
	void setTimeouts(const Timeout &timeouts);
	void setClientHeaderTimeout(int timeout);
	void setClientBodyTimeout(int timeout);
	void setKeepAliveTimeout(int timeout);
	void setSendTimeout(int timeout);
	void setErrorPages(const std::map<int, std::string> &errorPages);
	void setTypes(const std::multimap<std::string, std::string> &types);

	void addIndex(const std::string &index);
	void addErrorPage(int errorCode, const std::string &uri);
	void addType(const std::string &type, const std::string &extension);

	std::string root;
	std::vector<std::string> indexes;
	Timeout timeouts;
	std::map<int, std::string> errorPages; // value must be between 300 and 599
	std::multimap<std::string, std::string> types;

private:
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::HttpConfigCore &conf);
std::ostream &operator<<(std::ostream &os, const Hafserv::HttpConfigCore::Timeout &timeouts);

#endif