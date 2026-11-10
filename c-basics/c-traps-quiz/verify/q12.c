/* 6.1 macro not function */
#include <stdio.h>
#define abs(x) x > 0 ? x : -x
#define max(a, b) ((a) > (b) ? (a) : (b))
int main(void)
{
	int a = 3, b = 5, i = 1, j = 0;
	int r1 = abs(a - b);
	int r2 = max(i++, j);

	printf("r1=%d\n", r1);
	printf("r2=%d i=%d\n", r2, i);
	return 0;
}
