// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 04  str_tolower    ctype 的 (unsigned char) 陷阱
 * @fn: str_tolower
 *
 * 規格：把 s 原地轉成小寫。用 <ctype.h> 的 tolower()。
 *       （這是 my_grep 的 -i 會用到的零件）
 *
 * 動手前先答：
 *   Q1. tolower() 的參數型別是 int，你傳一個 char 進去，發生了什麼轉換？
 *       A:
 *   Q2. char 在 x86 Linux 上是 signed 還是 unsigned？所以 byte 0xC3 當 char 是多少？
 *       A:
 *   Q3. 為什麼一定要寫 tolower((unsigned char)*p) 而不是 tolower(*p)？
 *       A:
 *
 * 🔥 這題最重要的一課，跟「會不會寫」無關：
 *
 *    下面的測試**抓不到**少寫 cast 的 UB。
 *    你不寫 cast，測試照樣全綠——因為 glibc 的 ctype 表格剛好在負索引方向
 *    也放了資料（為了讓 EOF = -1 能用）。它放你一馬。
 *
 *    這正是 day-31 那條「綠燈不是證據」的另一種形狀：
 *    day-31 是「測試沒覆蓋到那條路徑」，這裡是「UB 在你的機器上剛好沒炸」。
 *    換一個 libc、換一個平台（ARM 的 char 預設是 unsigned）、換一個編譯器版本，
 *    它就炸給你看。而你的測試從頭到尾都是綠的。
 *
 *    ⇒ 有些正確性，測試永遠證明不了。那部分只能靠你知道規則。
 */
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>	/* strcmp, for the test only */

static void str_tolower(char *s)
{
	(void)s;	/* TODO: delete this and write your code */
}

int main(void)
{
	char mixed[] = "KerNeL PaNiC";
	char digits[] = "abc123XYZ";
	char nonascii[] = { (char)0xC3, 'A', '\0' };	/* high byte: negative as char */

	str_tolower(mixed);
	assert(strcmp(mixed, "kernel panic") == 0);

	str_tolower(digits);
	assert(strcmp(digits, "abc123xyz") == 0);

	str_tolower(nonascii);
	assert((unsigned char)nonascii[0] == 0xC3);	/* must survive untouched */
	assert(nonascii[1] == 'a');

	printf("kata 04: PASS\n");
	printf("  ^ 注意：這個綠燈不代表你寫對了。看檔頭 🔥 那段。\n");
	return 0;
}
