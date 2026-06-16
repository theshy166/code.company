// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2005, Intec Automation Inc.
 * Copyright (C) 2014, Freescale Semiconductor, Inc.
 */

#include <linux/mtd/spi-nor.h>

#include "core.h"

static const struct flash_info fudanmicro_parts[] = {
	{ "FM25Q128AI3", INFO(0xa14018, 0, 32 * 1024, 512,
				SECT_4K |SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	}
};

const struct spi_nor_manufacturer spi_nor_fudanmicro = {
	.name = "fudanmicro",
	.parts = fudanmicro_parts,
	.nparts = ARRAY_SIZE(fudanmicro_parts),
};
