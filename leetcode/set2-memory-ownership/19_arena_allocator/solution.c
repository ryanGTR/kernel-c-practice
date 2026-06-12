// SPDX-License-Identifier: GPL-2.0
/* Problem #19 — arena_allocator 參考解 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct arena {
	char *block;
	size_t cap;
	size_t offset;
};

static int arena_init(struct arena *a, size_t cap)
{
	a->block = malloc(cap);
	if (!a->block) return -1;
	a->cap = cap;
	a->offset = 0;
	return 0;
}

static void *arena_alloc(struct arena *a, size_t n)
{
	/* 簡化版：不處理 alignment */
	if (a->offset + n > a->cap)
		return NULL;
	void *p = a->block + a->offset;
	a->offset += n;
	return p;
}

static void arena_reset(struct arena *a)
{
	a->offset = 0;
}

static void arena_destroy(struct arena *a)
{
	free(a->block);
	memset(a, 0, sizeof(*a));
}

int main(void)
{
	struct arena a;
	assert(arena_init(&a, 256) == 0);

	int *p = arena_alloc(&a, sizeof(int));
	assert(p); *p = 42;

	char *s = arena_alloc(&a, 100);
	assert(s); strcpy(s, "hello arena");

	int *q = arena_alloc(&a, sizeof(int));
	assert(q); *q = 99;
	assert(*p == 42);

	void *huge = arena_alloc(&a, 200);
	assert(!huge);

	arena_reset(&a);
	void *q2 = arena_alloc(&a, 100);
	assert(q2 == a.block);

	arena_destroy(&a);
	printf("✅ Problem #19 arena_allocator passed\n");
	return 0;
}
