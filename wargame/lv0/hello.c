// SPDX-License-Identifier: GPL-2.0
//
// LV0 - "Hello, Kernel"
// Your first kernel module: print a line on load, another on unload.
// Java analogy: this is your first `public static void main` + println,
// except it runs in kernel space and output goes to dmesg, not the console.

#include <linux/module.h>
#include <linux/init.h>

static int __init hello_init(void)
{
	pr_info("Hello, Kernel\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("LV0: goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel wargame LV0 - hello world module");
