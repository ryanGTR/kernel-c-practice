// SPDX-License-Identifier: GPL-2.0
/* Problem #27 — ARRAY_SIZE macro */

#include <stdio.h>
#include <assert.h>

/* TODO: 定義 BUILD_BUG_ON_ZERO + __must_be_array + ARRAY_SIZE */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))   /* TODO: 加 __must_be_array */

int main(void)
{
	int arr[10];
	int matrix[3][4];

	assert(ARRAY_SIZE(arr) == 10);
	assert(ARRAY_SIZE(matrix) == 3);
	assert(ARRAY_SIZE(matrix[0]) == 4);

	const char *strs[] = { "a", "b", "c" };
	assert(ARRAY_SIZE(strs) == 3);

	/* 進階：取消下面註解，編譯應該失敗（pointer 不是陣列）*/
#if 0
	int *p = arr;
	size_t bad = ARRAY_SIZE(p);   /* ❌ should not compile */
	(void)bad;
#endif

	printf("✅ Problem #27 array_size_macro passed\n");
	return 0;
}
