/*
 * Copyright (c) Molchip Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/regmap.h>
#include <linux/pinctrl/pinconf-generic.h>
#include <dt-bindings/pinctrl/mcxcxx.h>

#include "pinctrl-mc-common.h"
#include "pinctrl-mc.h"

static const struct mc_pin_spec_pupd_set_samereg mc_spec_pupd[] = {
	/*group0  base_addr: 0x25B00000*/
	MC_PIN_PUPD_SPEC_SR(0, 0x10, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(1, 0x14, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(2, 0x18, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(3, 0x1c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(4, 0x20, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(5, 0x24, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(6, 0x28, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(7, 0x2c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(8, 0x30, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(9, 0x34, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(10, 0x38, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(11, 0x3c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(12, 0x40, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(13, 0x44, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(14, 0x48, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(15, 0x4c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(16, 0x50, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(17, 0x54, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(18, 0x58, 0, 2, 1),
	
///*group1  base_addr: 0x0B100000*/
	MC_PIN_PUPD_SPEC_SR(19, 0x28, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(20, 0x2c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(21, 0x30, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(22, 0x34, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(23, 0x38, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(24, 0x3c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(25, 0x40, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(26, 0x44, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(27, 0x48, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(28, 0x4c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(29, 0x50, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(30, 0x54, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(31, 0x58, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(32, 0x5c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(33, 0x60, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(34, 0x64, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(35, 0x68, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(36, 0x6c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(37, 0x70, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(38, 0x74, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(39, 0x78, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(40, 0x7c, 0, 2, 1),
	
/*group0  base_addr: 0x25B00000*/
	MC_PIN_PUPD_SPEC_SR(41, 0x5c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(42, 0x60, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(43, 0x64, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(44, 0x68, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(45, 0x6c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(46, 0x70, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(47, 0x74, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(48, 0x78, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(49, 0x7c, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(50, 0x80, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(51, 0x84, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(52, 0x88, 0, 2, 1),
	
///*group1  base_addr: 0x0B100000*/	
	MC_PIN_PUPD_SPEC_SR(53, 0x80, 0, 2, 1),
	MC_PIN_PUPD_SPEC_SR(54, 0x84, 0, 2, 1)
};

static int mc_spec_pull_set(struct regmap *regmap, unsigned int pin,
		bool isup, unsigned int r1r0)
{
	//printk("pin=0x%x,isup=0x%x,r1r0=0x%x\n",pin,isup,r1r0);
	return mc_pctrl_spec_pull_set_samereg(regmap, mc_spec_pupd,
		ARRAY_SIZE(mc_spec_pupd), pin, isup, r1r0);
}

static const struct mc_pin_smt_set mc_smt_set[] = {

	/*base_addr: 0x25b00000*/
	MC_PIN_SMT_SPEC(0, 0x10, 3),
	MC_PIN_SMT_SPEC(1, 0x14, 3),
	MC_PIN_SMT_SPEC(2, 0x18, 3),
	MC_PIN_SMT_SPEC(3, 0x1c, 3),
	MC_PIN_SMT_SPEC(4, 0x20, 3),
	MC_PIN_SMT_SPEC(5, 0x24, 3),
	MC_PIN_SMT_SPEC(6, 0x28, 3),
	MC_PIN_SMT_SPEC(7, 0x2c, 3),
	MC_PIN_SMT_SPEC(8, 0x30, 3),
	MC_PIN_SMT_SPEC(9, 0x34, 3),
	MC_PIN_SMT_SPEC(10, 0x38, 3),
	MC_PIN_SMT_SPEC(11, 0x3c, 3),
	MC_PIN_SMT_SPEC(12, 0x40, 3),
	MC_PIN_SMT_SPEC(13, 0x44, 3),
	MC_PIN_SMT_SPEC(14, 0x48, 3),
	MC_PIN_SMT_SPEC(15, 0x4c, 3),
	MC_PIN_SMT_SPEC(16, 0x50, 3),
	MC_PIN_SMT_SPEC(17, 0x54, 3),
	MC_PIN_SMT_SPEC(18, 0x58, 3),
	
/*group1  base_addr: 0x0B100000*/	
	MC_PIN_SMT_SPEC(19, 0x28, 3),
	MC_PIN_SMT_SPEC(20, 0x2c, 3),
	MC_PIN_SMT_SPEC(21, 0x30, 3),
	MC_PIN_SMT_SPEC(22, 0x34, 3),
	MC_PIN_SMT_SPEC(23, 0x38, 3),
	MC_PIN_SMT_SPEC(24, 0x3c, 3),
	MC_PIN_SMT_SPEC(25, 0x40, 3),
	MC_PIN_SMT_SPEC(26, 0x44, 3),
	MC_PIN_SMT_SPEC(27, 0x48, 3),
	MC_PIN_SMT_SPEC(28, 0x4c, 3),
	MC_PIN_SMT_SPEC(29, 0x50, 3),
	MC_PIN_SMT_SPEC(30, 0x54, 3),
	MC_PIN_SMT_SPEC(31, 0x58, 3),
	MC_PIN_SMT_SPEC(32, 0x5c, 3),
	MC_PIN_SMT_SPEC(33, 0x60, 3),
	MC_PIN_SMT_SPEC(34, 0x64, 3),
	MC_PIN_SMT_SPEC(35, 0x68, 3),
	MC_PIN_SMT_SPEC(36, 0x6c, 3),
	MC_PIN_SMT_SPEC(37, 0x70, 3),
	MC_PIN_SMT_SPEC(38, 0x74, 3),
	MC_PIN_SMT_SPEC(39, 0x78, 3),
	MC_PIN_SMT_SPEC(40, 0x7c, 3),
	/*base_addr: 0x25b00000*/
	MC_PIN_SMT_SPEC(41, 0x5c, 3),
	MC_PIN_SMT_SPEC(42, 0x60, 3),
	MC_PIN_SMT_SPEC(43, 0x64, 3),
	MC_PIN_SMT_SPEC(44, 0x68, 3),
	MC_PIN_SMT_SPEC(45, 0x6c, 3),
	MC_PIN_SMT_SPEC(46, 0x70, 3),
	MC_PIN_SMT_SPEC(47, 0x74, 3),
	MC_PIN_SMT_SPEC(48, 0x78, 3),
	MC_PIN_SMT_SPEC(49, 0x7c, 3),
	MC_PIN_SMT_SPEC(50, 0x80, 3),
	MC_PIN_SMT_SPEC(51, 0x84, 3),
	MC_PIN_SMT_SPEC(52, 0x88, 3),
	/*group1  base_addr: 0x0B100000*/
	MC_PIN_SMT_SPEC(53, 0x80, 3),
	MC_PIN_SMT_SPEC(54, 0x84, 3)
};

static int pin_smt_set(struct regmap *regmap, unsigned int pin,
		int value, enum pin_config_param arg)
{
		return mc_pconf_spec_set_smt_range(regmap, mc_smt_set,
			ARRAY_SIZE(mc_smt_set), pin, value);
}

static const struct mc_drv_group_desc mc_drv_grp[] =  {
	/* drv_stren 1/2/3/4/5/6/7/8 */
	MC_DRV_GRP(1, 12, 5, 7, 1),
	/* drv_stren 2/4/6/8/10/12/14/16 */
	MC_DRV_GRP(2, 16, 5, 7, 2)
};

static const struct mc_pin_drv_grp mc_pin_drv[] = {
	/*base_addr: 0x25b00000*/
	MC_PIN_DRV_GRP(0, 0x10, 16, 1),
	MC_PIN_DRV_GRP(1, 0x14, 16, 1),
	MC_PIN_DRV_GRP(2, 0x18, 16, 0),
	MC_PIN_DRV_GRP(3, 0x1c, 16, 0),
	MC_PIN_DRV_GRP(4, 0x20, 16, 0),
	MC_PIN_DRV_GRP(5, 0x24, 16, 0),
	MC_PIN_DRV_GRP(6, 0x28, 16, 0),
	MC_PIN_DRV_GRP(7, 0x2c, 16, 0),
	MC_PIN_DRV_GRP(8, 0x30, 16, 0),
	MC_PIN_DRV_GRP(9, 0x34, 16, 1),
	MC_PIN_DRV_GRP(10, 0x38, 16, 1),
	MC_PIN_DRV_GRP(11, 0x3c, 16, 1),
	MC_PIN_DRV_GRP(12, 0x40, 16, 1),
	MC_PIN_DRV_GRP(13, 0x44, 16, 1),
	MC_PIN_DRV_GRP(14, 0x48, 16, 1),
	MC_PIN_DRV_GRP(15, 0x4c, 16, 1),
	MC_PIN_DRV_GRP(16, 0x50, 16, 1),
	MC_PIN_DRV_GRP(17, 0x54, 16, 1),
	MC_PIN_DRV_GRP(18, 0x58, 16, 1),

	///*group1  base_addr: 0x0B100000*/
	MC_PIN_DRV_GRP(19, 0x28, 16, 1),
	MC_PIN_DRV_GRP(20, 0x2c, 16, 1),
	MC_PIN_DRV_GRP(21, 0x30, 16, 1),
	MC_PIN_DRV_GRP(22, 0x34, 16, 1),
	MC_PIN_DRV_GRP(23, 0x38, 16, 1),
	MC_PIN_DRV_GRP(24, 0x3c, 16, 1),
	MC_PIN_DRV_GRP(25, 0x40, 16, 1),
	MC_PIN_DRV_GRP(26, 0x44, 16, 1),
	MC_PIN_DRV_GRP(27, 0x48, 16, 1),
	MC_PIN_DRV_GRP(28, 0x4c, 16, 1),
	MC_PIN_DRV_GRP(29, 0x50, 16, 1),
	MC_PIN_DRV_GRP(30, 0x54, 16, 1),
	MC_PIN_DRV_GRP(31, 0x58, 16, 1),
	MC_PIN_DRV_GRP(32, 0x5c, 16, 1),
	MC_PIN_DRV_GRP(33, 0x60, 16, 1),
	MC_PIN_DRV_GRP(34, 0x64, 16, 1),
	MC_PIN_DRV_GRP(35, 0x68, 16, 1),
	MC_PIN_DRV_GRP(36, 0x6c, 16, 1),
	MC_PIN_DRV_GRP(37, 0x70, 16, 1),
	MC_PIN_DRV_GRP(38, 0x74, 16, 1),
	MC_PIN_DRV_GRP(39, 0x78, 16, 1),
	MC_PIN_DRV_GRP(40, 0x7c, 16, 1),

	/*base_addr: 0x25b00000*/
	MC_PIN_DRV_GRP(41, 0x5c, 16, 0),
	MC_PIN_DRV_GRP(42, 0x60, 16, 0),
	MC_PIN_DRV_GRP(43, 0x64, 16, 0),
	MC_PIN_DRV_GRP(44, 0x68, 16, 0),
	MC_PIN_DRV_GRP(45, 0x6c, 16, 0),
	MC_PIN_DRV_GRP(46, 0x70, 16, 0),
	MC_PIN_DRV_GRP(47, 0x74, 16, 0),
	MC_PIN_DRV_GRP(48, 0x78, 16, 0),
	MC_PIN_DRV_GRP(49, 0x7c, 16, 0),
	MC_PIN_DRV_GRP(50, 0x80, 16, 0),
	MC_PIN_DRV_GRP(51, 0x84, 16, 0),
	MC_PIN_DRV_GRP(52, 0x88, 16, 0),

	///*group1  base_addr: 0x0B100000*/
	MC_PIN_DRV_GRP(53, 0x80, 16, 0),
	MC_PIN_DRV_GRP(54, 0x84, 16, 0)
};

static const struct mc_pinctrl_devdata mc_pinctrl_data = {
	.pins = mc_pins,
	.npins = ARRAY_SIZE(mc_pins),
	.grp_desc = mc_drv_grp,
	.n_grp_cls = ARRAY_SIZE(mc_drv_grp),
	.pin_drv_grp = mc_pin_drv,
	.n_pin_drv_grps = ARRAY_SIZE(mc_pin_drv),
	.spec_pull_set = mc_spec_pull_set,
	.spec_smt_set = pin_smt_set,
};

static int mc_pinctrl_probe(struct platform_device *pdev)
{
	return mc_pctrl_init(pdev, &mc_pinctrl_data, NULL);
}

static const struct of_device_id mc_pctrl_match[] = {
	{
		.compatible = "molchip,mc-pinctrl",
	},
	{ }
};

static struct platform_driver mc_pinctrl_driver = {
	.probe = mc_pinctrl_probe,
	.driver = {
		.name = "molchip-mc-pinctrl",
		.of_match_table = mc_pctrl_match,
	},
};

static int __init mc_pinctrl_init(void)
{
	return platform_driver_register(&mc_pinctrl_driver);
}
arch_initcall(mc_pinctrl_init);
