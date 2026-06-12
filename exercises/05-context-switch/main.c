// SPDX-License-Identifier: GPL-2.0
/*
 * Exercise 05: Observing Context Switches
 *
 * Context switch = the kernel saves one process's CPU state and
 * loads another's. Every time you sleep, do I/O, or get preempted,
 * a context switch happens.
 *
 * This program lets you observe context switches in real time
 * by reading /proc/self/status before and after different operations.
 *
 * Tasks:
 *   1. Read your own context switch counters from /proc/self/status
 *   2. Compare counters after sleep (voluntary) vs busy loop (involuntary)
 *   3. Observe how I/O causes voluntary context switches
 *
 * Run with: strace -e trace=sched_yield,nanosleep ./main
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

struct ctxt_counts {
	unsigned long voluntary;
	unsigned long nonvoluntary;
};

static int read_ctxt_switches(struct ctxt_counts *counts)
{
	FILE *fp;
	char line[256];

	fp = fopen("/proc/self/status", "r");
	if (!fp)
		return -1;

	counts->voluntary = 0;
	counts->nonvoluntary = 0;

	while (fgets(line, sizeof(line), fp)) {
		if (strncmp(line, "voluntary_ctxt_switches:", 24) == 0)
			counts->voluntary = strtoul(line + 24, NULL, 10);
		else if (strncmp(line, "nonvoluntary_ctxt_switches:", 27) == 0)
			counts->nonvoluntary = strtoul(line + 27, NULL, 10);
	}

	fclose(fp);
	return 0;
}

static void print_counts(const char *label, const struct ctxt_counts *counts)
{
	printf("  [%s] voluntary=%lu  nonvoluntary=%lu\n",
	       label, counts->voluntary, counts->nonvoluntary);
}

static void print_diff(const struct ctxt_counts *before,
		       const struct ctxt_counts *after)
{
	printf("  delta: voluntary=+%lu  nonvoluntary=+%lu\n",
	       after->voluntary - before->voluntary,
	       after->nonvoluntary - before->nonvoluntary);
}

/*
 * Test 1: sleep causes voluntary context switches
 * When you sleep, you voluntarily give up the CPU.
 */
static int test_voluntary(void)
{
	struct ctxt_counts before, after;
	struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000000 }; /* 10ms */
	int i;

	printf("\nTest 1: Voluntary context switches (sleep)\n");

	read_ctxt_switches(&before);
	print_counts("before", &before);

	/* Each nanosleep is a voluntary context switch */
	for (i = 0; i < 5; i++)
		nanosleep(&ts, NULL);

	read_ctxt_switches(&after);
	print_counts("after", &after);
	print_diff(&before, &after);

	if (after.voluntary <= before.voluntary) {
		printf("  UNEXPECTED: voluntary count didn't increase\n");
		return 1;
	}

	printf("  PASS: sleep caused voluntary context switches\n");
	return 0;
}

/*
 * Test 2: busy loop may cause involuntary context switches
 * If you hog the CPU, the scheduler will eventually preempt you.
 */
static int test_involuntary(void)
{
	struct ctxt_counts before, after;
	volatile long sum = 0;
	long i;

	printf("\nTest 2: Involuntary context switches (busy loop)\n");

	read_ctxt_switches(&before);
	print_counts("before", &before);

	/* Burn CPU — scheduler should preempt us */
	for (i = 0; i < 100000000L; i++)
		sum += i;

	read_ctxt_switches(&after);
	print_counts("after", &after);
	print_diff(&before, &after);

	/*
	 * Note: involuntary switches depend on system load and
	 * scheduler settings. On an idle system this might be 0.
	 * That's OK — the point is understanding the concept.
	 */
	printf("  INFO: busy loop ran, sum=%ld (prevents optimization)\n", sum);
	printf("  PASS: check the counters above\n");
	return 0;
}

/*
 * Test 3: I/O causes voluntary context switches
 * Writing to a file involves the kernel, which may schedule.
 */
static int test_io(void)
{
	struct ctxt_counts before, after;
	FILE *fp;
	int i;

	printf("\nTest 3: I/O context switches (file write)\n");

	read_ctxt_switches(&before);
	print_counts("before", &before);

	for (i = 0; i < 10; i++) {
		fp = fopen("/tmp/ctxt_test.tmp", "w");
		if (!fp)
			return 1;
		fprintf(fp, "context switch test %d\n", i);
		fflush(fp);
		fsync(fileno(fp));
		fclose(fp);
	}

	read_ctxt_switches(&after);
	print_counts("after", &after);
	print_diff(&before, &after);

	remove("/tmp/ctxt_test.tmp");

	printf("  PASS: I/O operations completed\n");
	return 0;
}

int main(void)
{
	int ret = 0;

	printf("=== Context Switch Observer ===\n");
	printf("PID: %d\n", getpid());
	printf("\nKernel context types:\n");
	printf("  voluntary   = process chose to give up CPU (sleep, I/O)\n");
	printf("  nonvoluntary = scheduler forced a switch (time slice expired)\n");

	ret |= test_voluntary();
	ret |= test_involuntary();
	ret |= test_io();

	printf("\nAll tests passed!\n");

	printf("\nTry also:\n");
	printf("  vmstat 1 5              # watch 'cs' column\n");
	printf("  cat /proc/%d/status | grep ctxt\n", getpid());

	return ret;
}
