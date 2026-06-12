// SPDX-License-Identifier: GPL-2.0
/* Problem #07 — my_atoi 參考解 */

#include <stdio.h>
#include <ctype.h>
#include <assert.h>

static int my_atoi(const char *s)
{
	while (isspace((unsigned char)*s))
		s++;

	int sign = 1;
	if (*s == '+' || *s == '-')
		sign = (*s++ == '-') ? -1 : 1;

	int n = 0;
	while (isdigit((unsigned char)*s))
		n = n * 10 + (*s++ - '0');

	return sign * n;
}

int main(void)
{
	assert(my_atoi("42") == 42);
	assert(my_atoi("-42") == -42);
	assert(my_atoi("  +123") == 123);
	assert(my_atoi("abc") == 0);
	assert(my_atoi("12abc") == 12);
	assert(my_atoi("") == 0);
	assert(my_atoi("   ") == 0);
	assert(my_atoi("0") == 0);
	assert(my_atoi("+0") == 0);

	printf("✅ Problem #07 my_atoi passed\n");
	return 0;
}
