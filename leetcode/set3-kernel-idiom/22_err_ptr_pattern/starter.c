// SPDX-License-Identifier: GPL-2.0
/* Problem #22 — ERR_PTR / IS_ERR / PTR_ERR */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define MAX_ERRNO 4095

static inline void *ERR_PTR(long error)        { (void)error; return NULL; }   /* TODO */
static inline long  PTR_ERR(const void *ptr)   { (void)ptr; return 0; }        /* TODO */
static inline int   IS_ERR(const void *ptr)    { (void)ptr; return 0; }        /* TODO */

/* 模擬 kernel 函式 */
static struct foo {
	int x;
} *open_foo(int n)
{
	if (n < 0)
		return ERR_PTR(-EINVAL);
	struct foo *f = malloc(sizeof(*f));
	if (!f)
		return ERR_PTR(-ENOMEM);
	f->x = n;
	return f;
}

int main(void)
{
	struct foo *f = open_foo(-1);
	assert(IS_ERR(f));
	assert(PTR_ERR(f) == -EINVAL);

	struct foo *g = open_foo(42);
	assert(!IS_ERR(g));
	assert(g->x == 42);
	free(g);

	/* 額外測試 */
	void *p = ERR_PTR(-ENOMEM);
	assert(IS_ERR(p));
	assert(PTR_ERR(p) == -ENOMEM);

	/* 合法 pointer 不能被誤判為錯誤 */
	int x = 0;
	assert(!IS_ERR(&x));

	/* NULL 也不該被誤判 */
	assert(!IS_ERR(NULL));

	printf("✅ Problem #22 err_ptr_pattern passed\n");
	return 0;
}
