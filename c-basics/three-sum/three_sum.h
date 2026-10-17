#ifndef THREE_SUM_H
#define THREE_SUM_H

/*
 * Find every unique triplet a <= b <= c in nums with a + b + c == 0.
 *
 * nums is NOT sorted on entry; you may sort it in place (qsort).
 * Write each triplet into out[k][0..2] in ascending order, and list the
 * triplets in ascending (lexicographic) order.
 *
 * out has room for max_out triplets. Never write past it.
 * Return the number of triplets written, or -1 if there are more than
 * max_out unique triplets.
 */
int three_sum(int *nums, int n, int (*out)[3], int max_out);

#endif
