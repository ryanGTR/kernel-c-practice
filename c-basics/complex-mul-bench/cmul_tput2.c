#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define M 4096
#define R 50000
static double ar[M], ai[M], br[M], bi[M], cr[M], ci[M];
static double now(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
	return ts.tv_sec + ts.tv_nsec / 1e9;
}
static double run4(void)
{
	double s = 0;
	for (int r = 0; r < R; r++) {
		for (int i = 0; i < M; i++) {
			cr[i] = ar[i] * br[i] - ai[i] * bi[i];
			ci[i] = ar[i] * bi[i] + ai[i] * br[i];
		}
		s += cr[r % M] + ci[r % M];
	}
	return s;
}
static double run3(void)
{
	double s = 0;
	for (int r = 0; r < R; r++) {
		for (int i = 0; i < M; i++) {
			double k1 = ar[i] * br[i], k2 = ai[i] * bi[i];
			double k3 = (ar[i] + ai[i]) * (br[i] + bi[i]);

			cr[i] = k1 - k2;
			ci[i] = k3 - k1 - k2;
		}
		s += cr[r % M] + ci[r % M];
	}
	return s;
}
int main(void)
{
	double sink = 0, t0, t;
	for (int i = 0; i < M; i++) {
		ar[i] = rand() / (double)RAND_MAX; ai[i] = rand() / (double)RAND_MAX;
		br[i] = rand() / (double)RAND_MAX; bi[i] = rand() / (double)RAND_MAX;
	}
	sink += run4() + run3();	/* warm-up: page faults, cache, CPU clock */
	for (int k = 0; k < 4; k++) {
		t0 = now(); sink += run4(); t = now() - t0;
		printf("mul4 %.3f ns/op\n", t / ((double)M * R) * 1e9);
		t0 = now(); sink += run3(); t = now() - t0;
		printf("mul3 %.3f ns/op\n", t / ((double)M * R) * 1e9);
	}
	fprintf(stderr, "sink %g\n", sink);
	return 0;
}
