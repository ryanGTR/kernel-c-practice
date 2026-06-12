// SPDX-License-Identifier: GPL-2.0
/*
 * Day 19 Step 5a — 漏 free（definitely lost）
 *
 * 目標：故意不 free，用 valgrind 看「definitely lost」長什麼樣。
 *
 * 這支程式 **設計上就會 leak**——不要修。觀察 valgrind 的報告才是重點。
 *
 * 編譯：gcc -Wall -Wextra -g errors_leak.c -o errors_leak
 * 跑：valgrind --leak-check=full --show-leak-kinds=all ./errors_leak
 *
 * valgrind 預期會看到：
 *   ==XX== definitely lost: 40 bytes in 1 blocks
 *   ==XX==    at malloc (...)
 *   ==XX==    by main (errors_leak.c:LINE)
 *
 * 學到：
 *   - 「definitely lost」= 沒有任何 pointer 指到那塊記憶體了，glibc 回收不了
 *   - 程式結束時 OS 會釋放整個 process 記憶體，但這 **不算解** ——
 *     long-running 服務（kernel module / daemon）leak 一陣子就 OOM
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int *arr = malloc(10 * sizeof(*arr));
	if (!arr)
		return 1;

	for (size_t i = 0; i < 10; i++)
		arr[i] = (int)i;

	printf("arr[5] = %d\n", arr[5]);

	/* ★ 故意不 free(arr) — 觀察 valgrind */
	return 0;
}
