// SPDX-License-Identifier: GPL-2.0
//
// LV0.9 - "Talking to a device"
//
// Register a character device. When userspace reads /dev/lv09 it should
// receive the line:
//   LV0.9: you are talking to a char device
//
// Java analogy: a char device is like an InputStream the kernel exposes —
// open() gives you the stream, read() pulls bytes until EOF (return 0).

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "lv09"

static int major;
static const char msg[] = "LV0.9: you are talking to a char device\n";

static ssize_t dev_read(struct file *f, char __user *buf,
			size_t len, loff_t *off)
{
	/*
	 * TODO 1: send msg to user space, one shot:
	 *   - if *off > 0 the message was already delivered -> return 0 (EOF)
	 *     so `cat` stops instead of looping forever.
	 *   - otherwise copy_to_user(buf, msg, n), advance *off, return n.
	 *     copy_to_user returns the number of bytes NOT copied (0 == success).
	 */
	return 0;
}

static int dev_open(struct inode *i, struct file *f)	{ return 0; }
static int dev_release(struct inode *i, struct file *f)	{ return 0; }

static const struct file_operations fops = {
	.owner	 = THIS_MODULE,
	.read	 = dev_read,
	.open	 = dev_open,
	.release = dev_release,
};

static int __init lv09_init(void)
{
	/*
	 * TODO 2: major = register_chrdev(0, DEVICE_NAME, &fops);
	 *   0 lets the kernel pick a free major number. Then pr_info() the
	 *   major so you know what to pass to `mknod`.  Bail out if major < 0.
	 */
	return 0;
}

static void __exit lv09_exit(void)
{
	/* TODO 3: unregister_chrdev(major, DEVICE_NAME); */
}

module_init(lv09_init);
module_exit(lv09_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel wargame LV0.9 - char device");
