// SPDX-License-Identifier: GPL-2.0
/*
 * panic_notifier - register a custom panic notifier, with an optional
 *                  soft-lockup trigger
 *
 * !!! DANGER: ONLY insmod this inside QEMU/VM. The soft-lockup option
 *     hangs a CPU and the panic path can take the whole machine down.
 *     NEVER run this on your host (Framework 16). !!!
 *
 * Load:   insmod panic_notifier.ko [lockup=1]
 * Unload: rmmod panic_notifier
 *
 * Registers a callback on panic_notifier_list so it runs while the
 * kernel is panicking. With lockup=1 it also spins forever to make the
 * soft lockup detector fire.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/panic_notifier.h>

MODULE_LICENSE("GPL");
/* TODO: MODULE_AUTHOR, MODULE_DESCRIPTION */

/* TODO: declare a module_param "lockup" (int, default 0, 0444) */

/*
 * TODO: write the panic handler.
 *   Signature: static int my_panic_handler(struct notifier_block *nb,
 *					    unsigned long action, void *data)
 *   - print ONE short line (this runs in panic context: no sleeping,
 *     keep it minimal). e.g. pr_emerg("panic_notifier: bye!\n");
 *   - return NOTIFY_DONE;
 */

static struct notifier_block nb = {
	/* TODO: .notifier_call = my_panic_handler, */
};

static int __init panic_notifier_init(void)
{
	/*
	 * TODO: register the handler so it fires on panic:
	 *   atomic_notifier_chain_register(&panic_notifier_list, &nb);
	 */

	/*
	 * TODO (DANGER, QEMU only): if lockup is set, spin forever to
	 * trip the soft lockup detector:
	 *
	 *   if (lockup)
	 *           while (1)
	 *                   ;   // never yields the CPU -> soft lockup
	 *
	 * Think: why does a busy while(1) trip the *soft* lockup watchdog
	 * but not necessarily a hard lockup? (hint: interrupts still on)
	 * In QEMU, dmesg will show "BUG: soft lockup - CPU#N stuck".
	 */

	pr_info("panic_notifier: loaded (lockup option wired via module_param)\n");
	return 0;
}

static void __exit panic_notifier_exit(void)
{
	/*
	 * TODO: unregister on unload, or you leave a dangling callback:
	 *   atomic_notifier_chain_unregister(&panic_notifier_list, &nb);
	 */
	pr_info("panic_notifier: unloaded\n");
}

module_init(panic_notifier_init);
module_exit(panic_notifier_exit);
