/* SPDX-License-Identifier: GPL-2.0 */
#ifndef BST_H
#define BST_H

#include <stddef.h>

struct bst_node {
	int key;
	struct bst_node *left;
	struct bst_node *right;
};

/*
 * The tree is just a root pointer; an empty tree is NULL.
 * insert/delete return the (possibly new) root -- same convention as
 * 06_linked_list, because the root can change.
 */
struct bst_node *bst_insert(struct bst_node *root, int key);
struct bst_node *bst_find(struct bst_node *root, int key);
struct bst_node *bst_delete(struct bst_node *root, int key);
struct bst_node *bst_min(struct bst_node *root);
struct bst_node *bst_max(struct bst_node *root);
int bst_height(struct bst_node *root);	/* empty = 0, single node = 1 */
size_t bst_count(struct bst_node *root);
void bst_free(struct bst_node *root);

/* Traversals call visit(key, ctx) per node. ctx lets the caller collect. */
typedef void (*bst_visit_fn)(int key, void *ctx);
void bst_inorder(struct bst_node *root, bst_visit_fn visit, void *ctx);
void bst_preorder(struct bst_node *root, bst_visit_fn visit, void *ctx);
void bst_postorder(struct bst_node *root, bst_visit_fn visit, void *ctx);

/* Bonus */
int bst_is_valid(struct bst_node *root);	/* 1 if BST property holds */
void bst_inorder_iter(struct bst_node *root, bst_visit_fn visit, void *ctx);

#endif /* BST_H */
