// SPDX-License-Identifier: GPL-2.0
/*
 * Day 20 / Step 1 — strcpy 的契約裡缺了 Fail 欄。
 *
 * 契約四欄：
 *   Pre  : dst 至少 strlen(src)+1 bytes 可寫；src 是合法 NUL-terminated 字串
 *   Post : dst 是 src 的副本（含 \0）
 *   Fail : ❌ 沒寫 → Pre 不滿足 = UB（buffer overflow）
 *   Owns : caller 擁有 dst 與 src
 *
 * 任務：寫一段必然越界的 strcpy，編譯後跑 valgrind 看 "Invalid write"。
 */

#include <stdio.h>
#include <string.h>

int main(void)
{
	char dst[8];
	const char *src = "this is way too long for dst";

	/* TODO: 用 strcpy 把 src 灌進 dst — 觀察 stack 被踩到的後果 */
	strcpy(dst, src);
	printf("dst = %s\n", dst);
	return 0;
}
