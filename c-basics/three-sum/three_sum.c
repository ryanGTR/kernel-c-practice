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
		for (int y = x + 1; y < n; y++) {
			if (y > x + 1 && nums[y] == nums[y - 1])
				continue;
			for (int z = y + 1; z < n; z++) {
				if (z > y + 1 && nums[z] == nums[z - 1])
					continue;

				if ((long)nums[x] + nums[y] + nums[z] == 0) {
					if (cnt == max_out) {
						return -1;
					} else {
						out[cnt][0] = nums[x];
						out[cnt][1] = nums[y];
						out[cnt][2] = nums[z];
						cnt++;
					}
				}
			}
		}
	}
	return cnt;
}
