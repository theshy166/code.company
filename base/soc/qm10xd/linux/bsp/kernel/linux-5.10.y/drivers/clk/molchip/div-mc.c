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


long mc_div_helper_round_rate(struct mc_clk_common *clk_com,
				const struct div_internal *div,
				unsigned long rate,
				unsigned long *parent_rate)
{
	return divider_round_rate(&clk_com->hw, rate, parent_rate,
				  NULL, div->width, 0);
}
EXPORT_SYMBOL_GPL(mc_div_helper_round_rate);

static long mc_div_round_rate(struct clk_hw *hw, unsigned long rate,
				unsigned long *parent_rate)
{
	struct clkdiv *cd = hw_to_mc_div(hw);

	return mc_div_helper_round_rate(&cd->clk_com, &cd->div, rate, parent_rate);
}

unsigned long mc_div_helper_recalc_rate(struct mc_clk_common *clk_com,
					const struct div_internal *div,
					unsigned long parent_rate)
{
	unsigned long val;
	unsigned int reg;

	regmap_read(clk_com->regmap, clk_com->reg, &reg);
	val = reg >> div->shift;
	val &= (1 << div->width) - 1;

	return divider_recalc_rate(&clk_com->hw, parent_rate, val, NULL, 0, div->width);
}
EXPORT_SYMBOL_GPL(mc_div_helper_recalc_rate);

static unsigned long mc_div_recalc_rate(struct clk_hw *hw,
					  unsigned long parent_rate)
{
	struct clkdiv *cd = hw_to_mc_div(hw);

	return mc_div_helper_recalc_rate(&cd->clk_com, &cd->div, parent_rate);
}

int mc_div_helper_set_rate(struct mc_clk_common *clk_com,
				struct div_internal *div,
			    unsigned long rate,
			    unsigned long parent_rate)
{
	unsigned long val;
	unsigned int reg;

	val = divider_get_val(rate, parent_rate, NULL,
			      div->width, 0);

	regmap_read(clk_com->regmap, clk_com->reg, &reg);
	reg &= ~GENMASK(div->width + div->shift - 1, div->shift);

	regmap_write(clk_com->regmap, clk_com->reg,
			  reg | (val << div->shift));

	return 0;

}
EXPORT_SYMBOL_GPL(mc_div_helper_set_rate);

static int mc_div_set_rate(struct clk_hw *hw, unsigned long rate,
			     unsigned long parent_rate)
{
	struct clkdiv *cd = hw_to_mc_div(hw);

	return mc_div_helper_set_rate(&cd->clk_com, &cd->div,
					rate, parent_rate);
}

const struct clk_ops mc_div_ops = {
	.recalc_rate = mc_div_recalc_rate,
	.round_rate = mc_div_round_rate,
	.set_rate = mc_div_set_rate,
};
EXPORT_SYMBOL_GPL(mc_div_ops);

