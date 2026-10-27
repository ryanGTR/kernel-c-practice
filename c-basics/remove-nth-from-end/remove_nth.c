// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>

#include "remove_nth.h"

struct node *remove_nth_from_end(struct node *head, int n)
{
	if (!head)
		return NULL;

	struct node *prev = head, *cur = head;

	for (int i = 0; i < n; ++i)
		cur = cur->next;

	if (!cur) {
		struct node *new_head = prev->next;
		free(prev);
		return new_head;
	}

	while (cur->next) {
		cur = cur->next;
		prev = prev->next;
	}
	struct node *target = prev->next;
	prev->next = target->next;
	free(target);

	return head;
}
