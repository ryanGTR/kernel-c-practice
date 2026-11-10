/* 7.3 char signedness + 5.1 getc */
#include <stdio.h>
int main(void)
{
	char c = 200;

	printf("%d\n", c);
	printf("%d\n", c == 200);
	return 0;
}
