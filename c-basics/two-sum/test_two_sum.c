#include <limits.h>
#include <stdio.h>
#include "two_sum.h"

static int fails;

static void check(const char *name, const int *nums, int n, int target,
		  int want_ok, int want_i, int want_j)
{
	int i = -1, j = -1;
	int ok = two_sum(nums, n, target, &i, &j);

	if (ok != want_ok || (ok && (i != want_i || j != want_j))) {
		printf("FAIL %-12s got ok=%d (%d,%d), want ok=%d (%d,%d)\n",
		       name, ok, i, j, want_ok, want_i, want_j);
		fails++;
	} else {
		printf("ok   %s\n", name);
	}
}

int main(void)
{
	int a[] = { 2, 7, 11, 15 };
	int b[] = { -3, -1, 0, 4, 9 };
	int c[] = { 3, 3 };
	int d[] = { 1, 2, 3, 4, 5 };
	int e[] = { 5 };
	int f[] = { 1, INT_MAX - 1, INT_MAX };

	check("basic", a, 4, 9, 1, 0, 1);
	check("ends", a, 4, 17, 1, 0, 3);
	check("negative", b, 5, -4, 1, 0, 1);
	check("dup", c, 2, 6, 1, 0, 1);
	check("not_found", d, 5, 100, 0, -1, -1);
	check("single", e, 1, 10, 0, -1, -1);
	check("empty", NULL, 0, 0, 0, -1, -1);
	/* 1 + INT_MAX overflows: signed overflow is UB in C */
	check("overflow", f, 3, INT_MAX, 1, 0, 1);

	printf("%s (%d failed)\n", fails ? "RED" : "GREEN", fails);
	return fails != 0;
}
