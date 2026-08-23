/* SPDX-License-Identifier: GPL-2.0 */
#ifndef STACK_H
#define STACK_H

#include <stddef.h>

/*
 * Two stacks, same interface, different storage.
 * Write both, then fill in the comparison table in spec.md.
 */

/* --- array-backed stack (grows like 08_dynamic_array) --- */
struct astack {
	int *data;
	size_t size;
	size_t capacity;
};

struct astack *astack_create(void);
int astack_push(struct astack *s, int value);	/* 0 ok, -1 on alloc fail */
int astack_pop(struct astack *s, int *out);	/* 0 ok, -1 if empty */
int astack_peek(struct astack *s, int *out);	/* 0 ok, -1 if empty */
size_t astack_size(struct astack *s);
void astack_destroy(struct astack *s);

/* --- linked-list-backed stack (push/pop at head) --- */
struct lnode {
	int data;
	struct lnode *next;
};

struct lstack {
	struct lnode *top;
	size_t size;
};

struct lstack *lstack_create(void);
int lstack_push(struct lstack *s, int value);
int lstack_pop(struct lstack *s, int *out);
int lstack_peek(struct lstack *s, int *out);
size_t lstack_size(struct lstack *s);
void lstack_destroy(struct lstack *s);

#endif /* STACK_H */
