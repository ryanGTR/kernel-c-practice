// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 03  my_strrev      雙指標往中間走（原地修改）
 * @fn: my_strrev
 *
 * 規格：把 s 原地（in-place）反轉。不准配置新記憶體，不准用 <string.h>。
 *
 * 動手前先答：
 *   Q1. 參數為什麼不能是 const char *？（跟 kata 01 對照）
 *       A:
 *   Q2. 長度是奇數時，正中間那個字元要怎麼處理？
 *       A:
 *   Q3. 迴圈的停止條件是什麼？寫成 `left != right` 會怎樣？（提示：偶數長度）
 *       A:
 *
 * 記憶體圖：
 *   my_strrev frame:
 *     left    char *   <- 指向 s 的哪裡？
 *     right   char *   <- 一開始要指到哪？（小心 '\0'）
 *
 * ⚠️ 陷阱：字串字面量 "abc" 是唯讀的。
 *    char *s = "abc"; my_strrev(s);  → segfault（寫進 .rodata）
 *    char s[] = "abc"; my_strrev(s); → OK（stack 上的可寫副本）
 *    這兩行看起來只差一個符號。下面的測試用的是哪種？想清楚再寫。
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>	/* only for strcmp in the test, not for your impl */

static void my_strrev(char *s)
{
	(void)s;	/* TODO: delete this and write your code */
}

int main(void)
{
	char empty[] = "";
	char one[] = "a";
	char even[] = "abcd";
	char odd[] = "abcde";
	char word[] = "kernel";

	my_strrev(empty);
	assert(strcmp(empty, "") == 0);

	my_strrev(one);
	assert(strcmp(one, "a") == 0);

	my_strrev(even);
	assert(strcmp(even, "dcba") == 0);

	my_strrev(odd);
	assert(strcmp(odd, "edcba") == 0);

	my_strrev(word);
	assert(strcmp(word, "lenrek") == 0);

	/* Reversing twice gets you back where you started. */
	my_strrev(word);
	assert(strcmp(word, "kernel") == 0);

	printf("kata 03: PASS\n");
	return 0;
}
