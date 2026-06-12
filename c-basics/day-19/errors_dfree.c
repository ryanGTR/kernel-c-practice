// SPDX-License-Identifier: GPL-2.0
/*
 * Day 19 Step 5c — Double Free
 *
 * 目標：故意 free 兩次，看 valgrind / glibc 抓到。
 *
 * TODO：
 *   把下面 *** 那行的註解拿掉，重新編譯跑 valgrind 看報告。
 *
 * 編譯：gcc -Wall -Wextra -g errors_dfree.c -o errors_dfree
 * 跑：valgrind ./errors_dfree
 *
 * valgrind 預期：
 *   ==XX== Invalid free() / delete / delete[] / realloc()
 *   ==XX==    at free (...)
 *   ==XX==  Address ... is 0 bytes inside a block of size N free'd
 *
 * 或不透過 valgrind 直接跑時 glibc 自己會 abort：
 *   double free or corruption (...)
 *   Aborted (core dumped)
 *
 * 學到：
 *   - 每塊 heap block 只能 free **一次**。第二次是 UB
 *   - 防呆習慣：free 完立刻 p = NULL
 *     —— free(NULL) 是合法的 no-op，所以再 free 一次也沒事
 *   - 多人共用同一個 pointer（兩個 struct 都持有）→ 雙重釋放的常見來源
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int *p = malloc(sizeof(*p));
	if (!p)
		return 1;

	*p = 42;
	printf("*p = %d\n", *p);

	free(p);

	/* *** TODO: 拿掉下面這行的註解，再跑 valgrind ***
	 *
	 * free(p);
	 */

	return 0;
}
