// SPDX-License-Identifier: GPL-2.0
/*
 * Day 18 Step 2 — sizeof 的陷阱（array decay）
 *
 * 目標：親眼看到 sizeof 在「定義處」和「函式內」得到不同大小。
 *
 * TODO（照 wiki/daily/2026-04/day-18.md 步驟 2）：
 *   1. 寫一個 show_size(int arr[]) 函式，在裡面印 sizeof(arr)
 *   2. main 裡宣告 int arr[5]，外面印 sizeof(arr)，再呼叫 show_size(arr)
 *
 * 編譯：gcc -Wall -Wextra sizeof_trap.c -o sizeof_trap
 * 預期輸出：
 *   outside: sizeof(arr) = 20      ← 5 * sizeof(int)
 *   inside:  sizeof(arr) = 8       ← 只是 pointer
 *
 * gcc 會給警告：
 *   sizeof on array function parameter will return size of 'int *'
 * 這個警告記起來——看到它就代表有人把陣列傳進函式又想量長度。
 */

#include <stdio.h>

/* TODO: 定義 show_size(int arr[]) */
static void show_size(int arr[])
{
	printf("inside size of %zu\n", sizeof(arr));
}

int main(void)
{
	/* TODO: 定義 int arr[5] */
	int arr[5] = { 1, 2, 3, 4, 5 };
	/* TODO: 印 outside sizeof */
	printf("size of: %zu\n", sizeof(arr));

	/* TODO: 呼叫 show_size(arr) */
	show_size(arr);
	return 0;
}
