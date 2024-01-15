#ifndef PARSE_HPP
#define PARSE_HPP

#include <iostream>
#include <string>
#include <vector>

bool readParameters(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readToken(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readParameter(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readPadding(std::string::iterator &it);
bool isTokenChar(const char &ch);
void printVector(std::vector<std::string> &vec);
std::vector<std::string> parseContentType(std::string &str);
bool readQuote(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readTokenOrQuote(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);

#endif
