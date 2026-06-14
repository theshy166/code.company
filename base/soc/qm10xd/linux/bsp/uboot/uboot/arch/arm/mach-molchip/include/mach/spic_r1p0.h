/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _SPIC_R1P0_H_
#define _SPIC_R1P0_H_

#define   REG_SPI_CTRLR0		(REG_SPIC_CFG_BASE + 0x00)
#define   REG_SPI_CTRLR1		(REG_SPIC_CFG_BASE + 0x04)
#define   REG_SPI_SSIENR		(REG_SPIC_CFG_BASE + 0x08)
#define   REG_SPI_MWCR			(REG_SPIC_CFG_BASE + 0x0C)
#define   REG_SPI_SER			(REG_SPIC_CFG_BASE + 0x10)
#define   REG_SPI_BAUDR			(REG_SPIC_CFG_BASE + 0x14)
#define   REG_SPI_TXFTLR		(REG_SPIC_CFG_BASE + 0x18)
#define   REG_SPI_RXFTLR		(REG_SPIC_CFG_BASE + 0x1C)
#define   REG_SPI_TXFLR			(REG_SPIC_CFG_BASE + 0x20)
#define   REG_SPI_RXFLR			(REG_SPIC_CFG_BASE + 0x24)
#define   REG_SPI_SR			(REG_SPIC_CFG_BASE + 0x28)
#define   REG_SPI_IMR			(REG_SPIC_CFG_BASE + 0x2C)
#define   REG_SPI_ISR			(REG_SPIC_CFG_BASE + 0xc88)
#define   REG_SPI_RISR			(REG_SPIC_CFG_BASE + 0x34)
#define   REG_SPI_TXOICR		(REG_SPIC_CFG_BASE + 0x38)
#define   REG_SPI_RXOICR		(REG_SPIC_CFG_BASE + 0x3C)
#define   REG_SPI_RXUICR		(REG_SPIC_CFG_BASE + 0x40)
#define   REG_SPI_MSTICR		(REG_SPIC_CFG_BASE + 0x44)
#define   REG_SPI_ICR			(REG_SPIC_CFG_BASE + 0x48)
#define   REG_SPI_DMACR			(REG_SPIC_CFG_BASE + 0x4C)
#define   REG_SPI_DMATDLR		(REG_SPIC_CFG_BASE + 0x50)
#define   REG_SPI_DMARDLR		(REG_SPIC_CFG_BASE + 0x54)
#define   REG_SPI_IDR			(REG_SPIC_CFG_BASE + 0x58)
#define   REG_SPI_SSI_COMP_VERSION			(REG_SPIC_CFG_BASE + 0x5C)
#define   REG_SPI_DR			(REG_SPIC_CFG_BASE + 0x60)

#define   REG_SPI_RX_SAMPLE_DLY	(REG_SPIC_CFG_BASE + 0xF0)
#define   REG_SPI_CCFGR			(REG_SPIC_CFG_BASE + 0xF4)
#define   REG_SPI_OPCR			(REG_SPIC_CFG_BASE + 0xF8)
#define   REG_SPI_TIMCR			(REG_SPIC_CFG_BASE + 0xFC)
#define   REG_SPI_BBAR0			(REG_SPIC_CFG_BASE + 0x100)
#define   REG_SPI_BBAR1			(REG_SPIC_CFG_BASE + 0x104)


/*Bit fields in CTRL0 */
#define SPI_DFS_OFFSET		0
#define SPI_DFS_SIZE		4
#define SPI_FRF_OFFSET		4
#define SPI_FRF_SIZE		2
#define SPI_SCPH_OFFSET		6
#define SPI_SCPH_SIZE		1
#define SPI_SCPOL_OFFSET	7
#define SPI_SCPOL_SIZE		1
#define SPI_TMOD_OFFSET		8
#define SPI_TMOD_SIZE		2
#define SPI_SLV_OE_OFFSET	10
#define SPI_SLV_OE_SIZE		1
#define SPI_CFS_OFFSET		12
#define SPI_CFS_SIZE		4

/*Bit filelds in CTRL1 */
#define SPI_NDF_OFFSET		0
#define SPI_NDF_SIZE		16

/*Bit fields in ENR */
#define SPI_SSIEN_OFFSET	0
#define SPI_SSIEN_SIZE		1

/*Bit fields in SER */
#define SPI_SER_OFFSET		0
#define SPI_SER_SIZE		1

/*Bit fields in BAUDR */
#define SPI_SCKDV_OFFSET	0
#define SPI_SCKDV_SIZE		16

/*Bit fields in TXFTLR */
#define SPI_TFT_OFFSET		0
#define SPI_TFT_SIZE		5

/*Bit fields in RXFTLR */
#define SPI_RFT_OFFSET		0
#define SPI_RFT_SIZE		5

/*Bit fields in TXFLR */
#define SPI_TFTFL_OFFSET	0
#define SPI_TFTFL_SIZE		5

/*Bit fields in RXFLR */
#define SPI_RFTFL_OFFSET	0
#define SPI_RFTFL_SIZE		5

/*Bit fields in SR */
#define SPI_BUSY_OFFSET		0
#define SPI_BUSY_SIZE		1
#define SPI_TFNF_OFFSET		1
#define SPI_TFNF_SIZE		1
#define SPI_TFE_OFFSET		2
#define SPI_TFE_SIZE		1
#define SPI_RFNE_OFFSET		3
#define SPI_RFNE_SIZE		1
#define SPI_RFF_OFFSET		4
#define SPI_RFF_SIZE		1
#define SPI_TXE_OFFSET		5
#define SPI_TXE_SIZE		1
#define SPI_DCOL_OFFSET		6
#define SPI_DCOL_SIZE		1
#define SPI_ATFNF_OFFSET	8
#define SPI_ATFNF_SIZE		1
#define SPI_ATFE_OFFSET		9
#define SPI_ATFE_SIZE		1
#define SPI_ARFNE_OFFSET	10
#define SPI_ARFNE_SIZE		1
#define SPI_ARFF_OFFSET		11
#define SPI_ARFF_SIZE		1
#define SPI_ABTE_OFFSET		12
#define SPI_ABTE_SIZE		1

/*Bit fields in IMR */
#define SPI_TXEIM_OFFSET	0
#define SPI_TXEIM_SIZE		1
#define SPI_TXOIM_OFFSET	1
#define SPI_TXOIM_SIZE		1
#define SPI_RXUIM_OFFSET	2
#define SPI_RXUIM_SIZE		1
#define SPI_RXOIM_OFFSET	3
#define SPI_RXOIM_SIZE		1
#define SPI_RXFIM_OFFSET	4
#define SPI_RXFIM_SIZE		1
#define SPI_MSTIM_OFFSET	5
#define SPI_MSTIM_SIZE		1

/*Bit fields in IER */
#define SPI_TXEIS_OFFSET	0
#define SPI_TXEIS_SIZE		1
#define SPI_TXOIS_OFFSET	1
#define SPI_TXOIS_SIZE		1
#define SPI_RXUIS_OFFSET	2
#define SPI_RXUIS_SIZE		1
#define SPI_RXOIS_OFFSET	3
#define SPI_RXOIS_SIZE		1
#define SPI_RXFIS_OFFSET	4
#define SPI_RXFIS_SIZE		1
#define SPI_MSTIS_OFFSET	5
#define SPI_MSTIS_SIZE		1

/*Bit fields in RISR */
#define SPI_TXEIR_OFFSET	0
#define SPI_TXEIR_SIZE		1
#define SPI_TXOIR_OFFSET	1
#define SPI_TXOIR_SIZE		1
#define SPI_RXUIR_OFFSET	2
#define SPI_RXUIR_SIZE		1
#define SPI_RXOIR_OFFSET	3
#define SPI_RXOIR_SIZE		1
#define SPI_RXFIR_OFFSET	4
#define SPI_RXFIR_SIZE		1
#define SPI_MSTIR_OFFSET	5
#define SPI_MSTIR_SIZE		1

/*Bit fields in TXOICR */
#define SPI_TXOICR_OFFSET	0
#define SPI_TXOICR_SIZE		1

/*Bit fields in RXOICR */
#define SPI_RXOICR_OFFSET	0
#define SPI_RXOICR_SIZE		1

/*Bit fields in RXUICR */
#define SPI_RXUICR_OFFSET	0
#define SPI_RXUICR_SIZE		1

/*Bit fields in MSTICR */
#define SPI_MSTICR_OFFSET	0
#define SPI_MSTICR_SIZE		1

/*Bit fields in ICR */
#define SPI_ICR_OFFSET		0
#define SPI_ICR_SIZE		1

/*Bit fields in DMACR */
#define SPI_RDMAE_OFFSET	0
#define SPI_RDMAE_SIZE		1
#define SPI_TDMAE_OFFSET	1
#define SPI_TDMAE_SIZE		1

/*Bit fields in DMATDLR */
#define SPI_DMATDL_OFFSET	0
#define SPI_DMATDL_SIZE		8

/*Bit fields in RMATDLR */
#define SPI_DMARDL_OFFSET	0
#define SPI_DMARDL_SIZE		8

/*Bit fields in DR */
#define SPI_DR0_OFFSET		0
#define SPI_DR0_SIZE		16

/*Bit fields in RX_SAMPLE_DLAY */
#define SPI_RSD_OFFSET		0
#define SPI_RSD_SIZE		8


/*Bit fields in CCFGR */
#define SPI_WMOD_OFFSET		0
#define SPI_WMOD_SIZE		2
#define SPI_FLDFORM_OFFSET	2
#define SPI_FLDFORM_SIZE	2
#define SPI_FRMFORM_OFFSET	4
#define SPI_FRMFORM_SIZE	3
#define SPI_ADDRMODE_OFFSET	7
#define SPI_ADDRMODE_SIZE	1
#define SPI_DUMMYBYTE_OFFSET	8
#define SPI_DUMMYBYTE_SIZE		3
#define SPI_XIPEN_OFFSET	11
#define SPI_XIPEN_SIZE		1

/*Bit fields in OPCR */
#define SPI_RDOPC_OFFSET	0
#define SPI_RDOPC_SIZE		8
#define SPI_WROPC_OFFSET	8
#define SPI_WROPCS_SIZE		8
#define SPI_DPIEN_OFFSET	16
#define SPI_DPIEN_SIZE		1
#define SPI_QPIEN_OFFSET	17
#define SPI_QPIEN_SIZE		1
#define SPI_SPLITEN_OFFSET	18
#define SPI_SPLITEN_SIZE	1
#define SPI_PDATA_OFFSET	20
#define SPI_PDATA_SIZE		8

/*Constants for TMCR */
#define SPI_TSHSL_OFFSET	0
#define SPI_TSHSL_SIZE		12



#define SPI_BITS_8_BPT		0
#define SPI_BITS_9_BPT		1
#define SPI_BITS_10_BPT		2
#define SPI_BITS_11_BPT		3
#define SPI_BITS_12_BPT		4
#define SPI_BITS_13_BPT		5
#define SPI_BITS_14_BPT		6
#define SPI_BITS_15_BPT		7
#define SPI_BITS_16_BPT		8

#define SPI_ONE_DATA		0
#define SPI_TWO_DATA		1
#define SPI_FOUR_DATA		2

/* Status Register 2 bits */
#define SR2_QUAD_EN_BIT7	BIT(7)

/* Bit manipulation macros */
#define SPI_BIT(name) \
	(1 << SPI_##name##_OFFSET)
#define SPI_BF(name, value) \
	(((value) & ((1 << SPI_##name##_SIZE)-1)) << SPI_##name##_OFFSET)
#define SPI_BFEX(name, value) \
	(((value) >> SPI_##name##_OFFSET) & ((1 << SPI_##name##_SIZE) -1))
#define SPI_BFINS(name, value, old) \
	(((old) & ~(((1 << SPI_##name##_SIZE) - 1) << SPI_##name##_OFFSET)) \
	| SPI_BF(name, value))


#define spi_readl(reg) \
	__raw_readl(reg)
#define spi_writel(reg,value) \
	__raw_writel((value),reg)

//enable spi
#define SPI_DISABLE	0
#define SPI_ENABLE	1

//polarity
#define SPI_POLARITY_LOW	0
#define SPI_POLARITY_HIGH	1

//phase
#define SPI_PHASE_SCLK_TOGGLE_MIDDLE	0
#define SPI_PHASE_SCLK_TOGGLE_START		1

//frame format
#define SPI_FORMAT_MOTO		0
#define SPI_FORMAT_TI		1
#define SPI_FORMAT_NS		2

//data size
#define SPI_DATA_SIZE_4BIT	3
#define SPI_DATA_SIZE_5BIT	4
#define SPI_DATA_SIZE_6BIT	5
#define SPI_DATA_SIZE_7BIT	6
#define SPI_DATA_SIZE_8BIT	7
#define SPI_DATA_SIZE_9BIT	8
#define SPI_DATA_SIZE_10BIT	9

//transfer mode
#define SPI_TX_RX_MODE		0
#define SPI_ONLY_TX_MODE	1
#define SPI_ONLY_RX_MODE	2
#define SPI_EEPROM_MODE		3


#define SPI_PORT0		0
#define SPI_PORT1		1

#define SPI_SCLKIN		24000000
#define spi_baudrate(x) (SPI_SCLKIN/(x))

#define FILED_FORMAT_STD		0
#define FILED_FORMAT_DUAL_QUAD	1 //dual data,other series
#define FILED_FORMAT_DIO_QIO	2 //dual address and data, other series
#define FILED_FORMAT_DPI_QPI	3 //opcode,data,address dual or quard

#define WIRE_MODE_SPI			0
#define WIRE_MODE_DPI			1
#define WIRE_MODE_QPI			2

#define FRAME_FORM_RESERVED			0
#define FRAME_FORM_OPCODE_ONLY		1
#define FRAME_FORM_OPCODE_1bADDR	2
#define FRAME_FORM_OPCODE_2bADDR	3
#define FRAME_FORM_OPCODE_3bADDR	4
#define FRAME_FORM_OPCODE_4bADDR	5
#define FRAME_FORM_NO_OPCODE_3bADDR	6
#define FRAME_FORM_NO_OPCODE_4bADDR	7

#define ADDR_MODE_3BYTES			(0x0)
#define ADDR_MODE_4BYTES			(0x1)


#define	MAX_FIFO_SIZE		128
#define SPIC_OP_READ		0x0
#define SPIC_OP_WRITE		0x1

#define SPIC_CS_DIR		(0x13280008)
#define SPIC_CS_MASK		(0x13280004)
#define SPIC_CS_VAL		(0x13280000)

void CTRL_CS_HIGH(u8 port);

void CTRL_CS_LOW(u8 port);

void spi_rx_data(u8 *rxp,u32 len);
int spi_tx_data(u8 *txp,u32 len);

#endif




