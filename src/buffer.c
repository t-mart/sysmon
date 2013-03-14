#include <linux/spinlock_types.h>
#include <linux/spinlock.h>
#include <linux/seq_file.h>

#include "buffer.h"

struct log_entry
{
	int pid, tgid;
	unsigned long sys_call_n, arg0, arg1, arg2, arg3, arg4, arg5;
	/*struct pt_regs regs;*/
};

static log_entry sysmon_buffer[BUF_LEN];

static DEFINE_SPINLOCK(logbuf_lock);

static loff_t log_start = 0L; /* Index into log_buf: next char to be read by syslog() */
static loff_t log_end = 0L;   /* Index into log_buf: most-recently-written-char + 1 */

static void *sysmon_seq_start(struct seq_file *s, loff_t *pos)
{
	preempt_disable();

	/*raw_local_irq_save(flags);*/
	/*lockdep_off();*/
	spin_lock(&logbuf_lock);

	return sysmon_buffer + log_start;
}


static void *sysmon_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	loff_t next = log_start + 1;

	if (next >= BUF_LEN)
		next = 0;
	if (next > log_end)
		return NULL;

	log_start = next;

	return sysmon_buffer + log_start;
}

static void *sysmon_seq_stop(struct seq_file *s, void *v)
{
	spin_unlock(&logbuf_lock);
	/*lockdep_on();*/
	/*raw_local_irq_restore(flags);*/

	preempt_enable();
}

static int sysmon_seq_show(struct seq_file *s, void *v)
{
	struct *log_entry le = (struct log_entry *) v;
	
	seq_printf(s, "pid: %d, tgid: %d, sys call: %lu, args: (%lu, %lu, %lu, %lu, %lu, %lu)\n",
			   le->pid, le->tgid,
			   le->sys_call_n,
			   le->arg0, le->arg1, le->arg2, le->arg3, le->arg4, le->arg5);
	return 0;
}

struct seq_operations sysmon_seq_ops = {
	.start = sysmon_seq_start,
	.next  = sysmon_seq_next,
	.stop  = sysmon_seq_stop,
	.show  = sysmon_seq_show
};

int sysmon_buffer_write(struct pt_regs *regs)
{
	struct log_entry le;
	loff_t next = log_end + 1;

	if (next >= BUF_LEN)
		next = 0;
	if (next => log_start)
		return -1;

	le = *(sysmon_buffer + next);
	le = {
		.pid        = current->pid,
		.tgid       = curreng->tgid,
		.sys_call_n = (unsigned long) regs->rax,
		.arg0       = (unsigned long) regs->rdi,
		.arg1       = (unsigned long) regs->rsi,
		.arg2       = (unsigned long) regs->rdx,
		.arg3       = (unsigned long) regs->r10,
		.arg4       = (unsigned long) regs->r8,
		.arg5       = (unsigned long) regs->r9
	};

	log_end = next;
	return log_end;
}

int sysmon_buffer_read(char *buf, int len, int clear_after_read)
{
	unsigned long i, j, limit, count;
	char c;
	int error = 0;

	error = -EINVAL;
	if (!buf || len < 0)
		goto out;
	error = 0;
	if (!len)
		goto out;
	if (!access_ok(VERIFY_WRITE, buf, len)) {
		error = -EFAULT;
		goto out;
	}

	count = len;
	if (count > log_buf_len)
		count = log_buf_len;
	spin_lock_irq(&logbuf_lock);
	if (count > logged_chars)
		count = logged_chars;
	if (clear_after_read)
		logged_chars = 0;
	limit = log_end;

	/*
	 * __put_user() could sleep, and while we sleep
	 * printk() could overwrite the messages
	 * we try to copy to user space. Therefore
	 * the messages are copied in reverse. <manfreds>
	 */
	for (i = 0; i < count && !error; i++) {
		j = limit-1-i;
		if (j + log_buf_len < log_end)
			break;
		c = LOG_BUF(j);
		spin_unlock_irq(&logbuf_lock);

		/*error = __put_user(c,&buf[count-1-i]);*/
		//tim: we're not cpying to userspace here, so do this instead
		&buf[count-1-i] = c;
		error = 0;

		cond_resched();
		spin_lock_irq(&logbuf_lock);
	}

	spin_unlock_irq(&logbuf_lock);
	if (error)
		goto out;
	error = i;
out:
	return error;
}
// vim:tw=80:ts=4:sw=4:noexpandtab
