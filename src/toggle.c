#include "sysmon.h"
#include "proc.h"
#include "toggle.h"

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

int start_toggle(void)
{
	mode_t mode = S_IFREG | S_IRUSR | S_IWUSR;
	struct proc_dir_entry *toggle_ent;

	INFO_PRINT("creating /proc/" TOGGLE_ENT_NAME "...\n");

	toggle_ent = create_proc_entry(TOGGLE_ENT_NAME, mode, NULL);

	if (toggle_ent == NULL) {
		ERR_PRINT("creation of /proc/" TOGGLE_ENT_NAME " failed!\n");
		return -EFAULT;
	}

	toggle_ent->read_proc  = toggle_read;
	toggle_ent->write_proc = toggle_write;
	toggle_ent->owner      = THIS_MODULE;
	toggle_ent->uid        = 0; //root
	toggle_ent->gid        = 0; //root

	INFO_PRINT("/proc/" TOGGLE_ENT_NAME " created.\n");

	return 0;
}

void stop_toggle(void)
{
	INFO_PRINT("removing /proc" TOGGLE_ENT_NAME "...\n");
	remove_proc_entry(TOGGLE_ENT_NAME, &proc_root);
	INFO_PRINT("/proc/" TOGGLE_ENT_NAME " removed.\n");
}

// vim:tw=80:ts=4:sw=4:noexpandtab
