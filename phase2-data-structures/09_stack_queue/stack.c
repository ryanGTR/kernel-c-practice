// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>
#include "stack.h"

#define ASTACK_INIT_CAP 4

/* ---------- array-backed ---------- */

struct astack *astack_create(void)
{
	struct astack *as = malloc(sizeof(struct astack));

	if (!as)
		return NULL;

	as->data = malloc(sizeof(*as->data) * ASTACK_INIT_CAP);

	if (!as->data) {
		free(as);
		return NULL;
	}

	as->size = 0;
	as->capacity = ASTACK_INIT_CAP;

	return as;
}

int astack_push(struct astack *s, int value)
{
	if (!s)
		return -1;

	if (s->size == s->capacity) {
		void *tmp =
			realloc(s->data, sizeof(*s->data) * s->capacity * 2);

		if (!tmp)
			return -1;

		s->data = tmp;
		s->capacity = s->capacity * 2;
	}

	s->data[s->size] = value;
	s->size++;

	return 0;
}

int astack_pop(struct astack *s, int *out)
{
	if (!s || !s->size)
		return -1;

	*out = s->data[--s->size];

	return 0;
}

int astack_peek(struct astack *s, int *out)
{
	if (!s || !s->size)
		return -1;

	*out = s->data[s->size - 1];

	return 0;
}

size_t astack_size(struct astack *s)
{
	return s->size;
}

void astack_destroy(struct astack *s)
{
	if (s) {
		free(s->data);
		free(s);
	}
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
