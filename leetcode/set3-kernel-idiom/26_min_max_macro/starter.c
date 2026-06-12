// SPDX-License-Identifier: GPL-2.0
/* Problem #26 — type-safe min/max macro */

#include <stdio.h>
#include <assert.h>

/* TODO: 用 __typeof__ + ({}) 寫 type-safe min / max */
#define min(a, b) ((a) < (b) ? (a) : (b))   /* TODO: 換成 type-safe 版 */
#define max(a, b) ((a) > (b) ? (a) : (b))   /* TODO */

int main(void)
{
	assert(min(3, 5) == 3);
	assert(max(3, 5) == 5);
	assert(min(-1, 1) == -1);

	/* side-effect 安全測試：不該重複 evaluate */
	int i = 1, j = 2;
	int m = min(i++, j++);
	assert(m == 1);
	assert(i == 2);   /* 只 ++ 一次 */
	assert(j == 3);

	int x = 10, y = 20;
	int n = max(x++, y++);
	assert(n == 20);
	assert(x == 11 && y == 21);

	printf("✅ Problem #26 min_max_macro passed\n");
	return 0;
}
