// SPDX-License-Identifier: GPL-2.0
/*
 * Fib 遞迴版 — GDB Week Day 25/26 練習用。
 * 故意用遞迴而不是迴圈，因為 Day 26 要看 call stack。
 */
#include <stdio.h>

static int call_count;

int fib(int n)
{
	call_count++;
	if (n < 2)
		return n;
	return fib(n - 1) + fib(n - 2);
}

int main(void)
{
	int n = 5;
	int result = fib(n);

	printf("fib(%d) = %d (called %d times)\n", n, result, call_count);
	return 0;
}
