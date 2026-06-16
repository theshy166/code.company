/*
 * Copyright (c) 2019 MOLCHIP Inc.
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

#ifndef __PINCTRL_MC_COMMON_H
#define __PINCTRL_MC_COMMON_H

#include <linux/pinctrl/pinctrl.h>
#include <linux/regmap.h>
#include <linux/pinctrl/pinconf-generic.h>

#define MC_PINCTRL_NOT_SUPPORT	(0xffff)

struct mc_desc_function {
	const char *name;
	unsigned char muxval;
};

struct mc_desc_pin {
	struct pinctrl_pin_desc	pin;
	const struct mc_desc_function	*functions;
};

#define MC_PIN(_pin,  _pad, _chip,...)					\
	{							\
		.pin = _pin,					\
		.functions = (struct mc_desc_function[]){	\
			__VA_ARGS__, { } },			\
	}


#define MC_FUNCTION(_val, _name)				\
	{							\
		.muxval = _val,					\
		.name = _name,					\
	}

struct mc_pinctrl_group {
	const char	*name;
	unsigned long	config;
	unsigned	pin;
};

/**
 * struct mc_drv_group_desc - Provide driving group data.
 * @max_drv: The maximum current of this group.
 * @min_drv: The minimum current of this group.
 * @low_bit: The lowest bit of this group.
 * @high_bit: The highest bit of this group.
 * @step: The step current of this group.
 */
struct mc_drv_group_desc {
	unsigned char min_drv;
	unsigned char max_drv;
	unsigned char low_bit;
	unsigned char high_bit;
	unsigned char step;
};

#define MC_DRV_GRP(_min, _max, _low, _high, _step)	\
	{	\
		.min_drv = _min,	\
		.max_drv = _max,	\
		.low_bit = _low,	\
		.high_bit = _high,	\
		.step = _step,		\
	}

/**
 * struct mc_pin_drv_grp - Provide each pin driving info.
 * @pin: The pin number.
 * @offset: The offset of driving register for this pin.
 * @bit: The bit of driving register for this pin.
 * @grp: The group for this pin belongs to.
 */
struct mc_pin_drv_grp {
	unsigned int pin;
	unsigned int offset;
	unsigned char bit;
	unsigned char grp;
};

#define MC_PIN_DRV_GRP(_pin, _offset, _bit, _grp)	\
	{	\
		.pin = _pin,	\
		.offset = _offset,	\
		.bit = _bit,	\
		.grp = _grp,	\
	}

/**
 * struct mc_pin_spec_pupd_set_samereg
 * - For special pins' pull up/down setting which resides in same register
 * @pin: The pin number.
 * @offset: The offset of special pull up/down setting register.
 * @pupd_bit: The pull up/down bit in this register.
 * @r0_bit: The r0 bit of pull resistor.
 * @r1_bit: The r1 bit of pull resistor.
 */
struct mc_pin_spec_pupd_set_samereg {
	unsigned int pin;
	unsigned int offset;
	unsigned char pupd_bit;
	unsigned char r1_bit;
	unsigned char r0_bit;
};

#define MC_PIN_PUPD_SPEC_SR(_pin, _offset, _pupd, _r1, _r0)	\
	{	\
		.pin = _pin,	\
		.offset = _offset,	\
		.pupd_bit = _pupd,	\
		.r1_bit = _r1,		\
		.r0_bit = _r0,		\
	}

/**
 * struct mc_pin_ies_set - For special pins' ies and smt setting.
 * @start: The start pin number of those special pins.
 * @end: The end pin number of those special pins.
 * @offset: The offset of special setting register.
 * @bit: The bit of special setting register.
 */
struct mc_pin_smt_set {
	unsigned int pin;
	unsigned int offset;
	unsigned char bit;
};

#define MC_PIN_SMT_SPEC(_pin, _offset, _bit)	\
	{	\
		.pin = _pin,	\
		.bit = _bit,	\
		.offset = _offset,	\
	}

/**
 * struct mc_pinctrl_devdata - Provide HW GPIO related data.
 * @pins: An array describing all pins the pin controller affects.
 * @npins: The number of entries in @pins.
 *
 * @grp_desc: The driving group info.
 * @pin_drv_grp: The driving group for all pins.
 * @spec_pull_set: Each SoC may have special pins for pull up/down setting,
 *  these pins' pull setting are very different, they have separate pull
 *  up/down bit, R0 and R1 resistor bit, so they need special pull setting.
 *  If special setting is success, this should return 0, otherwise it should
 *  return non-zero value.
 * @spec_ies_smt_set: Some pins are irregular, their input enable and smt
 * control register are discontinuous, but they are mapping together. That
 * means when user set smt, input enable is set at the same time. So they
 * also need special control. If special control is success, this should
 * return 0, otherwise return non-zero value.
 * @spec_pinmux_set: In some cases, there are two pinmux functions share
 * the same value in the same segment of pinmux control register. If user
 * want to use one of the two functions, they need an extra bit setting to
 * select the right one.
 * @spec_dir_set: In very few SoCs, direction control registers are not
 * arranged continuously, they may be cut to parts. So they need special
 * dir setting.

 * @dir_offset: The direction register offset.
 * @pullen_offset: The pull-up/pull-down enable register offset.
 * @pinmux_offset: The pinmux register offset.
 *
 * @type1_start: Some chips have two base addresses for pull select register,
 *  that means some pins use the first address and others use the second. This
 *  member record the start of pin number to use the second address.
 * @type1_end: The end of pin number to use the second address.
 *
 * @port_shf: The shift between two registers.
 * @port_mask: The mask of register.
 * @port_align: Provide clear register and set register step.
 */
struct mc_pinctrl_devdata {
	const struct mc_desc_pin	*pins;
	unsigned int				npins;
	const struct mc_drv_group_desc	*grp_desc;
	unsigned int	n_grp_cls;
	const struct mc_pin_drv_grp	*pin_drv_grp;
	unsigned int	n_pin_drv_grps;
	int (*spec_pull_set)(struct regmap *reg, unsigned int pin,
			bool isup, unsigned int arg);
	int (*spec_smt_set)(struct regmap *reg, unsigned int pin,
			int value, enum pin_config_param arg);
	void (*spec_pinmux_set)(struct regmap *reg, unsigned int pin,
			unsigned int mode);
	void (*spec_dir_set)(unsigned int *reg_addr, unsigned int pin);
};

struct mc_pinctrl {
	struct regmap	*regmap0;
	struct regmap	*regmap1;
	struct regmap	*regmap2;
	struct pinctrl_desc pctl_desc;
	struct device           *dev;
	struct mc_pinctrl_group	*groups;
	unsigned			ngroups;
	const char          **grp_names;
	struct pinctrl_dev      *pctl_dev;
	const struct mc_pinctrl_devdata  *devdata;
	struct irq_domain	*domain;
	u32 *wake_mask;
	u32 *cur_mask;
};

int mc_pctrl_init(struct platform_device *pdev,
		const struct mc_pinctrl_devdata *data,
		struct regmap *regmap);

int mc_pctrl_spec_pull_set_samereg(struct regmap *regmap,
		const struct mc_pin_spec_pupd_set_samereg *pupd_infos,
		unsigned int info_num, unsigned int pin,
		bool isup, unsigned int r1r0);

int mc_pconf_spec_set_smt_range(struct regmap *regmap,
		const struct mc_pin_smt_set *smt_infos, unsigned int info_num,
		unsigned int pin, int value);

#endif /* __PINCTRL_MC_COMMON_H */
