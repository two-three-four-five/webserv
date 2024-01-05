#include <iostream>
#include "Request.hpp"

int main() 
{
    std::string requestStr = "Host: www.test101.com\r\nAccept: image/gif, image/jpeg, */*\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nUser-Agent: Mozilla/4.0\nContent-Length: 35";
    ft::Request request;

    request.parseFields(requestStr);
    request.printFields();
    return 0;
}
