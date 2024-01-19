/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpConfigModule.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:56:05 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 16:33:14 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACONFIG_HPP
#define ACONFIG_HPP

#include "HttpConfigCore.hpp"
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

protected:
	HttpConfigCore core;
};

} // namespace Hafserv

#endif