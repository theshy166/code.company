#include <common.h>
#include <command.h>
#include <console.h>
#include <div64.h>
//#include <mmc.h>
//#include <asm/io.h>
#include <linux/list.h>
#include <linux/sizes.h>
#include <linux/compiler.h>
#include <part.h>
#include <dm.h>
#include <malloc.h>

#include "./sd_mmc.h"
//#include "../drivers/mmc/mc_sdhci_ids.h"

#define __raw_readb(addr) \
		(*(volatile unsigned char *) ((unsigned int)addr))

#define __raw_writeb(b, addr)  ((*(volatile unsigned char *) ((unsigned int)addr)) = (unsigned int)(b))

#define __raw_readw(addr) \
		(*(volatile unsigned short *) ((unsigned int)addr))

#define __raw_writew(b, addr)  ((*(volatile unsigned short *) ((unsigned int)addr)) = (unsigned int)(b))

#define __raw_readl(addr) \
		(*(volatile unsigned int *) ((unsigned int)addr))
#define __raw_writel(b, addr) ((*(volatile unsigned int *) ((unsigned int)addr)) = (unsigned int)(b))

#define raw_writel(v,a)		__raw_writel(v, a)
#define raw_writew(v,a)		__raw_writew(v, a)
#define raw_writeb(v,a)		__raw_writeb(v, a)
#define raw_readl(a)		__raw_readl(a)
#define raw_readw(a)		__raw_readw(a)
#define raw_readb(a)		__raw_readb(a)

#define writel(v,a)		__raw_writel(v, a)
#define writew(v,a)		__raw_writew(v, a)
#define writeb(v,a)		__raw_writeb(v, a)
#define readl(a)		__raw_readl(a)
#define readw(a)		__raw_readw(a)
#define readb(a)		__raw_readb(a)

/**************************EMMC PHY INIT ********************************************/

#define BIT_EMMC_DAT_INPUT_EN                                     (0x1 << 31)
#define BIT_EMMC_OUTPUT_BIT_MODE(x)                               (((x) & 0x3) << 29)
#define BIT_EMMC_SAMPLE_EN                                        (0x1 << 28)
#define BIT_EMMC_FIFO_CLR                                         (0x1 << 27)
#define BIT_EMMC_PRBS_GEN_OUT_EN(x)                               (((x) & 0x1FF) << 18)
#define BIT_EMMC_RX_DL_BYPASS_CMD                                 (0x1 << 17)
#define BIT_EMMC_RX_DL_BYPASS_DAT                                 (0x1 << 16)
#define BIT_EMMC_TX_DL_BYPASS                                     (0x1 << 15)
#define BIT_EMMC_RX_CLK_INV_EN_CMD                                (0x1 << 14)
#define BIT_EMMC_RX_CLK_INV_EN_DAT                                (0x1 << 13)
#define BIT_EMMC_RX_CLK_SEL_CMD                                   (0x1 << 12)
#define BIT_EMMC_RX_CLK_SEL_DAT                                   (0x1 << 11)
#define BIT_EMMC_TX_CLK_GATE                                      (0x1 << 10)
#define BIT_EMMC_TX_CLK_INV_EN                                    (0x1 << 9)
#define BITS_EMMC_PRBS_CHK_IN_SEL(X)                              (((x) & 0xF) << 5)
#define BIT_EMMC_PRBS_MODE                                        (0x1 << 4)
#define BIT_EMMC_PRBS_CHK_EN                                      (0x1 << 3)
#define BIT_EMMC_PRBS_GEN_EN                                      (0x1 << 2)
#define BIT_EMMC_LOOPBACK_MODE                                    (0x1 << 1)
#define BIT_EMMC_PHY_MODE                                         (0x1 << 0)

#define BIT_EMMC_CART_RESET_N                                     (0x1 << 9)
#define BITS_EMMC_IE_CTRL_DAT(X)                                  (((x) & 0xFF) << 1)
#define BIT_EMMC_IE_CTRL_CMD                                      (0x1 << 0)

#define BIT_EMMC_TEST_CMD_OE                                      (0x1 << 16)
#define BITS_EMMC_TEST_DAT_OE(x)                                  (((x) & 0xFF) << 17)

#define BIT_EMMC_TEST_CMD_O                                       (0x1 << 0)
#define BITS_EMMC_TEST_DAT_O(x)                                   (((x) & 0xFF) << 1)

#define BITS_EMMC_CLK_STOP_EN                                     (0x1 << 3)
#define BITS_EMMC_TX_SAMPLE_CLK_SEL                               (0x1 << 0)
#define BITS_EMMC_TX_SAMPLE_CLK_1X_GATE			          (1 << 1)
#define BITS_EMMC_TX_SAMPLE_CLK_2X_GATE			          (1 << 2)

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

#define REG_HSIF_SYS_AHB_REG_BASE		(0x1c000194)
#define REG_EMMC_CLK_RST_CTRL			(REG_HSIF_SYS_AHB_REG_BASE)
#define REG_EMMC_CLK_RST_CTRL1			(REG_HSIF_SYS_AHB_REG_BASE + 4)

#define BIT_EMMC_HOST_SOFT_RST			(0x1 << 29)
#define BIT_EMMC_PHY_SOFT_RST			(0x1 << 28)
#define BIT_CKG_EMMC_PHYCFG_APB_EN		(0x1 << 27)
#define BIT_CKG_AHB_EMMC_EN				(0x1 << 26)
#define BIT_CKG_AXI_EMMC_EN				(0x1 << 25)
#define BIT_CKG_EMMC_2X_EN				(0x1 << 24)
#define BIT_CKG_EMMC_EN					(0x1 << 23)
#define BIT_CKG_EMMC_BASE_EN			(0x1 << 22)
#define BIT_CKG_EMMC_CQETM_EN			(0x1 << 21)
#define BIT_CKG_EMMC_TM_EN				(0x1 << 20)

#define BITS_CKG_EMMC_BASE_SEL(x)		(((x) & 0x3) << 18)
#define BITS_CKG_EMMC_BASE_MSK			(0x3 << 18)
#define BITS_CKG_EMMC_DIV(x)			(((x) & 0x7F) << 11)
#define BITS_CKG_EMMC_DIV_MSK			(0x7F << 11)
#define BITS_CKG_EMMC_2X_DIV(x)			(((x) & 0x7F) << 4)
#define BITS_CKG_EMMC_2X_DIV_MSK			(0x7F << 4)
#define BITS_CKG_EMMC_2X_SEL(x)			(((x) & 0x3) << 2)
#define BITS_CKG_EMMC_2X_SEL_MSK			(0x3 << 2)

#define BITS_CKG_EMMC_TM_DIV(x)				(((x) & 0x1F) << 9)
#define BITS_CKG_EMMC_TM_DIV_MSK			(0x1F << 9)

#define BITS_CKG_EMMC_TM_SEL(x)				(((x) & 0x3) << 7)
#define BITS_CKG_EMMC_TM_SEL_MSK			(0x3 << 7)


#define REG_EMMC_PHY_CFG_BASE			0x1ca00000
#define REG_EMMC_CTRL_CFG_BASE			0x1d200000

#define 	REG_SYS_TIMER_BASE				 (0x18a00000)
#define 	REG_SYS_TIMER_VALUE				 (REG_SYS_TIMER_BASE + 0x4)
#define 	get_sys_timer_val()				 __raw_readl(REG_SYS_TIMER_VALUE)

#define MMC_STATUS_MASK		(~0x0206BF7F)
#define MMC_STATUS_SWITCH_ERROR	(1 << 7)
#define MMC_STATUS_RDY_FOR_DATA (1 << 8)
#define MMC_STATUS_CURR_STATE	(0xf << 9)
#define MMC_STATUS_ERROR	(1 << 19)

#define MMC_STATE_PRG		(7 << 9)


#define EMMC_DLL_TX_NUM 0x100
#define EMMC_DLL_RX_NUM	0x100
#define EMMC_DLL_TOTAL_SIZE (0x100 * 0x100)
#define HS400_MODE	0

#define EXT_CSD_HS_TIMING		185
#define EXT_CSD_BUS_WIDTH		183

#define EXT_CSD_HI_SPD 0x1
#define EXT_CSD_SPD_HS200 0x2
#define EXT_CSD_SPD_HS400 0x3

#define EXT_CSD_BUS_WIDTH_8_DUAL_DS	0x86
#define EXT_CSD_BUS_WIDTH_8_DUAL	0x6
#define EXT_CSD_BUS_WIDTH_4_DUAL	0x5
#define EXT_CSD_BUS_WIDTH_8			0x2
#define EXT_CSD_BUS_WIDTH_4			0x1
#define EXT_CSD_BUS_WIDTH_1			0x0

#define EXT_CSD_DRIVER_TYPE_1		(1 << 4)

#define TEST_DLL_CLK		200000000
#define EMMC_LOWEST_FQ		400000
#define EMMC_MAX_BLOCK_LEN	512
#define EMMC_DLL_MAX_VAL	256

//#define TEST_USE_CMD_8
#define TEST_USE_RW

typedef struct {
	u8 width;
	u8 timing;
}TEST_EMMC;

#define EXT_CSD_ADDR		0x48000000

static TEST_EMMC extw[] = {
	{EXT_CSD_BUS_WIDTH_8_DUAL_DS, EXT_CSD_SPD_HS400},
	{EXT_CSD_BUS_WIDTH_8, EXT_CSD_SPD_HS200},
};

static u32 get_sys_ticks(void)
{
	u32 ret;
	ret = readl(REG_SYS_TIMER_VALUE);

	return ret;

}

static void delayms(uint ms)
{
	u32 v;
	u32 v1,v2;
	v1 = get_sys_ticks();
	do
	{
		v2 = get_sys_ticks();
		v = v2 - v1;
	}while(v < ms);

}

static inline void sdhci_writel(u32 val, int reg)
{
	writel(val, REG_EMMC_CTRL_CFG_BASE + reg);
}

static inline void sdhci_writew(u16 val, int reg)
{
	writew(val, REG_EMMC_CTRL_CFG_BASE + reg);
}

static inline void sdhci_writeb(u8 val, int reg)
{
	writeb(val, REG_EMMC_CTRL_CFG_BASE + reg);
}

static inline u32 sdhci_readl(int reg)
{
	return readl(REG_EMMC_CTRL_CFG_BASE + reg);
}

static inline u16 sdhci_readw(int reg)
{
	return readw(REG_EMMC_CTRL_CFG_BASE + reg);
}

static inline u8 sdhci_readb(int reg)
{
	return readb(REG_EMMC_CTRL_CFG_BASE + reg);

}
static inline void sdhci_phy_writel(u32 val, int reg)
{
	writel(val, reg);
}

static inline u32 sdhci_phy_readl(int reg)
{
	return readl(reg);
}

static void mmc_phy_init(void)
{
	u32 val;

	val = raw_readl(EMMC_PHY_CTRL);
	val &= ~(EMMC_PHY_MODE |  RX_CLK_SEL_DAT | RX_CLK_SEL_CMD);
	val |= TX_CLK_GATE | RX_DL_BYPASS_DAT | RX_DL_BYPASS_CMD | TX_DL_BYPASS;
	val |= TX_CLK_INV_EN | RX_CLK_INV_EN_DAT | RX_CLK_INV_EN_CMD;
	raw_writel(val, EMMC_PHY_CTRL);

	val |= FIFO_CLR;
	raw_writel(val, EMMC_PHY_CTRL);
	val &= ~FIFO_CLR;
	raw_writel(val, EMMC_PHY_CTRL);

	val = raw_readl(EMMC_PHY_IE_CTRL);
	val &= ~CARD_RESET_N;
	raw_writel(val, EMMC_PHY_IE_CTRL);

	val |= CARD_RESET_N;
	raw_writel(val, EMMC_PHY_IE_CTRL);

	val = raw_readl(EMMC_PHY_CTRL);
	val |= SAMPLE_EN | DAT_INPUT_EN | TX_CLK_INV_EN;
	val &= ~(OUTPUT_BIT_MODE_MSK << OUTPUT_BIT_MODE_OFS);
	val |= 3 << OUTPUT_BIT_MODE_OFS;
	raw_writel(val, EMMC_PHY_CTRL);

	/*config phy is sdr mode*/
	val = raw_readl(EMMC_PHY_CTRL2);
	val |= BITS_EMMC_CLK_STOP_EN;
	val &= ~(BITS_EMMC_TX_SAMPLE_CLK_SEL | BITS_EMMC_TX_SAMPLE_CLK_2X_GATE);
	val |= BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
	raw_writel(val, EMMC_PHY_CTRL2);

}

static u16 sdhci_calc_clk(unsigned int clock,
		   unsigned int *actual_clock)
{
	int div = 0; /* Initialized for compiler warning */
	int real_div = div, clk_mul = 1;
	u16 clk = 0;

	/* Version 3.00 divisors must be a multiple of 2. */
	if (SDHCI_CLK_BASE <= clock)
		div = 1;
	else {
		for (div = 2; div < SDHCI_MAX_DIV_SPEC_300;
		     div += 2) {
			if ((SDHCI_CLK_BASE / div) <= clock)
				break;
		}
	}
	real_div = div;
	div >>= 1;

	if (real_div)
		*actual_clock = (SDHCI_CLK_BASE * clk_mul) / real_div;
	clk |= (div & SDHCI_DIV_MASK) << SDHCI_DIVIDER_SHIFT;
	clk |= ((div & SDHCI_DIV_HI_MASK) >> SDHCI_DIV_MASK_LEN)
		<< SDHCI_DIVIDER_HI_SHIFT;

	return clk;
}

static unsigned int clk_emmc_2x[4] = {24000000, 200000000, 297000000, 400000000};
static void emmc_clk_div(unsigned int clock)
{
	u32 div, val;
	u32 clk_base;
	u32 index;

	if (clock < 24000000)
		index = 0;
	else
		index = 3;

	clk_base = clk_emmc_2x[index];
	div = clk_base /clock;

	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val &= ~(BIT_CKG_EMMC_EN | BIT_CKG_EMMC_2X_EN);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

	delayms(1);
	if (index == 0) {
		val &= ~(BITS_CKG_EMMC_BASE_MSK);
	} else {
		val &= ~(BITS_CKG_EMMC_BASE_MSK);
		val |= BITS_CKG_EMMC_BASE_SEL(1);
	}
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val &= ~(BITS_CKG_EMMC_DIV_MSK);
	val |= BITS_CKG_EMMC_DIV(div-1);

	val &= ~(BITS_CKG_EMMC_2X_SEL_MSK);
	val |= BITS_CKG_EMMC_2X_SEL(index);

	val &= ~(BITS_CKG_EMMC_2X_DIV_MSK);
	if (div == 1)
		val |= BITS_CKG_EMMC_2X_DIV(0);
	else
		val |= BITS_CKG_EMMC_2X_DIV(div/2 - 1);

	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

	delayms(1);

	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val |= (BIT_CKG_EMMC_EN | BIT_CKG_EMMC_2X_EN);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

}

static int sdhci_enable_clk(u16 clk)
{
	unsigned int t1, t2;
	clk |= SDHCI_CLOCK_INT_EN;
	sdhci_writew(clk, SDHCI_CLOCK_CONTROL);

	/* Wait max 150 ms */
	t1 = get_sys_timer_val();
	while (1) {
	   t2 = get_sys_timer_val();

	   clk = sdhci_readw(SDHCI_CLOCK_CONTROL);
	   if (clk & SDHCI_CLOCK_INT_STABLE)
		   break;
	   if (t2 - t1 > EMMC_WAIT_TIMEOUT) {
		   return false;
	   }
	   delayms(1);
	}

	clk |= SDHCI_CLOCK_CARD_EN;
	sdhci_writew(clk, SDHCI_CLOCK_CONTROL);

	return 0;
}

static void sdhci_set_clock(unsigned int clock)
{
	u16 clk;
	unsigned int actual_clk;

	sdhci_writew(0, SDHCI_CLOCK_CONTROL);

	if (clock == 0)
		return;

	clk = sdhci_calc_clk(clock, &actual_clk);
	emmc_clk_div(clock);
	sdhci_enable_clk(clk);
}

static void sdhci_set_bus_width(int width)
{
	u8 ctrl;

	ctrl = sdhci_readb(SDHCI_HOST_CONTROL);
	if (width == MMC_BUS_WIDTH_8) {
		ctrl &= ~SDHCI_CTRL_4BITBUS;
		ctrl |= SDHCI_CTRL_8BITBUS;
	} else {
		if (width == MMC_BUS_WIDTH_4)
			ctrl |= SDHCI_CTRL_4BITBUS;
		else
			ctrl &= ~SDHCI_CTRL_4BITBUS;
	}
	sdhci_writeb(ctrl, SDHCI_HOST_CONTROL);
}

void sdhci_set_ios(unsigned int clk)
{
	u8 ctrl;

	sdhci_set_clock(clk);
	sdhci_set_bus_width(0);

	ctrl = sdhci_readb(SDHCI_HOST_CONTROL);
	ctrl &= ~SDHCI_CTRL_HISPD;
	sdhci_writeb(ctrl, SDHCI_HOST_CONTROL);

}


static inline bool mmc_op_multi(u32 opcode)
{
	return opcode == MMC_WRITE_MULTIPLE_BLOCK ||
	       opcode == MMC_READ_MULTIPLE_BLOCK;
}

static void sdhci_set_transfer_mode(struct mmc_command *cmd)
{
	u16 mode = 0;
	struct mmc_data *data = cmd->data;

	if (data == NULL) {
		/* clear Auto CMD settings for no data CMDs */
			mode = sdhci_readw(SDHCI_TRANSFER_MODE);
			sdhci_writew(mode & ~(SDHCI_TRNS_AUTO_CMD12 |
				SDHCI_TRNS_AUTO_CMD23), SDHCI_TRANSFER_MODE);
		return;
	}

	mode = SDHCI_TRNS_BLK_CNT_EN;

	if (mmc_op_multi(cmd->opcode) || data->blocks > 1) {
		mode = SDHCI_TRNS_BLK_CNT_EN | SDHCI_TRNS_MULTI;
	}

	if (data->flags & MMC_DATA_READ)
		mode |= SDHCI_TRNS_READ;
	mode |= SDHCI_TRNS_DMA;
	if (cmd->opcode == MMC_GO_IDLE_STATE)
		mode &= ~(SDHCI_TRNS_AUTO_CMD12 | SDHCI_TRNS_AUTO_CMD23);

	sdhci_writew(mode, SDHCI_TRANSFER_MODE);
}

static void sdhci_read_rsp_136(struct mmc_command *cmd)
{
	int i, reg;

	for (i = 0; i < 4; i++) {
		reg = SDHCI_RESPONSE + (3 - i) * 4;
		cmd->resp[i] = sdhci_readl(reg);
	}

	/* CRC is stripped so we need to do some shifting */
	for (i = 0; i < 4; i++) {
		cmd->resp[i] <<= 8;
		if (i != 3)
			cmd->resp[i] |= cmd->resp[i + 1] >> 24;
	}
}

static void sdhci_finish_command(struct mmc_command *cmd)
{

	if (cmd->flags & MMC_RSP_PRESENT) {
		if (cmd->flags & MMC_RSP_136) {
			sdhci_read_rsp_136(cmd);
		} else {
			cmd->resp[0] = sdhci_readl(SDHCI_RESPONSE);
		}
	}

}

static int sdhci_reset(u8 mask)
{
	unsigned int t1, t2;

	sdhci_writeb(mask, SDHCI_SOFTWARE_RESET);

	/* Wait max 100 ms */
	t1 = get_sys_timer_val();

	/* hw clears the bit when it's done */
	while (1) {
		t2 = get_sys_timer_val();

		if (!(sdhci_readb(SDHCI_SOFTWARE_RESET) & mask))
			break;

		if (t2 - t1 > EMMC_WAIT_TIMEOUT) {
			return 1;
		}
	}

	return 0;
}

static int sdhci_send_command(struct mmc_command *cmd)
{
	int flags;
	u32 val;
	u32 t1, t2;

	sdhci_writel(cmd->arg, SDHCI_ARGUMENT);
	sdhci_set_transfer_mode(cmd);

	if (!(cmd->flags & MMC_RSP_PRESENT))
		flags = SDHCI_CMD_RESP_NONE;
	else if (cmd->flags & MMC_RSP_136)
		flags = SDHCI_CMD_RESP_LONG;
	else if (cmd->flags & MMC_RSP_BUSY)
		flags = SDHCI_CMD_RESP_SHORT_BUSY;
	else
		flags = SDHCI_CMD_RESP_SHORT;

	if (cmd->flags & MMC_RSP_CRC)
		flags |= SDHCI_CMD_CRC;
	if (cmd->flags & MMC_RSP_OPCODE)
		flags |= SDHCI_CMD_INDEX;

	/* CMD19 is special in that the Data Present Select should be set */
	if (cmd->data || cmd->opcode == MMC_SEND_TUNING_BLOCK ||
	    cmd->opcode == MMC_SEND_TUNING_BLOCK_HS200)
		flags |= SDHCI_CMD_DATA;

	sdhci_writew(SDHCI_MAKE_CMD(cmd->opcode, flags), SDHCI_COMMAND);

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		val = sdhci_readl(SDHCI_INT_STATUS);
		if(val & SDHCI_INT_ERROR){
			sdhci_writel(val, SDHCI_INT_STATUS);
			sdhci_reset(SDHCI_RESET_CMD | SDHCI_RESET_DATA);
			return 1;
		}
		if (val & SDHCI_INT_RESPONSE) {
			sdhci_finish_command(cmd);
			if (flags & SDHCI_CMD_DATA) {
				if(val & SDHCI_INT_DATA_END) {
					sdhci_writel(val, SDHCI_INT_STATUS);
					return 0;
				}
				else {
					if(val & SDHCI_INT_DMA_END){
						int start_addr;
						start_addr = sdhci_readl(SDHCI_ADMA_ADDRESS);
						sdhci_writel(SDHCI_INT_DMA_END, SDHCI_INT_STATUS);
						sdhci_writel(start_addr, SDHCI_ADMA_ADDRESS);
					}
				}
			} else if (cmd->flags & MMC_RSP_BUSY) {
				if(val & SDHCI_INT_DATA_END) {
					sdhci_writel(val, SDHCI_INT_STATUS);
					return 0;
				}
			}else {
				sdhci_writel(val, SDHCI_INT_STATUS);
				return 0;
			}
		}
		if (t2 - t1 > EMMC_WAIT_TIMEOUT)
			break;
	}while(1);

	return 2;

}

static int sdhci_card_busy(void)
{
	u32 present_state;

	/* Check whether DAT[0] is 0 */
	present_state = sdhci_readl(SDHCI_PRESENT_STATE);

	return !(present_state & SDHCI_DATA_0_LVL_MASK);
}

static int mmc_start_request(struct mmc_command *cmd)
{
	int ret;
	int val;

	memset(cmd->resp, 0, sizeof(cmd->resp));

	while (sdhci_card_busy())
		delayms(1);
	val = sdhci_readl(SDHCI_INT_STATUS);
	sdhci_writel(val, SDHCI_INT_STATUS);
	ret = sdhci_send_command(cmd);

	return ret;

}
#if 1
static int mmc_switch(u8 set, u8 index, u8 value)
{
		struct mmc_command cmd = {0};

		cmd.opcode = MMC_SWITCH;
		cmd.arg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
			  (index << 16) |
			  (value << 8) |
			  set;
		cmd.flags = MMC_CMD_AC;
		cmd.flags |= MMC_RSP_R1B;

		return mmc_start_request(&cmd);
}
#endif
static void sdhci_config_dma(int dma)
{
	u8 ctrl;

	ctrl = sdhci_readb(SDHCI_HOST_CONTROL);

	/*
	 * Always adjust the DMA selection as some controllers
	 * (e.g. JMicron) can't do PIO properly when the selection
	 * is ADMA.
	 */
	ctrl &= ~SDHCI_CTRL_DMA_MASK;

	/* Note if DMA Select is zero then SDMA is selected */
	ctrl |= dma;

	sdhci_writeb(ctrl, SDHCI_HOST_CONTROL);
}

static void sdhci_set_block_info(struct mmc_data *data)
{
	/* Set the DMA boundary value and block size */
	sdhci_writew(SDHCI_MAKE_BLKSZ(SDHCI_DEFAULT_BOUNDARY_ARG, data->blksz),
		     SDHCI_BLOCK_SIZE);

	if (sdhci_readw(SDHCI_BLOCK_COUNT))
		sdhci_writew(0, SDHCI_BLOCK_COUNT);
	sdhci_writew(data->blocks, SDHCI_32BIT_BLK_CNT);
}

static void sdhci_set_adma_addr(u32 addr)
{
	sdhci_writel(addr, SDHCI_ADMA_ADDRESS);
}
#if defined(TEST_USE_RW)
static int mmc_blk_mq_issue_rw_rq(u32 req, u32 addr, u32 sector, u32 len)
{
	int err = 0;
	u32 readcmd, writecmd;
	struct mmc_command stop = {0};
	struct mmc_command cmd = {0};
	struct mmc_data data = {0};

	data.blksz = 512;
	data.blocks = len >> SECTOR_SHIFT;
	data.blk_addr = sector;
	data.address = addr;
	sdhci_config_dma(SDHCI_CTRL_SDMA);

	sdhci_set_block_info(&data);
	sdhci_set_adma_addr(addr);
	cmd.arg = sector;
	cmd.data = &data;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;

	if (data.blocks > 1) {
		stop.opcode = MMC_STOP_TRANSMISSION;
		readcmd = MMC_READ_MULTIPLE_BLOCK;
		writecmd = MMC_WRITE_MULTIPLE_BLOCK;
	} else {
		readcmd = MMC_READ_SINGLE_BLOCK;
		writecmd = MMC_WRITE_BLOCK;
	}

	if (rq_data_dir(req) == READ) {
		data.flags = MMC_DATA_READ;
		cmd.opcode = readcmd;
		stop.flags = MMC_RSP_R1 | MMC_CMD_AC;
	} else {
		data.flags = MMC_DATA_WRITE;
		cmd.opcode = writecmd;
		stop.flags = MMC_RSP_R1B | MMC_CMD_AC;
	}
	
	err = mmc_start_request(&cmd);
	if (err == 1)
		return 1;

	if (stop.opcode)
		err =  mmc_start_request(&stop);

	return err;
}
#endif
static int mmc_go_idle(void)
{
	int err;
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_GO_IDLE_STATE;
	cmd.arg = 0;
	cmd.flags = MMC_RSP_NONE | MMC_CMD_BC;

	err = mmc_start_request(&cmd);

	delayms(1);

	return err;
}

static int mmc_send_op_cond(u32 ocr, u32 *rocr)
{
	struct mmc_command cmd = {};
	int i, err = 0;

	cmd.opcode = MMC_SEND_OP_COND;
	cmd.arg = ocr;
	cmd.flags = MMC_RSP_R3 | MMC_CMD_BCR;

	for (i = 100; i; i--) {
		err = mmc_start_request(&cmd);
		if (err)
			break;

		if (cmd.resp[0] & MMC_CARD_BUSY)
			break;

		delayms(10);

		/*
		 * According to eMMC specification v5.1 section 6.4.3, we
		 * should issue CMD1 repeatedly in the idle state until
		 * the eMMC is ready. Otherwise some eMMC devices seem to enter
		 * the inactive mode after mmc_init_card() issued CMD0 when
		 * the eMMC device is busy.
		 */
		if (!ocr)
			cmd.arg = cmd.resp[0] | BIT(30);
	}

	if (rocr)
		*rocr = cmd.resp[0];

	return err;
}

static int mmc_send_cxd_native(u32 arg, u32 *cxd, int opcode)
{
	int err;
	struct mmc_command cmd = {0};

	cmd.opcode = opcode;
	cmd.arg = arg;
	cmd.flags = MMC_RSP_R2 | MMC_CMD_AC;

	err = mmc_start_request(&cmd);
	if (err)
		return 1;

	memcpy(cxd, cmd.resp, sizeof(u32) * 4);

	return 0;
}

static int mmc_send_cid(u32 *cid)
{
	return mmc_send_cxd_native(0, cid, MMC_ALL_SEND_CID);
}

static int mmc_send_relative_addr(unsigned int *rca)
{
	int err;
	struct mmc_command cmd = {0};

	cmd.opcode = SD_SEND_RELATIVE_ADDR;
	cmd.arg = *rca << 16;
	cmd.flags = MMC_RSP_R6 | MMC_CMD_BCR;

	err = mmc_start_request(&cmd);
	if (err == false)
		return false;

	return true;
}

static int mmc_send_csd(u32 rca, u32 *csd)
{

	return mmc_send_cxd_native(rca << 16,	csd,
				MMC_SEND_CSD);
}

static int mmc_select_card(u32 rca)
{
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_SELECT_CARD;

	cmd.arg = rca << 16;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

	return mmc_start_request(&cmd);
}
#if 1
static int mmc_send_cxd_data(	u32 opcode, void *buf, unsigned len)
{
	struct mmc_command cmd = {0};
	struct mmc_data data = {0};

	cmd.opcode = opcode;
	cmd.arg = 0;

	/* NOTE HACK:  the MMC_RSP_SPI_R1 is always correct here, but we
	 * rely on callers to never use this with "native" calls for reading
	 * CSD or CID.  Native versions of those commands use the R2 type,
	 * not R1 plus a data block.
	 */
	cmd.flags = MMC_RSP_R1 | MMC_CMD_ADTC;
	cmd.data = &data;

	data.blksz = len;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	sdhci_set_block_info(&data);
	sdhci_config_dma(SDHCI_CTRL_SDMA);
//	sdhci_set_adma_addr((unsigned int)buf);
	sdhci_set_adma_addr(0x48000000);

	return mmc_start_request(&cmd);

}

static int mmc_get_ext_csd(u8 *new_ext_csd)
{
	int err;
	err = mmc_send_cxd_data(MMC_SEND_EXT_CSD, new_ext_csd,
				512);
	if (!err)
		return err;

	return err;
}
#endif
static int init_mmc(u32 *rca, u32 *csd)
{
	int err;
	u32 ocr, ocr_avail;
	u32 cid[4] = {0};
	u8 *wext_csd;
	u8 boot_partion;
	ocr_avail = MMC_VDD_165_195;

	mmc_go_idle();
	err = mmc_send_op_cond(0, &ocr);
	if (err)
		return false;
	if(!(ocr & ocr_avail))
		return false;

	mmc_go_idle();

	err = mmc_send_op_cond(ocr | (1 << 30), &ocr);
	if (err)
		return false;
	err = mmc_send_cid(cid);
	if(err)
		return false;
	err = mmc_send_relative_addr(rca);
	if (err)
		return false;
	err = mmc_send_csd(*rca, csd);
	if (err)
		return false;
	err = mmc_select_card(*rca);
	if (err)
		return false;
	err = mmc_get_ext_csd(wext_csd);
	if (err)
		return false;

	wext_csd = (u8 *)(u32)(EXT_CSD_ADDR);
	boot_partion = wext_csd[EXT_CSD_PART_CONFIG];
	boot_partion &= EXT_CSD_PART_CONFIG_BOOT_MASK;
	err = mmc_switch(EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONFIG, boot_partion);
	if (err)
		return false;

	return true;
}

static void mmc_ctl_reset(void)
{
	u32 val;
	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val |= BIT_EMMC_PHY_SOFT_RST | BIT_EMMC_HOST_SOFT_RST;
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);
	delayms(1);
	val &= ~(BIT_EMMC_PHY_SOFT_RST | BIT_EMMC_HOST_SOFT_RST);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);
}

static void sdhci_set_default_irqs(void)
{
	unsigned int ier;

	ier = SDHCI_INT_ADMA_ERROR | SDHCI_INT_DATA_END_BIT |
		    SDHCI_INT_DATA_CRC | SDHCI_INT_DATA_TIMEOUT |
		    SDHCI_INT_INDEX | SDHCI_INT_END_BIT | SDHCI_INT_CRC |
		    SDHCI_INT_TIMEOUT | SDHCI_INT_SPACE_AVAIL |SDHCI_INT_DATA_AVAIL |
		    SDHCI_INT_DMA_END | SDHCI_INT_DATA_END |
		    SDHCI_INT_RESPONSE;

	sdhci_writel(ier, SDHCI_INT_ENABLE);
	sdhci_writel(ier, SDHCI_SIGNAL_ENABLE);
}

static void sdhci_do_enable_v4_mode(void)
{
	u16 ctrl2;

	ctrl2 = sdhci_readw(SDHCI_HOST_CONTROL2);
	if (ctrl2 & SDHCI_CTRL_V4_MODE)
		return;

	ctrl2 |= SDHCI_CTRL_V4_MODE;
	sdhci_writew(ctrl2, SDHCI_HOST_CONTROL2);
}

static void sdhci_set_timeout(void)
{
	u8 count = 0xe;

	sdhci_writeb(count, SDHCI_TIMEOUT_CONTROL);
}

static void sdhci_init(int soft, unsigned int clk)
{

	if (soft)
		sdhci_reset(SDHCI_RESET_CMD | SDHCI_RESET_DATA);
	else
		sdhci_reset(SDHCI_RESET_ALL);

	sdhci_set_default_irqs();

	sdhci_set_ios(clk);

	sdhci_set_timeout();

	sdhci_do_enable_v4_mode();

	sdhci_config_dma(SDHCI_CTRL_SDMA);

	sdhci_writew(CARD_IS_EMMC, EMMC_CTRL_R);
}

static void mc_sdhci_set_ios_post(u32 phy_dl, u8 ddr_mode)
{
	u32 value;

	if(ddr_mode){
		value = BITS_EMMC_CLK_STOP_EN;
		value |= BITS_EMMC_TX_SAMPLE_CLK_2X_GATE;
		sdhci_phy_writel(value, EMMC_PHY_CTRL2);
		value |= BITS_EMMC_TX_SAMPLE_CLK_SEL;
		sdhci_phy_writel(value, EMMC_PHY_CTRL2);
		value &= ~BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
		sdhci_phy_writel(value,  EMMC_PHY_CTRL2);//phy ddr mode
		value =sdhci_phy_readl(EMMC_PHY_CTRL);

		value &= ~(BIT_EMMC_SAMPLE_EN |BIT_EMMC_RX_DL_BYPASS_DAT| BIT_EMMC_TX_DL_BYPASS |
			 BIT_EMMC_RX_CLK_INV_EN_DAT | BIT_EMMC_RX_DL_BYPASS_CMD | BIT_EMMC_RX_CLK_INV_EN_CMD);
		sdhci_phy_writel(value, EMMC_PHY_CTRL);
		value &= ~BIT_EMMC_DAT_INPUT_EN;
		sdhci_phy_writel(value,  EMMC_PHY_CTRL);
		value |= BIT_EMMC_FIFO_CLR;
		sdhci_phy_writel(value,  EMMC_PHY_CTRL);
		value &= ~BIT_EMMC_FIFO_CLR;
		sdhci_phy_writel(value,  EMMC_PHY_CTRL);

		sdhci_phy_writel(phy_dl, EMMC_PHY_DL_CTRL);

/*
dl 和反向
*/
		value |= (BIT_EMMC_TX_CLK_INV_EN);
		sdhci_phy_writel(value, EMMC_PHY_CTRL);

		value |= BIT_EMMC_DAT_INPUT_EN;
		sdhci_phy_writel(value,  EMMC_PHY_CTRL);

		value |= BIT_EMMC_SAMPLE_EN;
		sdhci_phy_writel(value,  EMMC_PHY_CTRL);

		value |= (BIT_EMMC_RX_CLK_SEL_DAT | BIT_EMMC_RX_CLK_SEL_CMD);
		sdhci_phy_writel(value,  EMMC_PHY_CTRL);

	}
	else{
		value = BITS_EMMC_CLK_STOP_EN;
		value &= ~(BITS_EMMC_TX_SAMPLE_CLK_2X_GATE);
		sdhci_phy_writel(value, EMMC_PHY_CTRL2);//phy sdr mode
		value &= ~(BITS_EMMC_TX_SAMPLE_CLK_SEL);
		sdhci_phy_writel(value, EMMC_PHY_CTRL2);
		value |= BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
		sdhci_phy_writel(value, EMMC_PHY_CTRL2);

		value = sdhci_phy_readl(EMMC_PHY_CTRL);
		value &= ~BIT_EMMC_SAMPLE_EN;
		sdhci_phy_writel(value,  EMMC_PHY_CTRL);

		value &= ~BIT_EMMC_DAT_INPUT_EN;
		sdhci_phy_writel(value, EMMC_PHY_CTRL);

		value &= ~(BIT_EMMC_SAMPLE_EN |BIT_EMMC_RX_DL_BYPASS_DAT| BIT_EMMC_TX_DL_BYPASS |
		 BIT_EMMC_RX_CLK_INV_EN_DAT | BIT_EMMC_RX_DL_BYPASS_CMD | BIT_EMMC_RX_CLK_INV_EN_CMD | BIT_EMMC_TX_CLK_INV_EN);

//		value |= BIT_EMMC_TX_CLK_INV_EN;
//		sdhci_phy_writel(value,  EMMC_PHY_CTRL);

		sdhci_phy_writel(phy_dl, EMMC_PHY_DL_CTRL);

		value |= BIT_EMMC_DAT_INPUT_EN;
		sdhci_phy_writel(value,  EMMC_PHY_CTRL);

		value |= BIT_EMMC_SAMPLE_EN;
		sdhci_phy_writel(value,  EMMC_PHY_CTRL);
	}

}

static void mmc_clk_disabled(void)
{
	u32 val;
	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val &= ~(BIT_CKG_EMMC_2X_EN | BIT_CKG_EMMC_EN | BIT_CKG_EMMC_BASE_EN | BIT_CKG_EMMC_CQETM_EN);
	val &= ~(BIT_CKG_EMMC_TM_EN | BIT_CKG_AXI_EMMC_EN | BIT_CKG_EMMC_PHYCFG_APB_EN);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);
}

static void mmc_clk_base_sel(void)
{
	u32 val;
	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val &= ~BITS_CKG_EMMC_BASE_MSK;
	val |= BITS_CKG_EMMC_BASE_SEL(1);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);
}

static void mmc_clk_enable(void)
{
	u32 val;
	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val |= BIT_CKG_EMMC_2X_EN | BIT_CKG_EMMC_EN | BIT_CKG_EMMC_BASE_EN | BIT_CKG_EMMC_CQETM_EN;
	val |= BIT_CKG_EMMC_TM_EN | BIT_CKG_AXI_EMMC_EN | BIT_CKG_EMMC_PHYCFG_APB_EN;
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);
}

static void driver_strenth(void)
{
	u32 val;
	int i = 0;
	void * addr;
	u32 base = 0x1f000004;
	for(i=0; i<11; i++) {
		addr = (void *)(base + i * 4);
		val = raw_readl(addr);
		val &= ~(0xf);
		val |= 0x8;
		raw_writel(val, addr);
	}

}

static void conflict_disable(void)
{
	u32 val;
	val = raw_readl(0x1c000128);
	val &= ~(0x1 << 4);
	raw_writel(val, 0x1c000128);

}
static void emmc_tm_clk_set(void)
{
	u32 val;
	void *addr = (void *)REG_EMMC_CLK_RST_CTRL1;
	val = raw_readl(addr);
	val &= ~(BITS_CKG_EMMC_TM_DIV_MSK);
	val |= BITS_CKG_EMMC_TM_DIV(0);
	raw_writel(val, addr);

	val = raw_readl(addr);
	val &= ~(BITS_CKG_EMMC_TM_SEL_MSK);
	val |= BITS_CKG_EMMC_TM_SEL(2);
	raw_writel(val, addr);
}

int emmc_reset_all_and_init(void)
{
	u32 rca = 1;
	u32 csd[4] = {0};
	int err = true;

	emmc_tm_clk_set();
	driver_strenth();
	conflict_disable();

	mmc_clk_disabled();
	mmc_clk_base_sel();
	delayms(1);
	mmc_clk_enable();
	mmc_ctl_reset();
	mmc_phy_init();

	sdhci_init(0, EMMC_LOWEST_FQ);
	err = init_mmc(&rca, csd);
	return err;
}

static int do_scan(u32 phy_dl, TEST_EMMC *test, u32 test_addr, u32 sector, u32 len)
{
	int err;
	u8 val;
	u32 ddr_mode = 0;
	u32 mmc_width;

#if defined(TEST_USE_CMD_8)
	u8 * wext_csd;
#endif

	err = emmc_reset_all_and_init();
	if(err == false){
		printf("init card err\r\n");
		goto ERR;
	}

	err = mmc_switch(1, EXT_CSD_HS_TIMING, EXT_CSD_HI_SPD);
	if ( err )
		goto ERR;
	
	err = mmc_switch(1, EXT_CSD_BUS_WIDTH, test->width);
	if ( err )
		goto ERR;
	
	err = mmc_switch(1, EXT_CSD_HS_TIMING, test->timing);
	if ( err )
		goto ERR;

	if (test->width & EXT_CSD_BUS_WIDTH_8)
		mmc_width = MMC_BUS_WIDTH_8;
	else
		mmc_width = MMC_BUS_WIDTH_4;

	sdhci_set_bus_width(mmc_width);

	val = sdhci_readb(SDHCI_HOST_CONTROL);
	val |= SDHCI_CTRL_HISPD;
	sdhci_writeb(val, SDHCI_HOST_CONTROL);

	if(test->timing == EXT_CSD_SPD_HS400){
		u16 tmp;
		tmp = sdhci_readw(SDHCI_HOST_CONTROL2);
		tmp |= (SDHCI_CTRL_HS400 );
		sdhci_writew(tmp , SDHCI_HOST_CONTROL2);
		ddr_mode = 1;
	}

	mc_sdhci_set_ios_post(phy_dl, ddr_mode);

	sdhci_set_clock(TEST_DLL_CLK);
	delayms(1);

#if defined(TEST_USE_CMD_8)

	err = mmc_get_ext_csd(wext_csd);
	if (err) {
		printf("e=%d\n", err);
	}

#endif

#if defined(TEST_USE_RW)

#if 1
	err = mmc_blk_mq_issue_rw_rq(READ, test_addr, sector, len);
	if(err) {
		goto ERR;
	}
#endif

#if 1
	err = mmc_blk_mq_issue_rw_rq(WRITE, test_addr, sector, len);
	if(err) {
		goto ERR;
	}
#endif
#endif
ERR:
	return err;
}

void mc_mmc_8bit_test(void)
{
	do_scan(0x0, &extw[1], 0x48000000, 0x20000, 0x200);
}

int get_valid_val_index(unsigned char * array, int size, int acc)
{
	int i = 0, cnt = 0;
	int index = i;
	int flag = 32 / acc;
	for (i = 0; i<size/acc; i+=1) {
		if (array[i] + acc != array[i+1]) {
			if(cnt > flag){
				break;
			}
			else{
				index = i + 1;
				cnt = 0;
			}
		}else{
			cnt++;
		}
	}
	return (index + (cnt/2));
}

static u32 mmc_dl_scan_1_way(int mode, u8 accuracy, u32 test_addr, u32 sector, u32 len){
	int err;
	u32 phy_dl;
	u32 i;
	u32 dl_rx = 0, dl_tx = 0;
	int index;
	int max_dll_val;
	u8 * array;
	TEST_EMMC *ios;

	array = malloc(EMMC_DLL_MAX_VAL);
	if (array == NULL)
		return 1;

	if (mode == HS400_MODE) {
		ios = &extw[0];
		max_dll_val = EMMC_DLL_MAX_VAL / 4;		//adjust for
	}
	else {
		ios = &extw[1];
		max_dll_val = EMMC_DLL_MAX_VAL / 4;
	}

	dl_rx_get:
		memset(array, 0, EMMC_DLL_MAX_VAL);
		for (i = 0; i < max_dll_val; i += accuracy) {
			dl_rx = i;
			phy_dl = ( (dl_rx << 16) | (dl_rx << 8) | dl_tx );
			err = do_scan(phy_dl, ios, test_addr, sector, len);
			if (!err) {
				array[i/accuracy] = dl_rx;
			}
		}
		index = get_valid_val_index(array, EMMC_DLL_MAX_VAL, accuracy);
		dl_rx = array[index];
		if (dl_rx == 0) {
			dl_tx += accuracy;
			if (dl_tx > max_dll_val)
				return 0;
			goto dl_rx_get;
		}
	dl_tx_get:
		memset(array, 0, EMMC_DLL_MAX_VAL);
		for (i = dl_tx; i < max_dll_val; i+=accuracy) {
			dl_tx = i;
			phy_dl = ( (dl_rx << 16) | (dl_rx << 8) | dl_tx );
			err = do_scan(phy_dl, ios, test_addr, sector, len);
			if (!err)
				array[i/accuracy] = dl_tx;
		}
		index = get_valid_val_index(array, EMMC_DLL_MAX_VAL, accuracy);
		dl_tx = array[index];
		if (dl_tx == 0) {
			if (mode == HS400_MODE)
				dl_rx -= accuracy;
			else
				dl_rx += accuracy;

			if (dl_rx >= max_dll_val || dl_rx == 0)
				return 0;

			goto dl_tx_get;
		}


	phy_dl = ( (dl_rx << 16) | (dl_rx << 8) | dl_tx );
	free(array);
	return phy_dl;	
}

static void mmc_printf_dl(u8 (*p_dll)[EMMC_DLL_TX_NUM], u8 accuracy, u8 mode)
{
	u32 num;
	u32 tx, rx;

	printf("Row coordinates:RX\nColumn coordinates:TX\n");
	if(mode == HS400_MODE)
		printf("emmc ds mode dll:\n");
	else
		printf("emmc HS200 mode dll:\n");

	printf("  ");
	for(num = 0; num < EMMC_DLL_TX_NUM; num+=accuracy){
		printf("%2x",num);
	}

	for(tx = 0; tx < EMMC_DLL_TX_NUM; tx+=accuracy){
		printf("\n%2x",tx);
		for(rx = 0; rx < EMMC_DLL_RX_NUM; rx+=accuracy){
			if(p_dll[tx][rx] == 0)
				printf("  ");
			else
				printf("%2x",p_dll[tx][rx]);
		}
	}
	printf("\n");
}

static int mmc_dl_scan_2_way(int mode, u8 accuracy, u32 test_addr, u32 sector, u32 len)
{

	int err;
	u32 phy_dl;
	u32 dl_rx, dl_tx;

	TEST_EMMC *ios;

	if (mode == HS400_MODE) 
		ios = &extw[0];
	else
		ios = &extw[1];

	u8 (*p_dll)[EMMC_DLL_TX_NUM] = malloc(EMMC_DLL_TOTAL_SIZE);
	memset(*p_dll, 0x0 , EMMC_DLL_TOTAL_SIZE);

	for (dl_tx = 0x0; dl_tx < EMMC_DLL_TX_NUM; dl_tx+=accuracy) {
		for (dl_rx = 0x0; dl_rx < EMMC_DLL_RX_NUM; dl_rx+=accuracy) {
			phy_dl = ( (dl_rx << 16) | (dl_rx << 8) | dl_tx );
			err = do_scan(phy_dl, ios, test_addr, sector, len);
			if(!err)
				p_dll[dl_tx][dl_rx] = 1;
		}
	}
	mmc_printf_dl(p_dll, accuracy, mode);

	free(p_dll);

	return err;
}

u32 do_scan_normal(int mode, int way, u8 accuracy, u32 test_addr, u32 sector, u32 len)
{
	u32 phy_dl;
	if(way == 0){
		phy_dl = mmc_dl_scan_1_way(mode, accuracy, test_addr, sector, len);
	}
	else
	{
		phy_dl = mmc_dl_scan_2_way(mode, accuracy,test_addr, sector, len);
	}
	return phy_dl;
}

static int mmc_dl_scan_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u32 phy_dl;

	u32 addr = 0x48000000;
	u32 sector = 0x20000;
	u32 len = 0x200;
#if 0
	if (argc != 7) {
	   printf("mmc_dl_test + mode + way + step + addr + sector + len,\n");
	   return CMD_RET_FAILURE;
	}
#endif
	u8 mode =  simple_strtoul(argv[1], NULL, 10);
	if (mode <0 || mode > 1) {
		printf("mode, hs400:0, hs200:1\n");
		return CMD_RET_FAILURE;
	}

	u8 way =  simple_strtoul(argv[2], NULL, 10);
	if (way <0 || way > 1) {
		printf("way, 0:1 way, 1: 2 way\n");
		return CMD_RET_FAILURE;
	}

	u8 accuracy =  simple_strtoul(argv[3], NULL, 10);
	if (accuracy <0 || accuracy > 16) {
		printf("etc:2,4,8,16\n");
		return CMD_RET_FAILURE;
	}

#if 0
	u64 addr =  simple_strtoul(argv[4], NULL, 16);
	u32 sector =  simple_strtoul(argv[5], NULL, 16);
	u32 len =  simple_strtoul(argv[6], NULL, 16);

#endif
	phy_dl = do_scan_normal(mode, way, accuracy, addr, sector, len);

	printf("0x%x\r\n", phy_dl);
	return 0;
}
U_BOOT_CMD(
 mcscan, 4, 0, mmc_dl_scan_cmd,
 "scan corrected mmc delay line settings mmc_dl_test + mode + way + step + addr + sector + len",
 ""
);

#if 0
static int mmc_dl_invliad(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int err;
	u8 wext_csd[512] = {0,};
	u8 boot_partition;
	u32 start_offset;
	struct emmc_phy_dll * ptr;

	err = emmc_reset_all_and_init();
	if (err == false)
		goto invliad_out;

	err = mmc_get_ext_csd(wext_csd);
	if (err)
		goto invliad_out;

	boot_partition = wext_csd[EXT_CSD_PART_CONFIG];
	boot_partition &= EXT_CSD_PART_CONFIG_BOOT_MASK;

	boot_partition &= ~0x07;
	boot_partition |= CONFIG_SYS_MMC_ENV_PART;
	err = mmc_switch(EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_PART_CONFIG, boot_partition);

	if (err)
		goto invliad_out;

	start_offset = ALIGN((CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE), 512) / 512;
	err = mmc_blk_mq_issue_rw_rq(READ, (u32)wext_csd, start_offset, 1);
	if (err)
		goto invliad_out;

	ptr = (struct emmc_phy_dll *)wext_csd;
	ptr->valid_flag = 0;
	err = mmc_blk_mq_issue_rw_rq(WRITE, (u32)wext_csd, start_offset, 1);
	if (err)
		goto invliad_out;

	return 0;

invliad_out:
	return 1;
}

U_BOOT_CMD(
 mc_mmc_dl_invalid, 1, 0, mmc_dl_invliad,
 "invalid dll",
 ""
);

#endif

