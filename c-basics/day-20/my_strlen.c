// SPDX-License-Identifier: GPL-2.0
/*
 * Day 20 / Step 4 加碼 — 自己實作 my_strlen。
 *
 * 契約四欄：
 *   Pre  : s 指向合法 NUL-terminated 字串
 *   Post : 回傳值 = s 從開頭到第一個 \0 之間的 byte 數（不含 \0）
 *   Fail : Pre 不滿足 = UB（沒辦法檢查長度，只能信 caller）
 *   Owns : caller 擁有 s
 *
 * 任務：從 0 開始走，遇到 \0 停。**不准呼叫 strlen**。
 */

#include <stdio.h>
#include <string.h>

static size_t my_strlen(const char *s)
{
	size_t count = 0;
	/* TODO: 走過去數，遇到 *p == '\0' 停 */
	while (*s != '\0') {
		count++;
		s++;
	}
	return count;
}

int main(void)
{
	const char *cases[] = {
		"",
		"a",
		"hello",
		"day-20 test",
	};

	for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
		size_t mine = my_strlen(cases[i]);
		size_t lib = strlen(cases[i]);
		printf("[%zu] '%s' my=%zu lib=%zu %s\n", i, cases[i], mine, lib,
		       mine == lib ? "OK" : "FAIL");
	}

	return 0;
}
