// SPDX-License-Identifier: GPL-2.0
/* RED against the stubs on purpose -- see ../../TODO.md. */
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include "bst.h"

struct collector {
	int buf[64];
	size_t n;
};

static void collect(int key, void *ctx)
{
	struct collector *c = ctx;

	c->buf[c->n++] = key;
}

static int same(const int *a, const int *b, size_t n)
{
	for (size_t i = 0; i < n; i++)
		if (a[i] != b[i])
			return 0;
	return 1;
}

/*
 *         50
 *       /    \
 *     30      70
 *    /  \    /  \
 *  20   40  60   80
 */
static const int keys[] = { 50, 30, 70, 20, 40, 60, 80 };
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define NKEYS ARRAY_SIZE(keys)

static struct bst_node *build(void)
{
	struct bst_node *root = NULL;

	for (size_t i = 0; i < NKEYS; i++)
		root = bst_insert(root, keys[i]);
	return root;
}

static void test_insert_find(void)
{
	struct bst_node *root = build();

	assert(root && root->key == 50);
	assert(bst_count(root) == NKEYS);
	assert(bst_height(root) == 3);
	assert(bst_find(root, 60) && bst_find(root, 60)->key == 60);
	assert(bst_find(root, 65) == NULL);
	assert(bst_min(root)->key == 20);
	assert(bst_max(root)->key == 80);

	root = bst_insert(root, 40);		/* duplicate: no change */
	assert(bst_count(root) == NKEYS);

	bst_free(root);
	printf("insert/find ok\n");
}

static void test_traversals(void)
{
	struct bst_node *root = build();
	struct collector c;
	static const int in[] = { 20, 30, 40, 50, 60, 70, 80 };
	static const int pre[] = { 50, 30, 20, 40, 70, 60, 80 };
	static const int post[] = { 20, 40, 30, 60, 80, 70, 50 };

	c.n = 0;
	bst_inorder(root, collect, &c);
	assert(c.n == NKEYS && same(c.buf, in, NKEYS));	/* sorted! */

	c.n = 0;
	bst_preorder(root, collect, &c);
	assert(c.n == NKEYS && same(c.buf, pre, NKEYS));

	c.n = 0;
	bst_postorder(root, collect, &c);
	assert(c.n == NKEYS && same(c.buf, post, NKEYS));

	bst_free(root);
	printf("traversals ok\n");
}

static void test_delete(void)
{
	struct bst_node *root = build();
	struct collector c;
	static const int after_leaf[] = { 30, 40, 50, 60, 70, 80 };
	static const int after_one[] = { 40, 50, 60, 70, 80 };
	static const int after_two[] = { 40, 60, 70, 80 };

	/* case 1: leaf */
	root = bst_delete(root, 20);
	c.n = 0;
	bst_inorder(root, collect, &c);
	assert(c.n == 6 && same(c.buf, after_leaf, 6));

	/* case 2: one child (30 now has only right child 40) */
	root = bst_delete(root, 30);
	c.n = 0;
	bst_inorder(root, collect, &c);
	assert(c.n == 5 && same(c.buf, after_one, 5));

	/* case 3: two children, and it is the root */
	root = bst_delete(root, 50);
	assert(root->key == 60);		/* in-order successor moved up */
	c.n = 0;
	bst_inorder(root, collect, &c);
	assert(c.n == 4 && same(c.buf, after_two, 4));

	/* not found: unchanged */
	root = bst_delete(root, 999);
	assert(bst_count(root) == 4);

	/* delete everything: root must end as NULL, LSan checks for leaks */
	root = bst_delete(root, 60);
	root = bst_delete(root, 80);
	root = bst_delete(root, 40);
	root = bst_delete(root, 70);
	assert(root == NULL);
	assert(bst_delete(NULL, 1) == NULL);	/* empty tree is safe */

	bst_free(NULL);
	printf("delete ok\n");
}

int main(void)
{
	printf("=== BST Tests ===\n");
	test_insert_find();
	test_traversals();
	test_delete();

	/* TODO: degenerate tree (insert 1..100 in order): what is bst_height? */
	/*
	 * TODO (bonus): bst_is_valid on a hand-built tree that only looks
	 * valid locally (e.g. 50 -> left 30 -> right 60).
	 */

	printf("\nAll tests passed!\n");
	return 0;
}
