// SPDX-License-Identifier: GPL-2.0
/*
 * GDB Week Day 27 — 故意 NULL deref，練習從 core dump 反推 segfault。
 *
 * crash() 故意拆成獨立函式，這樣 core dump 的 `bt` 會看到
 *     crash() <- main()
 * 比直接在 main 裡 deref 更能練「是誰 deref 了 NULL」。
 */
#include <stdio.h>

static void crash(int *p)
{
	*p = 42;		/* 💥 p == NULL → SIGSEGV */
}

int main(void)
{
	int *p = NULL;

	printf("about to deref NULL...\n");
	crash(p);
	printf("you should never see this\n");
	return 0;
}
