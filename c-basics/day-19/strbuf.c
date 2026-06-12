// SPDX-License-Identifier: GPL-2.0
/*
 * Day 19 Step 4 ★ 主菜 — 動態字串 strbuf
 *
 * 目標：
 *   - 寫一個會自動擴容的字串緩衝
 *   - init / append / free 三件式對稱介面
 *   - realloc 失敗保護（先存 tmp）
 *   - valgrind 必須 0 leaks 0 errors
 *
 * TODO（照 wiki/daily/2026-04/day-19.md 步驟 4）：
 *   1. 實作 strbuf_init(sb, cap)
 *   2. 實作 strbuf_append(sb, s)
 *      ★ 關鍵：cap 不夠時 realloc，**先存 tmp 再賦值**
 *   3. 實作 strbuf_free(sb)
 *   4. main 已經串好三段字串，你只需把三個函式填完
 *
 * 編譯：gcc -Wall -Wextra -g strbuf.c -o strbuf
 * 跑：valgrind --leak-check=full --show-leak-kinds=all ./strbuf
 *
 * 預期輸出：
 *   result: Hello, world! From day 19.
 *   len=27 cap=??（看你擴容策略，常見 30~64）
 * valgrind 預期：0 leaks 0 errors（**主菜通關線**）
 *
 * 三個關鍵習慣：
 *   [ ] init 配資源、free 釋放，名字對稱
 *   [ ] realloc 失敗時不能覆蓋 sb->data，所以先 tmp
 *   [ ] free 完置 NULL，把 ownership 交回給「沒人」
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct strbuf {
	char *data;
	size_t len;
	size_t cap;
};

static int strbuf_init(struct strbuf *sb, size_t cap)
{
	sb->data = malloc(cap);
	if (!sb->data)
		return -1;
	sb->data[0] = '\0';
	sb->len = 0;
	sb->cap = cap;
	return 0;
}

static int strbuf_append(struct strbuf *sb, const char *s)
{
	size_t slen = strlen(s);
	if (sb->len + slen + 1 > sb->cap) {
		size_t new_cap = (sb->len + slen + 1) * 2;
		char *tmp = realloc(sb->data, new_cap);
		if (!tmp)
			return -1;
		sb->data = tmp;
		sb->cap = new_cap;
	}

	memcpy(sb->data + sb->len, s, slen + 1);
	sb->len += slen;
	return 0;
}

static void strbuf_free(struct strbuf *sb)
{
	free(sb->data);
	sb->data = NULL;
	sb->len = 0;
	sb->cap = 0;
}

int main(void)
{
	struct strbuf sb;

	if (strbuf_init(&sb, 8) < 0)
		return 1;

	if (strbuf_append(&sb, "Hello, ") < 0)
		goto err;
	if (strbuf_append(&sb, "world") < 0)
		goto err;
	if (strbuf_append(&sb, "! From day 19.") < 0)
		goto err;

	printf("result: %s\n", sb.data);
	printf("len=%zu cap=%zu\n", sb.len, sb.cap);

	strbuf_free(&sb);
	return 0;

err:
	strbuf_free(&sb);
	return 1;
}
