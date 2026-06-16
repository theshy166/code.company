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


static long mc_comp_round_rate(struct clk_hw *hw, unsigned long rate,
				unsigned long *parent_rate)
{
	struct clkcomp *cc = hw_to_mc_compc(hw);

	return mc_div_helper_round_rate(&cc->clk_com, &cc->div,
					 rate, parent_rate);
}

static unsigned long mc_comp_recalc_rate(struct clk_hw *hw,
					  unsigned long parent_rate)
{
	struct clkcomp *cc = hw_to_mc_compc(hw);

	return mc_div_helper_recalc_rate(&cc->clk_com, &cc->div, parent_rate);
}

static int mc_comp_set_rate(struct clk_hw *hw, unsigned long rate,
			     unsigned long parent_rate)
{
	struct clkcomp *cc = hw_to_mc_compc(hw);

	return mc_div_helper_set_rate(&cc->clk_com, &cc->div,
				       rate, parent_rate);
}

static u8 mc_comp_get_parent(struct clk_hw *hw)
{
	struct clkcomp *cc = hw_to_mc_compc(hw);

	return mc_mux_helper_get_parent(&cc->clk_com, &cc->mux);
}

static int mc_comp_set_parent(struct clk_hw *hw, u8 index)
{
	struct clkcomp *cc = hw_to_mc_compc(hw);

	return mc_mux_helper_set_parent(&cc->clk_com, &cc->mux, index);
}

static int mc_comp_determine_rate(struct clk_hw *hw,
					struct clk_rate_request *req)
{
	struct clk_hw *parent;
	unsigned long parent_rate;
	long tmp_rate, best_rate = 0;
	unsigned long rate_diff;
	unsigned long best_rate_diff = ULONG_MAX;
	long rate = req->rate;
	int i;
	int ret;

	req->best_parent_hw = NULL;

	for (i = 0; i < clk_hw_get_num_parents(hw); i++) {
		parent = clk_hw_get_parent_by_index(hw, i);
		if (!parent)
			continue;

		ret = clk_hw_set_parent(hw, parent);
		if (ret)
			continue;

		if (clk_hw_get_flags(hw) & CLK_SET_RATE_PARENT)
			parent_rate = clk_hw_round_rate(parent, rate);
		else
			parent_rate = clk_hw_get_rate(parent);

		tmp_rate = mc_comp_round_rate(hw, req->rate,
						&parent_rate);
		if (tmp_rate < 0)
			continue;

		rate_diff = abs(req->rate - tmp_rate);

		if (!rate_diff || !req->best_parent_hw
			       || best_rate_diff > rate_diff) {
			req->best_parent_hw = parent;
			req->best_parent_rate = parent_rate;
			best_rate_diff = rate_diff;
			best_rate = tmp_rate;
		}

		if (!rate_diff)
			return 0;
	}

	req->rate = best_rate;
	return 0;
}

const struct clk_ops mc_comp_ops = {
	.get_parent	= mc_comp_get_parent,
	.set_parent	= mc_comp_set_parent,
    .determine_rate = mc_comp_determine_rate,

	.round_rate	= mc_comp_round_rate,
	.recalc_rate	= mc_comp_recalc_rate,
	.set_rate	= mc_comp_set_rate,
};
EXPORT_SYMBOL_GPL(mc_comp_ops);

struct clk *mc_clk_register_composite(struct device *dev, const char *name,
				      const char * const *parent_name, u8 num_parents,
				      unsigned long flags, struct regmap *regmap,
				      u32 reg,
				      u8 mux_shift, u8 mux_width,
				      u8 div_shift, u8 div_width)
{
	struct clkcomp *ccomp;
	struct clk *clk;
	struct clk_init_data init;

	ccomp = kzalloc(sizeof(*ccomp), GFP_KERNEL);
	if (!ccomp)
		return ERR_PTR(-ENOMEM);

	init.name = name;
	init.ops = &mc_comp_ops;
	init.flags = flags;
	init.parent_names = parent_name;
	init.num_parents =  num_parents;


	ccomp->mux.shift = mux_shift;
	ccomp->mux.width = mux_width;
	ccomp->div.shift = div_shift;
	ccomp->div.width = div_width;
	ccomp->clk_com.reg = reg;
	ccomp->clk_com.regmap= regmap;
	ccomp->clk_com.hw.init = &init;

	clk = clk_register(dev, &ccomp->clk_com.hw);
	if (IS_ERR(clk))
		kfree(ccomp);

	return clk;
}
EXPORT_SYMBOL_GPL(mc_clk_register_composite);
