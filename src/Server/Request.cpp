#include "Http/Request.hpp"
#include "util/Parse.hpp"
#include "util/string.hpp"
#include <iostream>

using namespace Hafserv;

Request::Request() : parseStatus(Created), contentLength(0), bodyLength(0) {}

Request::Request(const Request &other)
	: parseStatus(other.parseStatus), method(other.method), requestTarget(other.requestTarget), headers(other.headers),
	  boundary(other.boundary), contentLength(other.contentLength), bodyLength(other.bodyLength), body(other.body),
	  bodyVec(other.bodyVec), parseBody(other.parseBody)
{
}

Request &Request::operator=(const Request &rhs)
{
	if (this != &rhs)
	{
		parseStatus = rhs.parseStatus;
		method = rhs.method;
		requestTarget = rhs.requestTarget;
		headers = rhs.headers;
		boundary = rhs.boundary;
		contentLength = rhs.contentLength;
		bodyLength = rhs.bodyLength;
		body = rhs.body;
		bodyVec = rhs.bodyVec;
		parseBody = rhs.parseBody;
	}
	return *this;
}
Request::~Request() {}

void Request::readRequest(const int &fd)
{
	int idx;

	if (parseStatus < Body)
	{
		int str_len = recv(fd, charBuf, BUFFER_SIZE, 0);

		buffer += std::string(charBuf, str_len);
		while ((idx = buffer.find('\n')) != std::string::npos)
		{
			std::string line = buffer.substr(0, idx + 1);
			buffer = buffer.substr(idx + 1);
			parse(line);
			std::cout << "line : " << line;
		}
	}
	else
	{
		(this->*parseBody)(fd);
	}
}

int Request::parseByBoundary(const int &fd)
{
	ssize_t bytesRead = 0;

	if ((bytesRead = recv(fd, charBuf, BUFFER_SIZE, 0)) > 0)
	{
		oss.write(charBuf, bytesRead);
		bodyLength += bytesRead;
	}
	if (bodyLength == contentLength)
	{
		body = oss.str();
		parseStatus = End;
	}
}

int Request::parseByTransferEncoding(const int &fd)
{
	int idx = 0, newIdx = 0;
	static int chunkSize = 0;
	ssize_t bytesRead;

	if ((bytesRead = recv(fd, charBuf, BUFFER_SIZE, 0)) > 0)
	{
		buffer = std::string(charBuf, bytesRead);
		if (!chunkSize)
		{
			if ((idx = buffer.find('\n')) != std::string::npos)
			{
				chunkSize = std::stol(readHex(buffer), NULL, 16);
				if (!chunkSize)
				{
					body = oss.str();
					parseStatus = End;
					std::ofstream ofs("b.pdf", std::ios::binary);
					ofs << body;
					ofs.close();
				}
				buffer.erase(buffer.begin(), buffer.begin() + idx + 1);
			}
			else
				; // error
		}
		else
			idx = -1;
	}
	// else
	// 	return (1);

	while (chunkSize && buffer.length() > chunkSize)
	{
		oss.write(charBuf + idx + 1, chunkSize);
		buffer.erase(buffer.begin(), buffer.begin() + chunkSize + 2);
		idx += chunkSize + 3;
		chunkSize = 0;
		// 새로운 chunkSize가 있으면 새로 정의하고 while문으로 감
		if ((newIdx = buffer.find('\n')) != std::string::npos)
		{
			chunkSize = std::stol(readHex(buffer), NULL, 16);
			if (!chunkSize)
			{
				body = oss.str();
				parseStatus = End;
				std::ofstream ofs("c.pdf", std::ios::binary);
				ofs << body;
				ofs.close();
			}
			buffer.erase(buffer.begin(), buffer.begin() + newIdx + 1);
			idx += newIdx;
		}
	}
	if (chunkSize)
	{
		oss.write(charBuf + idx + 1, buffer.length());
		chunkSize -= (buffer.length());
	}
}

int Request::parse(std::string &request)
{
	if (parseStatus == Created)
		return parseStartLine(request);
	else if (parseStatus == Header)
		return parseHeaders(request);
	return 0;
}

int Request::parseStartLine(const std::string &request)
{
	std::istringstream requestStream(request);
	std::string rTarget, httpVersion;

	// GET /index.html HTTP/1.1
	std::getline(requestStream, method, ' ');
	std::getline(requestStream, rTarget, ' ');
	requestTarget = RequestTarget(rTarget);
	std::getline(requestStream, httpVersion, '\r');

	if (httpVersion.find("HTTP/") != 0)
	{
		parseStatus = End;
		return 400;
	} // 400 Bad Request
	else
	{
		std::istringstream iss(httpVersion.substr(5));
		double ver;

		iss >> ver;
		if (iss.fail() || !iss.eof())
		{
			parseStatus = End;
			return 400;
		} // 400 Bad Request

		if (httpVersion.substr(5) != "1.1")
		{
			parseStatus = End;
			return 505;
		} // 505 HTTP version not supported
	}
	if (method == "HEAD")
		parseStatus = End;
	else
		parseStatus = Header;
	return 0;
}

int Request::parseHeaders(const std::string &fieldLine)
{
	if (fieldLine == "\r\n")
	{
		// std::cout << "Header field end" << std::endl;
		if (method == "GET")
			parseStatus = End;
		else
			parseStatus = Body;
		buffer.clear();
		checkHeaderField();
		return 0;
	}
	std::istringstream iss(fieldLine);
	std::string key, value;

	std::getline(iss, key, ':');
	if (util::string::hasSpace(key))
	{
		parseStatus = End;
		return 400;
	} // 400 Bad Request
	while (std::getline(iss >> std::ws, value, ','))
	{
		if (value.empty())
			; // error ( NOT ERROR MAYBE )
		if (value[value.length() - 1] == '\n')
			value = value.substr(0, value.length() - 2);
		key = util::string::toLower(key);
		headers.insert(std::make_pair(key, value));
	}
	return 0;
}

void Request::checkHeaderField()
{
	// contentLength
	std::multimap<std::string, std::string>::iterator contentLengthIt = headers.find("content-length");
	if (contentLengthIt != headers.end())
	{
		contentLength = stoi(contentLengthIt->second);
		// parseBody = parseByContentLength;
		parseBody = &Request::parseByContentLength;
	}

	// Boundary
	std::multimap<std::string, std::string>::iterator contentTypeIt = headers.find("content-type");
	if (contentTypeIt != headers.end())
	{
		std::vector<std::string> contentType = parseContentType(contentTypeIt->second);
		std::vector<std::string>::iterator it = std::find(contentType.begin(), contentType.end(), "boundary");
		if (it != contentType.end())
		{
			boundary = *(it + 1);
			parseBody = &Request::parseByBoundary;
		}
	}
	// Transfer-Encoding
	std::multimap<std::string, std::string>::iterator tranferEncodingIt = headers.find("transfer-encoding");
	if (tranferEncodingIt != headers.end())
	{
		std::vector<std::string> transferEncoding = parseTransferEncoding(tranferEncodingIt->second);
		std::vector<std::string>::iterator it = std::find(transferEncoding.begin(), transferEncoding.end(), "chunked");
		if (it + 1 == transferEncoding.end())
			parseBody = &Request::parseByTransferEncoding;
		else if (it != transferEncoding.end() || contentLength == -1)
			; // chunked가 있는데 마지막에 있는게 아니거나 | tranferEncoding이 있는데 contentLength도 있으면 에러
	}
}

int Request::parseByContentLength(const int &fd)
{
	// 	bodyVec.push_back(line);
	// 	bodyLength += line.length();
	// 	if (line == "\r\n")
	// 	{
	// 		std::ostringstream oss;
	// 		for (std::vector<std::string>::iterator it = bodyVec.begin(); it != bodyVec.end(); it++)
	// 			oss << *it;
	// 		body = oss.str();
	// 		parseStatus = End;
	// 	}
	// 	return 0;
}

// int Request::parseByBoundary(std::string &line)
// {
// 	// 계속 읽다가 Boundary--일때면 멈춤
// 	bodyVec.push_back(line);
// 	bodyLength += line.length();
// 	if (line.substr(2, line.length() - 6) == boundary)
// 	{
// 		std::ostringstream oss;
// 		for (std::vector<std::string>::iterator it = bodyVec.begin(); it != bodyVec.end(); it++)
// 			oss << *it;
// 		body = oss.str();
// 		parseStatus = End;
// 	}
// 	return 0;
// }

// int Request::parseByTransferEncoding(std::string &line)
// {
// 	static int chunkSize;

// 	if (!chunkSize)
// 	{
// 		chunkSize = std::stoi(readHex(line), NULL, 16);
// 		if (chunkSize == 0)
// 		{
// 			std::ostringstream oss;
// 			for (std::vector<std::string>::iterator it = bodyVec.begin(); it != bodyVec.end(); it++)
// 				oss << *it;
// 			body = oss.str();
// 			body += (char)26;
// 			parseStatus = End;
// 			chunkSize = 0;
// 			contentLength = bodyLength;
// 		}
// 	}
// 	else
// 	{
// 		int length = line.length();

// 		bodyLength += length;
// 		chunkSize -= length;
// 		line.resize(length);
// 		bodyVec.push_back(line);
// 	}
// 	return 0;
// }

void Request::parseFormBody(char charBuf[], const int &bytesRead)
{
	body.append(charBuf, bytesRead);
	if (body.length() == contentLength)
	{
		parseStatus = End;
	}
}

std::string Request::getRawRequest()
{
	std::stringstream ss;

	ss << method << " " << requestTarget << " HTTP/1.1\r\n";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
		ss << it->first << ": " << it->second << "\r\n";
	if (method == "POST")
		ss << "\r\n" << body << "\r\n";
	return ss.str();
}

void Request::printRequest()
{
	std::cout << "<-------request------->" << std::endl;
	std::cout << method << " " << requestTarget << " HTTP/1.1\r\n";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
		std::cout << it->first << ": " << it->second << "\r\n";
	std::cout << body;
	// if (method == "POST")
	// 	std::cout << "\r\n" << body[0] << "\r\n";
	std::cout << "<-----request end----->" << std::endl;
}

void Request::printBody()
{
	for (int i = 0; i < bodyVec.size(); i++)
	{
		std::cout << "body[" << i << "]:" << std::endl << bodyVec[i] << std::endl;
	}
}

const int Hafserv::Request::getParseStatus() const { return parseStatus; }

const RequestTarget &Request::getRequestTarget() const { return requestTarget; }

const HeaderMultiMap &Request::getHeaders() const { return headers; }

const std::string &Hafserv::Request::getMethod() const { return method; }

const std::string &Hafserv::Request::getBody() const { return body; }

const void Hafserv::Request::setBody(std::string body)
{
	this->body = body;
	parseStatus = End;
}

const size_t Hafserv::Request::getContentLength() const { return contentLength; }
