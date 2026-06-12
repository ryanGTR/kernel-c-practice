// SPDX-License-Identifier: GPL-2.0
/*
 * Day 20 / Step 2 — strncpy 的兩個 Post 條款都是陷阱。
 *
 * 契約四欄：
 *   Pre  : dst 至少 n bytes 可寫；src 至少 n bytes 可讀（不必有 \0）
 *   Post : ① strlen(src) >= n → 複製 n bytes，**不補 \0**
 *          ② strlen(src) <  n → 複製字串 + 用 \0 **填到 n**
 *   Fail : Pre 滿足時不會 fail，但 Post 兩條本身就是地雷
 *   Owns : 同 strcpy
 *
 * 任務：示範條款 ① 的後果 — 沒 \0 的 dst 餵給 printf("%s") 會衝過去。
 */

#include <stdio.h>
#include <string.h>

int main(void)
{
	char dst[8];

	/* 故意先用 'A' 把 dst 塞滿 — 後面看哪些 byte 真的被改寫 */
	memset(dst, 'A', sizeof(dst));

	strncpy(dst, "this is way too long", sizeof(dst) - 1);
	dst[sizeof(dst) - 1] = '\0';
	/* ⚠️ dst 不保證有 \0，下面這行可能讀過界 */
	printf("dst = %s\n", dst);

	/* TODO: 寫一份「修正版」— strncpy 後手動補 \0
	 *   strncpy(dst, src, sizeof(dst) - 1);
	 *   dst[sizeof(dst) - 1] = '\0';
	 * 印出來比較。 */

	return 0;
}
