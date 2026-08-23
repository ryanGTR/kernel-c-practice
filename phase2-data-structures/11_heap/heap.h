/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

/*
 * Binary MIN-heap stored in a plain array. No pointers anywhere:
 *   parent(i)      = (i - 1) / 2
 *   left child(i)  = 2 * i + 1
 *   right child(i) = 2 * i + 2
 * The tree shape is implied by the indices -- that is the whole trick.
 */
struct heap {
	int *data;
	size_t size;
	size_t capacity;
};

struct heap *heap_create(void);
int heap_push(struct heap *h, int value);	/* 0 ok, -1 alloc fail */
int heap_pop(struct heap *h, int *out);		/* 0 ok, -1 if empty; *out = min */
int heap_peek(struct heap *h, int *out);
size_t heap_size(struct heap *h);
void heap_destroy(struct heap *h);

/* Bonus */
struct heap *heap_build(const int *arr, size_t n);	/* O(n) heapify */
void heap_sort(int *arr, size_t n);			/* in place, ascending */
int heap_is_valid(struct heap *h);			/* 1 if heap property holds */

#endif /* HEAP_H */
