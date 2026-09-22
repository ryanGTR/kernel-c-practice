// SPDX-License-Identifier: GPL-2.0
#include <stdlib.h>
#include <string.h>
#include "dlist.h"

struct dlist *dl_new(void)
{
	/* TODO: implement */
	return NULL;
}

void dl_free(struct dlist *l)
{
	/* TODO: implement */
	(void)l;
}

bool dl_insert_head(struct dlist *l, const char *s)
{
	/* TODO: implement */
	(void)l;
	(void)s;
	return false;
}

bool dl_insert_tail(struct dlist *l, const char *s)
{
	/* TODO: implement */
	(void)l;
	(void)s;
	return false;
}

char *dl_remove_head(struct dlist *l)
{
	/* TODO: implement */
	(void)l;
	return NULL;
}

char *dl_remove_tail(struct dlist *l)
{
	/* TODO: implement */
	(void)l;
	return NULL;
}

int dl_size(const struct dlist *l)
{
	/* TODO: implement */
	(void)l;
	return 0;
}

bool dl_delete_mid(struct dlist *l)
{
	/* TODO: implement */
	(void)l;
	return false;
}
