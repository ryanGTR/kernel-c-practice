// SPDX-License-Identifier: GPL-2.0
/* Problem #02 — my_strcpy 參考解 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

static char *my_strcpy(char *dst, const char *src)
{
	char *p = dst;
	while ((*p++ = *src++) != '\0')
		;   /* 經典 K&R 寫法 */
	return dst;
}

int main(void)
{
	char buf[32];
	assert(my_strcpy(buf, "hello") == buf);
	assert(strcmp(buf, "hello") == 0);

	my_strcpy(buf, "");
	assert(buf[0] == '\0');

	my_strcpy(buf, "a longer string");
	assert(strcmp(buf, "a longer string") == 0);

	printf("✅ Problem #02 my_strcpy passed\n");
	return 0;
}
