// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2023, Molchip Co., Ltd
 * Author: 
 */


#include <linux/suspend.h>
#include <linux/of.h>
#include <asm/suspend.h>
#include <linux/of_address.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <asm/fncpy.h>
#include <linux/molchip.h>
#include "core.h"

#include "molchip_pm.h"

#define MOL_SRAM_ACCESS		0x310

#define PMU_DEEP_SLEEP_SEL		0x0
#define PMU_DEEP_SLEEP_CTRL		0x4
#define PMU_DEEP_SLEEP_INT_DISABLE		0x300
#define PMU_DEEP_SLEEP_STATUS			0x308

#define PD_DMC_SYS_DEEP_STOP	BIT(1)

#define DEEP_SLEEP_AUTO_SEL		BIT(14)
#define CPU_SYS_DSLP_SEL		BIT(11)
#define DMC_SYS_DSLP_SEL		BIT(6)
#define PLL_DSLP_SEL			BIT(0)

#define DEEP_SLEEP_ENA			BIT(2)

#define DEEP_SLEEP_INT_DISABLE	BIT(0)

void __iomem *mol_pmu_reg_base;	//pmu reg remap
static void __iomem *suspend_ocram_base;	//iram remap, exec
static void __iomem *mol_sleep_cfg_in_iram;			//0x27001000 + 0x1000, clock cfg
static struct regmap *dmc_sys_apb_base;
static struct regmap *cpu_sys_ahb_base;


static u32 (*mol_sdram_self_refresh_in_ocram)(u32 sdr_base);
static u32 sram_base;

struct molchip_pm_data {
	const struct platform_suspend_ops *ops;
	int (*init)(struct device_node *np);
};

static int mol_sram_init(u32 address)
{
	int ret = 0;

	suspend_ocram_base = __arm_ioremap_exec(address, molchip_sdram_self_refresh_sz, false);

	if (!suspend_ocram_base) {
		pr_warn("%s: __arm_ioremap_exec failed!\n", __func__);
		ret = -ENOMEM;
		goto iram_err;
	}

	mol_sleep_cfg_in_iram = ioremap(sram_base + 0x1000, 0x1000);
	if (!mol_sleep_cfg_in_iram)
		ret = -ENOMEM;

iram_err:
	return ret;
}

static int mol_sram_copy(void)
{
	int ret;

	regmap_update_bits(dmc_sys_apb_base, MOL_SRAM_ACCESS, 0x1, 0x1);

	/* Copy the code that puts DDR in self refresh to ocram */
	mol_sdram_self_refresh_in_ocram =
		(void *)fncpy(suspend_ocram_base,
			      &molchip_sdram_self_refresh,
			      molchip_sdram_self_refresh_sz);

	WARN(!mol_sdram_self_refresh_in_ocram,
	     "could not copy function to ocram");
	if (!mol_sdram_self_refresh_in_ocram) {
		ret = -EFAULT;
		goto iram_err;
	}

	ret = mol_sleep_gate_cfg_to_iram(mol_sleep_cfg_in_iram);

iram_err:
	return ret;
}

static void disable_mmu_and_jump_to_iram(unsigned long iram_base_pa)
{

	asm volatile("mov r0,%0"::"r"(iram_base_pa));
	asm volatile("mov r1,#0x0");
	asm volatile("dmajti_lp_start: cmp r1,#0x0");
	asm volatile("mrceq p15, 0, r1, c1, c0, 0");
	asm volatile("biceq r1,r1,#0x5");
	asm volatile("mcreq p15, 0, r1, c1, c0, 0");
	asm volatile("blxeq r0");

}
//static volatile int debug_sel = 1;
static int molchip_lpmode_enter(unsigned long arg)
{

	flush_cache_all();
#if 1
//	do{}while(debug_sel);
	disable_mmu_and_jump_to_iram(sram_base);

#else
	dmb(sy);
	wfi();
#endif

	return 0;
}

static int mol_suspend_enter(suspend_state_t state)
{

	int ret;
	local_fiq_disable();
	
	ret = mol_sram_copy();
	if (ret) {
		pr_err("%s: sram run map failed %d\n", __func__, ret);
		return ret;
	}
	mol_sleep_io_func();
	cpu_suspend(0, molchip_lpmode_enter);
/*enable ckg_gpio_apb_en/ckg_syst_apb_en early*/
	regmap_update_bits(cpu_sys_ahb_base, 0xb0, (0x3 << 10), (0x3 << 10));
	mol_wakeup_io_restore_state();
	local_fiq_enable();

	mol_int_ctrlx_get_state();

	return 0;
}

static int mol_suspend_prepare(void)
{
	return 0;
}

static void mol_suspend_finish(void)
{
	return;
}


static const struct platform_suspend_ops mol_suspend_ops = {
	.enter   = mol_suspend_enter,
	.valid   = suspend_valid_only_mem,
	.prepare = mol_suspend_prepare,
	.finish  = mol_suspend_finish,

};


static int __init mol_suspend_init(struct device_node *np)
{
	volatile unsigned int val;
	mol_pmu_reg_base = of_iomap(np, 0);
	if (!mol_pmu_reg_base) {
		pr_err("%s: could not map pmu reg base\n", __func__);;
		return -ENOMEM;
	}

	val = readl(mol_pmu_reg_base + PMU_DEEP_SLEEP_SEL);
	val |= (DEEP_SLEEP_AUTO_SEL | CPU_SYS_DSLP_SEL | 
			DMC_SYS_DSLP_SEL | PLL_DSLP_SEL);
	writel(val, mol_pmu_reg_base + PMU_DEEP_SLEEP_SEL);

	return 0;
}

static const struct molchip_pm_data mol_pm_data __initconst = {
	.ops = &mol_suspend_ops,
	.init = mol_suspend_init,
};


static const struct of_device_id molchip_pmu_of_device_ids[] __initconst = {
	{
		.compatible = "mol,pmu-reg",
		.data = &mol_pm_data,
	},
	{ /* sentinel */ },
};

int __init molchip_suspend_init(void)
{
	const struct molchip_pm_data *pm_data;
	const struct of_device_id *match;
	struct device_node *np;

	int ret;
	printk(">>>>>>>%s %s %d\n",__FILE__,__func__,__LINE__);
	np = of_find_matching_node_and_match(NULL, molchip_pmu_of_device_ids,
					     &match);
	if (!match) {
		pr_err("Failed to find PMU node\n");
		ret = -ENODEV;
		goto put_devices;
	}

	ret = of_property_read_u32(np, "sram-base", &sram_base);
	if (ret) {
		pr_err("%s: failed get sram in dtb %d\n", __func__, ret);
		goto put_devices;
	}

	dmc_sys_apb_base = syscon_regmap_lookup_by_phandle(np,  "mol,syscon");
	if (IS_ERR(dmc_sys_apb_base)) {
	   pr_err("failed to map dmc sys apb \n");
	   ret = -ENODEV;
	   goto put_devices;
	 }

	cpu_sys_ahb_base = syscon_regmap_lookup_by_phandle(np,  "mol,cpu-syscon");
	if (IS_ERR(cpu_sys_ahb_base)) {
	   pr_err("failed to map cpu sys ahb \n");
	   ret = -ENODEV;
	   goto put_devices;
	 }

	pm_data = (struct molchip_pm_data *) match->data;

	if (pm_data->init) {
		ret = pm_data->init(np);
		if (ret) {
			pr_err("%s: matches init error %d\n", __func__, ret);
			goto put_devices;
		}
	}

	ret = mol_sram_init(sram_base);
	if (ret)
		goto put_devices;

	ret = mol_sleep_io_cfg_init();
	if (ret)
		goto put_devices;

	suspend_set_ops(pm_data->ops);
	printk(">>>>>>>%s %s %d\n",__FILE__,__func__,__LINE__);

put_devices:
	return ret;
}

arch_initcall(molchip_suspend_init);

