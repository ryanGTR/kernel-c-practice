// SPDX-License-Identifier: GPL-2.0
/*
 * Day 18 Step 4 — 故意越界，感受 UB
 *
 * 目標：看 valgrind 抓到越界讀寫，記住它的輸出長相。
 *
 * TODO：
 *   1. 宣告 int arr[5] = { 1, 2, 3, 4, 5 };
 *   2. 故意讀 arr[5]（超過合法範圍 0..4）
 *   3. 故意寫 arr[10] = 99;
 *
 * 編譯 & 跑：
 *   make oob
 *   valgrind --track-origins=yes ./oob
 *
 * 觀察 valgrind 輸出：
 *   - Invalid read of size 4
 *   - Invalid write of size 4
 *   - "X bytes after a block of size Y" 的表達方式
 *
 * ⚠️ 這個程式的行為是 undefined behavior。
 *    每次跑結果可能不一樣（或看起來都正常），這就是 UB 的恐怖之處。
 *    valgrind 是你的眼睛，幫你看到肉眼看不到的錯。
 */

#include <stdio.h>

int main(void)
{
	int arr[5] = { 1, 2, 3, 4, 5 };

	/* TODO: 越界讀 */
	printf("Damn it: %d\n", arr[5]);
	/* TODO: 越界寫 */
	arr[10] = 99;
	return 0;
}
