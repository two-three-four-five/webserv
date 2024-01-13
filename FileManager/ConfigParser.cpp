/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:12:28 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/13 11:56:26 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

using namespace Hafserv;

const std::string ConfigParser::meta = "# \t{};";

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

void ConfigParser::printConfig(config_t config)
{
	std::cout << "[key] : [value]" << std::endl;
	for (config_t::iterator it = config.begin(); it != config.end(); it++)
	{
		std::cout << "[";
		for (size_t i = 0; i < (*it).first.size(); i++)
			std::cout << (*it).first.at(i) << ".";
		std::cout << "]";

		std::cout << " : [" << (*it).second << "]" << std::endl;
	}
}

static bool hasIncludeInKey(config_t config)
{

	for (config_t::iterator it = config.begin(); it != config.end(); it++)
		for (size_t j = 0; j < (*it).first.size(); j++)
			if ((*it).first.at(j) == "include")
				return true;
	return false;
}

config_t ConfigParser::parse(std::string filename)
{
	config_t config;
	config = parseSingleFile(filename);
	// check syntax;

	while (hasIncludeInKey(config))
	{
		config_t::iterator include;
		for (config_t::iterator it = config.begin(); it != config.end(); it++)
			for (size_t j = 0; j < (*it).first.size(); j++)
				if ((*it).first.at(j) == "include")
					include = it;

		config_t fileConfig = parseSingleFile((*include).second);

		directive_t directive = (*include).first;
		directive.pop_back();
		for (config_t::iterator it = fileConfig.begin(); it != fileConfig.end(); it++)
		{
			directive_t newDirective = directive;
			newDirective.insert(newDirective.end(), (*it).first.begin(), (*it).first.end());
			config.insert(std::make_pair(newDirective, (*it).second));
		}
		config.erase(include);
		// check syntax
	}
	return config;
}

config_t ConfigParser::parseSingleFile(std::string filename)
{
	config_t config;

	std::vector<std::string> directive, waiting;
	std::vector<size_t> indexes;
	std::string front;
	size_t min = 0;

	File file = FileManager::openFile(filename);

	for (size_t i = 0; i < file.contents.size(); i++)
	{
		std::string line = file.contents.at(i);
		std::cout << line << std::endl;
		while (line.size())
		{
			std::cout << line << std::endl;
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
			case '\t':
				if (!front.empty())
					waiting.push_back(front);
				break;

			case '{':
				if (!front.empty())
					waiting.push_back(front);
				if (!waiting.empty())
				{
					std::string blockWaiting;
					for (size_t j = 0; j < waiting.size(); j++)
						blockWaiting += waiting.at(j) + " ";
					directive.push_back(blockWaiting.substr(0, blockWaiting.size() - 1));
				}
				waiting.clear();
				break;

			case '}':
				if (!front.empty() || !waiting.empty())
					exit(1); // TODO: ERROR
				directive.pop_back();
				break;

			case ';':
				if (!front.empty())
					waiting.push_back(front);
				if (!waiting.empty())
					directive.push_back(waiting.at(0));
				for (size_t i = 1; i < waiting.size(); i++)
					config.insert(std::make_pair(directive, waiting.at(i)));
				waiting.clear();
				directive.pop_back();
				break;

			default:
				std::cout << line << std::endl;
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