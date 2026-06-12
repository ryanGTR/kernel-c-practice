// SPDX-License-Identifier: GPL-2.0
/*
 * Day 19 Step 1 — 最小 malloc / free 循環
 *
 * 目標：建立「每個 malloc 配一個 free」的肌肉記憶，valgrind 確認 0 leaks。
 *
 * TODO（照 wiki/daily/2026-04/day-19.md 步驟 1）：
 *   1. malloc 一個 int
 *   2. 失敗檢查（NULL）
 *   3. 寫 42、印出 *p
 *   4. free 完置 NULL（防 use-after-free）
 *
 * 編譯：gcc -Wall -Wextra -g basic.c -o basic
 * 預期輸出：*p = 42
 * valgrind 預期：All heap blocks were freed -- no leaks are possible
 *
 * 兩個慣用法檢查：
 *   [ ] malloc(sizeof(*p))，不要寫 sizeof(int)
 *   [ ] free 完立刻 p = NULL
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	/* TODO: int *p = malloc(sizeof(*p)); */
	int *p = malloc(sizeof(*p));
	/* TODO: if (!p) return 1; */
	if (!p) {
		return 1;
	}
	/* TODO: *p = 42; */
	*p = 42;
	/* TODO: printf("*p = %d\n", *p); */
	printf("%d \n", *p);
	/* TODO: free(p); p = NULL; */
	free(p);
	p = NULL;
	return 0;
}
