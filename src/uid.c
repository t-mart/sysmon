#include "sysmon.h"
#include "proc.h"
#include "uid.h"

static int uid_read(char *buffer, char **start, off_t offset,
					int count, int *peof, void *dat)
{
	return 0;
}

static int uid_write(struct file *file, const char *buffer,
					 unsigned long count, void *data)
{
	return 0;
}

int start_uid(void)
{
	mode_t mode = S_IFREG | S_IRUSR | S_IWUSR;
	struct proc_dir_entry *uid_ent;

	INFO_PRINT("creating /proc/" ENT_NAME "...");

	uid_ent = create_proc_entry(ENT_NAME, mode, NULL);

	if (uid_ent == NULL) {
		ERR_PRINT("creation of /proc/" ENT_NAME " failed!\n");
		return -EFAULT;
	}

	uid_ent->read_proc  = uid_read;
	uid_ent->write_proc = uid_write;
	uid_ent->owner      = THIS_MODULE;
	uid_ent->uid        = 0; //root
	uid_ent->gid        = 0; //root

	INFO_PRINT("done\n");

	return 0;
}

void stop_uid(void)
{
	INFO_PRINT("removing /proc" ENT_NAME "...");
	remove_proc_entry(ENT_NAME, &proc_root);
	INFO_PRINT("done\n");
}

// vim:tw=80:ts=4:sw=4:noexpandtab
