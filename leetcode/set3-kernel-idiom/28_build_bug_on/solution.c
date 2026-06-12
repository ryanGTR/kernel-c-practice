// SPDX-License-Identifier: GPL-2.0
/* Problem #28 — BUILD_BUG_ON 參考解 */

#include <stdio.h>
#include <assert.h>

#define BUILD_BUG_ON_ZERO(e) ((int)(sizeof(struct { int:(-!!(e)); })))
#define BUILD_BUG_ON(cond) ((void)BUILD_BUG_ON_ZERO(cond))

/* C11 替代：_Static_assert(cond, "msg"); */

struct small_struct { int x; };

int main(void)
{
	BUILD_BUG_ON(0);
	BUILD_BUG_ON(sizeof(int) > 8);
	BUILD_BUG_ON(sizeof(struct small_struct) > 64);

	/* C11 寫法 */
	_Static_assert(sizeof(int) >= 2, "int must be at least 2 bytes");

	printf("✅ Problem #28 build_bug_on passed\n");
	return 0;
}
