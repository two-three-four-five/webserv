#include "Request.hpp"
#include "utils.hpp"
#include <iostream>

namespace ft
{
Request::Request() : status(CREATED)
{
}

void Request::parse(const std::string &request)
{
	if (status == CREATED)
		parseStartLine(request);
	else if (status == HEADER)
		parseFields(request);
}

void Request::parseStartLine(const std::string &request)
{
	std::istringstream requestStream(request);
	std::string httpVersion;

	// GET /index.html HTTP/1.1
	requestStream >> method >> requestTarget >> httpVersion;
	// if (!requestStream.eof())
	// 	throw new httpException(400); //throw 400 bad request

	if (httpVersion.find("HTTP/") != 0)
		throw httpException(400); // throw 400 bad request
	else
	{
		std::istringstream iss(httpVersion.substr(5));
		double ver;

		iss >> ver;
		if (iss.fail() || !iss.eof())
			throw httpException(400); // throw 400 bad request

		if (httpVersion.substr(5) != "1.1")
			throw httpException(505); // throw 505 HTTP version not supported
	}

	status = HEADER;
}

void Request::parseFields(const std::string &request)
{
	std::istringstream requestStream(request);
	std::pair<std::string, std::string> field;
	std::string line;

	while (requestStream.good())
	{
		std::getline(requestStream, line);

		std::istringstream lineStream(line);
		std::getline(lineStream, field.first, ':');		   // field name
		std::getline(lineStream >> std::ws, field.second); // field value
		// field name과 ':' 사이에 띄어쓰기가 있는지 || 라인에 :가 없어서
		// field.second가 비었는지
		if (isSpaceIncluded(field.first) || field.second.empty())
			continue; // throw error
		fields.insert(field);
		field.first = "";
		field.second = "";
	}
}

void Request::printFields()
{
	for (std::map<std::string, std::string>::iterator it = fields.begin();
		 it != fields.end(); ++it)
	{
		std::cout << "Field Name : " << it->first
				  << "            Field Value : " << it->second << '\n';
	}
};

} // namespace ft
