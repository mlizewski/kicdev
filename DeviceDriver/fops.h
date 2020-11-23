#ifndef _FOPS_H
#define _FOPS_H

extern int kicdev_open(struct inode *inode, struct file *filp);
extern int kicdev_release(struct inode *node, struct file *filp);

extern ssize_t kicdev_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
extern ssize_t kicdev_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset);

extern long kicdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

#endif
