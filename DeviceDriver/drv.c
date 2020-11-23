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
#include <linux/moduleparam.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>

#include <linux/interrupt.h>
#include <linux/delay.h>

#include "drv.h"
#include "fops.h"

#define DEF_KBD_IRQ_LINE    1

static int irq = DEF_KBD_IRQ_LINE;  /* Default PS2 Keyboard interrupt line */
static unsigned int kicdev_major = 0; /* Major number for device */
static unsigned int kicdev_minor = 0; /* Minor number for device */
static struct kicdev_t *kicdev_data = NULL; /* Module data */
static struct class *kicdev_class;
long irq_counter = DEFAULT_IRQ_COUNT; /* Interrup counter - since module loading */
struct timeval irq_reset_time; /* Intrrupt last reset time */

/* static struct cdev inside module data: kicdev_data->cdev */

struct file_operations kicdev_fops = {
    .open =             kicdev_open,
    .release =          kicdev_release,
    .read =             kicdev_read,
    .write =            kicdev_write,
    .unlocked_ioctl =   kicdev_ioctl,
};

/* INFO: to display description of parameters supported by module use following command: modinfo kicdev.ko */
/* INFO: to load module with changed irq line numer use following command: sudo insmod kicdrv.ko irq=<requested line number> */
/* INFO: to read irq parameter - go to: /sys/module/kicdev/parameters/irq */
module_param(irq, int, S_IRUGO);
MODULE_PARM_DESC(irq,"PS2 keyboard - interrupt line number");

static irqreturn_t kicdev_irq_handler(int irq, void *dev)
{
        PDEBUG("IRQ count:%ld\n", irq_counter++);
        return IRQ_NONE; /* Shered irq line - only for listening */
}
/* registering irq */
static int __init kicdev_init(void)
{
    dev_t devno;
    int err = 0;
    struct device *kicdev_device;

    printk(KERN_WARNING MODULE_NAME ": is loaded\n");

	kicdev_data = kmalloc(  sizeof(struct kicdev_t), 
	                        GFP_KERNEL);
	if (!kicdev_data) {
		PDEBUG("Cannot alloc memory!\n");
		return -ENOMEM;
	}

	memset( kicdev_data, 0, 
	        sizeof(struct kicdev_t));

    /* Get a range of minor numbers (starting with 0) to work with */
    err = alloc_chrdev_region(  &devno, 
                                kicdev_minor, 
                                KIC_DEV_NR, 
                                MODULE_NAME);
    if (err < 0) {
        PDEBUG("Can't get major number\n");
        goto err_char_reg;
    }
    kicdev_major = MAJOR(devno);
    kicdev_minor = MINOR(devno);
    devno = MKDEV(kicdev_major, kicdev_minor);
    PDEBUG("major number = %d, minor number = %d\n",kicdev_major, kicdev_minor);
       
    /* Create device class, visible in /sys/class */
    kicdev_class = class_create(THIS_MODULE, "kicdev_class");
    if (IS_ERR(kicdev_class)) {
        PDEBUG("Error when creating class\n");
        err = PTR_ERR(kicdev_class);
        goto err_class_crt;
    } 

    /* Initialize the char device and tie a file_operations to it */
    cdev_init(&kicdev_data->cdev, &kicdev_fops);
    kicdev_data->cdev.owner = THIS_MODULE;

    /* Now make the device live for the users to access */
    err = cdev_add( &kicdev_data->cdev, 
                    devno, 
                    KIC_DEV_NR);
	if (err) {
		PDEBUG("Error when adding char dev\n");
		goto err_cdev_add;
	}
    
    /* Create a device node at /dev/kicdev0
    *  With class used here, device can also be viewed under /sys/class/kicdev. */
    kicdev_device = device_create(  kicdev_class,
                                    NULL,   /* no parent device */
                                    devno,  /* associated dev_t */
                                    NULL,   /* no additional data */
                                    MODULE_NAME "%d", 0);  /* device name */

    if (IS_ERR(kicdev_device)) {
        PDEBUG("Error when creating device\n");
        err = PTR_ERR(kicdev_device);
        goto err_dev_crt;
    }

    
    err = request_irq( irq, 
                        kicdev_irq_handler, 
                        IRQF_SHARED, 
                        KBUILD_MODNAME, 
                        (void*)kicdev_irq_handler);
	if (err) {
		PDEBUG("Error when registering irq\n");
		goto err_req_irq;
	}
	PDEBUG("irq = %d registered\n", irq);
    return 0;

err_req_irq:
    device_destroy(kicdev_class, devno);
err_dev_crt:
    cdev_del(&kicdev_data->cdev);
err_cdev_add:
    class_destroy(kicdev_class);
err_class_crt:
    unregister_chrdev_region(devno, KIC_DEV_NR);
err_char_reg:
	kfree(kicdev_data);
	return err;
}

static void kicdev_exit(void)
{
    dev_t devno;
    devno = MKDEV(kicdev_major, kicdev_minor);
    
    synchronize_irq(irq); /* Synchronize interrupt */
    free_irq(irq, (void *)(kicdev_irq_handler));
    
    device_destroy(kicdev_class, devno);
    cdev_del(&kicdev_data->cdev);
    class_destroy(kicdev_class);
    unregister_chrdev_region(devno, KIC_DEV_NR);
	kfree(kicdev_data);
    
    printk(KERN_WARNING MODULE_NAME ": unloaded\n");
}

module_init(kicdev_init);
module_exit(kicdev_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Marcin Liżewski <marcin.lizewski(at)gmail.com>");
MODULE_DESCRIPTION("Keyboard Interrupt Counter (KIC) Module");

