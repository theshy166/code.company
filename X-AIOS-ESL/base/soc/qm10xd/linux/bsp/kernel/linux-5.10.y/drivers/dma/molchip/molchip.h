/*
 * Driver for the MOLCHIP DMA64 Controller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _DMA_MOLCHIP_H
#define _DMA_MOLCHIP_H

#include <linux/clk.h>
#include <linux/device.h>
#include <linux/dmaengine.h>
/*reg peri clk ctrl*/
#define REG_AHB_PERI_CLK_CTRL		0x0000

#define BIT_DMA_EB			(0x1<<1)
#define BIT_CKG_AXI_DMA_FORCE_EN	(0x1<<3)
#define BIT_CKG_AXI_DMA_AUTO_GATE_SEL	(0x1<<5)

/*reg ap peri soft rst*/
#define REG_AP_PERI_SOFT_RST		(0x0044)
#define BIT_DMA_SOFT_RST		(0x1 << 0)

/*reg vdsp clk rst ctrl*/
#define REG_VDSP_CLK_RST_CTRL		0x0020
#define BIT_CKG_VDSP_SYS_SEL(x)		((x & 0x3)<<0)

/*reg top clk cfg */
#define CKG_AP_SYS_CFG			0x0030
#define CKG_AP_SYS_SEL(x) 		((x & 0x3)<<0)

/**
 * struct dw_dma_chip - representation of MOLCHIP DMA64 controller hardware
 * @dev:		struct device of the DMA controller
 * @irq:		irq line
 * @regs:		memory mapped I/O space
 * @clk:		hclk clock
 * @dw:			struct dw_dma that is filed by dw_dma_probe()
 * @pdata:		pointer to platform data
 */
struct mc_dma64_chip {
	struct device	*dev;
	int		irq;
	void __iomem	*regs;
	void __iomem	*common_regs;
	void __iomem	*chan_regs;
	void __iomem	*req_cid_regs;
	struct regmap	*ap_sys_ahb;
	struct clk 	*dma_eb;
	struct clk	*dma_force;
	struct clk	*dma_auto;
	struct clk	*clk;
	struct mc_dma	*mc;

	const struct mc_dma64_data *pdata;
};


/**
 * struct mc_dma64_data - Controller configuration parameters
 * @nr_channels: Number of channels supported by hardware (max 8)
 * @is_nollp: The device channels does not support multi block transfers.
 * @chan_allocation_order: Allocate channels starting from 0 or 7
 * @chan_priority: Set channel priority increasing from 0 to 7 or 7 to 0.
 * @fragment_size: Maximum fragment_size supported by the controller
 * @nr_masters: Number of AHB masters supported by the controller
 * @data_width: Maximum data width supported by hardware per AHB master
 *		(in bytes, power of 2)
 */
struct mc_dma64_data {
	u32	nr_channels;
	bool		is_nollp;
#define CHAN_ALLOCATION_ASCENDING	0	/* 0 to 31 */
#define CHAN_ALLOCATION_DESCENDING	1	/* 31 to 0 */
	unsigned char	chan_allocation_order;
	unsigned char	nr_masters;
};

#endif
