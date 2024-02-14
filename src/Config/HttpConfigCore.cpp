/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfigCore.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:26:26 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/14 20:55:24 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config/HttpConfigCore.hpp"

using namespace Hafserv;

HttpConfigCore::Timeout::Timeout() : clientHeader(60), clientBody(60), keepAlive(75), send(60) {}

HttpConfigCore::HttpConfigCore()
	: clientBodyBufferSize(16384), autoIndex(false), root("html"), indexes(), timeouts(), errorPages(),
	  defaultType("text/plain"), types(), allowMethods()
{
	indexes.push_back("index.html");
}

HttpConfigCore::HttpConfigCore(const HttpConfigCore &other)
	: clientBodyBufferSize(other.clientBodyBufferSize), autoIndex(other.autoIndex), root(other.root),
	  indexes(other.indexes), timeouts(other.timeouts), errorPages(other.errorPages), defaultType(other.defaultType),
	  types(other.types), allowMethods(other.allowMethods)
{
}

HttpConfigCore &HttpConfigCore::operator=(const HttpConfigCore &other)
{
	if (this != &other)
	{
		clientBodyBufferSize = other.clientBodyBufferSize;
		autoIndex = other.autoIndex;
		root = other.root;
		indexes = other.indexes;
		timeouts = other.timeouts;
		errorPages = other.errorPages;
		defaultType = other.defaultType;
		types = other.types;
		allowMethods = other.allowMethods;
	}
	return *this;
}

HttpConfigCore::~HttpConfigCore() {}

const int HttpConfigCore::getClientBodyBufferSize() const { return clientBodyBufferSize; }
const bool HttpConfigCore::getAutoIndex() const { return autoIndex; }
const std::string &HttpConfigCore::getRoot() const { return root; }
const std::vector<std::string> &HttpConfigCore::getIndexes() const { return indexes; }
const HttpConfigCore::Timeout &HttpConfigCore::getTimeout() const { return timeouts; }
const std::map<int, std::string> &HttpConfigCore::getErrorPages() const { return errorPages; }
const std::string &HttpConfigCore::getDefaultType() const { return defaultType; }
const std::multimap<std::string, std::string> &HttpConfigCore::getTypes() const { return types; }
const std::vector<std::string> &HttpConfigCore::getAllowMethods() const { return allowMethods; }

void HttpConfigCore::setClientBodyBufferSize(const int clientBodyBufferSize)
{
	this->clientBodyBufferSize = clientBodyBufferSize;
}
void HttpConfigCore::setAutoIndex(const bool autoIndex) { this->autoIndex = autoIndex; }
void HttpConfigCore::setRoot(const std::string &root) { this->root = root; }
void HttpConfigCore::setIndexes(const std::vector<std::string> &indexes) { this->indexes = indexes; }
void HttpConfigCore::setTimeouts(const Timeout &timeouts) { this->timeouts = timeouts; }
void HttpConfigCore::setClientHeaderTimeout(int timeout) { this->timeouts.clientHeader = timeout; }
void HttpConfigCore::setClientBodyTimeout(int timeout) { this->timeouts.clientBody = timeout; }
void HttpConfigCore::setKeepAliveTimeout(int timeout) { this->timeouts.keepAlive = timeout; }
void HttpConfigCore::setSendTimeout(int timeout) { this->timeouts.send = timeout; }
void HttpConfigCore::setErrorPages(const std::map<int, std::string> &errorPages) { this->errorPages = errorPages; }
void HttpConfigCore::setDefaultType(const std::string &defaultType) { this->defaultType = defaultType; }
void HttpConfigCore::setTypes(const std::multimap<std::string, std::string> &types) { this->types = types; }
void HttpConfigCore::setAllowMethods(const std::vector<std::string> allowMethods) { this->allowMethods = allowMethods; }

void HttpConfigCore::addErrorPage(int errorCode, const std::string &uri)
{
	errorPages.insert(std::make_pair(errorCode, uri));
}

void HttpConfigCore::addIndex(const std::string &index)
{
	if (std::find(indexes.begin(), indexes.end(), index) != indexes.end())
		indexes.push_back(index);
}

void HttpConfigCore::addType(const std::string &type, const std::string &extension)
{
	types.insert(std::make_pair(type, extension));
}

void HttpConfigCore::addAllowMethod(const std::string &method)
{
	if (std::find(allowMethods.begin(), allowMethods.end(), method) == allowMethods.end())
		allowMethods.push_back(method);
}

std::ostream &operator<<(std::ostream &os, const HttpConfigCore &conf)
{
	os << "\t[HttpConfigCore]" << std::endl;
	os << "\t\tdefault_types: " << conf.getDefaultType() << std::endl;
	os << "\t\tautoindex: " << (conf.getAutoIndex() ? "on" : "off") << std::endl;
	os << "\t\troot: " << conf.getRoot() << std::endl;
	os << "\t\tindexes: ";
	for (size_t i = 0; i < conf.getIndexes().size(); i++)
		os << conf.getIndexes().at(i) << " ";
	os << std::endl;

	os << conf.getTimeout();
	os << std::endl;

	// errorPages
	if (conf.getErrorPages().size())
	{
		os << "\t\terror code: ";
		std::map<int, std::string>::const_iterator it = conf.getErrorPages().begin();
		for (; it != conf.getErrorPages().end(); it++)
			os << (*it).first << "=" << (*it).second << ", ";
		if (conf.getErrorPages().size())
			os << std::endl;
	}

	// types
	if (conf.getTypes().size())
	{
		os << "\t\textension: ";
		size_t i = 0;
		std::map<std::string, std::string>::const_iterator it2 = conf.getTypes().begin();
		for (; it2 != conf.getTypes().end() && i < 3; it2++)
		{
			os << (*it2).first << "=" << (*it2).second << ", ";
			i++;
		}
		if (it2 != conf.getTypes().end())
			os << " ..." << std::endl;
		else
			os << std::endl;
	}

	if (conf.getAllowMethods().size())
	{
		os << "\t\tallow methods: ";
		for (size_t i = 0; i < conf.getAllowMethods().size(); i++)
			os << conf.getAllowMethods()[i] << ", ";
		os << std::endl;
	}

	return os;
}

std::ostream &operator<<(std::ostream &os, const HttpConfigCore::Timeout &timeouts)
{
	os << "\t\ttimeouts: clientHeader=" << timeouts.clientHeader;
	os << ", clientBody=" << timeouts.clientBody;
	os << ", keepalive=" << timeouts.keepAlive;
	os << ", send=" << timeouts.send;
	return os;
}
