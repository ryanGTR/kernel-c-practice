// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdlib.h>

#include "merge_sort.h"

static int passed, total;

/* ids are the array positions, so equal keys can be told apart */
static struct node *build(const int *keys, int len, int first_id)
{
	struct node *head = NULL, **tail = &head;
	int i;

	for (i = 0; i < len; i++) {
		struct node *nd = malloc(sizeof(*nd));

		if (!nd)
			exit(1);
		nd->key = keys[i];
		nd->id = first_id + i;
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
		printf("%d#%d%s", head->key, head->id, head->next ? "," : "");
	printf("%s]", head ? "..." : "");
}

/* compare against want_keys/want_ids; want_ids may be NULL */
static int same(struct node *p, const int *want_keys, const int *want_ids,
		int want_len)
{
	int i;

	for (i = 0; i < want_len; i++, p = p->next) {
		if (!p || p->key != want_keys[i])
			return 0;
		if (want_ids && p->id != want_ids[i])
			return 0;
	}
	return !p;
}

static void report(const char *name, int ok, struct node *got)
{
	total++;
	if (ok) {
		passed++;
		printf("  ok    %s\n", name);
	} else {
		printf("  FAIL  %s  ", name);
		print("got=", got);
		printf("\n");
	}
}

static void check_merge(const char *name, const int *a, int alen,
			const int *b, int blen, const int *want_keys,
			const int *want_ids, int want_len)
{
	/* ids: a gets 0.., b gets 100.. */
	struct node *got = merge_two(build(a, alen, 0), build(b, blen, 100));

	report(name, same(got, want_keys, want_ids, want_len), got);
	free_all(got);
}

static void check_split(const char *name, const int *in, int len,
			const int *want_first, int first_len,
			const int *want_second, int second_len)
{
	struct node *head = build(in, len, 0);
	struct node *second = split_half(head);
	int ok = same(head, want_first, NULL, first_len) &&
		 same(second, want_second, NULL, second_len);

	total++;
	if (ok) {
		passed++;
		printf("  ok    %s\n", name);
	} else {
		printf("  FAIL  %s  ", name);
		print("first=", head);
		print(" second=", second);
		printf("\n");
	}
	free_all(head);
	free_all(second);
}

static void check_sort(const char *name, const int *in, int len,
		       const int *want_keys, const int *want_ids)
{
	struct node *got = merge_sort(build(in, len, 0));

	report(name, same(got, want_keys, want_ids, len), got);
	free_all(got);
}

static int cmp_int(const void *x, const void *y)
{
	return *(const int *)x - *(const int *)y;
}

int main(void)
{
	static int big[100000], big_want[100000];
	int i;

	puts("merge_two:");
	check_merge("both_empty", NULL, 0, NULL, 0, NULL, NULL, 0);
	check_merge("a_empty", NULL, 0, (int[]){ 1, 2 }, 2,
		    (int[]){ 1, 2 }, NULL, 2);
	check_merge("b_empty", (int[]){ 1, 2 }, 2, NULL, 0,
		    (int[]){ 1, 2 }, NULL, 2);
	check_merge("interleave", (int[]){ 1, 3, 5 }, 3, (int[]){ 2, 4, 6 }, 3,
		    (int[]){ 1, 2, 3, 4, 5, 6 }, NULL, 6);
	check_merge("a_all_smaller", (int[]){ 1, 2 }, 2, (int[]){ 8, 9 }, 2,
		    (int[]){ 1, 2, 8, 9 }, NULL, 4);
	check_merge("stable_equal_keys", (int[]){ 1, 5 }, 2,
		    (int[]){ 1, 5 }, 2, (int[]){ 1, 1, 5, 5 },
		    (int[]){ 0, 100, 1, 101 }, 4);

	puts("split_half:");
	check_split("two", (int[]){ 1, 2 }, 2, (int[]){ 1 }, 1,
		    (int[]){ 2 }, 1);
	check_split("three", (int[]){ 1, 2, 3 }, 3, (int[]){ 1, 2 }, 2,
		    (int[]){ 3 }, 1);
	check_split("four", (int[]){ 1, 2, 3, 4 }, 4, (int[]){ 1, 2 }, 2,
		    (int[]){ 3, 4 }, 2);
	check_split("five", (int[]){ 1, 2, 3, 4, 5 }, 5,
		    (int[]){ 1, 2, 3 }, 3, (int[]){ 4, 5 }, 2);

	puts("merge_sort:");
	check_sort("empty", NULL, 0, NULL, NULL);
	check_sort("single", (int[]){ 7 }, 1, (int[]){ 7 }, NULL);
	check_sort("two_swapped", (int[]){ 2, 1 }, 2, (int[]){ 1, 2 }, NULL);
	check_sort("reversed", (int[]){ 5, 4, 3, 2, 1 }, 5,
		   (int[]){ 1, 2, 3, 4, 5 }, NULL);
	check_sort("stable", (int[]){ 2, 1, 2, 1, 2 }, 5,
		   (int[]){ 1, 1, 2, 2, 2 }, (int[]){ 1, 3, 0, 2, 4 });

	/* O(n^2) on 100000 nodes takes far longer than O(n log n) */
	srand(42);
	for (i = 0; i < 100000; i++)
		big[i] = big_want[i] = rand() % 1000;
	qsort(big_want, 100000, sizeof(int), cmp_int);
	check_sort("big_100000", big, 100000, big_want, NULL);

	printf("%d/%d %s\n", passed, total, passed == total ? "GREEN" : "RED");
	/* flush before LeakSanitizer reports at exit */
	fflush(stdout);
	return passed == total ? 0 : 1;
}
