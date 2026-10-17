/* SPDX-License-Identifier: GPL-2.0 */
#ifndef PP_H
#define PP_H

struct node {
	int val;
	struct node *next;
};

int push_front(struct node **head, int val);	/* 0 ok, -1 alloc fail */
int pop_front(struct node **head, int *out);	/* 0 ok, -1 if empty */
void list_free(struct node **head);		/* *head is NULL afterwards */

#endif /* PP_H */
