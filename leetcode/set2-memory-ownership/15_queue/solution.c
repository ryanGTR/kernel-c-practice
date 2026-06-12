// SPDX-License-Identifier: GPL-2.0
/* Problem #15 — queue 參考解（同 #13 ring_buffer 結構）*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct queue { int *data; size_t cap; size_t head; size_t tail; size_t count; };

static int queue_init(struct queue *q, size_t cap)
{
	if (cap == 0) return -1;
	q->data = malloc(cap * sizeof(*q->data));
	if (!q->data) return -1;
	q->cap = cap; q->head = q->tail = q->count = 0;
	return 0;
}

static int queue_enqueue(struct queue *q, int v)
{
	if (q->count == q->cap) return -1;
	q->data[q->head] = v;
	q->head = (q->head + 1) % q->cap;
	q->count++;
	return 0;
}

static int queue_dequeue(struct queue *q, int *out)
{
	if (q->count == 0) return -1;
	*out = q->data[q->tail];
	q->tail = (q->tail + 1) % q->cap;
	q->count--;
	return 0;
}

static void queue_free(struct queue *q)
{
	free(q->data); q->data = NULL;
	q->cap = q->head = q->tail = q->count = 0;
}

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
	assert(queue_dequeue(&q, &v) != 0);

	queue_free(&q);
	printf("✅ Problem #15 queue passed\n");
	return 0;
}
