#ifndef SPI_LCD_DMA_H
#define SPI_LCD_DMA_H

// SPDX-License-Identifier: GPL-2.0-only
/*
 * Special handling for DW DMA core
 *
 * Copyright (c) 2009, 2014 Intel Corporation.
 */
#if defined(__KERNEL__)
#include <linux/completion.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/irqreturn.h>
#include <linux/jiffies.h>
#include <linux/pci.h>
#include <linux/platform_data/dma-dw.h>
#include <linux/spi/spi.h>
#include <linux/types.h>
//#include "../../lcdmgr_log.h"
#endif

typedef void (*spi_dma_callback_func)(void);

int spi_lcd_dma_setup(uint64_t dma_base_addr, spi_dma_callback_func stDmaCallback);
int spi_lcd_dma_transfer(unsigned long long tx_buf, unsigned len);
void spi_lcd_dma_release(void);
#endif