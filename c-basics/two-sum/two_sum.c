#include "two_sum.h"

int two_sum(const int *nums, int n, int target, int *i, int *j)
{
	long temp;
	/* TODO: two pointers, O(n) time, O(1) space */
	for (int x = 0; x < n; x++) {
		for (int y = x + 1; y < n; y++) {
			temp = (long)nums[x] + nums[y];
			if (target == temp) {
				*i = x;
				*j = y;
				return 1;
			}
		}
	}
	return 0;
}
