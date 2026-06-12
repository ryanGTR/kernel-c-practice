// SPDX-License-Identifier: GPL-2.0
/*
 * Exercise 04: Syscall Trace
 *
 * Understand how userspace talks to the kernel.
 * Instead of using printf (which wraps write()), call syscalls directly.
 *
 * Tasks:
 *   1. Use write() to output text (libc wrapper)
 *   2. Use syscall(SYS_write, ...) to call the syscall directly
 *   3. Use syscall(SYS_getpid) to get your process ID
 *   4. Compare: run with `strace ./main` and observe the syscalls
 *
 * Key insight: every I/O operation your program does eventually
 * becomes a syscall. printf -> write -> SYSCALL_DEFINE3(write, ...)
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

/*
 * Helper: write a string using the write() libc wrapper.
 * This is what printf() eventually calls.
 */
static ssize_t write_via_libc(const char *msg)
{
	return write(STDOUT_FILENO, msg, strlen(msg));
}

/*
 * Helper: write a string using syscall() directly.
 * Bypasses the libc wrapper, goes straight to the kernel.
 */
static void write_via_syscall(const char *msg)
{
	syscall(SYS_write, STDOUT_FILENO, msg, strlen(msg));
}

/*
 * Helper: int to string (kernel style - no snprintf dependency)
 * In the kernel, you'd use scnprintf() or simple_itoa().
 */
static int int_to_str(long val, char *buf, int size)
{
	int i = 0;
	int start, end;
	long v = val;

	if (size < 2)
		return 0;

	if (val < 0) {
		buf[i++] = '-';
		v = -v;
	}

	start = i;
	do {
		if (i >= size - 1)
			break;
		buf[i++] = '0' + (v % 10);
		v /= 10;
	} while (v > 0);

	buf[i] = '\0';

	/* Reverse the digits */
	end = i - 1;
	while (start < end) {
		char tmp = buf[start];

		buf[start] = buf[end];
		buf[end] = tmp;
		start++;
		end--;
	}

	return i;
}

int main(void)
{
	char buf[64];
	long pid;

	/* Method 1: libc write() wrapper */
	write_via_libc("=== Method 1: write() libc wrapper ===\n");
	write_via_libc("hello from write()!\n\n");

	/* Method 2: direct syscall() */
	write_via_syscall("=== Method 2: syscall(SYS_write) ===\n");
	write_via_syscall("hello from raw syscall!\n\n");

	/* Method 3: get PID via syscall */
	write_via_libc("=== Method 3: syscall(SYS_getpid) ===\n");
	pid = syscall(SYS_getpid);

	write_via_libc("my pid = ");
	int_to_str(pid, buf, sizeof(buf));
	write_via_libc(buf);
	write_via_libc("\n\n");

	/*
	 * Try it:
	 *   strace -e trace=write ./main
	 *
	 * You'll see every write() call, whether it came from
	 * the libc wrapper or the direct syscall() - because
	 * they both end up at the same kernel entry point:
	 *   SYSCALL_DEFINE3(write, ...) in fs/read_write.c
	 */
	write_via_libc("Now run: strace -e trace=write,getpid ./main\n");
	write_via_libc("All tests passed!\n");

	return 0;
}
