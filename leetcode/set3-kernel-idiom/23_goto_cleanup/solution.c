// SPDX-License-Identifier: GPL-2.0
/* Problem #23 — goto cleanup 參考解 */

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

static int  open_fake(void)    { return 42; }
static void close_fake(int fd) { (void)fd; }

static int do_setup(struct ctx *ctx, int fail_at)
{
	memset(ctx, 0, sizeof(*ctx));

	ctx->buf1 = malloc(64);
	if (!ctx->buf1 || fail_at == 1) goto err_buf1;

	ctx->buf2 = malloc(64);
	if (!ctx->buf2 || fail_at == 2) goto err_buf2;

	ctx->fd = open_fake();
	if (ctx->fd < 0 || fail_at == 3) goto err_fd;

	ctx->buf3 = malloc(64);
	if (!ctx->buf3 || fail_at == 4) goto err_buf3;

	if (fail_at == 5) goto err_register;
	ctx->registered = 1;
	return 0;

err_register:
	free(ctx->buf3);
err_buf3:
	close_fake(ctx->fd);
err_fd:
	free(ctx->buf2);
err_buf2:
	free(ctx->buf1);
err_buf1:
	memset(ctx, 0, sizeof(*ctx));
	return -1;
}

static void do_teardown(struct ctx *ctx)
{
	if (ctx->registered) {
		free(ctx->buf3);
		close_fake(ctx->fd);
		free(ctx->buf2);
		free(ctx->buf1);
	}
	memset(ctx, 0, sizeof(*ctx));
}

int main(void)
{
	struct ctx c;

	memset(&c, 0, sizeof(c));
	assert(do_setup(&c, 0) == 0);
	assert(c.registered == 1);
	do_teardown(&c);

	for (int i = 1; i <= 5; i++) {
		memset(&c, 0, sizeof(c));
		assert(do_setup(&c, i) == -1);
	}

	printf("✅ Problem #23 goto_cleanup passed\n");
	return 0;
}
