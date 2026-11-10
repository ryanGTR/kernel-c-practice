/* 1.3 greedy lexing: b---c */
#include <stdio.h>
int main(void)
{
	int a, b = 5, c = 2;

	a = b---c;
	printf("a=%d b=%d c=%d\n", a, b, c);
	return 0;
}
