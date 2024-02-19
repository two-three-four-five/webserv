/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhchoi <jinhchoi@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 19:59:17 by jukim2            #+#    #+#             */
/*   Updated: 2024/02/09 19:59:23 by jinhchoi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <string>
#include <vector>

#define SP 32
#define HTAB 9

std::vector<std::string> parseTransferEncoding(std::string &str);
std::vector<std::string> parseContentType(std::string &str);
bool readTransferCoding(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readTokenOrQuote(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readToken(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool isTokenChar(const char &ch);
bool readQuote(std::vector<std::string> &vec, std::string::iterator &it);
bool isQdtext(const unsigned char &ch);
bool isQuotedPair(const unsigned char &ch);
bool readParameters(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readParameter(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readOWS(std::string::iterator &it);
bool readRWS(std::string::iterator &it);
std::string readHex(std::string &str);
void printVector(std::vector<std::string> &vec);

#endif
