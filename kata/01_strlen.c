// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 01  my_strlen      指標走到 '\0' 為止
 * @fn: my_strlen
 *
 * 規格：回傳字串長度（不含結尾 '\0'）。不准 #include <string.h>。
 *
 * 動手前先答（答完再寫 code）：
 *   Q1. 這個函式有配置記憶體嗎？誰負責 free？
 *       A:
 *   Q2. 參數為什麼是 const char *？把 const 拿掉會怎樣？
 *       A:
 *
 * 記憶體圖（畫完再寫）：
 *   my_strlen frame:
 *     s   const char *   <- 指向誰的記憶體？我開的嗎？
 *     ?
 */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static size_t my_strlen(const char *s)
{
	size_t ch = 0;

	while (*s++)
		ch++;

	return ch;
}

int main(void)
{
	assert(my_strlen("") == 0);
	assert(my_strlen("a") == 1);
	assert(my_strlen("hello") == 5);
	assert(my_strlen("kernel panic") == 12);

	/* Embedded '\0' ends the string -- length is 3, not 7. */
	assert(my_strlen("abc\0def") == 3);

	printf("kata 01: PASS\n");
	return 0;
}
