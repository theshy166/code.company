#ifndef SDHCI_H
#define SDHCI_H
/*
 * Controller registers
 */
#define SDHCI_DMA_ADDRESS			0x00
#define SDHCI_ARGUMENT2				SDHCI_DMA_ADDRESS
#define SDHCI_32BIT_BLK_CNT			SDHCI_DMA_ADDRESS

#define SDHCI_ARGUMENT2				SDHCI_DMA_ADDRESS

#define SDHCI_BLOCK_SIZE			0x04
#define SDHCI_MAKE_BLKSZ(dma, blksz)		\
	(((dma & 0x7) << 12) | (blksz & 0xFFF))

#define SDHCI_BLOCK_COUNT			0x06

#define SDHCI_ARGUMENT				0x08

#define SDHCI_TRANSFER_MODE			0x0C
#define SDHCI_TRNS_DMA				0x01
#define SDHCI_TRNS_BLK_CNT_EN			0x02
#define SDHCI_TRNS_AUTO_CMD12			0x04
#define SDHCI_TRNS_AUTO_CMD23			0x08
#define SDHCI_TRNS_READ				0x10
#define SDHCI_TRNS_MULTI			0x20

#define SDHCI_COMMAND				0x0E
#define SDHCI_CMD_RESP_MASK			0x03
#define SDHCI_CMD_CRC				0x08
#define SDHCI_CMD_INDEX				0x10
#define SDHCI_CMD_DATA				0x20
#define SDHCI_CMD_ABORTCMD			0xC0

#define SDHCI_CMD_RESP_NONE			0x00
#define SDHCI_CMD_RESP_LONG			0x01
#define SDHCI_CMD_RESP_SHORT			0x02
#define SDHCI_CMD_RESP_SHORT_BUSY		0x03

#define SDHCI_MAKE_CMD(c, f)			\
	(((c & 0xff) << 8) | (f & 0xff))
#define SDHCI_GET_CMD(c)			((c>>8) & 0x3f)

#define SDHCI_RESPONSE				0x10

#define SDHCI_BUFFER				0x20

#define SDHCI_PRESENT_STATE			0x24
#define SDHCI_CMD_INHIBIT			0x00000001
#define SDHCI_DATA_INHIBIT			0x00000002
#define SDHCI_DOING_WRITE			0x00000100
#define SDHCI_DOING_READ			0x00000200
#define SDHCI_SPACE_AVAILABLE			0x00000400
#define SDHCI_DATA_AVAILABLE			0x00000800
#define SDHCI_CARD_PRESENT			0x00010000
#define SDHCI_WRITE_PROTECT			0x00080000
#define SDHCI_DATA_LVL_MASK			0x00F00000
#define SDHCI_DATA_LVL_SHIFT			20
#define SDHCI_DATA_0_LVL_MASK			0x00100000
#define SDHCI_CMD_LVL				0x01000000

#define SDHCI_HOST_CONTROL			0x28
#define SDHCI_CTRL_LED				0x01
#define SDHCI_CTRL_4BITBUS			0x02
#define SDHCI_CTRL_HISPD			0x04
#define SDHCI_CTRL_DMA_MASK			0x18
#define SDHCI_CTRL_SDMA				0x00
#define SDHCI_CTRL_ADMA1			0x08
#define SDHCI_CTRL_ADMA32			0x10
#define SDHCI_CTRL_ADMA64			0x18
#define SDHCI_CTRL_8BITBUS			0x20
#define SDHCI_CTRL_CDTEST_INS			0x40
#define SDHCI_CTRL_CDTEST_EN			0x80

#define SDHCI_POWER_CONTROL			0x29
#define SDHCI_POWER_ON				0x01
#define SDHCI_POWER_180				0x0A
#define SDHCI_POWER_300				0x0C
#define SDHCI_POWER_330				0x0E

#define SDHCI_BLOCK_GAP_CONTROL			0x2A

#define SDHCI_WAKE_UP_CONTROL			0x2B
#define SDHCI_WAKE_ON_INT			0x01
#define SDHCI_WAKE_ON_INSERT			0x02
#define SDHCI_WAKE_ON_REMOVE			0x04

#define SDHCI_CLOCK_CONTROL			0x2C
#define SDHCI_DIVIDER_SHIFT			8
#define SDHCI_DIVIDER_HI_SHIFT			6
#define SDHCI_DIV_MASK				0xFF
#define SDHCI_DIV_MASK_LEN			8
#define SDHCI_DIV_HI_MASK			0x300
#define SDHCI_PROG_CLOCK_MODE			0x0020
#define SDHCI_CLOCK_CARD_EN			0x0004
#define SDHCI_CLOCK_INT_STABLE			0x0002
#define SDHCI_CLOCK_INT_EN			0x0001

#define SDHCI_TIMEOUT_CONTROL			0x2E

#define SDHCI_SOFTWARE_RESET			0x2F
#define SDHCI_RESET_ALL				0x01
#define SDHCI_RESET_CMD				0x02
#define SDHCI_RESET_DATA			0x04

#define SDHCI_INT_STATUS			0x30
#define SDHCI_INT_ENABLE			0x34
#define SDHCI_SIGNAL_ENABLE			0x38
#define SDHCI_INT_RESPONSE			0x00000001
#define SDHCI_INT_DATA_END			0x00000002
#define SDHCI_INT_BLK_GAP			0x00000004
#define SDHCI_INT_DMA_END			0x00000008
#define SDHCI_INT_SPACE_AVAIL			0x00000010
#define SDHCI_INT_DATA_AVAIL			0x00000020
#define SDHCI_INT_CARD_INSERT			0x00000040
#define SDHCI_INT_CARD_REMOVE			0x00000080
#define SDHCI_INT_CARD_INT			0x00000100
#define SDHCI_INT_RETUNE			0x00001000
#define SDHCI_INT_ERROR				0x00008000
#define SDHCI_INT_TIMEOUT			0x00010000
#define SDHCI_INT_CRC				0x00020000
#define SDHCI_INT_END_BIT			0x00040000
#define SDHCI_INT_INDEX				0x00080000
#define SDHCI_INT_DATA_TIMEOUT			0x00100000
#define SDHCI_INT_DATA_CRC			0x00200000
#define SDHCI_INT_DATA_END_BIT			0x00400000
#define SDHCI_INT_BUS_POWER			0x00800000
#define SDHCI_INT_ACMD12ERR			0x01000000
#define SDHCI_INT_ADMA_ERROR			0x02000000

#define SDHCI_INT_NORMAL_MASK			0x00007FFF
#define SDHCI_INT_ERROR_MASK			0xFFFF8000

#define SDHCI_INT_CMD_MASK	(SDHCI_INT_RESPONSE | SDHCI_INT_TIMEOUT | \
		SDHCI_INT_CRC | SDHCI_INT_END_BIT | SDHCI_INT_INDEX)
#define SDHCI_INT_DATA_MASK	(SDHCI_INT_DATA_END | SDHCI_INT_DMA_END | \
		SDHCI_INT_DATA_AVAIL | SDHCI_INT_SPACE_AVAIL | \
		SDHCI_INT_DATA_TIMEOUT | SDHCI_INT_DATA_CRC | \
		SDHCI_INT_DATA_END_BIT | SDHCI_INT_ADMA_ERROR | \
		SDHCI_INT_BLK_GAP)
#define SDHCI_INT_ALL_MASK			((unsigned int)-1)

#define SDHCI_ACMD12_ERR			0x3C

#define SDHCI_HOST_CONTROL2			0x3E
#define SDHCI_CTRL_UHS_MASK			0x0007
#define SDHCI_CTRL_UHS_SDR12			0x0000
#define SDHCI_CTRL_UHS_SDR25			0x0001
#define SDHCI_CTRL_UHS_SDR50			0x0002
#define SDHCI_CTRL_UHS_SDR104			0x0003
#define SDHCI_CTRL_UHS_DDR50			0x0004
#define SDHCI_CTRL_HS400			0x0005 /* Non-standard */
#define SDHCI_CTRL_VDD_180			0x0008
#define SDHCI_CTRL_DRV_TYPE_MASK		0x0030
#define SDHCI_CTRL_DRV_TYPE_B			0x0000
#define SDHCI_CTRL_DRV_TYPE_A			0x0010
#define SDHCI_CTRL_DRV_TYPE_C			0x0020
#define SDHCI_CTRL_DRV_TYPE_D			0x0030
#define SDHCI_CTRL_EXEC_TUNING			0x0040
#define SDHCI_CTRL_TUNED_CLK			0x0080
#define SDHCI_CMD23_ENABLE			0x0800
#define SDHCI_CTRL_V4_MODE			0x1000
#define SDHCI_CTRL_64BIT_ADDR			0x2000
#define SDHCI_CTRL_PRESET_VAL_ENABLE		0x8000

#define SDHCI_CAPABILITIES			0x40
#define SDHCI_TIMEOUT_CLK_MASK			0x0000003F
#define SDHCI_TIMEOUT_CLK_SHIFT			0
#define SDHCI_TIMEOUT_CLK_UNIT			0x00000080
#define SDHCI_CLOCK_BASE_MASK			0x00003F00
#define SDHCI_CLOCK_V3_BASE_MASK		0x0000FF00
#define SDHCI_CLOCK_BASE_SHIFT			8
#define SDHCI_MAX_BLOCK_MASK			0x00030000
#define SDHCI_MAX_BLOCK_SHIFT			16
#define SDHCI_CAN_DO_8BIT			0x00040000
#define SDHCI_CAN_DO_ADMA2			0x00080000
#define SDHCI_CAN_DO_ADMA1			0x00100000
#define SDHCI_CAN_DO_HISPD			0x00200000
#define SDHCI_CAN_DO_SDMA			0x00400000
#define SDHCI_CAN_DO_SUSPEND			0x00800000
#define SDHCI_CAN_VDD_330			0x01000000
#define SDHCI_CAN_VDD_300			0x02000000
#define SDHCI_CAN_VDD_180			0x04000000
#define SDHCI_CAN_64BIT				0x10000000

#define SDHCI_SUPPORT_SDR50			0x00000001
#define SDHCI_SUPPORT_SDR104			0x00000002
#define SDHCI_SUPPORT_DDR50			0x00000004
#define SDHCI_DRIVER_TYPE_A			0x00000010
#define SDHCI_DRIVER_TYPE_C			0x00000020
#define SDHCI_DRIVER_TYPE_D			0x00000040
#define SDHCI_RETUNING_TIMER_COUNT_MASK		0x00000F00
#define SDHCI_RETUNING_TIMER_COUNT_SHIFT	8
#define SDHCI_USE_SDR50_TUNING			0x00002000
#define SDHCI_RETUNING_MODE_MASK		0x0000C000
#define SDHCI_RETUNING_MODE_SHIFT		14
#define SDHCI_CLOCK_MUL_MASK			0x00FF0000
#define SDHCI_CLOCK_MUL_SHIFT			16
#define SDHCI_SUPPORT_HS400			0x80000000 /* Non-standard */

#define SDHCI_CAPABILITIES_1			0x44

#define SDHCI_MAX_CURRENT			0x48
#define SDHCI_MAX_CURRENT_LIMIT			0xFF
#define SDHCI_MAX_CURRENT_330_MASK		0x0000FF
#define SDHCI_MAX_CURRENT_330_SHIFT		0
#define SDHCI_MAX_CURRENT_300_MASK		0x00FF00
#define SDHCI_MAX_CURRENT_300_SHIFT		8
#define SDHCI_MAX_CURRENT_180_MASK		0xFF0000
#define SDHCI_MAX_CURRENT_180_SHIFT		16
#define SDHCI_MAX_CURRENT_MULTIPLIER		4

/* 4C-4F reserved for more max current */

#define SDHCI_SET_ACMD12_ERROR			0x50
#define SDHCI_SET_INT_ERROR			0x52

#define SDHCI_ADMA_ERROR			0x54

/* 55-57 reserved */

#define SDHCI_ADMA_ADDRESS			0x58
#define SDHCI_ADMA_ADDRESS_HI			0x5C

/* 60-FB reserved */

#define SDHCI_PRESET_FOR_SDR12			0x66
#define SDHCI_PRESET_FOR_SDR25			0x68
#define SDHCI_PRESET_FOR_SDR50			0x6A
#define SDHCI_PRESET_FOR_SDR104			0x6C
#define SDHCI_PRESET_FOR_DDR50			0x6E
#define SDHCI_PRESET_FOR_HS400			0x74 /* Non-standard */
#define SDHCI_PRESET_DRV_MASK			0xC000
#define SDHCI_PRESET_DRV_SHIFT			14
#define SDHCI_PRESET_CLKGEN_SEL_MASK		0x400
#define SDHCI_PRESET_CLKGEN_SEL_SHIFT		10
#define SDHCI_PRESET_SDCLK_FREQ_MASK		0x3FF
#define SDHCI_PRESET_SDCLK_FREQ_SHIFT		0

#define SDHCI_SLOT_INT_STATUS			0xFC

#define SDHCI_HOST_VERSION			0xFE
#define SDHCI_VENDOR_VER_MASK			0xFF00
#define SDHCI_VENDOR_VER_SHIFT			8
#define SDHCI_SPEC_VER_MASK			0x00FF
#define SDHCI_SPEC_VER_SHIFT			0
#define SDHCI_SPEC_100				0
#define SDHCI_SPEC_200				1
#define SDHCI_SPEC_300				2

#define EMMC_CTRL_R				0x52c
#define CARD_IS_EMMC				BIT(0)

#define BOOT_CTRL_R				0x52e
#define BOOT_TOUT_CNT_OFS			12
#define BOOT_TOUT_CNT_MSK			0x7
#define	BOOT_ACK_ENABLE				BIT(8)
#define VALIDATE_BOOT				BIT(7)
#define BOOT_EN					BIT(0)


/* generic data direction definitions */
#define READ					0
#define WRITE					1

#define MMC_POWER_OFF				0
#define MMC_POWER_UP				1
#define MMC_POWER_ON				2
#define MMC_POWER_UNDEFINED			3
#define MMC_BUS_WIDTH_1				0
#define MMC_BUS_WIDTH_4				2
#define MMC_BUS_WIDTH_8				3
#define MMC_TIMING_LEGACY			0
#define MMC_TIMING_MMC_HS			1
#define MMC_TIMING_SD_HS			2
#define MMC_TIMING_UHS_SDR12			3
#define MMC_TIMING_UHS_SDR25			4
#define MMC_TIMING_UHS_SDR50			5

/*
*phy registers
*/
#define EMMC_PHY_VERSION_ID			(REG_EMMC_PHY_CFG_BASE + 0x0)
#define EMMC_PHY_INT_EN				(REG_EMMC_PHY_CFG_BASE + 0x4)
#define EMMC_PHY_INT_SRC			(REG_EMMC_PHY_CFG_BASE + 0x08)
#define EMMC_PHY_CTRL				(REG_EMMC_PHY_CFG_BASE + 0x0c)
#define EMMC_PHY_TEST_O				(REG_EMMC_PHY_CFG_BASE + 0x10)
#define EMMC_PHY_TEST_I				(REG_EMMC_PHY_CFG_BASE + 0x14)
#define EMMC_PHY_IE_CTRL			(REG_EMMC_PHY_CFG_BASE + 0x18)
#define EMMC_PHY_DL_CTRL			(REG_EMMC_PHY_CFG_BASE + 0x1c)
#define EMMC_PHY_CTRL2				(REG_EMMC_PHY_CFG_BASE + 0x24)


#define EMMC_PHY_MODE				BIT(0)
#define LOOPBACK_MODE				BIT(1)
#define PRBS_GEN_EN				BIT(2)
#define PRBS_CHK_EN				BIT(3)
#define PRBS_MODE				BIT(4)
#define PRBS_CHK_IN_SEL_OFS			6
#define PRBS_CHK_IN_SEL_MSK			0xf
#define TX_CLK_INV_EN				BIT(9)
#define TX_CLK_GATE				BIT(10)
#define RX_CLK_SEL_DAT				BIT(11)
#define RX_CLK_SEL_CMD				BIT(12)
#define RX_CLK_INV_EN_DAT			BIT(13)
#define RX_CLK_INV_EN_CMD			BIT(14)
#define TX_DL_BYPASS				BIT(15)
#define RX_DL_BYPASS_DAT			BIT(16)
#define RX_DL_BYPASS_CMD			BIT(17)
#define PRBS_GEN_OUT_EN_OFS			18
#define PRBS_GEN_OUT_EN_MSK			0x1ff
#define FIFO_CLR				BIT(27)
#define SAMPLE_EN				BIT(28)
#define OUTPUT_BIT_MODE_OFS			29
#define OUTPUT_BIT_MODE_MSK			0x3
#define DAT_INPUT_EN				BIT(31)

#define IE_CTRL_CMD				BIT(0)
#define IE_CTRL_DAT_OFS				1
#define IE_CTRL_DAT_MSK				0xff
#define CARD_RESET_N				BIT(9)

#define DELAY_LINE_CTRL_TX_OFS			0
#define DELAY_LINE_CTRL_TX_MSK			0xff
#define DELAY_LINE_CTRL_RX_DAT_OFS		8
#define DELAY_LINE_CTRL_RX_DAT_MSK		0xff
#define DELAY_LINE_CTRL_RX_CMD_OFS		16
#define DELAY_LINE_CTRL_RX_CMD_MSK		0xff
#define DELAY_LINE_TUNING_SEL			BIT(24)


#define BITS_EMMC_TX_SAMPLE_CLK_SEL		(0x1 << 0)
#define BITS_EMMC_TX_SAMPLE_CLK_1X_GATE		(0x1 << 1)
#define BITS_EMMC_TX_SAMPLE_CLK_2X_GATE		(0x1 << 2)
#define BITS_EMMC_CLK_STOP_EN			(0x1 << 3)


#define rq_data_dir(rq)				((rq) ? WRITE : READ)
#define MMC_DATA_WRITE				BIT(8)
#define MMC_DATA_READ				BIT(9)

#define BOOT_MODE_NORMAL				0x0
#define BOOT_MODE_MANDATORY_USE_PIO		0x1
#define BOOT_MODE_MANDATORY				0x2
#define BOOT_MODE_MAX					0x3

#define CLK_BASE_24M				24000000
#define CLK_BASE_400M				400000000
#define BUS_COND_50M_W8				50000000
#define BUS_COND_25M_W8				25000000
#define BUS_COND_12M_W8				12000000
#define BUS_COND_6M_W1				6000000
#define BUS_COND_400K_W1			400000

#define TRANS_DATA_IOS				BUS_COND_6M_W1

#define SDHCI_MAX_DIV_SPEC_300			2046

#define EMMC_WAIT_TIMEOUT			3000
#define BAD_ADDR_VAL 0xdeadbeef
#define BAD_ADDR1_VAL 0xabadf00d
#define SMP_SYNC_TIMEOUT	500

/*16 sector*/
#define SD_BOOT_ADDRESS				0
/*512K*/
#define SDHCI_DEFAULT_BOUNDARY_ARG		(0x7)

struct mmc_data {
	unsigned int blksz;	/* data block size */
	unsigned int blocks;	/* number of blocks */
	unsigned int blk_addr;	/* block address */
	unsigned int address;
	unsigned int flags;
};

struct mmc_request {
	struct mmc_command *sbc;	/* SET_BLOCK_COUNT for multiblock */
	struct mmc_command *cmd;
	struct mmc_data	 *data;
	struct mmc_command *stop;
};

struct mmc_command {
	u32 opcode;
	u32 arg;
	u32 resp[4];
	unsigned int flags;	/* expected response type */
	struct mmc_data *data;
};

/*
 * End of controller registers.
 */
static u8 readb(unsigned long addr)
{
	return *(const volatile u8 *) addr;
}

static u16 readw(unsigned long addr)
{
	return *(const volatile u16 *) addr;
}

static u32 readl(unsigned long addr)
{
	return *(const volatile u32 *) addr;
}

static void writeb(u8 b, unsigned long addr)
{
	*(volatile u8 *) addr = b;
}

static void writew(u16 b, unsigned long addr)
{
	*(volatile u16 *) addr = b;
}

static void writel(u32 b, unsigned long addr)
{
	*(volatile u32 *) addr = b;
}

static void sdhci_writel(u32 val, int reg)
{
	writel(val, REG_EMMC_CTRL_CFG_BASE + reg);
}

static void sdhci_writew(u16 val, int reg)
{
	writew(val, REG_EMMC_CTRL_CFG_BASE + reg);
}

static void sdhci_writeb(u8 val, int reg)
{
	writeb(val, REG_EMMC_CTRL_CFG_BASE + reg);
}

static u32 sdhci_readl(int reg)
{
	return readl(REG_EMMC_CTRL_CFG_BASE + reg);
}

static u16 sdhci_readw(int reg)
{
	return readw(REG_EMMC_CTRL_CFG_BASE + reg);
}

static u8 sdhci_readb(int reg)
{
	return readb(REG_EMMC_CTRL_CFG_BASE + reg);
}

void mmc_set_rx_timeout(void);
void mmc_io_driver_init(void);
void sdhci_init(int soft, unsigned int clk);
void sdhci_set_ios(unsigned int clk);
void mmc_host_reset_phy_init(void);
void sdhci_set_block_info(struct mmc_data *data);
void sdhci_set_adma_addr(u32 addr);
int mmc_start_request(struct mmc_command *cmd);
void sdhci_set_boot_ctrl(void);
int sdhci_read_block_pio(u32 *buf, u32 len);
int emmc_blk_mq_issue_rw_rq(u32 req, u32 addr, u32 sector, u32 len);
int mandatory_boot(u32 addr, u32 len, bool use_sdma);
void set_mmc_io_sts(u32 reg, u8 sts);


int emmc_init(void *data);
int emmc_read(unsigned int offset, unsigned char *buf, unsigned int size);
int emmc_write(unsigned int offset, unsigned char *buf, unsigned int size);

#endif


