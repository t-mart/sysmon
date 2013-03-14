#include <linux/kprobes.h>

#include "sysmon.h"
#include "interposer.h"
#include "uid.h"
#include "buffer.h"

static struct kprobe probe;

/* pt_regs defined in include/asm-x86/ptrace.h
 *
 * For information associating registers with function arguments, see:
 * http://lxr.linux.no/linux+v2.6.24.6/arch/x86/kernel/entry_64.S#L182
 */
static int sysmon_intercept_before(struct kprobe *kp, struct pt_regs *regs)
{
	int ret = 0;
	char *sys_call;
	if (current->uid != uid)
		return 0;

	switch (regs->rax) {
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
			ret = -1;
			break;
	}

	if(ret == 0) {
		DEBUG_PRINT(
			/* sycall pid tid args.. */
			"sysmon intercepted '%s'\n"
			"%lu %d %d args 0x%lu '%s' %d\n",
			sys_call,
			regs->rax, current->pid, current->tgid, 
			(uintptr_t)regs->rdi, (char*)regs->rdi, (int)regs->rsi);

			if (sysmon_buffer_write(regs) != -1)
				DEBUG_PRINT("wrote new log_entry about %s\n", sys_call);
			else
				DEBUG_PRINT("couldn't write\n");
	}

	return ret;
}


int start_interposer(void)
{
	INFO_PRINT("setting up interposer...\n");

	probe.symbol_name = "sys_mkdir";
	probe.pre_handler = sysmon_intercept_before; /* called prior to function */
	if (register_kprobe(&probe)) {
		ERR_PRINT("register_kprobe failed!\n");
		return -EFAULT;
	}
	INFO_PRINT("interposer set up.\n");
	return 0;
}

void stop_interposer(void)
{
	INFO_PRINT("tearing down interposer...\n");
	unregister_kprobe(&probe);
	INFO_PRINT("interposer torn down.\n");
}

#include <linux/module.h>
MODULE_LICENSE("GPL");

// vim:tw=80:ts=4:sw=4:noexpandtab
