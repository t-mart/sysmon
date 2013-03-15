#include "sys_calls.h"

// See arch/x86/kernel/asm-offsets_64.c
//     arch/x86/kernel/sys_calls.c
//     include/asm-x86/unistd_64.h
#define __NO_STUBS

#undef __SYSCALL
#define __SYSCALL(nr, sym) [ nr ] = { .sym_name = #sym, .sys_num = nr, .monitor = 0 }, 
#undef _ASM_X86_64_UNISTD_H_

struct sys_call_id_t sys_call_table[SYSCALL_MAX+1] = {
   [0 ... SYSCALL_MAX] = { .sym_name = "ERROR: invalid sys call index", .sys_num = -1, .monitor = 0 },
   #include <asm/unistd_64.h>
};

// Calls to monitor
const long sys_call_monitor[] = {
__NR_access, __NR_brk, __NR_chdir, __NR_chmod, __NR_clone, __NR_close, __NR_dup, __NR_dup2, __NR_execve, __NR_exit_group, __NR_fcntl, __NR_fork, __NR_getdents, __NR_getpid, __NR_gettid, __NR_ioctl, __NR_lseek, __NR_mkdir, __NR_mmap, __NR_munmap, __NR_open, __NR_pipe, __NR_read, __NR_rmdir, __NR_select, __NR_stat, __NR_fstat, __NR_lstat, __NR_wait4, __NR_write
};
