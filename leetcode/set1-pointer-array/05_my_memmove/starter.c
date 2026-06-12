// SPDX-License-Identifier: GPL-2.0
/* Problem #05 — my_memmove（處理重疊）*/

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

static void *my_memmove(void *dst, const void *src, size_t n)
{
	(void)dst;
	(void)src;
	(void)n;
	return NULL;   /* TODO */
}

int main(void)
{
	/* 不重疊 */
	char buf[10];
	my_memmove(buf, "hello", 6);
	assert(strcmp(buf, "hello") == 0);

	/* 往後搬（dst > src）：buf -> buf+1 */
	char a[] = "abcdef";
	my_memmove(a + 1, a, 5);
	assert(strncmp(a, "aabcde", 6) == 0);

	/* 往前搬（dst < src）：buf+1 -> buf */
	char b[] = "abcdef";
	my_memmove(b, b + 1, 5);
	assert(strncmp(b, "bcdef", 5) == 0);

	/* n = 0 */
	char c[] = "xyz";
	my_memmove(c, c + 1, 0);
	assert(strcmp(c, "xyz") == 0);

	printf("✅ Problem #05 my_memmove passed\n");
	return 0;
}
