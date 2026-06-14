#ifndef SPI_LCD_DMA_H
#define SPI_LCD_DMA_H

// SPDX-License-Identifier: GPL-2.0-only
/*
 * Special handling for DW DMA core
 *
 * Copyright (c) 2009, 2014 Intel Corporation.
 */

typedef void (*spi_dma_callback_func)(void);

int spi_lcd_dma_setup(uint64_t dma_base_addr, spi_dma_callback_func stDmaCallback);
int spi_lcd_dma_transfer(unsigned long long tx_buf, unsigned len);
void spi_lcd_dma_release(void);
#endif