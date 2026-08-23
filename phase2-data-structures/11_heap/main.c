// SPDX-License-Identifier: GPL-2.0
/* RED against the stubs on purpose -- see ../../TODO.md. */
#include <stdio.h>
#include <assert.h>
#include "heap.h"

static void test_push_pop(void)
{
	struct heap *h = heap_create();
	static const int in[] = { 5, 3, 8, 1, 9, 2, 7, 4, 6, 0 };
	int v, prev;

	assert(h);
	assert(heap_pop(h, &v) == -1);		/* empty */

	for (size_t i = 0; i < 10; i++)
		assert(heap_push(h, in[i]) == 0);
	assert(heap_size(h) == 10);
	assert(heap_peek(h, &v) == 0 && v == 0);

	prev = -1;
	for (size_t i = 0; i < 10; i++) {	/* pops come out sorted */
		assert(heap_pop(h, &v) == 0);
		assert(v == (int)i);
		assert(v > prev);
		prev = v;
	}
	assert(heap_size(h) == 0);
	assert(heap_pop(h, &v) == -1);

	heap_destroy(h);
	heap_destroy(NULL);
	printf("push/pop ok\n");
}

static void test_duplicates_and_grow(void)
{
	struct heap *h = heap_create();
	int v, prev = -1;

	assert(h);
	for (int i = 0; i < 1000; i++)		/* many grows */
		assert(heap_push(h, (i * 7919) % 101) == 0);	/* lots of dups */
	assert(heap_size(h) == 1000);

	for (int i = 0; i < 1000; i++) {
		assert(heap_pop(h, &v) == 0);
		assert(v >= prev);			/* non-decreasing, dups ok */
		prev = v;
	}
	heap_destroy(h);
	printf("duplicates/grow ok\n");
}

static void test_interleaved(void)
{
	struct heap *h = heap_create();
	int v;

	/* push/pop mixed: the heap must stay valid between operations */
	heap_push(h, 10);
	heap_push(h, 20);
	assert(heap_pop(h, &v) == 0 && v == 10);
	heap_push(h, 5);
	heap_push(h, 15);
	assert(heap_pop(h, &v) == 0 && v == 5);
	assert(heap_pop(h, &v) == 0 && v == 15);
	assert(heap_pop(h, &v) == 0 && v == 20);
	assert(heap_pop(h, &v) == -1);
	heap_destroy(h);
	printf("interleaved ok\n");
}

int main(void)
{
	printf("=== Heap Tests ===\n");
	test_push_pop();
	test_duplicates_and_grow();
	test_interleaved();

	/* TODO (bonus): heap_build on {9,8,...,0} then heap_is_valid == 1 */
	/* TODO (bonus): heap_sort on a random array, compare with qsort */

	printf("\nAll tests passed!\n");
	return 0;
}
