// SPDX-License-Identifier: GPL-2.0
/* Problem #11 — dynamic_int_array 參考解 */

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
	if (cap == 0)
		cap = 8;
	a->data = malloc(cap * sizeof(*a->data));
	if (!a->data)
		return -1;
	a->len = 0;
	a->cap = cap;
	return 0;
}

static int iarr_push(struct iarr *a, int v)
{
	if (a->len == a->cap) {
		size_t new_cap = a->cap == 0 ? 8 : a->cap * 2;
		int *tmp = realloc(a->data, new_cap * sizeof(*a->data));
		if (!tmp)
			return -1;
		a->data = tmp;
		a->cap = new_cap;
	}
	a->data[a->len++] = v;
	return 0;
}

static int iarr_pop(struct iarr *a, int *out)
{
	if (a->len == 0)
		return -1;
	*out = a->data[--a->len];
	return 0;
}

static void iarr_free(struct iarr *a)
{
	free(a->data);
	a->data = NULL;
	a->len = a->cap = 0;
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

	while (iarr_pop(&a, &v) == 0)
		;
	assert(a.len == 0);
	assert(iarr_pop(&a, &v) != 0);

	iarr_free(&a);
	iarr_free(&a);

	printf("✅ Problem #11 dynamic_int_array passed\n");
	return 0;
}
