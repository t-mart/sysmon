#include "sys_calls.h"

// See arch/x86/kernel/asm-offsets_64.c
//     arch/x86/kernel/sys_calls.c
//     include/asm-x86/unistd_64.h
#define __NO_STUBS

// Just hardcode it
#define __NR_syscall_max 285

#undef __SYSCALL
#define __SYSCALL(nr, sym) [ nr ] = { .sym_name = #sym, .sys_num = nr }, 
#undef _ASM_X86_64_UNISTD_H_

const struct sys_call_id_t sys_call_table[__NR_syscall_max+1] = {
   [0 ... __NR_syscall_max] = { .sym_name = "ERROR: invalid sys call index", .sys_num = -1 },
   #include <asm/unistd_64.h>
};
