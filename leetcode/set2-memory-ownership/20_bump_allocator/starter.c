// SPDX-License-Identifier: GPL-2.0
/* Problem #20 — bump_allocator */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#define BUMP_SIZE 4096

static char bump_buf[BUMP_SIZE];
static size_t bump_offset = 0;

static void *bump_alloc(size_t n)
{
	(void)n;
	return NULL;   /* TODO */
}

static size_t bump_used(void)
{
	return bump_offset;
}

int main(void)
{
	int *p = bump_alloc(sizeof(int));
	assert(p);
	*p = 42;

	char *s = bump_alloc(100);
	assert(s);
	strcpy(s, "bump");

	int *q = bump_alloc(sizeof(int));
	assert(q);
	*q = 99;

	assert(*p == 42 && *q == 99);   /* 之前的還有效 */

	void *huge = bump_alloc(BUMP_SIZE);   /* 配滿肯定爆 */
	assert(!huge);

	printf("bump_used = %zu bytes\n", bump_used());
	printf("✅ Problem #20 bump_allocator passed\n");
	return 0;
}
