// SPDX-License-Identifier: GPL-2.0
/* Problem #15 — queue (FIFO) */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct queue { int *data; size_t cap; size_t head; size_t tail; size_t count; };

static int  queue_init(struct queue *q, size_t cap)     { (void)q; (void)cap; return -1; }
static int  queue_enqueue(struct queue *q, int v)        { (void)q; (void)v; return -1; }
static int  queue_dequeue(struct queue *q, int *out)     { (void)q; (void)out; return -1; }
static void queue_free(struct queue *q)                  { (void)q; }

int main(void)
{
	struct queue q;
	assert(queue_init(&q, 4) == 0);

	assert(queue_enqueue(&q, 1) == 0);
	assert(queue_enqueue(&q, 2) == 0);
	assert(queue_enqueue(&q, 3) == 0);

	int v;
	assert(queue_dequeue(&q, &v) == 0 && v == 1);
	assert(queue_dequeue(&q, &v) == 0 && v == 2);

	assert(queue_enqueue(&q, 4) == 0);
	assert(queue_enqueue(&q, 5) == 0);

	assert(queue_dequeue(&q, &v) == 0 && v == 3);
	assert(queue_dequeue(&q, &v) == 0 && v == 4);
	assert(queue_dequeue(&q, &v) == 0 && v == 5);
	assert(queue_dequeue(&q, &v) != 0);   /* 空 */

	queue_free(&q);
	printf("✅ Problem #15 queue passed\n");
	return 0;
}
