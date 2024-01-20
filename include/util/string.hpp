/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 15:43:53 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/20 23:03:24 by gyoon            ###   ########.fr       */
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
std::pair<bool, int> stoi(const std::string &str);

std::vector<std::string> split(const std::string &str, char delimiter);

bool hasSpace(const std::string &str);

std::string toLower(const std::string &str);

} // namespace string
} // namespace util
} // namespace Hafserv

#endif