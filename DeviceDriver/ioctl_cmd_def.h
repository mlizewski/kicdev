#ifndef _IOCTL_CMD_DEF_H
#define _IOCTL_CMD_DEF_H

#define KIC_IOC_MAGIC		    'K'
#define KIC_SEQ_GET_IRQ_COUNT	0x01
#define KIC_SEQ_RESET_IRQ_COUNT 0x02
#define KIC_SEQ_GET_RESET_TIME	0x03

#define KIC_RESET_IRQ_COUNT		_IO(KIC_IOC_MAGIC, KIC_SEQ_RESET_IRQ_COUNT)
#define KIC_GET_IRQ_COUNT		_IOR(KIC_IOC_MAGIC, KIC_SEQ_GET_IRQ_COUNT, int)
#define KIC_GET_RESET_TIME		_IOR(KIC_IOC_MAGIC, KIC_SEQ_GET_RESET_TIME, int)

#endif