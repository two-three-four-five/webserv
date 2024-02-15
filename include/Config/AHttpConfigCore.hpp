/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpConfigCore.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:26:35 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/15 15:15:48 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONFIGCORE_HPP
#define HTTPCONFIGCORE_HPP

#include <Config/ConfigException.hpp>
#include <File/ConfigFile.hpp>
#include <iostream>
#include <map>
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

	const int getClientBodyBufferSize() const;
	const bool getAutoIndex() const;
	const std::string &getRoot() const;
	const std::vector<std::string> &getIndexes() const;
	const Timeout &getTimeout() const;
	const std::map<int, std::string> &getErrorPages() const;
	const std::string &getDefaultType() const;
	const std::multimap<std::string, std::string> &getTypes() const;
	const std::vector<std::string> &getAllowMethods() const;

	void setClientBodyBufferSize(const int clientBodyBufferSize);
	void setAutoIndex(const bool autoIndex);
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

	void setHttpConfigCore(const ConfigFile::directives_t &directives);
	void setHttpConfigCore(const ConfigFile::subblocks_t &subBlocks);

private:
	int clientBodyBufferSize;
	bool autoIndex;
	std::string root;
	std::vector<std::string> indexes;
	Timeout timeouts;
	std::map<int, std::string> errorPages; // value must be between 300 and 599
	std::string defaultType;
	std::multimap<std::string, std::string> types;
	std::vector<std::string> allowMethods;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::AHttpConfigCore &conf);
std::ostream &operator<<(std::ostream &os, const Hafserv::AHttpConfigCore::Timeout &timeouts);

#endif