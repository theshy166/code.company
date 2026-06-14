#include "spi_flash.h"

#ifndef __SPI_NAND_FLASH_H_
#define __SPI_NAND_FLASH_H_

/*
 * Standard SPI-NAND flash commands
 */
#define SPINAND_CMD_READ				0x13
#define SPINAND_CMD_READ_RDM			0x03
#define SPINAND_CMD_PROG_LOAD			0x02
#define SPINAND_CMD_PROG_RDM			0x84
#define SPINAND_CMD_PROG				0x10
#define SPINAND_CMD_ERASE_BLK			0xd8
#define SPINAND_CMD_WR_ENABLE			0x06
#define SPINAND_CMD_WR_DISABLE			0x04
#define SPINAND_CMD_READ_ID				0x9f
#define SPINAND_CMD_RESET				0xff
#define SPINAND_CMD_READ_REG			0x0f
#define SPINAND_CMD_WRITE_REG			0x1f

#define SPINAND_CMD_READ_CACHE_X2		0x3b
#define SPINAND_CMD_READ_CACHE_X4		0x6b
#define SPINAND_CMD_READ_CACHE_DUAL		0xbb
#define SPINAND_CMD_READ_CACHE_QUAD		0xeb

#define SPINAND_CMD_PROG_LOAD_X4		0x32
#define SPINAND_CMD_PROG_RDM_X4			0xC4 /*or 34*/

/* feature registers */
#define REG_BLOCK_LOCK					0xa0
#define REG_OTP							0xb0
#define REG_STATUS						0xc0/* timing */

/* status */
#define STATUS_OIP_MASK					0x01
#define STATUS_READY					(0 << 0)
#define STATUS_BUSY						(1 << 0)

#define STATUS_E_FAIL_MASK				0x04
#define STATUS_E_FAIL					(1 << 2)

#define STATUS_P_FAIL_MASK				0x08
#define STATUS_P_FAIL					(1 << 3)

#define STATUS_ECC_SUCCESS_MASK			0x60
#define STATUS_ECC_SUCCESS				0

#define NAND_READ_WAIT_TIME				(2000)

#define NAND_INFO_OFFSET				(24)
#define ECC_IN_OOB						(0x10)

/*OTP register defines */
#define OTP_ECC_MASK				(0x10)
#define OTP_ECC_ENABLE				(1 << 4)
#define OTP_ENABLE				(1<<6)
#define OTP_LOCK				(1<<7)
#define OTP_QUAD_ENABLE				(1<<0)

/*block lock**/
#define BL_ALL_LOCKED			0x38
#define BL_1_2_LOCKED			0x30
#define BL_1_4_LOCKED			0x28
#define BL_1_8_LOCKED			0x20
#define BL_1_16_LOCKED			0x18
#define BL_1_32_LOCKED			0x10
#define BL_1_64_LOCKED			0x08
#define BL_ALL_UNLOCKED			0x0

#define SPINAND_MAX_ID_LEN		4

#define SPINAND_ECC_MODE_24_bit	(1<<3)
#define SPINAND_NEED_PLANE_SELECT	(1<<0)
#define SPINAND_SUPPORT_DUAL		(1<<1)
#define SPINAND_SUPPORT_QUAD	(1<<2)

#define SPI_NAND_INFO(nm, mid, did, pagesz, sparesz, \
	pg_per_blk, blk_per_chip, opts)					\
{	\
	.name = (nm), \
	.mfr_id = (mid), \
	.dev_id = (did), \
	.page_size = (pagesz), \
	.page_spare_size = (sparesz), \
	.pages_per_blk = (pg_per_blk), \
	.blks_per_chip = (blk_per_chip), \
	.options = (opts)\
}

struct spi_nand_onfi_params {

};

struct spi_nand_cmd {
	u8		cmd;
	u32		n_addr;		/*number of address*/
	u8		addr[3];
	u32		n_tx;		/*number of tx bytes*/
	u8		*tx_buf;
	u8		tx_nbits;
	u32		n_rx;
	u8		*rx_buf;
	u8		rx_nbits;
	u8		ecc_enable;
	u8		ecc_mode;
};

struct	spi_nand_id_info{
	#define		SPI_NAND_ID_NO_DUMMY	(0xff)
	u8		id_addr;
	u8		id_len;
};

struct spi_nand_flash
{
	char		*name;
	struct spi_nand_id_info	id_info;
	u8		dev_id[SPINAND_MAX_ID_LEN];
	u32		page_size;
	u8		page_spare_size;
	u8		pages_per_blk;
	u32		blks_per_chip;
	u32		options;
	u8		ecc_mask;
	u8		ecc_uncorr;
	u8		block_shift;
	u8		page_shift;
	u32		block_size;
};

struct spi_nand_info
{
	u32 main_size;
	u32 oob_size;
	u32 ecc_mode;
	u32 ecc_pos;
	u8  ecc[48];
	u32 sector_num;
	u32 spare_size;
	u32 block_page;
};

/*
struct boot_header
{
	u32 magic_num;
	u32 addr;
	u32 length;
	u32 iv[4];
	u32 res;
};
*/
extern struct spi_nand_flash *chip;

int spi_nand_program_image(u32 dest, u32 len, u8 *write_buf);
int spi_nand_read(struct spi_flash *nand,u32 from, size_t len,void *read_buf);
int spi_nand_program(struct spi_flash *nand,u32 to, size_t len, const void *write_buf);
int spi_nand_erase(struct spi_flash *nand,u32 to, size_t len);
int spi_nand_scan(struct spi_flash *nand);
#endif


