// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <assert.h>
#include "linked_list.h"

int main(void)
{
	struct node *list = NULL;

	printf("=== Linked List Tests ===\n\n");

	/* TODO: Test list_create - should return NULL (empty list) */

	/* TODO: Test list_append - add 10, 20, 30; print should show 10 -> 20 -> 30 */

	/* TODO: Test list_prepend - prepend 5; print should show 5 -> 10 -> 20 -> 30 */

	/* TODO: Test list_find - find 20 (should succeed), find 99 (should return NULL) */

	/* TODO: Test list_delete - delete middle node (20), verify list */

	/* TODO: Test list_delete - delete head node (5), verify list */

	/* TODO: Test list_delete - delete tail node (30), verify list */

	/* TODO: Test list_delete - delete from single-element list */

	/* TODO: Test list_delete - delete nonexistent value, verify no change */

	/* TODO: Test list_free_all - free everything, run under valgrind */

	/* TODO: Bonus - test list_reverse */

	list_free_all(list);

	printf("\nAll tests passed!\n");
	return 0;
}
