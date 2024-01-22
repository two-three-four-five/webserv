/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpConfigModule.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:56:05 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/22 22:28:17 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACONFIG_HPP
#define ACONFIG_HPP

#include "ConfigFile.hpp"
#include "HttpConfigCore.hpp"
#include "util/string.hpp"
#include <string>
#include <vector>

namespace Hafserv
{
class AHttpConfigModule
{
public:
	AHttpConfigModule();
	AHttpConfigModule(const AHttpConfigModule &other);
	AHttpConfigModule(const HttpConfigCore &core);
	AHttpConfigModule &operator=(const AHttpConfigModule &other);
	~AHttpConfigModule();

	const HttpConfigCore &getHttpConfigCore() const;
	void setHttpConfigCore(const HttpConfigCore &core);
	void setHttpConfigCore(const ConfigFile::directives_t &directives);
	void setHttpConfigCore(const ConfigFile::subblocks_t &subBlocks);

protected:
	HttpConfigCore core;
};

} // namespace Hafserv

#endif