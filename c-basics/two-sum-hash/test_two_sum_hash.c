#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "two_sum_hash.h"

static int fails;

static void check(const char *name, const int *nums, int n, int target,
		  int want_ok, int want_i, int want_j)
{
	int i = -1, j = -1;
	int ok = two_sum_hash(nums, n, target, &i, &j);

	if (ok != want_ok || i != want_i || j != want_j) {
		printf("FAIL %-12s got ok=%d (%d,%d), want ok=%d (%d,%d)\n",
		       name, ok, i, j, want_ok, want_i, want_j);
		fails++;
	} else {
		printf("ok   %s\n", name);
	}
}

static void check_big(void)
{
	int n = 200000, k;
	int *a = malloc(sizeof(*a) * n);

	if (!a) {
		printf("FAIL big         malloc\n");
		fails++;
		return;
	}
	/* multiples of 1024: bad for a naive "key % capacity" hash */
	for (k = 0; k < n; k++)
		a[k] = k * 1024;
	a[n - 1] = -7;
	a[n - 2] = 7;
	check("big", a, n, 0, 1, n - 2, n - 1);
	free(a);
}

int main(void)
{
	int a[] = { 2, 7, 11, 15 };
	int b[] = { 3, 2, 4 };
	int c[] = { 3, 3 };
	int d[] = { -3, 4, 3, 90 };
	int e[] = { 1, 2, 3 };
	int f[] = { 5 };
	int g[] = { 1, 1, 1, 5 };
	int h[] = { INT_MIN, 3, -2 };
	int m[] = { 0, 4, 3, 0 };

	check("basic", a, 4, 9, 1, 0, 1);
	check("unsorted", b, 3, 6, 1, 1, 2);
	check("dup", c, 2, 6, 1, 0, 1);
	check("negative", d, 4, 0, 1, 0, 2);
	check("zeros", m, 4, 0, 1, 0, 3);
	check("not_found", e, 3, 100, 0, -1, -1);
	check("no_self", f, 1, 10, 0, -1, -1);
	check("self_twice", e, 3, 4, 1, 0, 2);
	check("empty", NULL, 0, 0, 0, -1, -1);
	/* j = 3 pairs with i = 0, 1 or 2: keep the smallest */
	check("first_i", g, 4, 6, 1, 0, 3);
	/* target - INT_MIN overflows: signed overflow is UB in C */
	check("overflow", h, 3, 1, 1, 1, 2);
	check_big();

	printf("%s (%d failed)\n", fails ? "RED" : "GREEN", fails);
	return fails != 0;
}
