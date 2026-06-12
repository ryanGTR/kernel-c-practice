// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <assert.h>
#include "dynarray.h"

int main(void)
{
	struct dynarray *da = NULL;

	printf("=== Dynamic Array Tests ===\n\n");

	/* TODO: Test dynarray_create - verify initial size=0, capacity=4 */

	/* TODO: Test dynarray_push_back - push 10, 20, 30, 40 (fills initial capacity) */

	/* TODO: Test dynarray_size - should return 4 */

	/* TODO: Test dynarray_get - verify all four values */

	/* TODO: Test dynarray_push_back - push 50 (triggers growth to capacity 8) */

	/* TODO: Test dynarray_set - set index 2 to 99, verify */

	/* TODO: Test dynarray_pop_back - pop and verify value is 50 */

	/* TODO: Test dynarray_pop_back - pop all remaining, verify size becomes 0 */

	/* TODO: Test bounds - get/set out of range (should handle gracefully) */

	/* TODO: Test dynarray_destroy - free everything, run under valgrind */

	/* TODO: Bonus - test dynarray_insert_at and dynarray_remove_at */

	dynarray_destroy(da);

	printf("\nAll tests passed!\n");
	return 0;
}
