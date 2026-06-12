// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

static unsigned long hash(const char *key, size_t size)
{
	/* TODO: implement hash function */
	(void)key;
	(void)size;
	return 0;
}

struct hash_table *ht_create(size_t size)
{
	/* TODO: implement */
	(void)size;
	return NULL;
}

int ht_put(struct hash_table *ht, const char *key, int value)
{
	/* TODO: implement - return 0 on success, -1 on failure */
	(void)hash(key, ht ? ht->size : 1);
	(void)value;
	return -1;
}

int ht_get(struct hash_table *ht, const char *key, int *out_value)
{
	/* TODO: implement - return 0 if found, -1 if not found */
	(void)ht;
	(void)key;
	(void)out_value;
	return -1;
}

int ht_delete(struct hash_table *ht, const char *key)
{
	/* TODO: implement - return 0 on success, -1 if not found */
	(void)ht;
	(void)key;
	return -1;
}

void ht_destroy(struct hash_table *ht)
{
	/* TODO: implement - free all entries, keys, and the table itself */
	(void)ht;
}

int ht_resize(struct hash_table *ht, size_t new_size)
{
	/* TODO: bonus - implement resize + rehash */
	(void)ht;
	(void)new_size;
	return -1;
}
