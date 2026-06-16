#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/version.h>

#define PROC_MPU_NAME "mc_sleep"
#define PROC_MPU_MODE 0644
#define MC_SLEEP_CPUID 1

extern int cpu_device_down(struct device *dev);

struct reg_info_t {
        unsigned int reg_addr;
        unsigned int regv;
        unsigned int mask;
};

typedef struct {
    const char *label;
    unsigned int *cfg;
} sleep_cfg_t;

unsigned int prg1_cfg[] = 
{
    #include "prg1_sleep.h"
};

unsigned int prg2_cfg[] = 
{
    #include "prg2_sleep.h"
};

unsigned int deep_cfg[] = 
{
    #include "deep_sleep.h"
};

static sleep_cfg_t sleep_configs[] = {
    {"deep", deep_cfg},
    {"prg1", prg1_cfg},
    {"prg2", prg2_cfg},
};

int ca7_enter_wfi(void) {
	struct device *mc_sleep_dev;
	int result;
	mc_sleep_dev = get_cpu_device(MC_SLEEP_CPUID);

	result = cpu_device_down(mc_sleep_dev);
	if (result) {
		printk(KERN_ERR "Failed to offline CPU %d,fail to sleep\r\n", MC_SLEEP_CPUID);
		return -1;
	} else {
		printk(KERN_INFO "Successfully offlined CPU %d\r\n", MC_SLEEP_CPUID);
	}
	local_irq_disable();
	local_fiq_disable();
	return 0;
}
void sleep_write2device(unsigned int phys_addr,unsigned int mask,unsigned int regv)
{
    void __iomem *mapped_addr;
	unsigned int reg_val;
    mapped_addr = ioremap(phys_addr, 0x1000); 
    if (!mapped_addr) {
        printk(KERN_ERR "Failed to remap IO memory\n");
        return;
    }
	reg_val = ioread32(mapped_addr);
	reg_val &= ~mask;
	reg_val |= regv;
    iowrite32(reg_val, mapped_addr);	
    iounmap(mapped_addr);
}

void lp_init(unsigned int *cfg)
{
    unsigned int i;
    unsigned int len;
    struct reg_info_t *reginfo;

    len = cfg[0];
    reginfo = (struct reg_info_t*)(&cfg[1]);

    for(i=0; i<len; i++) {
        if((reginfo[i].reg_addr & 0x3) == 0x0) {
            sleep_write2device(reginfo[i].reg_addr, reginfo[i].mask, reginfo[i].regv);
        }
    }
}

static ssize_t sleep_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
    char *kbuf;
	size_t i, len = sizeof(sleep_configs) / sizeof(sleep_configs[0]);

    kbuf = kmalloc(count + 1, GFP_KERNEL);
    if (!kbuf) {
        return -ENOMEM;
    }

    if (copy_from_user(kbuf, buf, count)) {
        kfree(kbuf);
        return -EFAULT;
    }
    kbuf[count] = '\0';
	strim(kbuf);

	for(i=0;i<len ;i++) {
		if(strcmp(kbuf, sleep_configs[i].label) == 0) {
			if(!ca7_enter_wfi()) {
				printk("%s sleep start\r\n",sleep_configs[i].label);
				lp_init(sleep_configs[i].cfg);
			}
			break;
		}
		if(i==(len-1)) {
			printk("not find input in lables\r\n");
			kfree(kbuf);
			return -EFAULT;
		}
	}
    kfree(kbuf);
    return count;	
}

static char labels_string[256];
static int labels_len = 0;
void build_labels_string(void) {
    int i;
    for (i = 0; i < sizeof(sleep_configs) / sizeof(sleep_cfg_t); i++) {
        if (i > 0) {
            strcat(labels_string, "\r\n");
        }
        strcat(labels_string, sleep_configs[i].label);
    }
	strcat(labels_string, "\r\n");
    labels_len = strlen(labels_string);
}

static ssize_t sleep_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    int len;
	len = labels_len;

    if (*pos >= len) {
        return 0;
    }

	if (count >= len) {
		if (copy_to_user(buf, labels_string, len)) {
		    return -EFAULT;
		}
		*pos += len;
		return len;
	} else {
		return -ENOMEM; 
	}
}

static const struct proc_ops proc_sleep_fops = {
	.proc_read = sleep_read,
    .proc_write = sleep_write,
};

static int __init mc_sleep_init(void)
{
	struct proc_dir_entry *mpu_entry;
	printk(KERN_DEBUG"mc_sleep_init\r\n");

    mpu_entry = proc_create(PROC_MPU_NAME, PROC_MPU_MODE, NULL, &proc_sleep_fops);
    if (mpu_entry == NULL) {
        return -ENOMEM;
    }
	build_labels_string();
    return 0;
}

static void __exit mc_sleep_exit(void)
{
	 printk(KERN_DEBUG"mc_sleep_exit\r\n");
}

module_init(mc_sleep_init);
module_exit(mc_sleep_exit);

MODULE_AUTHOR("martin.liu");
MODULE_DESCRIPTION("Misc Device Driver for mc_sleep_debug ");
MODULE_LICENSE("GPL v2");
