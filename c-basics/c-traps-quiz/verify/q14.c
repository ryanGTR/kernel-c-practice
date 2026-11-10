/* 4.8 integer overflow (unsigned wrap) */
#include <stdio.h>
#include <string.h>
int main(void)
{
	const char *s = "hi";
	int i = -1;

	if (strlen(s) > i)
		printf("longer\n");
	else
		printf("shorter\n");
	return 0;
}
