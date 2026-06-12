// SPDX-License-Identifier: GPL-2.0
/* Problem #06 — my_strdup 參考解 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static char *my_strdup(const char *s)
{
	size_t len = strlen(s);
	char *p = malloc(len + 1);
	if (!p)
		return NULL;
	memcpy(p, s, len + 1);   /* 含 '\0' */
	return p;
}

int main(void)
{
	char *p = my_strdup("hello");
	assert(p != NULL);
	assert(strcmp(p, "hello") == 0);
	free(p);

	char *empty = my_strdup("");
	assert(empty != NULL);
	assert(empty[0] == '\0');
	free(empty);

	printf("✅ Problem #06 my_strdup passed\n");
	return 0;
}
