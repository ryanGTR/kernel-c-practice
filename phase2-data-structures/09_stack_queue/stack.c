// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>
#include "stack.h"

#define ASTACK_INIT_CAP 4

/* ---------- array-backed ---------- */

struct astack *astack_create(void)
{
	/* TODO: allocate struct + data (ASTACK_INIT_CAP) */
	return NULL;
}

int astack_push(struct astack *s, int value)
{
	/* TODO: grow (double) when size == capacity; realloc into a temp first */
	(void)s;
	(void)value;
	return -1;
}

int astack_pop(struct astack *s, int *out)
{
	/* TODO: -1 if empty; else *out = data[--size] */
	(void)s;
	(void)out;
	return -1;
}

int astack_peek(struct astack *s, int *out)
{
	/* TODO */
	(void)s;
	(void)out;
	return -1;
}

size_t astack_size(struct astack *s)
{
	/* TODO */
	(void)s;
	return 0;
}

void astack_destroy(struct astack *s)
{
	/* TODO: free data, then struct; must accept NULL */
	(void)s;
}

/* ---------- linked-list-backed ---------- */

struct lstack *lstack_create(void)
{
	/* TODO */
	return NULL;
}

int lstack_push(struct lstack *s, int value)
{
	/* TODO: new node becomes top; O(1), no traversal */
	(void)s;
	(void)value;
	return -1;
}

int lstack_pop(struct lstack *s, int *out)
{
	/* TODO: unlink top, free it, -1 if empty */
	(void)s;
	(void)out;
	return -1;
}

int lstack_peek(struct lstack *s, int *out)
{
	/* TODO */
	(void)s;
	(void)out;
	return -1;
}

size_t lstack_size(struct lstack *s)
{
	/* TODO */
	(void)s;
	return 0;
}

void lstack_destroy(struct lstack *s)
{
	/* TODO: free every node (save next before free!), then struct */
	(void)s;
}
