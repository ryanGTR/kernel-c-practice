// SPDX-License-Identifier: GPL-2.0
/* Problem #05 — my_memmove 參考解 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

static void *my_memmove(void *dst, const void *src, size_t n)
{
	unsigned char *d = dst;
	const unsigned char *s = src;

	if (d == s || n == 0)
		return dst;

	if (d < s) {
		/* dst 在前面，從前往後搬 */
		while (n--)
			*d++ = *s++;
	} else {
		/* dst 在後面，從後往前搬，避免覆蓋 */
		d += n;
		s += n;
		while (n--)
			*--d = *--s;
	}
	return dst;
}

int main(void)
{
	char buf[10];
	my_memmove(buf, "hello", 6);
	assert(strcmp(buf, "hello") == 0);

	char a[] = "abcdef";
	my_memmove(a + 1, a, 5);
	assert(strncmp(a, "aabcde", 6) == 0);

	char b[] = "abcdef";
	my_memmove(b, b + 1, 5);
	assert(strncmp(b, "bcdef", 5) == 0);

	char c[] = "xyz";
	my_memmove(c, c + 1, 0);
	assert(strcmp(c, "xyz") == 0);

	printf("✅ Problem #05 my_memmove passed\n");
	return 0;
}
