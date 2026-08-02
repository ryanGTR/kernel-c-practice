// SPDX-License-Identifier: GPL-2.0
/*
 * @kata: 02  my_strcpy      寫進「別人的」buffer
 * @fn: my_strcpy
 *
 * 規格：把 src 複製到 dst（含結尾 '\0'），回傳 dst。不准 #include <string.h>。
 *
 * 動手前先答：
 *   Q1. dst 那塊記憶體是誰的？這個函式有辦法知道 dst 夠不夠大嗎？
 *       A:
 *   Q2. （承上）所以 strcpy 為什麼惡名昭彰？CVE 都長什麼樣？
 *       A:
 *   Q3. '\0' 要不要複製過去？不複製會怎樣？
 *       A:
 *
 * 記憶體圖：
 *   main frame:
 *     buf[?]   char[]   <- 誰開的？多大？誰負責回收？
 *   my_strcpy frame:
 *     ?
 *
 * 💡 這題的重點不是「會不會寫」，是**看懂所有權**：
 *    my_strcpy 對 dst 只有「寫入權」，沒有「所有權」。
 *    Java 沒有這個概念——那裡沒有人能寫爆你的物件。
 */
#include <assert.h>
#include <stdio.h>

static char *my_strcpy(char *dst, const char *src)
{
	// char *p = dst;
	// while ((*p++ = *src++))
	// 	;
	// return dst; /* TODO: your code here */

	char *p = dst;
	while (1) {
		char ch = *src;
		*p = ch;
		src++;
		p++;
		if (ch == '\0')
			break;
	}
	return dst;
}

int main(void)
{
	char buf[32];
	char *ret;

	ret = my_strcpy(buf, "hello");
	assert(buf[0] == 'h');
	assert(buf[4] == 'o');
	assert(buf[5] == '\0'); /* the terminator must be copied */
	assert(ret == buf); /* return value is dst itself */

	my_strcpy(buf, "");
	assert(buf[0] == '\0');

	/* Copying a shorter string leaves the tail alone -- only up to '\0'. */
	my_strcpy(buf, "abcdefgh");
	my_strcpy(buf, "xy");
	assert(buf[0] == 'x');
	assert(buf[1] == 'y');
	assert(buf[2] == '\0');
	assert(buf[3] == 'd'); /* leftover garbage from the old copy */

	printf("kata 02: PASS\n");
	return 0;
}
