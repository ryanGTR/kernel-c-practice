// SPDX-License-Identifier: GPL-2.0
/*
 * kprobe_trace - intercept a kernel function with a kprobe
 *
 * WARNING: best run inside QEMU/VM while learning.
 *
 * Load:   insmod kprobe_trace.ko   (registers the probe)
 * Unload: rmmod kprobe_trace       (unregisters it)
 *
 * Places a kprobe on a syscall entry function and logs each call from
 * the pre_handler. Watch the hits with: dmesg -w  (then run e.g. `ls`).
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kprobes.h>

MODULE_LICENSE("GPL");
/* TODO: MODULE_AUTHOR, MODULE_DESCRIPTION */

/*
 * The symbol you probe must match your kernel exactly. On x86-64 the
 * openat syscall entry is often "__x64_sys_openat" (older) or you may
 * prefer "do_sys_openat2". If insmod fails with -EINVAL / "could not
 * find symbol", look it up yourself:
 *     grep -E 'sys_openat|do_sys_openat' /proc/kallsyms
 * then put the real name below.
 */
static struct kprobe kp = {
	.symbol_name = "__x64_sys_openat",	/* TODO: verify / adjust */
};

/*
 * TODO: write the pre_handler.
 *   Signature: static int handler_pre(struct kprobe *p, struct pt_regs *regs)
 *   - print one line proving the probe fired
 *     (e.g. the symbol name p->symbol_name, and current->comm).
 *   - bonus: pull a syscall argument out of `regs` and print it.
 *     (Which register holds arg1 on x86-64? See pt_regs.)
 *   - return 0.
 */

static int __init kprobe_trace_init(void)
{
	/*
	 * TODO:
	 *   1. kp.pre_handler = handler_pre;
	 *   2. ret = register_kprobe(&kp);
	 *   3. if (ret < 0) { pr_err(...); return ret; }
	 *   4. pr_info("kprobe_trace: planted at %s\n", kp.symbol_name);
	 */
	pr_info("kprobe_trace: loaded\n");
	return 0;
}

static void __exit kprobe_trace_exit(void)
{
	/* TODO: unregister_kprobe(&kp); */
	pr_info("kprobe_trace: unloaded\n");
}

module_init(kprobe_trace_init);
module_exit(kprobe_trace_exit);
