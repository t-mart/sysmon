#include <linux/spinlock_types.h>
#include <linux/spinlock.h>

#include "buffer.h"

#define LOG_BUF_MASK    (log_buf_len-1)
#define LOG_BUF(idx) (sysmon_buffer[(idx) & LOG_BUF_MASK])

static char sysmon_buffer[BUF_LEN];
static char *sysmon_buffer;
static int log_buf_len = BUF_LEN;

static DEFINE_SPINLOCK(logbuf_lock);

static unsigned long log_start; /* Index into log_buf: next char to be read by syslog() */
static unsigned long con_start; /* Index into log_buf: next char to be sent to consoles */
static unsigned long log_end;   /* Index into log_buf: most-recently-written-char + 1 */

static unsigned long logged_chars; /* Number of chars produced since last read+clear operation */

int sysmon_buffer_write(const char *fmt, ...)
{
	va_list args;
	int r;
	unsigned long flags;
	int printed_len;
	char *p;
	static char tmp_buf[1024];

	va_start(args, fmt);

	preempt_disable();

	raw_local_irq_save(flags);
	lockdep_off();
	spin_lock(&logbuf_lock);

	/* Emit the output into the temporary buffer */
	printed_len = vscnprintf(tmp_buf, sizeof(tmp_buf), fmt, args);

	/*
	 * Copy the output into log_buf.  If the caller didn't provide
	 * appropriate log level tags, we insert them here
	 */
	for (p = tmp_buf; *p; p++) {
		LOG_BUF(log_end) = c;
		log_end++;
		if (log_end - log_start > log_buf_len)
			log_start = log_end - log_buf_len;
		if (log_end - con_start > log_buf_len)
			con_start = log_end - log_buf_len;
		if (logged_chars < log_buf_len)
			logged_chars++;
	}

		spin_unlock(&logbuf_lock);
		lockdep_on();
		raw_local_irq_restore(flags);
	}

	preempt_enable();
	va_end(args);

	return printed_len;
}

int sysmon_buffer_read(char *buf, int len, int clear_after_read)
{
	unsigned long i, j, limit, count;
	char c;
	int error = 0;

	error = -EINVAL;
	if (!buf || len < 0)
		goto out;
	error = 0;
	if (!len)
		goto out;
	if (!access_ok(VERIFY_WRITE, buf, len)) {
		error = -EFAULT;
		goto out;
	}

	count = len;
	if (count > log_buf_len)
		count = log_buf_len;
	spin_lock_irq(&logbuf_lock);
	if (count > logged_chars)
		count = logged_chars;
	if (clear_after_read)
		logged_chars = 0;
	limit = log_end;

	/*
	 * __put_user() could sleep, and while we sleep
	 * printk() could overwrite the messages
	 * we try to copy to user space. Therefore
	 * the messages are copied in reverse. <manfreds>
	 */
	for (i = 0; i < count && !error; i++) {
		j = limit-1-i;
		if (j + log_buf_len < log_end)
			break;
		c = LOG_BUF(j);
		spin_unlock_irq(&logbuf_lock);

		/*error = __put_user(c,&buf[count-1-i]);*/
		//tim: we're not cpying to userspace here, so do this instead
		&buf[count-1-i] = c;
		error = 0;

		cond_resched();
		spin_lock_irq(&logbuf_lock);
	}

	spin_unlock_irq(&logbuf_lock);
	if (error)
		goto out;
	error = i;
out:
	return error;
}
// vim:tw=80:ts=4:sw=4:noexpandtab
