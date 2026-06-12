// SPDX-License-Identifier: GPL-2.0
/* Problem #13 — ring_buffer */

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

static int  ring_init(struct ring *r, size_t cap) { (void)r; (void)cap; return -1; }
static int  ring_push(struct ring *r, int v)      { (void)r; (void)v; return -1; }
static int  ring_pop(struct ring *r, int *out)    { (void)r; (void)out; return -1; }
static void ring_free(struct ring *r)             { (void)r; }

int main(void)
{
	struct ring r;
	assert(ring_init(&r, 4) == 0);

	assert(ring_push(&r, 1) == 0);
	assert(ring_push(&r, 2) == 0);
	assert(ring_push(&r, 3) == 0);
	assert(ring_push(&r, 4) == 0);
	assert(ring_push(&r, 5) != 0);   /* 滿 */

	int v;
	assert(ring_pop(&r, &v) == 0 && v == 1);
	assert(ring_pop(&r, &v) == 0 && v == 2);

	assert(ring_push(&r, 5) == 0);   /* 有位 */
	assert(ring_push(&r, 6) == 0);
	assert(ring_push(&r, 7) != 0);   /* 又滿 */

	assert(ring_pop(&r, &v) == 0 && v == 3);
	assert(ring_pop(&r, &v) == 0 && v == 4);
	assert(ring_pop(&r, &v) == 0 && v == 5);
	assert(ring_pop(&r, &v) == 0 && v == 6);
	assert(ring_pop(&r, &v) != 0);   /* 空 */

	ring_free(&r);
	printf("✅ Problem #13 ring_buffer passed\n");
	return 0;
}
