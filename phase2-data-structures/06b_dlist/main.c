// SPDX-License-Identifier: GPL-2.0
/*
 * These tests are RED against the stubs on purpose. A test that has never
 * failed proves nothing.
 *
 * Every check walks the list TWICE -- forward via next, backward via prev.
 * A forgotten prev pointer passes the forward walk and fails the backward
 * one. That is the bug this station is about.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dlist.h"

/* The list must read exactly want[0..n-1] forward, and the reverse backward. */
static void expect(const struct dlist *l, const char *const want[], int n)
{
	const struct dnode *p;
	int i;

	assert(dl_size(l) == n);
	if (n == 0) {
		assert(!l->head && !l->tail);
		return;
	}
	assert(!l->head->prev);			/* ends are NULL, not circular */
	assert(!l->tail->next);

	i = 0;
	for (p = l->head; p; p = p->next)
		assert(i < n && strcmp(p->value, want[i++]) == 0);
	assert(i == n);

	i = n;
	for (p = l->tail; p; p = p->prev)
		assert(i > 0 && strcmp(p->value, want[--i]) == 0);
	assert(i == 0);
}

static void take(char *got, const char *want)
{
	assert(got && strcmp(got, want) == 0);
	free(got);				/* remove_* hands ownership to us */
}

static void test_empty(void)
{
	struct dlist *l = dl_new();

	assert(l);
	expect(l, NULL, 0);
	assert(!dl_remove_head(l));
	assert(!dl_remove_tail(l));
	assert(!dl_delete_mid(l));
	dl_free(l);
	dl_free(NULL);				/* must be a no-op */
	printf("  empty list: ok\n");
}

static void test_insert(void)
{
	struct dlist *l = dl_new();
	char buf[] = "bear";

	assert(dl_insert_head(l, "b"));
	expect(l, (const char *[]){ "b" }, 1);	/* head == tail */
	assert(dl_insert_head(l, "a"));
	assert(dl_insert_tail(l, "c"));
	expect(l, (const char *[]){ "a", "b", "c" }, 3);

	/* the list must keep its OWN copy, not the caller's pointer */
	assert(dl_insert_tail(l, buf));
	buf[0] = 'X';
	expect(l, (const char *[]){ "a", "b", "c", "bear" }, 4);

	dl_free(l);
	printf("  insert head/tail: ok\n");
}

static void test_remove(void)
{
	struct dlist *l = dl_new();

	dl_insert_tail(l, "a");
	dl_insert_tail(l, "b");
	dl_insert_tail(l, "c");

	take(dl_remove_head(l), "a");
	expect(l, (const char *[]){ "b", "c" }, 2);
	take(dl_remove_tail(l), "c");
	expect(l, (const char *[]){ "b" }, 1);
	take(dl_remove_tail(l), "b");		/* last one: head AND tail go NULL */
	expect(l, NULL, 0);

	assert(dl_insert_head(l, "again"));	/* still usable after emptying */
	expect(l, (const char *[]){ "again" }, 1);

	dl_free(l);
	printf("  remove head/tail: ok\n");
}

static void test_delete_mid(void)
{
	struct dlist *l = dl_new();
	static const char *const five[] = { "a", "b", "c", "d", "e" };
	int i;

	for (i = 0; i < 5; i++)
		dl_insert_tail(l, five[i]);

	assert(dl_delete_mid(l));		/* n=5 -> index 2 */
	expect(l, (const char *[]){ "a", "b", "d", "e" }, 4);
	assert(dl_delete_mid(l));		/* n=4 -> index 2 */
	expect(l, (const char *[]){ "a", "b", "e" }, 3);
	assert(dl_delete_mid(l));		/* n=3 -> index 1 */
	expect(l, (const char *[]){ "a", "e" }, 2);
	assert(dl_delete_mid(l));		/* n=2 -> index 1: the TAIL */
	expect(l, (const char *[]){ "a" }, 1);
	assert(dl_delete_mid(l));		/* n=1 -> index 0: head AND tail */
	expect(l, NULL, 0);
	assert(!dl_delete_mid(l));

	dl_free(l);
	printf("  delete mid: ok\n");
}

static void test_free_nonempty(void)
{
	struct dlist *l = dl_new();

	dl_insert_tail(l, "x");
	dl_insert_tail(l, "y");
	dl_insert_tail(l, "z");
	dl_free(l);				/* `make asan` must report no leak */
	printf("  free non-empty: ok (confirm with `make asan`)\n");
}

int main(void)
{
	printf("=== Doubly Linked List (textbook) ===\n\n");
	test_empty();
	test_insert();
	test_remove();
	test_delete_mid();
	test_free_nonempty();
	printf("\nAll tests passed!  Now run `make asan` -- that is the real judge.\n");
	return 0;
}
