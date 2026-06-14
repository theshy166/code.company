/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef __ASM_ARM_ARCH_CLK_H__
#define __ASM_ARM_ARCH_CLK_H__



void molchip_systimer_clk_enable(void);
void molchip_uart_clk_enable(unsigned int port);
void molchip_spic_clk_enable(void);
void molchip_dma_clk_enable(void);



#endif /* __ASM_ARM_ARCH_CLK_H__ */

