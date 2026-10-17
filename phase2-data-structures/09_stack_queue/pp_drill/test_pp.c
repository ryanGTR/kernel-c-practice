// SPDX-License-Identifier: GPL-2.0
#include <assert.h>
#include <stdio.h>
#include "pp.h"

static void test_push_changes_head(void)
{
	struct node *head = NULL;

	assert(push_front(&head, 5) == 0);
	assert(head != NULL);		/* caller's head really changed */
	assert(head->val == 5);
	assert(head->next == NULL);
	list_free(&head);
	assert(head == NULL);
	printf("push changes head ok\n");
}

static void test_order(void)
{
	struct node *head = NULL;
	int out;

	for (int i = 1; i <= 3; i++)
		assert(push_front(&head, i) == 0);
	/* list is now 3 -> 2 -> 1 */
	assert(pop_front(&head, &out) == 0 && out == 3);
	assert(pop_front(&head, &out) == 0 && out == 2);
	assert(pop_front(&head, &out) == 0 && out == 1);
	assert(head == NULL);		/* popping the last node empties it */
	printf("lifo order ok\n");
}

static void test_pop_empty(void)
{
	struct node *head = NULL;
	int out = 42;

	assert(pop_front(&head, &out) == -1);
	assert(out == 42);		/* untouched on failure */
	assert(head == NULL);
	printf("pop empty ok\n");
}

static void test_free_nonempty(void)
{
	struct node *head = NULL;

	for (int i = 0; i < 100; i++)
		assert(push_front(&head, i) == 0);
	list_free(&head);		/* ASan reports a leak if any node survives */
	assert(head == NULL);
	list_free(&head);		/* freeing an empty list is a no-op */
	printf("free ok\n");
}

int main(void)
{
	test_push_changes_head();
	test_order();
	test_pop_empty();
	test_free_nonempty();
	printf("\nAll pp tests passed!\n");
	return 0;
}
