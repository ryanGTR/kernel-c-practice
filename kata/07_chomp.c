// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 07  chomp          去掉 fgets 的行尾 '\n'
 * @fn: chomp
 *
 * 規格：如果 line 結尾是 '\n'，把它換成 '\0'，回傳 1；否則不動，回傳 0。
 *
 * 動手前先答：
 *   Q1. fgets 什麼時候**不會**在結尾給你 '\n'？（兩種情況）
 *       A:
 *   Q2. 🔥 line 是空字串 "" 時，`line[strlen(line) - 1]` 會讀到哪裡？
 *       strlen 回傳 size_t（unsigned）。0 - 1 是多少？
 *       A:
 *
 * 🔥 這題是專門設計來讓你 `make` 綠、`make asan` 紅的。
 *
 *    空字串時 strlen(line) - 1 → size_t 的 0 減 1 → **不是 -1，是 18446744073709551615**
 *    （unsigned 環繞）。line[那個天文數字] 讀出去，指標飛到宇宙外。
 *    就算你寫 (int) 讓它變成 line[-1]，那也是讀 buffer 前面一個 byte —— 一樣是越界。
 *
 *    而它**大概率不會 crash**，也大概率不等於 '\n' —— 所以 assert 全過、你看到綠燈。
 *    只有 ASan 會把它抓出來。
 *
 *    ⇒ 這一題請務必兩個都跑：
 *         make N=07        <- 可能綠
 *         make asan N=07   <- 這個才是裁判
 *
 *    day-31 學到「綠燈不是證據」，這裡是它的第三種形狀：
 *    測試有覆蓋到那條路徑、UB 也真的發生了，但 assert 這個工具看不見記憶體越界。
 *    **不同的 bug 要用不同的裁判抓。gcc、assert、ASan、checkpatch 抓的是四種東西。**
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>

static int chomp(char *line)
{
	(void)line;	/* TODO: delete this and write your code */
	return 0;
}

int main(void)
{
	char with_nl[] = "hello\n";
	char without_nl[] = "hello";
	char empty[] = "";		/* 🔥 the trap */
	char only_nl[] = "\n";
	char inner_nl[] = "a\nb";	/* '\n' in the middle, not at the end */

	assert(chomp(with_nl) == 1);
	assert(strcmp(with_nl, "hello") == 0);

	assert(chomp(without_nl) == 0);
	assert(strcmp(without_nl, "hello") == 0);

	assert(chomp(empty) == 0);	/* must not read empty[-1] */
	assert(strcmp(empty, "") == 0);

	assert(chomp(only_nl) == 1);
	assert(strcmp(only_nl, "") == 0);

	assert(chomp(inner_nl) == 0);	/* only the TRAILING newline counts */
	assert(strcmp(inner_nl, "a\nb") == 0);

	/* chomp is idempotent: running it twice changes nothing the second time */
	assert(chomp(with_nl) == 0);
	assert(strcmp(with_nl, "hello") == 0);

	printf("kata 07: PASS\n");
	printf("  ^ 綠了不算過。跑 `make asan N=07` —— 那個才是這題的裁判。\n");
	return 0;
}
