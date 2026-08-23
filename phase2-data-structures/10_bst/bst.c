// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>
#include "bst.h"

struct bst_node *bst_insert(struct bst_node *root, int key)
{
	/*
	 * TODO: recursive is fine. Duplicate keys: ignore (return root).
	 * On malloc failure return root unchanged.
	 */
	(void)key;
	return root;
}

struct bst_node *bst_find(struct bst_node *root, int key)
{
	/* TODO: iterative, no recursion needed */
	(void)root;
	(void)key;
	return NULL;
}

struct bst_node *bst_delete(struct bst_node *root, int key)
{
	/*
	 * TODO: three cases
	 *   1. leaf           -> free, return NULL
	 *   2. one child      -> free, return that child
	 *   3. two children   -> copy in-order successor (min of right subtree)
	 *                        into this node, then delete the successor
	 *                        from the right subtree (which is case 1 or 2)
	 * Key not found -> return root unchanged.
	 */
	(void)key;
	return root;
}

struct bst_node *bst_min(struct bst_node *root)
{
	/* TODO: walk left */
	(void)root;
	return NULL;
}

struct bst_node *bst_max(struct bst_node *root)
{
	/* TODO: walk right */
	(void)root;
	return NULL;
}

int bst_height(struct bst_node *root)
{
	/* TODO */
	(void)root;
	return 0;
}

size_t bst_count(struct bst_node *root)
{
	/* TODO */
	(void)root;
	return 0;
}

void bst_free(struct bst_node *root)
{
	/* TODO: which traversal order lets you free children before parent? */
	(void)root;
}

void bst_inorder(struct bst_node *root, bst_visit_fn visit, void *ctx)
{
	/* TODO: left, self, right */
	(void)root;
	(void)visit;
	(void)ctx;
}

void bst_preorder(struct bst_node *root, bst_visit_fn visit, void *ctx)
{
	/* TODO: self, left, right */
	(void)root;
	(void)visit;
	(void)ctx;
}

void bst_postorder(struct bst_node *root, bst_visit_fn visit, void *ctx)
{
	/* TODO: left, right, self */
	(void)root;
	(void)visit;
	(void)ctx;
}

int bst_is_valid(struct bst_node *root)
{
	/*
	 * TODO (bonus): NOT "left->key < key && right->key > key" -- that is
	 * the classic wrong answer. Pass (lo, hi) bounds down the recursion.
	 */
	(void)root;
	return 0;
}

void bst_inorder_iter(struct bst_node *root, bst_visit_fn visit, void *ctx)
{
	/* TODO (bonus): explicit stack (reuse 09_stack_queue idea) */
	(void)root;
	(void)visit;
	(void)ctx;
}
