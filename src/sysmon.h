#ifndef SYSMON_H
#define SYSMON_H

#include <linux/kernel.h>

#define MODULE_NAME "sysmon"
#define LOG_ID "[" MODULE_NAME "] "


#define SYSMON_DEBUG
#ifdef SYSMON_DEBUG
#define DEBUG_PRINT(fmt,...) printk(KERN_DEBUG LOG_ID fmt, ##__VA_ARGS__)
#define INFO_PRINT(fmt,...) printk(KERN_INFO LOG_ID fmt, ##__VA_ARGS__)
#define ERR_PRINT(fmt,...) printk(KERN_ERR LOG_ID fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt,...)
#define INFO_PRINT(fmt,...)
#define ERR_PRINT(fmt,...)
#endif

#endif

// vim:tw=80:ts=4:sw=4:noexpandtab
