// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>
#include "queue.h"

#define AQUEUE_INIT_CAP 4

/* ---------- array-backed ring buffer ---------- */

struct aqueue *aqueue_create(void)
{
	/* TODO */
	return NULL;
}

int aqueue_enqueue(struct aqueue *q, int value)
{
	/*
	 * TODO: if full, grow. Growing a ring buffer is NOT a plain realloc:
	 * the live elements may wrap around the end. Copy them in order into
	 * the new buffer starting at index 0, then head = 0.
	 * Then data[(head + size) % capacity] = value; size++.
	 */
	(void)q;
	(void)value;
	return -1;
}

int aqueue_dequeue(struct aqueue *q, int *out)
{
	/* TODO: -1 if empty; *out = data[head]; head = (head + 1) % cap */
	(void)q;
	(void)out;
	return -1;
}

int aqueue_peek(struct aqueue *q, int *out)
{
	/* TODO */
	(void)q;
	(void)out;
	return -1;
}

size_t aqueue_size(struct aqueue *q)
{
	/* TODO */
	(void)q;
	return 0;
}

void aqueue_destroy(struct aqueue *q)
{
	/* TODO */
	(void)q;
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
