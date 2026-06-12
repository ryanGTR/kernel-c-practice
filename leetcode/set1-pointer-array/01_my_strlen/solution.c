// SPDX-License-Identifier: GPL-2.0
/* Problem #01 — my_strlen 參考解 */

#include <stdio.h>
#include <stddef.h>
#include <assert.h>

static size_t my_strlen(const char *s)
{
	const char *p = s;
	while (*p)
		p++;
	return p - s;
}

int main(void)
{
	assert(my_strlen("") == 0);
	assert(my_strlen("a") == 1);
	assert(my_strlen("hello") == 5);
	assert(my_strlen("a\0bc") == 1);
	printf("✅ Problem #01 my_strlen passed\n");
	return 0;
}
