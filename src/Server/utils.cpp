#include "utils.hpp"
#include <string>

namespace Hafserv
{
bool isSpaceIncluded(const std::string &str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (std::isspace(str[i]))
			return (true);
	}
	return (false);
};
} // namespace Hafserv

void Hafserv::stringToLower(std::string &str)
{
	for (int i = 0; i < str.length(); i++)
	{
		str[i] = tolower(str[i]);
	}
}
