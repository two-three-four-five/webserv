#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <sstream>

namespace ft{
    
    class Request
    {
        private:
            std::map <std::string, std::string> fields;
        public:
            void parseFields(const std::string& request);
            void printFields();
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