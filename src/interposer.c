#include <linux/kprobes.h>

#include "sysmon.h"
#include "interposer.h"
#include "uid.h"
#include "buffer.h"
#include "sys_calls.h"

static struct kprobe probe[SYSCALL_MAX+1];

/* pt_regs defined in include/asm-x86/ptrace.h
 *
 * For information associating registers with function arguments, see:
 * http://lxr.linux.no/linux+v2.6.24.6/arch/x86/kernel/entry_64.S#L182
 */
static int sysmon_intercept_before(struct kprobe *kp, struct pt_regs *regs)
{
	int ret = 0;
	char *sys_call = sys_call_table[regs->rax].sym_name;

	if (current->uid != uid)
		return 0;

	DEBUG_PRINT(
		/* sycall pid tid args.. */
		"sysmon intercepted '%s'\n"
		"%lu %d %d args (%lu, %lu, %lu, %lu, %lu, %lu)\n",
		sys_call,
		regs->rax, current->pid, current->tgid, 
		regs->rdi, regs->rsi, regs->rdx, regs->r10, regs->r8, regs->r9);

	if (sysmon_buffer_write(regs) != -1)
		INFO_PRINT("wrote new log_entry about %s\n", sys_call);
	else
		INFO_PRINT("couldn't write\n");

	return ret;
}


int start_interposer(void)
{
	int i;
	INFO_PRINT("setting up interposer...\n");
	for(i=0; i <= SYSCALL_MAX; ++i) {
		if(sys_call_table[i].sys_num == -1)
			continue; // invalid probe

		probe[i].symbol_name = sys_call_table[i].sym_name;
		probe[i].pre_handler = sysmon_intercept_before; /* called prior to function */

		if (register_kprobe(&probe[i])) {
			ERR_PRINT("register_kprobe failed on '%s'!\n", sys_call_table[i].sym_name);
			return -EFAULT;
		}
	}
	INFO_PRINT("interposer set up.\n");
	return 0;
}

void stop_interposer(void)
{
	int i;
	INFO_PRINT("tearing down interposer...\n");
	for(i=0; i <= SYSCALL_MAX; ++i) {
		if(sys_call_table[i].sys_num == -1)
			continue; // invalid probe
		unregister_kprobe(&probe[i]);
	}
	INFO_PRINT("interposer torn down.\n");
}

#include <linux/module.h>
MODULE_LICENSE("GPL");

// vim:tw=80:ts=4:sw=4:noexpandtab
