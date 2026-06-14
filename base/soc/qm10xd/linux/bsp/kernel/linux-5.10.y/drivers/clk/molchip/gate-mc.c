/*
 *
 * Copyright (c) 2019 Molchip Limited.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/clk-provider.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/spinlock.h>

#include "clk.h"
#include <linux/clk-provider.h>
#include <linux/regmap.h>



static void clk_gate_toggle(const struct clkgate *cg, bool en)
{
	unsigned int reg;
	const struct mc_clk_common *clk_com = &cg->clk_com;
	bool set = cg->flags & CLK_GATE_SET_TO_DISABLE ? true : false;

	set ^= en;

	regmap_read(clk_com->regmap, clk_com->reg, &reg);

	if (set)
		reg |= cg->enable_mask;
	else
		reg &= ~cg->enable_mask;

	regmap_write(clk_com->regmap, clk_com->reg, reg);
}

static void clk_sc_gate_toggle(const struct clkgate *cg, bool en)
{
	bool set = cg->flags & CLK_GATE_SET_TO_DISABLE ? 1 : 0;
	unsigned int offset;
	const struct mc_clk_common *clk_com = &cg->clk_com;
	set ^= en;

	/*
	 * Each set/clear gate clock has three registers:
	 * cg->reg			- base register
	 * cg->reg + offset		- set register
	 * cg->reg + 2 * offset	- clear register
	 */
	offset = set ? cg->offset : cg->offset * 2;

	regmap_write(clk_com->regmap, clk_com->reg + offset,
			  cg->enable_mask);
}

static void clkgate_disable(struct clk_hw *hw)
{
	struct clkgate *cg = hw_to_mc_gate(hw);

	clk_gate_toggle(cg, false);
}

static int clkgate_enable(struct clk_hw *hw)
{
	struct clkgate *cg = hw_to_mc_gate(hw);

	clk_gate_toggle(cg, true);

	return 0;
}

static void sc_clkgate_disable(struct clk_hw *hw)
{
	struct clkgate *cg = hw_to_mc_gate(hw);

	clk_sc_gate_toggle(cg, false);
}

static int sc_clkgate_enable(struct clk_hw *hw)
{
	struct clkgate *cg = hw_to_mc_gate(hw);

	clk_sc_gate_toggle(cg, true);

	return 0;
}

static int clkgate_is_enabled(struct clk_hw *hw)
{
	struct clkgate *cg = hw_to_mc_gate(hw);
	struct mc_clk_common *clk_com = &cg->clk_com;
	unsigned int reg;

	regmap_read(clk_com->regmap, clk_com->reg, &reg);

	if (cg->flags & CLK_GATE_SET_TO_DISABLE)
		reg ^= cg->enable_mask;

	reg &= cg->enable_mask;

	return reg ? 1 : 0;
}
const struct clk_ops mc_gate_ops = {
	.disable	= clkgate_disable,
	.enable		= clkgate_enable,
	.is_enabled	= clkgate_is_enabled,
};
EXPORT_SYMBOL_GPL(mc_gate_ops);

const struct clk_ops mc_sc_gate_ops = {
	.disable	= sc_clkgate_disable,
	.enable		= sc_clkgate_enable,
	.is_enabled	= clkgate_is_enabled,
};
EXPORT_SYMBOL_GPL(mc_sc_gate_ops);

struct clk *mc_clk_register_clkgate(struct device *dev, const char *name,
				      const char *parent_name,
				      unsigned long flags, u32 reg,
				       struct regmap *regmap, u8 bit_idx,
				      u8 clk_gate_flags)
{
	struct clkgate *cclk;
	struct clk *clk;
	struct clk_init_data init;

	cclk = kzalloc(sizeof(*cclk), GFP_KERNEL);
	if (!cclk)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &mc_gate_ops;
	init.flags = flags;
	init.parent_names = (parent_name ? &parent_name : NULL);
	init.num_parents = (parent_name ? 1 : 0);

	cclk->clk_com.reg = reg;
	cclk->clk_com.regmap = regmap;
	cclk->enable_mask = BIT(bit_idx);
	cclk->flags = clk_gate_flags;
	cclk->clk_com.hw.init = &init;

	clk = clk_register(dev, &cclk->clk_com.hw);
	if (IS_ERR(clk))
		kfree(cclk);
	return clk;
}
EXPORT_SYMBOL_GPL(mc_clk_register_clkgate);
