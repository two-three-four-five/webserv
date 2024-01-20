/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 15:43:53 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/20 16:10:04 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_STRING_HPP
#define UTIL_STRING_HPP

#include <sstream>
#include <string>
#include <utility>

namespace Hafserv
{
namespace util
{
namespace string
{
std::pair<bool, int> stoi(const std::string &str);

bool hasSpace(const std::string &str);

std::string toLower(const std::string &str);

} // namespace string
} // namespace util
} // namespace Hafserv

#endif