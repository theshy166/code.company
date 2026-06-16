/*
 * mcxc01 pinctrl driver based on molchip pinctrl driver.
 * Copyright (c) 2019 MediaTek Inc.
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

#include <linux/io.h>
#include <linux/gpio/driver.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pinctrl/machine.h>
#include <linux/pinctrl/pinconf.h>
#include <linux/pinctrl/pinconf-generic.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/pinmux.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/bitops.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/pm.h>
#include <dt-bindings/pinctrl/mcxcxx.h>

#include "../core.h"
#include "../pinconf.h"
#include "../pinctrl-utils.h"
#include "pinctrl-mc-common.h"

#define GRP_0_PIN	18
#define GRP_1_PIN	40
#define GRP_2_PIN	52
#define GRP_3_PIN	54
#define PIN_MUX_REG_BASE	0x18
#define GPIO_MODE_BITS        3
#define GPIO_MAX_CNT	0x36

static const char * const mc_gpio_functions[] = {
	"func0", "func1", "func2", "func3", "func4", "func5", "func6", "func7"
};

/*
 * There are two base address for pull related configuration.
 */
static struct regmap *mc_get_regmap(struct mc_pinctrl *pctl,
		unsigned long pin)
{
	if (pin <=  GRP_0_PIN) {
		
		return pctl->regmap1;	
		
	} else if((pin > GRP_0_PIN) && (pin <=  GRP_1_PIN )){
	
		return pctl->regmap2;
		
	}else if((pin > GRP_1_PIN) && (pin <=  GRP_2_PIN )){
		
		return pctl->regmap1;
		
	}else if((pin > GRP_2_PIN) && (pin <=  GRP_3_PIN )){
		
		return pctl->regmap2;
	}else {

		printk("there are not find this pins,please check it !\n");
		return NULL;

	}
}
		

static int mc_pconf_set_smt(struct mc_pinctrl *pctl, unsigned pin,
		int value, enum pin_config_param arg)
{
	/**
	 * Due to some soc are not support smt config, add this special
	 * control to handle it.
	 */
	if (!pctl->devdata->spec_smt_set &&
			arg == PIN_CONFIG_INPUT_SCHMITT_ENABLE)
		return -EINVAL;

	/*
	 * Due to some pins are irregular, their input enable and smt
	 * control register are discontinuous, so we need this special handle.
	 */
	if (pctl->devdata->spec_smt_set) {
		return pctl->devdata->spec_smt_set(mc_get_regmap(pctl, pin),
			pin, arg, value);
	}

	return 0;
}

int mc_pconf_spec_set_smt_range(struct regmap *regmap,
		const struct mc_pin_smt_set *smt_infos, unsigned int info_num,
		unsigned int pin, int value)
{
	unsigned int i, reg_addr, bit;
	bool find = false;

	for (i = 0; i < info_num; i++) {
		if (pin == smt_infos[i].pin) {
			find = true;
			break;
		}
	}

	if (!find)
		return -EINVAL;

	reg_addr = smt_infos[i].offset;
	bit = BIT(smt_infos[i].bit);

	if (value)
		regmap_update_bits(regmap, reg_addr, 1 << 3, bit);
	else
		regmap_update_bits(regmap, reg_addr, 1 << 3, 0);

	return 0;
}

static const struct mc_pin_drv_grp *mc_find_pin_drv_grp_by_pin(
		struct mc_pinctrl *pctl,  unsigned long pin) {
	int i;
	
	for (i = 0; i < pctl->devdata->n_pin_drv_grps; i++) {
		const struct mc_pin_drv_grp *pin_drv =
				pctl->devdata->pin_drv_grp + i;
		if (pin == pin_drv->pin)
			return pin_drv;
	}

	return NULL;
}

static int mc_pconf_set_driving(struct mc_pinctrl *pctl,
		unsigned int pin, unsigned char driving)
{
	const struct mc_pin_drv_grp *pin_drv;
	unsigned int val;
	unsigned int bits, mask, shift;
	const struct mc_drv_group_desc *drv_grp;

	if (pin >= pctl->devdata->npins)
		return -EINVAL;

	pin_drv = mc_find_pin_drv_grp_by_pin(pctl, pin);
	if (!pin_drv || pin_drv->grp > pctl->devdata->n_grp_cls)
		return -EINVAL;

	drv_grp = pctl->devdata->grp_desc + pin_drv->grp;

	if (driving >= drv_grp->min_drv && driving <= drv_grp->max_drv
		&& !(driving % drv_grp->step)) {
		val = driving / drv_grp->step - 1;
		bits = drv_grp->high_bit - drv_grp->low_bit + 1;
		mask = BIT(bits) - 1;
		shift = drv_grp->low_bit;
		mask <<= shift;
		val <<= shift;
		return regmap_update_bits(mc_get_regmap(pctl, pin),
				pin_drv->offset, mask, val);
	}

	return -EINVAL;
}

int mc_pctrl_spec_pull_set_samereg(struct regmap *regmap,
		const struct mc_pin_spec_pupd_set_samereg *pupd_infos,
		unsigned int info_num, unsigned int pin,
		bool isup, unsigned int r1r0)
{
	unsigned int i;
	unsigned int offset, val;
	unsigned int bit_pupd, bit_r0, bit_r1;
	const struct mc_pin_spec_pupd_set_samereg *spec_pupd_pin;
	bool find = false;

	for (i = 0; i < info_num; i++) {
		if (pin == pupd_infos[i].pin) {
			find = true;
			break;
		}
	}

	if (!find)
		return -EINVAL;

	spec_pupd_pin = pupd_infos + i;
	offset = spec_pupd_pin->offset;

	bit_r1 = BIT(spec_pupd_pin->r1_bit);
	bit_pupd = BIT(spec_pupd_pin->pupd_bit);
	bit_r0 = BIT(spec_pupd_pin->r0_bit);

	switch (r1r0) {
	case MC_PUPD_SET_R1R0_00:
		val = 0;
		regmap_update_bits(regmap, offset, 0x7, val);
		break;
	case MC_PUPD_SET_R1R0_01:
		val = bit_pupd;
		regmap_update_bits(regmap, offset, 0x7, val);
		break;
	case MC_PUPD_SET_R1R0_10:
		val = bit_r0;
		regmap_update_bits(regmap, offset, 0x7, val);
		break;
	case MC_PUPD_SET_R1R0_11:
		val = bit_r0 | bit_pupd;
		regmap_update_bits(regmap, offset, 0x7, val);
		break;
	case MC_PUPD_SET_R2_0:
		val = 0;
		regmap_update_bits(regmap, offset, 0x7, val);
		break;
	case MC_PUPD_SET_R2_1:
		val = bit_r1;
		regmap_update_bits(regmap, offset, 0x7, val);
		break;
	default:
		return -EINVAL;
	}

	if (!isup)
		regmap_update_bits(regmap, offset, 0x7, 0x0);

	return 0;
}

static int mc_pconf_set_pull_select(struct mc_pinctrl *pctl,
		unsigned int pin, bool isup, unsigned int arg)
{
	int ret;

	/* Some pins' pull setting are very different,
	 * they have separate pull up/down bit, R0 and R1
	 * resistor bit, so we need this special handle.
	 */
	if (pctl->devdata->spec_pull_set) {
		ret = pctl->devdata->spec_pull_set(mc_get_regmap(pctl, pin),
			pin, isup, arg);
		if (!ret)
			return 0;
	}

	/* For generic pull config, default arg value should be 0 or 1. */
	if (arg != 0 && arg != 1) {
		dev_err(pctl->dev, "invalid pull-up argument %d on pin %d .\n",
			arg, pin);
		return -EINVAL;
	}

	return 0;
}

static int mc_pconf_parse_conf(struct pinctrl_dev *pctldev,
		unsigned int pin, enum pin_config_param param,
		enum pin_config_param arg)
{
	int ret = 0;
	struct mc_pinctrl *pctl = pinctrl_dev_get_drvdata(pctldev);

	switch (param) {
	case PIN_CONFIG_BIAS_DISABLE:
		ret = mc_pconf_set_pull_select(pctl, pin, false, arg);
		break;
	case PIN_CONFIG_BIAS_PULL_UP:
		ret = mc_pconf_set_pull_select(pctl, pin, true, arg);
		break;
	case PIN_CONFIG_BIAS_PULL_DOWN:
		ret = mc_pconf_set_pull_select(pctl, pin, true, arg);
		break;
	case PIN_CONFIG_INPUT_SCHMITT_ENABLE:
		ret = mc_pconf_set_smt(pctl, pin, arg, param);
		break;
	case PIN_CONFIG_DRIVE_STRENGTH:
		ret = mc_pconf_set_driving(pctl, pin, arg);
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}

static int mc_pconf_group_get(struct pinctrl_dev *pctldev,
				 unsigned group,
				 unsigned long *config)
{
	struct mc_pinctrl *pctl = pinctrl_dev_get_drvdata(pctldev);

	*config = pctl->groups[group].config;

	return 0;
}

static int mc_pconf_group_set(struct pinctrl_dev *pctldev, unsigned group,
				 unsigned long *configs, unsigned num_configs)
{
	struct mc_pinctrl *pctl = pinctrl_dev_get_drvdata(pctldev);
	struct mc_pinctrl_group *g = &pctl->groups[group];
	int i, ret;

	for (i = 0; i < num_configs; i++) {
		ret = mc_pconf_parse_conf(pctldev, g->pin,
			pinconf_to_config_param(configs[i]),
			pinconf_to_config_argument(configs[i]));
		if (ret < 0)
			return ret;

		g->config = configs[i];
	}

	return 0;
}

static const struct pinconf_ops mc_pconf_ops = {
	.pin_config_group_get	= mc_pconf_group_get,
	.pin_config_group_set	= mc_pconf_group_set,
};

static struct mc_pinctrl_group *
mc_pctrl_find_group_by_pin(struct mc_pinctrl *pctl, u32 pin)
{
	int i;

	for (i = 0; i < pctl->ngroups; i++) {
		struct mc_pinctrl_group *grp = pctl->groups + i;

		if (grp->pin == pin)
			return grp;
	}

	return NULL;
}

static const struct mc_desc_function *mc_pctrl_find_function_by_pin(
		struct mc_pinctrl *pctl, u32 pin_num, u32 fnum)
{
	const struct mc_desc_pin *pin = pctl->devdata->pins + pin_num;
	const struct mc_desc_function *func = pin->functions;

	while (func && func->name) {
		if (func->muxval == fnum)
			return func;
		func++;
	}

	return NULL;
}

static bool mc_pctrl_is_function_valid(struct mc_pinctrl *pctl,
		u32 pin_num, u32 fnum)
{
	int i;

	for (i = 0; i < pctl->devdata->npins; i++) {
		const struct mc_desc_pin *pin = pctl->devdata->pins + i;

		if (pin->pin.number == pin_num) {
			const struct mc_desc_function *func =
					pin->functions;

			while (func && func->name) {
				if (func->muxval == fnum)
					return true;
				func++;
			}

			break;
		}
	}

	return false;
}

static int mc_pctrl_dt_node_to_map_func(struct mc_pinctrl *pctl,
		u32 pin, u32 fnum, struct mc_pinctrl_group *grp,
		struct pinctrl_map **map, unsigned *reserved_maps,
		unsigned *num_maps)
{
	bool ret;

	if (*num_maps == *reserved_maps)
		return -ENOSPC;

	(*map)[*num_maps].type = PIN_MAP_TYPE_MUX_GROUP;
	(*map)[*num_maps].data.mux.group = grp->name;

	ret = mc_pctrl_is_function_valid(pctl, pin, fnum);
	if (!ret) {
		dev_err(pctl->dev, "invalid function %d on pin %d .\n",
				fnum, pin);
		return -EINVAL;
	}

	(*map)[*num_maps].data.mux.function = mc_gpio_functions[fnum];
	(*num_maps)++;

	return 0;
}

static int mc_pctrl_dt_subnode_to_map(struct pinctrl_dev *pctldev,
				      struct device_node *node,
				      struct pinctrl_map **map,
				      unsigned *reserved_maps,
				      unsigned *num_maps)
{
	struct property *pins;
	u32 pinfunc, pin, func;
	int num_pins, num_funcs, maps_per_pin;
	unsigned long *configs;
	unsigned int num_configs;
	bool has_config = 0;
	int i, err;
	unsigned reserve = 0;
	struct mc_pinctrl_group *grp;
	struct mc_pinctrl *pctl = pinctrl_dev_get_drvdata(pctldev);
		
	pins = of_find_property(node, "pinmux", NULL);
	if (!pins) {
		dev_err(pctl->dev, "missing pins property in node %s .\n",
				node->name);
		return -EINVAL;
	}

	err = pinconf_generic_parse_dt_config(node, pctldev, &configs,
		&num_configs);
	if (err){
			return err;
		}

	if (num_configs)
		has_config = 1;

	num_pins = pins->length / sizeof(u32);
	num_funcs = num_pins;
	maps_per_pin = 0;
	if (num_funcs)
		maps_per_pin++;
	if (has_config && num_pins >= 1)
		maps_per_pin++;

	if (!num_pins || !maps_per_pin) {
		err = -EINVAL;
		goto exit;
	}

	reserve = num_pins * maps_per_pin;

	err = pinctrl_utils_reserve_map(pctldev, map,
			reserved_maps, num_maps, reserve);
	if (err < 0)
		goto exit;

	for (i = 0; i < num_pins; i++) {
		err = of_property_read_u32_index(node, "pinmux",
				i, &pinfunc);
		if (err)
			goto exit;

		pin = MC_GET_PIN_NO(pinfunc);
		func = MC_GET_PIN_FUNC(pinfunc);

		if (pin >= pctl->devdata->npins ||
				func >= ARRAY_SIZE(mc_gpio_functions)) {
			dev_err(pctl->dev, "invalid pins value.\n");
			err = -EINVAL;
			goto exit;
		}

		grp = mc_pctrl_find_group_by_pin(pctl, pin);
		if (!grp) {
			dev_err(pctl->dev, "unable to match pin %d to group\n",
					pin);
			err = -EINVAL;
			goto exit;
		}

		err = mc_pctrl_dt_node_to_map_func(pctl, pin, func, grp, map,
				reserved_maps, num_maps);
		if (err < 0)
			goto exit;

		if (has_config) {
			err = pinctrl_utils_add_map_configs(pctldev, map,
					reserved_maps, num_maps, grp->name,
					configs, num_configs,
					PIN_MAP_TYPE_CONFIGS_GROUP);
			if (err < 0)
				goto exit;
		}
	}

	err = 0;

exit:
	kfree(configs);
	return err;
}

static int mc_pctrl_dt_node_to_map(struct pinctrl_dev *pctldev,
				 struct device_node *np_config,
				 struct pinctrl_map **map, unsigned *num_maps)
{
	struct device_node *np;
	unsigned reserved_maps;
	int ret;

	*map = NULL;
	*num_maps = 0;
	reserved_maps = 0;

	for_each_child_of_node(np_config, np) {
		ret = mc_pctrl_dt_subnode_to_map(pctldev, np, map,
				&reserved_maps, num_maps);
		if (ret < 0) {
			pinctrl_utils_free_map(pctldev, *map, *num_maps);
			of_node_put(np);
			return ret;
		}
	}

	return 0;
}

static int mc_pctrl_get_groups_count(struct pinctrl_dev *pctldev)
{
	struct mc_pinctrl *pctl = pinctrl_dev_get_drvdata(pctldev);

	return pctl->ngroups;
}

static const char *mc_pctrl_get_group_name(struct pinctrl_dev *pctldev,
					      unsigned group)
{
	struct mc_pinctrl *pctl = pinctrl_dev_get_drvdata(pctldev);

	return pctl->groups[group].name;
}

static int mc_pctrl_get_group_pins(struct pinctrl_dev *pctldev,
				      unsigned group,
				      const unsigned **pins,
				      unsigned *num_pins)
{
	struct mc_pinctrl *pctl = pinctrl_dev_get_drvdata(pctldev);

	*pins = (unsigned *)&pctl->groups[group].pin;
	*num_pins = 1;

	return 0;
}

static const struct pinctrl_ops mc_pctrl_ops = {
	.dt_node_to_map		= mc_pctrl_dt_node_to_map,
	.dt_free_map		= pinctrl_utils_free_map,
	.get_groups_count	= mc_pctrl_get_groups_count,
	.get_group_name		= mc_pctrl_get_group_name,
	.get_group_pins		= mc_pctrl_get_group_pins,
};

static int mc_pmx_get_funcs_cnt(struct pinctrl_dev *pctldev)
{
	return ARRAY_SIZE(mc_gpio_functions);
}

 const char *mc_pmx_get_func_name(struct pinctrl_dev *pctldev,
					   unsigned selector)
{

	return mc_gpio_functions[selector];
}

static int mc_pmx_get_func_groups(struct pinctrl_dev *pctldev,
				     unsigned function,
				     const char * const **groups,
				     unsigned * const num_groups)
{
	 

	struct mc_pinctrl *pctl = pinctrl_dev_get_drvdata(pctldev);

	*groups = pctl->grp_names;
	*num_groups = pctl->ngroups;

	return 0;
}

static int mc_pmx_set_mode(struct pinctrl_dev *pctldev,
		unsigned long pin, unsigned long mode)
{
	unsigned int reg_addr;
	unsigned int mask;
	u32 magic_pin_num = 0;
	struct mc_pinctrl *pctl = pinctrl_dev_get_drvdata(pctldev);

	mask = (1L << GPIO_MODE_BITS) - 1;
	mode &= mask;

	if (pin < (GPIO_MAX_CNT + 1)) {
		reg_addr = PIN_MUX_REG_BASE + (pin * 4);
	}else {
		magic_pin_num = 1;
	}
	if (!magic_pin_num)
		return regmap_update_bits(pctl->regmap0, reg_addr, mask, mode);
	else {
		printk("There are not find pins ,please checkout it \n");
		return 0;
	}
}

static int mc_pmx_set_mux(struct pinctrl_dev *pctldev,
			    unsigned function,
			    unsigned group)
{
	bool ret;
	const struct mc_desc_function *desc;
	struct mc_pinctrl *pctl = pinctrl_dev_get_drvdata(pctldev);
	struct mc_pinctrl_group *g = pctl->groups + group;

	ret = mc_pctrl_is_function_valid(pctl, g->pin, function);
	if (!ret) {
		dev_err(pctl->dev, "invalid function %d on group %d .\n",
				function, group);
		return -EINVAL;
	}

	desc = mc_pctrl_find_function_by_pin(pctl, g->pin, function);
	if (!desc)
		return -EINVAL;
	mc_pmx_set_mode(pctldev, g->pin, desc->muxval);

	return 0;
}

static const struct pinmux_ops mc_pmx_ops = {
	.get_functions_count	= mc_pmx_get_funcs_cnt,
	.get_function_name	= mc_pmx_get_func_name,
	.get_function_groups	= mc_pmx_get_func_groups,
	.set_mux		= mc_pmx_set_mux,
};

static int mc_pctrl_build_state(struct platform_device *pdev)
{
	struct mc_pinctrl *pctl = platform_get_drvdata(pdev);
	int i;

	pctl->ngroups = pctl->devdata->npins;

	/* Allocate groups */
	pctl->groups = devm_kcalloc(&pdev->dev, pctl->ngroups,
				    sizeof(*pctl->groups), GFP_KERNEL);
	if (!pctl->groups)
		return -ENOMEM;

	/* We assume that one pin is one group, use pin name as group name. */
	pctl->grp_names = devm_kcalloc(&pdev->dev, pctl->ngroups,
				       sizeof(*pctl->grp_names), GFP_KERNEL);
	if (!pctl->grp_names)
		return -ENOMEM;

	
	for (i = 0; i < pctl->devdata->npins; i++) {
		const struct mc_desc_pin *pin = pctl->devdata->pins + i;
		struct mc_pinctrl_group *group = pctl->groups + i;

		group->name = pin->pin.name;
		group->pin = pin->pin.number;

		pctl->grp_names[i] = pin->pin.name;
	}

	return 0;
}

int mc_pctrl_init(struct platform_device *pdev,
		const struct mc_pinctrl_devdata *data,
		struct regmap *regmap)
{
	struct pinctrl_pin_desc *pins;
	struct mc_pinctrl *pctl;
	struct device_node *np = pdev->dev.of_node, *node;
	struct property *prop;
	int i, ret;

	pctl = devm_kzalloc(&pdev->dev, sizeof(*pctl), GFP_KERNEL);
	if (!pctl)
		return -ENOMEM;

	platform_set_drvdata(pdev, pctl);

	prop = of_find_property(np, "pins-are-numbered", NULL);
	if (!prop) {
		dev_err(&pdev->dev, "only support pins-are-numbered format\n");
		return -EINVAL;
	}

	node = of_parse_phandle(np, "molchip,pctl-regmap", 0);
	if (node) {
		pctl->regmap0 = syscon_node_to_regmap(node);
		if (IS_ERR(pctl->regmap0))
			return PTR_ERR(pctl->regmap0);
	} else if (regmap) {
		pctl->regmap0  = regmap;
	} else {
		dev_err(&pdev->dev, "Pinctrl node has not register regmap.\n");
		return -EINVAL;
	}


	node = of_parse_phandle(np, "molchip,pctl-regmap", 1);
	if (node) {
		pctl->regmap1 = syscon_node_to_regmap(node);
		if (IS_ERR(pctl->regmap1))
			return PTR_ERR(pctl->regmap1);
	}

	node = of_parse_phandle(np, "molchip,pctl-regmap", 2);
	if (node) {
		pctl->regmap2 = syscon_node_to_regmap(node);
		if (IS_ERR(pctl->regmap2))
			return PTR_ERR(pctl->regmap2);
	}

	pctl->devdata = data;
	ret = mc_pctrl_build_state(pdev);
	if (ret) {
		dev_err(&pdev->dev, "build state failed: %d\n", ret);
		return -EINVAL;
	}

	pins = devm_kcalloc(&pdev->dev, pctl->devdata->npins, sizeof(*pins),
			    GFP_KERNEL);
	if (!pins)
		return -ENOMEM;

	for (i = 0; i < pctl->devdata->npins; i++)
		pins[i] = pctl->devdata->pins[i].pin;

	pctl->pctl_desc.name = dev_name(&pdev->dev);
	pctl->pctl_desc.owner = THIS_MODULE;
	pctl->pctl_desc.pins = pins;
	pctl->pctl_desc.npins = pctl->devdata->npins;
	pctl->pctl_desc.confops = &mc_pconf_ops;
	pctl->pctl_desc.pctlops = &mc_pctrl_ops;
	pctl->pctl_desc.pmxops = &mc_pmx_ops;
	pctl->dev = &pdev->dev;

	pctl->pctl_dev = devm_pinctrl_register(&pdev->dev, &pctl->pctl_desc,
					       pctl);
	if (IS_ERR(pctl->pctl_dev)) {
		dev_err(&pdev->dev, "couldn't register pinctrl driver\n");
		return PTR_ERR(pctl->pctl_dev);
	}


	return 0;
}
