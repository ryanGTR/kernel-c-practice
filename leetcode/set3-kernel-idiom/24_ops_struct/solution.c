// SPDX-License-Identifier: GPL-2.0
/* Problem #24 — ops struct 參考解 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct device;

struct device_ops {
	int  (*init)(struct device *d);
	int  (*read)(struct device *d, char *buf, size_t n);
	int  (*write)(struct device *d, const char *buf, size_t n);
	void (*close)(struct device *d);
};

struct device {
	const struct device_ops *ops;
	void *private_data;
};

static int device_init(struct device *d, const struct device_ops *ops)
{
	d->ops = ops;
	d->private_data = NULL;
	return ops->init(d);
}

static int device_read(struct device *d, char *buf, size_t n)
{
	return d->ops->read(d, buf, n);
}

static int device_write(struct device *d, const char *b, size_t n)
{
	return d->ops->write(d, b, n);
}

static void device_close(struct device *d)
{
	d->ops->close(d);
}

/* ---- 同 starter 的 null_ops / echo_ops ---- */
static int null_init(struct device *d)  { (void)d; return 0; }
static int null_read(struct device *d, char *buf, size_t n)
	{ (void)d; (void)buf; (void)n; return 0; }
static int null_write(struct device *d, const char *b, size_t n)
	{ (void)d; (void)b; return (int)n; }
static void null_close(struct device *d) { (void)d; }
static const struct device_ops null_ops = {
	.init = null_init, .read = null_read,
	.write = null_write, .close = null_close,
};

struct echo_state { char buf[256]; size_t len; };

static int echo_init(struct device *d) {
	d->private_data = calloc(1, sizeof(struct echo_state));
	return d->private_data ? 0 : -1;
}
static int echo_read(struct device *d, char *buf, size_t n) {
	struct echo_state *s = d->private_data;
	size_t cp = n < s->len ? n : s->len;
	memcpy(buf, s->buf, cp);
	memmove(s->buf, s->buf + cp, s->len - cp);
	s->len -= cp;
	return (int)cp;
}
static int echo_write(struct device *d, const char *b, size_t n) {
	struct echo_state *s = d->private_data;
	size_t space = sizeof(s->buf) - s->len;
	size_t cp = n < space ? n : space;
	memcpy(s->buf + s->len, b, cp);
	s->len += cp;
	return (int)cp;
}
static void echo_close(struct device *d) {
	free(d->private_data);
	d->private_data = NULL;
}
static const struct device_ops echo_ops = {
	.init = echo_init, .read = echo_read,
	.write = echo_write, .close = echo_close,
};

int main(void)
{
	struct device d1, d2;

	assert(device_init(&d1, &null_ops) == 0);
	assert(device_init(&d2, &echo_ops) == 0);

	assert(device_write(&d1, "hello", 5) == 5);
	char buf[16];
	assert(device_read(&d1, buf, 5) == 0);

	assert(device_write(&d2, "hello", 5) == 5);
	assert(device_read(&d2, buf, 5) == 5);
	buf[5] = '\0';
	assert(strcmp(buf, "hello") == 0);

	device_close(&d1);
	device_close(&d2);

	printf("✅ Problem #24 ops_struct passed\n");
	return 0;
}
