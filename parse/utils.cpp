#include <string>

namespace ft
{
    bool isSpaceIncluded(const std::string& str)
    {
        for (int i = 0; i < str.length(); i++)
        {
            if (std::isspace(str[i]))
                return (true);
        }
        return (false);
    };
}