// SPDX-License-Identifier: GPL-2.0
/*
 * hello - first kernel module
 *
 * Load:   insmod hello.ko [name=<string>]
 * Unload: rmmod hello
 */

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
/* TODO: MODULE_AUTHOR, MODULE_DESCRIPTION */

/* TODO: declare a module_param "name" (charp, default "kernel") */

static int __init hello_init(void)
{
	/* TODO: pr_info("Hello, %s!\n", name); */
	return 0;
}

static void __exit hello_exit(void)
{
	/* TODO: pr_info("Goodbye, %s!\n", name); */
}

module_init(hello_init);
module_exit(hello_exit);
