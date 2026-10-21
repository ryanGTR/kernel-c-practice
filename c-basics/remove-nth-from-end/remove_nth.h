/* SPDX-License-Identifier: GPL-2.0 */
#ifndef REMOVE_NTH_H
#define REMOVE_NTH_H

struct node {
	int val;
	struct node *next;
};

/*
 * remove_nth_from_end() - remove the n-th node counted from the end
 * @head: first node of a singly linked list (may be NULL)
 * @n:    1 means the last node; guaranteed 1 <= n <= length
 *
 * The removed node must be freed with free().
 *
 * Return: the head of the list after the removal (NULL if it became empty).
 */
struct node *remove_nth_from_end(struct node *head, int n);

#endif /* REMOVE_NTH_H */
