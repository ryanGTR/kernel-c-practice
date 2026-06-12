// SPDX-License-Identifier: GPL-2.0
/* Problem #03 — my_strscpy 參考解 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <assert.h>

#define E2BIG_NEG -7

static ssize_t my_strscpy(char *dst, const char *src, size_t size)
{
	if (size == 0)
		return E2BIG_NEG;

	size_t i = 0;
	while (i < size - 1 && src[i]) {
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';

	return src[i] ? E2BIG_NEG : (ssize_t)i;
}

int main(void)
{
	char buf[10];

	assert(my_strscpy(buf, "hello", 10) == 5);
	assert(strcmp(buf, "hello") == 0);

	assert(my_strscpy(buf, "hello", 3) == E2BIG_NEG);
	assert(strcmp(buf, "he") == 0);

	assert(my_strscpy(buf, "", 10) == 0);
	assert(buf[0] == '\0');

	assert(my_strscpy(buf, "exact", 6) == 5);
	assert(strcmp(buf, "exact") == 0);

	assert(my_strscpy(buf, "abc", 0) == E2BIG_NEG);

	printf("✅ Problem #03 my_strscpy passed\n");
	return 0;
}
