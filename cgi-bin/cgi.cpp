#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
	char *content_length_str = getenv("CONTENT_LENGTH");
	int content_length;

	if (content_length_str == nullptr || sscanf(content_length_str, "%d", &content_length) != 1)
	{
		std::cout << "Content-type: text/html\n\n";
		std::cout << "<html><head><title>File Upload Error</title></head><body>";
		std::cout << "<h1>Error</h1>";
		std::cout << "<p>Invalid content length</p>";
		std::cout << "</body></html>";
		return 1;
	}

	char *post_data = new char[content_length + 1];
	std::cin.read(post_data, content_length);
	std::cerr << "by cgi\n" << post_data;
	post_data[content_length] = '\0';

	// 파일 업로드 로직
	const char *boundary = "boundary=";
	char *boundary_start = strstr(getenv("CONTENT_TYPE"), boundary);

	if (boundary_start != nullptr)
	{
		boundary_start += strlen(boundary);
		std::string boundary_str = "--" + std::string(boundary_start);

		// 파일 업로드 로직을 추가하세요.
		// 멀티파트/form-data에서 각 파트를 처리하는 코드가 필요합니다.

		// 예를 들면:
		std::string file_start = "--" + boundary_str;
		std::string file_end = "--" + boundary_str + "--";

		// 파일 이름과 데이터를 추출하고 서버에 저장합니다.

		std::string file_part = post_data;
		size_t file_start_pos = file_part.find(file_start);
		size_t file_end_pos = file_part.find(file_end);
		std::string file_data =
			file_part.substr(file_start_pos + file_start.length(), file_end_pos - file_start_pos - file_start.length());

		// 파일 데이터를 서버에 저장하는 코드
		std::ofstream file_out("uploaded_file.txt", std::ios::out | std::ios::binary);
		file_out.write(file_data.c_str(), content_length);

		std::cout << "Content-type: text/html\n\n";
		std::cout << "<html><head><title>File Upload Result</title></head><body>";
		std::cout << "<h1>File Upload Successful</h1>";
		std::cout << "</body></html>";
	}
	else
	{
		std::cout << "Content-type: text/html\n\n";
		std::cout << "<html><head><title>File Upload Error</title></head><body>";
		std::cout << "<h1>Error</h1>";
		std::cout << "<p>Invalid form data</p>";
		std::cout << "</body></html>";
	}

	delete[] post_data;

	return 0;
}
