/* 2.2 / 4.9 shift precedence */
#include <stdio.h>
int main(void)
{
	unsigned int hi = 1, lo = 2;
	unsigned int r = hi << 4 + lo;

	printf("%u\n", r);
	return 0;
}
