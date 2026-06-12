// SPDX-License-Identifier: GPL-2.0
/* Problem #09 — count_words 參考解 */

#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include <assert.h>

static size_t count_words(const char *s)
{
	int in_word = 0;
	size_t count = 0;
	for (; *s; s++) {
		if (isspace((unsigned char)*s)) {
			in_word = 0;
		} else if (!in_word) {
			in_word = 1;
			count++;
		}
	}
	return count;
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
