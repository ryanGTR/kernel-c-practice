/* SPDX-License-Identifier: GPL-2.0 */
#ifndef MERGE_SORT_H
#define MERGE_SORT_H

/*
 * @key is what you compare. @id is only for the tests: it records the
 * original position so they can tell whether equal keys kept their order.
 */
struct node {
	int key;
	int id;
	struct node *next;
};

/*
 * merge_two() - merge two lists that are each sorted by key, ascending
 * @a: first sorted list (may be NULL)
 * @b: second sorted list (may be NULL)
 *
 * Must be stable: when keys are equal, nodes from @a come before nodes
 * from @b. Reuse the existing nodes; do not malloc or free.
 *
 * Return: head of the merged list.
 */
struct node *merge_two(struct node *a, struct node *b);

/*
 * split_half() - cut a list into two halves
 * @head: list with at least 2 nodes
 *
 * The first half keeps ceil(len / 2) nodes and is ended with NULL.
 * Example: 1->2->3->4->5 becomes 1->2->3 and 4->5.
 *
 * Return: head of the second half.
 */
struct node *split_half(struct node *head);

/*
 * merge_sort() - stable sort by key, ascending, in O(n log n)
 * @head: list to sort (may be NULL)
 *
 * Return: head of the sorted list.
 */
struct node *merge_sort(struct node *head);

#endif /* MERGE_SORT_H */
