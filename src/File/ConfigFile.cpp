/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gyoon <gyoon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 17:12:28 by gyoon             #+#    #+#             */
/*   Updated: 2024/01/14 16:48:40 by gyoon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigFile.hpp"

using namespace Hafserv;

const std::string ConfigFile::meta = " \t{};#";

ConfigFile::ConfigFile() : name("") {}

ConfigFile::ConfigFile(const ConfigFile &other)
	: name(other.name), parameters(parameters_t(other.parameters)), directives(directives_t(other.directives)),
	  subBlocks(subblocks_t(other.subBlocks))
{
}

ConfigFile::ConfigFile(const std::string &filename) : name("head")
{
	ConfigFile main = ConfigFile();
	main.name = "main";
	subBlocks.push_back(main);

	std::vector<std::vector<ConfigFile>::iterator> history;
	history.push_back(subBlocks.begin());

	std::vector<std::string> waiting;
	std::vector<size_t> indexes;
	std::string line, front;
	size_t min = -1;

	RegularFile file = RegularFile(filename);
	for (size_t i = 0; i < file.getContentsSize(); i++)
	{
		for (line = file.getline(i); line.size(); line = line.substr(min + 1, line.size() - (min + 1)))
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
					ConfigFile newConfig = ConfigFile();
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
}

ConfigFile &ConfigFile::operator=(const ConfigFile &other)
{
	if (this != &other)
	{
		name = other.name;
		parameters = parameters_t(other.parameters);
		directives = directives_t(other.directives);
		subBlocks = subblocks_t(other.subBlocks);
	}
	return *this;
}

ConfigFile::~ConfigFile() throw() {}

void ConfigFile::print() const
{
	std::cout << name << " ";
	for (size_t i = 0; i < parameters.size(); i++)
		std::cout << parameters.at(i) << " ";
	std::cout << std::endl;

	for (directives_t::const_iterator it = directives.begin(); it != directives.end(); it++)
		std::cout << "    [" << (*it).first << "] : [" << (*it).second << "]" << std::endl;

	for (size_t i = 0; i < subBlocks.size(); i++)
	{
		std::cout << std::endl << name << ".";
		subBlocks.at(i).print();
	}
}