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

	char *sys_call;
	switch (le->sys_call_n) {
		case __NR_access:
			sys_call = "access";
			break;
		case __NR_brk:
			sys_call = "brk";
			break;
		case __NR_chdir:
			sys_call = "chdir";
			break;
		case __NR_chmod:
			sys_call = "chmod";
			break;
		case __NR_clone:
			sys_call = "clone";
			break;
		case __NR_close:
			sys_call = "close";
			break;
		case __NR_dup:
			sys_call = "dup";
			break;
		case __NR_dup2:
			sys_call = "dup2";
			break;
		case __NR_execve:
			sys_call = "execve";
			break;
		case __NR_exit_group:
			sys_call = "exit_group";
			break;
		case __NR_fcntl:
			sys_call = "fcntl";
			break;
		case __NR_fork:
			sys_call = "fork";
			break;
		case __NR_getdents:
			sys_call = "getdents";
			break;
		case __NR_getpid:
			sys_call = "getpid";
			break;
		case __NR_gettid:
			sys_call = "gettid";
			break;
		case __NR_ioctl:
			sys_call = "ioctl";
			break;
		case __NR_lseek:
			sys_call = "lseek";
			break;
		case __NR_mkdir:
			sys_call = "mkdir";
			break;
		case __NR_mmap:
			sys_call = "mmap";
			break;
		case __NR_munmap:
			sys_call = "munmap";
			break;
		case __NR_open:
			sys_call = "open";
			break;
		case __NR_pipe:
			sys_call = "pipe";
			break;
		case __NR_read:
			sys_call = "read";
			break;
		case __NR_rmdir:
			sys_call = "rmdir";
			break;
		case __NR_select:
			sys_call = "select";
			break;
		case __NR_stat:
			sys_call = "stat";
			break;
		case __NR_fstat:
			sys_call = "fstat";
			break;
		case __NR_lstat:
			sys_call = "lstat";
			break;
		case __NR_wait4:
			sys_call = "wait4";
			break;
		case __NR_write:
			sys_call = "write";
			break;
		default:
			break;
	}

	seq_printf(s, "syscall: %s, pid: %d, tgid: %d, sys call: %lu, args: (%lu, %lu, %lu, %lu, %lu, %lu)\n",
			   sys_call,
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
	le->sys_call_n = regs->rax;
	le->arg0       = regs->rdi;
	le->arg1       = regs->rsi;
	le->arg2       = regs->rdx;
	le->arg3       = regs->r10;
	le->arg4       = regs->r8;
	le->arg5       = regs->r9;

	log_entries++;
	log_end = IN_BUF(next + 1);

	spin_unlock(&logbuf_lock);
	preempt_enable();

	//return the index we just wrote to
	return next;
}

// vim:tw=80:ts=4:sw=4:noexpandtab
