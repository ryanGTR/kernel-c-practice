// SPDX-License-Identifier: GPL-2.0
/* Problem #17 — refcounted_object */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct rc_obj {
	int ref;
	char *data;
};

static struct rc_obj *rc_new(const char *s) { (void)s; return NULL; }
static void rc_get(struct rc_obj *o)         { (void)o; }
static void rc_put(struct rc_obj *o)         { (void)o; }

int main(void)
{
	struct rc_obj *o = rc_new("shared data");
	assert(o != NULL);
	assert(o->ref == 1);
	assert(strcmp(o->data, "shared data") == 0);

	rc_get(o); assert(o->ref == 2);
	rc_get(o); assert(o->ref == 3);
	rc_put(o); assert(o->ref == 2);
	rc_put(o); assert(o->ref == 1);
	rc_put(o); /* ref → 0，o 已被 free，不能再讀 */

	/* 第二輪：alloc + 立刻 put */
	struct rc_obj *o2 = rc_new("temp");
	rc_put(o2);

	printf("✅ Problem #17 refcounted_object passed\n");
	return 0;
}
