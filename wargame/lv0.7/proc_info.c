// SPDX-License-Identifier: GPL-2.0
//
// LV0.7 - "A window into the kernel"
//
// Create a file /proc/wargame_lv07 that, when read, prints:
//   LV0.7: read count = N
// where N increases by 1 on every read.
//
// Java analogy: a /proc node is like a read-only REST endpoint the kernel
// exposes; `cat` is the GET request that pulls live state out.

#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define PROC_NAME "wargame_lv07"

static int read_count;			/* live state we expose */
static struct proc_dir_entry *entry;

static int lv07_show(struct seq_file *m, void *v)
{
	/*
	 * TODO 1: increment read_count, then seq_printf() the required line.
	 *   seq_file handles the user-space copy for you (safer than a raw
	 *   copy_to_user).
	 */
	return 0;
}

static int lv07_open(struct inode *inode, struct file *file)
{
	return single_open(file, lv07_show, NULL);
}

/* proc_ops replaced file_operations for /proc since kernel 5.6 */
static const struct proc_ops lv07_ops = {
	.proc_open	= lv07_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

static int __init lv07_init(void)
{
	/*
	 * TODO 2: create the entry with proc_create(PROC_NAME, 0444, NULL,
	 *         &lv07_ops); store the result in `entry`.
	 */
	return 0;
}

static void __exit lv07_exit(void)
{
	/* TODO 3: remove the /proc entry so it disappears on unload. */
}

module_init(lv07_init);
module_exit(lv07_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel wargame LV0.7 - /proc file");
