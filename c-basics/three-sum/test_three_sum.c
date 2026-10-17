#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "three_sum.h"

#define CAP 16

static int fails;

static void check(const char *name, int *nums, int n, int max_out,
		  int want_cnt, const int (*want)[3])
{
	int out[CAP][3];
	int cnt, k, bad = 0;

	memset(out, 0x7f, sizeof(out));
	cnt = three_sum(nums, n, out, max_out);

	if (cnt != want_cnt)
		bad = 1;
	for (k = 0; !bad && k < cnt; k++)
		if (memcmp(out[k], want[k], sizeof(out[k])))
			bad = 1;

	if (bad) {
		printf("FAIL %-12s got cnt=%d", name, cnt);
		for (k = 0; k < cnt && k < CAP; k++)
			printf(" [%d,%d,%d]", out[k][0], out[k][1], out[k][2]);
		printf(", want cnt=%d", want_cnt);
		for (k = 0; k < want_cnt; k++)
			printf(" [%d,%d,%d]", want[k][0], want[k][1], want[k][2]);
		printf("\n");
		fails++;
	} else {
		printf("ok   %s\n", name);
	}
}

int main(void)
{
	int a[] = { -1, 0, 1, 2, -1, -4 };
	const int wa[][3] = { { -1, -1, 2 }, { -1, 0, 1 } };
	int b[] = { 0, 1, 1 };
	int c[] = { 0, 0, 0 };
	const int wc[][3] = { { 0, 0, 0 } };
	int d[] = { 0, 0, 0, 0, 0 };
	int e[] = { -2, 0, 0, 2, 2 };
	const int we[][3] = { { -2, 0, 2 } };
	int f[] = { 3, -2, 1, 0, -1, 2, -3 };
	const int wf[][3] = { { -3, 0, 3 }, { -3, 1, 2 }, { -2, -1, 3 },
			      { -2, 0, 2 }, { -1, 0, 1 } };
	int g[] = { 1, -1 };
	/* INT_MIN + -1 overflows in int: signed overflow is UB in C */
	int h[] = { INT_MAX, 1, -1, INT_MIN };
	const int wh[][3] = { { INT_MIN, 1, INT_MAX } };
	int full[] = { 3, -2, 1, 0, -1, 2, -3 };

	check("basic", a, 6, CAP, 2, wa);
	check("none", b, 3, CAP, 0, NULL);
	check("zeros", c, 3, CAP, 1, wc);
	check("dup_zeros", d, 5, CAP, 1, wc);
	check("dup_pair", e, 5, CAP, 1, we);
	check("many", f, 7, CAP, 5, wf);
	check("too_short", g, 2, CAP, 0, NULL);
	check("empty", NULL, 0, CAP, 0, NULL);
	check("overflow", h, 4, CAP, 1, wh);
	/* 5 answers but room for only 2: must return -1, not overrun out */
	check("out_full", full, 7, 2, -1, NULL);

	printf("%s (%d failed)\n", fails ? "RED" : "GREEN", fails);
	return fails != 0;
}
