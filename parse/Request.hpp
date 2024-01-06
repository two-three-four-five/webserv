#ifndef REQUEST_HPP
#define REQUEST_HPP

#define CREATED 0
#define START_LINE 1
#define HEADER 2
#define BODY 3
#define TRAILER 4

#include <string>
#include <map>
#include <sstream>

namespace ft{

    class Request
    {
        private:
			int status;
			int errCode;
			std::string method;
			std::string requestTarget;
            std::map <std::string, std::string> fields;
        public:
			Request();
			void parse(const std::string& request);
			void parseStartLine(const std::string& request);
            void parseFields(const std::string& request);
            void printFields();

		class httpException : public std::exception
		{
			public:
				int errCode;
				httpException(int errCode) : errCode(errCode){};
				virtual const char *what() const throw() {
					return "HTTP ERROR";
				}
		};
	};
}

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
