/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 15:43:53 by gyoon             #+#    #+#             */
/*   Updated: 2024/02/16 20:55:20 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_STRING_HPP
#define UTIL_STRING_HPP

#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace Hafserv
{
namespace util
{
namespace string
{
std::pair<int, bool> stoi(const std::string &str);

std::pair<unsigned short, bool> stous(const std::string &str);

std::string itos(const int n);

std::vector<std::string> split(const std::string &str, char delimiter);

bool hasSpace(const std::string &str);

std::string toLower(const std::string &str);

int hexStrToDecInt(const std::string &hex);

} // namespace string
} // namespace util
} // namespace Hafserv

#endif
