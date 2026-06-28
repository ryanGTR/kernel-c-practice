// SPDX-License-Identifier: GPL-2.0
/*
 * my_wc - reimplement wc(1)
 *
 * Usage: my_wc [FILE...]
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct counts {
	long lines, words, bytes;
};

static struct counts wc_stream(FILE *fp)
{
	struct counts c = { 0 };

	enum { OUT, IN } state = OUT;
	int ch;

	while ((ch = getc(fp)) != EOF) {
		c.bytes++;

		if (ch == '\n')
			c.lines++;

		if (isspace(ch)) {
			state = OUT;
		} else if (state == OUT) {
			state = IN;
			c.words++;
		}
	}
	return c;
}

/* Number of decimal digits in n (matches GNU wc's dynamic column width). */
static int count_width(long n)
{
	int w = 1;

	while (n >= 10) {
		n /= 10;
		w++;
	}
	return w;
}

static void print_row(struct counts c, const char *name)
{
	/*
	 * bytes is always the largest of the three counts (every line needs
	 * a '\n' byte, every word at least one byte), so its width governs
	 * the whole row, right-justified like GNU wc.
	 */
	int w = count_width(c.bytes);

	printf("%*ld %*ld %*ld", w, c.lines, w, c.words, w, c.bytes);
	if (name[0])
		printf(" %s", name);
	putchar('\n');
}

int main(int argc, char *argv[])
{
	struct counts total = { 0 };
	int ret = 0;

	if (argc < 2) {
		struct counts c = wc_stream(stdin);
		print_row(c, "");
		return 0;
	}

	for (int i = 1; i < argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (!fp) {
			perror(argv[i]);
			ret = 1;
			continue;
		}
		struct counts c = wc_stream(fp);
		fclose(fp);
		print_row(c, argv[i]);
		total.lines += c.lines;
		total.words += c.words;
		total.bytes += c.bytes;
	}

	if (argc > 2) {
		print_row(total, "total");
	}
	return ret;
}
