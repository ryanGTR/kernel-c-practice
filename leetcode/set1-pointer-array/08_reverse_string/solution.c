// SPDX-License-Identifier: GPL-2.0
/* Problem #08 — reverse_string 參考解 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

static void reverse_string(char *s)
{
	size_t len = strlen(s);
	if (len < 2)
		return;

	char *l = s, *r = s + len - 1;
	while (l < r) {
		char t = *l;
		*l = *r;
		*r = t;
		l++;
		r--;
	}
}

int main(void)
{
	char a[] = "hello";   reverse_string(a); assert(strcmp(a, "olleh") == 0);
	char b[] = "a";       reverse_string(b); assert(strcmp(b, "a") == 0);
	char c[] = "";        reverse_string(c); assert(strcmp(c, "") == 0);
	char d[] = "abcd";    reverse_string(d); assert(strcmp(d, "dcba") == 0);
	char e[] = "abcde";   reverse_string(e); assert(strcmp(e, "edcba") == 0);

	printf("✅ Problem #08 reverse_string passed\n");
	return 0;
}
