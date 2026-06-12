// SPDX-License-Identifier: GPL-2.0
/* Problem #26 — type-safe min/max macro 參考解 */

#include <stdio.h>
#include <assert.h>

#define min(a, b) ({                    \
	__typeof__(a) _a = (a);             \
	__typeof__(b) _b = (b);             \
	(void)(&_a == &_b);                 \
	_a < _b ? _a : _b;                  \
})

#define max(a, b) ({                    \
	__typeof__(a) _a = (a);             \
	__typeof__(b) _b = (b);             \
	(void)(&_a == &_b);                 \
	_a > _b ? _a : _b;                  \
})

int main(void)
{
	assert(min(3, 5) == 3);
	assert(max(3, 5) == 5);
	assert(min(-1, 1) == -1);

	int i = 1, j = 2;
	int m = min(i++, j++);
	assert(m == 1);
	assert(i == 2);
	assert(j == 3);

	int x = 10, y = 20;
	int n = max(x++, y++);
	assert(n == 20);
	assert(x == 11 && y == 21);

	printf("✅ Problem #26 min_max_macro passed\n");
	return 0;
}
