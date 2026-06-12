// SPDX-License-Identifier: GPL-2.0
/*
 * Day 19 Step 2 — 配置動態陣列
 *
 * 目標：
 *   1. 用 malloc 配置 N 個 int 的陣列
 *   2. 用 sizeof(*arr) 慣用法（不寫 sizeof(int)）
 *   3. 寫值 + 印出來 + 釋放
 *
 * TODO（照 wiki/daily/2026-04/day-19.md 步驟 2）：
 *   1. size_t n = 10; int *arr = malloc(n * sizeof(*arr));
 *   2. NULL 檢查失敗 return 1
 *   3. for 迴圈寫 arr[i] = i * i
 *   4. for 迴圈印每個值（用 ", " 分隔）
 *   5. free
 *
 * 編譯：gcc -Wall -Wextra -g arrays.c -o arrays
 * 預期輸出：0, 1, 4, 9, 16, 25, 36, 49, 64, 81
 * valgrind 預期：0 leaks 0 errors
 *
 * 為什麼用 sizeof(*arr) 而非 sizeof(int)？
 *   如果你之後改 arr 型別（int → long），sizeof(*arr) 自動跟著變。
 *   寫 sizeof(int) 就會對不上 → 配置不夠 → 越界。
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	size_t n = 10;

	/* TODO: int *arr = malloc(n * sizeof(*arr)); */
	int *arr = malloc(n * sizeof(*arr));
	/* TODO: NULL 檢查 */
	if (!arr) {
		return -1;
	}
	/* TODO: 寫值 i*i */
	for (size_t i = 0; i < n; i++) {
		arr[i] = i * i;
	}

	/* TODO: 印出（最後一個不要 trailing 逗號）*/
	for (size_t i = 0; i < n; i++) {
		printf("%d ", arr[i]);

		if (i != n - 1) {
			printf(" ,");
		}
	}

	/* TODO: free */
	free(arr);
	(void)n;
	return 0;
}
