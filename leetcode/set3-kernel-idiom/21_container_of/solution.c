// SPDX-License-Identifier: GPL-2.0
/* Problem #21 — container_of 參考解 */

#include <stdio.h>
#include <stddef.h>
#include <assert.h>

#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - offsetof(type, member)))

struct foo {
	int x;
	char buf[16];
	int y;
	double z;
};

int main(void)
{
	struct foo f = { .x = 1, .y = 99, .z = 3.14 };

	int *py = &f.y;
	struct foo *p = container_of(py, struct foo, y);
	assert(p == &f);
	assert(p->x == 1);

	int *px = &f.x;
	struct foo *p2 = container_of(px, struct foo, x);
	assert(p2 == &f);

	double *pz = &f.z;
	struct foo *p3 = container_of(pz, struct foo, z);
	assert(p3 == &f);

	printf("✅ Problem #21 container_of passed\n");
	return 0;
}
