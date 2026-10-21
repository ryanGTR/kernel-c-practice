// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdlib.h>

#include "remove_nth.h"

static int passed, total;

static struct node *build(const int *a, int len)
{
	struct node *head = NULL, **tail = &head;
	int i;

	for (i = 0; i < len; i++) {
		struct node *nd = malloc(sizeof(*nd));

		if (!nd)
			exit(1);
		nd->val = a[i];
		nd->next = NULL;
		*tail = nd;
		tail = &nd->next;
	}
	return head;
}

static void free_all(struct node *head)
{
	while (head) {
		struct node *next = head->next;

		free(head);
		head = next;
	}
}

static void print(const char *tag, struct node *head)
{
	int shown = 0;

	printf("%s[", tag);
	for (; head && shown < 12; head = head->next, shown++)
		printf("%d%s", head->val, head->next ? "," : "");
	printf("%s]", head ? "..." : "");
}

static void check(const char *name, const int *in, int len, int n,
		  const int *want, int want_len)
{
	struct node *head = build(in, len);
	struct node *got = remove_nth_from_end(head, n);
	struct node *p = got;
	int i, ok = 1;

	for (i = 0; i < want_len; i++, p = p->next) {
		if (!p || p->val != want[i]) {
			ok = 0;
			break;
		}
	}
	if (ok && p)
		ok = 0;

	total++;
	if (ok) {
		passed++;
		printf("  ok    %s\n", name);
	} else {
		printf("  FAIL  %s  n=%d  ", name, n);
		print("got=", got);
		printf("\n");
	}
	free_all(got);
}

int main(void)
{
	int a5[] = { 1, 2, 3, 4, 5 };
	int a2[] = { 1, 2 };
	int a1[] = { 7 };
	int big[1000];
	int big_want[999];
	int i;

	puts("remove_nth_from_end:");

	check("lc_example", a5, 5, 2, (int[]){ 1, 2, 3, 5 }, 4);
	check("remove_last", a5, 5, 1, (int[]){ 1, 2, 3, 4 }, 4);
	check("remove_head", a5, 5, 5, (int[]){ 2, 3, 4, 5 }, 4);
	check("remove_middle", a5, 5, 3, (int[]){ 1, 2, 4, 5 }, 4);
	check("single_to_empty", a1, 1, 1, NULL, 0);
	check("two_remove_last", a2, 2, 1, (int[]){ 1 }, 1);
	check("two_remove_head", a2, 2, 2, (int[]){ 2 }, 1);

	for (i = 0; i < 1000; i++)
		big[i] = i;
	for (i = 0; i < 999; i++)
		big_want[i] = i < 500 ? i : i + 1;
	check("big_n500", big, 1000, 500, big_want, 999);

	printf("%d/%d %s\n", passed, total, passed == total ? "GREEN" : "RED");
	return passed == total ? 0 : 1;
}
