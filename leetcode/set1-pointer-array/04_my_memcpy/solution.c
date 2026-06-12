// SPDX-License-Identifier: GPL-2.0
/* Problem #04 — my_memcpy 參考解 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

static void *my_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char *d = dst;
	const unsigned char *s = src;
	while (n--)
		*d++ = *s++;
	return dst;
}

int main(void)
{
	char buf[10];
	assert(my_memcpy(buf, "hello", 6) == buf);
	assert(strcmp(buf, "hello") == 0);

	int a[3] = {1, 2, 3}, b[3] = {0};
	my_memcpy(b, a, sizeof a);
	assert(b[0] == 1 && b[1] == 2 && b[2] == 3);

	char before[] = "xxx";
	my_memcpy(before, "yyy", 0);
	assert(strcmp(before, "xxx") == 0);

	printf("✅ Problem #04 my_memcpy passed\n");
	return 0;
}
