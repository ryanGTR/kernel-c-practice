#include <stdlib.h>
#include "three_sum.h"

static int cmp(const void *a, const void *b)
{
	int x = *(const int *)a, y = *(const int *)b;
	return (x > y) - (x < y);
}

int three_sum(int *nums, int n, int (*out)[3], int max_out)
{
	if (!nums)
		return 0;

	qsort(nums, n, sizeof(int), cmp);
	int cnt = 0;
	/* TODO: sort, fix nums[i], two pointer on the rest, skip duplicates */
	for (int x = 0; x < n; x++) {
		if (x > 0 && nums[x] == nums[x - 1])
			continue;

		int l = x + 1;
		int r = n - 1;

		while (l < r) {
			long sum = (long)nums[x] + nums[l] + nums[r];

			if (sum < 0) {
				l++;
			} else if (sum > 0) {
				r--;
			} else {
				if (cnt == max_out)
					return -1;
				out[cnt][0] = nums[x];
				out[cnt][1] = nums[l];
				out[cnt][2] = nums[r];
				cnt++;

				while (l < r && nums[l] == nums[l + 1])
					l++;
				while (l < r && nums[r] == nums[r - 1])
					r--;

				l++;
				r--;
			}
		}
	}
	return cnt;
}
