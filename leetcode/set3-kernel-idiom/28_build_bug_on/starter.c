// SPDX-License-Identifier: GPL-2.0
/* Problem #28 — BUILD_BUG_ON */

#include <stdio.h>
#include <assert.h>

/* TODO: 定義 BUILD_BUG_ON */
#define BUILD_BUG_ON(cond)   /* TODO */

struct small_struct { int x; };

int main(void)
{
	BUILD_BUG_ON(0);
	BUILD_BUG_ON(sizeof(int) > 8);   /* int 永遠 <= 8 bytes，過 */
	BUILD_BUG_ON(sizeof(struct small_struct) > 64);

	/* 進階：取消註解編譯應該失敗 */
#if 0
	BUILD_BUG_ON(1);
	BUILD_BUG_ON(sizeof(int) == 4 && 1);
#endif

	printf("✅ Problem #28 build_bug_on passed\n");
	return 0;
}
