// SPDX-License-Identifier: GPL-2.0
/*
 * my_grep - reimplement simplified grep(1)
 *
 * Usage: my_grep [-i] [-n] [-c] PATTERN [FILE...]
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	/* TODO: implement grep (fixed string, not regex) */
	(void)argc;
	(void)argv;
	return 0;
}
