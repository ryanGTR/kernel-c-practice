/* 2.2 precedence: & vs != */
#include <stdio.h>
#define FLAG 4
int main(void)
{
	int flags = 4;

	if (flags & FLAG != 0)
		printf("set\n");
	else
		printf("not set\n");
	return 0;
}
