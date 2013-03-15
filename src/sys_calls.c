#include "sys_calls.h"


// 
// See arch/x86/kernel/asm-offsets_64.c
//     arch/x86/kernel/sys_calls.c
//     include/asm-x86/unistd_64.h
#define DEFINE(sym, val) \
        asm volatile("\n->" #sym " %0 " #val : : "i" (val))

#define __NO_STUBS 1
#undef __syscall
#undef _asm_x86_64_unistd_h
#define __syscall(nr, sym) [nr] = 1
static char num_calls[] = {
#include <asm/unistd.h>
};

//DEFINE(__NR_syscall_max, sizeof(num_calls)-1);
// Just hardcode it
#define __NR_syscall_max 285

#undef __syscall
#undef _asm_x86_64_unistd_h
#define __syscall(nr, sym) [ nr ] = { .sym_name = #sym, .sys_num = nr }, 

const struct sys_call_id_t sys_call_table[__NR_syscall_max+1] = {
   [0 ... __NR_syscall_max] = { .sym_name = "ERROR: invalid sys call", .sys_num = -1 },
#include <asm/unistd_64.h>
};
