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


u8 mc_mux_helper_get_parent(const struct mc_clk_common *clk_com,
					const struct mux_sel *mux)
{
	unsigned int reg;
	u8 parent;
	int num_parents;
	int i;

	regmap_read(clk_com->regmap, clk_com->reg, &reg);
	parent = reg >> mux->shift;
	parent &= (1 << mux->width) - 1;

	if (!mux->table)
		return parent;

	num_parents = clk_hw_get_num_parents(&clk_com->hw);

	for (i = 0; i < num_parents - 1; i++)
		if (parent >= mux->table[i] && parent < mux->table[i + 1])
			return i;

	return num_parents - 1;
}
EXPORT_SYMBOL_GPL(mc_mux_helper_get_parent);

static u8 mc_mux_get_parent(struct clk_hw *hw)
{
	struct clkmux *cm = hw_to_mc_mux(hw);

	return mc_mux_helper_get_parent(&cm->clk_com, &cm->mux);
}

int mc_mux_helper_set_parent(const struct mc_clk_common *clk_com,
					const struct mux_sel *mux,
					u8 index)
{
	unsigned int reg;

	if (mux->table)
		index = mux->table[index];

	regmap_read(clk_com->regmap, clk_com->reg, &reg);
	reg &= ~GENMASK(mux->width + mux->shift - 1, mux->shift);
	regmap_write(clk_com->regmap, clk_com->reg,
			  reg | (index << mux->shift));

	return 0;
}
EXPORT_SYMBOL_GPL(mc_mux_helper_set_parent);

static int mc_mux_set_parent(struct clk_hw *hw, u8 index)
{
	struct clkmux *cm = hw_to_mc_mux(hw);

	return mc_mux_helper_set_parent(&cm->clk_com, &cm->mux, index);
}

const struct clk_ops mc_mux_ops = {
	.get_parent = mc_mux_get_parent,
	.set_parent = mc_mux_set_parent,
	.determine_rate = __clk_mux_determine_rate,
};
EXPORT_SYMBOL_GPL(mc_mux_ops);


struct clk *mc_clk_register_clkmux(struct device *dev, const char *name,
				      const char * const *parent_name, u8 num_parents,
				      unsigned long flags,
				      struct regmap *regmap,
				      u32 reg, u8 shift, u32 width,
				      u8 clk_mux_flags, u32 *tables)
{
	struct clkmux *cmux;
	struct clk *clk;
	struct clk_init_data init;

	cmux = kzalloc(sizeof(*cmux), GFP_KERNEL);
	if (!cmux)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &mc_mux_ops;
	init.flags = flags;
	init.parent_names = parent_name;
	init.num_parents =  num_parents;

	cmux->clk_com.reg = reg;
	cmux->clk_com.regmap = regmap;
	cmux->mux.shift = shift;
	cmux->mux.width = width;
	cmux->clk_com.hw.init = &init;
	cmux->mux.table = tables;

	clk = clk_register(dev, &cmux->clk_com.hw);
	if (IS_ERR(clk))
		kfree(cmux);
	return clk;
}
EXPORT_SYMBOL_GPL(mc_clk_register_clkmux);
