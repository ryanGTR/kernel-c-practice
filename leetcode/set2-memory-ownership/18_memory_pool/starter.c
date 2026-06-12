// SPDX-License-Identifier: GPL-2.0
/* Problem #18 — memory_pool */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

struct mempool {
	void *block;
	size_t obj_size;
	size_t capacity;
	size_t allocated;
	void *free_list;
};

static int   mempool_init(struct mempool *p, size_t obj_size, size_t cap) {
	(void)p; (void)obj_size; (void)cap; return -1;
}
static void *mempool_alloc(struct mempool *p)               { (void)p; return NULL; }
static void  mempool_free(struct mempool *p, void *obj)      { (void)p; (void)obj; }
static void  mempool_destroy(struct mempool *p)              { (void)p; }

int main(void)
{
	struct mempool p;
	assert(mempool_init(&p, sizeof(int), 4) == 0);

	int *a = mempool_alloc(&p); assert(a);
	int *b = mempool_alloc(&p); assert(b && b != a);
	int *c = mempool_alloc(&p); assert(c);
	int *d = mempool_alloc(&p); assert(d);
	int *e = mempool_alloc(&p); assert(!e);   /* 池滿 */

	*a = 100; *b = 200;
	assert(*a == 100 && *b == 200);

	mempool_free(&p, b);
	int *b2 = mempool_alloc(&p);
	assert(b2 == b);   /* free 後重用 */

	mempool_destroy(&p);
	printf("✅ Problem #18 memory_pool passed\n");
	return 0;
}
