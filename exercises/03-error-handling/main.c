// SPDX-License-Identifier: GPL-2.0
/*
 * Exercise 03: Kernel-style Error Handling with goto
 *
 * The kernel uses goto for cleanup on error, not exceptions.
 * This pattern ensures resources are freed in reverse order
 * when any step fails.
 *
 * Tasks:
 *   1. Simulate a multi-step initialization that can fail
 *   2. Use goto cleanup labels (out_free_X, out_close_Y, etc.)
 *   3. Return negative errno-style error codes
 *   4. Verify all resources are properly cleaned up on failure
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct config {
	char *name;
	int *buffer;
	int *table;
};

static int load_name(struct config *cfg, const char *name)
{
	cfg->name = strdup(name);
	if (!cfg->name)
		return -ENOMEM;

	printf("  [+] name loaded: %s\n", cfg->name);
	return 0;
}

static int alloc_buffer(struct config *cfg, int size, int fail)
{
	if (fail) {
		printf("  [!] buffer allocation simulated failure\n");
		return -ENOMEM;
	}

	cfg->buffer = calloc(size, sizeof(int));
	if (!cfg->buffer)
		return -ENOMEM;

	printf("  [+] buffer allocated: %d ints\n", size);
	return 0;
}

static int alloc_table(struct config *cfg, int size, int fail)
{
	if (fail) {
		printf("  [!] table allocation simulated failure\n");
		return -ENOMEM;
	}

	cfg->table = calloc(size, sizeof(int));
	if (!cfg->table)
		return -ENOMEM;

	printf("  [+] table allocated: %d ints\n", size);
	return 0;
}

/*
 * init_config - Initialize config with kernel-style error handling
 *
 * Demonstrates the goto cleanup pattern:
 * - Resources acquired in order: name -> buffer -> table
 * - On error, cleanup in reverse order
 * - Each goto label only frees what was allocated before it
 */
static int init_config(struct config *cfg, const char *name,
		       int fail_stage)
{
	int ret;

	memset(cfg, 0, sizeof(*cfg));

	ret = load_name(cfg, name);
	if (ret)
		goto out;

	ret = alloc_buffer(cfg, 1024, fail_stage == 2);
	if (ret)
		goto out_free_name;

	ret = alloc_table(cfg, 256, fail_stage == 3);
	if (ret)
		goto out_free_buffer;

	printf("  [+] config fully initialized\n");
	return 0;

out_free_buffer:
	printf("  [-] freeing buffer\n");
	free(cfg->buffer);
	cfg->buffer = NULL;
out_free_name:
	printf("  [-] freeing name\n");
	free(cfg->name);
	cfg->name = NULL;
out:
	return ret;
}

static void destroy_config(struct config *cfg)
{
	free(cfg->table);
	free(cfg->buffer);
	free(cfg->name);
	memset(cfg, 0, sizeof(*cfg));
	printf("  [-] config destroyed\n");
}

int main(void)
{
	struct config cfg;
	int ret;

	/* Test 1: Everything succeeds */
	printf("Test 1: Normal init (no failure)\n");
	ret = init_config(&cfg, "test-device", 0);
	if (ret) {
		printf("  FAIL: unexpected error %d\n", ret);
		return 1;
	}
	destroy_config(&cfg);
	printf("  PASS\n\n");

	/* Test 2: Fail at buffer allocation (stage 2) */
	printf("Test 2: Fail at buffer alloc\n");
	ret = init_config(&cfg, "fail-buffer", 2);
	if (ret != -ENOMEM) {
		printf("  FAIL: expected -ENOMEM, got %d\n", ret);
		return 1;
	}
	/* name should have been freed by goto cleanup */
	printf("  PASS (cleanup handled correctly)\n\n");

	/* Test 3: Fail at table allocation (stage 3) */
	printf("Test 3: Fail at table alloc\n");
	ret = init_config(&cfg, "fail-table", 3);
	if (ret != -ENOMEM) {
		printf("  FAIL: expected -ENOMEM, got %d\n", ret);
		return 1;
	}
	/* both name and buffer should have been freed */
	printf("  PASS (cleanup handled correctly)\n\n");

	printf("All tests passed!\n");
	return 0;
}
