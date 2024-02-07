#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Server.hpp"
#include <ctime>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace Hafserv
{

typedef std::multimap<std::string, std::string> HeaderMultiMap;

enum RequestParseStatus
{
	Created,
	StartLine,
	Header,
	Body,
	Trailer,
	End
};

class Request
{
public:
	Request();
	int parse(const std::string &request);
	int parseStartLine(const std::string &request);
	int parseHeaders(const std::string &fieldLine);
	int parseBody(const std::string &body);
	std::string getRawRequest();
	void printRequest();
	void printBody();

	const int getParseStatus() const;
	const std::string getRequestTarget() const;
	const HeaderMultiMap &getHeaders() const;
	const std::string &getMethod() const;
	const std::string &getBody() const;

	friend class Response;

private:
	RequestParseStatus parseStatus;

	std::string method;
	std::string requestTarget;
	HeaderMultiMap headers;
	std::vector<std::string> body;
	size_t bodyLength;
};

} // namespace Hafserv

/*
	스타트라인 파싱

	해드 필드 파싱
	1. :으로 구분 필드 네임과 필드 값 분리
	2. 필드 네임 검사
		E : 400코드 리턴
	3. 필드 네임에 따라 파싱함수 적용
		E :
	4. 반복
*/

#endif
