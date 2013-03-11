#ifndef BUFFER_H
#define BUFFER_H

//64KB
#define BUF_LEN (1 << 16)

extern int sysmon_buffer_write(const char *fmt, ...);
extern int sysmon_buffer_read(char *buf, int len, int clear_after_read);

#endif

// vim:tw=80:ts=4:sw=4:noexpandtab
