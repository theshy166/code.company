/*
 * mclicon Hi3620 clock gate driver
 *
 * Copyright (c) 2012-2013 mclicon Limited.
 * Copyright (c) 2012-2013 Linaro Limited.
 *
 * Author: Haojian Zhuang <haojian.zhuang@linaro.org>
 *	   Xin Li <li.xin@linaro.org>
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

#ifndef	__mc_CLK_H
#define	__mc_CLK_H

#include <linux/clk-provider.h>
#include <linux/io.h>
#include <linux/spinlock.h>

struct platform_device;

struct mc_clock_data {
	struct clk_onecell_data	clk_data;
	struct regmap	*regmap;
};

struct mc_fixed_rate_clock {
	unsigned int		id;
	char			*name;
	const char		*parent_name;
	unsigned long		flags;
	unsigned long		fixed_rate;
};

struct mc_fixed_factor_clock {
	unsigned int		id;
	char			*name;
	const char		*parent_name;
	unsigned long		mult;
	unsigned long		div;
	unsigned long		flags;
};

struct mc_mux_clock {
	unsigned int		id;
	const char		*name;
	const char		*const *parent_names;
	u8			num_parents;
	unsigned long		flags;
	unsigned long		offset;
	u8			shift;
	u8			width;
	u8			mux_flags;
	u32			*table;
	const char		*alias;
};

struct mc_composite {
	int id;
	const char *name;
	const char * const *parent_names;
	u8 num_parents;
	unsigned long flags;

	unsigned long reg;

	signed char mux_shift;
	signed char mux_width;

	signed char divider_shift;
	signed char divider_width;
	const char		*alias;
};

struct mc_divider_clock {
	unsigned int		id;
	const char		*name;
	const char		*parent_name;
	unsigned long		flags;
	unsigned long		offset;
	u8			shift;
	u8			width;
	u8			div_flags;
	struct clk_div_table	*table;
	const char		*alias;
};


struct mc_gate_clock {
	unsigned int		id;
	const char		*name;
	const char		*parent_name;
	unsigned long		flags;
	unsigned long		offset;
	u8			bit_idx;
	u8			gate_flags;
	const char		*alias;
};

struct mc_clk_common {
	struct regmap	*regmap;
	u32		reg;
	struct clk_hw	hw;
};

struct clkgate {
	struct mc_clk_common clk_com;
	u32 enable_mask;
	u16 flags;
	u16 offset;
};

struct mux_sel {
	u8	 shift;	/* bits in enable/disable register */
	u8	 width;
	u32  *table;
};

struct clkmux {
	struct mc_clk_common clk_com;
	struct mux_sel mux;
};

struct div_internal {
	u8 shift;
	u8 width;
};

struct clkdiv {
	struct mc_clk_common clk_com;
	struct div_internal div;
};

struct clkcomp {
	struct mc_clk_common clk_com;
	struct div_internal div;
	struct mux_sel mux;
};

static inline struct mc_clk_common *
	hw_to_mc_clk_common(const struct clk_hw *hw)
{
	return container_of(hw, struct mc_clk_common, hw);
}

static inline struct clkgate *hw_to_mc_gate(const struct clk_hw *hw)
{
	struct mc_clk_common *clk_com = hw_to_mc_clk_common(hw);

	return container_of(clk_com, struct clkgate, clk_com);
}

static inline struct clkmux *hw_to_mc_mux(const struct clk_hw *hw)
{
	struct mc_clk_common *clk_com = hw_to_mc_clk_common(hw);

	return container_of(clk_com, struct clkmux, clk_com);
}

static inline struct clkdiv *hw_to_mc_div(const struct clk_hw *hw)
{
	struct mc_clk_common *clk_com = hw_to_mc_clk_common(hw);

	return container_of(clk_com, struct clkdiv, clk_com);
}


static inline struct clkcomp *hw_to_mc_compc(const struct clk_hw *hw)
{
	struct mc_clk_common *clk_com = hw_to_mc_clk_common(hw);

	return container_of(clk_com, struct clkcomp, clk_com);
}

extern const struct clk_ops mc_mux_ops;
extern const struct clk_ops mc_gate_ops;
extern const struct clk_ops mc_div_ops;

#define MUX_DIV_FLAGS(_id, _name, _parents, _reg, _shift, _width,	\
		_div_shift,div_width, _flags) {		\
		.id = _id,							\
		.name = _name,						\
		.reg = _reg,						\
		.mux_shift = _shift,				\
		.mux_width = _width,				\
		.divider_shift = _div_shift,		\
		.divider_width = div_width,			\
		.parent_names = _parents,			\
		.num_parents = ARRAY_SIZE(_parents),\
		.flags = _flags,					\
	}

struct mc_clock_data *mc_clk_alloc(struct platform_device *, int);
struct mc_clock_data *mc_clk_init(struct device *, int);
int mc_clk_register_fixed_rate(const struct mc_fixed_rate_clock *,
				int, struct mc_clock_data *);
int mc_clk_register_fixed_factor(const struct mc_fixed_factor_clock *,
				int, struct mc_clock_data *);
int mc_clk_register_mux(const struct mc_mux_clock *, int,
				struct mc_clock_data *);
int mc_clk_register_divider(const struct mc_divider_clock *,
				int, struct mc_clock_data *);
int mc_clk_register_gate(const struct mc_gate_clock *,
				int, struct mc_clock_data *);
int mc_clk_register_composites(const struct mc_composite *mcs,
		int num, struct mc_clock_data *data);
struct clk *mc_clk_register_clkgate(struct device *dev, const char *name,
				      const char *parent_name,
				      unsigned long flags, u32 reg,
				       struct regmap *regmap, u8 bit_idx,
				      u8 clk_gate_flags);
struct clk *mc_clk_register_clkmux(struct device *dev, const char *name,
				      const char * const *parent_name, u8 num_parents,
				      unsigned long flags,
				      struct regmap *regmap,
				      u32 reg, u8 shift, u32 width,
				      u8 clk_mux_flags, u32 *tables);
struct clk *mc_clk_register_composite(struct device *dev, const char *name,
				      const char * const *parent_name, u8 num_parents,
				      unsigned long flags, struct regmap *regmap,
				      u32 reg,
				      u8 mux_shift, u8 mux_width,
				      u8 div_shift, u8 div_width);
long mc_div_helper_round_rate(struct mc_clk_common *clk_com,
				const struct div_internal *div,
				unsigned long rate,
				unsigned long *parent_rate);
unsigned long mc_div_helper_recalc_rate(struct mc_clk_common *clk_com,
					const struct div_internal *div,
					unsigned long parent_rate);
int mc_div_helper_set_rate(struct mc_clk_common *clk_com,
				struct div_internal *div,
			    unsigned long rate,
			    unsigned long parent_rate);
u8 mc_mux_helper_get_parent(const struct mc_clk_common *clk_com,
					const struct mux_sel *mux);
int mc_mux_helper_set_parent(const struct mc_clk_common *clk_com,
					const struct mux_sel *mux,
					u8 index);
#define mc_clk_unregister(type) \
static inline \
void mc_clk_unregister_##type(const struct mc_##type##_clock *clks, \
				int nums, struct mc_clock_data *data) \
{ \
	struct clk **clocks = data->clk_data.clks; \
	int i; \
	for (i = 0; i < nums; i++) { \
		int id = clks[i].id; \
		if (clocks[id])  \
			clk_unregister_##type(clocks[id]); \
	} \
}

mc_clk_unregister(fixed_rate)
mc_clk_unregister(fixed_factor)
mc_clk_unregister(mux)
mc_clk_unregister(divider)
mc_clk_unregister(gate)

void mc_register_reset_controller(struct device_node *np,
			unsigned int num_regs, int regofs);

#endif	/* __mc_CLK_H */
