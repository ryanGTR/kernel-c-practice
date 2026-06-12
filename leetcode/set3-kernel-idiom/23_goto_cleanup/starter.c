// SPDX-License-Identifier: GPL-2.0
/* Problem #23 — goto cleanup 階梯 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct ctx {
	char *buf1;
	char *buf2;
	int   fd;
	char *buf3;
	int   registered;
};

/* 模擬 fd */
static int  open_fake(void)  { return 42; }
static void close_fake(int fd) { (void)fd; }

static int do_setup(struct ctx *ctx, int fail_at)
{
	(void)ctx;
	(void)fail_at;
	return -1;   /* TODO */
}

static void do_teardown(struct ctx *ctx)
{
	(void)ctx;   /* TODO */
}

int main(void)
{
	struct ctx c;

	/* 全部成功 */
	memset(&c, 0, sizeof(c));
	assert(do_setup(&c, 0) == 0);
	assert(c.registered == 1);
	do_teardown(&c);

	/* 各階段失敗：每次都應該 valgrind 乾淨 */
	for (int i = 1; i <= 5; i++) {
		memset(&c, 0, sizeof(c));
		assert(do_setup(&c, i) == -1);
	}

	printf("✅ Problem #23 goto_cleanup passed\n");
	return 0;
}
