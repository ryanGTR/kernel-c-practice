// SPDX-License-Identifier: GPL-2.0
/* Problem #07 — my_atoi */

#include <stdio.h>
#include <ctype.h>
#include <assert.h>

static int my_atoi(const char *s)
{
	(void)s;
	return 0;   /* TODO */
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
