/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfigCore.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:26:26 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 17:03:13 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConfigCore.hpp"

using namespace Hafserv;

HttpConfigCore::HttpConfigCore() : root(), indexes() {}

HttpConfigCore::HttpConfigCore(const HttpConfigCore &other) : root(other.root), indexes(other.indexes) {}

HttpConfigCore::HttpConfigCore(const std::string &root, std::vector<std::string> &indexes)
	: root(root), indexes(indexes)
{
}

HttpConfigCore &HttpConfigCore::operator=(const HttpConfigCore &other)
{
	if (this != &other)
	{
		root = other.root;
		indexes = other.indexes;
	}
	return *this;
}

HttpConfigCore::~HttpConfigCore() {}

const std::string &HttpConfigCore::getRoot() const { return root; }

const std::vector<std::string> &HttpConfigCore::getIndexes() const { return indexes; }

void HttpConfigCore::setRoot(const std::string root) { this->root = root; }

std::ostream &operator<<(std::ostream &os, const HttpConfigCore &conf)
{
	os << "[HttpConfigCore]" << std::endl;
	os << "\troot: " << conf.getRoot() << std::endl;
	os << "\tindexes: ";
	for (size_t i = 0; i < conf.getIndexes().size(); i++)
		os << conf.getIndexes().at(i) << " ";
	return os;
}
