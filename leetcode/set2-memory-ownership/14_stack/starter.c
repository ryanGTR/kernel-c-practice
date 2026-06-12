// SPDX-License-Identifier: GPL-2.0
/* Problem #14 — stack (LIFO) */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct stack { int *data; size_t len; size_t cap; };

static int  stack_init(struct stack *s, size_t cap)        { (void)s; (void)cap; return -1; }
static int  stack_push(struct stack *s, int v)              { (void)s; (void)v; return -1; }
static int  stack_pop(struct stack *s, int *out)            { (void)s; (void)out; return -1; }
static int  stack_peek(const struct stack *s, int *out)     { (void)s; (void)out; return -1; }
static void stack_free(struct stack *s)                     { (void)s; }

int main(void)
{
	struct stack s;
	assert(stack_init(&s, 4) == 0);

	assert(stack_push(&s, 1) == 0);
	assert(stack_push(&s, 2) == 0);
	assert(stack_push(&s, 3) == 0);

	int v;
	assert(stack_peek(&s, &v) == 0 && v == 3);
	assert(s.len == 3);   /* peek 不改 len */

	assert(stack_pop(&s, &v) == 0 && v == 3);
	assert(stack_pop(&s, &v) == 0 && v == 2);
	assert(stack_pop(&s, &v) == 0 && v == 1);
	assert(stack_pop(&s, &v) != 0);   /* 空 */

	stack_free(&s);
	printf("✅ Problem #14 stack passed\n");
	return 0;
}
