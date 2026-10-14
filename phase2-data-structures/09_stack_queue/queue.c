// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>
#include "queue.h"

#define AQUEUE_INIT_CAP 4

/* ---------- array-backed ring buffer ---------- */

struct aqueue *aqueue_create(void)
{
	/* TODO */
	struct aqueue *aq = malloc(sizeof(*aq));

	if (!aq)
		return NULL;

	aq->capacity = AQUEUE_INIT_CAP;

	aq->data = malloc(AQUEUE_INIT_CAP * sizeof(*aq->data));
	if (!aq->data) {
		free(aq);
		return NULL;
	}
	aq->size = 0;
	aq->head = 0;
	return aq;
}

int aqueue_enqueue(struct aqueue *q, int value)
{
	/*
	 * TODO: if full, grow. Growing a ring buffer is NOT a plain realloc:
	 * the live elements may wrap around the end. Copy them in order into
	 * the new buffer starting at index 0, then head = 0.
	 * Then data[(head + size) % capacity] = value; size++.
	 */

	if (!q)
		return -1;

	if (q->size < q->capacity) {
		q->data[(q->head + q->size) % q->capacity] = value;
		q->size++;
	} else {
		size_t new_cap = 2 * q->capacity;
		int *new_data = malloc(new_cap * sizeof(*new_data));

		if (!new_data)
			return -1;

		for (size_t i = 0; i < q->size; i++)
			new_data[i] = q->data[(q->head + i) % q->capacity];

		free(q->data);
		q->data = new_data;
		q->capacity = new_cap;
		q->head = 0;

		q->data[(q->head + q->size) % q->capacity] = value;
		q->size++;
	}

	return 0;
}

int aqueue_dequeue(struct aqueue *q, int *out)
{
	/* TODO: -1 if empty; *out = data[head]; head = (head + 1) % cap */

	if (!q || q->size == 0)
		return -1;

	*out = q->data[q->head];

	q->head = (q->head + 1) % q->capacity;

	q->size--;

	return 0;
}

int aqueue_peek(struct aqueue *q, int *out)
{
	if (!q || !q->size)
		return -1;

	*out = q->data[q->head];

	return 0;
}

size_t aqueue_size(struct aqueue *q)
{
	return q->size;
}

void aqueue_destroy(struct aqueue *q)
{
	if (q) {
		free(q->data);
		free(q);
	}
}

/* ---------- linked-list-backed ---------- */

struct lqueue *lqueue_create(void)
{
	/* TODO */
	return NULL;
}

int lqueue_enqueue(struct lqueue *q, int value)
{
	/* TODO: append at tail; handle the empty case (head == tail == NULL) */
	(void)q;
	(void)value;
	return -1;
}

int lqueue_dequeue(struct lqueue *q, int *out)
{
	/* TODO: pop head; if it was the last node, tail must become NULL too */
	(void)q;
	(void)out;
	return -1;
}

int lqueue_peek(struct lqueue *q, int *out)
{
	/* TODO */
	(void)q;
	(void)out;
	return -1;
}

size_t lqueue_size(struct lqueue *q)
{
	/* TODO */
	(void)q;
	return 0;
}

void lqueue_destroy(struct lqueue *q)
{
	/* TODO */
	(void)q;
}
