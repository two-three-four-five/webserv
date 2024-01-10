/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:12:28 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/10 15:19:36 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {}

ConfigParser::ConfigParser(const ConfigParser &other) {}

ConfigParser &ConfigParser::operator=(const ConfigParser &other)
{
	if (this != &other)
	{
	}
	return *this;
}

ConfigParser::~ConfigParser() {}

ConfigFile ConfigParser::parse(std::string filename)
{
	ConfigFile configFile;

	File file = FileManager::openFile(filename);

	std::vector<std::string> directive;
	std::vector<std::string> stack;
	int parameter = -1;
	for (size_t i = 0; i < file.contents.size(); i++)
	{
		// 1. find out what is this part
		// 2. do something
		// 3. update line
		std::string line = file.contents.at(i);
		while (line.size())
		{
			std::cout << line << std::endl;
			std::vector<size_t> its;
			its.push_back(line.find(' '));
			its.push_back(line.find('{'));
			its.push_back(line.find('}'));
			its.push_back(line.find(';'));
			its.push_back(line.find('#'));
			its.push_back(line.find('\t'));
			its.push_back(std::string::npos);

			size_t meta = *std::min_element(its.begin(), its.end());
			std::string part = line.substr(0, meta);

			switch (line[meta])
			{
			case '#':
				if (!part.empty())
					stack.push_back(part);
				line.clear();
				meta = -1;
				break;
			case ' ':
			case '\t':
				if (!part.empty())
					stack.push_back(part);
				break;

			case '{':
				if (!part.empty())
					stack.push_back(part);

				if (!stack.empty())
					directive.push_back(stack.at(0));
				stack.clear();
				break;
			case '}':
				if (!part.empty() || !stack.empty())
					exit(1); // error
				directive.pop_back();
				break;
			case ';':
				if (!part.empty())
					stack.push_back(part);

				if (!stack.empty())
					directive.push_back(stack.at(0));
				for (size_t i = 1; i < stack.size(); i++)
					configFile.config.insert(std::make_pair(directive, stack.at(i)));

				stack.clear();
				directive.pop_back();

				break;
			default:
				break;
			};
			line = line.substr(meta + 1, line.size() - (meta + 1));
		}
	}
	return configFile;
}