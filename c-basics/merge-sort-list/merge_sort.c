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
	struct node *slow = head;
	struct node *fast = head;

	while (fast->next && fast->next->next) {
		slow = slow->next;
		fast = fast->next->next;
	}

	struct node *second = slow->next;
	slow->next = NULL;

	return second;
}

struct node *merge_sort(struct node *head)
{
	if (!head || !head->next)
		return head;

	struct node *second = split_half(head);
	struct node *left = merge_sort(head);
	struct node *right = merge_sort(second);

	return merge_two(left, right);
}
