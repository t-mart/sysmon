#include "sysmon.h"
#include "proc.h"
#include "uid.h"

int uid;

static int uid_read(char *buffer, char **start, off_t offset,
					int count, int *peof, void *dat)
{
	int ret;

	INFO_PRINT("reading /proc/" ENT_NAME "...");

	if(offset > 0) {
		ret = 0;
	} else {
		ret = sprintf(buffer, "%u\n", uid) + 1;
	}

	INFO_PRINT(" uid -> %u\n", uid);

	return ret;
}

static int uid_write(struct file *file, const char *buffer,
					 unsigned long count, void *data)
{
	unsigned long size = 1024;
	char uid_buf[size];

	int len = min(count, size);

	INFO_PRINT("writing into /proc/" ENT_NAME "...");

	if(copy_from_user(uid_buf, buffer, len)) {
		INFO_PRINT("failed to copy from user\n");
		return -EFAULT;
	}

	sscanf(uid_buf, "%u", &uid);

	INFO_PRINT(" uid <- %u\n", uid);

	return len;
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

	uid = -1;

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
