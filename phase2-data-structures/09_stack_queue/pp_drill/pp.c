// SPDX-License-Identifier: GPL-2.0
/*
 * Q: push_front 的第一個參數是什麼型別？為什麼不是 struct node * ？
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
