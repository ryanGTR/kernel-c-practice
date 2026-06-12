/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct node {
	int data;
	struct node *next;
};

struct node *list_create(void);
struct node *list_append(struct node *head, int value);
struct node *list_prepend(struct node *head, int value);
struct node *list_find(struct node *head, int value);
struct node *list_delete(struct node *head, int value);
void list_print(struct node *head);
void list_free_all(struct node *head);

/* Bonus */
struct node *list_reverse(struct node *head);

#endif /* LINKED_LIST_H */
