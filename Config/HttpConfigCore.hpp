/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConfigCore.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:26:35 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 17:10:16 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCONFIGCORE_HPP
#define HTTPCONFIGCORE_HPP

#include <iostream>
#include <string>
#include <vector>

namespace Hafserv
{
class HttpConfigCore
{
public:
	HttpConfigCore();
	HttpConfigCore(const HttpConfigCore &other);
	HttpConfigCore(const std::string &root, std::vector<std::string> &indexes);
	HttpConfigCore &operator=(const HttpConfigCore &other);
	~HttpConfigCore();

	const std::string &getRoot() const;
	const std::vector<std::string> &getIndexes() const;
	void setRoot(const std::string root);
	// TODO: setIndexes
private:
	std::string root;
	std::vector<std::string> indexes;
};

} // namespace Hafserv

std::ostream &operator<<(std::ostream &os, const Hafserv::HttpConfigCore &conf);

#endif