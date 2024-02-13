#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
	std::cout << "argv" << std::endl;
	for (int i = 0; i < argc; i++)
		std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
	std::cout << "envp" << std::endl;
	for (int i = 0; envp[i]; i++)
		std::cout << "envp[" << i << "]: " << envp[i] << std::endl;
	int readByte;
	size_t byte = 0;
	char buf[100001];
	while ((readByte = read(0, buf, 100000)) > 0)
	{
		byte += readByte;
		std::cout << byte << std::endl;
	}

	return 0;
}
