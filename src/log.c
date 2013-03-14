#include "sysmon.h"
#include "proc.h"
#include "log.h"
#include "buffer.h"

static int log_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &sysmon_seq_ops);
}

static struct file_operations sysmon_proc_ops = {
	.owner   = THIS_MODULE,
	.open    = log_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release
};

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

	log_ent->proc_fops = &sysmon_proc_ops;
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
