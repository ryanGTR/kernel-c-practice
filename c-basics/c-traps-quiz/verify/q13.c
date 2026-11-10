/* 6.2 macro not typedef */
#include <stdio.h>
#define IPTR int *
int main(void)
{
	IPTR a, b;

	printf("%zu %zu\n", sizeof(a), sizeof(b));
	return 0;
}
