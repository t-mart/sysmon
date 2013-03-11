#include <linux/kernel.h>
#include <linux/module.h>

#include "sysmon.h"
#include "interposer.h"
#include "uid.h"
#include "toggle.h"
#include "log.h"

#define LICENSE "GPL"
#define AUTHORS "Eric Huang <ehuang3@gatech.edu>, Tim Martin <tim.martin@gatech.edu>"
#define DESC "A system call interposer and logger for Georgia Tech's CS3210, Spring 2013"

static int __init sysmon_init(void)
{
	char failure;

	INFO_PRINT("initializing...\n");
	printk(KERN_INFO "foobar\n");

	// if any of this fails, you probably need to reboot
	failure = start_interposer() | start_uid() | start_toggle() | start_log();

	INFO_PRINT("initialization done\n");

	return failure;
}

static void __exit sysmon_exit(void)
{
	INFO_PRINT("exiting...\n");

	stop_interposer();
	stop_uid();
	stop_toggle();
	stop_log();

	INFO_PRINT("bye!\n");
}

module_init(sysmon_init);
module_exit(sysmon_exit);

MODULE_LICENSE(LICENSE);
MODULE_AUTHOR(AUTHORS);
MODULE_DESCRIPTION(DESC);

// vim:tw=80:ts=4:sw=4:noexpandtab
