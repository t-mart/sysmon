#ifndef _SYS_CALLS_H
#define _SYS_CALLS_H

#define SYSCALL_MAX 285 // max index into sys_call_table

struct sys_call_id_t {
   char *sym_name; // e.g. 'sys_mkdir'
   long sys_num; // e.g. __NR_mkdir
   int monitor; // 0 do not, 1 do monitor
};

extern struct sys_call_id_t sys_call_table[];

#endif //_SYS_CALLS_H
