// SPDX-License-Identifier: GPL-2.0
/* Problem #27 — ARRAY_SIZE 參考解 */

#include <stdio.h>
#include <assert.h>

#define BUILD_BUG_ON_ZERO(e) ((int)(sizeof(struct { int:(-!!(e)); })))

#define __must_be_array(a) \
	BUILD_BUG_ON_ZERO(__builtin_types_compatible_p(__typeof__(a), __typeof__(&(a)[0])))

#define ARRAY_SIZE(arr) \
	(sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

int main(void)
{
	int arr[10];
	int matrix[3][4];

	assert(ARRAY_SIZE(arr) == 10);
	assert(ARRAY_SIZE(matrix) == 3);
	assert(ARRAY_SIZE(matrix[0]) == 4);

	const char *strs[] = { "a", "b", "c" };
	assert(ARRAY_SIZE(strs) == 3);

	printf("✅ Problem #27 array_size_macro passed\n");
	return 0;
}
