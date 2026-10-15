// SPDX-License-Identifier: GPL-2.0
/*
 * These tests are RED against the stubs on purpose. A test that has never
 * failed proves nothing (see ../../TODO.md, "驗收清單本身會有洞").
 */
#include <stdio.h>
#include <assert.h>
#include "stack.h"
#include "queue.h"

static void test_astack(void)
{
	struct astack *s = astack_create();
	int v;

	assert(s);
	assert(astack_size(s) == 0);
	assert(astack_pop(s, &v) == -1);		/* empty pop */

	for (int i = 0; i < 100; i++)		/* forces several grows */
		assert(astack_push(s, i) == 0);
	assert(astack_size(s) == 100);
	assert(astack_peek(s, &v) == 0 && v == 99);

	for (int i = 99; i >= 0; i--) {		/* LIFO order */
		assert(astack_pop(s, &v) == 0);
		assert(v == i);
	}
	assert(astack_size(s) == 0);
	assert(astack_pop(s, &v) == -1);

	astack_destroy(s);
	astack_destroy(NULL);			/* must be safe */
	printf("astack ok\n");
}

static void test_lstack(void)
{
	struct lstack *s = lstack_create();
	int v;

	assert(s);
	assert(lstack_pop(s, &v) == -1);
	for (int i = 0; i < 100; i++)
		assert(lstack_push(s, i) == 0);
	assert(lstack_size(s) == 100);
	for (int i = 99; i >= 0; i--) {
		assert(lstack_pop(s, &v) == 0);
		assert(v == i);
	}
	assert(lstack_pop(s, &v) == -1);

	/* destroy with live nodes: LSan catches a leak here */
	for (int i = 0; i < 10; i++)
		lstack_push(s, i);
	lstack_destroy(s);
	lstack_destroy(NULL);
	printf("lstack ok\n");
}

static void test_aqueue(void)
{
	struct aqueue *q = aqueue_create();
	int v;

	assert(q);
	assert(aqueue_dequeue(q, &v) == -1);

	/* wrap-around: fill, drain half, fill again so head != 0 when growing */
	for (int i = 0; i < 4; i++)
		assert(aqueue_enqueue(q, i) == 0);
	assert(aqueue_dequeue(q, &v) == 0 && v == 0);
	assert(aqueue_dequeue(q, &v) == 0 && v == 1);
	for (int i = 4; i < 50; i++)		/* crosses capacity, wraps */
		assert(aqueue_enqueue(q, i) == 0);
	assert(aqueue_size(q) == 48);
	for (int i = 2; i < 50; i++) {		/* FIFO order survives grow */
		assert(aqueue_dequeue(q, &v) == 0);
		assert(v == i);
	}
	assert(aqueue_dequeue(q, &v) == -1);

	aqueue_destroy(q);
	aqueue_destroy(NULL);
	printf("aqueue ok\n");
}

static void test_lqueue(void)
{
	struct lqueue *q = lqueue_create();
	int v;

	assert(q);
	assert(lqueue_dequeue(q, &v) == -1);
	assert(lqueue_enqueue(q, 1) == 0);
	assert(lqueue_dequeue(q, &v) == 0 && v == 1);	/* back to empty */
	assert(lqueue_enqueue(q, 2) == 0);		/* tail was reset? */
	assert(lqueue_enqueue(q, 3) == 0);
	assert(lqueue_size(q) == 2);
	assert(lqueue_peek(q, &v) == 0 && v == 2);
	assert(lqueue_dequeue(q, &v) == 0 && v == 2);
	assert(lqueue_dequeue(q, &v) == 0 && v == 3);
	assert(lqueue_size(q) == 0);

	for (int i = 0; i < 10; i++)
		lqueue_enqueue(q, i);
	lqueue_destroy(q);
	lqueue_destroy(NULL);
	printf("lqueue ok\n");
}

int main(void)
{
	printf("=== Stack / Queue Tests ===\n");
	test_astack();
	test_lstack();
	test_aqueue();
	test_lqueue();

	/* TODO: add one edge case of your own per structure, make it RED first */

	printf("\nAll tests passed!\n");
	return 0;
}
