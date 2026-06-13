// SPDX-License-Identifier: GPL-2.0
/*
 * kasan_uaf - deliberately trigger a use-after-free, caught by KASAN
 *
 * WARNING: only insmod this inside QEMU/VM. It corrupts freed memory.
 *
 * Load:   insmod kasan_uaf.ko   (triggers the bug at init time)
 * Unload: rmmod kasan_uaf
 *
 * Needs a kernel built with CONFIG_KASAN=y. After insmod, the KASAN
 * report shows up in dmesg with "Allocated by" / "Freed by" stacks.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
/* TODO: MODULE_AUTHOR, MODULE_DESCRIPTION */

static int __init kasan_uaf_init(void)
{
	char *buf;

	/* TODO: kmalloc a small buffer (e.g. 32 bytes, GFP_KERNEL) into buf */

	/* TODO: kfree(buf) -- the buffer is now dead */

	/*
	 * TODO: deliberately touch buf AFTER the kfree above
	 *       (e.g. write a byte: buf[0] = 'x';).
	 *       This is the use-after-free KASAN must catch.
	 *       Think: what should the report's two stacks point at?
	 */

	pr_info("kasan_uaf: loaded (check dmesg for the KASAN report)\n");
	return 0;
}

static void __exit kasan_uaf_exit(void)
{
	pr_info("kasan_uaf: unloaded\n");
}

module_init(kasan_uaf_init);
module_exit(kasan_uaf_exit);
