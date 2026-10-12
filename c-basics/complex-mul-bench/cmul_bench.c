// SPDX-License-Identifier: GPL-2.0
/*
 * cmul_bench.c - compare three ways to multiply complex numbers.
 *
 *   mul4:    (ac - bd) + (ad + bc)i, 4 multiplies + 2 add/sub
 *   mul3:    k1 = ac, k2 = bd, k3 = (a + b)(c + d)
 *            real = k1 - k2, imag = k3 - k1 - k2, 3 multiplies + 5 add/sub
 *   builtin: C99 z1 * z2 (may call __muldc3 for Annex G inf/NaN rules)
 *
 * usage: ./cmul_bench [s] [4|3|b]  (default: all three)
 *
 * Each variant runs a dependent chain so the compiler cannot skip work.
 */
#define _POSIX_C_SOURCE 199309L	/* clock_gettime under -std=c11 */
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 100000000L

struct cpx {
	double re;
	double im;
};

static struct cpx mul4(struct cpx x, struct cpx y)
{
	struct cpx r = { x.re * y.re - x.im * y.im,
			 x.re * y.im + x.im * y.re };
	return r;
}

static struct cpx mul3(struct cpx x, struct cpx y)
{
	double k1 = x.re * y.re;
	double k2 = x.im * y.im;
	double k3 = (x.re + x.im) * (y.re + y.im);
	struct cpx r = { k1 - k2, k3 - k1 - k2 };
	return r;
}

static double now(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
	return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(int argc, char **argv)
{
	/* runtime input so the compiler cannot precompute the answer */
	double s = argc > 1 ? atof(argv[1]) : 0.9999999;
	struct cpx w = { s, 1e-8 };
	struct cpx a = { 1.0, 0.0 };
	double complex wz = s + 1e-8 * I;
	double complex az = 1.0;
	double t0, t4, t3, tb;
	const char *only = argc > 2 ? argv[2] : "all";
	long i;

	if (strcmp(only, "all") && strcmp(only, "4"))
		goto skip4;
	t0 = now();
	for (i = 0; i < N; i++)
		a = mul4(a, w);
	t4 = now() - t0;
	printf("mul4    %.3f s  (%.2f ns/op)  result %.6g%+.6gi\n",
	       t4, t4 / N * 1e9, a.re, a.im);

skip4:
	if (strcmp(only, "all") && strcmp(only, "3"))
		goto skip3;
	a.re = 1.0;
	a.im = 0.0;
	t0 = now();
	for (i = 0; i < N; i++)
		a = mul3(a, w);
	t3 = now() - t0;
	printf("mul3    %.3f s  (%.2f ns/op)  result %.6g%+.6gi\n",
	       t3, t3 / N * 1e9, a.re, a.im);

skip3:
	if (strcmp(only, "all") && strcmp(only, "b"))
		return 0;
	t0 = now();
	for (i = 0; i < N; i++)
		az = az * wz;
	tb = now() - t0;
	printf("builtin %.3f s  (%.2f ns/op)  result %.6g%+.6gi\n",
	       tb, tb / N * 1e9, creal(az), cimag(az));
	return 0;
}
