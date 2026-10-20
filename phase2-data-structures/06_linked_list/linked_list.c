// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

struct node *list_create(void)
{
	return NULL;
}

struct node *list_append(struct node *head, int value)
{
	struct node *new = malloc(sizeof(struct node));
	struct node *cur;

	if (!new)
		return head;

	new->data = value;
	new->next = NULL;

	if (!head)
		return new;

	cur = head;
	while (cur->next != NULL)
		cur = cur->next;

	cur->next = new;

	return head;
}

struct node *list_prepend(struct node *head, int value)
{
	struct node *cur = malloc(sizeof(struct node));

	if (!cur)
		return head;

	cur->data = value;
	cur->next = head;

	return cur;
}

struct node *list_find(struct node *head, int value)
{
	struct node *cur = head;

	while (cur != NULL) {
		if (cur->data == value)
			return cur;

		cur = cur->next;
	}

	return NULL;
}

struct node *list_delete(struct node *head, int value)
{
	if (!head)
		return head;

	struct node *prev = head;
	struct node *cur = head;

	if (prev->data == value) {
		prev = cur->next;
		free(cur);
		return prev;
	}

	while (cur != NULL) {
		if (cur->data == value) {
			prev->next = cur->next;
			free(cur);
			return head;
		}
		prev = cur;
		cur = cur->next;
	}

	return head;
}

void list_print(struct node *head)
{
	if (!head)
		return;

	struct node *cur = head;

	printf("%d", cur->data);

	if (cur->next != NULL)
		printf("->");

	list_print(cur->next);
}

void list_free_all(struct node *head)
{
	struct node *cur = head, *next;

	while (cur != NULL) {
		next = cur->next;
		free(cur);
		cur = next;
	}
}

struct node *list_reverse(struct node *head)
{
	/* TODO: bonus - implement */
	struct node *cur = head, *prev = NULL;

	while (cur) {
		struct node *next = cur->next;

		cur->next = prev;
		prev = cur;
		cur = next;
	}

	return prev;
}
