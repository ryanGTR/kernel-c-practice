// SPDX-License-Identifier: GPL-2.0
/* Problem #09 — count_words */

#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include <assert.h>

static size_t count_words(const char *s)
{
	(void)s;
	return 0;   /* TODO */
}

int main(void)
{
	assert(count_words("hello world") == 2);
	assert(count_words("  multiple   spaces  ok  ") == 3);
	assert(count_words("") == 0);
	assert(count_words("   ") == 0);
	assert(count_words("oneword") == 1);
	assert(count_words("a b c") == 3);
	assert(count_words("\t\nhello\tworld\n") == 2);

	printf("✅ Problem #09 count_words passed\n");
	return 0;
}
