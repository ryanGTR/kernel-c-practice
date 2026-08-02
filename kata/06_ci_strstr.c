// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 06  ci_strstr      不分大小寫搜尋（my_grep -i 的真正零件）
 * @fn: ci_strstr
 *
 * 規格：跟 kata 05 一樣，但比對時忽略大小寫。
 *       **零複製**：不准 malloc 一份小寫副本。邊走邊 tolower 比。
 *
 * 動手前先答：
 *   Q1. 另一種做法是「複製一份小寫的 hay，再用一般 strstr 找」。
 *       這兩種做法的記憶體成本差在哪？誰要負責 free？
 *       A:
 *   Q2. 🔥 假設你真的做了小寫副本並在副本上找到了。
 *       你回傳的指標指向**副本**還是**原本的 hay**？grep 要印的是哪一行？
 *       A:
 *   Q3. （承上）所以「零複製」除了省記憶體，還解決了什麼問題？
 *       A:
 *
 * 💡 Q2 是這題的靈魂。my_grep 找到後要印的是**原始那一行**（保留大小寫），
 *    不是你為了比對而做的小寫副本。回錯指標 → 印出來的東西整行變小寫。
 *    這種 bug 編譯器一個字都不會說。
 *
 * 別忘了 kata 04 的 (unsigned char) cast。
 */
#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

static const char *ci_strstr(const char *hay, const char *needle)
{
	(void)hay;
	(void)needle;	/* TODO: delete these and write your code */
	return NULL;
}

int main(void)
{
	const char *hay = "Kernel PANIC in my_cat";
	const char *p;

	/* exact case still works */
	p = ci_strstr(hay, "Kernel");
	assert(p == hay);

	/* needle lowercase, hay mixed */
	p = ci_strstr(hay, "kernel");
	assert(p == hay);

	/* needle uppercase, hay lowercase */
	p = ci_strstr(hay, "MY_CAT");
	assert(p == hay + 16);

	/* both mixed */
	p = ci_strstr(hay, "PaNiC");
	assert(p == hay + 7);

	/* 🔥 the returned pointer must point INTO hay -- the original, not a copy */
	p = ci_strstr(hay, "panic");
	assert(p >= hay && p < hay + 22);
	assert(*p == 'P');		/* the ORIGINAL uppercase P, not 'p' */

	/* no hit */
	assert(ci_strstr(hay, "zzz") == NULL);

	/* empty needle */
	assert(ci_strstr(hay, "") == hay);

	/* partial match then fail (kata 05's trap, now case-insensitive) */
	p = ci_strstr("aAaB", "AaB");
	assert(p != NULL);
	assert(*p == 'A');		/* hay[1], the original uppercase A */

	printf("kata 06: PASS\n");
	return 0;
}
