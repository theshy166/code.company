/*
 *
 *  Copyright (C) 2019  Kui.Wang  <kui.wang@molchip.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/profile.h>
#include <linux/timex.h>
#include <linux/sched.h>
#include <linux/clockchips.h>
#include <linux/platform_device.h>
#include <linux/smp.h>
#include <linux/reboot.h>
#include <linux/uaccess.h>
#include <asm/system_misc.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>

static volatile unsigned int clr_timer_cnt;

struct perfmon_info_t {
	unsigned int cfg_regs[10];
	unsigned int rtrans_cnt;
	unsigned int rbw_cnt;
	unsigned int rlatency_cnt;
	unsigned int wtrans_cnt;
	unsigned int wbw_cnt;
	unsigned int wlatency_cnt;
	unsigned int bw_current_max;
	unsigned int rbw_cnt_hig;
	unsigned int wbw_cnt_hig;
};
static struct mc_perfmon *perfmon;
#define DDR_MAX_CHN	8
struct mc_perfmon {
	void __iomem            *regs[DDR_MAX_CHN];
	struct device           *dev;
	spinlock_t              io_lock;
	struct perfmon_info_t   *perf_infos[DDR_MAX_CHN];
};

static ssize_t show_reg(struct device *dev,
                struct device_attribute *attr, char *buf)
{
	unsigned int i;

	for(i=0;i<DDR_MAX_CHN;i++) {
		perfmon->perf_infos[i]->cfg_regs[0] |= (0x1 << 11);
		printk("CH[%d] RL : 0x%08x\n",i,perfmon->perf_infos[i]->rbw_cnt);
		printk("CH[%d] RH : 0x%08x\n",i,perfmon->perf_infos[i]->rbw_cnt_hig);
		printk("CH[%d] WL : 0x%08x\n",i,perfmon->perf_infos[i]->wbw_cnt);
		printk("CH[%d] WH : 0x%08x\n",i,perfmon->perf_infos[i]->wbw_cnt_hig);
		printk("CH[%d] RD : 0x%08x\n",i,perfmon->perf_infos[i]->rlatency_cnt);
		printk("CH[%d] WD : 0x%08x\n",i,perfmon->perf_infos[i]->wlatency_cnt);
	}

        return 0;
}

static ssize_t store_reg(struct device *dev,
                struct device_attribute *attr,
                const char *buf, size_t len)
{
	printk("echo cmd\n");
	return len;
}
static DEVICE_ATTR(perfmon, S_IWUSR|S_IRUSR, show_reg, store_reg);

static struct of_device_id mc_perfmon_of_match[] = {
        { .compatible = "mc,perfmon", },
        { /* end of table */ }
};
MODULE_DEVICE_TABLE(of, mc_perfmon_of_match);

static unsigned int dfimon_time_ctrl_offset_reg ,dfimon_high_time_ctrl_offset_reg,dfimon_low_time_ctrl_offset_reg;
struct regmap *dmc_apb_base;
static int mc_perfmon_probe(struct platform_device *pdev)
{
	static struct kobject *mc_perf_kobj;
	struct resource *res;
	unsigned int i, reg_val, ret;

	struct device_node *node = of_find_compatible_node(NULL, NULL,
							"mc,perfmon");
	if (!node) {
		pr_err("failed to find node ");
	}

	dmc_apb_base = syscon_regmap_lookup_by_phandle(node,  "molchip,syscon");
	if (IS_ERR(dmc_apb_base)) {
		pr_err("failed to map dmc_apb_base \n");
		return -1;
	}

	ret = of_property_read_u32(node, "dfimon-time-ctrl-offset-reg",
						&dfimon_time_ctrl_offset_reg);
	if (WARN_ON(ret)){
		pr_err("failed to map dfimon-time-ctrl-offset-reg \n");
		return -1;
	}

	//high time
	ret = of_property_read_u32(node, "dfimon-time-high-ctrl-offset-reg",
						&dfimon_high_time_ctrl_offset_reg);
	if (WARN_ON(ret)){
		pr_err("failed to map dfimon-time-high-ctrl-offset-reg \n");
		return -1;
	}

	//low time
	ret = of_property_read_u32(node, "dfimon-time-low-ctrl-offset-reg",
							&dfimon_low_time_ctrl_offset_reg);
	if (WARN_ON(ret)){
		pr_err("failed to map dfimon-time-high-ctrl-offset-reg \n");
		return -1;
	}

	regmap_update_bits(dmc_apb_base, dfimon_time_ctrl_offset_reg,
			(0x3), (0x3));
	regmap_update_bits(dmc_apb_base, dfimon_time_ctrl_offset_reg,
			(0x3), (1));

	//high time
	regmap_update_bits(dmc_apb_base, dfimon_high_time_ctrl_offset_reg,
			(0xFFFFFFFF), (0xFFFFFFFF));

	//low time
	regmap_update_bits(dmc_apb_base, dfimon_low_time_ctrl_offset_reg,
			(0xFFFFFFFF), (0x1));
	perfmon = devm_kzalloc(&pdev->dev, sizeof(struct mc_perfmon), GFP_KERNEL);
	if (!perfmon)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	for(i=0;i<DDR_MAX_CHN;i++) {
		perfmon->regs[i] = ioremap(res->start + (i << 16),res->end - res->start);
		if(IS_ERR(perfmon->regs[i]))
			return PTR_ERR(perfmon->regs[i]);
		perfmon->perf_infos[i] = (struct perfmon_info_t*)perfmon->regs[i];

		reg_val = perfmon->perf_infos[i]->cfg_regs[0];
		reg_val |= ((0x1 << 31) | (0x7 << 20) | (0x1FF) | (0x1 << 10));
		perfmon->perf_infos[i]->cfg_regs[0] = reg_val;

		reg_val = perfmon->perf_infos[i]->cfg_regs[1];
		reg_val |= ((0x7 << 5) | (0xF));
		perfmon->perf_infos[i]->cfg_regs[1] = reg_val;

		reg_val = perfmon->perf_infos[i]->cfg_regs[2];
		reg_val |= 0xF;
		perfmon->perf_infos[i]->cfg_regs[2] = reg_val;

		reg_val = perfmon->perf_infos[i]->cfg_regs[3];
		reg_val |= 0x1;
		perfmon->perf_infos[i]->cfg_regs[3] = reg_val;

		reg_val = perfmon->perf_infos[i]->cfg_regs[4];
		reg_val |= 0x1;
		perfmon->perf_infos[i]->cfg_regs[4] = reg_val;

		reg_val = perfmon->perf_infos[i]->cfg_regs[5];
		reg_val |= 0x1;
		perfmon->perf_infos[i]->cfg_regs[5] = reg_val;

		reg_val = perfmon->perf_infos[i]->cfg_regs[6];
		reg_val |= 0x1;
		perfmon->perf_infos[i]->cfg_regs[6] = reg_val;

		reg_val = perfmon->perf_infos[i]->cfg_regs[7];
		reg_val |= 0x1;
		perfmon->perf_infos[i]->cfg_regs[7] = reg_val;

		reg_val = perfmon->perf_infos[i]->cfg_regs[8];
		reg_val |= 0x1;
		perfmon->perf_infos[i]->cfg_regs[8] = reg_val;

		reg_val = perfmon->perf_infos[i]->cfg_regs[9];
		reg_val |= 0x1;
		perfmon->perf_infos[i]->cfg_regs[9] = reg_val;
	}

	mc_perf_kobj = kobject_create_and_add("perf", NULL);
	ret = sysfs_create_file(mc_perf_kobj, &dev_attr_perfmon.attr);
	if (ret)
		printk(KERN_EMERG "Can not create the mc perfmon sysfs\n");


	printk(KERN_EMERG "Can  create the mc perfmon sysfs\n");
	return 0;
}

/* Driver Structure */
static struct platform_driver mc_perfmon_driver = {
	.probe          = mc_perfmon_probe,
	.driver         = {
		.name   = "mc-perfmon",
		.of_match_table = mc_perfmon_of_match,
	},
};

static ssize_t perf_read(struct file *file, char __user *buf,
                        size_t count, loff_t *ppos)
{
	unsigned int i,it;
	struct timespec64 ts;

	if(clr_timer_cnt == 0) {
	regmap_update_bits(dmc_apb_base, dfimon_time_ctrl_offset_reg,
			(0x3), (0x0));
	regmap_update_bits(dmc_apb_base, dfimon_time_ctrl_offset_reg,
			(0x3), (1));

		clr_timer_cnt = 1;
	} else {
		clr_timer_cnt = 0;
	}

	for(i=0;i<DDR_MAX_CHN;i++){
		perfmon->perf_infos[i]->cfg_regs[0] |= (0x1 << 11);
	}

	ktime_get_boottime_ts64(&ts);

	for(i=0;i<DDR_MAX_CHN;i++) {
		it = perfmon->perf_infos[i]->rtrans_cnt;
		if(copy_to_user(buf+(i << 5)+0x00,&it,4)) {
			return -EFAULT;
		}

		it = perfmon->perf_infos[i]->rbw_cnt;
		if(copy_to_user(buf+(i << 5)+0x04,&it,4)) {
			return -EFAULT;
		}

		it = perfmon->perf_infos[i]->rlatency_cnt;
		if (copy_to_user(buf+(i << 5)+0x08,&it,4)) {
			return -EFAULT;
		}

		it = perfmon->perf_infos[i]->wtrans_cnt;
		if (copy_to_user(buf+(i << 5)+0x0C,&it,4)) {
			return -EFAULT;
		}

		it = perfmon->perf_infos[i]->wbw_cnt;
		if (copy_to_user(buf+(i << 5)+0x10,&it,4)) {
			return -EFAULT;
		}

		it = perfmon->perf_infos[i]->wlatency_cnt;
		if (copy_to_user(buf+(i << 5)+0x14,&it,4)) {
			return -EFAULT;
		}

		it = perfmon->perf_infos[i]->rbw_cnt_hig;
		if (copy_to_user(buf+(i << 5)+0x18,&it,4)) {
			return -EFAULT;
		}

		it = perfmon->perf_infos[i]->wbw_cnt_hig;
		if (copy_to_user(buf+(i << 5)+0x1C,&it,4)) {
			return -EFAULT;
		}
#if 0
		printk("CH[%d] 0x%08x\n",i,perfmon->perf_infos[i]->rtrans_cnt);
		printk("CH[%d] 0x%08x\n",i,perfmon->perf_infos[i]->rbw_cnt);
		printk("CH[%d] 0x%08x\n",i,perfmon->perf_infos[i]->rlatency_cnt);
		printk("CH[%d] 0x%08x\n",i,perfmon->perf_infos[i]->wtrans_cnt);
		printk("CH[%d] 0x%08x\n",i,perfmon->perf_infos[i]->wbw_cnt);
		printk("CH[%d] 0x%08x\n",i,perfmon->perf_infos[i]->wlatency_cnt);
		printk("CH[%d] 0x%08x\n",i,perfmon->perf_infos[i]->rbw_cnt_hig);
		printk("CH[%d] 0x%08x\n",i,perfmon->perf_infos[i]->wbw_cnt_hig);
#endif
	}

	//printk("%08x %08x\n",tv.tv_sec,tv.tv_usec);

	if (copy_to_user(buf+(10 << 5)+0x20,&ts,sizeof(ts))) {
			return -EFAULT;
	}
	return count;

}

static int perf_open(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations perf_fops = {
        .owner          = THIS_MODULE,
        .read           = perf_read,
        .open           = perf_open,
};

static struct miscdevice perf_dev = {
        .minor          = MISC_DYNAMIC_MINOR,
        .name           = "perf",
        .fops           = &perf_fops,
};

static int __init perf_init(void)
{
	misc_register(&perf_dev);
	return 0;
}

module_init(perf_init);
module_platform_driver(mc_perfmon_driver);
