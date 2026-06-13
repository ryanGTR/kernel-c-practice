// SPDX-License-Identifier: GPL-2.0
/*
 * kmemleak_demo - deliberately leak memory, caught by kmemleak
 *
 * WARNING: only insmod this inside QEMU/VM.
 *
 * Load:   insmod kmemleak_demo.ko   (allocates and "forgets" a buffer)
 * Unload: rmmod kmemleak_demo       (does NOT free -> the leak)
 *
 * Needs a kernel with CONFIG_DEBUG_KMEMLEAK=y. Trigger a scan via
 * /sys/kernel/debug/kmemleak, then read it to see the unreferenced
 * object whose alloc stack points back into this module.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
/* TODO: MODULE_AUTHOR, MODULE_DESCRIPTION */

static int __init kmemleak_demo_init(void)
{
	/*
	 * TODO: kmalloc a buffer into a *local* variable only, then return.
	 *       Deliberately do NOT store the pointer in any global / static,
	 *       and do NOT free it in exit. Once init returns, the only
	 *       pointer is gone -> kmemleak sees an unreferenced object.
	 *
	 * Hint: if you accidentally keep a global pointer to it, kmemleak
	 *       considers it still referenced and will NOT report it. Think
	 *       about why "reachable from a known pointer" == "not a leak".
	 */

	pr_info("kmemleak_demo: loaded (a buffer was just leaked)\n");
	return 0;
}

static void __exit kmemleak_demo_exit(void)
{
	/* TODO: intentionally leave this empty -- no kfree -> the leak */
	pr_info("kmemleak_demo: unloaded (nothing freed on purpose)\n");
}

module_init(kmemleak_demo_init);
module_exit(kmemleak_demo_exit);
