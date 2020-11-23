#ifndef _DRV_H
#define _DRV_H

#define PRINTK_LEVEL KERN_DEBUG

#ifndef MODULE_NAME
#define MODULE_NAME		"kicdev"
#endif

#undef PDEBUG
#ifdef ENABLE_DEBUG
#  ifdef __KERNEL__
#    define PDEBUG(format, args...) printk(PRINTK_LEVEL MODULE_NAME ": " format, ##args)
#  else
#    define PDEBUG(format, args...) fprintf(stderr, MODULE_NAME ": " format, ##args)
#  endif
#else
#  define PDEBUG(format, args...)
#endif

#define KIC_DEV_NR		    1

#define KIC_MAXNR		    5

#define DEFAULT_IRQ_COUNT	0
#define DEFAULT_MESSAGE		""
#define BUFF_SIZE		    PAGE_SIZE

#include "ioctl_cmd_def.h"

extern struct timeval irq_reset_time; /* Inside drv.c */
extern long irq_counter; /* Inside drv.c */

struct kicdev_t {
	struct cdev cdev;
	loff_t buf_len;
	char buff[BUFF_SIZE];
};

#endif
