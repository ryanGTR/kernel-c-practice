// SPDX-License-Identifier: GPL-2.0
/* Problem #25 — list_for_each_safe */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct list_node {
	struct list_node *next;
	int value;
};

/* TODO: 定義 macro */
#define list_for_each_safe(cur, tmp, head) \
	for ((cur) = NULL; 0; )   /* TODO 替換 */

static struct list_node *make_node(int v)
{
	struct list_node *n = malloc(sizeof(*n));
	n->value = v;
	n->next = NULL;
	return n;
}

static struct list_node *build_list(int n)
{
	struct list_node *head = NULL, *tail = NULL;
	for (int i = 1; i <= n; i++) {
		struct list_node *node = make_node(i);
		if (!head) head = tail = node;
		else { tail->next = node; tail = node; }
	}
	return head;
}

int main(void)
{
	struct list_node *head = build_list(5);

	int sum = 0;
	struct list_node *cur, *tmp;
	list_for_each_safe(cur, tmp, head) {
		sum += cur->value;
		free(cur);
	}
	assert(sum == 15);

	/* 空 list 也要安全 */
	head = NULL;
	list_for_each_safe(cur, tmp, head) {
		assert(0 && "should not iterate empty list");
	}

	printf("✅ Problem #25 list_for_each_safe passed\n");
	return 0;
}
