/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AHttpConfigModule.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 14:56:11 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/19 16:30:52 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AHttpConfigModule.hpp"

using namespace Hafserv;

AHttpConfigModule::AHttpConfigModule() : core() {}

AHttpConfigModule::AHttpConfigModule(const AHttpConfigModule &other) : core(other.core) {}

AHttpConfigModule::AHttpConfigModule(const HttpConfigCore &core) : core(core) {}

AHttpConfigModule &AHttpConfigModule::operator=(const AHttpConfigModule &other)
{
	if (this != &other)
	{
		core = other.core;
	}
	return *this;
}

AHttpConfigModule::~AHttpConfigModule() {}

const HttpConfigCore &AHttpConfigModule::getHttpConfigCore() const { return core; }

void AHttpConfigModule::setHttpConfigCore(const HttpConfigCore &core) { this->core = core; }