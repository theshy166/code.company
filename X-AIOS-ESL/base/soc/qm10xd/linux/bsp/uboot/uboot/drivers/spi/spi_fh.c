/*
 * Copyright (C) 2016 Fullhan Corporation
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>
#include <mach/type.h>

//#define FH_SPI_DEBUG printf
#define FH_SPI_DEBUG(...)

#define SPI_FIFO_DEPTH			(32)
#define	SPI0_REG_BASE						(0x130E0000)

#define SPI_CTRL0_OFFSET            		(0x00)
#define SPI_CTRL1_OFFSET            		(0x04)
#define SPI_SSIENR_OFFSET               	(0x08)
#define SPI_MWCR_OFFSET             		(0x0c)
#define SPI_SER_OFFSET              		(0x10)
#define SPI_BAUD_OFFSET             		(0x14)
#define SPI_TXFTL_OFFSET            		(0x18)
#define SPI_RXFTL_OFFSET            		(0x1c)
#define SPI_TXFL_OFFSET             		(0x20)
#define SPI_RXFL_OFFSET             		(0x24)
#define SPI_STATUS_OFFSET               	(0x28)
#define SPI_IMR_OFFSET              		(0x2c)
#define SPI_ISR_OFFSET              		(0x30)
#define SPI_RISR_OFFSET             		(0x34)
#define SPI_TXOIC_OFFSET            		(0x38)
#define SPI_RXOIC_OFFSET            		(0x3c)
#define SPI_RXUIC_OFFSET            		(0x40)
#define SPI_MSTIC_OFFSET            		(0x44)
#define SPI_INTCLR_OFFSET               	(0x48)
#define SPI_DMACTRL_OFFSET              	(0x4c)
#define SPI_DMATDL_OFFSET               	(0x50)
#define SPI_DMARDL_OFFSET               	(0x54)
#define SPI_IDR_OFFSET              		(0x58)
#define SPI_SSI_COMPVER_OFFSET              (0x5c)
#define SPI_DATA_OFFSET             		(0x60)
#define SPI_SAMPLE_DELAY             		(0xf0)
#define SPI_INTERVAL                		(0x100000)
#define REG_SPI_CTRLR0(n)                  	(n + SPI_CTRL0_OFFSET)
#define REG_SPI_CTRLR1(n)                   (n + SPI_CTRL1_OFFSET)
#define REG_SPI_SSIENR(n)                   (n + SPI_SSIENR_OFFSET)
#define REG_SPI_MWCR(n)                     (n + SPI_MWCR_OFFSET)
#define REG_SPI_SER(n)                      (n  + SPI_SER_OFFSET)
#define REG_SPI_BAUDR(n)                    (n  + SPI_BAUD_OFFSET)
#define REG_SPI_TXFTLR(n)                   (n  + SPI_TXFTL_OFFSET)
#define REG_SPI_RXFTLR(n)                   (n  + SPI_RXFTL_OFFSET)
#define REG_SPI_TXFLR(n)                    (n + SPI_TXFL_OFFSET)
#define REG_SPI_RXFLR(n)                    (n  + SPI_RXFL_OFFSET)
#define REG_SPI_SR(n)                       (n  + SPI_STATUS_OFFSET)
#define REG_SPI_IMR(n)                      (n  + SPI_IMR_OFFSET)
#define REG_SPI_ISR(n)                      (n  + SPI_ISR_OFFSET)
#define REG_SPI_RISR(n)                     (n  + SPI_RISR_OFFSET)
#define REG_SPI_TXOICR(n)                   (n  + SPI_TXOIC_OFFSET)
#define REG_SPI_RXOICR(n)                   (n  + SPI_RXOIC_OFFSET)
#define REG_SPI_RXUICR(n)                   (n  + SPI_RXUIC_OFFSET)
#define REG_SPI_MSTICR(n)                   (n  + SPI_MSTIC_OFFSET)
#define REG_SPI_ICR(n)                      (n + SPI_INTCLR_OFFSET)
#define REG_SPI_DMACR(n)                    (n  + SPI_DMACTRL_OFFSET)
#define REG_SPI_DMATDLR(n)                  (n  + SPI_DMATDL_OFFSET)
#define REG_SPI_DMARDLR(n)                  (n  + SPI_DMARDL_OFFSET)
#define REG_SPI_IDR(n)                      (n  + SPI_IDR_OFFSET)
#define REG_SPI_SSI_COMP_VERSION(n)         (n  + SPI_SSI_COMPVER_OFFSET)
#define REG_SPI_SSI_DR(n)                   (n  + SPI_DATA_OFFSET)
#define REG_SPI_SSI_SAMPLE_DELAY(n)         (n  + SPI_SAMPLE_DELAY)

#define SPI_CONTINUE_READ_MAX_DATA_NUM		0x10000  //64K
#define SPI_CONTINUE_READ_MIN_DATA_NUM		0x01     //one byte
#define SPI_TX_FIFO_DEPTH					128
#define SPI_RX_FIFO_DEPTH					128
#define SPI_IRQ_TXEIS						(lift_shift_bit_num(0))
#define SPI_IRQ_TXOIS						(lift_shift_bit_num(1))
#define SPI_IRQ_RXUIS						(lift_shift_bit_num(2))
#define SPI_IRQ_RXOIS						(lift_shift_bit_num(3))
#define SPI_IRQ_RXFIS						(lift_shift_bit_num(4))
#define SPI_IRQ_MSTIS						(lift_shift_bit_num(5))
#define SPI_STATUS_BUSY        				(lift_shift_bit_num(0))
#define SPI_STATUS_TFNF         			(lift_shift_bit_num(1))
#define SPI_STATUS_TFE         				(lift_shift_bit_num(2))
#define SPI_STATUS_RFNE        				(lift_shift_bit_num(3))
#define SPI_STATUS_RFF         				(lift_shift_bit_num(4))
#define SPI_STATUS_TXE         				(lift_shift_bit_num(5))
#define SPI_STATUS_DCOL        				(lift_shift_bit_num(6))
#define SPI_READ_BYTE_TIME_LIMIT			5000
#define SPI_WRITE_BYTE_TIME_LIMIT			5000
#define SPI_WRITE_READ_BYTE_TIME_LIMIT		5000
#define SPI_EEPROM_WREN						0x06
#define SPI_EEPROM_RDSR						0x05
#define SPI_EEPROM_READ						0x03
#define SPI_EEPROM_WRITE					0x02
#ifndef CONFIG_DEFAULT_SPI_BUS
#define CONFIG_DEFAULT_SPI_BUS				0
#endif

#ifndef CONFIG_DEFAULT_SPI_MODE
#define CONFIG_DEFAULT_SPI_MODE 			SPI_MODE_0
#endif

#define MAX_RX_ONE_TIME	128
//error status
enum {
	CONFIG_OK = 0,
	CONFIG_PARA_ERROR = lift_shift_bit_num(0),
	//only for the set slave en/disable
	CONFIG_BUSY = lift_shift_bit_num(1),
	//only for write_read mode
	WRITE_READ_OK = 0,
	WRITE_READ_ERROR = lift_shift_bit_num(2),
	WRITE_READ_TIME_OUT = lift_shift_bit_num(3),
	//only for write only mode
	WRITE_ONLY_OK = 0,
	WRITE_ONLY_ERROR = lift_shift_bit_num(4),
	WRITE_ONLY_TIME_OUT = lift_shift_bit_num(5),
	//only for read only mode
	READ_ONLY_OK = 0,
	READ_ONLY_ERROR = lift_shift_bit_num(6),
	READ_ONLY_TIME_OUT = lift_shift_bit_num(7),
	//eeprom mode
	EEPROM_OK = 0,
	EEPROM_ERROR = lift_shift_bit_num(8),
	EEPROM_TIME_OUT = lift_shift_bit_num(9),
	/* if read/write/eeprom error,the error below could give you more
	 * info by reading the 'Spi_ReadTransferError' function
	 */
	MULTI_MASTER_ERROR = lift_shift_bit_num(10),
	TX_OVERFLOW_ERROR = lift_shift_bit_num(11),
	RX_OVERFLOW_ERROR = lift_shift_bit_num(12),
};

//enable spi
typedef enum enum_spi_enable {
	SPI_DISABLE = 0,
	SPI_ENABLE = (lift_shift_bit_num(0)),
} spi_enable_e;

//polarity
typedef enum enum_spi_polarity {
	SPI_POLARITY_LOW = 0,
	SPI_POLARITY_HIGH = (lift_shift_bit_num(7)),
	//bit pos
	SPI_POLARITY_RANGE = (lift_shift_bit_num(7)),
} spi_polarity_e;

//phase
typedef enum enum_spi_phase {
	SPI_PHASE_RX_FIRST = 0,
	SPI_PHASE_TX_FIRST = (lift_shift_bit_num(6)),
	//bit pos
	SPI_PHASE_RANGE = (lift_shift_bit_num(6)),
} spi_phase_e;

//frame format
typedef enum enum_spi_format {
	SPI_MOTOROLA_MODE = 0x00,
	SPI_TI_MODE = 0x10, SPI_MICROWIRE_MODE = 0x20,
	//bit pos
	SPI_FRAME_FORMAT_RANGE = 0x30,
} spi_format_e;

//data size
typedef enum enum_spi_data_size {
	SPI_DATA_SIZE_4BIT = 0x03,
	SPI_DATA_SIZE_5BIT = 0x04,
	SPI_DATA_SIZE_6BIT = 0x05,
	SPI_DATA_SIZE_7BIT = 0x06,
	SPI_DATA_SIZE_8BIT = 0x07,
	SPI_DATA_SIZE_9BIT = 0x08,
	SPI_DATA_SIZE_10BIT = 0x09,
	//bit pos
	SPI_DATA_SIZE_RANGE = 0x0f,
} spi_data_size_e;

//transfer mode
typedef enum enum_spi_transfer_mode {
	SPI_TX_RX_MODE = 0x000,
	SPI_ONLY_TX_MODE = 0x100,
	SPI_ONLY_RX_MODE = 0x200,
	SPI_EEPROM_MODE = 0x300,
	//bit pos
	SPI_TRANSFER_MODE_RANGE = 0x300,
} spi_transfer_mode_e;

//spi baudrate
typedef enum enum_spi_baudrate {
	SPI_SCLKIN = 50000000,      //54M clk_in
	SPI_SCLKOUT_27000000 = (SPI_SCLKIN / 27000000),  //27M
	SPI_SCLKOUT_13500000 = (SPI_SCLKIN / 13500000),  //13.5M
	SPI_SCLKOUT_6000000 = (SPI_SCLKIN / 6000000),  //6.75M
	SPI_SCLKOUT_6750000 = (SPI_SCLKIN / 6750000),  //6.75M
	SPI_SCLKOUT_4500000 = (SPI_SCLKIN / 4500000),	 //4.5M
	SPI_SCLKOUT_3375000 = (SPI_SCLKIN / 3375000),  //3.375M
	SPI_SCLKOUT_2700000 = (SPI_SCLKIN / 2700000),	 //2.7M
	SPI_SCLKOUT_1500000 = (SPI_SCLKIN / 1500000),  //1.5M
	SPI_SCLKOUT_100000 = (SPI_SCLKIN / 100000),  //0.1M
	SPI_SCLKOUT_1000000 = (SPI_SCLKIN / 1000000),  //1M
} spi_baudrate_e;

//spi_irq
typedef enum enum_spi_irq {
	SPI_IRQ_TXEIM = (lift_shift_bit_num(0)),
	SPI_IRQ_TXOIM = (lift_shift_bit_num(1)),
	SPI_IRQ_RXUIM = (lift_shift_bit_num(2)),
	SPI_IRQ_RXOIM = (lift_shift_bit_num(3)),
	SPI_IRQ_RXFIM = (lift_shift_bit_num(4)),
	SPI_IRQ_MSTIM = (lift_shift_bit_num(5)),
	SPI_IRQ_ALL = 0x3f,
} spi_irq_e;

//spi_slave_port
typedef enum enum_spi_slave {
	SPI_SLAVE_PORT0 = (lift_shift_bit_num(0)),
	SPI_SLAVE_PORT1 = (lift_shift_bit_num(1)),
} spi_slave_e;

//dma control
typedef enum enum_spi_dma_control_mode {
	SPI_DMA_RX_POS = (lift_shift_bit_num(0)),
	SPI_DMA_TX_POS = (lift_shift_bit_num(1)),
	//bit pos
	SPI_DMA_CONTROL_RANGE = 0x03,
} spi_dma_control_mode_e;

typedef struct _fh_spi_controller {
	unsigned int base;
	unsigned int freq; /* Default frequency */
	unsigned int mode;
	struct spi_slave slave;
} fh_spi_controller;

#define SPIC_CS_DIR		(0x13280008)
#define SPIC_CS_MASK		(0x13280004)
#define SPIC_CS_VAL		(0x13280000)

#define SPI_MAX_CS_NUM			2
#define SPI_MAX_BUS_NUM         1


const unsigned int FH_SPI_BASE[] = {
	SPI0_REG_BASE,
};

SINT32 Spi_Enable(UINT32 base, spi_enable_e enable)
{
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, enable);
	SET_REG(REG_SPI_SSIENR(base), enable);
	return CONFIG_OK;
}

SINT32 Spi_SetPolarity(UINT32 base, spi_polarity_e polarity)
{
	UINT32 data;

	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, polarity);
	data = GET_REG(REG_SPI_CTRLR0(base));
	data &= ~(UINT32) SPI_POLARITY_RANGE;
	data |= polarity;
	SET_REG(REG_SPI_CTRLR0(base), data);
	return CONFIG_OK;
}

SINT32 Spi_SetPhase(UINT32 base, spi_phase_e phase)
{
	UINT32 data;
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, phase);
	data = GET_REG(REG_SPI_CTRLR0(base));
	data &= ~(UINT32) SPI_PHASE_RANGE;
	data |= phase;
	SET_REG(REG_SPI_CTRLR0(base), data);
	return CONFIG_OK;
}

SINT32 Spi_SetFrameFormat(UINT32 base, spi_format_e format)
{
	UINT32 data = 0;
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, format);
	data = GET_REG(REG_SPI_CTRLR0(base));
	data &= ~(UINT32) SPI_FRAME_FORMAT_RANGE;
	data |= format;
	SET_REG(REG_SPI_CTRLR0(base), data);
	return CONFIG_OK;
}

SINT32 Spi_SetDataSize(UINT32 base, spi_data_size_e size)
{
	UINT32 data = 0;
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, size);
	data = GET_REG(REG_SPI_CTRLR0(base));
	data &= ~(UINT32) SPI_DATA_SIZE_RANGE;
	data |= size;
	SET_REG(REG_SPI_CTRLR0(base), data);
	return CONFIG_OK;
}

SINT32 Spi_SetTransferMode(UINT32 base, spi_transfer_mode_e mode)
{
	UINT32 data = 0;
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, mode);
	data = GET_REG(REG_SPI_CTRLR0(base));
	data &= ~(UINT32) SPI_TRANSFER_MODE_RANGE;
	data |= mode;
	SET_REG(REG_SPI_CTRLR0(base), data);
	return CONFIG_OK;
}

SINT32 Spi_SetContinueReadDataNum(UINT32 base, UINT32 num)
{
	UINT32 data = 0;
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, num);
	data = num - 1;
	SET_REG(REG_SPI_CTRLR1(base), data);
	return CONFIG_OK;
}

SINT32 Spi_SetBaudrate(UINT32 base, spi_baudrate_e baudrate)
{
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, baudrate);
	SET_REG(REG_SPI_BAUDR(base), baudrate);
	return CONFIG_OK;
}


SINT32 Spi_SetSampleDelay(UINT32 base,  UINT32 delay)
{
	SET_REG(REG_SPI_SSI_SAMPLE_DELAY(base), delay);
	return CONFIG_OK;
}

SINT32 Spi_DisableIrq(UINT32 base, UINT32 irq)
{
	UINT32 data = 0;
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, irq);
	data = GET_REG(REG_SPI_IMR(base));
	data &= ~irq;
	SET_REG(REG_SPI_IMR(base), data);
	return CONFIG_OK;
}

SINT32 Spi_ReadStatus(UINT32 base)
{
	FH_SPI_DEBUG("sf %s(%x)\n", __func__, base);
	return (UINT8) GET_REG(REG_SPI_SR(base));
}

SINT32 Spi_EnableSlaveen(UINT32 base, int bus, int cs)
{
	UINT32 data;
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, cs);

	data = GET_REG(REG_SPI_SER(base));
	data |= cs + SPI_SLAVE_PORT0;
	SET_REG(REG_SPI_SER(base), data);
	return CONFIG_OK;
}

static UINT32 Spi_ReadTxfifolevel(UINT32 base)
{
	//UINT32 data;
	FH_SPI_DEBUG("sf %s(%x)\n", __func__, base);
	return GET_REG(REG_SPI_TXFLR(base));
}

static inline UINT32 tx_max_tx_only(UINT32 base, UINT32 tx_len)
{
	UINT32 hw_tx_level;
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, tx_len);
	hw_tx_level = Spi_ReadTxfifolevel(base);
	hw_tx_level = SPI_FIFO_DEPTH - hw_tx_level;
	//hw_tx_level /= 2;
	return min(hw_tx_level, tx_len);
}

static inline UINT32 tx_max(UINT32 base, UINT32 tx_total_len)
{

	UINT32 hw_tx_level, hw_rx_level;
	UINT32 temp_tx_lev;
	UINT32 rx_lev_reg, tx_lev_reg;

	rx_lev_reg = base + SPI_RXFL_OFFSET;
	tx_lev_reg = base + SPI_TXFL_OFFSET;

	//temp_tx_lev = SPI_ReadTxFifoLevel(spi_obj);
	temp_tx_lev = GET_REG(tx_lev_reg);
	// hw_rx_level = temp_tx_lev + SPI_ReadRxFifoLevel(spi_obj);
	hw_rx_level = temp_tx_lev + GET_REG(rx_lev_reg);
	//add shift data... maybe should add apb bus delay
	// hw_rx_level++;

	// hw_tx_level = temp_tx_lev;
	hw_tx_level = SPI_FIFO_DEPTH - temp_tx_lev;
	hw_rx_level = SPI_FIFO_DEPTH - hw_rx_level;

	return min(min(hw_tx_level, tx_total_len), hw_rx_level); //min(hw_tx_level, fh_spi->dwc.tx_len);
}

SINT32 Spi_DisableSlaveen(UINT32 base, int bus, int cs)
{
	UINT32 data;
	FH_SPI_DEBUG("sf %s(%x,%d)\n", __func__, base, cs);

	data = GET_REG(REG_SPI_SER(base));
	data &= ~(cs + SPI_SLAVE_PORT0);
	SET_REG(REG_SPI_SER(base), data);
	return CONFIG_OK;
}

static inline fh_spi_controller *to_fh_spi(struct spi_slave *slave)
{
	return container_of(slave, fh_spi_controller, slave);
}

int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	if ((bus >= SPI_MAX_BUS_NUM) || (cs >= SPI_MAX_CS_NUM))
		return 0;
	return 1;
}

void spi_cs_activate(struct spi_slave *slave)
{
	fh_spi_controller *fh_spi = to_fh_spi(slave);
	FH_SPI_DEBUG("sf %s(%x)\n", __func__, slave);
	Spi_EnableSlaveen(fh_spi->base, fh_spi->slave.bus, fh_spi->slave.cs);
	SET_REG(SPIC_CS_VAL,0x10);
}

void spi_cs_deactivate(struct spi_slave *slave)
{
	FH_SPI_DEBUG("sf %s(%x)\n", __func__, slave);
	fh_spi_controller *fh_spi = to_fh_spi(slave);
	Spi_DisableSlaveen(fh_spi->base, fh_spi->slave.bus, fh_spi->slave.cs);
	SET_REG(SPIC_CS_VAL,0x0);
}

void spi_init(void)
{

}

void spi_init_f(void)
{

}

ssize_t spi_read(uchar *addr, int alen, uchar *buffer, int len)
{
	return 0;
}

ssize_t spi_write(uchar *addr, int alen, uchar *buffer, int len)
{
	return 0;
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
	fh_spi_controller *fh_spi;
	FH_SPI_DEBUG("sf %s(%d,%d,%d,%d)\n", __func__, bus, cs, max_hz, mode);
	if (!spi_cs_is_valid(bus, cs))
		return NULL;

	fh_spi = malloc(sizeof(fh_spi_controller));
	if (!fh_spi)
		return NULL;
	memset(fh_spi, 0, sizeof(fh_spi_controller));
	//para is default by test
	fh_spi->freq = SPI_SCLKIN /
			 max_hz; /*SPI_SCLKOUT_6000000; SPI_SCLKOUT_100000; */
	fh_spi->base = FH_SPI_BASE[bus]; /* SPI0_PORT; */
	fh_spi->mode = mode;
	fh_spi->slave.bus = bus;
	fh_spi->slave.cs = cs;
	FH_SPI_DEBUG(" fh_spi freq=%d, base=%x, mode=%d, bus=%d, cs=%d\n",
		     fh_spi->freq, fh_spi->base, fh_spi->mode,
		     fh_spi->slave.bus, fh_spi->slave.cs);

	return &fh_spi->slave;
}

void spi_free_slave(struct spi_slave *slave)
{
	fh_spi_controller *fh_spi = to_fh_spi(slave);
	FH_SPI_DEBUG("sf %s(%x)\n", __func__, slave);
	free(fh_spi);
}

int spi_claim_bus(struct spi_slave *slave)
{
	fh_spi_controller *fh_spi = to_fh_spi(slave);
	if (!fh_spi)
		return -1;
	FH_SPI_DEBUG("sf %s(%x), spi(%x)\n", __func__, slave, fh_spi);
	Spi_Enable(fh_spi->base, SPI_DISABLE);
	Spi_SetSampleDelay(fh_spi->base, 1);
	//frame format
	Spi_SetFrameFormat(fh_spi->base, SPI_MOTOROLA_MODE);
	//spi polarty
	Spi_SetPolarity(fh_spi->base, SPI_POLARITY_HIGH);
	//spi phase
	Spi_SetPhase(fh_spi->base, SPI_PHASE_TX_FIRST);
	//transfer data size
	Spi_SetDataSize(fh_spi->base, SPI_DATA_SIZE_8BIT);
	//baudrate
	Spi_SetBaudrate(fh_spi->base, 2);
	(void) Spi_DisableIrq(fh_spi->base, SPI_IRQ_ALL);
	Spi_SetTransferMode(fh_spi->base, SPI_TX_RX_MODE);
	Spi_Enable(fh_spi->base, SPI_ENABLE);
	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	fh_spi_controller *fh_spi = to_fh_spi(slave);
	FH_SPI_DEBUG("sf %s(%x)\n", __func__, slave);
	Spi_Enable(fh_spi->base, SPI_DISABLE);

	Spi_Enable(fh_spi->base, SPI_ENABLE);
}

int fh_spi_high_speed_rx_process(uint base, uchar *rxp, uint len)
{
	uint time_out = 0xffff;
	uint error_status;
	uint data_addr = base + SPI_DATA_OFFSET;
	register uint rx_one_time = 0;
	uint i;

	rx_one_time = len;
	Spi_Enable(base, SPI_DISABLE);
	Spi_SetTransferMode(base, SPI_ONLY_RX_MODE);
	Spi_SetContinueReadDataNum(base, len);
	Spi_Enable(base, SPI_ENABLE);
	//set dummy data;
	SET_REG(data_addr, 0xff);
	//set read data no
	while (len) {
		rx_one_time = GET_REG(REG_SPI_RXFLR(base));
		len -= rx_one_time;
		for (i = 0; i < rx_one_time; i++) {
			*rxp++ = GET_REG(data_addr);
		}
		time_out--;
		if (!time_out) {
			printf("len is %d\n", len);

			error_status = GET_REG(base + 0x34);
			printf("error is %d\n", error_status);
			return 0;
		}
	}

	return 0;
}

int fh_spi_tx_rx_handle(uint base,uchar *src_txp, uchar *src_rxp, uint len)
{

	register uint rx_fifo_capability, tx_fifo_capability;
	uchar *rxp;
	uchar *txp;
	register uint dummy_data;
	uint rx_xfer_len;
	uint tx_xfer_len;
	uint i;
	uint status;

	tx_xfer_len = rx_xfer_len = len;
	rxp = src_rxp;
	txp = src_txp;

	register uint data_reg;
	uint rx_lev_reg, tx_lev_reg;
	data_reg = base + SPI_DATA_OFFSET;
	rx_lev_reg = base + SPI_RXFL_OFFSET;
	tx_lev_reg = base + SPI_TXFL_OFFSET;

	goto first;

start:
	//rx_fifo_capability = rx_max(spi_control);
	rx_fifo_capability = GET_REG(rx_lev_reg);
	rx_xfer_len -= rx_fifo_capability;
	//fh_spi->dwc.rx_len += rx_fifo_capability;

	if (rxp != NULL) {
		//fh_spi->dwc.rx_buff += rx_fifo_capability;
		while (rx_fifo_capability) {
			// *rxp++ = SPI_ReadData(spi_obj);
			*rxp++ = (uchar) GET_REG(data_reg);
			rx_fifo_capability--;
		}

	} else {
		while (rx_fifo_capability) {
			dummy_data = GET_REG(data_reg);
			rx_fifo_capability--;
		}
	}

	if (rx_xfer_len == 0) {
		return 0;
	}

first:
	tx_fifo_capability = tx_max(base, tx_xfer_len);
	//rt_kprintf("%d ",tx_fifo_capability);
	tx_xfer_len -= tx_fifo_capability;
	if (txp != NULL) {
		//   fh_spi->dwc.tx_buff += tx_fifo_capability;
		while (tx_fifo_capability) {
			//SPI_WriteData(spi_obj, *txp++);
			//  GET_REG(data_reg);
			SET_REG(data_reg, *txp++);
			tx_fifo_capability--;
		}

	} else {
		while (tx_fifo_capability) {
			// SPI_WriteData(spi_obj, 0xff);
			SET_REG(data_reg, 0xff);
			tx_fifo_capability--;
		}
	}

	goto start;
}

int fh_spi_high_speed_xfer(uint base, uchar *rxp, const uchar *txp, uint len)
{
	uint mode;
	uint xfer_one_time = 0;
	uint i;
	uint data_addr = base + SPI_DATA_OFFSET;
	uint status;
	//uint time_out = 0xffff;
	//uint error_status;
	status = Spi_ReadStatus(base);

	if (txp && rxp) {
		mode = SPI_TX_RX_MODE;
	} else if (txp) {
		//only tx...
		mode = SPI_ONLY_TX_MODE;
	} else if (rxp) {
		//only rx...
		mode = SPI_ONLY_RX_MODE;
	} else {
		return 0;
	}

	//only tx...
	if (mode == SPI_ONLY_TX_MODE) {
		Spi_Enable(base, SPI_DISABLE);
		Spi_SetTransferMode(base, mode);
		Spi_Enable(base, SPI_ENABLE);

		while (len) {
			xfer_one_time = tx_max_tx_only(base, len);
			len -= xfer_one_time;
			for (i = 0; i < xfer_one_time; i++) {
				//temp = *txp++;
				SET_REG(data_addr, *txp++);
			}
		}
		//wait tx done...
		do {
			status = Spi_ReadStatus(base);
		} while (!(status & 0x04));
	} else if (mode == SPI_ONLY_RX_MODE) {

		do {
			xfer_one_time = min(MAX_RX_ONE_TIME, len);
			fh_spi_high_speed_rx_process(base, rxp, xfer_one_time);
			len -= xfer_one_time;
			rxp += xfer_one_time;
		} while (len);

	}

	else if (mode == SPI_TX_RX_MODE) {
		Spi_Enable(base, SPI_DISABLE);
		Spi_SetTransferMode(base, mode);
		Spi_Enable(base, SPI_ENABLE);
		fh_spi_tx_rx_handle(base, txp, rxp, len);
	}

	return 0;
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
                void *din, unsigned long flags)
{
	uint txupto, rxupto;

	uchar *rxp;
	const uchar *txp;

	FH_SPI_DEBUG("sf %s(%x,%d,%x,%x,%d)\n", __func__, slave, bitlen, dout, din,
			flags);
	rxp = din;
	txp = dout;
	fh_spi_controller *fh_spi = to_fh_spi(slave);
	unsigned int data_addr;
	data_addr = REG_SPI_SSI_DR(fh_spi->base);
	if (bitlen % 8) {
		flags |= SPI_XFER_END;
		goto out;
	}

	spi_release_bus(slave);
	if ((flags & SPI_XFER_BEGIN))
		spi_cs_activate(slave);
	txupto = rxupto = bitlen / 8;
	fh_spi_high_speed_xfer(fh_spi->base, rxp, txp, txupto);
out:
	if ((flags & SPI_XFER_END))
		spi_cs_deactivate(slave);
	return 0;
}


