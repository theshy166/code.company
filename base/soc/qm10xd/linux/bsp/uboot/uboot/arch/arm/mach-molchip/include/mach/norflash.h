/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _MOLCHIP_NOR_FLASH_H_
#define _MOLCHIP_NOR_FLASH_H_

#define BIT(nr)					(1UL << (nr))
#ifdef CONFIG_ARM64
//#define	BITS_PER_LONG			32
#else
#define	BITS_PER_LONG			32
#endif

#define SPINOR_EN_RESET			0x66
#define SPINOR_RESET_DEVICE		0x99

#define	SNOR_MFR_GIGADEVICE		0xc8
#define SNOR_MFR_WINBOND		0xef
#define SNOR_W25Q256_ID			0xef4019
#define SNOR_GD25LQ128E_ID             0x1860c8


#define SPINOR_OP_WREN              0x06 /* Write enable */
#define SPINOR_OP_RDSR              0x05 /* Read status register */
#define SPINOR_OP_WRSR              0x01 /* Write status register 1 byte */
#define SPINOR_OP_RDSR2             0x3f /* Read status register 2 */
#define SPINOR_OP_RDSR2_2           0x35 /* Read status register 2 */
#define SPINOR_OP_WRSR2             0x3e /* Write status register 2 */
#define SPINOR_OP_WRSR2_2           0x31 /* Write status register 2_2 */
#define SPINOR_OP_RDSR3             0x15 /* Read status register */
#define SPINOR_OP_WRSR3             0x11 /* Write status register 1 byte */
#define	BIT_SPINOR_LATENCY_CODE     BIT(4)
#define SPINOR_OP_READ              0x03 /* Read data bytes (low frequency) */
#define SPINOR_OP_READ_FAST         0x0b /* Read data bytes (high frequency) */
#define SPINOR_OP_READ_1_1_2        0x3b /* Read data bytes (Dual Output SPI) */
#define SPINOR_OP_READ_1_2_2        0xbb /* Read data bytes (Dual I/O SPI) */
#define SPINOR_OP_READ_1_1_4        0x6b /* Read data bytes (Quad Output SPI) */
#define SPINOR_OP_READ_1_4_4        0xeb /* Read data bytes (Quad I/O SPI) */
#define SPINOR_OP_PP                0x02
#define SPINOR_OP_PP_1_1_4          0x32
#define SPINOR_OP_PP_1_4_4          0x38
#define SPINOR_OP_BE_4K             0x20
#define SPINOR_OP_BE_4K_PMC         0xd7
#define SPINOR_OP_BE_32K            0x52
#define SPINOR_OP_CHIP_ERASE        0xc7
#define SPINOR_OP_SE                0xd8
#define SPINOR_OP_RDID              0x9f
#define SPINOR_OP_RDSFDP            0x5a
#define SPINOR_OP_RDCR              0x35
#define SPINOR_OP_RDFSR             0x70
#define SPINOR_OP_CLFSR             0x50
#define SPINOR_OP_REEAR             0xc8
#define SPINOR_OP_WREAR             0xc5

#define	BIT_SPINOR_QUAD_ENABLE      BIT(1)


#define SPINOR_OP_READ_4B           0x13
#define SPINOR_OP_READ_FAST_4B      0x0c
#define SPINOR_OP_READ_1_1_2_4B     0x3c
#define SPINOR_OP_READ_1_2_2_4B     0xbc
#define SPINOR_OP_READ_1_1_4_4B     0x6c
#define SPINOR_OP_READ_1_4_4_4B     0xec
#define SPINOR_OP_PP_4B             0x12
#define SPINOR_OP_PP_1_1_4_4B       0x34
#define SPINOR_OP_PP_1_4_4_4B       0x3e
#define SPINOR_OP_BE_4K_4B          0x21
#define SPINOR_OP_BE_32K_4B         0x5c
#define SPINOR_OP_BE_4B             0xdc

#define SPINOR_OP_READ_1_1_1_DTR	0x0d
#define SPINOR_OP_READ_1_2_2_DTR	0xbd
#define SPINOR_OP_READ_1_4_4_DTR	0xed

#define SPINOR_OP_READ_1_1_1_DTR_4B	0x0e
#define SPINOR_OP_READ_1_2_2_DTR_4B	0xbe
#define SPINOR_OP_READ_1_4_4_DTR_4B	0xee

#define	SPINOR_OP_BP		0x02
#define	SPINOR_OP_WRDT		0x04
#define	SPINOR_OP_AAI_WP	0xad

#define	SPINOR_OP_XSE		0x50
#define	SPINOR_OP_XPP		0x82
#define	SPINOR_OP_XRDSR		0xd7

#define	XSR_PAGESIZE		BIT(0)
#define	XSR_RDY				BIT(7)


#define	FSR_READY			BIT(7)
#define	FSR_E_ERR			BIT(5)
#define	FSR_P_ERR			BIT(4)
#define	FSR_PT_ERR			BIT(1)

#define	CR_QUAD_EN_SPAN		BIT(1)

#define	SR2_QUAD_EN_BIT7	BIT(7)
#define	SR2_QUAD_EN_MX		BIT(6)

#define	EVCR_QUAD_EN_MICRON	BIT(7)


#define SPINOR_OP_EN4B		0xb7
#define SPINOR_OP_EX4B		0xe9

#define SPINOR_OP_BRWR		0x17
#define SPINOR_OP_CLSR		0x30

#define SPINOR_OP_RD_EVCR	0x65
#define SPINOR_OP_WD_EVCR	0x61

#define SPI_NOR_MAX_ID_LEN		6


enum{
	Sct_Erase_4K = 0,
	Sct_Erase_32K,
	Sct_Erase_64K,
	Sct_Erase_Invalid,
};

struct spi_nor{
	u32 		page_size;
	u32 		sector_size;
	u32 		sector_count;
};
struct norflash_info{
	char 	*name;
	void 	(*spi_nor_feature)(void);
	u8		id[SPI_NOR_MAX_ID_LEN];
	u8		id_len;

	unsigned int sector_size;
	u16		n_sectors;

	u16		page_size;
	u16		addr_width;
};

#define SR_WIP		BIT(0)
#define SR_WEL		BIT(1)
#define SR_BP0		BIT(2)
#define SR_BP1		BIT(3)
#define SR_BP2		BIT(4)
#define SR_TB		BIT(5)
#define SR_SRWD		BIT(7)
#define SR_E_ERR	BIT(5)
#define SR_P_ERR	BIT(6)


#define DEFAULT_READY_WAIT_JIFFIES		(0X40000UL)
#define NOR_WAIT_READY_TIME			3000



/* Used when the "_ext_id" is two bytes at most */
#define INFO(_jedec_id, _ext_id, _sector_size, _n_sectors)	\
		.id = {							\
			((_jedec_id) >> 16) & 0xff,			\
			((_jedec_id) >> 8) & 0xff,			\
			(_jedec_id) & 0xff,				\
			((_ext_id) >> 8) & 0xff,			\
			(_ext_id) & 0xff,				\
			},						\
		.id_len = (!(_jedec_id) ? 0 : (3 + ((_ext_id) ? 2 : 0))),	\
		.sector_size = (_sector_size),				\
		.n_sectors = (_n_sectors),				\
		.page_size = 256,					\

bool spi_nor_read_data(u32 from, u32 len,u8 *read_buf);

int spi_nor_identify(void);

int spi_nor_program_image(u32 dest, u32 len, u8 *write_buf);
//int spl_nor_load_image(struct spl_image_info *spl_image);
int sched_nor_load_image(struct boot_image_info * info);

#endif
