/* SPDX-License-Identifier: GPL-2.0 */
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

struct ht_entry {
	char *key;
	int value;
	struct ht_entry *next;
};

struct hash_table {
	struct ht_entry **buckets;
	size_t size;
	size_t count;
};

struct hash_table *ht_create(size_t size);
int ht_put(struct hash_table *ht, const char *key, int value);
int ht_get(struct hash_table *ht, const char *key, int *out_value);
int ht_delete(struct hash_table *ht, const char *key);
void ht_destroy(struct hash_table *ht);

/* Bonus */
int ht_resize(struct hash_table *ht, size_t new_size);

#endif /* HASH_TABLE_H */
