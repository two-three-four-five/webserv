#ifndef REQUEST_HPP
#define REQUEST_HPP

#define CREATED 0
#define START_LINE 1
#define HEADER 2
#define BODY 3
#define TRAILER 4
#define PARSE_END 5

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace Hafserv
{

class Request
{
private:
	int parseStatus;
	int statusCode;
	bool inBoundary;
	std::string method;
	std::string requestTarget;
	std::string boundary;
	std::map<std::string, std::string> fields;
	std::vector<std::string> body;
	// std::map<std::string, std::vector<std::string> > message;

public:
	Request();
	int parse(const std::string &request);
	void parseStartLine(const std::string &request);
	void parseFieldLine(const std::string &fieldLine);
	void parseBody(const std::string &body);
	std::string getRawRequest();
	// void printMessage();
	// std::map<std::string, std::vector<std::string> > &getMessage();
	void printRequest();
	void printBody();
	friend class Response;
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