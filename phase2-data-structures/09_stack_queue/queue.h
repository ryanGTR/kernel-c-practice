/* SPDX-License-Identifier: GPL-2.0 */
#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

/*
 * Two queues, same interface, different storage.
 *
 * The array version MUST be a ring buffer (head index + count, wrap with %).
 * A naive "shift everything left on dequeue" is O(n) per op -- that is the
 * trap this station exists to show you.
 */

/* --- array-backed queue (ring buffer, grows when full) --- */
struct aqueue {
	int *data;
	size_t head;		/* index of the front element */
	size_t size;		/* number of elements */
	size_t capacity;
};

struct aqueue *aqueue_create(void);
int aqueue_enqueue(struct aqueue *q, int value);	/* 0 ok, -1 alloc fail */
int aqueue_dequeue(struct aqueue *q, int *out);		/* 0 ok, -1 if empty */
int aqueue_peek(struct aqueue *q, int *out);
size_t aqueue_size(struct aqueue *q);
void aqueue_destroy(struct aqueue *q);

/* --- linked-list-backed queue (enqueue at tail, dequeue at head) --- */
struct qnode {
	int data;
	struct qnode *next;
};

struct lqueue {
	struct qnode *head;
	struct qnode *tail;
	size_t size;
};

struct lqueue *lqueue_create(void);
int lqueue_enqueue(struct lqueue *q, int value);
int lqueue_dequeue(struct lqueue *q, int *out);
int lqueue_peek(struct lqueue *q, int *out);
size_t lqueue_size(struct lqueue *q);
void lqueue_destroy(struct lqueue *q);

#endif /* QUEUE_H */
