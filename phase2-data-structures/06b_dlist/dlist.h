/* SPDX-License-Identifier: GPL-2.0 */
#ifndef DLIST_H
#define DLIST_H

#include <stdbool.h>

/*
 * The "textbook" doubly linked list: the node WRAPS the data, the list
 * keeps head/tail pointers, and both ends are terminated by NULL.
 *
 * Compare with lab0-c (list.h): there the node is EMBEDDED in the data,
 * and a sentinel makes the list circular -- no NULL anywhere.
 */
struct dnode {
	char *value;			/* owned: malloc'd copy of the string */
	struct dnode *prev;
	struct dnode *next;
};

struct dlist {
	struct dnode *head;		/* NULL when empty */
	struct dnode *tail;		/* NULL when empty */
};

struct dlist *dl_new(void);
void dl_free(struct dlist *l);

bool dl_insert_head(struct dlist *l, const char *s);
bool dl_insert_tail(struct dlist *l, const char *s);

/* Unlink and return the value; the CALLER frees it. NULL if empty. */
char *dl_remove_head(struct dlist *l);
char *dl_remove_tail(struct dlist *l);

int dl_size(const struct dlist *l);

/* Delete the floor(n / 2)-th node (0-based), same rule as q_delete_mid. */
bool dl_delete_mid(struct dlist *l);

#endif /* DLIST_H */
