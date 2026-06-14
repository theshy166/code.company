#ifndef __SPIC_REG_H_
#define __SPIC_REG_H_


#define REG_SPI_CTRLR0				(REG_SPIC_BASE + 0x00)
#define REG_SPI_CTRLR1				(REG_SPIC_BASE + 0x04)
#define REG_SPI_SSIENR				(REG_SPIC_BASE + 0x08)
#define REG_SPI_MWCR				(REG_SPIC_BASE + 0x0C)
#define REG_SPI_SER				(REG_SPIC_BASE + 0x10)
#define REG_SPI_BAUDR				(REG_SPIC_BASE + 0x14)
#define REG_SPI_TXFTLR				(REG_SPIC_BASE + 0x18)
#define REG_SPI_RXFTLR				(REG_SPIC_BASE + 0x1C)
#define REG_SPI_TXFLR				(REG_SPIC_BASE + 0x20)
#define REG_SPI_RXFLR				(REG_SPIC_BASE + 0x24)
#define REG_SPI_SR				(REG_SPIC_BASE + 0x28)
#define REG_SPI_IMR				(REG_SPIC_BASE + 0x2C)
#define REG_SPI_ISR				(REG_SPIC_BASE + 0xc88)
#define REG_SPI_RISR				(REG_SPIC_BASE + 0x34)
#define REG_SPI_TXOICR				(REG_SPIC_BASE + 0x38)
#define REG_SPI_RXOICR				(REG_SPIC_BASE + 0x3C)
#define REG_SPI_RXUICR				(REG_SPIC_BASE + 0x40)
#define REG_SPI_MSTICR				(REG_SPIC_BASE + 0x44)
#define REG_SPI_ICR				(REG_SPIC_BASE + 0x48)
#define REG_SPI_DMACR				(REG_SPIC_BASE + 0x4C)
#define REG_SPI_DMATDLR				(REG_SPIC_BASE + 0x50)
#define REG_SPI_DMARDLR				(REG_SPIC_BASE + 0x54)
#define REG_SPI_IDR				(REG_SPIC_BASE + 0x58)
#define REG_SPI_SSI_COMP_VERSION		(REG_SPIC_BASE + 0x5C)
#define REG_SPI_DR				(REG_SPIC_BASE + 0x60)

#define REG_SPI_RX_SAMPLE_DLY			(REG_SPIC_BASE + 0xf0)
#define REG_SPI_CCFGR				(REG_SPIC_BASE + 0xf4)
#define REG_SPI_OPCR				(REG_SPIC_BASE + 0xf8)
#define REG_SPI_TIMCR				(REG_SPIC_BASE + 0xfc)

#define REG_NFC_CMD_MODE			(REG_SPIC_BASE + 0xc00)
#define REG_NFC_ACCESS_START			(REG_SPIC_BASE + 0xc04)
#define REG_NFC_CMD_SET0			(REG_SPIC_BASE + 0xc08)
#define REG_NFC_CMD_SET1			(REG_SPIC_BASE + 0xc0c)
#define REG_NFC_CMD_SET2			(REG_SPIC_BASE + 0xc10)
#define REG_NFC_CMD_SET3			(REG_SPIC_BASE + 0xc14)
#define REG_NFC_CMD_SET4			(REG_SPIC_BASE + 0xc24)
#define REG_NFC_DEV_MODE			(REG_SPIC_BASE + 0xc18)
#define REG_NFC_STATUS_SET0			(REG_SPIC_BASE + 0xc1c)
#define REG_NFC_BUF_SOFT			(REG_SPIC_BASE + 0xc20)
#define REG_NFC_DTR_CFG				(REG_SPIC_BASE + 0xc30)
#define REG_NFC_DTR_CFG1			(REG_SPIC_BASE + 0xc34)
#define REG_NFC_ECC_INFO0			(REG_SPIC_BASE + 0xc40)
#define REG_NFC_ECC_INFO1			(REG_SPIC_BASE + 0xc44)
#define REG_NFC_ECC_CFG0			(REG_SPIC_BASE + 0xc48)
#define REG_NFC_ECC_CFG1			(REG_SPIC_BASE + 0xc4c)
#define REG_NFC_ECC_CFG2			(REG_SPIC_BASE + 0xc50)
#define REG_NFC_INTR_ST				(REG_SPIC_BASE + 0xc80)
#define REG_NFC_INTR_EN				(REG_SPIC_BASE + 0xc84)
#define REG_NFC_INTR_RAW			(REG_SPIC_BASE + 0xc88)
#define REG_NFC_INTR_SET			(REG_SPIC_BASE + 0xc8c)
#define REG_NFC_DL_OUT				(REG_SPIC_BASE + 0xca0)
#define REG_NFC_DL_OUT_LP			(REG_SPIC_BASE + 0xca4)
#define REG_NFC_CHIPSEL				(REG_SPIC_BASE + 0xcc0)
#define REG_NFC_FLASH_REG			(REG_SPIC_BASE + 0xcc4)
#define REG_NFC_DMA_COMMON			(REG_SPIC_BASE + 0xd00)
#define REG_NFC_DMA_TX_CH0			(REG_SPIC_BASE + 0xd10)
#define REG_NFC_DMA_TX_CH0_CFG1			(REG_SPIC_BASE + 0xd14)
#define REG_NFC_DMA_TX_CH1			(REG_SPIC_BASE + 0xd1c)
#define REG_NFC_DMA_TX_CH1_CFG1			(REG_SPIC_BASE + 0xd20)
#define REG_NFC_DMA_RX_CH0			(REG_SPIC_BASE + 0xd28)
#define REG_NFC_DMA_RX_CH0_CFG1			(REG_SPIC_BASE + 0xd2c)
#define REG_NFC_DMA_RX_CH1			(REG_SPIC_BASE + 0xd34)
#define REG_NFC_DMA_RX_CH1_CFG1			(REG_SPIC_BASE + 0xd38)

#define REG_DATA_REGION				(REG_SPIC_BASE + 0x2000)
#define REG_SPARE_REGION			(REG_SPIC_BASE + 0x3000)
#define REG_DATA_REGION0			(REG_SPIC_BASE + 0x4000)
#define REG_SPARE_REGION0			(REG_SPIC_BASE + 0x5000)

/* Bitfields in CTRL0 */
#define SPI_DFS_OFFSET				0
#define SPI_DFS_SIZE				4
#define SPI_FRF_OFFSET				4
#define SPI_FRF_SIZE				2
#define SPI_SCPH_OFFSET				6
#define SPI_SCPH_SIZE				1
#define SPI_SCPOL_OFFSET			7
#define SPI_SCPOL_SIZE				1
#define SPI_TMOD_OFFSET				8
#define SPI_TMOD_SIZE				2
#define SPI_SLV_OE_OFFSET			10
#define SPI_SLV_OE_SIZE				1
#define SPI_CFS_OFFSET				12
#define SPI_CFS_SIZE				4

/* Bitfields in CTRL1 */
#define SPI_NDF_OFFSET				0
#define SPI_NDF_SIZE				16

/* Bitfields in ENR */
#define SPI_SSIEN_OFFSET			0
#define SPI_SSIEN_SIZE				1

/* Bitfields in SER */
#define SPI_SER_OFFSET				0
#define SPI_SER_SIZE				1

/* Bitfields in BAUDR */
#define SPI_SCKDV_OFFSET			0
#define SPI_SCKDV_SIZE				16

/* Bitfields in TXFTLR */
#define SPI_TFT_OFFSET				0
#define SPI_TFT_SIZE				5

/* Bitfields in RXFTLR */
#define SPI_RFT_OFFSET				0
#define SPI_RFT_SIZE				5

/* Bitfields in TXFLR */
#define SPI_TFTFL_OFFSET			0
#define SPI_TFTFL_SIZE				5

/* Bitfields in RXFLR */
#define SPI_RFTFL_OFFSET			0
#define SPI_RFTFL_SIZE				5

/* Bitfields in SR */
#define SPI_BUSY_OFFSET				0
#define SPI_BUSY_SIZE				1
#define SPI_TFNF_OFFSET				1
#define SPI_TFNF_SIZE				1
#define SPI_TFE_OFFSET				2
#define SPI_TFE_SIZE				1
#define SPI_RFNE_OFFSET				3
#define SPI_RFNE_SIZE				1
#define SPI_RFF_OFFSET				4
#define SPI_RFF_SIZE				1
#define SPI_TXE_OFFSET				5
#define SPI_TXE_SIZE				1
#define SPI_DCOL_OFFSET				6
#define SPI_DCOL_SIZE				1
#define SPI_ATFNF_OFFSET			8
#define SPI_ATFNF_SIZE				1
#define SPI_ATFE_OFFSET				9
#define SPI_ATFE_SIZE				1
#define SPI_ARFNE_OFFSET			10
#define SPI_ARFNE_SIZE				1
#define SPI_ARFF_OFFSET				11
#define SPI_ARFF_SIZE				1
#define SPI_ABTE_OFFSET				12
#define SPI_ABTE_SIZE				1

/* Bitfields in IMR */
#define SPI_TXEIM_OFFSET			0
#define SPI_TXEIM_SIZE				1
#define SPI_TXOIM_OFFSET			1
#define SPI_TXOIM_SIZE				1
#define SPI_RXUIM_OFFSET			2
#define SPI_RXUIM_SIZE				1
#define SPI_RXOIM_OFFSET			3
#define SPI_RXOIM_SIZE				1
#define SPI_RXFIM_OFFSET			4
#define SPI_RXFIM_SIZE				1
#define SPI_MSTIM_OFFSET			5
#define SPI_MSTIM_SIZE				1

/* Bitfields in IER */
#define SPI_TXEIS_OFFSET			0
#define SPI_TXEIS_SIZE				1
#define SPI_TXOIS_OFFSET			1
#define SPI_TXOIS_SIZE				1
#define SPI_RXUIS_OFFSET			2
#define SPI_RXUIS_SIZE				1
#define SPI_RXOIS_OFFSET			3
#define SPI_RXOIS_SIZE				1
#define SPI_RXFIS_OFFSET			4
#define SPI_RXFIS_SIZE				1
#define SPI_MSTIS_OFFSET			5
#define SPI_MSTIS_SIZE				1

/* Bitfields in RISR */
#define SPI_TXEIR_OFFSET			0
#define SPI_TXEIR_SIZE				1
#define SPI_TXOIR_OFFSET			1
#define SPI_TXOIR_SIZE				1
#define SPI_RXUIR_OFFSET			2
#define SPI_RXUIR_SIZE				1
#define SPI_RXOIR_OFFSET			3
#define SPI_RXOIR_SIZE				1
#define SPI_RXFIR_OFFSET			4
#define SPI_RXFIR_SIZE				1
#define SPI_MSTIR_OFFSET			5
#define SPI_MSTIR_SIZE				1

/* Bitfields in TXOICR */
#define SPI_TXOICR_OFFSET			0
#define SPI_TXOICR_SIZE				1

/* Bitfields in RXOICR */
#define SPI_RXOICR_OFFSET			0
#define SPI_RXOICR_SIZE				1

/* Bitfields in RXUICR */
#define SPI_RXUICR_OFFSET			0
#define SPI_RXUICR_SIZE				1

/* Bitfields in MSTICR */
#define SPI_MSTICR_OFFSET			0
#define SPI_MSTICR_SIZE				1

/* Bitfields in ICR */
#define SPI_ICR_OFFSET				0
#define SPI_ICR_SIZE				1

/* Bitfields in DMACR */
#define SPI_RDMAE_OFFSET			0
#define SPI_RDMAE_SIZE				1
#define SPI_TDMAE_OFFSET			1
#define SPI_TDMAE_SIZE				1

/* Bitfields in DMATDLR */
#define SPI_DMATDL_OFFSET			0
#define SPI_DMATDL_SIZE				8

/* Bitfields in RMATDLR */
#define SPI_DMARDL_OFFSET			0
#define SPI_DMARDL_SIZE				8

/* Bitfields in DR */
#define SPI_DR0_OFFSET				0
#define SPI_DR0_SIZE				16

/* Bitfields in RX_SAMPLE_DLAY */
#define SPI_RSD_OFFSET				0
#define SPI_RSD_SIZE				8

/* Bitfields in CCFGR */
#define SPI_WMOD_OFFSET				0
#define SPI_WMOD_SIZE				2
#define SPI_FLDFORM_OFFSET			2
#define SPI_FLDFORM_SIZE			2
#define SPI_FRMFORM_OFFSET			4
#define SPI_FRMFORM_SIZE			3
#define SPI_ADDRMODE_OFFSET			7
#define SPI_ADDRMODE_SIZE			1
#define SPI_DUMMYBYTE_OFFSET			8
#define SPI_DUMMYBYTE_SIZE			4
#define SPI_XIPEN_OFFSET			11
#define SPI_XIPEN_SIZE				1

/* Bitfields in OPCR */
#define SPI_RDOPC_OFFSET			0
#define SPI_RDOPC_SIZE				8
#define SPI_WROPC_OFFSET			8
#define SPI_WROPCS_SIZE				8
#define SPI_DPIEN_OFFSET			16
#define SPI_DPIEN_SIZE				1
#define SPI_QPIEN_OFFSET			17
#define SPI_QPIEN_SIZE				1
#define SPI_SPLITEN_OFFSET			18
#define SPI_SPLITEN_SIZE			1
#define SPI_PDATA_OFFSET			20
#define SPI_PDATA_SIZE				8


/* Constants for TMCR */
#define SPI_TSHSL_OFFSET			0
#define SPI_TSHSL_SIZE				4

#define SPI_BITS_8_BPT				0
#define SPI_BITS_9_BPT				1
#define SPI_BITS_10_BPT				2
#define SPI_BITS_11_BPT				3
#define SPI_BITS_12_BPT				4
#define SPI_BITS_13_BPT				5
#define SPI_BITS_14_BPT				6
#define SPI_BITS_15_BPT				7
#define SPI_BITS_16_BPT				8
#define SPI_ONE_DATA				0
#define SPI_TWO_DATA				1
#define SPI_FOUR_DATA				2

/* NFC_CMD_MODE Register bits. */
#define NFC_WRITE_CMD				BIT(0)
#define NFC_READ_CMD				BIT(1)
#define ECC_CAL_EB				BIT(2)
#define NFC_DATA_TYPE_OFFSET			3
#define	NFC_DATA_TYPE_SIZE			2
#define DATA_NONE				0
#define DATA_ONLY				1
#define DATA_WITH_SPARE				2
#define SPI_XF_MODE_OFFSET			8
#define SPI_XF_MODE_SIZE			2
#define DMA_MODE				BIT(10)
#define NFC_HIGHCLK_MODE			BIT(18)
#define NFC_CMD_TYPE_OFFSET			23
#define NFC_CMD_TYPE_SIZE			3
#define NFC_CONTINUE_MODE			BIT(26)
#define NFC_ADDR_4BYTE_MODE			BIT(27)

/* NFC_ACCESS_START Register bits. */
#define NFC_ACC_START				BIT(0)
#define NFC_ECC_STS_CLR				BIT(1)		//WC
#define NFC_FLOWBUF_CLR				BIT(2)		//WC


/* NFC_CMD_SET0 Register bits. */
#define NFC_ADDR_LENGTH_OFFSET			0
#define NFC_ADDR_LENGTH_SIZE			3
#define NFC_SPARE_LENGTH_OFFSET			8
#define NFC_SPARE_LENGTH_SIZE			8
#define NFC_DATA_LENGTH_OFFSET			16
#define NFC_DATA_LENGTH_SIZE			12
#define NFC_SET0_PAGE_SIZE_OFFSET		16
#define NFC_SET0_PAGE_SIZE_SIZE			12

/* NFC_CMD_SET1 Register bits. */
#define NFC_CMD_VALUE_OFFSET			0
#define NFC_CMD_VALUE_SIZE			8
#define NFC_ADDR1_OFFSET			8
#define NFC_ADDR1_SIZE				8
#define NFC_ADDR2_OFFSET			16
#define NFC_ADDR2_SIZE				8
#define NFC_ADDR3_OFFSET			24
#define NFC_ADDR3_SIZE				8

/* NFC_CMD_SET2 Register bits. */
#define NFC_ADDR4_OFFSET			0
#define NFC_ADDR4_SIZE				8
#define NFC_ADDR5_OFFSET			8
#define NFC_ADDR5_SIZE				8
#define NFC_ECC_POS_OFFSET			16
#define NFC_ECC_POS_SIZE			8
#define NFC_ECC_SPARE_OFFSET			24
#define NFC_ECC_SPARE_SIZE			8

/* NFC_CMD_SET3 Register Bits */
#define SPI_DATA_LENGTH_XC_OFFSET		0
#define SPI_DATA_LENGTH_XC_SIZE			24
#define SPI_RD_DLY_NUM_XC_OFFSET		24
#define SPI_RD_DLY_NUM_XC_SIZE			8
#define SPI_RD_DLY_NUM_MASK			(0xffffff)

/* NFC_CMD_SET4 Register Bits */
#define NFC_CCMD_DATA_LENGTH_XC_OFFSET		0
#define NFC_CCMD_DATA_LENGTH_XC_SIZE		16
#define SPARE_LEN_1K_OFFSET					16
#define SPARE_LEN_1K_MASK					0XFF
#define SPARE_SKIP_1K_OFFSET				24
#define SPARE_SKIP_1K_MASK				24

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

/*NFC_ECC_CFG0 fileds*/
#define ECC_DATA_LEN_OFS			0
#define ECC_DATA_LEN_MSK			0xfff

/*NFC_ECC_CFG1 fileds*/
#define ECC_SPARE_BYTE_NUM_OFS			0
#define ECC_SPARE_BYTE_NUM_MSK			0xff
#define ECC_SPARE_BASE_BYTE_ADDR_OFS	8
#define ECC_SPARE_BASE_BYTE_ADDR_MSK	0xff
#define ECC_ERASE_0_THD_OFS				16
#define ECC_ERASE_0_THD_MSK				0x1f
#define ECC_NO_IO_FLAG					BIT(21)
#define ECC_ERASE_CHECK_EB				BIT(22)
#define ECC_ENCODE_MODE					BIT(23)
#define ECC_MODE						BIT(24)
#define ECC_ECC_CAL_EB						BIT(25)

#define ECC_DECODE					0
#define ECC_ENCODE					1

#define ECC_ERASE_CHECK_THLD		4

/*NFC_ECC_CFG2 fileds*/
#define ECC_PAGE_SIZE_OFS			0
#define ECC_PAGE_SIZE_MSK			0xfff

#define SFC_DMA_NONE				0
#define SFC_DMA_RX					1
#define SFC_DMA_TX					2
/*NFC_BUF_SOFT fileds*/
#define NFC_SOFT_FB_P0_RDY_BIT			BIT(0)	//RO
#define NFC_SOFT_FB_P0_ACK			BIT(1)	//WC
#define NFC_SOFT_FB_P2_RDY_BIT			BIT(2)	//RO
#define NFC_SOFT_FB_P2_ACK			BIT(3)	//wc
#define NFC_IS_RUNNING_TX_DMA_CH0		BIT(16)	//RO
#define NFC_IS_RUNNING_TX_DMA_CH1		BIT(17)	//RO
#define NFC_IS_RUNNING_RX_DMA_CH0		BIT(18)	//RO
#define NFC_IS_RUNNING_RX_DMA_CH1		BIT(19)	//RO

/*NFC_DTR_CFG1 fileds*/
#define NFC_SCLK_DTR_QPI_MASK_VALUE_OFFSET	0
#define NFC_SCLK_DTR_QPI_MASK_VALUE_SIZE	8
#define NFC_SCLK_DTR_QPI_MASK_EN		BIT(8)

/*NFC_ECC_INFO0 fileds, all RO */
#define B0_ECC_STS_OFFSET			0
#define B0_ECC_STS_MASK				3
#define B1_ECC_STS_OFFSET			2
#define B1_ECC_STS_MASK				3
#define B2_ECC_STS_OFFSET			4
#define B2_ECC_STS_MASK				3
#define B3_ECC_STS_OFFSET			6
#define B3_ECC_STS_MASK				3
#define XC_B0_ERROR_NUM_OFFSET			8
#define XC_B0_ERROR_NUM_MASK			0x1F
#define XC_B1_ERROR_NUM_OFFSET			13
#define XC_B1_ERROR_NUM_MASK			0x1F
#define XC_B2_ERROR_NUM_OFFSET			18
#define XC_B2_ERROR_NUM_MASK			0x1F
#define XC_B3_ERROR_NUM_OFFSET			23
#define XC_B3_ERROR_NUM_MASK			0x1F

#define NFC_B0_ALL_FF_FLAG			BIT(28)
#define NFC_B1_ALL_FF_FLAG			BIT(29)
#define NFC_B2_ALL_FF_FLAG			BIT(30)
#define NFC_B3_ALL_FF_FLAG			BIT(31)

/*NFC_ECC_INFO1 fileds */
#define NFC_B0_ERASE_0_NUM_OFFSET		0
#define NFC_B0_ERASE_0_NUM_MASK			0xff
#define NFC_B1_ERASE_0_NUM_OFFSET		8
#define NFC_B1_ERASE_0_NUM_MASK			0xff
#define NFC_B2_ERASE_0_NUM_OFFSET		16
#define NFC_B2_ERASE_0_NUM_MASK			0xff
#define NFC_B3_ERASE_0_NUM_OFFSET		0
#define NFC_B3_ERASE_0_NUM_MASK			0xff

/*NFC_INTR_RAW fileds, RW1C*/
#define SPI_FINISH				BIT(0)
#define SCMD_FINISH				BIT(1)
#define ECC_ERR					BIT(8)
#define ECC_WARN				BIT(9)
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

/* NFC_DL_OUT */
#define NFC_DL_OUT_STEPS_OFFSET			0
#define NFC_DL_OUT_STEPS_MASK			0xff
#define NFC_DL_OUT_STEPS_UPDATE			BIT(8)
#define NFC_DL_OUT_GATE_EN			BIT(9)
#define NFC_DL_OUT_INV_EN			BIT(10)
#define NFC_DL_OUT_BYP_EN			BIT(11)

/*NFC_DL_OUT_LP */
#define NFC_DL_OUT_LP_STEPS_OFFSET		0
#define NFC_DL_OUT_LP_STEPS_MASK		0xff
#define NFC_DL_OUT_LP_STEPS_UPDATE		BIT(8)
#define NFC_DL_OUT_LP_GATE_EN			BIT(9)
#define NFC_DL_OUT_LP_INV_EN			BIT(10)
#define NFC_DL_OUT_LP_BYP_EN			BIT(11)
#define NFC_DL_OUT_LP_SRC_SEL_OFFSET		16
#define NFC_DL_OUT_LP_SRC_SEL_SIZE		2

/*NFC_CHIPSEL*/
#define NFC_CS_SEL_0				BIT(0)
#define NFC_CS_SEL_1				BIT(1)
#define NFC_CS_N_0				BIT(2)
#define NFC_CS_N_1				BIT(3)

/*NFC_FLASH_CFG*/
#define NFC_FLASH_CONFIG_OFFSET			0
#define NFC_FLASH_CONFIG_MASK			0xff
#define NFC_FLASH_STATUS_OFFSET			16
#define NFC_FLASH_STATUS_MASK			0xff
#define NFC_FLASH_STATUS_SIZE			8


/*NFC_DMA_COMMON*/
#define NFC_DMA_REG_INTER_NUM_OFFSET		0
#define NFC_DMA_REG_INTER_NUM_SIZE		8
#define FB_INCDEC_MODE				BIT(8)
#define NFC_RX_CH0_DMA_EN			BIT(28)
#define NFC_RX_CH1_DMA_EN			BIT(29)
#define NFC_TX_CH0_DMA_EN			BIT(30)
#define NFC_TX_CH1_DMA_EN			BIT(31)

/*NFC_DMA_TX_CH0*/
#define NFC_DMA_TX0_NUM_OFFSET			0
#define NFC_DMA_TX0_NUM_SIZE			16
#define NFC_DMA_TX0_LEN_OFFSET			16
#define NFC_DMA_TX0_LEN_SIZE			13
#define DMA_TX0_RUN				BIT(31)

/*NFC_DMA_TX_CH0_CFG1*/
#define NFC_DMA_TX0_CNT_INTR_CFG_OFFSET		0
#define NFC_DMA_TX0_CNT_INTR_CFG_SIZE		10

/*NFC_DMA_TX_CH1*/
#define NFC_DMA_TX1_NUM_OFFSET			0
#define NFC_DMA_TX1_NUM_SIZE			16
#define NFC_DMA_TX1_LEN_OFFSET			16
#define NFC_DMA_TX1_LEN_SIZE			13
#define DMA_TX1_RUN				BIT(31)

/*NFC_DMA_TX_CH1_CFG1*/
#define NFC_DMA_TX1_CNT_INTR_CFG_OFFSET		0
#define NFC_DMA_TX1_CNT_INTR_CFG_SIZE		10

/*NFC_DMA_RX_CH0*/
#define NFC_DMA_RX0_NUM_OFFSET			0
#define NFC_DMA_RX0_NUM_SIZE			16
#define NFC_DMA_RX0_LEN_OFFSET			16
#define NFC_DMA_RX0_LEN_SIZE			13
#define DMA_RX0_RUN				BIT(31)

/*NFC_DMA_RX_CH0_CFG1*/
#define NFC_DMA_RX0_CNT_INTR_CFG_OFFSET		0
#define NFC_DMA_RX0_CNT_INTR_CFG_SIZE		10

/*NFC_DMA_RX_CH1*/
#define NFC_DMA_RX1_NUM_OFFSET			0
#define NFC_DMA_RX1_NUM_SIZE			16
#define NFC_DMA_RX1_LEN_OFFSET			16
#define NFC_DMA_RX1_LEN_SIZE			13
#define DMA_RX1_RUN				BIT(31)

/*NFC_DMA_TX_CH1_CFG1*/
#define NFC_DMA_RX1_CNT_INTR_CFG_OFFSET		0
#define NFC_DMA_RX1_CNT_INTR_CFG_SIZE		10

/* Status Register 2 bits. */
#define SR2_QUAD_EN_BIT7			BIT(7)

#define SPIC_WAIT_READY				5000
#define SPIC_FB_READY				1000

#define FB_INCDEC_READ				1
#define FB_INCDEC_WRITE				0

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


#define spi_readl(reg)				__raw_readl(reg)
#define spi_writel(reg, value)			__raw_writel((value), reg)

#define __SWP32(A)				\
	((((u32)(A) & 0xff000000) >> 24) | (((u32)(A) & 0x00ff0000) >> 8) \
	| (((u32)(A) & 0x0000ff00) << 8) | (((u32)(A) & 0x000000ff) << 24))


struct wire_mode {
	u8 opcode;
	u8 addr;
	u8 data;
};

#define SPIC_CS_DIR				(0x13280008)
#define SPIC_CS_MASK				(0x13280004)
#define SPIC_CS_VAL				(0x13280000)

#define SPI_CS_OUTPUT()				\
	(__raw_writel(0x10, SPIC_CS_DIR))
#define SPI_CS_UNMASK()				\
	(__raw_writel(0x10, SPIC_CS_MASK))

#define CTRL_CS_HIGH(port)			\
	{ __raw_writel(0x10, SPIC_CS_VAL);	\
	spi_disalble_port(port); }

#define CTRL_CS_LOW(port)			\
	{ spi_enalble_port(port);		\
	__raw_writel(0, SPIC_CS_VAL); }

void enalbe_spic(u32 enable);
void reset_spic(void);
void spi_enable(u32 enable);
void spi_disalble_slave(u32 date);
void spi_set_polarity(u32 polarity);
void spi_set_phase(u32 phase);
void spi_cs_holdtime(unsigned int data);
void spi_set_dummy_byte(u8 dummy_byte);
void nfc_all_clr(void);
void nfc_dev_mode_clr(void);
void spi_set_frame_format(u32 format);
void spi_set_data_size(u32 size);
void spi_set_transfer_mode(u32 mode);
void spi_set_read_data_num(u32 num);
void spi_set_baudrate(u32 baudrate);
void spi_set_imr(unsigned int data);
void spi_disable_irq(u32 irq);
void spi_enable_irq(u32 irq);
u32 spi_get_status(void);
void spi_enalble_port(u32 port);
void spi_disalble_port(u32 port);
void spi_rx_sample_delay(u32 delay);
void spi_timing(u32 value);
void spi_set_field_format(u8 fld_form);
void spi_set_frame_form(u8 frm_form);
void spi_set_addr_mode(u8 addr_mode);
void sfc_cmd_write(void);
void sfc_cmd_read(void);
void sfc_ecc_enable(bool en);
void sfc_data_type(u32 type);
void sfc_addr_length(u32 len);
void sfc_spare_length(u32 len);
void sfc_data_length(u32 len);
void sfc_cmd(u32 cmd);
void ecc_encode(u32 encode);
bool get_ecc_sts(u32 num);
void sfc_addr(u8 *addr, u32 len);
void sfc_ecc_pos(u32 pos);
void sfc_dma_mode(void);
void sfc_write_spare(u8 *src, u32 len, u32 count);
void sfc_fill_spare(void);
void sfc_read_spare(u8 *dest, u32 len, u32 count);
void sfc_ecc_enc_mode(u32 encode);
void  sfc_spare_ecc_byte(u32 ecc);
void sfc_ecc_cor_mode(void);
void sfc_ecc_mode(u32 mode);
void nfc_addr4(unsigned int data);
void nfc_addr_length(unsigned int len);
void nfc_dma_common_clr(void);
void nfc_dma_en(unsigned int ch, unsigned char enable);
void nfc_fb_incdec_mode(unsigned char mode);
void nfc_clear_ecc(void);
void nfc_dma_reg_inter_num(unsigned char cycle);
void nfc_dma_rx_ch0_start(unsigned int num, unsigned int len,
	unsigned char run);
void spic_addr(unsigned int addr, unsigned int len);
void nfc_data_type(unsigned int type);
void nfc_read_en(void);
void nfc_set_page_size(unsigned int page_size);
void nfc_cmd_type(unsigned int type);
void nfc_ccmd_datalength(unsigned int len);
void nfc_data_length(unsigned int len);
void nfc_cmd(unsigned char opcode);
void nfc_set_tmod(unsigned char mode);
void nfc_set_fldform(unsigned char mode);
void nfc_set_frmform(unsigned char mode);
void nfc_set_cmd_wmode(unsigned char wire);
void nfc_set_data_wmode(unsigned char wire);
void nfc_set_phase_vld(unsigned int bits, unsigned char vld);
void nfc_check_buf_p2_rdy(void);
void ecc_sts_clr(void);
void sfc_start(void);
bool sfc_done(u32 finish);
void nfc_set_addr_wmode(u8 wire);
void access_start_clr(void);
void access_start_set(u32 able, u32 flowbuf);
void dma_rx0_cfg1(u32 interrupt);
void nfc_cmd_mode_clr(void);
void nfc_cmd_set_clr(void);
void nfc_dev_mode_clr(void);
void nfc_dma_common_reg_set(u32 able, u32 ch, u8 pclk_cycle, u32 mode);
void nfc_dma_rx_ch0_cfg1_reg_set(u32 interrupt);
void nfc_dma_rx_ch0_reg_set(u32 num, u32 len, u8 run);
void nfc_access_start_reg_set(u32 able, u32 flowbuf);
void nfc_cmd_mode_reg_set(u32 type);
void nfc_cmd_set_reg_set(u32 from, u32 page_num, u32 page_size,
	u32 opcode, u32 ccmd_pagesize);
void nfc_dev_mode_reg_set(u32 tmod, u32 fldform, u32 frmform, u32 data,
	u32 addr);
void write_nfc_soft_fb_p2_ack(void);
void nfc_flowbuf_clr(void);
void sfc_access_start(void);

void sfc_flowbuf_clr(void);
void sfc_dev_mode_reg_set(u32 frmform, u32 vld);
int sfc_start_exec(u8 dma);
u32 sfc_read_reg(void);
void sfc_write_reg(u32 cfg);
bool sfc_get_ecc_sts(u32 num);
void sfc_ecc_clr(void);
void sfc_ecc_page_size(u32 size);
void sfc_spare_len_1k(u32 len);
void sfc_dma_tx0_enable(void);
void sfc_dma_rx0_enable(void);
void sfc_dma_disable(void);
void sfc_dma_tx0_start(u32 len);
void sfc_dma_tx0_stop(void);
void sfc_dma_rx0_start(u32 len);
void sfc_dma_rx0_stop(void);
void sfc_read_ecc(u8 *dest, u32 len);
void sfc_ecc_erase_check_eb(u32 enable, u32 len);
int sfc_start_ecc_encode(void);
int sfc_start_ecc_decode(void);
void sfc_write_data_region0(u8 *src, u32 len);
void sfc_write_spare_region0(u8 *src, u32 len);
void nfc_dma_clr(void);
void sfc_fbuf2_ack(void);
void sfc_fbuf0_ack(void);
int sfc_wait_fbuf2_ready(void);
int sfc_wait_fbuf0_ready(void);

#endif

