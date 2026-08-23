// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>
#include "heap.h"

#define HEAP_INIT_CAP 4

static void swap(int *a, int *b)
{
	int t = *a;

	*a = *b;
	*b = t;
}

static void sift_up(struct heap *h, size_t i)
{
	/* TODO: while i > 0 and data[i] < data[parent], swap and move up */
	(void)h;
	(void)i;
	(void)swap;
}

static void sift_down(struct heap *h, size_t i)
{
	/*
	 * TODO: pick the smaller child; if it is smaller than data[i], swap
	 * and continue from there. Careful: right child may not exist.
	 */
	(void)h;
	(void)i;
}

struct heap *heap_create(void)
{
	/* TODO */
	return NULL;
}

int heap_push(struct heap *h, int value)
{
	/* TODO: grow if needed; append at data[size]; size++; sift_up(size-1) */
	(void)h;
	(void)value;
	(void)sift_up;
	return -1;
}

int heap_pop(struct heap *h, int *out)
{
	/*
	 * TODO: *out = data[0]; move data[size-1] to data[0]; size--;
	 * sift_down(0). Empty -> -1.
	 */
	(void)h;
	(void)out;
	(void)sift_down;
	return -1;
}

int heap_peek(struct heap *h, int *out)
{
	/* TODO */
	(void)h;
	(void)out;
	return -1;
}

size_t heap_size(struct heap *h)
{
	/* TODO */
	(void)h;
	return 0;
}

void heap_destroy(struct heap *h)
{
	/* TODO */
	(void)h;
}

struct heap *heap_build(const int *arr, size_t n)
{
	/*
	 * TODO (bonus): copy arr, then sift_down from the last internal node
	 * (n/2 - 1) down to 0. Why is this O(n) and not O(n log n)?
	 */
	(void)arr;
	(void)n;
	return NULL;
}

void heap_sort(int *arr, size_t n)
{
	/*
	 * TODO (bonus): build a MAX-heap in place, then repeatedly swap
	 * arr[0] with arr[end] and sift_down within [0, end).
	 */
	(void)arr;
	(void)n;
}

int heap_is_valid(struct heap *h)
{
	/* TODO (bonus): every child >= its parent */
	(void)h;
	return 0;
}
