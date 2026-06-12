// SPDX-License-Identifier: GPL-2.0
/*
 * proc_info - create a /proc/my_info entry with system information
 *
 * Load:   insmod proc_info.ko
 * Unload: rmmod proc_info
 * Read:   cat /proc/my_info
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
/* TODO: MODULE_AUTHOR, MODULE_DESCRIPTION */

/* TODO: static unsigned long load_time;       -- store jiffies at init */
/* TODO: static atomic_t read_count;           -- atomic read counter */
/* TODO: static struct proc_dir_entry *entry;  -- proc entry handle */

/*
 * TODO: implement my_info_read()
 *
 * Signature:
 *   static ssize_t my_info_read(struct file *file, char __user *buf,
 *                               size_t count, loff_t *pos)
 *
 * Steps:
 *   1. atomic_inc the read counter
 *   2. snprintf into a local buffer:
 *      - "Module loaded at: %lu jiffies\n"
 *      - "Read count: %d\n"
 *      - "Kernel version: %s\n"  (use utsname()->release)
 *   3. return simple_read_from_buffer(buf, count, pos, local_buf, len)
 */

/*
 * TODO: define proc_ops struct
 *
 *   static const struct proc_ops my_proc_ops = {
 *       .proc_read = my_info_read,
 *   };
 */

static int __init proc_info_init(void)
{
	/* TODO: save jiffies to load_time */
	/* TODO: init atomic counter */
	/* TODO: proc_create("my_info", 0444, NULL, &my_proc_ops) */
	/* TODO: pr_info log */
	return 0;
}

static void __exit proc_info_exit(void)
{
	/* TODO: proc_remove(entry) */
	/* TODO: pr_info log */
}

module_init(proc_info_init);
module_exit(proc_info_exit);
