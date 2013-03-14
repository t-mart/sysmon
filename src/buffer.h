#ifndef BUFFER_H
#define BUFFER_H

//64K entries
#define BUF_LEN (1L << 16)

extern int sysmon_buffer_write(struct pt_regs *regs);

extern struct seq_operations sysmon_seq_ops;

#endif

// vim:tw=80:ts=4:sw=4:noexpandtab
