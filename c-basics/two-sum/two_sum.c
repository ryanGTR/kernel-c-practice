#include "two_sum.h"

int two_sum(const int *nums, int n, int target, int *i, int *j)
{
	/* TODO: two pointers, O(n) time, O(1) space */
	int r = n - 1;
	int l = 0;
	long sum = nums[l] + (long)nums[r];
	while (l < r) {
		if (target == sum) {
			*i = l;
			*j = r;
			return 1;
		}
		if (target > sum) {
			l++;
		} else {
			r--;
		}
	}
	return 0;
}
