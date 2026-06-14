#ifndef SD_MMC_H
#define SD_MMC_H

/* bc                          */
#define MMC_GO_IDLE_STATE			0
/* bcr  [31:0] OCR         R3  */
#define MMC_SEND_OP_COND			1
/* bcr                     R2  */
#define MMC_ALL_SEND_CID			2
/* ac   [31:16] RCA        R1  */
#define MMC_SET_RELATIVE_ADDR			3
/* bc   [31:16] RCA            */
#define MMC_SET_DSR				4
/* ac   [31:16] RCA 15:flg R1b */
#define MMC_SLEEP_AWAKE				5
/* ac   [31:0] See below   R1b */
#define MMC_SWITCH				6
/* ac   [31:16] RCA        R1  */
#define MMC_SELECT_CARD				7
/* adtc                    R1  */
#define MMC_SEND_EXT_CSD			8
/* ac   [31:16] RCA        R2  */
#define MMC_SEND_CSD				9
/* ac   [31:16] RCA        R2  */
#define MMC_SEND_CID				10
/* adtc [31:0] dadr        R1  */
#define MMC_READ_DAT_UNTIL_STOP			11
/* ac                      R1b */
#define MMC_STOP_TRANSMISSION			12
/* ac   [31:16] RCA        R1  */
#define MMC_SEND_STATUS				13
/* adtc                    R1  */
#define MMC_BUS_TEST_R				14
/* ac   [31:16] RCA            */
#define MMC_GO_INACTIVE_STATE			15
/* adtc                    R1  */
#define MMC_BUS_TEST_W				19
/* spi                  spi_R3 */
#define MMC_SPI_READ_OCR			58
/* spi  [0:0] flag      spi_R1 */
#define MMC_SPI_CRC_ON_OFF			59

  /* class 2 */

/* ac   [31:0] block len   R1  */
#define MMC_SET_BLOCKLEN			16
/* adtc [31:0] data addr   R1  */
#define MMC_READ_SINGLE_BLOCK			17
/* adtc [31:0] data addr   R1  */
#define MMC_READ_MULTIPLE_BLOCK			18
/* adtc                    R1  */
#define MMC_SEND_TUNING_BLOCK			19
/* adtc R1  */
#define MMC_SEND_TUNING_BLOCK_HS200		21

  /* class 3 */

/* adtc [31:0] data addr   R1  */
#define MMC_WRITE_DAT_UNTIL_STOP		20

  /* class 4 */

/* adtc [31:0] data addr   R1  */
#define MMC_SET_BLOCK_COUNT			23
/* adtc [31:0] data addr   R1  */
#define MMC_WRITE_BLOCK				24
/* adtc                    R1  */
#define MMC_WRITE_MULTIPLE_BLOCK		25
/* adtc                    R1  */
#define MMC_PROGRAM_CID				26
/* adtc                    R1  */
#define MMC_PROGRAM_CSD				27

  /* class 6 */

/* ac   [31:0] data addr   R1b */
#define MMC_SET_WRITE_PROT			28
/* ac   [31:0] data addr   R1b */
#define MMC_CLR_WRITE_PROT			29
/* adtc [31:0] wpdata addr R1  */
#define MMC_SEND_WRITE_PROT			30

  /* class 5 */

/* ac   [31:0] data addr   R1  */
#define MMC_ERASE_GROUP_START			35
/* ac   [31:0] data addr   R1  */
#define MMC_ERASE_GROUP_END			36
/* ac                      R1b */
#define MMC_ERASE				38

  /* class 9 */

/* ac   <Complex>          R4  */
#define MMC_FAST_IO				39
/* bcr                     R5  */
#define MMC_GO_IRQ_STATE			40

  /* class 7 */

/* adtc                    R1b */
#define MMC_LOCK_UNLOCK				42

  /* class 8 */

/* ac   [31:16] RCA        R1  */
#define MMC_APP_CMD				55
/* adtc [0] RD/WR          R1  */
#define MMC_GEN_CMD				56

  /* class 11 */

/* ac   [20:16] task id    R1  */
#define MMC_QUE_TASK_PARAMS			44
/* ac   [31:0] data addr   R1  */
#define MMC_QUE_TASK_ADDR			45
/* adtc [20:16] task id    R1  */
#define MMC_EXECUTE_READ_TASK			46
/* adtc [20:16] task id    R1  */
#define MMC_EXECUTE_WRITE_TASK			47
/* ac   [20:16] task id    R1b */
#define MMC_CMDQ_TASK_MGMT			48


/* SD commands                           type  argument     response */
  /* class 0 */
/* This is basically the same command as for MMC with some quirks. */

/* bcr                     R6  */
#define SD_SEND_RELATIVE_ADDR			3
/* bcr  [11:0] See below   R7  */
#define SD_SEND_IF_COND				8
/* ac                      R1  */
#define SD_SWITCH_VOLTAGE			11

  /* class 10 */

/* adtc [31:0] See below   R1  */
#define SD_SWITCH				6

  /* class 5 */

/* ac   [31:0] data addr   R1  */
#define SD_ERASE_WR_BLK_START			32
/* ac   [31:0] data addr   R1  */
#define SD_ERASE_WR_BLK_END			33

  /* Application commands */

/* ac   [1:0] bus width    R1  */
#define SD_APP_SET_BUS_WIDTH			6
/* adtc                    R1  */
#define SD_APP_SD_STATUS			13
/* adtc                    R1  */
#define SD_APP_SEND_NUM_WR_BLKS			22
/* bcr  [31:0] OCR         R3  */
#define SD_APP_OP_COND				41
/* adtc                    R1  */

#define SD_APP_SEND_SCR				51

#define MMC_RSP_PRESENT				(1 << 0)
/* 136 bit response */
#define MMC_RSP_136				(1 << 1)
/* expect valid crc */
#define MMC_RSP_CRC				(1 << 2)
/* card may send busy */
#define MMC_RSP_BUSY				(1 << 3)
/* response contains opcode */
#define MMC_RSP_OPCODE				(1 << 4)

/* non-SPI command type */
#define MMC_CMD_MASK				(3 << 5)
#define MMC_CMD_AC				(0 << 5)
#define MMC_CMD_ADTC				(1 << 5)
#define MMC_CMD_BC				(2 << 5)
#define MMC_CMD_BCR				(3 << 5)

/*
 * These are the native response types, and correspond to valid bit
 * patterns of the above flags.  One additional valid pattern
 * is all zeros, which means we don't expect a response.
 */
#define MMC_RSP_NONE				(0)
#define MMC_RSP_R1				(MMC_RSP_PRESENT | \
	MMC_RSP_CRC | MMC_RSP_OPCODE)
#define MMC_RSP_R1B				(MMC_RSP_PRESENT | \
	MMC_RSP_CRC | MMC_RSP_OPCODE | MMC_RSP_BUSY)
#define MMC_RSP_R2				(MMC_RSP_PRESENT | \
	MMC_RSP_136 | MMC_RSP_CRC)
#define MMC_RSP_R3				(MMC_RSP_PRESENT)
#define MMC_RSP_R4				(MMC_RSP_PRESENT)
#define MMC_RSP_R5				(MMC_RSP_PRESENT | \
	MMC_RSP_CRC | MMC_RSP_OPCODE)
#define MMC_RSP_R6				(MMC_RSP_PRESENT | \
	MMC_RSP_CRC | MMC_RSP_OPCODE)
#define MMC_RSP_R7				(MMC_RSP_PRESENT | \
	MMC_RSP_CRC | MMC_RSP_OPCODE)

/*
 *MMC status in R1, for native mode (SPI bits are different)
 *Type
 *	e : error bit
 *	s : status bit
 *	r : detected and set for the actual command response
 *	x : detected and set during command execution. the host must poll
 *	the card by sending status command in order to read these bits.
 * Clear condition
 *	a : according to the card state
 *	b : always related to the previous command. Reception of
 *	a valid command will clear it (with a delay of one command)
 *	c : clear by read
 */

#define R1_OUT_OF_RANGE				(1 << 31)	/* er, c */
#define R1_ADDRESS_ERROR			(1 << 30)	/* erx, c */
#define R1_BLOCK_LEN_ERROR			(1 << 29)	/* er, c */
#define R1_ERASE_SEQ_ERROR			(1 << 28)	/* er, c */
#define R1_ERASE_PARAM				(1 << 27)	/* ex, c */
#define R1_WP_VIOLATION				(1 << 26)	/* erx, c */
#define R1_CARD_IS_LOCKED			(1 << 25)	/* sx, a */
#define R1_LOCK_UNLOCK_FAILED			(1 << 24)	/* erx, c */
#define R1_COM_CRC_ERROR			(1 << 23)	/* er, b */
#define R1_ILLEGAL_COMMAND			(1 << 22)	/* er, b */
#define R1_CARD_ECC_FAILED			(1 << 21)	/* ex, c */
#define R1_CC_ERROR				(1 << 20)	/* erx, c */
#define R1_ERROR				(1 << 19)	/* erx, c */
#define R1_UNDERRUN				(1 << 18)	/* ex, c */
#define R1_OVERRUN				(1 << 17)	/* ex, c */

/* erx, c, CID/CSD overwrite */
#define R1_CID_CSD_OVERWRITE			(1 << 16)
#define R1_WP_ERASE_SKIP			(1 << 15)	/* sx, c */
#define R1_CARD_ECC_DISABLED			(1 << 14)	/* sx, a */
#define R1_ERASE_RESET				(1 << 13)	/* sr, c */
#define R1_STATUS(x)				(x & 0xFFFFE000)

/* sx, b (4 bits) */
#define R1_CURRENT_STATE(x)			((x & 0x00001E00) >> 9)
#define R1_READY_FOR_DATA			(1 << 8)	/* sx, a */
#define R1_SWITCH_ERROR				(1 << 7)	/* sx, c */
#define R1_EXCEPTION_EVENT			(1 << 6)	/* sr, a */
#define R1_APP_CMD				(1 << 5)	/* sr, c */
/*
 * OCR bits are mostly in host.h
 */

/* Card Capacity Status */
#define SD_OCR_CCS				(1 << 30)
/* Card Power up status bit */
#define MMC_CARD_BUSY				0x80000000

#define MMC_CMD_RETRIES				3

#define SD_BUS_WIDTH_1				0
#define SD_BUS_WIDTH_4				2

/* Can be used by core to poll after switch to MMC HS mode */
#define MMC_RSP_R1_NO_CRC			\
	(MMC_RSP_PRESENT | MMC_RSP_OPCODE)

#define mmc_resp_type(cmd)			\
	((cmd)->flags & (MMC_RSP_PRESENT | MMC_RSP_136 | \
	MMC_RSP_CRC | MMC_RSP_BUSY | MMC_RSP_OPCODE))

/*
 * These are the command types.
 */
#define mmc_cmd_type(cmd)			((cmd)->flags & MMC_CMD_MASK)

#define SECTOR_SHIFT				9
#define SECTOR_SIZE				(1 << SECTOR_SHIFT)

/* VDD voltage 1.65 - 1.95 */
#define MMC_VDD_165_195				0x00000080
/* VDD voltage 2.0 ~ 2.1 */
#define MMC_VDD_20_21				0x00000100
/* VDD voltage 2.1 ~ 2.2 */
#define MMC_VDD_21_22				0x00000200
/* VDD voltage 2.2 ~ 2.3 */
#define MMC_VDD_22_23				0x00000400
/* VDD voltage 2.3 ~ 2.4 */
#define MMC_VDD_23_24				0x00000800
/* VDD voltage 2.4 ~ 2.5 */
#define MMC_VDD_24_25				0x00001000
/* VDD voltage 2.5 ~ 2.6 */
#define MMC_VDD_25_26				0x00002000
/* VDD voltage 2.6 ~ 2.7 */
#define MMC_VDD_26_27				0x00004000
/* VDD voltage 2.7 ~ 2.8 */
#define MMC_VDD_27_28				0x00008000
/* VDD voltage 2.8 ~ 2.9 */
#define MMC_VDD_28_29				0x00010000
/* VDD voltage 2.9 ~ 3.0 */
#define MMC_VDD_29_30				0x00020000
/* VDD voltage 3.0 ~ 3.1 */
#define MMC_VDD_30_31				0x00040000
/* VDD voltage 3.1 ~ 3.2 */
#define MMC_VDD_31_32				0x00080000
/* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_32_33				0x00100000
/* VDD voltage 3.3 ~ 3.4 */
#define MMC_VDD_33_34				0x00200000
/* VDD voltage 3.4 ~ 3.5 */
#define MMC_VDD_34_35				0x00400000
/* VDD voltage 3.5 ~ 3.6 */
#define MMC_VDD_35_36				0x00800000

/*
 * MMC_SWITCH access modes
 */
/* Change the command set */
#define MMC_SWITCH_MODE_CMD_SET			0x00
/* Set bits which are 1 in value */
#define MMC_SWITCH_MODE_SET_BITS		0x01
/* Clear bits which are 1 in value */
#define MMC_SWITCH_MODE_CLEAR_BITS		0x02
/* Set target to value */
#define MMC_SWITCH_MODE_WRITE_BYTE		0x03

#define EXT_CSD_CMD_SET_NORMAL			(1<<0)
#define EXT_CSD_CMD_SET_SECURE			(1<<1)
#define EXT_CSD_CMD_SET_CPSECURE		(1<<2)

/*
 * EXT_CSD fields
 */

#define EXT_CSD_CMDQ_MODE_EN			15	/* R/W */
#define EXT_CSD_FLUSH_CACHE			32	/* W */
#define EXT_CSD_CACHE_CTRL			33	/* R/W */
#define EXT_CSD_POWER_OFF_NOTIFICATION		34	/* R/W */
#define EXT_CSD_PACKED_FAILURE_INDEX		35	/* RO */
#define EXT_CSD_PACKED_CMD_STATUS		36	/* RO */
#define EXT_CSD_EXP_EVENTS_STATUS		54	/* RO, 2 bytes */
#define EXT_CSD_EXP_EVENTS_CTRL			56	/* R/W, 2 bytes */
#define EXT_CSD_DATA_SECTOR_SIZE		61	/* R */
#define EXT_CSD_GP_SIZE_MULT			143	/* R/W */
#define EXT_CSD_PARTITION_SETTING_COMPLETED	155	/* R/W */
#define EXT_CSD_PARTITION_ATTRIBUTE		156	/* R/W */
#define EXT_CSD_PARTITION_SUPPORT		160	/* RO */
#define EXT_CSD_HPI_MGMT			161	/* R/W */
#define EXT_CSD_RST_N_FUNCTION			162	/* R/W */
#define EXT_CSD_BKOPS_EN			163	/* R/W */
#define EXT_CSD_BKOPS_START			164	/* W */
#define EXT_CSD_SANITIZE_START			165	/* W */
#define EXT_CSD_WR_REL_PARAM			166	/* RO */
#define EXT_CSD_RPMB_MULT			168	/* RO */
#define EXT_CSD_FW_CONFIG			169	/* R/W */
#define EXT_CSD_BOOT_WP				173	/* R/W */
#define EXT_CSD_ERASE_GROUP_DEF			175	/* R/W */
#define EXT_CSD_BOOT_BUS_COND			177	/* R/W */
#define EXT_CSD_PART_CONFIG			179	/* R/W */
#define EXT_CSD_ERASED_MEM_CONT			181	/* RO */
#define EXT_CSD_BUS_WIDTH			183	/* R/W */
#define EXT_CSD_STROBE_SUPPORT			184	/* RO */
#define EXT_CSD_HS_TIMING			185	/* R/W */
#define EXT_CSD_POWER_CLASS			187	/* R/W */
#define EXT_CSD_REV				192	/* RO */
#define EXT_CSD_STRUCTURE			194	/* RO */
#define EXT_CSD_CARD_TYPE			196	/* RO */
#define EXT_CSD_DRIVER_STRENGTH			197	/* RO */
#define EXT_CSD_OUT_OF_INTERRUPT_TIME		198	/* RO */
#define EXT_CSD_PART_SWITCH_TIME		199	/* RO */
#define EXT_CSD_PWR_CL_52_195			200	/* RO */
#define EXT_CSD_PWR_CL_26_195			201	/* RO */
#define EXT_CSD_PWR_CL_52_360			202	/* RO */
#define EXT_CSD_PWR_CL_26_360			203	/* RO */
#define EXT_CSD_SEC_CNT				212	/* RO, 4 bytes */
#define EXT_CSD_S_A_TIMEOUT			217	/* RO */
#define EXT_CSD_REL_WR_SEC_C			222	/* RO */
#define EXT_CSD_HC_WP_GRP_SIZE			221	/* RO */
#define EXT_CSD_ERASE_TIMEOUT_MULT		223	/* RO */
#define EXT_CSD_HC_ERASE_GRP_SIZE		224	/* RO */
#define EXT_CSD_BOOT_MULT			226	/* RO */
#define EXT_CSD_SEC_TRIM_MULT			229	/* RO */
#define EXT_CSD_SEC_ERASE_MULT			230	/* RO */
#define EXT_CSD_SEC_FEATURE_SUPPORT		231	/* RO */
#define EXT_CSD_TRIM_MULT			232	/* RO */
#define EXT_CSD_PWR_CL_200_195			236	/* RO */
#define EXT_CSD_PWR_CL_200_360			237	/* RO */
#define EXT_CSD_PWR_CL_DDR_52_195		238	/* RO */
#define EXT_CSD_PWR_CL_DDR_52_360		239	/* RO */
#define EXT_CSD_BKOPS_STATUS			246	/* RO */
#define EXT_CSD_POWER_OFF_LONG_TIME		247	/* RO */
#define EXT_CSD_GENERIC_CMD6_TIME		248	/* RO */
#define EXT_CSD_CACHE_SIZE			249	/* RO, 4 bytes */
#define EXT_CSD_PWR_CL_DDR_200_360		253	/* RO */
#define EXT_CSD_FIRMWARE_VERSION		254	/* RO, 8 bytes */
#define EXT_CSD_PRE_EOL_INFO			267	/* RO */
#define EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_A	268	/* RO */
#define EXT_CSD_DEVICE_LIFE_TIME_EST_TYP_B	269	/* RO */
#define EXT_CSD_CMDQ_DEPTH			307	/* RO */
#define EXT_CSD_CMDQ_SUPPORT			308	/* RO */
#define EXT_CSD_SUPPORTED_MODE			493	/* RO */
#define EXT_CSD_TAG_UNIT_SIZE			498	/* RO */
#define EXT_CSD_DATA_TAG_SUPPORT		499	/* RO */
#define EXT_CSD_MAX_PACKED_WRITES		500	/* RO */
#define EXT_CSD_MAX_PACKED_READS		501	/* RO */
#define EXT_CSD_BKOPS_SUPPORT			502	/* RO */
#define EXT_CSD_HPI_FEATURES			503	/* RO */

#define EXT_CSD_PART_CONFIG_BOOT_MASK		(0x38)
#define EXT_CSD_PART_CONFIG_BOOT_PARTITION1	(0x8)
#define EXT_CSD_PART_CONFIG_BOOT_PARTITION2	(0x10)
#define EXT_CSD_PART_CONFIG_BOOT_USER		(0x38)

#define EXT_CSD_PART_CONFIG_ACC_MASK		(0x7)
#define EXT_CSD_PART_CONFIG_ACC_BOOT1		(0x1)
#define EXT_CSD_PART_CONFIG_ACC_BOOT2		(0x2)

/*card*/
int mmc_go_pre_idle(void);
extern int (*init_emmc)(u32 *rca, u32 *csd);

#endif
