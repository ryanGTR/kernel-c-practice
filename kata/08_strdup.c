// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 08  my_strdup      第一次 malloc：所有權轉移
 * @fn: my_strdup
 *
 * 規格：回傳一份 s 的副本，記憶體用 malloc 配置。malloc 失敗回 NULL。
 *       呼叫者負責 free。不准用 strdup()。
 *
 * 動手前先答：
 *   Q1. 這塊記憶體誰開的？誰負責 free？函式簽名有把這件事講出來嗎？
 *       A:
 *   Q2. 🔥 malloc 要配多少 bytes？strlen(s) 還是 strlen(s) + 1？為什麼？
 *       A:
 *   Q3. malloc 失敗時回 NULL —— 呼叫者如果不檢查，下一行會發生什麼？
 *       A:
 *
 * 💡 Java 類比：
 *    Java 的 `new String(s)` 之後你就不管了，GC 會處理。
 *    C 的 my_strdup 回傳的那塊記憶體，**所有權跟著回傳值一起交給呼叫者**。
 *    這是 C 函式簽名裡看不見、但必須寫在文件（或你腦子）裡的契約。
 *    kernel 裡到處都是這種契約：kstrdup / kmalloc 回來的東西，誰 kfree？
 *
 * ⚠️ 兩個裁判都要跑：
 *      make N=08        <- assert：內容對不對
 *      make asan N=08   <- ASan/LSan：+1 忘了寫 → heap-buffer-overflow
 *                          忘了 free   → memory leak
 *    忘記 +1 的話，assert 很可能全過（那個 byte 剛好是 0），只有 ASan 會叫。
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *my_strdup(const char *s)
{
	(void)s;	/* TODO: delete this and write your code */
	return NULL;
}

int main(void)
{
	const char *orig = "kernel panic";
	char *copy;
	char *empty;

	copy = my_strdup(orig);
	assert(copy != NULL);
	assert(copy != orig);			/* a real copy, not the same pointer */
	assert(strcmp(copy, orig) == 0);	/* same content */
	assert(copy[12] == '\0');		/* the terminator made it across */

	/* Writing to the copy must not touch the original. */
	copy[0] = 'K';
	assert(orig[0] == 'k');

	free(copy);

	empty = my_strdup("");
	assert(empty != NULL);
	assert(empty[0] == '\0');
	free(empty);

	printf("kata 08: PASS\n");
	printf("  ^ 再跑 `make asan N=08`。忘記 +1 或忘記 free，只有它會叫。\n");
	return 0;
}
