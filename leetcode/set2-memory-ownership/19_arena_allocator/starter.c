// SPDX-License-Identifier: GPL-2.0
/* Problem #19 — arena_allocator */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct arena {
	char *block;
	size_t cap;
	size_t offset;
};

static int   arena_init(struct arena *a, size_t cap)        { (void)a; (void)cap; return -1; }
static void *arena_alloc(struct arena *a, size_t n)         { (void)a; (void)n; return NULL; }
static void  arena_reset(struct arena *a)                   { (void)a; }
static void  arena_destroy(struct arena *a)                 { (void)a; }

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
	assert(*p == 42);   /* 之前的 alloc 仍有效 */

	void *huge = arena_alloc(&a, 200);
	assert(!huge);   /* 不夠 */

	arena_reset(&a);
	void *q2 = arena_alloc(&a, 100);
	assert(q2 == a.block);   /* reset 後從頭開始 */

	arena_destroy(&a);
	printf("✅ Problem #19 arena_allocator passed\n");
	return 0;
}
