#ifndef TWO_SUM_H
#define TWO_SUM_H

/*
 * nums is sorted in non-decreasing order, length n.
 * Find i < j with nums[i] + nums[j] == target.
 * On success, store indices (0-based) in *i, *j and return 1.
 * If no such pair exists, return 0 and leave *i, *j untouched.
 */
int two_sum(const int *nums, int n, int target, int *i, int *j);

#endif
