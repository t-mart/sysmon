#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/proc_fs.h>   // give us access to proc_fs

#include "sysmon.h"

#define ENT_NAME MODULE_NAME "_toggle"

static int toggle_read(char *buffer, char **start, off_t offset,
					int count, int *peof, void *dat)
{
	return 0;
}

static int toggle_write(struct file *file, const char *buffer,
					 unsigned long count, void *data)
{
	return 0;
}

static int start_toggle(void)
{
	mode_t mode = S_IFREG | S_IRUSR | S_IWUSR;
	struct proc_dir entry *toggle_ent;

	INFO_PRINT("creating /proc/" ENT_NAME "...");

	toggle_ent = create_proc_entry(ENT_NAME, mode, NULL);

	if (toggle_ent == NULL) {
		ERR_PRINT("creation of /proc/" ENT_NAME " failed!\n");
		return -EFAULT;
	}

	toggle_ent->read_proc  = toggle_read;
	toggle_ent->write_proc = toggle_write;
	toggle_ent->owner      = THIS_MODULE;
	toggle_ent->uid        = 0; //root
	toggle_ent->gid        = 0; //root

	INFO_PRINT("done\n");

	return 0;
}

static void stop_toggle(void)
{
	INFO_PRINT("removing /proc" ENT_NAME "...");
	remove_proc_entry(ENT_NAME, &proc_root);
	INFO_PRINT("done\n");
}

MODULE_LICENSE(LICENSE);
MODULE_AUTHOR(AUTHORS);
MODULE_DESCRIPTION(DESC);

// vim:tw=80:ts=4:sw=4:noexpandtab
