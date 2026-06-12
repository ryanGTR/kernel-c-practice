// SPDX-License-Identifier: GPL-2.0
/*
 * Day 21 Step 2 — heap 上的 student（呼應 Day 19 的 malloc/free）
 *
 * 目標：第一次「在 heap 配 struct」、體會 ownership 移轉。
 *
 * TODO：
 *   1. struct student *s = student_create("Bob", 85);
 *   2. NULL 檢查 → return 1
 *   3. student_print(s);
 *   4. student_destroy(s);
 *   5. （好習慣）s = NULL;
 *
 * 編譯：make step2
 * 預期輸出：
 *   Bob                   85
 *
 * valgrind：必須 0 leaks 0 errors（這是通關條件）
 *   make valgrind  或 valgrind --leak-check=full ./step2
 *
 * 觀念：student_create 把記憶體所有權「移交」給呼叫端。
 *       誰拿到 pointer 誰負責 destroy。
 */

#include "student.h"

int main(void)
{
	/* TODO: 照上面 5 步驟做 */
	struct student *s = student_create("Bob", 85);
	if (!s)
		return 1;
	student_print(s);
	student_destroy(s);
	s = NULL;
	return 0;
}
