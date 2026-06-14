// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2005, Intec Automation Inc.
 * Copyright (C) 2014, Freescale Semiconductor, Inc.
 */

#include <linux/mtd/spi-nor.h>

#include "core.h"



static const struct flash_info dosilicon_parts[] = {
	{ "ds25m4ab", INFO(0xe54218, 0, 32 * 1024, 256,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB) },
	{ "ds25m4ba", INFO(0xe54219, 0, 64 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB | SPI_NOR_4B_OPCODES | SPI_NOR_SKIP_SFDP) },
	{ "ds25q4aa", INFO(0xe53118, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB) },
};

/**
 * dosilicon_set_4byte_addr_mode() - Set 4-byte address mode for dosilicon flashes.
 * @nor:	pointer to 'struct spi_nor'.
 * @enable:	true to enter the 4-byte address mode, false to exit the 4-byte
 *		address mode.
 *
 * Return: 0 on success, -errno otherwise.
 */
static int dosilicon_set_4byte_addr_mode(struct spi_nor *nor, bool enable)
{
	int ret;

	ret = spi_nor_set_4byte_addr_mode(nor, enable);
	if (ret || enable)
		return ret;

	/*
	 * On dosilicon W25Q256FV, leaving 4byte mode causes the Extended Address
	 * Register to be set to 1, so all 3-byte-address reads come from the
	 * second 16M. We must clear the register to enable normal behavior.
	 */
	ret = spi_nor_write_enable(nor);
	if (ret)
		return ret;

	ret = spi_nor_write_ear(nor, 0);
	if (ret)
		return ret;

	return spi_nor_write_disable(nor);
}

static void dosilicon_default_init(struct spi_nor *nor)
{
	nor->params->set_4byte_addr_mode = dosilicon_set_4byte_addr_mode;
}

static const struct spi_nor_fixups dosilicon_fixups = {
	.default_init = dosilicon_default_init,
};

const struct spi_nor_manufacturer spi_nor_dosilicon = {
	.name = "dosilicon",
	.parts = dosilicon_parts,
	.nparts = ARRAY_SIZE(dosilicon_parts),
	.fixups = &dosilicon_fixups,
};

