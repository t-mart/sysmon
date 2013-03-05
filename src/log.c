#include "sysmon.h"
#include "proc.h"
#include "log.h"

static int log_read(char *buffer, char **start, off_t offset,
					int count, int *peof, void *dat)
{
	return 0;
}

static int log_write(struct file *file, const char *buffer,
					 unsigned long count, void *data)
{
	return 0;
}

int start_log(void)
{
	mode_t mode = S_IFREG | S_IRUSR;
	struct proc_dir_entry *log_ent;

	INFO_PRINT("creating /proc/" ENT_NAME "...");

	log_ent = create_proc_entry(ENT_NAME, mode, NULL);

	if (log_ent == NULL) {
		ERR_PRINT("creation of /proc/" ENT_NAME " failed!\n");
		return -EFAULT;
	}

	log_ent->read_proc  = log_read;
	log_ent->write_proc = log_write;
	log_ent->owner      = THIS_MODULE;
	log_ent->uid        = 0; //root
	log_ent->gid        = 0; //root

	INFO_PRINT("done\n");

	return 0;
}

void stop_log(void)
{
	INFO_PRINT("removing /proc" ENT_NAME "...");
	remove_proc_entry(ENT_NAME, &proc_root);
	INFO_PRINT("done\n");
}

// vim:tw=80:ts=4:sw=4:noexpandtab
