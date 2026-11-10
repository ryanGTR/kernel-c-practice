/* 2.3 stray semicolon */
#include <stdio.h>
int main(void)
{
	int max = 10, x = 3;

	if (x > max);
		max = x;
	printf("%d\n", max);
	return 0;
}
