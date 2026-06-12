// SPDX-License-Identifier: GPL-2.0
/* Problem #30 — atomic counter 參考解 */

#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include <assert.h>

struct counter {
	_Atomic int value;
};

static void counter_init(struct counter *c, int v)
{
	atomic_init(&c->value, v);
}

static void counter_inc(struct counter *c)
{
	atomic_fetch_add(&c->value, 1);
}

static void counter_dec(struct counter *c)
{
	atomic_fetch_sub(&c->value, 1);
}

static int counter_get(const struct counter *c)
{
	/* atomic_load 對 const pointer 仍 OK */
	return atomic_load((_Atomic int *)&c->value);
}

static int counter_inc_and_get(struct counter *c)
{
	return atomic_fetch_add(&c->value, 1) + 1;
}

#define N_THREADS 4
#define N_INC_PER_THREAD 100000

static void *worker(void *arg)
{
	struct counter *c = arg;
	for (int i = 0; i < N_INC_PER_THREAD; i++)
		counter_inc(c);
	return NULL;
}

int main(void)
{
	struct counter c;
	counter_init(&c, 0);
	assert(counter_get(&c) == 0);

	counter_inc(&c);
	counter_inc(&c);
	counter_inc(&c);
	assert(counter_get(&c) == 3);

	counter_dec(&c);
	assert(counter_get(&c) == 2);

	int v = counter_inc_and_get(&c);
	assert(v == 3);

	counter_init(&c, 0);
	pthread_t th[N_THREADS];
	for (int i = 0; i < N_THREADS; i++)
		pthread_create(&th[i], NULL, worker, &c);
	for (int i = 0; i < N_THREADS; i++)
		pthread_join(th[i], NULL);

	assert(counter_get(&c) == N_THREADS * N_INC_PER_THREAD);

	printf("✅ Problem #30 atomic_counter passed\n");
	return 0;
}
