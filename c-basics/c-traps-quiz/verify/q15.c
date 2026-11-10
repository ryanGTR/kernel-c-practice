/* 4.7 / 4.5 strcat malloc +1 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void)
{
	const char *s = "ab", *t = "cd";
	char *r = malloc(strlen(s) + strlen(t));

	strcpy(r, s);
	strcat(r, t);
	printf("%s\n", r);
	free(r);
	return 0;
}
