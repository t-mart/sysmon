#ifndef _SYS_CALLS_H
#define _SYS_CALLS_H

#define SYSCALL_MAX 285 // max index into sys_call_table

struct sys_call_id_t {
   char *sym_name; // e.g. 'sys_mkdir'
   long sys_num; // e.g. __NR_mkdir
};

extern const struct sys_call_id_t sys_call_table[];

// Table of sys calls to monitor by sys number, e.g. __NR_mkdir
extern const int sys_call_monitor_size;
extern const long sys_call_monitor[];

#endif //_SYS_CALLS_H
