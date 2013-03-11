#include <linux/kprobes.h>

#include "sysmon.h"
#include "interposer.h"
#include "uid.h"

static struct kprobe probe;

/* pt_regs defined in include/asm-x86/ptrace.h
 *
 * For information associating registers with function arguments, see:
 * http://lxr.linux.no/linux+v2.6.24.6/arch/x86/kernel/entry_64.S#L182
 */
static int sysmon_intercept_before(struct kprobe *kp, struct pt_regs *regs)
{
	int ret = 0;
	if (current->uid != uid)
		return 0;
	switch (regs->rax) {
		case __NR_mkdir:
			DEBUG_PRINT(
				   /* sycall pid tid args.. */
				   "%lu %d %d args 0x%lu '%s' %d\n",
				   regs->rax, current->pid, current->tgid, 
				   (uintptr_t)regs->rdi, (char*)regs->rdi, (int)regs->rsi);
			break;
		default:
			ret = -1;
			break;
	}
	return ret;
}


int start_interposer(void)
{
	INFO_PRINT("setting up interposer...");

	probe.symbol_name = "sys_mkdir";
	probe.pre_handler = sysmon_intercept_before; /* called prior to function */
	if (register_kprobe(&probe)) {
		ERR_PRINT("register_kprobe failed\n");
		return -EFAULT;
	}
	INFO_PRINT("done\n");
	return 0;
}

void stop_interposer(void)
{
	INFO_PRINT("tearing down interposer...");
	unregister_kprobe(&probe);
	INFO_PRINT("done\n");
}

// vim:tw=80:ts=4:sw=4:noexpandtab
