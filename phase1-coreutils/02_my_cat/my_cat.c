// SPDX-License-Identifier: GPL-2.0
/*
 * my_cat - reimplement cat(1)
 *
 * Usage: my_cat [-n] [FILE...]
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

static void cat_stream(FILE *fp, int number, long *lineno, int *at_line_start)
{
	int ch;

	while ((ch = getc(fp)) != EOF) {
		if (number && *at_line_start) {
			printf("%6ld\t", ++(*lineno));
			*at_line_start = 0;
		}
		putchar(ch);
		if (ch == '\n')
			*at_line_start = 1;
	}
}

int main(int argc, char *argv[])
{
	int number = 0;
	int start = 1;
	long lineno = 0;
	int status = 0;
	int at_line_start = 1;

	if (argc > 1 && strcmp(argv[1], "-n") == 0) {
		number = 1;
		start = 2;
	}

	if (start == argc) {
		cat_stream(stdin, number, &lineno, &at_line_start);
		return 0;
	}

	for (int i = start; i < argc; i++) {
		FILE *fp;

		if (strcmp(argv[i], "-") == 0) {
			fp = stdin;
		} else {
			fp = fopen(argv[i], "r");

			if (!fp) {
				fprintf(stderr, "my_cat: %s: %s\n", argv[i],
					strerror(errno));
				status = 1;
				continue;
			}
		}
		cat_stream(fp, number, &lineno, &at_line_start);
		if (fp != stdin)
			fclose(fp);
	}

	return status;
}
