/* 2.4 switch fallthrough */
#include <stdio.h>
int main(void)
{
	int n = 2;

	switch (n) {
	case 1: printf("one ");
	case 2: printf("two ");
	case 3: printf("three ");
	default: printf("other ");
	}
	printf("\n");
	return 0;
}
