// SPDX-License-Identifier: GPL-2.0
/*
 * cmul_trace.c - record per-round timing from a cold start, so the
 * warm-up ramp (page faults, cache, CPU clock) is visible in a plot.
 *
 * usage: ./cmul_trace <order>   order = 43 (mul4 first) or 34 (mul3 first)
 * output (stdout, CSV): variant,round,ns_per_op
 */
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M	4096	/* complex numbers per pass */
#define PASSES	50	/* passes per timed round */
#define ROUNDS	400

static double ar[M], ai[M], br[M], bi[M], cr[M], ci[M];

static double now(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec + ts.tv_nsec / 1e9;
}

static void pass4(void)
{
	int i;

	for (i = 0; i < M; i++) {
		cr[i] = ar[i] * br[i] - ai[i] * bi[i];
		ci[i] = ar[i] * bi[i] + ai[i] * br[i];
	}
}

static void pass3(void)
{
	double k1, k2, k3;
	int i;

	for (i = 0; i < M; i++) {
		k1 = ar[i] * br[i];
		k2 = ai[i] * bi[i];
		k3 = (ar[i] + ai[i]) * (br[i] + bi[i]);
		cr[i] = k1 - k2;
		ci[i] = k3 - k1 - k2;
	}
}

static double trace(const char *name, void (*pass)(void))
{
	double sink = 0, t0;
	int r, p;

	for (r = 0; r < ROUNDS; r++) {
		t0 = now();
		for (p = 0; p < PASSES; p++) {
			pass();
			sink += cr[p] + ci[p];
		}
		printf("%s,%d,%.4f\n", name,
		       r, (now() - t0) / ((double)M * PASSES) * 1e9);
	}
	return sink;
}

int main(int argc, char **argv)
{
	double sink = 0;
	int i;

	for (i = 0; i < M; i++) {
		ar[i] = rand() / (double)RAND_MAX;
		ai[i] = rand() / (double)RAND_MAX;
		br[i] = rand() / (double)RAND_MAX;
		bi[i] = rand() / (double)RAND_MAX;
	}
	if (argc > 1 && !strcmp(argv[1], "34")) {
		sink += trace("mul3", pass3);
		sink += trace("mul4", pass4);
	} else {
		sink += trace("mul4", pass4);
		sink += trace("mul3", pass3);
	}
	fprintf(stderr, "sink %g\n", sink);
	return 0;
}
