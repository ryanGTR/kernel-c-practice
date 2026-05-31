// SPDX-License-Identifier: GPL-2.0
/*
 * GDB Week Day 27 — 無窮迴圈，用來練 `gdb -p <PID>` attach 到正在跑的程式。
 *
 * 跑起來後另開一個終端：
 *     gdb -p $(pgrep loop)
 * attach 後 Ctrl-C 會停在當前行，可以 bt / print tick。
 */
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	unsigned long tick = 0;

	while (1) {
		tick++;
		if (tick % 100000000UL == 0)
			printf("tick = %lu\n", tick);
	}
	return 0;
}
