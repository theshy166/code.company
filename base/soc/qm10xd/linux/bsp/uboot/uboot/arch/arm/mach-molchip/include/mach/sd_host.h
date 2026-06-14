#ifndef SD_HOST_H
#define SD_HOST_H
#include <common.h>

#define MMC_GO_IDLE_STATE		0 /* bc                          */
#define MMC_SEND_OP_COND		1 /* bcr  [31:0] OCR         R3  */
#define MMC_ALL_SEND_CID		2 /* bcr                     R2  */
#define MMC_SET_RELATIVE_ADDR		3 /* ac   [31:16] RCA        R1  */
#define MMC_SET_DSR			4 /* bc   [31:16] RCA            */
#define MMC_SLEEP_AWAKE			5 /* ac   [31:16] RCA 15:flg R1b */
#define MMC_SWITCH			6 /* ac   [31:0] See below   R1b */
#define MMC_SELECT_CARD			7 /* ac   [31:16] RCA        R1  */
#define MMC_SEND_EXT_CSD		8 /* adtc                    R1  */
#define MMC_SEND_CSD			9 /* ac   [31:16] RCA        R2  */
#define MMC_SEND_CID			10 /* ac   [31:16] RCA        R2  */
#define MMC_READ_DAT_UNTIL_STOP		11 /* adtc [31:0] dadr        R1  */
#define MMC_STOP_TRANSMISSION		12 /* ac                      R1b */
#define MMC_SEND_STATUS			13 /* ac   [31:16] RCA        R1  */
#define MMC_BUS_TEST_R			14 /* adtc                    R1  */
#define MMC_GO_INACTIVE_STATE		15 /* ac   [31:16] RCA            */
#define MMC_BUS_TEST_W			19 /* adtc                    R1  */
#define MMC_SPI_READ_OCR		58 /* spi                  spi_R3 */
#define MMC_SPI_CRC_ON_OFF		59 /* spi  [0:0] flag      spi_R1 */

  /* class 2 */
#define MMC_SET_BLOCKLEN		16 /* ac   [31:0] block len   R1  */
#define MMC_READ_SINGLE_BLOCK		17 /* adtc [31:0] data addr   R1  */
#define MMC_READ_MULTIPLE_BLOCK		18 /* adtc [31:0] data addr   R1  */
#define MMC_SEND_TUNING_BLOCK		19 /* adtc                    R1  */
#define MMC_SEND_TUNING_BLOCK_HS200	21 /* adtc R1  */

  /* class 3 */
#define MMC_WRITE_DAT_UNTIL_STOP	20 /* adtc [31:0] data addr   R1  */

  /* class 4 */
#define MMC_SET_BLOCK_COUNT		23 /* adtc [31:0] data addr   R1  */
#define MMC_WRITE_BLOCK			24 /* adtc [31:0] data addr   R1  */
#define MMC_WRITE_MULTIPLE_BLOCK	25 /* adtc                    R1  */
#define MMC_PROGRAM_CID			26 /* adtc                    R1  */
#define MMC_PROGRAM_CSD			27 /* adtc                    R1  */

  /* class 6 */
#define MMC_SET_WRITE_PROT		28 /* ac   [31:0] data addr   R1b */
#define MMC_CLR_WRITE_PROT		29 /* ac   [31:0] data addr   R1b */
#define MMC_SEND_WRITE_PROT		30 /* adtc [31:0] wpdata addr R1  */

  /* class 5 */
#define MMC_ERASE_GROUP_START		35 /* ac   [31:0] data addr   R1  */
#define MMC_ERASE_GROUP_END		36 /* ac   [31:0] data addr   R1  */
#define MMC_ERASE			38 /* ac                      R1b */

  /* class 9 */
#define MMC_FAST_IO			39 /* ac   <Complex>          R4  */
#define MMC_GO_IRQ_STATE		40 /* bcr                     R5  */

  /* class 7 */
#define MMC_LOCK_UNLOCK			42 /* adtc                    R1b */

  /* class 8 */
#define MMC_APP_CMD			55 /* ac   [31:16] RCA        R1  */
#define MMC_GEN_CMD			56 /* adtc [0] RD/WR          R1  */

  /* class 11 */
#define MMC_QUE_TASK_PARAMS		44 /* ac   [20:16] task id    R1  */
#define MMC_QUE_TASK_ADDR		45 /* ac   [31:0] data addr   R1  */
#define MMC_EXECUTE_READ_TASK		46 /* adtc [20:16] task id    R1  */
#define MMC_EXECUTE_WRITE_TASK		47 /* adtc [20:16] task id    R1  */
#define MMC_CMDQ_TASK_MGMT		48 /* ac   [20:16] task id    R1b */


/* SD commands                           type  argument     response */
/* class 0 */
/* This is basically the same command as for MMC with some quirks. */
#define SD_SEND_RELATIVE_ADDR		3 /* bcr                     R6  */
#define SD_SEND_IF_COND			8 /* bcr  [11:0] See below   R7  */
#define SD_SWITCH_VOLTAGE		11 /* ac                      R1  */

/* class 10 */
#define SD_SWITCH			6 /* adtc [31:0] See below   R1  */

/* class 5 */
#define SD_ERASE_WR_BLK_START		32 /* ac   [31:0] data addr   R1  */
#define SD_ERASE_WR_BLK_END		33 /* ac   [31:0] data addr   R1  */

/* Application commands */
#define SD_APP_SET_BUS_WIDTH		6 /* ac   [1:0] bus width    R1  */
#define SD_APP_SD_STATUS		13 /* adtc                    R1  */
#define SD_APP_SEND_NUM_WR_BLKS		22 /* adtc                    R1  */
#define SD_APP_OP_COND			41 /* bcr  [31:0] OCR         R3  */
#define SD_APP_SEND_SCR			51 /* adtc                    R1  */

#define MMC_RSP_PRESENT			(1 << 0)
#define MMC_RSP_136			(1 << 1) /* 136 bit response */
#define MMC_RSP_CRC			(1 << 2) /* expect valid crc */
#define MMC_RSP_BUSY			(1 << 3) /* card may send busy */
#define MMC_RSP_OPCODE			(1 << 4) /* response contains opcode */

#define MMC_CMD_MASK			(3 << 5) /* non-SPI command type */
#define MMC_CMD_AC			(0 << 5)
#define MMC_CMD_ADTC			(1 << 5)
#define MMC_CMD_BC			(2 << 5)
#define MMC_CMD_BCR			(3 << 5)

/*
 * These are the native response types, and correspond to valid bit
 * patterns of the above flags.  One additional valid pattern
 * is all zeros, which means we don't expect a response.
 */
#define MMC_RSP_NONE			(0)
#define MMC_RSP_R1			(MMC_RSP_PRESENT | MMC_RSP_CRC | \
	MMC_RSP_OPCODE)
#define MMC_RSP_R1B			(MMC_RSP_PRESENT | MMC_RSP_CRC | \
	MMC_RSP_OPCODE | MMC_RSP_BUSY)
#define MMC_RSP_R2			(MMC_RSP_PRESENT | MMC_RSP_136 | \
	MMC_RSP_CRC)
#define MMC_RSP_R3			(MMC_RSP_PRESENT)
#define MMC_RSP_R4			(MMC_RSP_PRESENT)
#define MMC_RSP_R5			(MMC_RSP_PRESENT | MMC_RSP_CRC | \
	MMC_RSP_OPCODE)
#define MMC_RSP_R6			(MMC_RSP_PRESENT | MMC_RSP_CRC | \
	MMC_RSP_OPCODE)
#define MMC_RSP_R7			(MMC_RSP_PRESENT | MMC_RSP_CRC | \
	MMC_RSP_OPCODE)

/*
 *MMC status in R1, for native mode (SPI bits are different)
  Type
	e : error bit
	s : status bit
	r : detected and set for the actual command response
	x : detected and set during command execution. the host must poll
	the card by sending status command in order to read these bits.
  Clear condition
	a : according to the card state
	b : always related to the previous command. Reception of
	a valid command will clear it (with a delay of one command)
	c : clear by read
 */

#define R1_OUT_OF_RANGE			(1 << 31) /* er, c */
#define R1_ADDRESS_ERROR		(1 << 30) /* erx, c */
#define R1_BLOCK_LEN_ERROR		(1 << 29) /* er, c */
#define R1_ERASE_SEQ_ERROR		(1 << 28) /* er, c */
#define R1_ERASE_PARAM			(1 << 27) /* ex, c */
#define R1_WP_VIOLATION			(1 << 26) /* erx, c */
#define R1_CARD_IS_LOCKED		(1 << 25) /* sx, a */
#define R1_LOCK_UNLOCK_FAILED		(1 << 24) /* erx, c */
#define R1_COM_CRC_ERROR		(1 << 23) /* er, b */
#define R1_ILLEGAL_COMMAND		(1 << 22) /* er, b */
#define R1_CARD_ECC_FAILED		(1 << 21) /* ex, c */
#define R1_CC_ERROR			(1 << 20) /* erx, c */
#define R1_ERROR			(1 << 19) /* erx, c */
#define R1_UNDERRUN			(1 << 18) /* ex, c */
#define R1_OVERRUN			(1 << 17) /* ex, c */
#define R1_CID_CSD_OVERWRITE		(1 << 16) /* erx, c, CID/CSD overwrite*/
#define R1_WP_ERASE_SKIP		(1 << 15) /* sx, c */
#define R1_CARD_ECC_DISABLED		(1 << 14) /* sx, a */
#define R1_ERASE_RESET			(1 << 13) /* sr, c */
#define R1_STATUS(x)			(x & 0xFFFFE000)
/* sx, b (4 bits) */
#define R1_CURRENT_STATE(x)		((x & 0x00001E00) >> 9)
#define R1_READY_FOR_DATA		(1 << 8) /* sx, a */
#define R1_SWITCH_ERROR			(1 << 7) /* sx, c */
#define R1_EXCEPTION_EVENT		(1 << 6) /* sr, a */
#define R1_APP_CMD			(1 << 5) /* sr, c */
/*
 * OCR bits are mostly in host.h
 */

#define SD_OCR_CCS			(1 << 30) /* Card Capacity Status */
#define MMC_CARD_BUSY			0x80000000 /* Card Power up status bit*/

#define MMC_CMD_RETRIES			3

#define SD_BUS_WIDTH_1			0
#define SD_BUS_WIDTH_4			2

/* Can be used by core to poll after switch to MMC HS mode */
#define MMC_RSP_R1_NO_CRC		(MMC_RSP_PRESENT | MMC_RSP_OPCODE)

#define mmc_resp_type(cmd)		((cmd)->flags & (MMC_RSP_PRESENT |\
	MMC_RSP_136 | MMC_RSP_CRC | MMC_RSP_BUSY | MMC_RSP_OPCODE))

/*
 * These are the command types.
 */
#define mmc_cmd_type(cmd)		((cmd)->flags & MMC_CMD_MASK)

#define SECTOR_SHIFT			9
#define SECTOR_SIZE			(1 << SECTOR_SHIFT)

/* generic data direction definitions */
#define READ				0
#define WRITE				1

#define MMC_POWER_OFF			0
#define MMC_POWER_UP			1
#define MMC_POWER_ON			2
#define MMC_POWER_UNDEFINED		3
#define MMC_BUS_WIDTH_1			0
#define MMC_BUS_WIDTH_4			2
#define MMC_BUS_WIDTH_8			3
#define MMC_TIMING_LEGACY		0
#define MMC_TIMING_MMC_HS		1
#define MMC_TIMING_SD_HS		2
#define MMC_TIMING_UHS_SDR12		3
#define MMC_TIMING_UHS_SDR25		4
#define MMC_TIMING_UHS_SDR50		5

#define MMC_VDD_165_195			0x00000080 /* VDD voltage 1.65 - 1.95 */
#define MMC_VDD_20_21			0x00000100 /* VDD voltage 2.0 ~ 2.1 */
#define MMC_VDD_21_22			0x00000200 /* VDD voltage 2.1 ~ 2.2 */
#define MMC_VDD_22_23			0x00000400 /* VDD voltage 2.2 ~ 2.3 */
#define MMC_VDD_23_24			0x00000800 /* VDD voltage 2.3 ~ 2.4 */
#define MMC_VDD_24_25			0x00001000 /* VDD voltage 2.4 ~ 2.5 */
#define MMC_VDD_25_26			0x00002000 /* VDD voltage 2.5 ~ 2.6 */
#define MMC_VDD_26_27			0x00004000 /* VDD voltage 2.6 ~ 2.7 */
#define MMC_VDD_27_28			0x00008000 /* VDD voltage 2.7 ~ 2.8 */
#define MMC_VDD_28_29			0x00010000 /* VDD voltage 2.8 ~ 2.9 */
#define MMC_VDD_29_30			0x00020000 /* VDD voltage 2.9 ~ 3.0 */
#define MMC_VDD_30_31			0x00040000 /* VDD voltage 3.0 ~ 3.1 */
#define MMC_VDD_31_32			0x00080000 /* VDD voltage 3.1 ~ 3.2 */
#define MMC_VDD_32_33			0x00100000 /* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34			0x00200000 /* VDD voltage 3.3 ~ 3.4 */
#define MMC_VDD_34_35			0x00400000 /* VDD voltage 3.4 ~ 3.5 */
#define MMC_VDD_35_36			0x00800000 /* VDD voltage 3.5 ~ 3.6 */

#define rq_data_dir(rq)			((rq) ? WRITE : READ)

#define MMC_DATA_WRITE			BIT(8)
#define MMC_DATA_READ			BIT(9)

/* Each descriptor can transfer up to 4KB of data in chained mode */
#define DW_MCI_DESC_DATA_LENGTH		0x1000

/* Number of descriptors in the ring buffer */
#define DW_MCI_DESC_RING_NUM		4096
#define BUS_HZ				50000000
#define TRANS_DATA_FREQS		50000000

#define SD_BOOT_ADDRESS			16	// 16 sector
#define WAIT_TIMEOUT_MS			1000

struct mmc_data {
	unsigned int blksz; /* data block size */
	unsigned int blocks; /* number of blocks */
	unsigned int blk_addr; /* block address */
	unsigned int address;
	unsigned int flags;
};

struct mmc_command {
	u32 opcode;
	u32 arg;
	u32 resp[4];
	unsigned int flags; /* expected response type */
	struct mmc_data *data;
};

struct idmac_desc {
	u32 des0; /* Control Descriptor */

	u32 des1; /* Buffer sizes */

	u32 des2; /* buffer 1 physical address */

	u32 des3; /* buffer 2 physical address */
};

void mci_selection(u32 boot_mode);
int dw_mci_start_command(struct mmc_command *cmd, u32 cmd_flags);
u32 dw_mci_prepare_command(struct mmc_command *cmd);
void dw_mci_prepare_desc32(u32 mem_addr, u32 length);
void dw_mci_idmac_start_dma(u32 addr, unsigned int len);
int dw_mci_submit_data_dma(u32 address, u32 length);
int dw_mci_start_request(struct mmc_command *cmd);
void dw_mci_setup_bus(u32 clock, u32 ctype);
void dw_mci_set_ios(u32 clock, u32 bus_width);
void dw_mci_hw_reset(void);
int dw_mci_init_dma(void);
int dw_mci_init(void);

#endif

