#if defined(__KERNEL__)
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/iommu.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/firmware.h>
#include <linux/kobject.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/proc_fs.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/sizes.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_reserved_mem.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/random.h>
#include <linux/semaphore.h>
#else
#include "fy_osal.h"
#endif
#include "data_type.h"
/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define OS_LINUX 1
#else
#define OS_RTT 1
#endif

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/
#if defined(OS_RTT)
fhosal_dev_t s_lcm_module_dev = {0x00};
#endif

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/
static int mc_lcm_module_probe(void)
{
    printf("--[LCM_Module]Version:[%s, %s]--\n",__DATE__, __TIME__);

    //kfree(gHandle->reg_base);
    //kfree(gHandle);

    //printk("mc_lcm_module_probe\n");

    return 0;
}

static int mc_lcm_module_remove(void)
{
    //kfree(gHandle->reg_base);
    //kfree(gHandle);

    //printk("mc_lcm_module_remove\n");

    return 0;
}

#if defined(OS_LINUX)
static int _mc_lcm_module_open(struct inode* inode, struct file *file)
{
    int s32Ret = 0;

    return s32Ret;
}

static int _mc_lcm_module_close(struct inode* inode, struct file *file)
{
    int s32Ret = 0;

    return s32Ret;
}

static int _mc_lcm_module_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int s32Ret = 0;

    return s32Ret;
}

static long _mc_lcm_module_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int s32Ret = 0;

    return s32Ret;
}

#elif defined(OS_RTT)
static int _mc_lcm_module_open(void * inode, void *file)
{
    int s32Ret = 0;

    return s32Ret;
}

static int _mc_lcm_module_close(void * inode, void *file)
{
    int s32Ret = 0;

    return s32Ret;
}

static long _mc_lcm_module_ioctl(void *file, unsigned int cmd, unsigned long arg)
{
    int s32Ret = 0;

    return s32Ret;
}

#endif

#if defined(OS_LINUX)
static struct file_operations s_lcmModuleFileOp = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = _mc_lcm_module_ioctl,
    .open           = _mc_lcm_module_open,
    .release        = _mc_lcm_module_close,
    .read           = _mc_lcm_module_read,
};

static struct miscdevice lcm_module_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = "mc_lcm_module",
    .fops  = &s_lcmModuleFileOp,
};
#elif defined(OS_RTT)
static struct fhosal_fileops s_stLcmModuleFileOp = {
	.unlocked_ioctl = _mc_lcm_module_ioctl,
	.open           = _mc_lcm_module_open,
	.release        = _mc_lcm_module_close,
//	.read           = DRVI_VO_Read,
};
#endif

int mc_lcm_module_init(void)
{
    int rtn = 0;

    rtn = mc_lcm_module_probe();

    #if defined(OS_LINUX)
    rtn = fhosal_misc_register(&lcm_module_miscdev);
    #elif defined(OS_RTT)
    fhosal_memcpy(s_lcm_module_dev.name, "lcm_module", 10);
    s_lcm_module_dev.fops = &s_stLcmModuleFileOp;
    rtn = fhosal_misc_register(&s_lcm_module_dev);
    #endif

    EXIT_IF_FAIL(rtn, NULL, "lcm module register ~_~!");

    EXIT:

    return rtn;
}

void mc_lcm_module_exit(void)
{

    mc_lcm_module_remove();

    #if defined(OS_LINUX)
    fhosal_misc_deregister(&lcm_module_miscdev);
    #elif defined(OS_RTT)
    fhosal_memset(&s_lcm_module_dev,0,sizeof(s_lcm_module_dev));
    fhosal_memcpy(s_lcm_module_dev.name, "lcm_module", 3);
    s_lcm_module_dev.fops = &s_stLcmModuleFileOp;
    fhosal_misc_deregister(&s_lcm_module_dev);
    #endif
}

#if defined(OS_LINUX)
module_init(mc_lcm_module_init);
module_exit(mc_lcm_module_exit);
//module_platform_driver(mc_lcm_driver);
//MODULE_LICENSE("Dual BSD/GPL");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Molchip Lcm Module driver");
MODULE_ALIAS("lcm module");
#else
INIT_COMPONENT_EXPORT(mc_lcm_module_init);
#endif

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
