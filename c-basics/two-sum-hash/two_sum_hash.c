#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "two_sum_hash.h"

/*
 * Open addressing + linear probing.
 * Java analogy: HashMap<Integer, Integer> (value -> index), but you
 * build the buckets yourself.
 */
struct ht {
	int *keys; /* nums value */
	int *vals; /* index in nums */
	bool *used; /* slot occupied? */
	unsigned int mask; /* capacity - 1, capacity is a power of two */
};

/* Given: spread bits so nearby keys don't land in nearby slots. */
static unsigned int hash(int key)
{
	unsigned int x = (unsigned int)key * 2654435761u;

	return x ^ (x >> 16); /* fold high bits down: you will use & mask */
}

/* TODO: capacity = smallest power of two >= 2 * n (at least 2). */
static int ht_init(struct ht *h, int n)
{
	unsigned int cap = 2;

	while (cap < (unsigned int)2 * n)
		cap = cap * 2;

	h->mask = cap - 1;
	h->keys = malloc(cap * sizeof(int));
	h->vals = malloc(cap * sizeof(int));
	h->used = calloc(cap, sizeof(bool));

	if (!h->keys || !h->vals || !h->used) {
		free(h->keys);
		free(h->vals);
		free(h->used);
		return -1;
	}

	return 0;
}

/* TODO */
static void ht_free(struct ht *h)
{
	free(h->keys);
	free(h->vals);
	free(h->used);
}

/* TODO: return true and set *val if key is present. */
static bool ht_find(const struct ht *h, int key, int *val)
{
	unsigned int i = hash(key) & h->mask;

	for (;;) {
		if (h->used[i] && h->keys[i] == key) {
			*val = h->vals[i];
			return true;
		}

		if (!h->used[i])
			return false;

		i = i + 1;

		if (i == h->mask + 1)
			i = 0;
	}

	return false;
}

/* TODO: insert key -> val. Should a duplicate key overwrite? (see .h) */
static void ht_insert(struct ht *h, int key, int val)
{
	unsigned int i = hash(key) & h->mask;

	for (;;) {
		if (h->used[i] && h->keys[i] == key)
			return;

		if (!h->used[i]) {
			h->used[i] = true;
			h->keys[i] = key;
			h->vals[i] = val;
			return;
		}

		i = i + 1;

		if (i == h->mask + 1)
			i = 0;
	}
}

int two_sum_hash(const int *nums, int n, int target, int *i, int *j)
{
	/* TODO: one pass, O(n) time, O(n) space */
	struct ht h;
	int k, idx;

	if (ht_init(&h, n))
		return -1;

	for (k = 0; k < n; k++) {
		long long want = (long long)target - nums[k];

		if (want >= INT_MIN && want <= INT_MAX &&
		    ht_find(&h, want, &idx)) {
			*i = idx;
			*j = k;
			ht_free(&h);
			return 1;
		}
		ht_insert(&h, nums[k], k);
	}
	ht_free(&h);

	return 0;
}
