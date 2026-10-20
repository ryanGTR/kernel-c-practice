// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <assert.h>
#include "linked_list.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* Build a list holding vals[0..n-1] in order. */
static struct node *build(const int *vals, int n)
{
	struct node *head = NULL;

	for (int i = 0; i < n; i++)
		head = list_append(head, vals[i]);
	return head;
}

/* The list must hold exactly want[0..n-1], then end. */
static void check(struct node *head, const int *want, int n)
{
	for (int i = 0; i < n; i++) {
		assert(head != NULL);
		assert(head->data == want[i]);
		head = head->next;
	}
	assert(head == NULL);
}

static void test_reverse(void)
{
	/* empty list stays empty */
	assert(list_reverse(NULL) == NULL);

	/* one node: same node comes back, still terminated */
	struct node *one = list_append(NULL, 42);

	assert(list_reverse(one) == one);
	assert(one->next == NULL);
	list_free_all(one);

	/* two nodes: the smallest case where links actually flip */
	const int two[] = { 1, 2 };
	const int two_rev[] = { 2, 1 };
	struct node *l2 = build(two, ARRAY_SIZE(two));

	l2 = list_reverse(l2);
	check(l2, two_rev, ARRAY_SIZE(two_rev));
	list_free_all(l2);

	/* many nodes, in place: old head becomes the tail, no new nodes */
	const int five[] = { 10, 20, 30, 40, 50 };
	const int five_rev[] = { 50, 40, 30, 20, 10 };
	struct node *l5 = build(five, ARRAY_SIZE(five));
	struct node *old_head = l5;
	struct node *old_tail = l5->next->next->next->next;

	l5 = list_reverse(l5);
	check(l5, five_rev, ARRAY_SIZE(five_rev));
	assert(l5 == old_tail);
	assert(old_head->next == NULL);

	/* reversing twice gives back the original */
	l5 = list_reverse(l5);
	check(l5, five, ARRAY_SIZE(five));
	assert(l5 == old_head);
	list_free_all(l5);

	printf("list_reverse: all cases passed\n");
}

int main(void)
{
	struct node *list = NULL;

	printf("=== Linked List Tests ===\n\n");

	/* TODO: Test list_create - should return NULL (empty list) */
	list = list_create();
	assert(list == NULL);
	/* TODO: Test list_append - add 10, 20, 30; print should show 10 -> 20 -> 30 */
	list = list_append(list, 10);
	assert(list != NULL);
	assert(list->data == 10);
	list = list_append(list, 20);
	assert(list->next->data == 20);
	list = list_append(list, 30);
	assert(list->next->next->data == 30);

	/* TODO: Test list_prepend - prepend 5; print should show 5 -> 10 -> 20 -> 30 */
	list = list_prepend(list, 5);
	assert(list != NULL);
	assert(list->data == 5);
	assert(list->next->data == 10);
	assert(list->next->next->data == 20);
	assert(list->next->next->next->data == 30);

	/* TODO: Test list_find - find 20 (should succeed), find 99 (should return NULL) */
	struct node *find_20 = list_find(list, 20);

	assert(find_20 != NULL);

	struct node *find_99 = list_find(list, 99);

	assert(find_99 == NULL);

	struct node *find_NULL = list_find(NULL, 20);

	assert(find_NULL == NULL);

	/* TODO: Test list_delete - delete middle node (20), verify list */
	list = list_delete(list, 20);
	assert(list != NULL);
	assert(list->data == 5);
	assert(list->next->data == 10);
	assert(list->next->next->data == 30);
	assert(list->next->next->next == NULL);

	/* TODO: Test list_delete - delete head node (5), verify list */
	list = list_delete(list, 5);
	assert(list != NULL);
	assert(list->data == 10);
	assert(list->next->data == 30);
	assert(list->next->next == NULL);

	/* TODO: Test list_delete - delete tail node (30), verify list */
	list = list_delete(list, 30);
	assert(list != NULL);
	assert(list->data == 10);
	assert(list->next == NULL);

	/* TODO: Test list_delete - delete from single-element list */
	list = list_delete(list, 10);
	assert(list == NULL);

	/* TODO: Test list_delete - delete nonexistent value, verify no change */
	list = list_append(list, 1);
	list = list_append(list, 2);
	list = list_delete(list, 97);
	assert(list->data == 1);
	assert(list->next->data == 2);
	assert(list->next->next == NULL);
	list_print(list);

	/* TODO: Test list_free_all - free everything, run under valgrind */
	list_free_all(NULL);

	struct node *one = list_append(NULL, 1);

	list_free_all(one);

	struct node *many = list_append(NULL, 1);

	many = list_append(many, 1);
	many = list_append(many, 2);
	many = list_append(many, 3);
	list_free_all(many);

	/* Bonus - test list_reverse */
	test_reverse();

	list_free_all(list);

	printf("\nAll tests passed!\n");
	return 0;
}
