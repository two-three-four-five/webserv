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
					std::string value;
					for (size_t j = 1; j < waiting.size() - 1; j++)
						value += waiting.at(j) + " ";
					value += waiting.back();
					(*history.back()).directives.insert(std::make_pair(waiting.at(0), value));
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

void ConfigFile::include()
{
	directives_t::iterator inc = this->directives.lower_bound("include");
	directives_t::iterator end = this->directives.upper_bound("include");
	for (; inc != end; inc++)
	{
		RegularFile toInclude = RegularFile((*inc).second);
		if (toInclude.getCode() == File::REGULAR_FILE)
		{
			ConfigFile file = ConfigFile(toInclude.getName());
			directives_t::iterator toIncBegin = file.subBlocks.at(0).directives.begin();
			directives_t::iterator toIncEnd = file.subBlocks.at(0).directives.end();
			for (; toIncBegin != toIncEnd; ++toIncBegin)
				this->directives.insert(*toIncBegin);
			for (size_t i = 0; i < file.subBlocks.at(0).subBlocks.size(); i++)
				this->subBlocks.push_back(file.subBlocks.at(0).subBlocks.at(i));
		}
		else
			std::cout << "NO FILE : " << toInclude.getName() << std::endl;
	}
	if (this->subBlocks.size())
	{
		for (size_t i = 0; i < this->subBlocks.size(); i++)
			this->subBlocks.at(i).include();
	}
	this->directives.erase("include");
}

std::ostream &operator<<(std::ostream &os, const ConfigFile &configFile)
{
	os << configFile.name << " ";
	for (size_t i = 0; i < configFile.parameters.size(); i++)
		os << configFile.parameters.at(i) << " ";
	os << std::endl;

	for (ConfigFile::directives_t::const_iterator it = configFile.directives.begin(); it != configFile.directives.end();
		 it++)
		os << "    [" << (*it).first << "] : [" << (*it).second << "]" << std::endl;

	for (size_t i = 0; i < configFile.subBlocks.size(); i++)
	{
		os << std::endl << configFile.name << ".";
		os << configFile.subBlocks.at(i);
	}
	return os;
}