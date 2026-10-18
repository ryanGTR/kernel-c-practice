#include <stdbool.h>
#include <stdlib.h>
#include "two_sum_hash.h"

/*
 * Open addressing + linear probing.
 * Java analogy: HashMap<Integer, Integer> (value -> index), but you
 * build the buckets yourself.
 */
struct ht {
	int *keys;	/* nums value */
	int *vals;	/* index in nums */
	bool *used;	/* slot occupied? */
	unsigned int mask;	/* capacity - 1, capacity is a power of two */
};

/* Given: spread bits so nearby keys don't land in nearby slots. */
static unsigned int hash(int key)
{
	unsigned int x = (unsigned int)key * 2654435761u;

	return x ^ (x >> 16);	/* fold high bits down: you will use & mask */
}

/* TODO: capacity = smallest power of two >= 2 * n (at least 2). */
static int ht_init(struct ht *h, int n)
{
	(void)h;
	(void)n;
	return -1;
}

/* TODO */
static void ht_free(struct ht *h)
{
	(void)h;
}

/* TODO: return true and set *val if key is present. */
static bool ht_find(const struct ht *h, int key, int *val)
{
	(void)h;
	(void)key;
	(void)val;
	return false;
}

/* TODO: insert key -> val. Should a duplicate key overwrite? (see .h) */
static void ht_insert(struct ht *h, int key, int val)
{
	(void)h;
	(void)key;
	(void)val;
}

int two_sum_hash(const int *nums, int n, int target, int *i, int *j)
{
	/* TODO: one pass, O(n) time, O(n) space */
	(void)nums;
	(void)n;
	(void)target;
	(void)i;
	(void)j;
	(void)hash;
	(void)ht_init;
	(void)ht_free;
	(void)ht_find;
	(void)ht_insert;
	return 0;
}
