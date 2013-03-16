#include <linux/kprobes.h>

#include "sysmon.h"
#include "interposer.h"
#include "uid.h"
#include "buffer.h"
#include "sys_calls.h"
#include "toggle.h"

static struct kprobe probe[SYSCALL_MAX+1];

static void delay_loop(unsigned int loops)
{
	char toy = 0;
	unsigned int i;
	for (i = 0; i < loops; i++)
		toy = ~toy;
}

/* pt_regs defined in include/asm-x86/ptrace.h
 *
 * For information associating registers with function arguments, see:
 * http://lxr.linux.no/linux+v2.6.24.6/arch/x86/kernel/entry_64.S#L182
 */
static int sysmon_intercept_before(struct kprobe *kp, struct pt_regs *regs)
{
	int ret = 0;
	long nr = regs->rax;
	struct sys_call_id_t sys_call;

	if (current->uid != uid)
		return 0;
	if (!sysmon_active)
		return 0;

	if(nr < 0 || SYSCALL_MAX < nr)
		return 0;

	sys_call = sys_call_table[nr];

	//delay_loop(5);

	if (!(sys_call.monitor)) {
		return 0;
	}

	INFO_PRINT(
		/* sycall pid tid args.. */
		"sysmon intercepted '%s'\n"
		"nr: %lu, pid: %d, tgid: %d, uid: %d\n"
		"args (%lu, %lu, %lu, %lu, %lu, %lu)\n",
		sys_call.sym_name,
	 	regs->rax, current->pid, current->tgid, current->uid,
		regs->rdi, regs->rsi, regs->rdx, regs->r10, regs->r8, regs->r9);

	if (sysmon_buffer_write(regs) != -1)
		INFO_PRINT("wrote new log_entry about %s\n", sys_call.sym_name);
	else
		INFO_PRINT("couldn't write\n");

	return ret;
}

void sysmon_intercept_after(struct kprobe *p, struct pt_regs *regs,
							unsigned long flags)
{
	// Do something?
}

int start_interposer(void)
{
	int i;
	struct sys_call_id_t this_call;
	INFO_PRINT("setting up interposer...\n");
	for(i=0; i <= SYSCALL_MAX; ++i) {
		this_call = sys_call_table[i];
		if(!(this_call.monitor))
			continue; // invalid probe
		// probe every sys_call 
		probe[i].symbol_name = this_call.sym_name;
		probe[i].pre_handler = sysmon_intercept_before; /* called prior to function */

		if (register_kprobe(&probe[i])) {
			ERR_PRINT("register_kprobe failed on '%s'!\n", this_call.sym_name);
			return -EFAULT;
		}
	}

	INFO_PRINT("interposer set up.\n");
	return 0;
}

void stop_interposer(void)
{
	int i;
	struct sys_call_id_t this_call;
	INFO_PRINT("tearing down interposer...\n");
	for(i=0; i <= SYSCALL_MAX; ++i) {
		this_call = sys_call_table[i];
		if (this_call.monitor)
			unregister_kprobe(&probe[i]);
	}
	INFO_PRINT("interposer torn down.\n");
}

#include <linux/module.h>
MODULE_LICENSE("GPL");

// vim:tw=80:ts=4:sw=4:noexpandtab
