/* SPDX-License-Identifier: GPL-2.0 */
#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stddef.h>

#define DYNARRAY_INIT_CAP 4

struct dynarray {
	int *data;
	size_t size;
	size_t capacity;
};

struct dynarray *dynarray_create(void);
int dynarray_push_back(struct dynarray *da, int value);
int dynarray_get(struct dynarray *da, size_t index);
int dynarray_set(struct dynarray *da, size_t index, int value);
int dynarray_pop_back(struct dynarray *da);
size_t dynarray_size(struct dynarray *da);
void dynarray_destroy(struct dynarray *da);

/* Bonus */
int dynarray_insert_at(struct dynarray *da, size_t index, int value);
int dynarray_remove_at(struct dynarray *da, size_t index);

#endif /* DYNARRAY_H */
