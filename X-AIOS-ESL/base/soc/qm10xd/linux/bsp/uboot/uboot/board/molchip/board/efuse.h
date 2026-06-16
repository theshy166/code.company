#ifndef _EFUSE_H_
#define _EFUSE_H_

#define EFUSE_CMD			0x0
#define EFUSE_CONFIG			0x4
#define EFUSE_MATCH_KEY			0x8
#define EFUSE_TIMING0			0xc
#define EFUSE_TIMING1			0x10
#define EFUSE_TIMING2			0x14
#define EFUSE_TIMING3			0x18
#define EFUSE_TIMING4			0x1c
#define EFUSE_TIMING5			0x20
#define EFUSE_TIMING6			0x24
#define EFUSE_DOUT			0x28
#define EFUSE_STATUS0			0x2c
#define EFUSE_STATUS1			0x30
#define EFUSE_STATUS2			0x34
#define EFUSE_STATUS3			0x38
#define EFUSE_STATUS4			0x3c
#define EFUSE_WRITE_VALUE		0x40
#define EFUSE_STATUS6			0x44
#define EFUSE_STATUS7			0x48
#define EFUSE_STATUS8			0x4c

#define OPCOM_MASK			0xf
#define OPCMD_SHIFT			0
#define ADDR_MASK			0xff
#define ADDR_SHIFT			4
#define BITWRFLAG_MASK			0xff
#define BITWRFLAG_SHIFT			12
#define AES_KEY_MASK			0x7
#define AES_KEY_SHIFT			20

#define AVDD_SW_PRIORITY		BIT(0)
#define ECE				BIT(18)
#define MAP_ENTRY_SHIFT			24
#define MAP_ENTRY_MASK			0xff

#define WFLAG_DETECT_COMPLT		BIT(0)
#define LOAD_USER_CMD_COMPLT		BIT(1)
#define W_COMPLT			BIT(2)
#define R_COMPLT			BIT(3)
#define AES_COMPLT			BIT(4)

#define WFLAG_AUTO_COMPLT		BIT(8)
#define OP_PROCESSING			BIT(14)
#define OP_COMPLT			BIT(15)
#define CUR_CMD_SHFIT			16
#define CUR_CMD_MASK			0xf

#define AES_LOCK_SHIFT			24
#define AES_LOCK_READ0			BIT(24)
#define AES_LOCK_READ1			BIT(25)
#define AES_LOCK_READ2			BIT(26)
#define AES_LOCK_READ3			BIT(27)
#define AES_KEY0_1			24
#define AES_KEY2_3			25
#define AES_KEY4_5			26
#define AES_KEY6_7			27

#define CMD_WFLGA			0
#define CMD_LOAD_USERCMD		1
#define CMD_WRITE			2
#define CMD_READ			3
#define CMD_TRANS_AESKEY		4
#define CMD_WFLAG_AUTO			8
#define CMD_RELEASE_AESKEY		9
#define CMD_COMPARE			0x0f

#define EFUSE_CMD_WAIT_TIME		5000

#define STATUS3_AES_ERR_SHIFT		2
#define STATUS3_CMD_ERR			BIT(3)
#define STATUS3_OP_ERR			BIT(4)
#define STATUS3_RMAX_ERR		BIT(8)

#define EFUSE_ENTRY(x)			(x)
#define EFUSE_BLOCK(x)			(x)

/* bitmap. */
#define HASH_START_BLOCK		EFUSE_BLOCK(40) /*entry 40 -- 47*/
#define	HW_BONDING_OPT			EFUSE_BLOCK(56)
#define	SW_BONDING_OPT			EFUSE_BLOCK(55)
//#define HSIF_CLK_SEL			EFUSE_BLOCK(55)
#define CLASS_CODE_CFG			EFUSE_BLOCK(54)
#define EMMC_RX_TIMEOUT			EFUSE_BLOCK(53)
#define EMMC_RX_CLK_POLARITY_BLOCK	EFUSE_BLOCK(52)
#define HIFREQ_CHECK			EFUSE_BLOCK(52)
#define AES_RD_LOCK			EFUSE_BLOCK(63)
#define WRITE_LOCK_H			EFUSE_BLOCK(62)
#define WRITE_LOCK_L			EFUSE_BLOCK(61)
#define CBC_KEY_BLOCK			EFUSE_BLOCK(32)
#define EMMC_CFG_BLOCK			EFUSE_BLOCK(53)
#define AES_IV_DATA			EFUSE_BLOCK(36)

#define EFUSE_MAX_BLOCK			64

#define HASH_WLOCK_BIT_MASK		0xff00
#define HASH_RLOCK_BIT_MASK		0xc00

#define AES_WLOCK_BIT_MASK		0xf
#define AES_RLOCK_BIT_MASK		0xff0100

#define EFUSE_ENTRY_IN_BLK		4

#define AES_KEY_LEN			128
#define LOW_FREQ			BIT(0)
#define EMMC_RX_CLK_DAT_POLARITY	BIT(1)
#define EMMC_RX_CLK_CMD_POLARITY	BIT(2)

#define OPT_SECURE			BIT(31)
#define LOCK_HASH0			BIT(2)
#define LOCK_HASH1			BIT(2)
#define LOCK_HASH2			BIT(2)
#define LOCK_HASH3			BIT(2)
#define AES_KEY_LOCK			(BIT(4) | BIT(5))

#define PCIE_BONDING_CONTROLLER_0	BIT(11)
#define PCIE_BONDING_CONTROLLER_1	BIT(22)

#define EMMC_RX_TIMEOUT_MASK		(BIT(0) | BIT(1))

#define EMMC_BASE_CLK_SEL_24M			(BIT(8))
#define EMMC_BASE_CLK_SEL_200M			(BIT(9))

#define EMMC_BOOT_MODE_MANDATORY_ONLY			(BIT(16))
#define EMMC_BOOT_MODE_NORMAL_ONLY			(BIT(17))
#define EMMC_BOOT_TRN_PIO_ONLY			(BIT(18))

#define MATCH_KEY			0x92fc0025
#define USER_BLOCK_MAX_NUM		56

#define efuse_read(b, x)		\
	__raw_readl((unsigned long)((b) + (x)))
#define efuse_write(v, b, x)		\
	__raw_writel((v),  (unsigned long)((b) + (x)))

enum{
	TRANS_AESKEY_32 = 3,
	TRANS_AESKEY_64 = 7,
	TRANS_AESKEY_128 = 15,
	TRANS_AESKEY_192 = 23,
	TRANS_AESKEY_256 = 31,
};

void efuse_init(void);
bool efuse_wcmd_auto(u32 base);
bool efuse_apb_read(u32 base, u32 block, u32 *val);
bool efuse_write_cmd(u32 base, u32 block, u32 value);
bool efuse_xfer_aes_key(u32 base, u32 block, u32 len);
bool efuse_compare(u32 *hash);
bool read_efuse_entry(u32 *edata, u32 num, u32 block);

#endif
