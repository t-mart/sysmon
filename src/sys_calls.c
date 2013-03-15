#include "sys_calls.h"

#define __NO_STUBS 1
#undef __syscall
#undef _asm_x86_64_unistd_h
#define __syscall(nr, sym) [nr] = 1
static char num_calls[] = {
#include <asm/unistd.h>
};

const uint32_t __NR_syscall_max = sizeof(num_calls)-1;

#undef __syscall
#undef _asm_x86_64_unistd_h
#define __syscall(nr, sym) [ nr ] = { .sym_name = #sym, .sys_num = nr }, 

const struct sys_call_id_t sys_call_table[__NR_syscall_max+1] = {
   [0 ... __NR_syscall_max] = { .sym_name = "ERROR: invalid sys call", .sys_num = -1 },
#include <asm/unistd_64.h>
};
