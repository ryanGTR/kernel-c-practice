#ifndef TWO_SUM_HASH_H
#define TWO_SUM_HASH_H

/*
 * nums is NOT sorted, length n (nums may be NULL when n == 0).
 * Find i < j with nums[i] + nums[j] == target, using a hash table.
 *
 * If several pairs exist, pick the one with the smallest j; for that j,
 * pick the smallest i. (Scan j left to right, look up target - nums[j]
 * among nums[0..j-1].)
 *
 * On success, store the original indices in *i, *j and return 1.
 * If no pair exists, return 0 and leave *i, *j untouched.
 * Return -1 if memory allocation fails.
 */
int two_sum_hash(const int *nums, int n, int target, int *i, int *j);

#endif
