#ifndef SD_MMC_H
#define SD_MMC_H

#define ADMA2_LINK	0x31
#define ADMA2_TRAN_VALID	0x21
#define ADMA2_NOP_END_VALID	0x3
#define ADMA2_END		0x2

#define MMC_GO_IDLE_STATE         0   /* bc                          */
#define MMC_SEND_OP_COND          1   /* bcr  [31:0] OCR         R3  */
#define MMC_ALL_SEND_CID          2   /* bcr                     R2  */
#define MMC_SET_RELATIVE_ADDR     3   /* ac   [31:16] RCA        R1  */
#define MMC_SET_DSR               4   /* bc   [31:16] RCA            */
#define MMC_SLEEP_AWAKE		  5   /* ac   [31:16] RCA 15:flg R1b */
#define MMC_SWITCH                6   /* ac   [31:0] See below   R1b */
#define MMC_SELECT_CARD           7   /* ac   [31:16] RCA        R1  */
#define MMC_SEND_EXT_CSD          8   /* adtc                    R1  */
#define MMC_SEND_CSD              9   /* ac   [31:16] RCA        R2  */
#define MMC_SEND_CID             10   /* ac   [31:16] RCA        R2  */
#define MMC_READ_DAT_UNTIL_STOP  11   /* adtc [31:0] dadr        R1  */
#define MMC_STOP_TRANSMISSION    12   /* ac                      R1b */
#define MMC_SEND_STATUS          13   /* ac   [31:16] RCA        R1  */
#define MMC_BUS_TEST_R           14   /* adtc                    R1  */
#define MMC_GO_INACTIVE_STATE    15   /* ac   [31:16] RCA            */
#define MMC_BUS_TEST_W           19   /* adtc                    R1  */
#define MMC_SPI_READ_OCR         58   /* spi                  spi_R3 */
#define MMC_SPI_CRC_ON_OFF       59   /* spi  [0:0] flag      spi_R1 */

  /* class 2 */
#define MMC_SET_BLOCKLEN         16   /* ac   [31:0] block len   R1  */
#define MMC_READ_SINGLE_BLOCK    17   /* adtc [31:0] data addr   R1  */
#define MMC_READ_MULTIPLE_BLOCK  18   /* adtc [31:0] data addr   R1  */
#define MMC_SEND_TUNING_BLOCK    19   /* adtc                    R1  */
#define MMC_SEND_TUNING_BLOCK_HS200	21	/* adtc R1  */

  /* class 3 */
#define MMC_WRITE_DAT_UNTIL_STOP 20   /* adtc [31:0] data addr   R1  */

  /* class 4 */
#define MMC_SET_BLOCK_COUNT      23   /* adtc [31:0] data addr   R1  */
#define MMC_WRITE_BLOCK          24   /* adtc [31:0] data addr   R1  */
#define MMC_WRITE_MULTIPLE_BLOCK 25   /* adtc                    R1  */
#define MMC_PROGRAM_CID          26   /* adtc                    R1  */
#define MMC_PROGRAM_CSD          27   /* adtc                    R1  */

  /* class 6 */
#define MMC_SET_WRITE_PROT       28   /* ac   [31:0] data addr   R1b */
#define MMC_CLR_WRITE_PROT       29   /* ac   [31:0] data addr   R1b */
#define MMC_SEND_WRITE_PROT      30   /* adtc [31:0] wpdata addr R1  */

  /* class 5 */
#define MMC_ERASE_GROUP_START    35   /* ac   [31:0] data addr   R1  */
#define MMC_ERASE_GROUP_END      36   /* ac   [31:0] data addr   R1  */
#define MMC_ERASE                38   /* ac                      R1b */

  /* class 9 */
#define MMC_FAST_IO              39   /* ac   <Complex>          R4  */
#define MMC_GO_IRQ_STATE         40   /* bcr                     R5  */

  /* class 7 */
#define MMC_LOCK_UNLOCK          42   /* adtc                    R1b */

  /* class 8 */
#define MMC_APP_CMD              55   /* ac   [31:16] RCA        R1  */
#define MMC_GEN_CMD              56   /* adtc [0] RD/WR          R1  */

  /* class 11 */
#define MMC_QUE_TASK_PARAMS      44   /* ac   [20:16] task id    R1  */
#define MMC_QUE_TASK_ADDR        45   /* ac   [31:0] data addr   R1  */
#define MMC_EXECUTE_READ_TASK    46   /* adtc [20:16] task id    R1  */
#define MMC_EXECUTE_WRITE_TASK   47   /* adtc [20:16] task id    R1  */
#define MMC_CMDQ_TASK_MGMT       48   /* ac   [20:16] task id    R1b */


/* SD commands                           type  argument     response */
  /* class 0 */
/* This is basically the same command as for MMC with some quirks. */
#define SD_SEND_RELATIVE_ADDR     3   /* bcr                     R6  */
#define SD_SEND_IF_COND           8   /* bcr  [11:0] See below   R7  */
#define SD_SWITCH_VOLTAGE         11  /* ac                      R1  */

  /* class 10 */
#define SD_SWITCH                 6   /* adtc [31:0] See below   R1  */

  /* class 5 */
#define SD_ERASE_WR_BLK_START    32   /* ac   [31:0] data addr   R1  */
#define SD_ERASE_WR_BLK_END      33   /* ac   [31:0] data addr   R1  */

  /* Application commands */
#define SD_APP_SET_BUS_WIDTH      6   /* ac   [1:0] bus width    R1  */
#define SD_APP_SD_STATUS         13   /* adtc                    R1  */
#define SD_APP_SEND_NUM_WR_BLKS  22   /* adtc                    R1  */
#define SD_APP_OP_COND           41   /* bcr  [31:0] OCR         R3  */
#define SD_APP_SEND_SCR          51   /* adtc                    R1  */

#define MMC_RSP_PRESENT	(1 << 0)
#define MMC_RSP_136	(1 << 1)		/* 136 bit response */
#define MMC_RSP_CRC	(1 << 2)		/* expect valid crc */
#define MMC_RSP_BUSY	(1 << 3)		/* card may send busy */
#define MMC_RSP_OPCODE	(1 << 4)		/* response contains opcode */

#define MMC_CMD_MASK	(3 << 5)		/* non-SPI command type */
#define MMC_CMD_AC	(0 << 5)
#define MMC_CMD_ADTC	(1 << 5)
#define MMC_CMD_BC	(2 << 5)
#define MMC_CMD_BCR	(3 << 5)

/*
 * These are the native response types, and correspond to valid bit
 * patterns of the above flags.  One additional valid pattern
 * is all zeros, which means we don't expect a response.
 */
#define MMC_RSP_NONE	(0)
#define MMC_RSP_R1	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2	(MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3	(MMC_RSP_PRESENT)
#define MMC_RSP_R4	(MMC_RSP_PRESENT)
#define MMC_RSP_R5	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

/*
  MMC status in R1, for native mode (SPI bits are different)
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

#define R1_OUT_OF_RANGE		(1 << 31)	/* er, c */
#define R1_ADDRESS_ERROR	(1 << 30)	/* erx, c */
#define R1_BLOCK_LEN_ERROR	(1 << 29)	/* er, c */
#define R1_ERASE_SEQ_ERROR      (1 << 28)	/* er, c */
#define R1_ERASE_PARAM		(1 << 27)	/* ex, c */
#define R1_WP_VIOLATION		(1 << 26)	/* erx, c */
#define R1_CARD_IS_LOCKED	(1 << 25)	/* sx, a */
#define R1_LOCK_UNLOCK_FAILED	(1 << 24)	/* erx, c */
#define R1_COM_CRC_ERROR	(1 << 23)	/* er, b */
#define R1_ILLEGAL_COMMAND	(1 << 22)	/* er, b */
#define R1_CARD_ECC_FAILED	(1 << 21)	/* ex, c */
#define R1_CC_ERROR		(1 << 20)	/* erx, c */
#define R1_ERROR		(1 << 19)	/* erx, c */
#define R1_UNDERRUN		(1 << 18)	/* ex, c */
#define R1_OVERRUN		(1 << 17)	/* ex, c */
#define R1_CID_CSD_OVERWRITE	(1 << 16)	/* erx, c, CID/CSD overwrite */
#define R1_WP_ERASE_SKIP	(1 << 15)	/* sx, c */
#define R1_CARD_ECC_DISABLED	(1 << 14)	/* sx, a */
#define R1_ERASE_RESET		(1 << 13)	/* sr, c */
#define R1_STATUS(x)            (x & 0xFFFFE000)
#define R1_CURRENT_STATE(x)	((x & 0x00001E00) >> 9)	/* sx, b (4 bits) */
#define R1_READY_FOR_DATA	(1 << 8)	/* sx, a */
#define R1_SWITCH_ERROR		(1 << 7)	/* sx, c */
#define R1_EXCEPTION_EVENT	(1 << 6)	/* sr, a */
#define R1_APP_CMD		(1 << 5)	/* sr, c */
/*
 * OCR bits are mostly in host.h
 */

#define SD_OCR_CCS		(1 << 30)    /* Card Capacity Status */
#define MMC_CARD_BUSY	0x80000000	/* Card Power up status bit */

#define MMC_CMD_RETRIES        3

#define SD_BUS_WIDTH_1		0
#define SD_BUS_WIDTH_4		2

/* Can be used by core to poll after switch to MMC HS mode */
#define MMC_RSP_R1_NO_CRC	(MMC_RSP_PRESENT|MMC_RSP_OPCODE)

#define mmc_resp_type(cmd)	((cmd)->flags & (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC|MMC_RSP_BUSY|MMC_RSP_OPCODE))

/*
 * These are the command types.
 */
#define mmc_cmd_type(cmd)	((cmd)->flags & MMC_CMD_MASK)

#define SECTOR_SHIFT 9
#define SECTOR_SIZE (1 << SECTOR_SHIFT)

/* generic data direction definitions */
#define READ			0
#define WRITE			1

#define MMC_POWER_OFF		0
#define MMC_POWER_UP		1
#define MMC_POWER_ON		2
#define MMC_POWER_UNDEFINED	3
#define MMC_BUS_WIDTH_1		0
#define MMC_BUS_WIDTH_4		2
#define MMC_BUS_WIDTH_8		3
#define MMC_TIMING_LEGACY	0
#define MMC_TIMING_MMC_HS	1
#define MMC_TIMING_SD_HS	2
#define MMC_TIMING_UHS_SDR12	3
#define MMC_TIMING_UHS_SDR25	4
#define MMC_TIMING_UHS_SDR50	5

#define MMC_VDD_165_195		0x00000080	/* VDD voltage 1.65 - 1.95 */
#define MMC_VDD_20_21		0x00000100	/* VDD voltage 2.0 ~ 2.1 */
#define MMC_VDD_21_22		0x00000200	/* VDD voltage 2.1 ~ 2.2 */
#define MMC_VDD_22_23		0x00000400	/* VDD voltage 2.2 ~ 2.3 */
#define MMC_VDD_23_24		0x00000800	/* VDD voltage 2.3 ~ 2.4 */
#define MMC_VDD_24_25		0x00001000	/* VDD voltage 2.4 ~ 2.5 */
#define MMC_VDD_25_26		0x00002000	/* VDD voltage 2.5 ~ 2.6 */
#define MMC_VDD_26_27		0x00004000	/* VDD voltage 2.6 ~ 2.7 */
#define MMC_VDD_27_28		0x00008000	/* VDD voltage 2.7 ~ 2.8 */
#define MMC_VDD_28_29		0x00010000	/* VDD voltage 2.8 ~ 2.9 */
#define MMC_VDD_29_30		0x00020000	/* VDD voltage 2.9 ~ 3.0 */
#define MMC_VDD_30_31		0x00040000	/* VDD voltage 3.0 ~ 3.1 */
#define MMC_VDD_31_32		0x00080000	/* VDD voltage 3.1 ~ 3.2 */
#define MMC_VDD_32_33		0x00100000	/* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34		0x00200000	/* VDD voltage 3.3 ~ 3.4 */
#define MMC_VDD_34_35		0x00400000	/* VDD voltage 3.4 ~ 3.5 */
#define MMC_VDD_35_36		0x00800000	/* VDD voltage 3.5 ~ 3.6 */

#define rq_data_dir(rq)		((rq) ? WRITE : READ)

#define MMC_DATA_WRITE		BIT(8)
#define MMC_DATA_READ		BIT(9)
struct mmc_data {
	unsigned int		blksz;		/* data block size */
	unsigned int		blocks;		/* number of blocks */
	unsigned int		blk_addr;	/* block address */
	ulong	 		address;
	unsigned int		flags;
};

struct mmc_request {
	struct mmc_command	*sbc;		/* SET_BLOCK_COUNT for multiblock */
	struct mmc_command	*cmd;
	struct mmc_data		*data;
	struct mmc_command	*stop;
};

struct mmc_command {
	u32			opcode;
	u32			arg;
	u32			resp[4];
	unsigned int		flags;		/* expected response type */
	struct mmc_data  *data;
};

/*
 * Controller registers
 */
#define SDHCI_DMA_ADDRESS	0x00
#define SDHCI_ARGUMENT2		SDHCI_DMA_ADDRESS
#define SDHCI_32BIT_BLK_CNT	SDHCI_DMA_ADDRESS

#define SDHCI_ARGUMENT2		SDHCI_DMA_ADDRESS

#define SDHCI_BLOCK_SIZE	0x04
#define  SDHCI_MAKE_BLKSZ(dma, blksz) (((dma & 0x7) << 12) | (blksz & 0xFFF))

#define SDHCI_BLOCK_COUNT	0x06

#define SDHCI_ARGUMENT		0x08

#define SDHCI_TRANSFER_MODE	0x0C
#define  SDHCI_TRNS_DMA		0x01
#define  SDHCI_TRNS_BLK_CNT_EN	0x02
#define  SDHCI_TRNS_AUTO_CMD12	0x04
#define  SDHCI_TRNS_AUTO_CMD23	0x08
#define  SDHCI_TRNS_READ	0x10
#define  SDHCI_TRNS_MULTI	0x20

#define SDHCI_COMMAND		0x0E
#define  SDHCI_CMD_RESP_MASK	0x03
#define  SDHCI_CMD_CRC		0x08
#define  SDHCI_CMD_INDEX	0x10
#define  SDHCI_CMD_DATA		0x20
#define  SDHCI_CMD_ABORTCMD	0xC0

#define  SDHCI_CMD_RESP_NONE	0x00
#define  SDHCI_CMD_RESP_LONG	0x01
#define  SDHCI_CMD_RESP_SHORT	0x02
#define  SDHCI_CMD_RESP_SHORT_BUSY 0x03

#define SDHCI_MAKE_CMD(c, f) (((c & 0xff) << 8) | (f & 0xff))
#define SDHCI_GET_CMD(c) ((c>>8) & 0x3f)

#define SDHCI_RESPONSE		0x10

#define SDHCI_BUFFER		0x20

#define SDHCI_PRESENT_STATE	0x24
#define  SDHCI_CMD_INHIBIT	0x00000001
#define  SDHCI_DATA_INHIBIT	0x00000002
#define  SDHCI_DOING_WRITE	0x00000100
#define  SDHCI_DOING_READ	0x00000200
#define  SDHCI_SPACE_AVAILABLE	0x00000400
#define  SDHCI_DATA_AVAILABLE	0x00000800
#define  SDHCI_CARD_PRESENT	0x00010000
#define  SDHCI_WRITE_PROTECT	0x00080000
#define  SDHCI_DATA_LVL_MASK	0x00F00000
#define   SDHCI_DATA_LVL_SHIFT	20
#define   SDHCI_DATA_0_LVL_MASK	0x00100000
#define  SDHCI_CMD_LVL		0x01000000

#define SDHCI_HOST_CONTROL	0x28
#define  SDHCI_CTRL_LED		0x01
#define  SDHCI_CTRL_4BITBUS	0x02
#define  SDHCI_CTRL_HISPD	0x04
#define  SDHCI_CTRL_DMA_MASK	0x18
#define   SDHCI_CTRL_SDMA	0x00
#define   SDHCI_CTRL_ADMA1	0x08
#define   SDHCI_CTRL_ADMA32	0x10
#define   SDHCI_CTRL_ADMA64	0x18
#define   SDHCI_CTRL_8BITBUS	0x20
#define  SDHCI_CTRL_CDTEST_INS	0x40
#define  SDHCI_CTRL_CDTEST_EN	0x80

#define SDHCI_POWER_CONTROL	0x29
#define  SDHCI_POWER_ON		0x01
#define  SDHCI_POWER_180	0x0A
#define  SDHCI_POWER_300	0x0C
#define  SDHCI_POWER_330	0x0E

#define SDHCI_BLOCK_GAP_CONTROL	0x2A

#define SDHCI_WAKE_UP_CONTROL	0x2B
#define  SDHCI_WAKE_ON_INT	0x01
#define  SDHCI_WAKE_ON_INSERT	0x02
#define  SDHCI_WAKE_ON_REMOVE	0x04

#define SDHCI_CLOCK_CONTROL	0x2C
#define  SDHCI_DIVIDER_SHIFT	8
#define  SDHCI_DIVIDER_HI_SHIFT	6
#define  SDHCI_DIV_MASK	0xFF
#define  SDHCI_DIV_MASK_LEN	8
#define  SDHCI_DIV_HI_MASK	0x300
#define  SDHCI_PROG_CLOCK_MODE	0x0020
#define  SDHCI_CLOCK_CARD_EN	0x0004
#define  SDHCI_CLOCK_INT_STABLE	0x0002
#define  SDHCI_CLOCK_INT_EN	0x0001

#define SDHCI_TIMEOUT_CONTROL	0x2E

#define SDHCI_SOFTWARE_RESET	0x2F
#define  SDHCI_RESET_ALL	0x01
#define  SDHCI_RESET_CMD	0x02
#define  SDHCI_RESET_DATA	0x04

#define SDHCI_INT_STATUS	0x30
#define SDHCI_INT_ENABLE	0x34
#define SDHCI_SIGNAL_ENABLE	0x38
#define  SDHCI_INT_RESPONSE	0x00000001
#define  SDHCI_INT_DATA_END	0x00000002
#define  SDHCI_INT_BLK_GAP	0x00000004
#define  SDHCI_INT_DMA_END	0x00000008
#define  SDHCI_INT_SPACE_AVAIL	0x00000010
#define  SDHCI_INT_DATA_AVAIL	0x00000020
#define  SDHCI_INT_CARD_INSERT	0x00000040
#define  SDHCI_INT_CARD_REMOVE	0x00000080
#define  SDHCI_INT_CARD_INT	0x00000100
#define  SDHCI_INT_RETUNE	0x00001000
#define  SDHCI_INT_ERROR	0x00008000
#define  SDHCI_INT_TIMEOUT	0x00010000
#define  SDHCI_INT_CRC		0x00020000
#define  SDHCI_INT_END_BIT	0x00040000
#define  SDHCI_INT_INDEX	0x00080000
#define  SDHCI_INT_DATA_TIMEOUT	0x00100000
#define  SDHCI_INT_DATA_CRC	0x00200000
#define  SDHCI_INT_DATA_END_BIT	0x00400000
#define  SDHCI_INT_BUS_POWER	0x00800000
#define  SDHCI_INT_ACMD12ERR	0x01000000
#define  SDHCI_INT_ADMA_ERROR	0x02000000

#define  SDHCI_INT_NORMAL_MASK	0x00007FFF
#define  SDHCI_INT_ERROR_MASK	0xFFFF8000

#define  SDHCI_INT_CMD_MASK	(SDHCI_INT_RESPONSE | SDHCI_INT_TIMEOUT | \
		SDHCI_INT_CRC | SDHCI_INT_END_BIT | SDHCI_INT_INDEX)
#define  SDHCI_INT_DATA_MASK	(SDHCI_INT_DATA_END | SDHCI_INT_DMA_END | \
		SDHCI_INT_DATA_AVAIL | SDHCI_INT_SPACE_AVAIL | \
		SDHCI_INT_DATA_TIMEOUT | SDHCI_INT_DATA_CRC | \
		SDHCI_INT_DATA_END_BIT | SDHCI_INT_ADMA_ERROR | \
		SDHCI_INT_BLK_GAP)
#define SDHCI_INT_ALL_MASK	((unsigned int)-1)

#define SDHCI_ACMD12_ERR	0x3C

#define SDHCI_HOST_CONTROL2		0x3E
#define  SDHCI_CTRL_UHS_MASK		0x0007
#define   SDHCI_CTRL_UHS_SDR12		0x0000
#define   SDHCI_CTRL_UHS_SDR25		0x0001
#define   SDHCI_CTRL_UHS_SDR50		0x0002
#define   SDHCI_CTRL_UHS_SDR104		0x0003
#define   SDHCI_CTRL_UHS_DDR50		0x0004
#define   SDHCI_CTRL_HS400		0x0007 /* Non-standard */
#define  SDHCI_CTRL_VDD_180		0x0008
#define  SDHCI_CTRL_DRV_TYPE_MASK	0x0030
#define   SDHCI_CTRL_DRV_TYPE_B		0x0000
#define   SDHCI_CTRL_DRV_TYPE_A		0x0010
#define   SDHCI_CTRL_DRV_TYPE_C		0x0020
#define   SDHCI_CTRL_DRV_TYPE_D		0x0030
#define  SDHCI_CTRL_EXEC_TUNING		0x0040
#define  SDHCI_CTRL_TUNED_CLK		0x0080
#define  SDHCI_CMD23_ENABLE		0x0800
#define  SDHCI_CTRL_V4_MODE		0x1000
#define  SDHCI_CTRL_64BIT_ADDR		0x2000
#define  SDHCI_CTRL_PRESET_VAL_ENABLE	0x8000

#define SDHCI_CAPABILITIES	0x40
#define  SDHCI_TIMEOUT_CLK_MASK	0x0000003F
#define  SDHCI_TIMEOUT_CLK_SHIFT 0
#define  SDHCI_TIMEOUT_CLK_UNIT	0x00000080
#define  SDHCI_CLOCK_BASE_MASK	0x00003F00
#define  SDHCI_CLOCK_V3_BASE_MASK	0x0000FF00
#define  SDHCI_CLOCK_BASE_SHIFT	8
#define  SDHCI_MAX_BLOCK_MASK	0x00030000
#define  SDHCI_MAX_BLOCK_SHIFT  16
#define  SDHCI_CAN_DO_8BIT	0x00040000
#define  SDHCI_CAN_DO_ADMA2	0x00080000
#define  SDHCI_CAN_DO_ADMA1	0x00100000
#define  SDHCI_CAN_DO_HISPD	0x00200000
#define  SDHCI_CAN_DO_SDMA	0x00400000
#define  SDHCI_CAN_DO_SUSPEND	0x00800000
#define  SDHCI_CAN_VDD_330	0x01000000
#define  SDHCI_CAN_VDD_300	0x02000000
#define  SDHCI_CAN_VDD_180	0x04000000
#define  SDHCI_CAN_64BIT	0x10000000

#define  SDHCI_SUPPORT_SDR50	0x00000001
#define  SDHCI_SUPPORT_SDR104	0x00000002
#define  SDHCI_SUPPORT_DDR50	0x00000004
#define  SDHCI_DRIVER_TYPE_A	0x00000010
#define  SDHCI_DRIVER_TYPE_C	0x00000020
#define  SDHCI_DRIVER_TYPE_D	0x00000040
#define  SDHCI_RETUNING_TIMER_COUNT_MASK	0x00000F00
#define  SDHCI_RETUNING_TIMER_COUNT_SHIFT	8
#define  SDHCI_USE_SDR50_TUNING			0x00002000
#define  SDHCI_RETUNING_MODE_MASK		0x0000C000
#define  SDHCI_RETUNING_MODE_SHIFT		14
#define  SDHCI_CLOCK_MUL_MASK	0x00FF0000
#define  SDHCI_CLOCK_MUL_SHIFT	16
#define  SDHCI_SUPPORT_HS400	0x80000000 /* Non-standard */

#define SDHCI_CAPABILITIES_1	0x44

#define SDHCI_MAX_CURRENT		0x48
#define  SDHCI_MAX_CURRENT_LIMIT	0xFF
#define  SDHCI_MAX_CURRENT_330_MASK	0x0000FF
#define  SDHCI_MAX_CURRENT_330_SHIFT	0
#define  SDHCI_MAX_CURRENT_300_MASK	0x00FF00
#define  SDHCI_MAX_CURRENT_300_SHIFT	8
#define  SDHCI_MAX_CURRENT_180_MASK	0xFF0000
#define  SDHCI_MAX_CURRENT_180_SHIFT	16
#define   SDHCI_MAX_CURRENT_MULTIPLIER	4

/* 4C-4F reserved for more max current */

#define SDHCI_SET_ACMD12_ERROR	0x50
#define SDHCI_SET_INT_ERROR	0x52

#define SDHCI_ADMA_ERROR	0x54

/* 55-57 reserved */

#define SDHCI_ADMA_ADDRESS	0x58
#define SDHCI_ADMA_ADDRESS_HI	0x5C

/* 60-FB reserved */

#define SDHCI_PRESET_FOR_SDR12 0x66
#define SDHCI_PRESET_FOR_SDR25 0x68
#define SDHCI_PRESET_FOR_SDR50 0x6A
#define SDHCI_PRESET_FOR_SDR104        0x6C
#define SDHCI_PRESET_FOR_DDR50 0x6E
#define SDHCI_PRESET_FOR_HS400 0x74 /* Non-standard */
#define SDHCI_PRESET_DRV_MASK  0xC000
#define SDHCI_PRESET_DRV_SHIFT  14
#define SDHCI_PRESET_CLKGEN_SEL_MASK   0x400
#define SDHCI_PRESET_CLKGEN_SEL_SHIFT	10
#define SDHCI_PRESET_SDCLK_FREQ_MASK   0x3FF
#define SDHCI_PRESET_SDCLK_FREQ_SHIFT	0

#define SDHCI_SLOT_INT_STATUS	0xFC

#define SDHCI_HOST_VERSION	0xFE
#define  SDHCI_VENDOR_VER_MASK	0xFF00
#define  SDHCI_VENDOR_VER_SHIFT	8
#define  SDHCI_SPEC_VER_MASK	0x00FF
#define  SDHCI_SPEC_VER_SHIFT	0
#define   SDHCI_SPEC_100	0
#define   SDHCI_SPEC_200	1
#define   SDHCI_SPEC_300	2

#define EMMC_CTRL_R			0x52c
#define CARD_IS_EMMC		BIT(0)

#define BOOT_CTRL_R			0x52e
#define BOOT_TOUT_CNT_OFS	12
#define BOOT_TOUT_CNT_MSK	0x7
#define	BOOT_ACK_ENABLE		BIT(8)
#define VALIDATE_BOOT		BIT(7)
#define BOOT_EN				BIT(0)

#define SDHCI_MAX_DIV_SPEC_300	2046
#define SDHCI_CLK_BASE			240000000

#define EMMC_WAIT_TIMEOUT		100

/*
 * MMC_SWITCH access modes
 */
#define MMC_SWITCH_MODE_CMD_SET		0x00	/* Change the command set */
#define MMC_SWITCH_MODE_SET_BITS	0x01	/* Set bits which are 1 in value */
#define MMC_SWITCH_MODE_CLEAR_BITS	0x02	/* Clear bits which are 1 in value */
#define MMC_SWITCH_MODE_WRITE_BYTE	0x03	/* Set target to value */

#define EXT_CSD_CMD_SET_NORMAL		(1<<0)
#define EXT_CSD_CMD_SET_SECURE		(1<<1)
#define EXT_CSD_CMD_SET_CPSECURE	(1<<2)

/*
 * EXT_CSD fields
 */

#define EXT_CSD_CMDQ_MODE_EN		15	/* R/W */
#define EXT_CSD_FLUSH_CACHE		32      /* W */
#define EXT_CSD_CACHE_CTRL		33      /* R/W */
#define EXT_CSD_POWER_OFF_NOTIFICATION	34	/* R/W */
#define EXT_CSD_PACKED_FAILURE_INDEX	35	/* RO */
#define EXT_CSD_PACKED_CMD_STATUS	36	/* RO */
#define EXT_CSD_EXP_EVENTS_STATUS	54	/* RO, 2 bytes */
#define EXT_CSD_EXP_EVENTS_CTRL		56	/* R/W, 2 bytes */
#define EXT_CSD_DATA_SECTOR_SIZE	61	/* R */
#define EXT_CSD_GP_SIZE_MULT		143	/* R/W */
#define EXT_CSD_PARTITION_SETTING_COMPLETED 155	/* R/W */
#define EXT_CSD_PARTITION_ATTRIBUTE	156	/* R/W */
#define EXT_CSD_PARTITION_SUPPORT	160	/* RO */
#define EXT_CSD_HPI_MGMT		161	/* R/W */
#define EXT_CSD_RST_N_FUNCTION		162	/* R/W */
#define EXT_CSD_BKOPS_EN		163	/* R/W */
#define EXT_CSD_BKOPS_START		164	/* W */
#define EXT_CSD_SANITIZE_START		165     /* W */
#define EXT_CSD_WR_REL_PARAM		166	/* RO */
#define EXT_CSD_RPMB_MULT		168	/* RO */
#define EXT_CSD_FW_CONFIG		169	/* R/W */
#define EXT_CSD_BOOT_WP			173	/* R/W */
#define EXT_CSD_ERASE_GROUP_DEF		175	/* R/W */
#define EXT_CSD_BOOT_BUS_WIDTH		177	/* R/W */
#define EXT_CSD_PART_CONFIG		179	/* R/W */
#define EXT_CSD_ERASED_MEM_CONT		181	/* RO */
#define EXT_CSD_BUS_WIDTH		183	/* R/W */
#define EXT_CSD_STROBE_SUPPORT		184	/* RO */
#define EXT_CSD_HS_TIMING		185	/* R/W */
#define EXT_CSD_POWER_CLASS		187	/* R/W */
#define EXT_CSD_REV			192	/* RO */
#define EXT_CSD_STRUCTURE		194	/* RO */
#define EXT_CSD_CARD_TYPE		196	/* RO */
#define EXT_CSD_DRIVER_STRENGTH		197	/* RO */
#define EXT_CSD_OUT_OF_INTERRUPT_TIME	198	/* RO */
#define EXT_CSD_PART_SWITCH_TIME        199     /* RO */
#define EXT_CSD_PWR_CL_52_195		200	/* RO */
#define EXT_CSD_PWR_CL_26_195		201	/* RO */
#define EXT_CSD_PWR_CL_52_360		202	/* RO */
#define EXT_CSD_PWR_CL_26_360		203	/* RO */
#define EXT_CSD_SEC_CNT			212	/* RO, 4 bytes */
#define EXT_CSD_S_A_TIMEOUT		217	/* RO */
#define EXT_CSD_REL_WR_SEC_C		222	/* RO */
#define EXT_CSD_HC_WP_GRP_SIZE		221	/* RO */
#define EXT_CSD_ERASE_TIMEOUT_MULT	223	/* RO */
#define EXT_CSD_HC_ERASE_GRP_SIZE	224	/* RO */
#define EXT_CSD_BOOT_MULT		226	/* RO */
#define EXT_CSD_SEC_TRIM_MULT		229	/* RO */
#define EXT_CSD_SEC_ERASE_MULT		230	/* RO */
#define EXT_CSD_SEC_FEATURE_SUPPORT	231	/* RO */
#define EXT_CSD_TRIM_MULT		232	/* RO */
#define EXT_CSD_PWR_CL_200_195		236	/* RO */
#define EXT_CSD_PWR_CL_200_360		237	/* RO */
#define EXT_CSD_PWR_CL_DDR_52_195	238	/* RO */
#define EXT_CSD_PWR_CL_DDR_52_360	239	/* RO */
#define EXT_CSD_BKOPS_STATUS		246	/* RO */
#define EXT_CSD_POWER_OFF_LONG_TIME	247	/* RO */
#define EXT_CSD_GENERIC_CMD6_TIME	248	/* RO */
#define EXT_CSD_CACHE_SIZE		249	/* RO, 4 bytes */
#define EXT_CSD_PWR_CL_DDR_200_360	253	/* RO */
#define EXT_CSD_FIRMWARE_VERSION	254	/* RO, 8 bytes */
#define EXT_CSD_PRE_EOL_INFO		267	/* RO */
#define EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_A	268	/* RO */
#define EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_B	269	/* RO */
#define EXT_CSD_CMDQ_DEPTH		307	/* RO */
#define EXT_CSD_CMDQ_SUPPORT		308	/* RO */
#define EXT_CSD_SUPPORTED_MODE		493	/* RO */
#define EXT_CSD_TAG_UNIT_SIZE		498	/* RO */
#define EXT_CSD_DATA_TAG_SUPPORT	499	/* RO */
#define EXT_CSD_MAX_PACKED_WRITES	500	/* RO */
#define EXT_CSD_MAX_PACKED_READS	501	/* RO */
#define EXT_CSD_BKOPS_SUPPORT		502	/* RO */
#define EXT_CSD_HPI_FEATURES		503	/* RO */

#define EXT_CSD_PART_CONFIG_BOOT_MASK	(0x38)
#define EXT_CSD_PART_CONFIG_BOOT_PARTITION1	(0x8)
#define EXT_CSD_PART_CONFIG_BOOT_PARTITION2	(0x10)
#define EXT_CSD_PART_CONFIG_BOOT_USER	(0x38)

#define EXT_CSD_PART_CONFIG_ACC_MASK	(0x7)
#define EXT_CSD_PART_CONFIG_ACC_BOOT1	(0x1)
#define EXT_CSD_PART_CONFIG_ACC_BOOT2	(0x2)

#define BUS_HZ					240000000
#define TRANS_DATA_FREQS		24000000

#define SD_BOOT_ADDRESS			0 // 16 sector

#define SDHCI_DEFAULT_BOUNDARY_ARG   (0x7) // 512K

#define EMMC_PHY_VERSION_ID		(REG_EMMC_PHY_CFG_BASE + 0x0)
#define EMMC_PHY_INT_EN			(REG_EMMC_PHY_CFG_BASE + 0x4)
#define EMMC_PHY_INT_SRC		(REG_EMMC_PHY_CFG_BASE + 0x08)
#define EMMC_PHY_CTRL			(REG_EMMC_PHY_CFG_BASE + 0x0c)
#define EMMC_PHY_TEST_O			(REG_EMMC_PHY_CFG_BASE + 0x10)
#define EMMC_PHY_TEST_I			(REG_EMMC_PHY_CFG_BASE + 0x14)
#define EMMC_PHY_IE_CTRL		(REG_EMMC_PHY_CFG_BASE + 0x18)
#define EMMC_PHY_DL_CTRL		(REG_EMMC_PHY_CFG_BASE + 0x1c)
#define EMMC_PHY_CTRL2                  (REG_EMMC_PHY_CFG_BASE + 0x24)


#define 	EMMC_PHY_MODE		BIT(0)
#define 	LOOPBACK_MODE		BIT(1)
#define 	PRBS_GEN_EN			BIT(2)
#define 	PRBS_CHK_EN			BIT(3)
#define 	PRBS_MODE			BIT(4)
#define 	PRBS_CHK_IN_SEL_OFS	6
#define 	PRBS_CHK_IN_SEL_MSK	0xf
#define 	TX_CLK_INV_EN		BIT(9)
#define 	TX_CLK_GATE			BIT(10)
#define 	RX_CLK_SEL_DAT		BIT(11)
#define 	RX_CLK_SEL_CMD		BIT(12)
#define 	RX_CLK_INV_EN_DAT	BIT(13)
#define 	RX_CLK_INV_EN_CMD	BIT(14)
#define 	TX_DL_BYPASS		BIT(15)
#define 	RX_DL_BYPASS_DAT	BIT(16)
#define 	RX_DL_BYPASS_CMD	BIT(17)
#define 	PRBS_GEN_OUT_EN_OFS	18
#define 	PRBS_GEN_OUT_EN_MSK	0x1ff
#define 	FIFO_CLR			BIT(27)
#define 	SAMPLE_EN			BIT(28)
#define 	OUTPUT_BIT_MODE_OFS	29
#define 	OUTPUT_BIT_MODE_MSK	0x3
#define 	DAT_INPUT_EN		BIT(31)

#define 	IE_CTRL_CMD   		BIT(0)
#define 	IE_CTRL_DAT_OFS		1
#define 	IE_CTRL_DAT_MSK		0xff
#define 	CARD_RESET_N		BIT(9)

#define 	DELAY_LINE_CTRL_TX_OFS	0
#define 	DELAY_LINE_CTRL_TX_MSK	0xff
#define 	DELAY_LINE_CTRL_RX_DAT_OFS	8
#define 	DELAY_LINE_CTRL_RX_DAT_MSK	0xff
#define 	DELAY_LINE_CTRL_RX_CMD_OFS	16
#define 	DELAY_LINE_CTRL_RX_CMD_MSK	0xff
#define		DELAY_LINE_TUNING_SEL		BIT(24)

#define 	BITS_EMMC_CLK_STOP_EN                       (0x1 << 3)
#define 	BITS_EMMC_TX_SAMPLE_CLK_SEL                 (0x1 << 0)
#define 	BITS_EMMC_TX_SAMPLE_CLK_1X_GATE		    (1 << 1)
#define 	BITS_EMMC_TX_SAMPLE_CLK_2X_GATE		    (1 << 2)


/*
 * Host SDMA buffer boundary. Valid values from 4K to 512K in powers of 2.
 */
#define SDHCI_DEFAULT_BOUNDARY_SIZE	(512 * 1024)

struct sdhci_adma2_32_desc {
	u16	cmd;
	u16	len;
	u32	addr;
};

struct sdhci_adma2_64_desc {
	 u16 cmd;
	 u16 len;
	 u32 addr_lo;
	 u32 addr_hi;
 };

#endif
