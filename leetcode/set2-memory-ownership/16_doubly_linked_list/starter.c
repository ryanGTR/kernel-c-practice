// SPDX-License-Identifier: GPL-2.0
/* Problem #16 — doubly_linked_list */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

struct dlist_node {
	int value;
	struct dlist_node *prev;
	struct dlist_node *next;
};

struct dlist {
	struct dlist_node *head;
	struct dlist_node *tail;
	size_t len;
};

static void dlist_init(struct dlist *l)              { (void)l; }
static int  dlist_push_front(struct dlist *l, int v) { (void)l; (void)v; return -1; }
static int  dlist_push_back(struct dlist *l, int v)  { (void)l; (void)v; return -1; }
static int  dlist_pop_front(struct dlist *l, int *out){ (void)l; (void)out; return -1; }
static int  dlist_pop_back(struct dlist *l, int *out) { (void)l; (void)out; return -1; }
static void dlist_free(struct dlist *l)              { (void)l; }

int main(void)
{
	struct dlist l;
	dlist_init(&l);
	assert(l.head == NULL && l.tail == NULL && l.len == 0);

	assert(dlist_push_back(&l, 1) == 0);
	assert(dlist_push_back(&l, 2) == 0);
	assert(dlist_push_back(&l, 3) == 0);
	assert(l.len == 3);
	assert(l.head->value == 1 && l.tail->value == 3);

	int v;
	assert(dlist_pop_front(&l, &v) == 0 && v == 1);
	assert(dlist_pop_back(&l, &v) == 0 && v == 3);
	assert(l.len == 1);
	assert(l.head == l.tail && l.head->value == 2);

	assert(dlist_pop_front(&l, &v) == 0 && v == 2);
	assert(l.head == NULL && l.tail == NULL && l.len == 0);

	assert(dlist_pop_front(&l, &v) != 0);   /* 空 */

	dlist_push_front(&l, 10);
	dlist_push_front(&l, 20);
	dlist_push_front(&l, 30);
	assert(l.head->value == 30 && l.tail->value == 10);
	dlist_free(&l);

	printf("✅ Problem #16 doubly_linked_list passed\n");
	return 0;
}
