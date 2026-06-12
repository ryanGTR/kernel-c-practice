// SPDX-License-Identifier: GPL-2.0
/* Problem #29 — kfifo */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct kfifo {
	unsigned char *buf;
	unsigned int size;
	unsigned int mask;
	unsigned int in;
	unsigned int out;
};

static int          kfifo_init(struct kfifo *f, unsigned int size)
	{ (void)f; (void)size; return -1; }
static unsigned int kfifo_in(struct kfifo *f, const void *buf, unsigned int n)
	{ (void)f; (void)buf; (void)n; return 0; }
static unsigned int kfifo_out(struct kfifo *f, void *buf, unsigned int n)
	{ (void)f; (void)buf; (void)n; return 0; }
static unsigned int kfifo_len(struct kfifo *f)
	{ (void)f; return 0; }
static void         kfifo_free(struct kfifo *f)
	{ (void)f; }

int main(void)
{
	struct kfifo f;
	assert(kfifo_init(&f, 16) == 0);
	assert(kfifo_len(&f) == 0);

	unsigned char data[] = "hello";
	assert(kfifo_in(&f, data, 5) == 5);
	assert(kfifo_len(&f) == 5);

	unsigned char buf[16];
	assert(kfifo_out(&f, buf, 5) == 5);
	assert(memcmp(buf, "hello", 5) == 0);
	assert(kfifo_len(&f) == 0);

	/* 滿了測試 */
	for (int i = 0; i < 16; i++) {
		unsigned char x = (unsigned char)i;
		assert(kfifo_in(&f, &x, 1) == 1);
	}
	unsigned char x = 99;
	assert(kfifo_in(&f, &x, 1) == 0);   /* 滿，寫不進 */
	assert(kfifo_len(&f) == 16);

	/* 多輪 in/out 測試 wrap-around */
	for (int i = 0; i < 100; i++) {
		unsigned char tmp;
		assert(kfifo_out(&f, &tmp, 1) == 1);
		unsigned char nx = (unsigned char)(i + 100);
		assert(kfifo_in(&f, &nx, 1) == 1);
	}

	kfifo_free(&f);
	printf("✅ Problem #29 kfifo passed\n");
	return 0;
}
