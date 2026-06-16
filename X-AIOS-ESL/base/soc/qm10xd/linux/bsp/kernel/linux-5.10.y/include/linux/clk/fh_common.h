/*
 * mach/fh_common.h
 *
 * Copyright (C) 2019 Fullhan Microelectronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __FH_COMMON_H
#define __FH_COMMON_H

#include <linux/clk.h>
#include <linux/irqdomain.h>

/*change the scheduling policy and/or RT priority of `current` thread. */
void fh_setscheduler(int policy, int priority);

int fh_clk_enable(struct clk *clk);
unsigned long fh_clk_get_rate(struct clk *clk);
int fh_clk_set_rate(struct clk *clk, unsigned long rate);
int fh_clk_prepare(struct clk *clk);
void fh_clk_disable(struct clk *clk);

unsigned int fh_irq_create_mapping(struct irq_domain *domain,
                irq_hw_number_t hwirq);

#endif /* __FH_COMMON_H */
