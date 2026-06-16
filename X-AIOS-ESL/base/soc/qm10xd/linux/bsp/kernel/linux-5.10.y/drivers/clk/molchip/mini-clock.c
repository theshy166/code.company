// SPDX-License-Identifier: GPL-2.0
/*
 * Molchip clock driver
 *
 * Copyright (C) 2024 Molchip Electronics Co., Ltd.
 *		https://www.molchip.com
 *
 * Author: Jerry Cao <jerry.cao@molchip.com>
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/mini_clock/clock.h>
#include <linux/mini_clock/clock-array.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>

#define DRIVER_NAME			"mini_clk"

#define MOL_CPU_SYS_AHB			0xc000000
#define MOL_CPU_SYS_CLK_RF		0x9200000
#define MOL_TOP_SYS_GATE		0x10000000
#define MOL_TOP_SYS_CLK_RF		0x10300000
#define MOL_DMC_SYS_AHB			0x24000000

#define GET_MASK_FUN(x)		((1 << (x)) - 1)
#define GET_REG_OFFSET(x)	((x) & 0xFFF)


static DEFINE_SPINLOCK(enable_lock);
static struct task_struct *enable_owner;
static int enable_refcnt;
static const char *mc_summary = "summary";
static const char *mc_event = "event";
static struct dentry * dirs;
static struct dentry * summary, *event;

static struct regmap * cpu_sys_regmap;
static struct regmap * cpu_sys_rf_regmap;
static struct regmap * top_sys_regmap;
static struct regmap * top_sys_rf_regmap;
static struct regmap * dmc_sys_regmap;


/***           locking             ***/
static unsigned long clk_enable_lock(void)
	__acquires(enable_lock)
{
	unsigned long flags;

	/*
	 * On UP systems, spin_trylock_irqsave() always returns true, even if
	 * we already hold the lock. So, in that case, we rely only on
	 * reference counting.
	 */
	if (!IS_ENABLED(CONFIG_SMP) ||
	    !spin_trylock_irqsave(&enable_lock, flags)) {
		if (enable_owner == current) {
			enable_refcnt++;
			__acquire(enable_lock);
			if (!IS_ENABLED(CONFIG_SMP))
				local_save_flags(flags);
			return flags;
		}
		spin_lock_irqsave(&enable_lock, flags);
	}
	WARN_ON_ONCE(enable_owner != NULL);
	WARN_ON_ONCE(enable_refcnt != 0);
	enable_owner = current;
	enable_refcnt = 1;
	return flags;
}

static void clk_enable_unlock(unsigned long flags)
	__releases(enable_lock)
{
	WARN_ON_ONCE(enable_owner != current);
	WARN_ON_ONCE(enable_refcnt == 0);

	if (--enable_refcnt) {
		__release(enable_lock);
		return;
	}
	enable_owner = NULL;
	spin_unlock_irqrestore(&enable_lock, flags);
}

static struct regmap * mol_sys_regmap_get(unsigned int address)
{
	struct regmap * mmap = NULL;
	u32 addr = address & (~0xfff);

	switch(addr) {
		case MOL_CPU_SYS_AHB:
			mmap = cpu_sys_regmap;
			break;
		case MOL_DMC_SYS_AHB:
			mmap = dmc_sys_regmap;
			break;
		case MOL_CPU_SYS_CLK_RF:
			mmap = cpu_sys_rf_regmap;
			break;
		case MOL_TOP_SYS_GATE:
			mmap = top_sys_regmap;
			break;
		case MOL_TOP_SYS_CLK_RF:
			mmap = top_sys_rf_regmap;
			break;
		default:
			break;
	}
	return mmap;
}

static int mol_sys_reg_update(struct regmap * mmap, unsigned int address, int offset,
			int width, int data)
{
	unsigned int mask;
	unsigned int val;
	unsigned long flags;
	int ret;

	if (!mmap)
		return -EINVAL;

	mask = GET_MASK_FUN(width);
	val = data << offset;

	flags = clk_enable_lock();
	ret = regmap_update_bits(mmap, address, mask << offset, val);
	clk_enable_unlock(flags);
	return ret;
}

static int mol_sys_reg_read(struct regmap * mmap, unsigned int address, unsigned int *data)
{
	if (!mmap)
		return -EINVAL;
	
	return regmap_read(mmap, address, data);
}

static struct mc_clock_mini *get_clk_info_by_id(int clk_id)
{
	struct mc_clock_mini *clk = NULL;

	if (clk_id & SYS_TOP_CLK_ID)
		clk = &mc_sys_top_clock[clk_id ^ SYS_TOP_CLK_ID];
	else if (clk_id & SYS_CPU_CLK_ID)
		clk = &mc_sys_cpu_clock[clk_id ^ SYS_CPU_CLK_ID];
	else if (clk_id & SYS_DMC_CLK_ID)
		clk = &mc_sys_dmc_clock[clk_id ^ SYS_DMC_CLK_ID];

	return clk;
}

static struct mc_clock_mini *fixed_clk_get_by_dev_id(const char *dev_id)
{
	int i = 0;
	struct mc_clock_mini *p_clk = NULL;

	for (i = 0; i < ARRAY_SIZE(mc_fixed_clock_array); i++) {
		if (!strcmp(dev_id, mc_fixed_clock_array[i].dev_id)) {
			p_clk = &mc_fixed_clock_array[i];
			break;
		}
	}

	return p_clk;
}

static int clk_gate_toggle(const struct clk_addr_info *cai, bool en)
{
	struct regmap *mmap;
	int ret;
	unsigned int reg_offset;
	if (!cai)
		return -EINVAL;

	mmap = mol_sys_regmap_get(cai->address);
	if (!mmap)
		return -EINVAL;

	reg_offset = GET_REG_OFFSET(cai->address);

	if (en) {
		ret = mol_sys_reg_update(mmap, reg_offset, cai->bit_offset, cai->bit_width, 0x1);
	} else {
		ret = mol_sys_reg_update(mmap, reg_offset, cai->bit_offset, cai->bit_width, 0x0);
	}

	return 0;
}


static bool clk_gate_enabled(const struct clk_addr_info *cai)
{
	struct regmap *mmap;
	unsigned int val;
	unsigned int reg_offset;
	int ret;

	if (!cai)
		return false;

	mmap = mol_sys_regmap_get(cai->address);
	if (!mmap)
		return false;

	reg_offset = GET_REG_OFFSET(cai->address);

	ret = mol_sys_reg_read(mmap, reg_offset, &val);
	if (ret) {
		printk(KERN_ERR "regmap read err %d\n", ret);
		return false;
	}

	return val & (1 << cai->bit_offset) ? true : false;

}

static int clk_regmap_write(unsigned int address, int offset, int width, int data)
{
	struct regmap *mmap;
	unsigned int reg_offset;

	mmap = mol_sys_regmap_get(address);
	if (!mmap)
		return -EINVAL;

	reg_offset = GET_REG_OFFSET(address);

	return mol_sys_reg_update(mmap, reg_offset, offset, width, data);
}

static int clk_regmap_read(unsigned int address, int offset, int width, unsigned int *data)
{
	struct regmap *mmap;
	unsigned int val;
	unsigned int reg_offset;
	int ret;

	mmap = mol_sys_regmap_get(address);
	if (!mmap)
		return -EINVAL;

	reg_offset = GET_REG_OFFSET(address);

	ret = mol_sys_reg_read(mmap, reg_offset, &val);
	if (ret) {
		printk(KERN_ERR "refmap read err %d\r\n", ret);
		return ret;
	}

	val >>= offset;
	val &= GET_MASK_FUN(width);
	*data = val;

	return 0;
}

static unsigned long clk_mux_get_rate(struct mc_clock_mini *clk)
{
	struct mc_clock_mini *p_clk = NULL;
	unsigned int div = 0;
	unsigned int index = 0;
	int ret = 0;

	if (!clk || clk->preoperty != MUX_P)
		goto Get_Rate_Err;

	if (clk->divider_info) 
		ret = clk_regmap_read(clk->divider_info->address, clk->divider_info->bit_offset, 
								clk->divider_info->bit_width, &div);

	if (ret)
		goto Get_Rate_Err;

	if (clk->addr_info)
		ret = clk_regmap_read(clk->addr_info->address, clk->addr_info->bit_offset,
								clk->addr_info->bit_width, &index);

	if (ret)
		goto Get_Rate_Err;

	p_clk = fixed_clk_get_by_dev_id(clk->parent_names[index]);
	if (!p_clk)
		goto Get_Rate_Err;

	return p_clk->current_rate / (div + 1);

Get_Rate_Err:
	return ret;
}

static int clk_mini_core_enable(struct mc_clock_mini *clk)
{
	int ret = 0;
	unsigned long flags;
	if (!clk)
		return -EINVAL;

	if (clk->enabled_count > 0)
		printk(KERN_WARNING "clk %s is already en\n", clk->dev_id);
	ret = clk_gate_toggle(clk->addr_info, true);

	flags = clk_enable_lock();
	clk->enabled_count++;
	clk_enable_unlock(flags);

	return ret;

}

static int clk_mini_core_disable(struct mc_clock_mini *clk)
{
	int ret = 0;
	unsigned long flags;

	if (!clk)
		return -EINVAL;

	flags = clk_enable_lock();
	if (clk->enabled_count == 0 || --clk->enabled_count == 0) {
		ret = clk_gate_toggle(clk->addr_info, false);
	}
	clk_enable_unlock(flags);
	return ret;
}

static bool clk_mini_core_is_enabled(struct mc_clock_mini *clk)
{
	if (!clk || clk->preoperty != GATE_P)
		return true;

	return clk_gate_enabled(clk->addr_info);
}

static unsigned long clk_mini_core_get_rate(struct mc_clock_mini *clk)
{
	if (!clk || clk->preoperty != MUX_P)
		return 0;

	return clk_mux_get_rate(clk);
}

static int clk_mux_set(struct mc_clock_mini *clk, int index, int div)
{
	int ret = 0;

	if (!clk)
		return -EINVAL;

	if (clk->divider_info) {
		if (div == 0)
			ret = clk_regmap_write(clk->divider_info->address, clk->divider_info->bit_offset,
				clk->divider_info->bit_width, 0);
		else
			ret = clk_regmap_write(clk->divider_info->address, clk->divider_info->bit_offset,
				clk->divider_info->bit_width, --div);
	}
	if (ret)
		return ret;

	if (clk->addr_info)
		ret = clk_regmap_write(clk->addr_info->address, clk->addr_info->bit_offset,
			clk->addr_info->bit_width, index);

	return ret;
}

static int clk_calc_new_rates(struct mc_clock_mini *clk, unsigned long rate)
{
	struct mc_clock_mini *p_clk = NULL;
	int chose_idx = clk->num_parents - 1;
	int chose_div = 0;
	unsigned int remainder1 = -1, remainder2 = 0;
	int i = 0;
	int div = 0;
	int div_mask = 1;

	if (clk->divider_info)
		div_mask = (1 << clk->divider_info->bit_width) - 1;

	for (i = 0; i < clk->num_parents; i++) {
		p_clk = fixed_clk_get_by_dev_id(clk->parent_names[i]);
		if (!p_clk) {
			printk("error:can not find factor clk: %s\n", clk->parent_names[i]);
			chose_idx = (i == 0) ? 0 : (i -1);			//for change to 0.7v.
			break;
		}

	/*default*/
		clk->current_rate = p_clk->current_rate;

		if (p_clk->current_rate < rate)
			continue;

		remainder2 = p_clk->current_rate % rate;
		div = p_clk->current_rate / rate;


	/*multiple relation*/
		if (remainder2 == 0 && div <= div_mask) {
			chose_idx = i;
			chose_div = div;
			clk->current_rate = p_clk->current_rate / chose_div;
			break;
		}
	/*select the nearest clock source*/
		if (remainder2 < remainder1) {
			remainder1 = remainder2;
			chose_idx = i;

			if (div > div_mask)
				break;

			chose_div = div;

			clk->current_rate = p_clk->current_rate / chose_div;
		}
	}
	return clk_mux_set(clk, chose_idx, chose_div);
}

static int clk_mini_core_set_rate(struct mc_clock_mini *clk, unsigned long rate)
{
	int ret = 0;
	if (!clk || rate == 0)
		return -EINVAL;

	ret = clk_calc_new_rates(clk, rate);
	return ret;
}
int clk_mini_enable(int clk_id)
{
	struct mc_clock_mini * clk;
	int ret = 0;
	clk = get_clk_info_by_id(clk_id);
	if (!clk)
		return -EINVAL;

	if (clk->preoperty == GATE_P)
		ret = clk_mini_core_enable(clk);
	return ret;
}
EXPORT_SYMBOL(clk_mini_enable);

int clk_mini_disable(int clk_id)
{
	struct mc_clock_mini * clk;
	int ret = 0;

	clk = get_clk_info_by_id(clk_id);
	if (!clk)
		return -EINVAL;

	if (clk->preoperty == GATE_P)
		ret = clk_mini_core_disable(clk);

	return ret;
}
EXPORT_SYMBOL(clk_mini_disable);

bool clk_mini_is_enabled(int clk_id)
{
	struct mc_clock_mini * clk;
	bool ret = true;

	clk = get_clk_info_by_id(clk_id);
	if (!clk)
		return ret;

	if (clk->preoperty == GATE_P)
		ret = clk_mini_core_is_enabled(clk);
	return ret;

}
EXPORT_SYMBOL(clk_mini_is_enabled);

int clk_mini_set_rate(int clk_id, unsigned long rate)
{
	struct mc_clock_mini * clk;
	int ret = 0;

	clk = get_clk_info_by_id(clk_id);
	if (!clk)
		return -EINVAL;

	if (clk->preoperty == MUX_P)
		ret = clk_mini_core_set_rate(clk, rate);

	return ret;
}
EXPORT_SYMBOL(clk_mini_set_rate);

unsigned long clk_mini_get_rate(int clk_id)
{
	struct mc_clock_mini * clk;
	unsigned long ret = 0;

	clk = get_clk_info_by_id(clk_id);
	if (!clk)
		return -EINVAL;

	if (clk->preoperty != GATE_P) {
		if (in_interrupt()) {
			return clk->current_rate;
		} else {
			ret = clk_mini_core_get_rate(clk);
		}
	}
	return ret;

}
EXPORT_SYMBOL(clk_mini_get_rate);

static void printk_clk_status(struct seq_file *sfile, struct mc_clock_mini __force * mcm)
{
	int level = 0;

	if (!mcm)
		return;

	seq_printf(sfile, "%*s%-*s %8d %11u\n",  level * 3 + 1, "", 30 - level * 3,
				mcm->dev_id, mcm->enabled_count, mcm->current_rate);

}

static void *summary_seq_start(struct seq_file *s, loff_t *pos)
{
	static unsigned long counter;
	if (*pos == 0)
		return &counter;
	else {
	//	*pos = 0;
		return NULL;
	}
}

static void summary_seq_stop(struct seq_file *s, void *v)
{
}

static void *summary_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;
	return NULL;
}

static int summary_seq_show(struct seq_file *sfile, void *v)
{
	int i = 0;


	seq_puts(sfile, "   clock                          enable count               rate\n");
	seq_puts(sfile, "-----------------------------------------------------------------\n");


	for (i = 0; i < ARRAY_SIZE(mc_sys_top_clock); i++)
		printk_clk_status(sfile, &mc_sys_top_clock[i]);

	for (i = 0; i < ARRAY_SIZE(mc_sys_cpu_clock); i++)
		printk_clk_status(sfile, &mc_sys_cpu_clock[i]);

	for (i = 0; i < ARRAY_SIZE(mc_sys_dmc_clock); i++)
		printk_clk_status(sfile, &mc_sys_dmc_clock[i]);

	return 0;

}

static const struct seq_operations clk_summary_seq_file = {
	.start	= summary_seq_start,
	.next	= summary_seq_next,
	.stop	= summary_seq_stop,
	.show	= summary_seq_show,
};

static int clk_summary_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &clk_summary_seq_file);
}

static int __clk_set_clock(struct mc_clock_mini __force * mcm, char *param)
{
	unsigned long rate = 0;
	int ret = 0;
	if (!mcm)
		return -EINVAL;

	if (!strncmp(param, "enable", 6)) {
		ret = clk_mini_core_enable(mcm);
	}
	else if (!strncmp(param, "disable", 7)) {
		ret = clk_mini_core_disable(mcm);
	}
	else {
		rate = simple_strtoul(param, NULL, 10);
		if (mcm->preoperty == MUX_P)
			ret = clk_mini_core_set_rate(mcm, rate);
	}
	return ret;
}
static int __clk_event_set(char *name, char *param)
{
	int i = 0;
	int ret = 0;

	for (i = 0; i < ARRAY_SIZE(mc_sys_top_clock); i++) {
		if (strnstr(mc_sys_top_clock[i].dev_id, name, strlen(mc_sys_top_clock[i].dev_id))) {
			ret = __clk_set_clock(&mc_sys_top_clock[i], param);
			if (ret)
				return ret;
		}
	}
	for (i = 0; i < ARRAY_SIZE(mc_sys_cpu_clock); i++) {
		if (strnstr(mc_sys_cpu_clock[i].dev_id, name, strlen(mc_sys_cpu_clock[i].dev_id))) {
			ret = __clk_set_clock(&mc_sys_cpu_clock[i], param);
			if (ret)
				return ret;
		}
	}
	for (i = 0; i < ARRAY_SIZE(mc_sys_dmc_clock); i++) {
		if (strnstr(mc_sys_dmc_clock[i].dev_id, name, strlen(mc_sys_dmc_clock[i].dev_id))) {
			ret = __clk_set_clock(&mc_sys_dmc_clock[i], param);
			if (ret)
				return ret;
		}
	}
	return 0;
}

static int clk_event_set(char *str)
{
	char *name;
	char *param;

	name = strsep(&str, "_");
	if (!name || strcmp(name, "clock"))
		return -EINVAL;

	name = strsep(&str, "_");
	if (!name)
		return -EINVAL;

	param = str;

	return __clk_event_set(name, param);
}

static ssize_t clk_event_write(struct file *file, const char __user *buf, size_t length, loff_t *off)
{

	char *buffer, *str;
	int err = 0;

	if (!buf || length > PAGE_SIZE)
		return -EINVAL;

	buffer = (char *)__get_free_page(GFP_KERNEL);
	if (!buffer)
		return -ENOMEM;

	if (copy_from_user(buffer, buf, length))
		goto write_out;

	err = -EINVAL;
	if (length < PAGE_SIZE)
		buffer[length] = '\0';
	else if (buffer[PAGE_SIZE-1])
		goto write_out;

	str = buffer;

	err = clk_event_set(str);

write_out:
	free_page((unsigned long)buffer);
	return (err == 0) ? length : err;
}

static const struct file_operations mc_clk_summary_ops = {
	.open = clk_summary_open,
	.read = seq_read,
};

static const struct file_operations mc_clk_event_ops = {
	.write = clk_event_write,
};

static int __init mc_simple_clk_init(void)
{
	int err;

	cpu_sys_regmap = syscon_regmap_lookup_by_compatible("mol,cpu-sys-ahb");
	if (IS_ERR(cpu_sys_regmap)) {
		return PTR_ERR(cpu_sys_regmap);
	}
	cpu_sys_rf_regmap = syscon_regmap_lookup_by_compatible("mol,cpu-sys-clk-rf");
	if (IS_ERR(cpu_sys_rf_regmap)) {
		return PTR_ERR(cpu_sys_rf_regmap);
	}
	top_sys_regmap = syscon_regmap_lookup_by_compatible("mol,top-sys-gate");
	if (IS_ERR(top_sys_regmap)) {
		return PTR_ERR(top_sys_regmap);
	}
	top_sys_rf_regmap = syscon_regmap_lookup_by_compatible("mol,top-sys-clk-rf");
	if (IS_ERR(top_sys_rf_regmap)) {
		return PTR_ERR(top_sys_rf_regmap);
	}
	dmc_sys_regmap = syscon_regmap_lookup_by_compatible("mol,dmc-sys-apb");
	if (IS_ERR(dmc_sys_regmap)) {
		return PTR_ERR(dmc_sys_regmap);
	}

	dirs = debugfs_create_dir(DRIVER_NAME, 0);
	if (IS_ERR(dirs)) {
		err = PTR_ERR(dirs);
		goto ERR_OUT;
	}

	summary = debugfs_create_file(mc_summary, 0444, dirs, NULL, &mc_clk_summary_ops);
	if (IS_ERR(summary)) {
		err = PTR_ERR(summary);
		goto ERR_OUT;
	}

	event = debugfs_create_file(mc_event, 0444, dirs, NULL, &mc_clk_event_ops);
	if (IS_ERR(event)) {
		err = PTR_ERR(event);
		goto ERR_OUT;
	}

	pr_info("mc simple clock register\r\n");
	return 0;

ERR_OUT:
	debugfs_remove(summary);
	debugfs_remove(event);
	debugfs_remove(dirs);
	return err;
}

static void mc_simple_clk_exit(void)
{
	debugfs_remove(summary);
	debugfs_remove(event);
	debugfs_remove(dirs);
}

postcore_initcall(mc_simple_clk_init);
module_exit(mc_simple_clk_exit);

MODULE_DESCRIPTION("mc clock test");
MODULE_LICENSE("GPL v2");
