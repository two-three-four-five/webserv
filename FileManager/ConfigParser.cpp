/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:12:28 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 01:34:08 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

using namespace Hafserv;

void Config::print() const
{
	std::cout << name << " ";
	for (size_t i = 0; i < parameters.size(); i++)
		std::cout << parameters.at(i) << " ";
	std::cout << std::endl;

	for (directives_t::const_iterator it = directives.begin(); it != directives.end(); it++)
		std::cout << "\t[" << (*it).first << "] : [" << (*it).second << "]" << std::endl;

	for (size_t i = 0; i < subBlocks.size(); i++)
	{
		std::cout << std::endl << name << "." << std::endl;
		subBlocks.at(i).print();
	}
}

const std::string ConfigParser::meta = " \t{};#";

ConfigParser::ConfigParser() {}

Config ConfigParser::parse(std::string filename)
{
	Config config = Config();
	config.name = "config";

	Config main = Config();
	main.name = "main";
	config.subBlocks.push_back(main);

	std::vector<std::vector<Config>::iterator> history;
	history.push_back(config.subBlocks.begin());

	std::vector<std::string> waiting;
	std::vector<size_t> indexes;
	std::string front;
	size_t min = -1;

	RegularFile file = FileManager::openFile(filename);
	for (size_t i = 0; i < file.contents.size(); i++)
	{
		for (std::string line = file.contents.at(i); line.size(); line = line.substr(min + 1, line.size() - (min + 1)))
		{
			indexes.clear();
			for (size_t j = 0; j < meta.size(); j++)
				indexes.push_back(line.find(meta.at(j)));
			indexes.push_back(std::string::npos);

			min = *std::min_element(indexes.begin(), indexes.end());
			front = line.substr(0, min);

			switch (line[min])
			{
			case ' ':
			case '\t':
				if (!front.empty())
					waiting.push_back(front);
				break;

			case '{':
				if (!front.empty())
					waiting.push_back(front);
				if (!waiting.empty())
				{
					Config newConfig = Config();
					newConfig.name = *waiting.begin();

					for (size_t j = 1; j < waiting.size(); j++)
						newConfig.parameters.push_back(waiting.at(j));

					(*history.back()).subBlocks.push_back(newConfig);
					history.push_back(--(*history.back()).subBlocks.end());
				}
				waiting.clear();
				break;

			case '}':
				if (!front.empty() || !waiting.empty())
					exit(1); // TODO: ERROR
				history.pop_back();
				break;

			case ';':
				if (!front.empty())
					waiting.push_back(front);
				if (!waiting.empty())
				{
					for (size_t j = 1; j < waiting.size(); j++)
						(*history.back()).directives.insert(std::make_pair(waiting.at(0), waiting.at(j)));
				}
				waiting.clear();
				break;

			default:
				if (!front.empty())
					waiting.push_back(front);
				min = line.size() - 1;
				break;
			};
		}
	}
	return config;
}