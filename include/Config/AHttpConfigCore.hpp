/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpConfigCore.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:26:35 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/19 11:02:38 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONFIGCORE_HPP
#define HTTPCONFIGCORE_HPP

#include <Config/ConfigException.hpp>
#include <File/ConfigFile.hpp>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <util/string.hpp>
#include <vector>

namespace Hafserv
{
class AHttpConfigCore
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

	AHttpConfigCore();
	AHttpConfigCore(const AHttpConfigCore &other);
	AHttpConfigCore &operator=(const AHttpConfigCore &other);
	~AHttpConfigCore();

	const std::set<std::string> &getAllowMethods() const;
	const bool getAutoIndex() const;
	const std::string &getRoot() const;
	const std::set<std::string> &getIndexes() const;
	const std::map<int, std::string> &getErrorPages() const;
	const Timeout &getTimeout() const;
	const std::string &getDefaultType() const;
	const std::multimap<std::string, std::string> &getTypes() const;
	const int getClientMaxBodySize() const;

	void setAllowMethods(const std::set<std::string> &allowMethods);
	void setAutoIndex(const bool autoIndex);
	void setRoot(const std::string &root);
	void setIndexes(const std::set<std::string> &indexes);
	void setErrorPages(const std::map<int, std::string> &errorPages);
	void setTimeouts(const Timeout &timeouts);
	void setClientHeaderTimeout(int timeout);
	void setClientBodyTimeout(int timeout);
	void setKeepAliveTimeout(int timeout);
	void setSendTimeout(int timeout);
	void setDefaultType(const std::string &defaultType);
	void setTypes(const std::multimap<std::string, std::string> &types);
	void setClientMaxBodySize(const int clientMaxBodySize);

	const bool addAllowMethod(const std::string &method);
	const bool addIndex(const std::string &index);
	void addErrorPage(int errorCode, const std::string &uri);
	void addType(const std::string &type, const std::string &extension);

	const bool isAllowedMethod(const std::string &method) const;

	void setHttpConfigCore(const ConfigFile::directives_t &directives) throw(std::logic_error);
	void setHttpConfigCore(const ConfigFile::subblocks_t &subBlocks) throw();

private:
	std::set<std::string> allowMethods;

	bool autoIndex;
	std::string root;
	std::set<std::string> indexes;
	std::map<int, std::string> errorPages; // value must be between 300 and 599

	Timeout timeouts;
	std::string defaultType;
	std::multimap<std::string, std::string> types;
	long long clientMaxBodySize;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::AHttpConfigCore &conf);
std::ostream &operator<<(std::ostream &os, const Hafserv::AHttpConfigCore::Timeout &timeouts);

#endif