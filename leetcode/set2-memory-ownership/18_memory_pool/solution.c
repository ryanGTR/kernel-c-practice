// SPDX-License-Identifier: GPL-2.0
/* Problem #18 — memory_pool 參考解 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

struct mempool {
	void *block;
	size_t obj_size;
	size_t capacity;
	size_t allocated;
	void *free_list;
};

static int mempool_init(struct mempool *p, size_t obj_size, size_t cap)
{
	if (obj_size < sizeof(void *))
		obj_size = sizeof(void *);   /* 至少要能放 next pointer */

	p->block = malloc(obj_size * cap);
	if (!p->block) return -1;

	p->obj_size = obj_size;
	p->capacity = cap;
	p->allocated = 0;

	/* 把 block 切成 cap 個格子，串成 free list */
	char *q = p->block;
	for (size_t i = 0; i < cap - 1; i++) {
		void **slot = (void **)(q + i * obj_size);
		*slot = q + (i + 1) * obj_size;
	}
	*(void **)(q + (cap - 1) * obj_size) = NULL;

	p->free_list = p->block;
	return 0;
}

static void *mempool_alloc(struct mempool *p)
{
	if (!p->free_list) return NULL;
	void *obj = p->free_list;
	p->free_list = *(void **)p->free_list;
	p->allocated++;
	return obj;
}

static void mempool_free(struct mempool *p, void *obj)
{
	*(void **)obj = p->free_list;
	p->free_list = obj;
	p->allocated--;
}

static void mempool_destroy(struct mempool *p)
{
	free(p->block);
	memset(p, 0, sizeof(*p));
}

int main(void)
{
	struct mempool p;
	assert(mempool_init(&p, sizeof(int), 4) == 0);

	int *a = mempool_alloc(&p); assert(a);
	int *b = mempool_alloc(&p); assert(b && b != a);
	int *c = mempool_alloc(&p); assert(c);
	int *d = mempool_alloc(&p); assert(d);
	int *e = mempool_alloc(&p); assert(!e);

	*a = 100; *b = 200;
	assert(*a == 100 && *b == 200);

	mempool_free(&p, b);
	int *b2 = mempool_alloc(&p);
	assert(b2 == b);

	mempool_destroy(&p);
	printf("✅ Problem #18 memory_pool passed\n");
	return 0;
}
