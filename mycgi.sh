#!/bin/bash

echo "Content-type: text/html" $'\r'

if [ "$REQUEST_METHOD" = "POST" ]; then
    # 처리할 POST 요청의 내용
    read -n $CONTENT_LENGTH data
    echo $'\r'
    echo "<html><head><title>POST Data</title></head><body><h1>POST Data</h1><p>$data</p></body></html>" $'\r'
else
    # 기본 페이지
    echo $'\r'
    echo "<html><head><title>CGI with Bash</title></head><body><h1>Hello CGI with Bash</h1></body></html>" $'\r'
fi
