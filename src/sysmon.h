#ifndef SYSMON_H
#define SYSMON_H

#include <linux/kernel.h>

#define MODULE_NAME "sysmon"
#define LOG_ID "[" MODULE_NAME "] "

#define INFO_PRINT(fmt,...) printk(KERN_INFO LOG_ID fmt, ##__VA_ARGS__)
#define ERR_PRINT(fmt,...) printk(KERN_ERR LOG_ID fmt, ##__VA_ARGS__)

#define SYSMON_DEBUG
#ifdef SYSMON_DEBUG
#define DEBUG_PRINT(fmt,...) printk(KERN_DEBUG LOG_ID fmt, ##__VA_ARGS__)
#endif

#endif

// vim:tw=80:ts=4:sw=4:noexpandtab
