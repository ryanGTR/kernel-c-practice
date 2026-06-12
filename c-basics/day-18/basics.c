// SPDX-License-Identifier: GPL-2.0
/*
 * Day 18 Step 1 — 陣列 = 連續記憶體
 *
 * TODO（照 wiki/daily/2026-04/day-18.md 步驟 1）：
 *   1. 宣告 int arr[5] = { 10, 20, 30, 40, 50 };
 *   2. 印出 arr[2]、*(arr+2)、2[arr] 三個寫法
 *   3. 用 for 迴圈印出 &arr[0..4]，確認位址差 4 bytes
 *
 * 編譯：gcc -Wall -Wextra basics.c -o basics
 * 預期：三種寫法都印出 30；位址等差 4
 */

#include <stdio.h>

int main(void)
{
	/* TODO: 宣告陣列 */
	int arr[5] = { 10, 20, 30, 40, 50 };
	/* TODO: 印三種等價寫法 */
	printf("1: %d\n", arr[2]);
	printf("2: %d\n", *(arr + 2));
	printf("3: %d\n", 2 [arr]);

	/* TODO: 迴圈印位址 */
	for (int i = 0; i <= 4; i++) {
		printf("&arr[%d] = %p\n", i, &arr[i]);
	}

	return 0;
}
