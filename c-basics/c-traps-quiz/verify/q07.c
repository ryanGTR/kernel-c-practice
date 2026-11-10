/* 2.6 dangling else */
#include <stdio.h>
int main(void)
{
	int x = 0, y = 1;

	if (x == 1)
		if (y == 1)
			printf("A\n");
	else
		printf("B\n");
	printf("end\n");
	return 0;
}
