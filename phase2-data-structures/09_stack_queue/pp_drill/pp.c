// SPDX-License-Identifier: GPL-2.0
/*
 * Q: push_front 裡為什麼要寫 *head = n，而不是 head = n ？
 * A:
 */
#include <stdlib.h>
#include "pp.h"

int push_front(struct node **head, int val)
{
	/* TODO: malloc a node, link it in front, make the caller's head point to it */
	(void)head;
	(void)val;
	return -1;
}

int pop_front(struct node **head, int *out)
{
	/* TODO: empty -> -1; else take the first node out, write its val, free it */
	(void)head;
	(void)out;
	return -1;
}

void list_free(struct node **head)
{
	/* TODO: free every node; caller's head must end up NULL */
	(void)head;
}
