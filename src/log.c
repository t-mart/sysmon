#include "sysmon.h"
#include "proc.h"
#include "log.h"
#include "buffer.h"

static int log_read(char *buffer, char **start, off_t offset,
					int count, int *peof, void *dat)
{
	int ret;
	if(offset > 0) {
		ret = 0;
	} else {
		sysmon_buffer_read(buffer, count, 1);
		ret = count;
	}
	return ret;
}

static int log_write(struct file *file, const char *buffer,
					 unsigned long count, void *data)
{
	return 0;
}

static int log_open(struct inode *inode, struct *file)
{
	return seq_open(file, &sysmon_seq_ops);
}

int start_log(void)
{
	mode_t mode = S_IFREG | S_IRUSR;
	struct proc_dir_entry *log_ent;

	INFO_PRINT("creating /proc/" LOG_ENT_NAME "...\n");

	log_ent = create_proc_entry(LOG_ENT_NAME, mode, NULL);

	if (log_ent == NULL) {
		ERR_PRINT("creation of /proc/" LOG_ENT_NAME " failed!\n");
		return -EFAULT;
	}

	log_ent->owner   = THIS_MODULE;
	log_ent->open    = sysmon_log_open;
	log_ent->read    = seq_read;
	log_ent->llseek  = seq_lseek;
	log_ent->release = seq_release;
	log_ent->uid     = 0; //root
	log_ent->gid     = 0; //root

	INFO_PRINT("/proc/" LOG_ENT_NAME " created.\n");

	return 0;
}

void stop_log(void)
{
	INFO_PRINT("removing /proc/" LOG_ENT_NAME "...\n");
	remove_proc_entry(LOG_ENT_NAME, &proc_root);
	INFO_PRINT("/proc/" LOG_ENT_NAME " removed.\n");
}

// vim:tw=80:ts=4:sw=4:noexpandtab
