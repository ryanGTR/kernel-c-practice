// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 05  my_strstr      子字串搜尋（my_grep 的心臟）
 * @fn: my_strstr
 *
 * 規格：回傳 needle 在 hay 裡第一次出現的位置；找不到回 NULL。
 *       不准 #include <string.h>（測試用的除外）。
 *
 * 動手前先答：
 *   Q1. 回傳的指標指向誰的記憶體？呼叫者要不要 free 它？
 *       A:
 *   Q2. needle 是空字串 "" 時應該回什麼？（查 man strstr）
 *       A:
 *   Q3. 「部分匹配後失敗」要怎麼辦？例：hay="aaab", needle="aab"
 *       從 hay[0] 開始比，比到第 3 個字元才失敗 —— 下一輪要從 hay[1] 還是 hay[3] 開始？
 *       A:
 *
 * 記憶體圖：
 *   my_strstr frame:
 *     ?   <- 你需要幾個指標？各自指向哪裡？
 *
 * 💡 Q3 就是這題的全部。答錯 → 下面的 "aaab"/"aab" 測試會紅。
 *    （答對了你就順手理解了為什麼會有 KMP 演算法：它就是在省下這個「退回去」的成本。）
 */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static const char *my_strstr(const char *hay, const char *needle)
{
	(void)hay;
	(void)needle;	/* TODO: delete these and write your code */
	return NULL;
}

int main(void)
{
	const char *hay = "kernel panic";
	const char *p;

	/* hit at the start */
	p = my_strstr(hay, "kernel");
	assert(p == hay);

	/* hit in the middle */
	p = my_strstr(hay, "nel");
	assert(p == hay + 3);

	/* hit at the end */
	p = my_strstr(hay, "panic");
	assert(p == hay + 7);

	/* single char */
	p = my_strstr(hay, "p");
	assert(p == hay + 7);

	/* no hit */
	assert(my_strstr(hay, "zzz") == NULL);

	/* needle longer than hay */
	assert(my_strstr("ab", "abc") == NULL);

	/* empty needle: strstr(3) says return hay */
	assert(my_strstr(hay, "") == hay);

	/* empty hay */
	assert(my_strstr("", "a") == NULL);
	assert(my_strstr("", "") != NULL);

	/* 🔥 partial match then fail -- must back up, not skip ahead */
	p = my_strstr("aaab", "aab");
	assert(p != NULL);
	assert(*p == 'a' && p[1] == 'a' && p[2] == 'b');

	/* same trap, longer */
	p = my_strstr("abcabcabd", "abcabd");
	assert(p != NULL && p[5] == 'd');

	printf("kata 05: PASS\n");
	return 0;
}
