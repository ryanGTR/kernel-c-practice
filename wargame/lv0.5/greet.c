// SPDX-License-Identifier: GPL-2.0
//
// LV0.5 - "Pass me a value"
//
// Make this module accept two load-time parameters:
//   name  (string) default "world"
//   times (int)    default 1
// On load, print this line, repeated <times> times:
//   LV0.5: hello, <name>!
//
// Java analogy: module_param() is like constructor args / @Value injection
// — values handed in from outside at "construction" (insmod) time.

#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

/*
 * TODO 1: declare the two parameter variables, with their defaults.
 *   hint: a string param is a `char *`, an int param is an `int`.
 */


/*
 * TODO 2: expose them as module parameters so insmod can set them.
 *   hint: module_param(name_of_var, type, perm);
 *   the types you need are `charp` (char pointer) and `int`.
 *   perm controls the /sys/module/.../parameters file mode, e.g. 0444.
 */


static int __init greet_init(void)
{
	int i;

	/* TODO 3: loop <times> times, printing the greeting each iteration. */

	return 0;
}

static void __exit greet_exit(void)
{
	pr_info("LV0.5: unloaded\n");
}

module_init(greet_init);
module_exit(greet_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel wargame LV0.5 - module parameters");
