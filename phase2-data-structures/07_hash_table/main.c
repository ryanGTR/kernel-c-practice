// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <assert.h>
#include "hash_table.h"

int main(void)
{
	struct hash_table *ht = NULL;
	int val = 0;

	printf("=== Hash Table Tests ===\n\n");

	/* TODO: Test ht_create - create table with size 8 */

	/* TODO: Test ht_put - insert ("apple", 1), ("banana", 2), ("cherry", 3) */

	/* TODO: Test ht_get - retrieve each key, verify values */

	/* TODO: Test ht_get - look up nonexistent key, should return -1 */

	/* TODO: Test ht_put - update existing key ("apple", 99), verify new value */

	/* TODO: Test collision handling - insert keys that hash to same bucket */
	/*       Hint: with a small table size, collisions are likely */

	/* TODO: Test ht_delete - delete "banana", verify it's gone */

	/* TODO: Test ht_delete - delete nonexistent key, should return -1 */

	/* TODO: Test ht_destroy - free everything, run under valgrind */

	/* TODO: Bonus - test resize when load factor exceeds 0.75 */

	ht_destroy(ht);
	(void)val;

	printf("\nAll tests passed!\n");
	return 0;
}
