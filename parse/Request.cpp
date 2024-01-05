#include "Request.hpp"
#include "utils.hpp"
#include <iostream>

namespace ft{

    void Request::parseFields(const std::string& request)
    {
        std::istringstream  requestStream(request);
        std::pair<std::string, std::string> field;
        std::string line;

        while (requestStream.good())
        {
            std::getline(requestStream, line);
            
            std::istringstream  lineStream(line);
            std::getline(lineStream, field.first, ':'); // field name
            std::getline(lineStream >> std::ws, field.second); // field value
            // field name과 ':' 사이에 띄어쓰기가 있는지 || 라인에 :가 없어서 field.second가 비었는지
            if (isSpaceIncluded(field.first) || field.second.empty())
                ;// throw error
            fields.insert(field);
            field.first = "";
            field.second = "";
        }
    }

    void Request::printFields()
    {
        for (std::map<std::string, std::string>::iterator it = fields.begin(); it != fields.end(); ++it)
        {
            std::cout << "Field Name : "  << it->first << "            Field Value : " << it->second << '\n';
        }
    };

}