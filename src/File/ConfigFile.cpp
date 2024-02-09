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
#include <sstream>

using namespace Hafserv;

const std::string ConfigFile::meta = " \t{};#";

ConfigFile::ConfigFile() {}

ConfigFile::ConfigFile(const ConfigFile &other)
	: RegularFile(other), lines(other.lines), blockDirective(other.blockDirective),
	  parameters(parameters_t(other.parameters)), directives(directives_t(other.directives)),
	  subBlocks(subblocks_t(other.subBlocks))
{
}

ConfigFile::ConfigFile(const RegularFile &other) : RegularFile(other) {}

ConfigFile::ConfigFile(const std::string &filename) : RegularFile(filename), blockDirective("head")
{
	if (code != File::REGULAR_FILE)
		; // throw config error

	// initialize lines
	std::stringstream ss(contents);
	std::string temp;
	while (std::getline(ss, temp))
		lines.push_back(temp);

	ConfigFile main = ConfigFile();
	main.blockDirective = "main";
	subBlocks.push_back(main);

	std::vector<std::vector<ConfigFile>::iterator> history;
	history.push_back(subBlocks.begin());

	std::vector<std::string> waiting;
	std::vector<size_t> indexes;
	std::string line, front;
	size_t min = -1;

	for (size_t i = 0; i < lines.size(); i++)
	{
		for (line = lines[i]; line.size(); line = line.substr(min + 1, line.size() - (min + 1)))
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
					ConfigFile newConfig = ConfigFile(*dynamic_cast<RegularFile *>(this));
					newConfig.blockDirective = *waiting.begin();

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
		RegularFile::operator=(other);
		blockDirective = other.blockDirective;
		parameters = parameters_t(other.parameters);
		directives = directives_t(other.directives);
		subBlocks = subblocks_t(other.subBlocks);
	}
	return *this;
}

ConfigFile::~ConfigFile() throw() {}

const std::string &ConfigFile::getBlockDirective() const { return blockDirective; }
const ConfigFile::parameters_t &ConfigFile::getParameters() const { return parameters; }
const ConfigFile::directives_t &ConfigFile::getDirectives() const { return directives; }
const ConfigFile::subblocks_t &ConfigFile::getSubBlocks() const { return subBlocks; }

void ConfigFile::include() throw(IncludeError)
{
	directives_t::iterator incBegin = this->directives.lower_bound("include");
	directives_t::iterator incEnd = this->directives.upper_bound("include");
	for (; incBegin != incEnd; incBegin++)
	{
		std::string filename = this->name.substr(0, this->name.rfind('/') + 1) + (*incBegin).second;
		RegularFile toInclude = RegularFile(filename);
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
			throw IncludeError("NO FILE EXIST : " + toInclude.getName());
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
	os << configFile.getBlockDirective() << " ";
	for (size_t i = 0; i < configFile.getParameters().size(); i++)
		os << configFile.getParameters().at(i) << " ";
	os << std::endl;

	os << "    path: " << configFile.getName() << std::endl;

	for (ConfigFile::directives_t::const_iterator it = configFile.getDirectives().begin();
		 it != configFile.getDirectives().end(); it++)
		os << "    [" << (*it).first << "] : [" << (*it).second << "]" << std::endl;

	for (size_t i = 0; i < configFile.getSubBlocks().size(); i++)
	{
		os << std::endl << configFile.getBlockDirective() << ".";
		os << configFile.getSubBlocks().at(i);
	}
	return os;
}
