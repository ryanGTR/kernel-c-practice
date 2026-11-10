/* 4.5 sizeof array param */
#include <stdio.h>
static void f(int a[10])
{
	printf("in f: %zu\n", sizeof(a));
}
int main(void)
{
	int a[10];

	printf("in main: %zu\n", sizeof(a));
	f(a);
	return 0;
}
