// SPDX-License-Identifier: GPL-2.0
/*
 * Day 20 / Step 4 ★ 主菜 — 自己實作 my_strscpy。
 *
 * 契約四欄（這是 kernel lib/string.c 的 strscpy 抽出來的精華）：
 *   Pre  : dst 至少 count bytes 可寫；src 是合法字串；count > 0 才有意義
 *   Post : dst 一定以 \0 結尾；
 *          內容 = src 前 min(strlen(src), count-1) bytes；
 *          不浪費填 0
 *   Fail : 截斷或 count == 0 回 -E2BIG（明確錯誤碼，不是 UB、不是靜默）
 *   Owns : caller 擁有 dst 與 src
 *
 * 任務：寫 my_strscpy 並讓每一行對到契約四欄的哪一條。
 *
 * 預期輸出：
 *   [1] 'short' (ret=5)
 *   [2] 'way too' (ret=-7)      ← -E2BIG，截斷
 *   [3] '' (ret=0)
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

static ssize_t my_strscpy(char *dest, const char *src, size_t count)
{
	size_t i = 0;
	/* TODO Fail 欄: count == 0 → return -E2BIG */
	if (count == 0) {
		return -E2BIG;
	}
	/* TODO Post 欄: for 迴圈複製 min(strlen(src), count-1) bytes
	 *   注意 count - 1，留一格給 \0 */
	for (i = 0; i < count - 1 && src[i]; i++) {
		dest[i] = src[i];
	}

	/* TODO Post 欄: dest[i] = '\0';  迴圈無論怎麼跳出都要走到這 */

	dest[i] = '\0';
	if (src[i] != '\0')
		return -E2BIG;
	/* TODO Fail 欄: 若 src[i] != '\0' → 截斷發生，return -E2BIG */

	/* TODO 成功值: return (ssize_t)i; */
	return (size_t)i;
}

int main(void)
{
	char buf[8];
	ssize_t r;

	r = my_strscpy(buf, "short", sizeof(buf));
	printf("[1] '%s' (ret=%zd)\n", buf, r);

	r = my_strscpy(buf, "way too long input", sizeof(buf));
	printf("[2] '%s' (ret=%zd)\n", buf, r);

	r = my_strscpy(buf, "", sizeof(buf));
	printf("[3] '%s' (ret=%zd)\n", buf, r);

	return 0;
}
