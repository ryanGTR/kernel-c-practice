// SPDX-License-Identifier: GPL-2.0
/*
 * Problem #12 — strbuf 參考解
 *
 * ★ 寫完 starter.c 再看這個 ★
 *
 * 設計重點：
 *  1. realloc 失敗保護：tmp 中介
 *  2. doubling 擴容策略
 *  3. memcpy 連 \0 一起搬（slen + 1）
 *  4. free 後置 NULL → strbuf_free 冪等
 *  5. init 設 data[0] = '\0' → 從一開始就是合法 C string
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
	if (cap == 0)
		cap = 16;                 /* 防呆：給合理預設 */

	sb->data = malloc(cap);
	if (!sb->data)
		return -1;

	sb->data[0] = '\0';               /* ★ 從一開始就合法 */
	sb->len = 0;
	sb->cap = cap;
	return 0;
}

static int strbuf_append(struct strbuf *sb, const char *s)
{
	size_t slen = strlen(s);

	/* 擴容判斷：要放 slen + 1（含 \0）*/
	if (sb->len + slen + 1 > sb->cap) {
		size_t new_cap = (sb->cap + slen + 1) * 2;

		/* ★ tmp 中介：realloc 失敗時 sb->data 還有效 */
		char *tmp = realloc(sb->data, new_cap);
		if (!tmp)
			return -1;

		sb->data = tmp;
		sb->cap = new_cap;
	}

	/* +1 把 '\0' 一起搬，sb->data 自動 \0 結尾 */
	memcpy(sb->data + sb->len, s, slen + 1);
	sb->len += slen;                  /* '\0' 不算長度 */
	return 0;
}

static void strbuf_free(struct strbuf *sb)
{
	free(sb->data);                   /* free(NULL) 合法 */
	sb->data = NULL;                  /* ★ 冪等的關鍵 */
	sb->len = sb->cap = 0;
}

/* ============================================================
 * 測試 main（同 starter.c）
 * ============================================================ */
int main(void)
{
	struct strbuf sb;

	if (strbuf_init(&sb, 8) < 0) {
		fprintf(stderr, "init failed\n");
		return 1;
	}

	if (strbuf_append(&sb, "Hello, ") < 0)  goto err;
	if (strbuf_append(&sb, "world") < 0)    goto err;
	if (strbuf_append(&sb, "! From day 19.") < 0) goto err;

	printf("result: %s\n", sb.data);
	printf("len=%zu cap=%zu\n", sb.len, sb.cap);

	struct strbuf big;
	if (strbuf_init(&big, 4) < 0) goto err;
	for (int i = 0; i < 100; i++) {
		if (strbuf_append(&big, "ab") < 0) {
			strbuf_free(&big);
			goto err;
		}
	}
	printf("big len=%zu cap=%zu (expect len=200)\n", big.len, big.cap);
	strbuf_free(&big);

	strbuf_free(&sb);
	strbuf_free(&sb);

	return 0;

err:
	strbuf_free(&sb);
	return 1;
}
