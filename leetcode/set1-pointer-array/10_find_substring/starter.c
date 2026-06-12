// SPDX-License-Identifier: GPL-2.0
/* Problem #10 — find_substring */

#include <stdio.h>
#include <string.h>
#include <assert.h>

static const char *find_substring(const char *haystack, const char *needle)
{
	(void)haystack;
	(void)needle;
	return NULL;   /* TODO */
}

int main(void)
{
	const char *p;

	p = find_substring("hello world", "world");
	assert(p && strcmp(p, "world") == 0);

	p = find_substring("hello", "x");
	assert(p == NULL);

	p = find_substring("aaa", "aa");
	assert(p && strncmp(p, "aaa", 3) == 0);

	p = find_substring("abc", "");
	assert(p && strcmp(p, "abc") == 0);

	p = find_substring("", "x");
	assert(p == NULL);

	p = find_substring("abc", "abcd");
	assert(p == NULL);

	p = find_substring("abc", "abc");
	assert(p && strcmp(p, "abc") == 0);

	printf("✅ Problem #10 find_substring passed\n");
	return 0;
}
