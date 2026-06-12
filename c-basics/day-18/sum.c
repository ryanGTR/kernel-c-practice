// SPDX-License-Identifier: GPL-2.0
/*
 * Day 18 Step 3 — 正確的陣列函式簽名
 *
 * 目標：寫 sum(const int *arr, size_t n)，用 ARRAY_SIZE macro 算長度。
 *
 * TODO（照 wiki/daily/2026-04/day-18.md 步驟 3）：
 *   1. 定義 ARRAY_SIZE(x) macro
 *   2. 寫 sum(const int *arr, size_t n)：累加全部回傳 int
 *   3. main 裡用 int nums[] = { 3, 1, 4, 1, 5, 9, 2, 6 };
 *      呼叫 sum(nums, ARRAY_SIZE(nums))
 *
 * 編譯：gcc -Wall -Wextra sum.c -o sum
 * 預期輸出：sum = 31
 *
 * 三個習慣檢查：
 *   [ ] 參數用 const int *arr（宣告意圖：不改陣列）
 *   [ ] 長度用 size_t n（不要用 int）
 *   [ ] 在 main 用 ARRAY_SIZE 算長度（不要手寫 8）
 */

#include <stdio.h>
#include <stddef.h>

/* TODO: 定義 ARRAY_SIZE(x) */

/* TODO: 實作 sum */

int main(void)
{
	int nums[] = { 3, 1, 4, 1, 5, 9, 2, 6 };

	/* TODO: 呼叫 sum 並印出結果 */

	return 0;
}
