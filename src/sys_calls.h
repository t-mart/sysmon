#ifndef _SYS_CALLS_H
#define _SYS_CALLS_H

struct sys_call_id_t {
   char *sym_name; // e.g. 'sys_mkdir'
   long sys_num; // e.g. __NR_mkdir
};

extern const struct sys_call_id_t sys_call_table[];

#endif //_SYS_CALLS_H
