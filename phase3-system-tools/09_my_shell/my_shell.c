// SPDX-License-Identifier: GPL-2.0
/*
 * my_shell - a minimal interactive shell
 *
 * Usage: ./my_shell
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main(void)
{
	char line[MAX_LINE];

	for (;;) {
		/* Print prompt */
		printf("mysh> ");
		fflush(stdout);

		/* Read a line */
		if (!fgets(line, sizeof(line), stdin))
			break;

		/* Remove trailing newline */
		line[strcspn(line, "\n")] = '\0';

		/* Skip empty lines */
		if (line[0] == '\0')
			continue;

		/* TODO: tokenize line into argv[] using strtok */

		/* TODO: handle built-in commands (cd, exit) */

		/* TODO: fork + execvp + waitpid */
	}

	printf("\n");
	return 0;
}
