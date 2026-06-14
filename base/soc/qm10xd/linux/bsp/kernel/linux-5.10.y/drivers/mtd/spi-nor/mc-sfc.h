/*
  *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _MC_SFC_H
#define _MC_SFC_H
/* Hardware register offsets and field definitions */
#define SFC_CTRLR0				0x00
#define CTRLR0_DFS_MASK			GENMASK(3,0)
#define CTRLR0_FRF_MASK			GENMASK(5,4)
#define CTRLR0_SCPH				BIT(6)
#define CTRLR0_SCPOL			BIT(7)
#define CTRLR0_CMD_DTR_EN			BIT(21)
#define CTRLR0_ADDR_DTR_EN			BIT(22)
#define CTRLR0_DATA_DTR_EN			BIT(23)
#define SFC_CTRLR1				0x04
#define SFC_SSIENR				0x08
#define SPI_SER_OFFSET			0
#define SPI_SER_SIZE			1
#define SSIENR_SSIEN			BIT(0)
#define SFC_MWCR				0x0C
#define SFC_SER					0x10
#define SER_SER_MASK			GENMASK(1,0)
#define SER_SELECT(x)			(1 << x)
#define SFC_BAUDR				0x14
#define BAUDR_SCKDV_MASK		GENMASK(15,0)
#define SCKDV(x)				(x)
#define SFC_TXFTLR				0x18
#define SFC_RXFTLR				0x1C
#define SFC_TXFLR				0x20
#define SFC_RXFLR				0x24
#define SFC_SR					0x28
#define SR_BUSY					BIT(0)
#define SR_TFNF					BIT(1)
#define SR_TFE					BIT(2)
#define SR_RFNE					BIT(3)
#define SR_RFF					BIT(4)
#define SR_TXE					BIT(5)
#define SR_DCOL					BIT(6)
#define SFC_IMR					0x2C
#define IMR_TXEIM				BIT(0)
#define IMR_TXOIM				BIT(1)
#define IMR_RXUIM				BIT(2)
#define IMR_RXOIM				BIT(3)
#define IMR_RXFIM				BIT(4)
#define IMR_CMD_DONE			BIT(5)
#define SFC_ISR					0x30
#define ISR_TXEIS				BIT(0)
#define ISR_TXOIS				BIT(1)
#define ISR_RXUIS				BIT(2)
#define ISR_RXOIS				BIT(3)
#define ISR_RXFIS				BIT(4)
#define ISR_CMD_DONE			BIT(5)
#define SFC_RISR				0x34
#define SFC_TXOICR				0x38
#define SFC_RXOICR				0x3C
#define SFC_RXUICR				0x40
#define SFC_MSTICR				0x44
#define SFC_ICR					0x48
#define SFC_DMATDLR				0x50
#define SFC_DMARDLR				0x54
#define SFC_IDR					0x58
#define SFC_SSI_COMP_VERSION	0x5C
#define SFC_DR					0x60
#define SFC_RX_SAMPLE_DLY		0xF0
#define SFC_CCFGR				0xF4
#define SPI_DUMMYBYTE_CLEAR		0
#define SPI_DUMMYBYTE_OFFSET	8
#define SPI_DUMMYBYTE_SIZE		4
#define SFC_CCFGR_DUMBYTE_MASK	GENMASK(10,8)
#define SFC_OPCR				0xF8
#define SFC_TIMCR				0xFC
#define SFC_CMD_MODE			0x100
#define CMD_WRITE				BIT(0)
#define CMD_READ				BIT(1)
#define CMD_ECC_EN				BIT(2)
#define DATA_TYPE_MASK			GENMASK(4,3)
#define DATA_TYPE_NONE			(0 << 3)
#define DATA_TYPE_DATA_ONLY		(1 << 3)
#define DATA_TYPE_DATA_SPARE    (2 << 3)
#define ECC_MODE_MASK			BIT_MASK(5)
#define ECC_MODE_8B				(0 << 5)
#define ECC_MODE_24B			(1 << 5)
#define ECC_ENCODE_MODE			BIT(6)
#define ECC_CORRECT_MODE		BIT(7)
#define SPI_XF_MODE_MASK		GENMASK(9,8)
#define XF_MODE(x)				((x) << 8)
#define XF_MODE_SPI				(0 << 8)
#define XF_MODE_DPI				(1 << 8)
#define XF_MODE_QPI				(2 << 8)
#define DMA_MODE				BIT(10)
#define SFC_ACCESS_STAR			0x104
#define SFC_ACC_START			BIT(0)
#define SFC_CMD_SET0			0x108
#define ADDR_LENGTH_MASK		GENMASK(2,0)
#define ADDR_LENGHT(x)			((x) << 0)
#define SPARE_LENGTH_MASK		GENMASK(13,8)
#define SPARE_LENGTH(x)			(((x) - 1) << 8)
#define DATA_LENGTH_MASK		GENMASK(27,16)
#define DATA_LENGTH(x)			(((x) - 1) << 16)
#define SFC_CMD_SET1			0x10C
#define CMD_VAL_MASK			GENMASK(7,0)
#define CMD_VAL(x)				((x) << 0)
#define ADDR1_MASK				GENMASK(15,8)
#define ADDR1(x)				((x) << 8)
#define ADDR2_MASK				GENMASK(23,16)
#define ADDR2(x)				((x) << 16)
#define ADDR3_MASK				GENMASK(31,24)
#define ADDR3(x)				((x) << 24)
#define SFC_CMD_SET2			0x110
#define ADDR4_MASK				GENMASK(7,0)
#define ADDR4(x)				((x) << 0)
#define ADDR5_MASK				GENMASK(15,8)
#define ADDR5(x)				((x) << 8)
#define ECC_START_POS_MASK		GENMASK(21,16)
#define ECC_START_POS(x)		((x) << 16)
#define SFC_ECC_INFO			0x114
#define SFC_SPARE_BUFFER		0xF00
#define SFC_DATA_BUFFER			0x1000

#define SFC_WAIT_TIMEOUT		1000000
#define SFC_DMA_MAX_LEN			(4096)

#define REG_NFC_CMD_MODE			(0xc00)
/* NFC_CMD_MODE Register bits */
#define NFC_WRITE_CMD				BIT(0)
#define NFC_READ_CMD				BIT(1)
#define ECC_CAL_EB					BIT(2)
#define NFC_DATA_TYPE_OFFSET			3
#define	NFC_DATA_TYPE_SIZE				2
#define DATA_NONE						0
#define DATA_ONLY						1
#define DATA_WITH_SPARE					2
#define SPI_XF_MODE_OFFSET				8
#define SPI_XF_MODE_SIZE				2
#define DMA_MODE					BIT(10)
#define NFC_HIGHCLK_MODE			BIT(18)
#define NFC_CMD_TYPE_OFFSET			23
#define NFC_CMD_TYPE_SIZE			3
#define NFC_CONTINUE_MODE			BIT(26)
#define NFC_ADDR_4BYTE_MODE			BIT(27)
#define REG_NFC_ACCESS_START		(0xc04)
#define NFC_ACC_START				BIT(0)
#define NFC_ECC_STS_CLR				BIT(1)		
#define NFC_FLOWBUF_CLR				BIT(2)		
#define REG_NFC_CMD_SET0			(0xc08)
#define NFC_ADDR_LENGTH_OFFSET			0
#define NFC_ADDR_LENGTH_SIZE			3
#define NFC_SPARE_LENGTH_OFFSET			8
#define NFC_SPARE_LENGTH_SIZE			8
#define NFC_DATA_LENGTH_OFFSET			16
#define NFC_DATA_LENGTH_SIZE			12
#define NFC_SET0_PAGE_SIZE_OFFSET		16
#define NFC_SET0_PAGE_SIZE_SIZE			12
#define REG_NFC_CMD_SET1			(0xc0c)
#define NFC_CMD_VALUE_OFFSET	0
#define NFC_CMD_VALUE_SIZE		8
#define NFC_ADDR1_OFFSET		8
#define NFC_ADDR1_SIZE			8
#define NFC_ADDR2_OFFSET		16
#define NFC_ADDR2_SIZE			8
#define NFC_ADDR3_OFFSET		24
#define NFC_ADDR3_SIZE			8
#define REG_NFC_CMD_SET2			(0xc10)
#define REG_NFC_CMD_SET3			(0xc14)
#define SPI_DATA_LENGTH_XC_OFFSET		0
#define SPI_DATA_LENGTH_XC_SIZE			24
#define SPI_RD_DLY_NUM_XC_OFFSET		24
#define SPI_RD_DLY_NUM_XC_SIZE			8
#define SPI_RD_DLY_NUM_MASK			(0xffffff)
#define REG_NFC_CMD_SET4			(0xc24)
#define REG_NFC_DEV_MODE			(0xc18)
#define REG_NFC_STATUS_SET0			(0xc1c)
#define REG_NFC_BUF_SOFT			(0xc20)
#define NFC_SOFT_FB_P0_RDY_BIT			BIT(0)	//RO
#define NFC_SOFT_FB_P0_ACK			BIT(1)	//WC
#define NFC_SOFT_FB_P2_RDY_BIT			BIT(2)	//RO
#define NFC_SOFT_FB_P2_ACK			BIT(3)	//wc
#define NFC_IS_RUNNING_TX_DMA_CH0		BIT(16)	//RO
#define NFC_IS_RUNNING_TX_DMA_CH1		BIT(17)	//RO
#define NFC_IS_RUNNING_RX_DMA_CH0		BIT(18)	//RO
#define NFC_IS_RUNNING_RX_DMA_CH1		BIT(19)	//RO
#define REG_NFC_DTR_CFG				(0xc30)
#define REG_NFC_DTR_CFG1			(0xc34)
#define REG_NFC_ECC_INFO0			(0xc40)
#define REG_NFC_ECC_INFO1			(0xc44)
#define REG_NFC_ECC_CFG0			(0xc48)
#define REG_NFC_ECC_CFG1			(0xc4c)
#define REG_NFC_ECC_CFG2			(0xc50)
#define REG_NFC_INTR_ST				(0xc80)
#define REG_NFC_INTR_EN				(0xc84)
#define REG_NFC_INTR_RAW			(0xc88)
#define SPI_FINISH					BIT(0)
#define SCMD_FINISH					BIT(1)
#define ECC_ERR						BIT(8)
#define ECC_WARN					BIT(9)
#define ECC_ALL_FF_WARN				BIT(10)
#define ECC_CORRECTED_WARN			BIT(11)
#define DMA_TX0_FINISH				BIT(16)
#define DMA_TX0_PAGE_CNT			BIT(17)
#define DMA_TX0_LEN_ERROR			BIT(19)
#define DMA_TX1_FINISH				BIT(20)
#define	DMA_TX1_PAGE_CNT			BIT(21)
#define DMA_TX1_LEN_ERROR			BIT(23)
#define DMA_RX0_FINISH				BIT(24)
#define DMA_RX0_PAGE_CNT			BIT(25)
#define DMA_RX0_LEN_ERROR			BIT(27)
#define DMA_RX1_FINISH				BIT(28)
#define DMA_RX1_PAGE_CNT			BIT(29)
#define DMA_RX1_LEN_ERROR			BIT(31)
#define REG_NFC_INTR_SET			(0xc8c)
#define REG_NFC_DL_OUT				(0xca0)
#define REG_NFC_DL_OUT_LP			(0xca4)
#define REG_NFC_CHIPSEL				(0xcc0)
#define REG_NFC_FLASH_REG			(0xcc4)
#define REG_NFC_DMA_COMMON			(0xd00)
#define NFC_DMA_REG_INTER_NUM_OFFSET	0
#define NFC_DMA_REG_INTER_NUM_SIZE		8
#define FB_INCDEC_MODE				BIT(8)
#define NFC_RX_CH0_DMA_EN			BIT(28)
#define NFC_RX_CH1_DMA_EN			BIT(29)
#define NFC_TX_CH0_DMA_EN			BIT(30)
#define NFC_TX_CH1_DMA_EN			BIT(31)
#define REG_NFC_DMA_TX_CH0			(0xd10)
#define REG_NFC_DMA_TX_CH0_CFG1		(0xd14)
#define REG_NFC_DMA_TX_CH1			(0xd1c)
#define REG_NFC_DMA_TX_CH1_CFG1		(0xd20)
#define REG_NFC_DMA_RX_CH0			(0xd28)
#define REG_NFC_DMA_RX_CH0_CFG1		(0xd2c)
#define REG_NFC_DMA_RX_CH1			(0xd34)
#define REG_NFC_DMA_RX_CH1_CFG1		(0xd38)

#define REG_DATA_REGION				(0x2000)
#define REG_SPARE_REGION			(0x3000)
#define REG_DATA_REGION0			(0x4000)
#define REG_SPARE_REGION0			(0x5000)

#define BUFFER_FLOW_WRITE			0
#define BUFFER_FLOW_READ			1


/* Status Register 2 bits */
#define SR2_QUAD_EN_BIT7	BIT(7)

/*NFC_DEV_MODE fileds*/
#define NFC_DATA_WMODE_OFFSET			0
#define NFC_DATA_WMODE_SIZE			2
#define NFC_ADDR_WMODE_OFFSET			2
#define NFC_ADDR_WMODE_SIZE			2
#define NFC_CMD_WMODE_OFFSET			4
#define NFC_CMD_WMODE_SIZE			2
#define NFC_DATA_PHASE_VLD			BIT(8)
#define NFC_ADDR_PHASE_VLD			BIT(9)
#define NFC_CMD_PHASE_VLD			BIT(10)
#define NFC_REG_PHASE_VLD			BIT(11)
#define NFC_DTR_CONTINUE_EN			BIT(12)
#define NFC_DATA_DTR_EN				BIT(13)
#define NFC_ADDR_DTR_EN				BIT(14)
#define NFC_CMD_DTR_EN				BIT(15)

#define NFC_FRMFORM_OFFSET			16
#define NFC_FRMFORM_SIZE			3
#define NFC_FLDFORM_OFFSET			19
#define NFC_FLDFORM_SIZE			2
#define NFC_TMOD_OFFSET				21
#define NFC_TMOD_SIZE				2
#define NFC_CMD_DUMMY_FRM_NUM_OFFSET		23
#define NFC_CMD_DUMMY_FRM_NUM_SIZE		3
#define NFC_ADDR_DUMMY_FRM_NUM_OFFSET		26
#define NFC_ADDR_DUMMY_FRM_NUM_SIZE		3

#define WMODE_SINGLE					0
#define WMODE_DUAL						1
#define WMODE_QUAD						2

#define VLD_CMD							0
#define VLD_CMD_DATA					1
#define VLD_CMD_ADDR					2
#define VLD_CMD_ADDR_DATA				3
#define VLD_CMD_ADDR_REG				4

#define TMOD_TRANSMIT_RECEIVE			0
#define TMOD_TRANSMIT_ONLY				1
#define TMOD_RECEIVE_ONLY				2
#define TMOD_EEPROM_READ				3

#define FLDFORM_ALL_SERIAL				0
#define FLDFORM_DQ_DATA_OTHRE_SERIAL	1
#define FLDFORM_SERIAL_OP_OTHER_DQ		2
#define FLDFORM_ALL_QUAD				3

#define FRMFORM_DATA_ONLY				0
#define FRMFORM_OPCODE_ONLY				1
#define FRMFORM_OPCODE_1ADDR			2
#define FRMFORM_OPCODE_2ADDR			3
#define FRMFORM_OPCODE_3ADDR			4
#define FRMFORM_OPCODE_4ADDR			5
#define FRMFORM_3ADDR_ONLY				6
#define FRMFORM_4ADDR_ONLY				7

/*spic*/
#define NFC_ENABLE						1
#define NFC_DISABLE						0

#define SFC_BAUDR_2 					2
#define SFC_BAUDR_4 					4

#define SFC_RX_SAMPLE_DLY_2				2
#define SFC_RX_SAMPLE_DLY_4				4

#define OP_READ_QUAD					0x6B
#define OP_READ_QUAD_4B					0x6C
#define OP_READ_DTR						0xED
#define OP_READ_DTR_4B					0xEE
#define OP_READ_SR						0x5
#define POWRE_DOWN						0xB9

#define SFC_CLK_FB						1

#define SFC_CLK_400M_REQ				400000000
#define SFC_CLK_300M_REQ				300000000

/* Bit manipulation macros */
#define SPI_BIT(name)				\
	(1 << SPI_##name##_OFFSET)
#define SPI_BF(name, value)			\
	(((value) & ((1 << SPI_##name##_SIZE) - 1)) << SPI_##name##_OFFSET)
#define SPI_BFEXT(name, value)			\
	(((value) >> SPI_##name##_OFFSET) & ((1 << SPI_##name##_SIZE) - 1))

#define SPI_BFINS(name, value, old)		\
	(((old) & ~(((1UL << SPI_##name##_SIZE) - 1) << SPI_##name##_OFFSET)) \
	| SPI_BF(name, value))

#define SPI_NFC_BF(name, value)			\
		(((value) & ((1 << NFC_##name##_SIZE)-1)) \
		<< NFC_##name##_OFFSET)
#define SPI_NFC_BFINS(name, value, old) \
		(((old) & ~(((1 << NFC_##name##_SIZE) - 1) \
		<< NFC_##name##_OFFSET)) \
		| SPI_NFC_BF(name, value))

enum mc_sfc_iftype {
	IF_TYPE_STD,
	IF_TYPE_DUAL,
	IF_TYPE_QUAD,
};

struct mc_sfc_priv {
	u32 chipselect;
	struct mc_sfc_host *host;
};

#define MC_MAX_CHIP_NUM		2
struct mc_sfc_host {
	struct device *dev;
	struct mutex lock;

	void __iomem *regbase;
	void __iomem *iobase;
	struct clk *apb_spic;
	struct clk *ahb_spic;
	struct clk *spic_ssi;
	struct clk *clk;
	u32 clkrate;
	void *buffer;
	dma_addr_t dma_buffer;

	struct spi_nor	*nor[MC_MAX_CHIP_NUM];
	u32 num_chip;
#ifdef CONFIG_PM_SLEEP
	u8 ctrl_sus;//Determine if it is possible to enter the suspend state
#define CTRL_START		1
#define CTRL_END		0
	u32 sus_timeout;//Set timeout when waiting for the controller to be idle in suspend
#define SUS_DEFAULT_TIMEOUT		500
	u32 force_suspend;//Set whether to enter suspend state forcibly when timeout
#endif
};

extern spinlock_t spinlock_nor_nand;

#endif