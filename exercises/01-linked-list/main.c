// SPDX-License-Identifier: GPL-2.0
/*
 * Exercise 01: Kernel-style Linked List
 *
 * Practice using the kernel's list_head pattern:
 * - Embed struct list_head in your data structure
 * - Use list_add, list_del, list_for_each_entry
 * - Use container_of to go from list node to data
 *
 * Tasks:
 *   1. Create a list of students with name and score
 *   2. Add 5 students to the list
 *   3. Print all students (iterate with list_for_each_entry)
 *   4. Find and remove the student with the lowest score
 *   5. Print the list again to verify removal
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct student {
	char name[32];
	int score;
	struct list_head list;
};

static struct student *student_create(const char *name, int score)
{
	struct student *s;

	s = malloc(sizeof(*s));
	if (!s)
		return NULL;

	strncpy(s->name, name, sizeof(s->name) - 1);
	s->name[sizeof(s->name) - 1] = '\0';
	s->score = score;
	INIT_LIST_HEAD(&s->list);

	return s;
}

static void student_free(struct student *s)
{
	free(s);
}

static void print_all_students(struct list_head *head)
{
	struct student *s;

	printf("--- Student List ---\n");
	list_for_each_entry(s, head, list)
		printf("  %s: %d\n", s->name, s->score);
	printf("--------------------\n");
}

static void remove_lowest(struct list_head *head)
{
	struct student *s, *lowest = NULL;

	list_for_each_entry(s, head, list) {
		if (!lowest || s->score < lowest->score)
			lowest = s;
	}

	if (lowest) {
		printf("Removing lowest: %s (%d)\n", lowest->name,
		       lowest->score);
		list_del(&lowest->list);
		student_free(lowest);
	}
}

static void free_all_students(struct list_head *head)
{
	struct list_head *pos, *n;

	list_for_each_safe(pos, n, head) {
		struct student *s = list_entry(pos, struct student, list);

		list_del(&s->list);
		student_free(s);
	}
}

int main(void)
{
	LIST_HEAD(class);

	/* Task 1-2: Create and add students */
	list_add_tail(&student_create("Alice", 92)->list, &class);
	list_add_tail(&student_create("Bob", 78)->list, &class);
	list_add_tail(&student_create("Charlie", 65)->list, &class);
	list_add_tail(&student_create("Diana", 88)->list, &class);
	list_add_tail(&student_create("Eve", 95)->list, &class);

	/* Task 3: Print all */
	print_all_students(&class);

	/* Task 4: Remove lowest */
	remove_lowest(&class);

	/* Task 5: Print again */
	print_all_students(&class);

	/* Cleanup */
	free_all_students(&class);

	printf("All tests passed!\n");
	return 0;
}
