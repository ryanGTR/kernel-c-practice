// SPDX-License-Identifier: GPL-2.0
/* Problem #11 — dynamic_int_array */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct iarr {
	int *data;
	size_t len;
	size_t cap;
};

static int iarr_init(struct iarr *a, size_t cap)
{
	(void)a;
	(void)cap;
	return -1;   /* TODO */
}

static int iarr_push(struct iarr *a, int v)
{
	(void)a;
	(void)v;
	return -1;   /* TODO */
}

static int iarr_pop(struct iarr *a, int *out)
{
	(void)a;
	(void)out;
	return -1;   /* TODO */
}

static void iarr_free(struct iarr *a)
{
	(void)a;   /* TODO */
}

int main(void)
{
	struct iarr a;
	assert(iarr_init(&a, 4) == 0);

	for (int i = 0; i < 100; i++)
		assert(iarr_push(&a, i) == 0);
	assert(a.len == 100);

	int v;
	assert(iarr_pop(&a, &v) == 0);
	assert(v == 99 && a.len == 99);

	/* pop 到底 */
	while (iarr_pop(&a, &v) == 0)
		;
	assert(a.len == 0);
	assert(iarr_pop(&a, &v) != 0);   /* 空 → 失敗 */

	iarr_free(&a);
	iarr_free(&a);   /* 冪等 */

	printf("✅ Problem #11 dynamic_int_array passed\n");
	return 0;
}
