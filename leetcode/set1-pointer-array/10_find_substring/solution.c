// SPDX-License-Identifier: GPL-2.0
/* Problem #10 — find_substring 參考解（O(n*m) 簡單版）*/

#include <stdio.h>
#include <string.h>
#include <assert.h>

static const char *find_substring(const char *haystack, const char *needle)
{
	if (!*needle)
		return haystack;

	for (; *haystack; haystack++) {
		const char *h = haystack;
		const char *n = needle;
		while (*h && *n && *h == *n) {
			h++;
			n++;
		}
		if (!*n)
			return haystack;
	}
	return NULL;
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
