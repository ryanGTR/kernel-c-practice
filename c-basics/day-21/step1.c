// SPDX-License-Identifier: GPL-2.0
/*
 * Day 21 Step 1 — value student（stack 上）
 *
 * 目標：第一次定義 struct、用 `&` 傳給函式、用 `->` 存 member。
 *
 * TODO：
 *   1. 在 main 裡宣告一個 struct student s（stack 上）
 *   2. 用 student_fill(&s, "Alice", 92) 填值
 *   3. 用 student_print(&s) 印
 *
 * 編譯：make step1
 * 預期輸出：
 *   Alice                 92
 */

#include "student.h"

int main(void)
{
	struct student s;
	student_fill(&s, "Alice", 92);
	student_print(&s);
	return 0;
}
