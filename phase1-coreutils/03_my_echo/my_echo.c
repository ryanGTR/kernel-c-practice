// SPDX-License-Identifier: GPL-2.0
/*
 * my_echo - reimplement echo(1)
 *
 * Usage: my_echo [-n] [STRING...]
 */

#include <stdio.h>

int main(int argc, char *argv[])
{
	int start = 1;
	int newline = 1;

	if (argc > 1 && strcmp(argv[1], '-n') == 0) {
		start = 2;
		newline = 0;
	}
	return 0;
}
