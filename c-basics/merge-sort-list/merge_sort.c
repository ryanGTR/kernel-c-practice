// SPDX-License-Identifier: GPL-2.0
#include <stddef.h>

#include "merge_sort.h"

struct node *merge_two(struct node *a, struct node *b)
{
	if (!a)
		return b;

	if (!b)
		return a;

	struct node dummy;
	struct node *tail = &dummy;

	while (a && b) {
		if (a->key <= b->key) {
			tail->next = a;
			a = a->next;
		} else {
			tail->next = b;
			b = b->next;
		}
		tail = tail->next;
	}

	if (a)
		tail->next = a;
	if (b)
		tail->next = b;

	return dummy.next;
}

struct node *split_half(struct node *head)
{
	/* TODO */
	(void)head;
	return NULL;
}

struct node *merge_sort(struct node *head)
{
	/* TODO: use split_half() and merge_two() */
	return head;
}
