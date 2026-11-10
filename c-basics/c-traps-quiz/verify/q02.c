/* 1.5 'a' vs "a" */
#include <stdio.h>
int main(void)
{
	printf("%zu %zu\n", sizeof('a'), sizeof("a"));
	return 0;
}
