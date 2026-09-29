// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <assert.h>
#include "linked_list.h"

int main(void)
{
	struct node *list = NULL;

	printf("=== Linked List Tests ===\n\n");

	/* TODO: Test list_create - should return NULL (empty list) */
	list = list_create();
	assert(list == NULL);
	/* TODO: Test list_append - add 10, 20, 30; print should show 10 -> 20 -> 30 */
	list = list_append(list, 10);
	assert(list != NULL);
	assert(list->data == 10);
	list = list_append(list, 20);
	assert(list->next->data == 20);
	list = list_append(list, 30);
	assert(list->next->next->data == 30);

	/* TODO: Test list_prepend - prepend 5; print should show 5 -> 10 -> 20 -> 30 */
	list = list_prepend(list, 5);
	assert(list != NULL);
	assert(list->data == 5);
	assert(list->next->data == 10);
	assert(list->next->next->data == 20);
	assert(list->next->next->next->data == 30);

	/* TODO: Test list_find - find 20 (should succeed), find 99 (should return NULL) */
	struct node *find_20 = list_find(list, 20);

	assert(find_20 != NULL);

	struct node *find_99 = list_find(list, 99);

	assert(find_99 == NULL);

	struct node *find_NULL = list_find(NULL, 20);

	assert(find_NULL == NULL);

	/* TODO: Test list_delete - delete middle node (20), verify list */
	list = list_delete(list, 20);
	assert(list != NULL);
	assert(list->data == 5);
	assert(list->next->data == 10);
	assert(list->next->next->data == 30);
	assert(list->next->next->next == NULL);

	/* TODO: Test list_delete - delete head node (5), verify list */
	list = list_delete(list, 5);
	assert(list != NULL);
	assert(list->data == 10);
	assert(list->next->data == 30);
	assert(list->next->next == NULL);

	/* TODO: Test list_delete - delete tail node (30), verify list */
	list = list_delete(list, 30);
	assert(list != NULL);
	assert(list->data == 10);
	assert(list->next == NULL);

	/* TODO: Test list_delete - delete from single-element list */
	list = list_delete(list, 10);
	assert(list == NULL);

	/* TODO: Test list_delete - delete nonexistent value, verify no change */
	list = list_append(list, 1);
	list = list_append(list, 2);
	list = list_delete(list, 97);
	assert(list->data == 1);
	assert(list->next->data == 2);
	assert(list->next->next == NULL);
	list_print(list);

	/* TODO: Test list_free_all - free everything, run under valgrind */
	list_free_all(NULL);

	struct node *one = list_append(NULL, 1);

	list_free_all(one);

	struct node *many = list_append(NULL, 1);

	many = list_append(many, 1);
	many = list_append(many, 2);
	many = list_append(many, 3);
	list_free_all(many);

	/* TODO: Bonus - test list_reverse */

	list_free_all(list);

	printf("\nAll tests passed!\n");
	return 0;
}
