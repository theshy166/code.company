/*-
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 */
#ifndef __LINUX_MTD_SPI_NAND_H
#define __LINUX_MTD_SPI_NAND_H

/*
 * Standard SPI-NAND flash commands
 */
#define SPINAND_CMD_READ			0x13
#define SPINAND_CMD_READ_RDM			0x03
#define SPINAND_CMD_PROG_LOAD			0x02
#define SPINAND_CMD_PROG_RDM			0x84
#define SPINAND_CMD_PROG			0x10
#define SPINAND_CMD_ERASE_BLK			0xd8
#define SPINAND_CMD_WR_ENABLE			0x06
#define SPINAND_CMD_WR_DISABLE			0x04
#define SPINAND_CMD_READ_ID			0x9f
#define SPINAND_CMD_RESET			0xff
#define SPINAND_CMD_READ_REG			0x0f
#define SPINAND_CMD_WRITE_REG			0x1f

#define SPINAND_CMD_READ_CACHE_X2		0x3b
#define SPINAND_CMD_READ_CACHE_X4		0x6b
#define SPINAND_CMD_READ_CACHE_DUAL		0xbb
#define SPINAND_CMD_READ_CACHE_QUAD		0xeb

#define SPINAND_CMD_PROG_LOAD_X4		0x32
#define SPINAND_CMD_PROG_RDM_X4			0xC4 /*or 34*/

/* feature registers */
#define REG_BLOCK_LOCK			0xa0
#define REG_OTP				0xb0
#define REG_STATUS			0xc0/* timing */

/* status */
#define STATUS_OIP_MASK			0x01
#define STATUS_READY			(0 << 0)
#define STATUS_BUSY			(1 << 0)

#define STATUS_E_FAIL_MASK		0x04
#define STATUS_E_FAIL			(1 << 2)

#define STATUS_P_FAIL_MASK		0x08
#define STATUS_P_FAIL			(1 << 3)

/*OTP register defines*/
#define OTP_ECC_MASK			0X10
#define OTP_ECC_ENABLE			(1 << 4)
#define OTP_ENABLE			(1 << 6)
#define OTP_LOCK			(1 << 7)

/* block lock */
#define BL_ALL_LOCKED      0x38
#define BL_1_2_LOCKED      0x30
#define BL_1_4_LOCKED      0x28
#define BL_1_8_LOCKED      0x20
#define BL_1_16_LOCKED     0x18
#define BL_1_32_LOCKED     0x10
#define BL_1_64_LOCKED     0x08
#define BL_ALL_UNLOCKED    0

#define SPI_NAND_ECC_SHIFT		4

#define SPI_NAND_MT29F_ECC_MASK		3
#define SPI_NAND_MT29F_ECC_CORRECTED	1
#define SPI_NAND_MT29F_ECC_UNCORR	2
#define SPI_NAND_MT29F_ECC_RESERVED	3
#define SPI_NAND_MT29F_ECC_SHIFT	4

#define SPI_NAND_GD5F_ECC_MASK		7
#define SPI_NAND_GD5F_ECC_UNCORR	7
#define SPI_NAND_GD5F_ECC_SHIFT		4

#define SPINAND_MAX_ID_LEN		4

struct spi_nand_cmd {
	u8		cmd;
	u32		n_addr;		/* Number of address */
	u8		addr[3];	/* Reg Offset */
	u32		n_tx;		/* Number of tx bytes */
	u8		*tx_buf;	/* Tx buf */
	u8		tx_nbits;
	u32		n_rx;		/* Number of rx bytes */
	u8		*rx_buf;	/* Rx buf */
	u8		rx_nbits;
};

#define SPINAND_NEED_PLANE_SELECT	(1 << 0)

#define SPINAND_MFR_MICRON		0x2C
#define SPINAND_MFR_GIGADEVICE	0xC8

#endif /* __LINUX_MTD_SPI_NAND_H */


