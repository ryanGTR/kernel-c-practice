// SPDX-License-Identifier: GPL-2.0
/* Problem #14 — stack 參考解 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct stack { int *data; size_t len; size_t cap; };

static int stack_init(struct stack *s, size_t cap)
{
	if (cap == 0) cap = 8;
	s->data = malloc(cap * sizeof(*s->data));
	if (!s->data) return -1;
	s->len = 0; s->cap = cap;
	return 0;
}

static int stack_push(struct stack *s, int v)
{
	if (s->len == s->cap) {
		size_t nc = s->cap * 2;
		int *tmp = realloc(s->data, nc * sizeof(*s->data));
		if (!tmp) return -1;
		s->data = tmp; s->cap = nc;
	}
	s->data[s->len++] = v;
	return 0;
}

static int stack_pop(struct stack *s, int *out)
{
	if (s->len == 0) return -1;
	*out = s->data[--s->len];
	return 0;
}

static int stack_peek(const struct stack *s, int *out)
{
	if (s->len == 0) return -1;
	*out = s->data[s->len - 1];
	return 0;
}

static void stack_free(struct stack *s)
{
	free(s->data); s->data = NULL;
	s->len = s->cap = 0;
}

int main(void)
{
	struct stack s;
	assert(stack_init(&s, 4) == 0);

	assert(stack_push(&s, 1) == 0);
	assert(stack_push(&s, 2) == 0);
	assert(stack_push(&s, 3) == 0);

	int v;
	assert(stack_peek(&s, &v) == 0 && v == 3);
	assert(s.len == 3);

	assert(stack_pop(&s, &v) == 0 && v == 3);
	assert(stack_pop(&s, &v) == 0 && v == 2);
	assert(stack_pop(&s, &v) == 0 && v == 1);
	assert(stack_pop(&s, &v) != 0);

	stack_free(&s);
	printf("✅ Problem #14 stack passed\n");
	return 0;
}
