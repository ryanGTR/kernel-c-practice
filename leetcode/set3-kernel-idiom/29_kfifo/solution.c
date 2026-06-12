// SPDX-License-Identifier: GPL-2.0
/* Problem #29 — kfifo 參考解 */

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

/* 檢查 size 是 2 的次方 */
static int is_power_of_2(unsigned int n)
{
	return n != 0 && (n & (n - 1)) == 0;
}

static int kfifo_init(struct kfifo *f, unsigned int size)
{
	if (!is_power_of_2(size)) return -1;
	f->buf = malloc(size);
	if (!f->buf) return -1;
	f->size = size;
	f->mask = size - 1;
	f->in = f->out = 0;
	return 0;
}

static unsigned int kfifo_len(struct kfifo *f)
{
	return f->in - f->out;
}

static unsigned int kfifo_in(struct kfifo *f, const void *src, unsigned int n)
{
	unsigned int avail = f->size - kfifo_len(f);
	if (n > avail) n = avail;

	const unsigned char *s = src;
	for (unsigned int i = 0; i < n; i++) {
		f->buf[f->in & f->mask] = s[i];
		f->in++;
	}
	return n;
}

static unsigned int kfifo_out(struct kfifo *f, void *dst, unsigned int n)
{
	unsigned int avail = kfifo_len(f);
	if (n > avail) n = avail;

	unsigned char *d = dst;
	for (unsigned int i = 0; i < n; i++) {
		d[i] = f->buf[f->out & f->mask];
		f->out++;
	}
	return n;
}

static void kfifo_free(struct kfifo *f)
{
	free(f->buf);
	memset(f, 0, sizeof(*f));
}

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

	for (int i = 0; i < 16; i++) {
		unsigned char x = (unsigned char)i;
		assert(kfifo_in(&f, &x, 1) == 1);
	}
	unsigned char x = 99;
	assert(kfifo_in(&f, &x, 1) == 0);
	assert(kfifo_len(&f) == 16);

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
