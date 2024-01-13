/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:12:28 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/13 20:30:49 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

using namespace Hafserv;

const std::string ConfigParser::meta = "# \t{};";

ConfigParser::ConfigParser() {}

void ConfigParser::printConfig(Config config)
{
	std::cout << "name : " << config.name << std::endl;
	std::cout << "\tpara : ";
	for (size_t i = 0; i < config.options.size(); i++)
		std::cout << config.options.at(i) << " ";
	std::cout << std::endl;

	std::cout << "\tvalues : " << std::endl;
	for (auto it = config.values.begin(); it != config.values.end(); it++)
		std::cout << "\t[" << (*it).first << "] : [" << (*it).second << "]" << std::endl;

	for (size_t i = 0; i < config.subBlocks.size(); i++)
	{
		std::cout << std::endl;
		printConfig(config.subBlocks.at(i));
	}
}

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
	size_t min = 0;

	File file = FileManager::openFile(filename);
	for (size_t i = 0; i < file.contents.size(); i++)
	{
		std::string line = file.contents.at(i);
		while (line.size())
		{
			indexes.clear();
			for (size_t j = 0; j < meta.size(); j++)
				indexes.push_back(line.find(meta.at(j)));
			indexes.push_back(std::string::npos);

			min = *std::min_element(indexes.begin(), indexes.end());
			front = line.substr(0, min);

			switch (line[min])
			{
			case '#':
				if (!front.empty())
					waiting.push_back(front);
				line.clear();
				min = -1;
				break;

			case ' ':
				if (!front.empty())
					waiting.push_back(front);
				break;
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
						newConfig.options.push_back(waiting.at(j));

					// newConfig.options.insert(newConfig.options.end(), waiting.begin()++, wating.end());
					(*history.back()).subBlocks.push_back(newConfig);

					// printConfig(newConfig);
					// printConfig((*history.back()).subBlocks.back());

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
						(*history.back()).values.insert(std::make_pair(waiting.at(0), waiting.at(j)));
					// curr->values.insert(std::make_pair(waiting.at(0), waiting.at(j)));
				}

				waiting.clear();
				break;

			default:
				if (!front.empty())
					waiting.push_back(front);
				min = line.size() - 1;
				break;
			};
			line = line.substr(min + 1, line.size() - (min + 1));
		}
	}
	return config;
}