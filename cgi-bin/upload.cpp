#include <stdio.h>

int main()
{
	char ch;
	printf("Content-type: text/html\n\n");

	printf("<PRE>\n");
	while (1)
	{
		fscanf(stdin, "%c", &ch);
		if (feof(stdin))
			break;
		printf("%c", ch);
	}
	printf("</PRE><HR>");
}
