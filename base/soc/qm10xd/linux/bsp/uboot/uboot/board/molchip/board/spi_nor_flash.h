#ifndef __SPI_NOR_FLASH_
#define __SPI_NOR_FLASH_

	/* Flash opcodes. */
#define SPINOR_EN_RESET		0x66
#define SPINOR_RESET_DEVICE	0x99

#define SPINOR_OP_WREN		0x06	/* Write enable */
#define SPINOR_OP_RDSR		0x05	/* Read status register */
#define SPINOR_OP_WRSR		0x01	/* Write status register 1 byte */
#define SPINOR_OP_RDSR2		0x3f	/* Read status register 2 */
#define SPINOR_OP_WRSR2		0x3e	/* Write status register 2 */
#define SPINOR_OP_READ		0x03	/* Read data bytes (low frequency) */
#define SPINOR_OP_READ_FAST	0x0b	/* Read data bytes (high frequency) */
#define SPINOR_OP_READ_1_1_2	0x3b	/* Read data bytes (Dual Output SPI) */
#define SPINOR_OP_READ_1_2_2	0xbb	/* Read data bytes (Dual I/O SPI) */
#define SPINOR_OP_READ_1_1_4	0x6b	/* Read data bytes (Quad Output SPI) */
#define SPINOR_OP_READ_1_4_4	0xeb	/* Read data bytes (Quad I/O SPI) */
#define SPINOR_OP_PP		0x02	/* Page program (up to 256 bytes) */
#define SPINOR_OP_PP_1_1_4	0x32	/* Quad page program */
#define SPINOR_OP_PP_1_4_4	0x38	/* Quad page program */
#define SPINOR_OP_BE_4K		0x20	/* Erase 4KiB block */
#define SPINOR_OP_BE_4K_PMC	0xd7	/* Erase 4KiB block on PMC chips */
#define SPINOR_OP_BE_32K	0x52	/* Erase 32KiB block */
#define SPINOR_OP_CHIP_ERASE	0xc7	/* Erase whole flash chip */
#define SPINOR_OP_SE		0xd8	/* Sector erase (usually 64KiB) */
#define SPINOR_OP_RDID		0x9f	/* Read JEDEC ID */
#define SPINOR_OP_RDSFDP	0x5a	/* Read SFDP */
#define SPINOR_OP_RDCR		0x35	/* Read configuration register */
#define SPINOR_OP_RDFSR		0x70	/* Read flag status register */
#define SPINOR_OP_CLFSR		0x50	/* Clear flag status register */
#define SPINOR_OP_RDEAR		0xc8	/* Read Extended Address Register */
#define SPINOR_OP_WREAR		0xc5	/* Write Extended Address Register */


/* Status Register bits. */
#define SR_WIP			BIT(0)	/* Write in progress */
#define SR_WEL			BIT(1)	/* Write enable latch */

#define NOR_WAIT_READY_TIME	5000

//phase
#define SPI_PHASE_SCLK_TOGGLE_MIDDLE	0
#define SPI_PHASE_SCLK_TOGGLE_START	1

void spi_nor_reset(void);

int spi_nor_read_reg(u8 opcode, u8 *buf, int len);
int spi_nor_write_reg(u8 opcode);
bool spi_nor_wait_ready(void);
void spi_nor_write_enable(void);
void spi_nor_write_enable(void);
bool spi_nor_read_data(u32 from, u32 len, u8 *read_buf);
bool spi_transction_dma_read(u32 from, u32 len, u8 *read_buf);
int sfc_config(u32 from, u32 len, u8 *read_buf, struct boot_header *header);
int spi_nor_quad_write_data( u32 to, u8 *buf, u32 len);
bool spi_nor_erase(u32 opcode, u32 block);//0xd8
bool sfc_transfer_done(void);
#endif
