#ifndef __SPI_NAND_FLASH_H_
#define __SPI_NAND_FLASH_H_

/*
 * Standard SPI-NAND flash commands
 */
#define SPINAND_CMD_READ		0x13
#define SPINAND_CMD_READ_RDM		0x03
#define SPINAND_CMD_PROG_LOAD		0x02
#define SPINAND_CMD_PROG_RDM		0x84
#define SPINAND_CMD_PROG		0x10
#define SPINAND_CMD_ERASE_BLK		0xd8
#define SPINAND_CMD_WR_ENABLE		0x06
#define SPINAND_CMD_WR_DISABLE		0x04
#define SPINAND_CMD_READ_ID		0x9f
#define SPINAND_CMD_RESET		0xff
#define SPINAND_CMD_READ_REG		0x0f
#define SPINAND_CMD_WRITE_REG		0x1f

#define SPINAND_CMD_READ_CACHE_X2	0x3b
#define SPINAND_CMD_READ_CACHE_X4	0x6b
#define SPINAND_CMD_READ_CACHE_DUAL	0xbb
#define SPINAND_CMD_READ_CACHE_QUAD	0xeb

#define SPINAND_CMD_PROG_LOAD_X4	0x32
#define SPINAND_CMD_PROG_RDM_X4		0xC4	/*or 34*/

/* feature registers */
#define REG_BLOCK_LOCK			0xa0
#define REG_OTP				0xb0
#define REG_STATUS			0xc0	/* timing */

/* status */
#define STATUS_OIP_MASK			0x01
#define STATUS_READY			(0 << 0)
#define STATUS_BUSY			(1 << 0)

#define STATUS_E_FAIL_MASK		0x04
#define STATUS_E_FAIL			(1 << 2)

#define STATUS_P_FAIL_MASK		0x08
#define STATUS_P_FAIL			(1 << 3)

#define OTP_ECC_EN			(1 << 4)

#define NAND_READ_WAIT_TIME		(2000)

#define NAND_INFO_OFFSET		(24)

struct spi_nand_ident_info {
	u32 main_size;
	u32 oob_size;
	u32 ecc_mode;
	u32 ecc_pos;
};

struct spi_nand_info {
	u32 main_size;
	u32 oob_size;
	u32 ecc_mode;
	u32 ecc_pos;
	u8  ecc[48];
	u32 sector_num;
	u32 info_size;
	u32 block_page;
};

int spi_nand_boot(void);
int spi_nand_init(void *data);
int spi_nand_erase(u32 addr , unsigned int unit_size, u32 len);
int spi_nand_write(u32 addr, u8 *buf, u32 len);
int spi_nand_read(u32 addr, u8 *buf, u32 len);

#endif
