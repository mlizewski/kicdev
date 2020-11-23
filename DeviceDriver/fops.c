/*  Keyboard Interrupt Counter (KIC) - Character Device Module - kicdev
*   
*   Copyright (C) 2020, Marcin Liżewski
*
*   Released under the GNU General Public License, version 2
*   included herein by reference.
*/
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ktime.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#include "drv.h"
#include "fops.h"

int kicdev_open(struct inode *inode, struct file *filp)
{
	PDEBUG("%s() is invoked\n", __FUNCTION__);

	filp->private_data = container_of(inode->i_cdev, struct kicdev_t, cdev);

	return 0;
}

int kicdev_release(struct inode *inode, struct file *filp)
{
	PDEBUG("%s() is invoked\n", __FUNCTION__);
    PDEBUG("Module released\n");
    
	return 0;
}

ssize_t kicdev_read(struct file *filp, char __user *buff, size_t count,
	loff_t *f_pos)
{
	/* struct kicdev_t *kicdev_data = filp->private_data; */
	
	/* ... */
	
	PDEBUG("%s() is invoked\n", __FUNCTION__);
	PDEBUG("Can't read any data - to implement...\n");
	return count;
}

ssize_t kicdev_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	/* struct kicdev_t *kicdev_data = filp->private_data; */
	
	/* ... */
	
	PDEBUG("%s() is invoked\n", __FUNCTION__);
	PDEBUG("Can't accept any data - to implement...\n");
	return count;
}

long kicdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int err = 0;
	int retval = 0;
	/* struct kicdev_t *kicdev_data = filp->private_data; */

	PDEBUG("%s() is invoked\n", __FUNCTION__);

	if (_IOC_TYPE(cmd) != KIC_IOC_MAGIC) {
		PDEBUG("ioctl command error\n");
		return -ENOTTY;
	}

	if (_IOC_NR(cmd) > KIC_MAXNR) {
		PDEBUG("Number of ioctl parameters error\n");
		return -ENOTTY;
	}
	
	if (_IOC_DIR(cmd) & _IOC_READ)
        #if LINUX_VERSION_CODE <  KERNEL_VERSION(5,0,0)
            err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
        #else
            err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
        #endif
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
        #if LINUX_VERSION_CODE <  KERNEL_VERSION(5,0,0)
            err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
        #else
            err = !access_ok((void __user *)arg, _IOC_SIZE(cmd));
        #endif
	if (err)
		return -EFAULT;
	
	switch (cmd) {
	case KIC_RESET_IRQ_COUNT:
	    /* Get current time of reset */
	    irq_reset_time = ktime_to_timeval(ktime_get_real());
	    /* Irq counter reset */
	    irq_counter = DEFAULT_IRQ_COUNT;
	    PDEBUG("ioctl -> cmd: irq counter reset - Time: %ld \n",irq_reset_time.tv_sec);
		break;
	case KIC_GET_IRQ_COUNT:
	    if (copy_to_user((long*)arg, &irq_counter, sizeof(long*))) {
		    PDEBUG("copy arguments to user error\n");
		    retval = -EFAULT;
		    return retval;
	    }
		PDEBUG("ioctl -> cmd: get irq counter value\n");
		break;
	case KIC_GET_RESET_TIME:
		PDEBUG("ioctl -> cmd: get time of last counter reset\n");
	    if (copy_to_user((struct timeval*)arg, &irq_reset_time.tv_sec, sizeof(struct timeval*))) {
		    PDEBUG("copy arguments to user error\n");
		    retval = -EFAULT;
		    return retval;
	    }
		break;
	default:
		return -ENOTTY;
	}

	return retval;
}


