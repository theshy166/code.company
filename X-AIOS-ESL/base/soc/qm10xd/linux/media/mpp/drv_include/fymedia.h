#ifndef _LINUX_FYMEDIA_DEVICE_H_
#define _LINUX_FYMEDIA_DEVICE_H_

#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/major.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "cconfig.h"
#include "rtdef.h"
#include "rthw.h"
#include "rtthread.h"
#include "rtdbg.h"
//#include "ioremap.h"
//#include "dfs_file.h"
//#include "boot_args.h"
#include "fy_osal.h"
#endif

#ifndef __KERNEL__

#ifndef KERN_EMERG
#define KERN_EMERG ""
#endif
#ifndef KERN_ALERT
#define KERN_ALERT ""
#endif
#ifndef KERN_CRIT
#define KERN_CRIT ""
#endif
#ifndef KERN_ERR
#define KERN_ERR ""
#endif
#ifndef KERN_WARNING
#define KERN_WARNING ""
#endif
#ifndef KERN_NOTICE
#define KERN_NOTICE ""
#endif
#ifndef KERN_INFO
#define KERN_INFO ""
#endif
#ifndef KERN_DEBUG
#define KERN_DEBUG ""
#endif

#endif

#define FY_MEDIA_USE_MISC_DEV

#ifndef  FY_MEDIA_USE_MISC_DEV
#define FYMEDIA_DEVICE_MAJOR  218
#define FYMEDIA_DYNAMIC_MINOR 255

struct fymedia_device;

struct fymedia_ops {
    //pm methos
    int (*pm_prepare)(struct fymedia_device *);
    void (*pm_complete)(struct fymedia_device *);

    int (*pm_suspend)(struct fymedia_device *);
    int (*pm_resume)(struct fymedia_device *);

    int (*pm_freeze)(struct fymedia_device *);
    int (*pm_thaw)(struct fymedia_device *);
    int (*pm_poweroff)(struct fymedia_device *);
    int (*pm_restore)(struct fymedia_device *);

    int (*pm_suspend_late)(struct fymedia_device *);
    int (*pm_resume_early)(struct fymedia_device *);
    int (*pm_freeze_late)(struct fymedia_device *);
    int (*pm_thaw_early)(struct fymedia_device *);
    int (*pm_poweroff_late)(struct fymedia_device *);
    int (*pm_restore_early)(struct fymedia_device *);

    int (*pm_suspend_noirq)(struct fymedia_device *);
    int (*pm_resume_noirq)(struct fymedia_device *);

    int (*pm_freeze_noirq)(struct fymedia_device *);
    int (*pm_thaw_noirq)(struct fymedia_device *);
    int (*pm_poweroff_noirq)(struct fymedia_device *);
    int (*pm_restore_noirq)(struct fymedia_device *);
};
#endif

struct fymedia_driver{
#ifndef  FY_MEDIA_USE_MISC_DEV
    struct device_driver driver;
    struct fymedia_ops *ops;
	char name[1];
#else
#ifdef __KERNEL__
    struct miscdevice  driver;
#else
    struct fhosal_dev  driver;
#endif
#endif
};

#define to_fymedia_driver(drv)	\
    container_of((drv), struct fymedia_driver, driver)

struct fymedia_device  {
#ifdef __KERNEL__
    struct list_head list;
#else
    struct rt_list_node list;
#endif

#define MAX_LEN 32
    char devfs_name[MAX_LEN];

	int minor;

#ifdef __KERNEL__
    struct device device;
    struct module *owner;
#else
    struct rt_device device;
    void *owner;
#endif

	const struct file_operations *fops;

#ifndef  FY_MEDIA_USE_MISC_DEV
	struct fymedia_ops *drvops;
#endif

    /*for internal use*/
	struct fymedia_driver *driver;
};


#define to_fymedia_device(dev) \
    container_of((dev), struct fymedia_device, device)

int fymedia_register(struct fymedia_device *pdev);

int fymedia_unregister(struct fymedia_device *pdev);

#define MODULE_ALIAS_FYMEDIA(minor) \
	MODULE_ALIAS("fymedia-char-major-" __stringify(FYMEDIA_DEVICE_MAJOR) \
	"-" __stringify(minor))

#endif /*_LINUX_FYMEDIA_DEVICE_H_*/
