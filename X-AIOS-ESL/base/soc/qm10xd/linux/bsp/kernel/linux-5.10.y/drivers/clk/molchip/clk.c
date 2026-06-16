/*
 * Molchip clock driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <linux/kernel.h>
#include <linux/clkdev.h>
#include <linux/clk-provider.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include "clk.h"


static const struct regmap_config mc_clk_regmap_config = {
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
	.max_register	= 0xffff,
	.fast_io	= true,
};

struct mc_clock_data *mc_clk_init(struct device *dev,
					     int nr_clks)
{
	struct mc_clock_data *clk_data;
	struct clk **clk_table;
	void __iomem *base;
	struct regmap *regmap;
	struct device_node *np = dev->of_node;

	if (of_find_property(np, "mc,syscon", NULL)) {
		regmap = syscon_regmap_lookup_by_phandle(np, "mc,syscon");
		if (IS_ERR_OR_NULL(regmap)) {
			dev_err(dev, "%s: failed to get syscon regmap\n", __func__);
			goto err;
		}
	} else {
		base = of_iomap(np, 0);
		if (!base) {
			dev_err(dev, "%s: failed to map clock registers\n", __func__);
			goto err;
		}
		regmap = devm_regmap_init_mmio(dev, base,
					       &mc_clk_regmap_config);
		if (IS_ERR_OR_NULL(regmap)) {
			dev_err(dev, "failed to init regmap\n");
			goto err;
		}

	}

	clk_data = kzalloc(sizeof(*clk_data), GFP_KERNEL);
	if (!clk_data)
		goto err;

	clk_data->regmap = regmap;

	clk_table = kcalloc(nr_clks, sizeof(*clk_table), GFP_KERNEL);
	if (!clk_table)
		goto err_data;

	clk_data->clk_data.clks = clk_table;
	clk_data->clk_data.clk_num = nr_clks;
	of_clk_add_provider(np, of_clk_src_onecell_get, &clk_data->clk_data);
	return clk_data;
err_data:
	kfree(clk_data);
err:
	return NULL;
}
EXPORT_SYMBOL_GPL(mc_clk_init);

int mc_clk_register_fixed_rate(const struct mc_fixed_rate_clock *clks,
					 int nums, struct mc_clock_data *data)
{
	struct clk *clk;
	int i;

	for (i = 0; i < nums; i++) {
		clk = clk_register_fixed_rate(NULL, clks[i].name,
					      clks[i].parent_name,
					      clks[i].flags,
					      clks[i].fixed_rate);
		if (IS_ERR(clk)) {
			pr_err("%s: failed to register clock %s\n",
			       __func__, clks[i].name);
			goto err;
		}
		data->clk_data.clks[clks[i].id] = clk;
	}

	return 0;

err:
	while (i--)
		clk_unregister_fixed_rate(data->clk_data.clks[clks[i].id]);

	return PTR_ERR(clk);
}
EXPORT_SYMBOL_GPL(mc_clk_register_fixed_rate);

int mc_clk_register_fixed_factor(const struct mc_fixed_factor_clock *clks,
					   int nums,
					   struct mc_clock_data *data)
{
	struct clk *clk;
	int i;

	for (i = 0; i < nums; i++) {
		clk = clk_register_fixed_factor(NULL, clks[i].name,
						clks[i].parent_name,
						clks[i].flags, clks[i].mult,
						clks[i].div);
		if (IS_ERR(clk)) {
			pr_err("%s: failed to register clock %s\n",
			       __func__, clks[i].name);
			goto err;
		}
		data->clk_data.clks[clks[i].id] = clk;
	}

	return 0;

err:
	while (i--)
		clk_unregister_fixed_factor(data->clk_data.clks[clks[i].id]);

	return PTR_ERR(clk);
}
EXPORT_SYMBOL_GPL(mc_clk_register_fixed_factor);

int mc_clk_register_mux(const struct mc_mux_clock *clks,
				  int nums, struct mc_clock_data *data)
{
	struct clk *clk;
	struct regmap *regmap = data->regmap;

	int i;

	for (i = 0; i < nums; i++) {
		clk = mc_clk_register_clkmux(NULL, clks[i].name,
					clks[i].parent_names,
					clks[i].num_parents, clks[i].flags,
					regmap, clks[i].offset, clks[i].shift,
					clks[i].width, clks[i].mux_flags,
					clks[i].table);

		if (IS_ERR(clk)) {
			pr_err("%s: failed to register clock %s\n",
			       __func__, clks[i].name);
			continue;
		}

		if (clks[i].alias)
			clk_register_clkdev(clk, clks[i].alias, NULL);

		data->clk_data.clks[clks[i].id] = clk;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(mc_clk_register_mux);


int mc_clk_register_gate(const struct mc_gate_clock *clks,
				       int nums, struct mc_clock_data *data)
{
	struct clk *clk;
	struct regmap *regmap = data->regmap;
	int i;

	for (i = 0; i < nums; i++) {
		clk = mc_clk_register_clkgate(NULL,clks[i].name,
						clks[i].parent_name,
						clks[i].flags,
						clks[i].offset,
						regmap,
						clks[i].bit_idx,
						clks[i].gate_flags);
		if (IS_ERR(clk)) {
			pr_err("%s: failed to register clock %s\n",
			       __func__, clks[i].name);
			continue;
		}

		if (clks[i].alias)
			clk_register_clkdev(clk, clks[i].alias, NULL);

		data->clk_data.clks[clks[i].id] = clk;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(mc_clk_register_gate);

int mc_clk_register_composites(const struct mc_composite *mcs,
		int num, struct mc_clock_data *data)
{
	struct clk *clk;
	int i;
	struct regmap *regmap = data->regmap;

	for (i = 0; i < num; i++) {

		clk = mc_clk_register_composite(NULL, mcs[i].name,
					mcs[i].parent_names, mcs[i].num_parents,
					mcs[i].flags, regmap,
					mcs[i].reg,
					mcs[i].mux_shift, mcs[i].mux_width,
					mcs[i].divider_shift, mcs[i].divider_width);

		if (IS_ERR(clk)) {
			pr_err("%s: Failed to register clock %s\n",
					__func__, mcs[i].name);
			continue;
		}

		if (mcs[i].alias)
			clk_register_clkdev(clk, mcs[i].alias, NULL);

		data->clk_data.clks[mcs[i].id] = clk;
	}
	return 0;

}
EXPORT_SYMBOL_GPL(mc_clk_register_composites);
