#ifndef _SYS_CALLS_H
#define _SYS_CALLS_H

#define SYSCALL_MAX 285 // max index into sys_call_table

struct sys_call_id_t {
   char *sym_name; // e.g. 'sys_mkdir'
   long sys_num; // e.g. __NR_mkdir
   int monitor; // 0 do not, 1 do monitor
};

extern struct sys_call_id_t sys_call_table[];

// Table of sys calls to monitor by sys number, e.g. __NR_mkdir
#define sys_call_monitor_size 30
extern const long sys_call_monitor[];

#endif //_SYS_CALLS_H
