// SPDX-License-Identifier: GPL-2.0
/* Problem #13 — ring_buffer 參考解 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct ring {
	int *data;
	size_t cap;
	size_t head;
	size_t tail;
	size_t count;
};

static int ring_init(struct ring *r, size_t cap)
{
	if (cap == 0)
		return -1;
	r->data = malloc(cap * sizeof(*r->data));
	if (!r->data)
		return -1;
	r->cap = cap;
	r->head = r->tail = r->count = 0;
	return 0;
}

static int ring_push(struct ring *r, int v)
{
	if (r->count == r->cap)
		return -1;
	r->data[r->head] = v;
	r->head = (r->head + 1) % r->cap;
	r->count++;
	return 0;
}

static int ring_pop(struct ring *r, int *out)
{
	if (r->count == 0)
		return -1;
	*out = r->data[r->tail];
	r->tail = (r->tail + 1) % r->cap;
	r->count--;
	return 0;
}

static void ring_free(struct ring *r)
{
	free(r->data);
	r->data = NULL;
	r->cap = r->head = r->tail = r->count = 0;
}

int main(void)
{
	struct ring r;
	assert(ring_init(&r, 4) == 0);

	assert(ring_push(&r, 1) == 0);
	assert(ring_push(&r, 2) == 0);
	assert(ring_push(&r, 3) == 0);
	assert(ring_push(&r, 4) == 0);
	assert(ring_push(&r, 5) != 0);

	int v;
	assert(ring_pop(&r, &v) == 0 && v == 1);
	assert(ring_pop(&r, &v) == 0 && v == 2);

	assert(ring_push(&r, 5) == 0);
	assert(ring_push(&r, 6) == 0);
	assert(ring_push(&r, 7) != 0);

	assert(ring_pop(&r, &v) == 0 && v == 3);
	assert(ring_pop(&r, &v) == 0 && v == 4);
	assert(ring_pop(&r, &v) == 0 && v == 5);
	assert(ring_pop(&r, &v) == 0 && v == 6);
	assert(ring_pop(&r, &v) != 0);

	ring_free(&r);
	printf("✅ Problem #13 ring_buffer passed\n");
	return 0;
}
