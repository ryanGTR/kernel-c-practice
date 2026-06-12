// SPDX-License-Identifier: GPL-2.0
/*
 * my_hexdump - reimplement hexdump -C
 *
 * Usage: my_hexdump [FILE]
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BYTES_PER_LINE 16

int main(int argc, char *argv[])
{
	FILE *fp;
	unsigned char buf[BYTES_PER_LINE];
	size_t n;
	unsigned int offset = 0;

	if (argc > 2) {
		fprintf(stderr, "Usage: %s [FILE]\n", argv[0]);
		return 1;
	}

	if (argc == 2) {
		fp = fopen(argv[1], "rb");
		if (!fp) {
			perror(argv[1]);
			return 1;
		}
	} else {
		fp = stdin;
	}

	/* TODO: read 16 bytes at a time with fread */
	/* TODO: print offset in hex (%08x) */
	/* TODO: print hex bytes (first 8, space, next 8) */
	/* TODO: print ASCII column (isprint -> char, else '.') */
	/* TODO: handle last incomplete line (pad hex with spaces) */

	(void)buf;
	(void)n;
	(void)offset;

	/* TODO: print final offset (total bytes) */

	if (fp != stdin)
		fclose(fp);

	return 0;
}
