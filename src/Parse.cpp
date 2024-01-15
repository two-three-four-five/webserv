#include <iostream>
#include <map>
#include <string>
#include <vector>

bool readParameters(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readToken(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readParameter(std::vector<std::string> &vec, std::string &str, std::string::iterator &it);
bool readPadding(std::string::iterator &it);
bool isTokenChar(const char &ch);

void printVector(std::vector<std::string> &vec)
{
	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		std::cout << *it << "   ";
	}
}

std::vector<std::string> parseContentType(std::string &str)
{
	std::vector<std::string> vec;
	std::string::iterator it = str.begin();

	if (readToken(vec, str, it) && (*it++ == '/') && readToken(vec, str, it) && readParameters(vec, str, it))
	{
		printVector(vec);
		return (vec);
	}
	else
	{
		// throw error
		return (vec);
	}
}

bool readToken(std::vector<std::string> &vec, std::string &str, std::string::iterator &it)
{
	std::string::iterator initIt = it;
	while (isTokenChar(*it))
		it++;
	vec.push_back(str.substr(initIt - str.begin(), it - initIt));
	return (initIt != it);
}

bool isTokenChar(const char &ch)
{
	if (std::isalnum(ch) || ch == '!' || ch == '#' || ch == '$' || ch == '%' || ch == '&' || ch == '\'' || ch == '*' ||
		ch == '+' || ch == '-' || ch == '.' || ch == '^' || ch == '_' || ch == '`' || ch == '|' || ch == '~')
		return (true);
	else
		return (false);
}

void readTokenOrQuote(std::vector<std::string> &vec, std::string &str, std::string::iterator &it) {}

bool readParameters(std::vector<std::string> &vec, std::string &str, std::string::iterator &it)
{
	std::string name, value;

	while (str.end() - it != 2 && *it != '\r' && *(it + 1) != '\n')
	{
		if (!readPadding(it))
			return (false);
		readParameter(vec, str, it);
	}
	return (true);
}

bool readParameter(std::vector<std::string> &vec, std::string &str, std::string::iterator &it)
{
	readToken(vec, str, it);
	if (*it++ != '=')
		return (false);
	readToken(vec, str, it);
	return (true);
}

bool readPadding(std::string::iterator &it)
{
	int cntSemicolon = 0;

	while (*it == ' ' || *it == '	' || *it == ';')
	{
		if (*it == ';')
			cntSemicolon++;
		it++;
	}
	return (cntSemicolon == 1);
}
