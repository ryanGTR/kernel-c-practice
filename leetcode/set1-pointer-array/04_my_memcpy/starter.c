// SPDX-License-Identifier: GPL-2.0
/* Problem #04 — my_memcpy */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>

static void *my_memcpy(void *dst, const void *src, size_t n)
{
	(void)dst;
	(void)src;
	(void)n;
	return NULL;   /* TODO */
}

int main(void)
{
	char buf[10];
	assert(my_memcpy(buf, "hello", 6) == buf);
	assert(strcmp(buf, "hello") == 0);

	int a[3] = {1, 2, 3}, b[3] = {0};
	my_memcpy(b, a, sizeof a);
	assert(b[0] == 1 && b[1] == 2 && b[2] == 3);

	/* n = 0 */
	char before[] = "xxx";
	my_memcpy(before, "yyy", 0);
	assert(strcmp(before, "xxx") == 0);   /* 沒變 */

	printf("✅ Problem #04 my_memcpy passed\n");
	return 0;
}
