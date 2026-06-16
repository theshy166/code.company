#ifndef __SPIC_H_
#define __SPIC_H_
#include "system.h"

//spic
#define NO_ERROR				0
#define CORRECT_SECCESS				1
#define CORRECT_FAIL				2

#define ECC_8BIT				0
#define ECC_24BIT				1

#define NFC_ENABLE				1
#define NFC_DISABLE				0

#define HIGHCLK					1
#define NORMALCLK				0

/*
 *0:DMA->ECC->SPI->DMA(write page)
 *1:SPI->ECC->DMA->SPI(read page)
 */
#define BUFFER_FLOW_WRITE			0
#define BUFFER_FLOW_READ			1

#define DATA_WMODE_0				0
#define DATA_WMODE_1				1
#define DATA_WMODE_2				2

#define ADDR_WMODE_0				0
#define ADDR_WMODE_1				1
#define ADDR_WMODE_2				2

//enable spi
#define	SPI_DISABLE				0
#define	SPI_ENABLE				1

//polarity
#define SPI_POLARITY_LOW			0
#define SPI_POLARITY_HIGH			1
//phase
#define SPI_PHASE_SCLK_TOGGLE_MIDDLE		0
#define SPI_PHASE_SCLK_TOGGLE_START		1

//frame format
#define SPI_FORMAT_MOTO				0
#define SPI_FORMAT_TI				1
#define SPI_FORMATE_NS				2

//data size

#define	SPI_DATA_SIZE_4BIT			3
#define	SPI_DATA_SIZE_5BIT			4
#define	SPI_DATA_SIZE_6BIT			5
#define	SPI_DATA_SIZE_7BIT			6
#define	SPI_DATA_SIZE_8BIT			7
#define	SPI_DATA_SIZE_9BIT			8
#define	SPI_DATA_SIZE_10BIT			9


//transfer mode
#define	SPI_TX_RX_MODE				0
#define	SPI_ONLY_TX_MODE			1
#define	SPI_ONLY_RX_MODE			2
#define	SPI_EEPROM_MODE				3

#define SPI_PORT0				0
#define SPI_PORT1				1

#define	SPI_SCLKIN				24000000
#define spi_baudrate(x)				(SPI_SCLKIN / (x))


#define	FILED_FORMAT_STD			0
/* dual data,other series */
#define	FILED_FORMAT_DUAL_QUAD			1
/* dual address and data,other series */
#define	FILED_FORMAT_DIO_QIO			2
/* opcode,data,address dual or quard */
#define	FILED_FORMAT_DPI_QPI			3

#define WIRE_MODE_SPI				0
#define WIRE_MODE_DPI				1
#define WIRE_MODE_QPI				2

#define FRAME_FORM_RESERVED			0
#define FRAME_FORM_OPCODE_ONLY			1
#define FRAME_FORM_OPCODE_1bADDR		2
#define FRAME_FORM_OPCODE_2bADDR		3
#define FRAME_FORM_OPCODE_3bADDR		4
#define FRAME_FORM_OPCODE_4bADDR		5
#define FRAME_FORM_NO_OPCODE_3bADDR		6
#define FRAME_FORM_NO_OPCODE_4bADDR		7

#define ADDR_MODE_3BYTES			(0x0)
#define ADDR_MODE_4BYTES			(0x1)

#define MAX_FIFO_SIZE				128
#define SPIC_OP_READ				0x0
#define SPIC_OP_WRITE				0x1

//write pagesize不能�?
#define W_PAGESIZE				0x100
#define SFC_BUF_SIZE			4096
#define SFC_SPARE_BUF_SIZE			256
typedef struct _sfc_ctrl_config {
	unsigned int clock;
	unsigned int baud_rate;
	unsigned int sample_delay;
	unsigned int dummy_byte;
} sfc_ctrl_config;

void sfc_write_data(u8 *src, u32 len);
void sfc_read_data(u8 *dest, u32 len);
void sfc_dma_write_data(u8 *src, u32 len);
void sfc_dma_read_data(u8 *dest, u32 len);
void sfc_dma_read_spare(u8 *dest, u32 len);
void sfc_dma_wait_ready(void);
void spi_init(unsigned int speed_mode);
void  dma_full_cascade_handle(struct boot_header *header, u32 flag,
	int sfc, unsigned int len);
bool dma_cascade_done(struct boot_header *header, int sfc);

int nor_spi_init(void *data);
int nor_write_data(u32 to,  u8 *buf, u32 len);
int sfc_transfer_start(u32 src, u8 *dst, u32 size);
void sfc_dma_nor_read_data(u8 *dest, u32 len);
int nor_flash_erase(u32 to, u32 sector_size, u32 len);
void dma_cascade_start(struct boot_header *header, int sfc);
bool sfc_soft_dma_wait_ready(void);
void spi_set_rd_delay_cnt(unsigned int rd_delay_cnt);

#endif
