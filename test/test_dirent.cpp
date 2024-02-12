#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Function to print file properties
void printFileProperties(struct dirent *file)
{
	struct stat fileStat;
	if (stat(file->d_name, &fileStat) < 0)
	{
		perror("stat");
		return;
	}

	printf("Name: %s\n", file->d_name);
	printf("Size: %ld bytes\n", fileStat.st_size);
	printf("Last modified: %s", ctime(&fileStat.st_mtime));
	printf("--------------------------------\n");
}

int main()
{
	// Open the current directory
	DIR *dir = opendir(".");
	if (dir == NULL)
	{
		perror("Unable to open directory");
		return 1;
	}

	// Read directory contents
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_REG)
		{ // Check if it's a regular file
			printFileProperties(entry);
		}
	}

	// Close the directory
	closedir(dir);

	return 0;
}