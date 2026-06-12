// SPDX-License-Identifier: GPL-2.0
/* Problem #17 — refcounted_object 參考解 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct rc_obj {
	int ref;
	char *data;
};

static struct rc_obj *rc_new(const char *s)
{
	struct rc_obj *o = malloc(sizeof(*o));
	if (!o) return NULL;
	o->data = strdup(s);
	if (!o->data) {
		free(o);
		return NULL;
	}
	o->ref = 1;
	return o;
}

static void rc_get(struct rc_obj *o)
{
	o->ref++;
}

static void rc_put(struct rc_obj *o)
{
	if (--o->ref == 0) {
		free(o->data);
		free(o);
	}
}

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
	rc_put(o);

	struct rc_obj *o2 = rc_new("temp");
	rc_put(o2);

	printf("✅ Problem #17 refcounted_object passed\n");
	return 0;
}
