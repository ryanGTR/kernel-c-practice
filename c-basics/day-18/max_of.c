// SPDX-License-Identifier: GPL-2.0
/*
 * Day 18 今日挑戰 —— 自己寫 max_of()
 *
 * 規格：
 *   static int max_of(const int *arr, size_t n);
 *
 * 要求：
 *   - 回傳 arr 中的最大值
 *   - 參數用 const int *arr（你不改內容）
 *   - 長度用 size_t n
 *   - n == 0 時的行為：你自己決定（提示：可以回傳 INT_MIN 或用 assert）
 *
 * 驗收：
 *   - gcc -Wall -Wextra -Werror 零 warning
 *   - 至少測三種輸入：全正、有負、單一元素
 *
 * ⚠️ 不要看 sum.c 的答案。自己寫一次，卡住再回去翻 wiki。
 *    「寫得出來 > 看得懂」—— 你自己寫的戒律 #2。
 */

#include <assert.h>
#include <stdio.h>
#include <stddef.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* TODO: 實作 max_of */
static int max_of(const int *arr, size_t n)
{
	assert(n > 0);
	int max = arr[0];
	for (size_t i = 0; i < n; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	return max;
}

int main(void)
{
	int a[] = { 3, 1, 4, 1, 5, 9, 2, 6 };
	int b[] = { -5, -2, -8, -1, -3 };
	int c[] = { 42 };

	/* TODO: 呼叫 max_of 三次並印出結果
	 *   預期：a → 9, b → -1, c → 42
	 */
	printf(" max of a: %d\n", max_of(a, ARRAY_SIZE(a)));
	printf(" max of b: %d\n", max_of(b, ARRAY_SIZE(b)));
	printf(" max of c: %d\n", max_of(c, ARRAY_SIZE(c)));
	return 0;
}
