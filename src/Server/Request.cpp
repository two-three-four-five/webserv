#include "Http/Request.hpp"
#include "util/Parse.hpp"
#include "util/string.hpp"
#include <iostream>

using namespace Hafserv;

Request::Request() : parseStatus(Created), contentLength(0), bodyLength(0), isEnd(false) {}

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

int Request::readRequest(const int fd)
{

	int idx;

	// std::cout << "Buf len[Init] : " << buffer.length() << std::endl;
	if (parseStatus < Body)
	{
		int str_len = recv(fd, charBuf, BUFFER_SIZE, 0);

		buffer += std::string(charBuf, str_len);
		// std::cout << "Buf len[Added] : " << buffer.length() << std::endl;
		while ((idx = buffer.find('\n')) != std::string::npos && parseStatus < Body)
		{
			std::string line = buffer.substr(0, idx + 1);
			// std::cout << "line : " << line;
			// std::cout << "line len : " << line.length() << std::endl;
			// std::cout << "idx : " << idx << std::endl;
			int status;

			// buffer.erase(buffer.begin(), buffer.begin() + idx + 1);
			buffer = buffer.substr(idx + 1);
			if ((status = parse(line)))
				return status;
		}
		// std::cout << "Buf len[Erased] : " << buffer.length() << std::endl;
	}
	else
	{
		return (this->*parseBody)(fd);
	}
	return 0;
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
	parseStatus = Header;
	return 0;
}

int Request::parseHeaders(const std::string &fieldLine)
{
	if (fieldLine == "\r\n")
	{
		// std::cout << "Header field end" << std::endl;
		if (method == "POST")
			parseStatus = Body;
		else
			parseStatus = End;
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

int Request::parseByBoundary(const int &fd)
{
	ssize_t bytesRead = 0;

	// ++ \r\n까지읽기
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
	return 0;
}

int Request::parseByTransferEncoding(const int &fd)
{
	static int chunkSize = 0;
	ssize_t bytesRead = read(fd, charBuf, BUFFER_SIZE);
	buffer += std::string(charBuf, bytesRead);

	while (true)
	{
		// std::cout << "buflen: " << buffer.length() << std::endl;
		// for (int i = 0; i < buffer.length(); i++)
		// 	std::cout << (int)buffer[i] << " ";
		if (chunkSize) // no buffer
		{
			if (chunkSize < buffer.length())
			{
				oss << buffer.substr(0, chunkSize);
				// std::cout << "c<b" << chunkSize << std::endl;
				buffer = buffer.substr(chunkSize);
				chunkSize = 0;
			}
			else
			{
				oss << buffer;
				// std::cout << "c>b" << buffer.length();
				chunkSize -= buffer.length();
				buffer.clear();
				break;
			}
		}
		// read chunksize
		if (isEnd && buffer.length() == 2)
		{
			body = oss.str();
			// std::cout << "END" << std::endl;
			std::ofstream ofs("my.pdf");
			ofs << body;
			ofs.close();
			parseStatus = End;
			break;
		}
		if (!isEnd && buffer.length() >= 2 && buffer.substr(0, 2) == "\r\n")
			buffer.erase(0, 2);
		int idx;
		if ((idx = buffer.find("\r\n")) != std::string::npos)
		{
			chunkSize = std::stol(readHex(buffer), NULL, 16);
			buffer.erase(0, idx + 2);
			if (chunkSize == 0)
				isEnd = true;
		}
		else
			break;
	}

	return 0;
}

int Request::parseByContentLength(const int &fd)
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
	return 0;
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

void Request::printRequest() const
{
	std::cout << "<-------request------->" << std::endl;
	std::cout << method << " " << requestTarget << " HTTP/1.1\r\n";
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); it++)
		std::cout << it->first << ": " << it->second << "\r\n";
	// std::cout << body;
	std::cout << "<-----request end----->" << std::endl;
}

const int Hafserv::Request::getParseStatus() const { return parseStatus; }

const RequestTarget &Request::getRequestTarget() const { return requestTarget; }

const HeaderMultiMap &Request::getHeaders() const { return headers; }

const std::string &Hafserv::Request::getMethod() const { return method; }

const std::string &Hafserv::Request::getBody() const { return body; }

const size_t Hafserv::Request::getContentLength() const { return contentLength; }

const void Hafserv::Request::setBody(std::string body)
{
	this->body = body;
	parseStatus = End;
}
