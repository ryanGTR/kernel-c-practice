// SPDX-License-Identifier: GPL-2.0
/*
 * Day 19 Step 5b — Use-After-Free（UAF）
 *
 * 目標：故意 free 後再讀，看 valgrind 抓到「Invalid read」。
 *
 * TODO：
 *   把下面 *** 那行的註解拿掉，重新編譯跑 valgrind 看報告。
 *
 * 編譯：gcc -Wall -Wextra -g errors_uaf.c -o errors_uaf
 * 跑：valgrind ./errors_uaf
 *
 * valgrind 預期：
 *   ==XX== Invalid read of size 4
 *   ==XX==    at main (errors_uaf.c:LINE)
 *   ==XX==  Address ... is 0 bytes inside a block of size 4 free'd
 *   ==XX==    at free (...)
 *   ==XX==    by main (errors_uaf.c:LINE)
 *
 * 學到：
 *   - free 後 pointer 還在但記憶體已歸還 glibc
 *   - 「free 完置 NULL」就是為了讓這種情況立刻 segfault
 *     而不是默默讀垃圾、靜悄悄毀掉整個系統
 *   - 真實 CVE 大宗：driver 漏 NULL 化指標 → attacker 觸發 UAF → 提權
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int *p = malloc(sizeof(*p));
	if (!p)
		return 1;

	*p = 42;
	printf("before free: *p = %d\n", *p);

	free(p);

	/* *** TODO: 拿掉下面這行的註解，再跑 valgrind ***
	 *
	 * printf("after free:  *p = %d\n", *p);
	 */

	return 0;
}
