// SPDX-License-Identifier: GPL-2.0
/*
 * Day 19 Step 3 — calloc vs malloc
 *
 * 目標：親眼看到 malloc 配的記憶體是「垃圾值」，calloc 是「全 0」。
 *
 * TODO（照 wiki/daily/2026-04/day-19.md 步驟 3）：
 *   1. malloc 10 個 int → 直接讀第一格（不寫先讀）
 *      ⚠️ 這是 undefined behavior（讀未初始化），但這是練習觀察的點
 *   2. calloc 10 個 int → 讀第一格
 *   3. 印兩者第一格的值
 *   4. 都 free
 *
 * 編譯：gcc -Wall -Wextra -g calloc_demo.c -o calloc_demo
 * 跑：valgrind --leak-check=full ./calloc_demo
 *
 * valgrind 預期：
 *   malloc 那邊噴 "Conditional jump or move depends on uninitialised value(s)"
 *   或 "Use of uninitialised value of size 4"
 *   calloc 那邊乾淨
 *
 * 學到：要讀的記憶體就用 calloc 或先寫過。malloc 拿到的是不可預測的垃圾。
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	size_t n = 10;

	/* TODO: int *a = malloc(n * sizeof(*a)); */
	int *a = malloc(n * sizeof(*a));
	/* TODO: int *b = calloc(n, sizeof(*b)); */
	int *b = calloc(n, sizeof(*b));
	/* TODO: NULL 檢查兩個 */
	if (!a || !b) {
		free(a);
		free(b);
		return 1;
	}
	printf("malloc[0] = %d (垃圾值)\n", a[0]);
	printf("calloc[0] = %d (應該是 0)\n", b[0]);

	/* TODO: free 兩個 */
	free(a);
	free(b);
	a = NULL;
	b = NULL;
	return 0;
}
