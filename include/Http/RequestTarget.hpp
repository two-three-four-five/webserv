/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTarget.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:58:25 by jinhchoi          #+#    #+#             */
/*   Updated: 2024/02/09 19:58:25 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTTARGET_HPP
#define REQUESTTARGET_HPP

#include <string>

namespace Hafserv
{

class RequestTarget
{
public:
	RequestTarget(std::string requestTarget);
	~RequestTarget();

private:
	std::string targetURI;
	std::string queryString;
};

} // namespace Hafserv

#endif
