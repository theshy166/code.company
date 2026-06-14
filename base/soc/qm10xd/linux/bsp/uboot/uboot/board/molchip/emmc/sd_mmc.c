/*
 * (C) Copyright 2020 Molchip technology
 *
 * SD_MMC
 *
 */
#include <common.h>
#include <mach/hardware.h>
#include <asm/io.h>
#include <mach/systimer.h>

#include "mach/sd_mmc.h"

struct sdhci_adma2_32_desc adma_32_desc_table[64];

#define raw_readl(x)	__raw_readl((void *)x)
#define raw_readw(x)	__raw_readw((void *)x)
#define raw_readb(x)	__raw_readb((void *)x)

#define raw_writel(v,a)		__raw_writel(v, (void *)a)
#define raw_writew(v,a)		__raw_writew(v, (void *)a)
#define raw_writeb(v,a)		__raw_writeb(v, (void *)a)

void mmc_clk_set(void)
{
	u32 val;
	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val &= ~(BIT_CKG_EMMC_PHYCFG_APB_EN | BIT_CKG_AHB_EMMC_EN | 
				BIT_CKG_AXI_EMMC_EN |BIT_CKG_EMMC_2X_EN | 
				BIT_CKG_EMMC_EN | BIT_CKG_EMMC_BASE_EN |
				BIT_CKG_EMMC_CQETM_EN | BIT_CKG_EMMC_TM_EN);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

	delayms(1);

	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val |= (BIT_EMMC_HOST_SOFT_RST | BIT_EMMC_PHY_SOFT_RST);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

	delayms(1);

	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val &= ~(BITS_CKG_EMMC_BASE_MSK);
	val &= ~(BITS_CKG_EMMC_2X_SEL_MSK);

	val |= BITS_CKG_EMMC_BASE_SEL(EMMC_BASE_SEL_24M);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

	delayms(1);

	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val &= ~(BIT_EMMC_HOST_SOFT_RST | BIT_EMMC_PHY_SOFT_RST);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val |= (BIT_CKG_EMMC_PHYCFG_APB_EN | BIT_CKG_AHB_EMMC_EN | 
				BIT_CKG_AXI_EMMC_EN |BIT_CKG_EMMC_2X_EN | 
				BIT_CKG_EMMC_EN | BIT_CKG_EMMC_BASE_EN |
				BIT_CKG_EMMC_CQETM_EN | BIT_CKG_EMMC_TM_EN);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

}

void mmc_ctl_reset(void)
{
	u32 val;
	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val |= BIT_EMMC_PHY_SOFT_RST | BIT_EMMC_HOST_SOFT_RST;
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);
	delayms(1);
	val &= ~(BIT_EMMC_PHY_SOFT_RST | BIT_EMMC_HOST_SOFT_RST);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);
}

void emmc_clk_div(unsigned int clock)
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
		raw_writel(val, REG_EMMC_CLK_RST_CTRL);
	}

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

	val &= ~(BITS_CKG_EMMC_BASE_MSK);
	if (div < 60)
		val |= BITS_CKG_EMMC_BASE_SEL(2);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

	delayms(1);

	val = raw_readl(REG_EMMC_CLK_RST_CTRL);
	val |= (BIT_CKG_EMMC_EN | BIT_CKG_EMMC_2X_EN);
	raw_writel(val, REG_EMMC_CLK_RST_CTRL);

}
void emmc_tm_clk_set(void)
{
	u32 val;
	void *addr = (void *)REG_EMMC_CLK_RST_CTRL1;
	val = readl(addr);
	val &= ~(BITS_CKG_EMMC_TM_DIV_MSK);
	val |= BITS_CKG_EMMC_TM_DIV(23);
	writel(val, addr);
}

void conflict_disable(void)
{
	u32 val;
	val = readl(0x1c000128);
	val &= ~(0x1 << 4);
	writel(val, 0x1c000128);

}

void mmc_phy_init(void)
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

	val = readl(EMMC_PHY_CTRL2);
	val |= BITS_EMMC_CLK_STOP_EN | BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
	writel(val, EMMC_PHY_CTRL2);

}

void mmc_hard_reset(void)
{
	u32 val;

	val = raw_readl(EMMC_PHY_IE_CTRL);
	val &= ~CARD_RESET_N;
	raw_writel(val, EMMC_PHY_IE_CTRL);
	delayms(1);

	val |= CARD_RESET_N;
	raw_writel(val, EMMC_PHY_IE_CTRL);
	delayms(1);

}

int sdhci_reset(u8 mask)
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
			return false;
		}
	}

	return true;
}

static void sdhci_set_default_irqs(void)
{
	unsigned int ier;

	ier = SDHCI_INT_BUS_POWER | SDHCI_INT_DATA_END_BIT |
		    SDHCI_INT_DATA_CRC | SDHCI_INT_DATA_TIMEOUT |
		    SDHCI_INT_INDEX | SDHCI_INT_END_BIT | SDHCI_INT_CRC |
		    SDHCI_INT_TIMEOUT | SDHCI_INT_DMA_END | SDHCI_INT_DATA_END |
		    SDHCI_INT_RESPONSE;

	sdhci_writel(ier, SDHCI_INT_ENABLE);
	sdhci_writel(ier, SDHCI_SIGNAL_ENABLE);
}

void sdhci_set_bus_width(int width)
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

u16 sdhci_calc_clk(unsigned int clock,
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

int sdhci_enable_clk(u16 clk)
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

	return true;
}

void sdhci_set_clock(unsigned int clock)
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

void sdhci_set_ios(unsigned int clk)
{
	u8 ctrl;

	sdhci_set_clock(clk);
	sdhci_set_bus_width(0);

	ctrl = sdhci_readb(SDHCI_HOST_CONTROL);
	ctrl &= ~SDHCI_CTRL_HISPD;
	sdhci_writeb(ctrl, SDHCI_HOST_CONTROL);

}

void sdhci_set_timeout(void)
{
	u8 count = 0xe;

	sdhci_writeb(count, SDHCI_TIMEOUT_CONTROL);
}


void sdhci_do_enable_v4_mode(void)
{
	u16 ctrl2;

	ctrl2 = sdhci_readw(SDHCI_HOST_CONTROL2);
	if (ctrl2 & SDHCI_CTRL_V4_MODE)
		return;

	ctrl2 |= SDHCI_CTRL_V4_MODE;
	sdhci_writew(ctrl2, SDHCI_HOST_CONTROL2);
}

void sdhci_config_dma(int dma)
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


void sdhci_init(int soft, unsigned int clk)
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
		mode |= SDHCI_TRNS_AUTO_CMD23;
		sdhci_writel(data->blocks, SDHCI_ARGUMENT2);
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


void sdhci_finish_command(struct mmc_command *cmd)
{

	if (cmd->flags & MMC_RSP_PRESENT) {
		if (cmd->flags & MMC_RSP_136) {
			sdhci_read_rsp_136(cmd);
		} else {
			cmd->resp[0] = sdhci_readl(SDHCI_RESPONSE);
		}
	}

}


int sdhci_send_command(struct mmc_command *cmd)
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
			printf("emmc error, error = 0x%x", val);
			return false;
		}
		if (val & SDHCI_INT_RESPONSE) {
			sdhci_finish_command(cmd);
			if (flags & SDHCI_CMD_DATA) {
					return true;
			} else if (cmd->flags & MMC_RSP_BUSY) {
				if(val & SDHCI_INT_DATA_END) {
					sdhci_writel(val, SDHCI_INT_STATUS);
					return true;
				}
			}else {
				sdhci_writel(val, SDHCI_INT_STATUS);
				return true;
			}
		}
		if (t2 - t1 > EMMC_WAIT_TIMEOUT)
			break;
	}while(1);

	return false;

}


void sdhci_set_block_info(struct mmc_data *data)
{
	/* Set the DMA boundary value and block size */
	sdhci_writew(SDHCI_MAKE_BLKSZ(SDHCI_DEFAULT_BOUNDARY_ARG, data->blksz),
		     SDHCI_BLOCK_SIZE);

	if (sdhci_readw(SDHCI_BLOCK_COUNT))
		sdhci_writew(0, SDHCI_BLOCK_COUNT);
	sdhci_writew(data->blocks, SDHCI_32BIT_BLK_CNT);
}

void sdhci_set_adma_addr(u32 addr)
{
	sdhci_writel(addr, SDHCI_ADMA_ADDRESS);
}

int sdhci_card_busy(void)
{
	u32 present_state;

	/* Check whether DAT[0] is 0 */
	present_state = sdhci_readl(SDHCI_PRESENT_STATE);

	return !(present_state & SDHCI_DATA_0_LVL_MASK);
}

int mmc_start_request(struct mmc_command *cmd)
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

void sdhci_set_boot_ctrl(void)
{
	sdhci_writew(VALIDATE_BOOT | BOOT_EN, BOOT_CTRL_R);
}


void sdhci_adma_write_desc(struct sdhci_adma2_32_desc *dma_desc, u32 addr, int len, unsigned int cmd)
{
//	struct sdhci_adma2_64_desc *dma_desc = *desc;

	/* 32-bit and 64-bit descriptors have these members in same position */
	dma_desc->cmd = cmd;  //cpu_to_le16(cmd);
	dma_desc->len = len; //cpu_to_le16(len);
	dma_desc->addr = lower_32_bits(addr);

//	dma_desc->addr_hi = upper_32_bits(addr);

//	*desc += 8;


}

void sdhci_adma_table_pre(struct mmc_data *data)
{
	struct sdhci_adma2_32_desc *adma_table = &adma_32_desc_table[0];
	memset(adma_32_desc_table, 0 , sizeof(adma_32_desc_table));

	sdhci_adma_write_desc(adma_table, data->blk_addr, (data->blocks * data->blksz), ADMA2_TRAN_VALID);

	adma_table = &adma_32_desc_table[1];

	sdhci_adma_write_desc(adma_table, data->blk_addr, (data->blocks * data->blksz), ADMA2_NOP_END_VALID);


	sdhci_set_adma_addr((u32)(unsigned long)adma_32_desc_table);

}

void mc_phy_set_ios_post(u8 ddr_mode)
{
	u32 value;

	if(ddr_mode){
		value = BITS_EMMC_CLK_STOP_EN;
		value |= BITS_EMMC_TX_SAMPLE_CLK_2X_GATE;
		raw_writel(value, EMMC_PHY_CTRL2);
		value |= BITS_EMMC_TX_SAMPLE_CLK_SEL;
		raw_writel(value, EMMC_PHY_CTRL2);
		value &= ~BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
		raw_writel(value,  EMMC_PHY_CTRL2);//phy ddr mode
		value =raw_readl(EMMC_PHY_CTRL);

		value &= ~(SAMPLE_EN |RX_DL_BYPASS_DAT| TX_DL_BYPASS |
			 RX_CLK_INV_EN_DAT | RX_DL_BYPASS_CMD | RX_CLK_INV_EN_CMD);
		raw_writel(value, EMMC_PHY_CTRL);
		value &= ~(DAT_INPUT_EN);
		raw_writel(value,  EMMC_PHY_CTRL);
		value |= FIFO_CLR;
		raw_writel(value,  EMMC_PHY_CTRL);
		value &= ~(FIFO_CLR);
		raw_writel(value,  EMMC_PHY_CTRL);

		raw_writel(EMM_PHY_DELAY_LINE_HS400, EMMC_PHY_DL_CTRL);

/*
dl 和反向
*/
		value |= (TX_CLK_INV_EN);
		raw_writel(value, EMMC_PHY_CTRL);

		value |= DAT_INPUT_EN;
		raw_writel(value,  EMMC_PHY_CTRL);

		value |= SAMPLE_EN;
		raw_writel(value,  EMMC_PHY_CTRL);

		value |= (RX_CLK_SEL_DAT | RX_CLK_SEL_CMD);
		raw_writel(value,  EMMC_PHY_CTRL);

	}
	else{
		value = BITS_EMMC_CLK_STOP_EN;
		value &= ~(BITS_EMMC_TX_SAMPLE_CLK_2X_GATE);
		raw_writel(value, EMMC_PHY_CTRL2);//phy sdr mode
		value &= ~(BITS_EMMC_TX_SAMPLE_CLK_SEL);
		raw_writel(value, EMMC_PHY_CTRL2);
		value |= BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
		raw_writel(value, EMMC_PHY_CTRL2);

		value = raw_readl(EMMC_PHY_CTRL);
		value &= ~(SAMPLE_EN);
		raw_writel(value,  EMMC_PHY_CTRL);

		value &= ~(DAT_INPUT_EN);
		raw_writel(value, EMMC_PHY_CTRL);

		value &= ~(SAMPLE_EN |RX_DL_BYPASS_DAT| TX_DL_BYPASS |
		 RX_CLK_INV_EN_DAT | RX_DL_BYPASS_CMD | RX_CLK_INV_EN_CMD | TX_CLK_INV_EN);

		raw_writel(value, EMMC_PHY_CTRL);

		raw_writel(EMMC_PHY_DELAY_LINE_HS200, EMMC_PHY_DL_CTRL);

		value |= DAT_INPUT_EN;
		raw_writel(value,  EMMC_PHY_CTRL);

		value |= SAMPLE_EN;
		raw_writel(value,  EMMC_PHY_CTRL);
	}

}


int mandatory_boot(u32 addr, u32 len)
{
	u32 t1, t2, val;

	sdhci_set_adma_addr(addr);

	sdhci_writew(SDHCI_MAKE_BLKSZ(SDHCI_DEFAULT_BOUNDARY_ARG, 512),
		     SDHCI_BLOCK_SIZE);

	if (sdhci_readw(SDHCI_BLOCK_COUNT))
		sdhci_writew(0, SDHCI_BLOCK_COUNT);
	sdhci_writew(len >> SECTOR_SHIFT, SDHCI_32BIT_BLK_CNT);

	val = SDHCI_TRNS_BLK_CNT_EN | SDHCI_TRNS_MULTI;
	val |= SDHCI_TRNS_READ | SDHCI_TRNS_DMA;
	sdhci_writew(val, SDHCI_TRANSFER_MODE);

	sdhci_set_boot_ctrl();

	t1 = get_sys_timer_val();
	while(1) {
		
		val = sdhci_readl(SDHCI_INT_STATUS);
		if ((val & SDHCI_INT_DATA_END) && (val & SDHCI_INT_RESPONSE)){
			sdhci_writel(val, SDHCI_INT_STATUS);
			return true;
		}

		t2 = get_sys_timer_val();
		if (t2 - t1 > EMMC_WAIT_TIMEOUT)
			break;

		delayms(1);
	}

	return false;
}

