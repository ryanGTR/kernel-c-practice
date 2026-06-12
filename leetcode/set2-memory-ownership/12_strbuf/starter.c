// SPDX-License-Identifier: GPL-2.0
/*
 * Problem #12 — strbuf (Day 19 主菜)
 *
 * 規格：見 PROBLEM.md
 *
 * 你的任務：填三個 function。寫完跑 `make check`。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct strbuf {
	char *data;
	size_t len;
	size_t cap;
};

/* ============================================================
 * TODO 1: strbuf_init
 *   - malloc cap bytes
 *   - data[0] = '\0'  ← 必須，否則 strlen 看垃圾
 *   - len = 0, cap = cap
 *   - return 0 / -1
 * ============================================================ */
static int strbuf_init(struct strbuf *sb, size_t cap)
{
	(void)sb;
	(void)cap;
	return -1;   /* TODO: 替換 */
}

/* ============================================================
 * TODO 2: strbuf_append
 *   - 算 slen = strlen(s)
 *   - 如果 sb->len + slen + 1 > sb->cap：
 *       new_cap = (sb->cap + slen + 1) * 2
 *       ★ 用 tmp 中介 realloc（不要直接覆蓋 sb->data）
 *       失敗 return -1（sb->data 還有效）
 *       成功更新 sb->data 跟 sb->cap
 *   - memcpy(sb->data + sb->len, s, slen + 1)   ← +1 搬 '\0'
 *   - sb->len += slen   ← 不要 += slen + 1
 *   - return 0
 * ============================================================ */
static int strbuf_append(struct strbuf *sb, const char *s)
{
	(void)sb;
	(void)s;
	return -1;   /* TODO: 替換 */
}

/* ============================================================
 * TODO 3: strbuf_free
 *   - free(sb->data)
 *   - sb->data = NULL  ← 必須，讓重複呼叫安全
 *   - sb->len = sb->cap = 0
 * ============================================================ */
static void strbuf_free(struct strbuf *sb)
{
	(void)sb;
}

/* ============================================================
 * 測試 main（不要動）
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

	/* 進階測試：多次擴容 */
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

	/* 冪等測試 */
	strbuf_free(&sb);
	strbuf_free(&sb);   /* 第二次必須無害 */

	return 0;

err:
	strbuf_free(&sb);
	return 1;
}
