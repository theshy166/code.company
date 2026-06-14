/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Copyright (C) 2012 ARM Limited
 */

#include <linux/gpio/driver.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/mfd/core.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/platform_data/syscon.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <linux/reboot.h>
#include <asm/system_misc.h>
#include <asm/unwind.h>
#include <linux/mini_clock/clock.h>

unsigned int task_offset;
unsigned int task_name_offset;
unsigned int log_buff;
unsigned int log_buff_len;

static char mbuf[] = "Usage:\nRead Command:\n\techo -r address -l num > lookat\nWrite Command:\n\techo -s address -l num value > lookat";
static char para_list[5][11];
//void __iomem *mc_reboot_base = 0;
u32 sw_rst_offset_reg, sw_rst_offset_bit, sw_rst_protect_offset_reg;
struct regmap *mc_sysreg_base;
struct regmap *dmc_sys_apb_reg_base;
u32 sfc_ctrl_reg, sfc_soft_rst_bit;

void molchip_flags_set(u32 data)
{
	struct regmap *base;
	u32 cpu1_start_addr_reg;
	int ret;

	struct device_node *node = of_find_compatible_node(NULL, NULL,
			"arm,molchip-sysreg");

	base = syscon_regmap_lookup_by_phandle(node,
						       "molchip,syscon-cpu-reg");

	if (WARN_ON(!base))
		return;

	ret = of_property_read_u32(node, "cpu1_start_addr_reg",
			     &cpu1_start_addr_reg);
	if (WARN_ON(ret))
		return;

	regmap_update_bits(base, cpu1_start_addr_reg,
		0xffffffff, data);
}

void molchip_flags_holding_pen(void)
{
	struct regmap *base;
	int ret;
	u32 cpu_core_ctrl_reg, cpu_soft_reset_reg;
	u32 core1_holding_pen_bit, core1_release_bit;
	struct device_node *node = of_find_compatible_node(NULL, NULL,
			"arm,molchip-sysreg");

	base = syscon_regmap_lookup_by_phandle(node,
						       "molchip,syscon-cpu-reg");

	if (WARN_ON(!base))
		return;

	ret = of_property_read_u32(node, "cpu_core_ctrl_reg",
			     &cpu_core_ctrl_reg);
	if (WARN_ON(ret))
		return;

	ret = of_property_read_u32(node, "cpu_soft_reset_reg",
			     &cpu_soft_reset_reg);
	if (WARN_ON(ret))
		return;

	ret = of_property_read_u32(node, "core1_holding_pen_bit",
			     &core1_holding_pen_bit);
	if (WARN_ON(ret))
		return;

	ret = of_property_read_u32(node, "core1_release_bit",
			     &core1_release_bit);
	if (WARN_ON(ret))
		return;

	regmap_update_bits(base, cpu_core_ctrl_reg,
		(1 << core1_holding_pen_bit), (1 << core1_holding_pen_bit));

	regmap_update_bits(base, cpu_soft_reset_reg,
		(1 << core1_release_bit), ~(1 << core1_release_bit));
}

/*
static void mc_cpu_access_share_ram(void)
{
	struct regmap *base;
	int ret;
	u32 vou_mem_ctrl_reg, cpu_access_share_ram_bit;
	struct device_node *node = of_find_compatible_node(NULL, NULL,
			"arm,molchip-sysreg");

	base = syscon_regmap_lookup_by_phandle(node, "molchip,dmc-syscon");

	if (WARN_ON(!base))
		return;

	ret = of_property_read_u32(node, "vou-mem-ctrl-reg",
			&vou_mem_ctrl_reg);
	if (WARN_ON(ret))
		return;

	ret = of_property_read_u32(node, "cpu-access-share-ram-bit",
			&cpu_access_share_ram_bit);
	if (WARN_ON(ret))
		return;

	regmap_update_bits(base, vou_mem_ctrl_reg,
		(1 << cpu_access_share_ram_bit),
		(1 << cpu_access_share_ram_bit));
}*/

static ssize_t show_reg(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%s\n", mbuf);
}

static ssize_t store_reg(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t len)
{
	u32 x = 0, y = 0, i;
	u32 base_addr, num, set_val;
	void __iomem *io_base;
	//sprintf(mbuf, "%s", buf);
	for (i = 0; buf[i] != 0; i++) {
		if ((x >= 5) || (y >= 11)) {
			dev_err(dev, "parameters greater than 5 or string is too long\n");
			return len;
		}
		if (buf[i] != ' ') {
			para_list[x][y++] = buf[i];
		} else {
			if (y) {
				para_list[x++][y] = 0;
			}
			y = 0;
		}
	}

	if (x >= 3) {
		if (para_list[0][1] == 's') {
			if (x == 4) {
				base_addr = simple_strtol(para_list[1], NULL, 0);
				num = simple_strtol(para_list[3], NULL, 0);
				set_val = simple_strtol(para_list[4], NULL, 0);
				if (base_addr < (ARCH_PFN_OFFSET << 12))
					io_base = ioremap(base_addr, num << 4);
				else
					io_base = ioremap(base_addr, num << 4);

				if (io_base == NULL) {
					dev_err(dev, "Error ioremap\n");
					return len;
				} else {
					for (i = 0; i < num; i++)
						writel_relaxed(set_val, io_base + (i << 2));
					for (i = 0; i < num; i++)
						dev_err(dev, "[0x%08x] [0x%08x]\n", base_addr + (i << 2), readl_relaxed(io_base + (i << 2)));
				}
				iounmap(io_base);
			} else {
				dev_err(dev, "Wrong parameter list\n");
				return len;
			}
		} else if (para_list[0][1] == 'r') {
			if (x == 3) {
				base_addr = simple_strtol(para_list[1], NULL, 0);
				num = simple_strtol(para_list[3], NULL, 0);
				if (base_addr < (ARCH_PFN_OFFSET << 12))
					io_base = ioremap(base_addr, num << 4);
				else
					io_base = ioremap(base_addr, num << 4);
				if (io_base == NULL) {
					dev_err(dev, "Error ioremap\n");
					return len;
				} else {
					for (i = 0; i < num; i++)
						dev_err(dev, "[0x%08x] [0x%08x]\n", base_addr + (i << 2), readl_relaxed(io_base + (i << 2)));
					iounmap(io_base);
				}
			} else {
				dev_err(dev, "Wrong parameter list\n");
				return len;
			}
		}
	} else {
		dev_err(dev, "Error parameter list\n");
		return len;
	}
	return len;
}
static DEVICE_ATTR(lookat, S_IWUSR|S_IRUSR, show_reg, store_reg);

static ssize_t show_irqtrace(struct device *dev, struct device_attribute *attr,
			char *buf)
{
#ifdef CONFIG_ARM64
	unwind_backtrace_dump_all();
	unwind_irq_backtrace_dump();
#endif
	return 0;
}

static ssize_t store_irqtrace(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t len)
{
#ifdef CONFIG_ARM64
	u32 data;

	if (kstrtouint(buf, 0, &data))
		return len;

	if (data & 0x80000000)
		unwind_irq_backtrace_enable(data & 0x3, (data >> 4) & 0x7ffffff);
	else
		unwind_backtrace_enable((data >> 2) & 0x3, data & 0x3, (data >> 4) & 0x7ffffff);
#endif
	return len;
}

static DEVICE_ATTR(irqtrace, S_IWUSR|S_IRUSR, show_irqtrace, store_irqtrace);

static unsigned int *reg_data;
static int reg_data_len;
static unsigned int sfc_base;

static void restart_init(void)
{
	//if(!mc_reboot_base) {
		int ret;
		struct device_node *node = of_find_compatible_node(NULL, NULL,
					"arm,molchip-sysreg");
		if (!node) {

			pr_err("failed to find node ");
			return ;
		}

		mc_sysreg_base = syscon_regmap_lookup_by_phandle(node,  "molchip,syscon");

		if (IS_ERR(mc_sysreg_base)) {
		   pr_err("failed to map glapb_base \n");
		   return ;
		 }

		ret = of_property_read_u32(node, "sw-rst-offset-reg",
				     &sw_rst_offset_reg);
		if (WARN_ON(ret))
			return;

		ret = of_property_read_u32(node, "sw-rst-offset-bit",
				     &sw_rst_offset_bit);
		if (WARN_ON(ret))
			return;

		dmc_sys_apb_reg_base = syscon_regmap_lookup_by_phandle(node,  "molchip,dmc_sys_apb_syscon");

		if (IS_ERR(dmc_sys_apb_reg_base)) {
			pr_err("failed to map dmc sys apb reg base \n");
			return ;
		}

		ret = of_property_read_u32(node, "sfc_ctrl_reg",
			&sfc_ctrl_reg);
		if (WARN_ON(ret))
			return;

		ret = of_property_read_u32(node, "sfc_soft_rst_bit",
			&sfc_soft_rst_bit);
		if (WARN_ON(ret))
			return;

	//}

	ret = of_property_read_u32(node, "sfc-base",
			     &sfc_base);
	if (!ret) {
		printk("use sfc_ram reboot\r\n");
		reg_data = (unsigned int *)kmalloc(0x1000, GFP_KERNEL);
		if(!reg_data) {
			printk("kmalloc fail for reg-data property\r\n");
			return;
		}
		reg_data_len = of_property_read_variable_u32_array(node, "reg-data", reg_data, 1, 0x1000);
		if (reg_data_len < 0) {
			printk("Failed to read reg-data property\r\n");
			return;
		}
	} else {
		sfc_base = 0;
	}
	return;
}

struct reg_st {
	unsigned int addr;
	unsigned int val;
	unsigned int mask;
	unsigned int flag;
};
#define FLAG_POLL 0
#define FLAG_SET  1

/* asm code at the end of the file */
static unsigned int iram_mach_code[0x100] = {
	0xE2800C01, 0xE5901000, 0xE3510000, 0x0A000006,
	0xE590100C, 0xE2011001, 0xE3510001, 0x0A000003,
	0xEA00000C, 0xE2800010, 0xEAFFFFF5, 0xEAFFFFFE,
	0xE5901000, 0xE5902004, 0xE5903008, 0xE5914000,
	0xE1E05003, 0xE0044005, 0xE0022003, 0xE1844002,
	0xE5814000, 0xEAFFFFF2, 0xE5901000, 0xE5902004,
	0xE5903008, 0xE5914000, 0xE1A05003, 0xE0044005,
	0xE1540002, 0x1AFFFFFA, 0xEAFFFFE9, 0xE52DB004,
	0xE28DB000, 0xE3A03000, 0xE1A00003, 0xE24BD000,
	0xE49DB004, 0xE12FFF1E, 0 //end flag
};

static void disable_mmu_and_jump_to_iram(unsigned long iram_base_pa)
{

	asm volatile("mov r0,%0"::"r"(iram_base_pa));
	asm volatile("mov r1,#0x0");
	asm volatile("dmajti_lp_start: cmp r1,#0x1");
	asm volatile("mrceq p15, 0, r1, c1, c0, 0");
	asm volatile("biceq r1,r1,#0x5");
	asm volatile("mcreq p15, 0, r1, c1, c0, 0");
	asm volatile("blxeq r0");
	asm volatile("mov r1,#0x1");
	asm volatile("b dmajti_lp_start");
}

static void do_iram_reboot(void)
{
	struct reg_st *cfg_base;
	unsigned long sfc_base_va;
	unsigned int i, reg_val, ret;

	ret = clk_mini_enable(CKG_SFC_EN);
	if (ret)
		printk("CKG_SFC_EN get ail \r\n");

	ret = clk_mini_enable(CKG_APB_SFC_EN);
	if (ret)
		printk("CKG_APB_SFC_EN get ail \r\n");

	sfc_base_va = (unsigned long)ioremap(sfc_base, 0x4000);
	if (sfc_base_va == 0)
		return;

	//NFC_DMA_COMMON reg cfg bit 8 encode mode
	reg_val = *(volatile unsigned int *)(sfc_base_va + 0x0D00);
	reg_val &= (~(1 << 8));
	*(volatile unsigned int *)(sfc_base_va + 0x0D00) = reg_val;

	//NFC_ACCESS_START bit 2 clean  before use buffer
	reg_val = *(volatile unsigned int *)(sfc_base_va + 0x0C04);
	reg_val |= (1 << 2);
	*(volatile unsigned int *)(sfc_base_va + 0x0C04) = reg_val;


	// offset 0x2000 is sfc buffer
	for (i=0; iram_mach_code[i] != 0; i++) {
		*(volatile unsigned int *)(sfc_base_va + 0x2000 + i*4) =
			iram_mach_code[i];
	}

	cfg_base = (struct reg_st *)(sfc_base_va + 0x2000 + 0x100);
	for (i=0; i<reg_data_len; i=i+4) {
		cfg_base->addr = *reg_data++;
		cfg_base->val = *reg_data++;
		cfg_base->mask = *reg_data++;
		cfg_base->flag = *reg_data++;
		cfg_base++;
	}

	iounmap((void __iomem *)sfc_base_va);

	local_irq_disable();
	local_fiq_disable();
	disable_mmu_and_jump_to_iram(sfc_base + 0x2000);
}

static int check_sfc(void)
{
	unsigned long sfc_base_va;
	unsigned int reg_val, ret;
	unsigned int mask = (1 << 2) | (1 << 3) | (1 << 16) | (1 << 17) |
		(1 << 18) | (1 << 19);
	unsigned int time_out = 50000;

	ret = clk_mini_enable(CKG_SFC_EN);
	if (ret)
		printk("CKG_SFC_EN get ail \r\n");

	ret = clk_mini_enable(CKG_APB_SFC_EN);
	if (ret)
		printk("CKG_APB_SFC_EN get ail \r\n");

	// CHECK SFC BUSY
	sfc_base_va = (unsigned long)ioremap(sfc_base, 0x4000);
	if (sfc_base_va == 0)
		return 0;
	do {
		reg_val = *(volatile unsigned int *)(sfc_base_va + 0x0C04);
		if ((reg_val & mask) == 0)
			break;
		udelay(1);
		time_out--;
	} while ((time_out != 0));

	iounmap((void __iomem *)sfc_base_va);

	// SFC SOFT RESET
	regmap_update_bits(dmc_sys_apb_reg_base, sfc_ctrl_reg,
		(1 << sfc_soft_rst_bit), (1 << sfc_soft_rst_bit));

	udelay(5);

	regmap_update_bits(dmc_sys_apb_reg_base, sfc_ctrl_reg,
		(1 << sfc_soft_rst_bit), 0);

	return 1;
}

void mc_soc_restart(enum reboot_mode mode, const char *cmd)
{
	//if (!mc_reboot_base) {
	if (check_sfc()) {
		do_iram_reboot();
		while(1);
	}
		if (sw_rst_protect_offset_reg)
			regmap_update_bits(mc_sysreg_base, sw_rst_protect_offset_reg,
				RESET_REG_WRITE_ENABLE,RESET_REG_WRITE_ENABLE_MASK);

		regmap_update_bits(mc_sysreg_base, sw_rst_offset_reg,
			(1 << sw_rst_offset_bit), (1 << sw_rst_offset_bit));
	//} else {
	//	writel_relaxed(0x1ACCE551, mc_reboot_base + 0x18);
	//	writel_relaxed(0x1, mc_reboot_base + 0);
	//	writel_relaxed(0xe, mc_reboot_base + 4);
	//}
}

void soc_restart(void)
{
	if (sw_rst_protect_offset_reg)
		regmap_update_bits(mc_sysreg_base, sw_rst_protect_offset_reg,
			RESET_REG_WRITE_ENABLE,RESET_REG_WRITE_ENABLE_MASK);
	regmap_update_bits(mc_sysreg_base, sw_rst_offset_reg,
		(1 << sw_rst_offset_bit), (1 << sw_rst_offset_bit));
}

static int molchip_sysreg_probe(struct platform_device *pdev)
{
	int ret;

	ret = device_create_file(&pdev->dev, &dev_attr_lookat);
	if (ret) {
		dev_err(&pdev->dev, "failed to register attributes, ret=%d\n", ret);
		return ret;
	}

	ret = device_create_file(&pdev->dev, &dev_attr_irqtrace);
	if (ret) {
		dev_err(&pdev->dev, "failed to register attributes, ret=%d\n", ret);
		return ret;
	}

	return 0;
}

static const struct of_device_id molchip_sysreg_match[] = {
	{ .compatible = "arm,molchip-sysreg", },
	{},
};

static struct platform_driver molchip_sysreg_driver = {
	.driver = {
		.name = "molchip-sysreg",
		.of_match_table = molchip_sysreg_match,
	},
	.probe = molchip_sysreg_probe,
};

static int __init molchip_sysreg_init(void)
{
	struct device_node *node;

	restart_init();
	arm_pm_restart = mc_soc_restart;
	/* Need the sysreg early, before any other device... */
	for_each_matching_node(node, molchip_sysreg_match)
		of_platform_device_create(node, NULL, NULL);

	return platform_driver_register(&molchip_sysreg_driver);
}
core_initcall(molchip_sysreg_init);

#if 0 	// ram code from reboot
/* r0 = _start = iram_base_va
 * 0x100000 ~ 0x1000ff code
 * 0x100100 ~ 0x103fff reg_data
 */
	.section .text.first
	.global _start
_start:
	add r0,r0,#0x100       // r0 = config[0]
cfg_lp:
	ldr r1,[r0]            // r1 = config[0].addr
	cmp r1,#0x0
	beq unexpected_lp
	ldr r1, [r0,#0xc]      // get config[0].flag
	and r1,r1,#0x1
	cmp r1,#0x1
	beq set_reg
	b check_reg
cntn:
	add r0,r0,#0x10
	b cfg_lp
unexpected_lp:
	b unexpected_lp

set_reg:
	ldr r1,[r0]            // r1 = config[n].addr
	ldr r2,[r0,#0x4]       // r2 = config[n].val
	ldr r3,[r0,#0x8]       // r3 = config[n].mask
	ldr r4,[r1]            // r4 = data of config[0].addr
	mvn r5,r3              // r5 = ~mask
	and r4,r4,r5           // data &= ~mask
	and r2,r2,r3           // val &= mask
	orr r4,r4,r2           // data |= val
	str r4,[r1]            // config data to addr
	b cntn
check_reg:
	ldr r1,[r0]            // r1 = config[n].addr
	ldr r2,[r0,#0x4]       // r2 = config[n].val
	ldr r3,[r0,#0x8]       // r3 = config[n].mask
ck_lp:	ldr r4,[r1]            // r4 = data of config[0].addr
	mov r5,r3              // data &= mask
	and r4,r4,r5
	cmp r4,r2              // data == val ?
	bne ck_lp
	b cntn
#endif
