#include <linux/spinlock_types.h>
#include <linux/spinlock.h>
#include <linux/seq_file.h>
#include <linux/kprobes.h>

#include "buffer.h"

struct log_entry {
	int pid, tgid;
	unsigned long sys_call_n, arg0, arg1, arg2, arg3, arg4, arg5;
	/*struct pt_regs regs;*/
};

static struct log_entry sysmon_buffer[BUF_LEN];

static DEFINE_SPINLOCK(logbuf_lock);

static loff_t log_start = 0L; //index of first entry
static loff_t log_end = 0L;   //index of last entry + 1
static unsigned long log_entries = 0L; //number of entries

#define IN_BUF(index) ((index) % BUF_LEN)
#define LOG_EMTPY (log_entries == 0L)
#define LOG_FULL (log_entries == BUF_LEN)

static void *sysmon_seq_start(struct seq_file *s, loff_t *pos)
{
	/*preempt_disable();*/

	/*raw_local_irq_save(flags);*/
	/*lockdep_off();*/
	spin_lock(&logbuf_lock);

	//nothing in the log
	if (LOG_EMTPY)
		return NULL;

	return sysmon_buffer + log_start;
}


static void *sysmon_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	loff_t next = IN_BUF(log_start + 1);

	if (LOG_EMTPY)
		return NULL;

	log_start = next;
	log_entries--;

	return sysmon_buffer + log_start;
}

static void sysmon_seq_stop(struct seq_file *s, void *v)
{
	spin_unlock(&logbuf_lock);
	/*lockdep_on();*/
	/*raw_local_irq_restore(flags);*/

	//preempt_enable();
}

static int sysmon_seq_show(struct seq_file *s, void *v)
{
	struct log_entry *le = (struct log_entry *) v;
	
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
	loff_t next = log_end;
	struct log_entry *le;

	preempt_disable();
	spin_lock(&logbuf_lock);

	if (LOG_FULL)
		//no more room. don't write
		return -1;

	le = sysmon_buffer + next;

	le->pid        = current->pid;
	le->tgid       = current->tgid;
	le->sys_call_n = (unsigned long) regs->rax;
	le->arg0       = (unsigned long) regs->rdi;
	le->arg1       = (unsigned long) regs->rsi;
	le->arg2       = (unsigned long) regs->rdx;
	le->arg3       = (unsigned long) regs->r10;
	le->arg4       = (unsigned long) regs->r8;
	le->arg5       = (unsigned long) regs->r9;

	log_entries++;
	log_end = IN_BUF(next + 1);

	spin_unlock(&logbuf_lock);
	preempt_enable();

	//return the index we just wrote to
	return next;
}

// vim:tw=80:ts=4:sw=4:noexpandtab
