// SPDX-License-Identifier: GPL-2.0
/* Problem #01 — my_strlen */

#include <stdio.h>
#include <stddef.h>
#include <assert.h>

static size_t my_strlen(const char *s)
{
	(void)s;
	return 0;   /* TODO */
}

int main(void)
{
	assert(my_strlen("") == 0);
	assert(my_strlen("a") == 1);
	assert(my_strlen("hello") == 5);
	assert(my_strlen("a\0bc") == 1);   /* \0 終止 */
	printf("✅ Problem #01 my_strlen passed\n");
	return 0;
}
