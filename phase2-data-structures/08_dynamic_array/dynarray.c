// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdlib.h>
#include "dynarray.h"

struct dynarray *dynarray_create(void)
{
	/* TODO: implement - allocate struct, allocate data with DYNARRAY_INIT_CAP */
	return NULL;
}

int dynarray_push_back(struct dynarray *da, int value)
{
	/* TODO: implement - grow if size == capacity, return 0 on success, -1 on failure */
	(void)da;
	(void)value;
	return -1;
}

int dynarray_get(struct dynarray *da, size_t index)
{
	/* TODO: implement - check bounds, return value at index */
	(void)da;
	(void)index;
	return 0;
}

int dynarray_set(struct dynarray *da, size_t index, int value)
{
	/* TODO: implement - check bounds, return 0 on success, -1 on failure */
	(void)da;
	(void)index;
	(void)value;
	return -1;
}

int dynarray_pop_back(struct dynarray *da)
{
	/* TODO: implement - check empty, return removed value */
	(void)da;
	return 0;
}

size_t dynarray_size(struct dynarray *da)
{
	/* TODO: implement */
	(void)da;
	return 0;
}

void dynarray_destroy(struct dynarray *da)
{
	/* TODO: implement - free data, then free struct */
	(void)da;
}

int dynarray_insert_at(struct dynarray *da, size_t index, int value)
{
	/* TODO: bonus - insert at index, shift elements right */
	(void)da;
	(void)index;
	(void)value;
	return -1;
}

int dynarray_remove_at(struct dynarray *da, size_t index)
{
	/* TODO: bonus - remove at index, shift elements left, return removed value */
	(void)da;
	(void)index;
	return 0;
}
