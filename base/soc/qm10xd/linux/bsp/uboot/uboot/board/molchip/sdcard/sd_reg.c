/*
 * (C) Copyright 2020 Molchip technology
 *
 * SD_MMC
 *
 */
#include <mach/hardware.h>
#include <common.h>
#include <mach/systimer.h>
#include <mach/spl.h>
#include <mach/sd_reg.h>
#include <asm/io.h>


static unsigned int reg_sdio_base;
static unsigned int reg_sdio_ctrl0;
static unsigned int reg_sdio_ctrl1;
static unsigned int reg_sdio_ctrl2;
static unsigned int reg_sdio_spl1;
static unsigned int reg_sdio_clk_mux;
static unsigned int sdio_dllout_status_reg_spl;
static unsigned int sdio_dllout_status_reg_drv;

void mci_addr_selection(u32 boot_mode)
{
	if (boot_mode == BOOT_DEVICE_SDIO0) {
		reg_sdio_base = REG_SDIO0_BASE;
		reg_sdio_ctrl0 = REG_SDIO0_CTRL0;
		reg_sdio_ctrl1 = REG_SDIO0_CTRL1;
		reg_sdio_ctrl2 = REG_SDIO0_CTRL2;
		reg_sdio_spl1 = REG_SDIO0_SPL1;
		reg_sdio_clk_mux = REG_SDIO0_CLK_MUX;
		sdio_dllout_status_reg_spl = SDIO0_DLLOUT_STATUS_SPL;
		sdio_dllout_status_reg_drv = SDIO0_DLLOUT_STATUS_DRV;
	}else{
		reg_sdio_base = REG_SDIO1_BASE;
		reg_sdio_ctrl0 = REG_SDIO1_CTRL0;
		reg_sdio_ctrl1 = REG_SDIO1_CTRL1;
		reg_sdio_ctrl2 = REG_SDIO1_CTRL2;
		reg_sdio_spl1 = REG_SDIO1_SPL1;
		reg_sdio_clk_mux = REG_SDIO1_CLK_MUX;
		sdio_dllout_status_reg_spl = SDIO1_DLLOUT_STATUS_SPL;
		sdio_dllout_status_reg_drv = SDIO1_DLLOUT_STATUS_DRV;
	}
}

void mci_drive_strength(u32 boot_mode)
{
	u32 val;

	if(boot_mode == BOOT_DEVICE_SDIO0){
		val = __raw_readl(DRI_PULL_UP_SD0_CD);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD0_CD);

		val = __raw_readl(DRI_PULL_UP_SD0_CLK);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD0_CLK);

		val = __raw_readl(DRI_PULL_UP_SD0_CMD);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD0_CMD);

		val = __raw_readl(DRI_PULL_UP_SD0_D0);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD0_D0);

		val = __raw_readl(DRI_PULL_UP_SD0_D1);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD0_D1);

		val = __raw_readl(DRI_PULL_UP_SD0_D2);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD0_D2);

		val = __raw_readl(DRI_PULL_UP_SD0_D3);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD0_D3);
	}else{
		val = __raw_readl(DRI_PULL_UP_SD1_CD);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD1_CD);

		val = __raw_readl(DRI_PULL_UP_SD1_CLK);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD1_CLK);

		val = __raw_readl(DRI_PULL_UP_SD1_CMD);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD1_CMD);

		val = __raw_readl(DRI_PULL_UP_SD1_D0);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD1_D0);

		val = __raw_readl(DRI_PULL_UP_SD1_D1);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD1_D1);

		val = __raw_readl(DRI_PULL_UP_SD1_D2);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD1_D2);

		val = __raw_readl(DRI_PULL_UP_SD1_D3);
		val = DRI_STRENGTH(val, 6);
		__raw_writel(val, DRI_PULL_UP_SD1_D3);
	}
}

void mci_pull_up(u32 boot_mode)
{
	u32 val;

	if(boot_mode == BOOT_DEVICE_SDIO0){
		val = __raw_readl(DRI_PULL_UP_SD0_CD);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD0_CD);

		val = __raw_readl(DRI_PULL_UP_SD0_CLK);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD0_CLK);

		val = __raw_readl(DRI_PULL_UP_SD0_CMD);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD0_CMD);

		val = __raw_readl(DRI_PULL_UP_SD0_D0);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD0_D0);

		val = __raw_readl(DRI_PULL_UP_SD0_D1);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD0_D1);

		val = __raw_readl(DRI_PULL_UP_SD0_D2);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD0_D2);

		val = __raw_readl(DRI_PULL_UP_SD0_D3);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD0_D3);
	}else{
		val = __raw_readl(DRI_PULL_UP_SD1_CD);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD1_CD);

		val = __raw_readl(DRI_PULL_UP_SD1_CLK);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD1_CLK);

		val = __raw_readl(DRI_PULL_UP_SD1_CMD);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD1_CMD);

		val = __raw_readl(DRI_PULL_UP_SD1_D0);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD1_D0);

		val = __raw_readl(DRI_PULL_UP_SD1_D1);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD1_D1);

		val = __raw_readl(DRI_PULL_UP_SD1_D2);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD1_D2);

		val = __raw_readl(DRI_PULL_UP_SD1_D3);
		val = PULL_UP(val);
		__raw_writel(val, DRI_PULL_UP_SD1_D3);
	}
}

void mci_io_mux(u32 boot_mode)
{
	if(boot_mode == BOOT_DEVICE_SDIO0){
		__raw_writel(IO_MUX_SD0, IO_SD0_CD);
		__raw_writel(IO_MUX_SD0, IO_SD0_CLK);
		__raw_writel(IO_MUX_SD0, IO_SD0_CMD);
		__raw_writel(IO_MUX_SD0, IO_SD0_D0);
		__raw_writel(IO_MUX_SD0, IO_SD0_D1);
		__raw_writel(IO_MUX_SD0, IO_SD0_D2);
		__raw_writel(IO_MUX_SD0, IO_SD0_D3);
	}else{
		__raw_writel(IO_MUX_SD1, IO_SD1_CD);
		__raw_writel(IO_MUX_SD1, IO_SD1_CLK);
		__raw_writel(IO_MUX_SD1, IO_SD1_CMD);
		__raw_writel(IO_MUX_SD1, IO_SD1_D0);
		__raw_writel(IO_MUX_SD1, IO_SD1_D1);
		__raw_writel(IO_MUX_SD1, IO_SD1_D2);
		__raw_writel(IO_MUX_SD1, IO_SD1_D3);
	}
}

void mci_clk_enable(void)
{
	u32 val;

	val = __raw_readl(reg_sdio_ctrl2);
	if (reg_sdio_ctrl2 == REG_SDIO0_CTRL2) {
		__raw_writel(SDIO0_CLK_MUX_VALUE, reg_sdio_clk_mux);
		val |= CKG_AHB_SDIO0_EN | CKG_SDIO0_DLLREF_EN | CKG_SDIO0_EN;
	} else {
		val |= CKG_AHB_SDIO1_EN | CKG_SDIO1_DLLREF_EN | CKG_SDIO1_EN
			| SDIO1_CLK_MUX_VALUE;
	}
	__raw_writel(val, reg_sdio_ctrl2);
}

void mci_ctl_reset(void)
{
	u32 val;

	val = __raw_readl(reg_sdio_ctrl1);
	val |= AHB_SDIO_SOFT_RST;
	__raw_writel(val, reg_sdio_ctrl1);
	delayms(1);
	val &= ~AHB_SDIO_SOFT_RST;
	__raw_writel(val, reg_sdio_ctrl1);
}

bool dw_mci_ctrl_reset(u32 reset)
{
	u32 ctrl;
	u32 t1, t2;

	ctrl = mci_readl(CTRL);
	ctrl |= reset;
	mci_writel(CTRL, ctrl);

	/* wait till resets clear */
	t1 = get_sys_timer_val();
	while (1) {
		ctrl = mci_readl(CTRL);
		if (!(ctrl & reset))
			break;
		t2 = get_sys_timer_val();
		if ((t2 - t1) > MCI_WAIT_TIME_OUT)
			break;
	}

	ctrl = mci_readl(CTRL);
	if (ctrl & reset)
		return false;

	return true;
}

void dw_mci_idmac_reset(void)
{
	u32 bmod = mci_readl(BMOD);

	/* Software reset of DMA */
	bmod |= SDMMC_IDMAC_SWRESET;
	mci_writel(BMOD, bmod);
}

void dw_mci_select_idmac_interface(void)
{
	u32 temp;

	temp = mci_readl(CTRL);
	temp |= SDMMC_CTRL_USE_IDMAC;
	mci_writel(CTRL, temp);
}

void dw_mci_deable_idmac(void)
{
	u32 temp;

	temp = mci_readl(BMOD);
	temp |= SDMMC_IDMAC_ENABLE | SDMMC_IDMAC_FB;
	mci_writel(BMOD, temp);
}

void dw_mci_start_run(void)
{
	mci_writel(PLDMND, 1);
}

void dw_mci_idmac_stop_dma(void)
{
	u32 temp;

	/* Disable and reset the IDMAC interface */
	temp = mci_readl(CTRL);
	temp &= ~SDMMC_CTRL_USE_IDMAC;
	temp |= SDMMC_CTRL_DMA_RESET;
	mci_writel(CTRL, temp);

	/* Stop the IDMAC running */
	temp = mci_readl(BMOD);
	temp &= ~(SDMMC_IDMAC_ENABLE | SDMMC_IDMAC_FB);
	temp |= SDMMC_IDMAC_SWRESET;
	mci_writel(BMOD, temp);
}

void dw_mci_mask_interrupts(void)
{
	/* Mask out interrupts - get Tx & Rx complete only */
	mci_writel(IDSTS, IDMAC_INT_CLR);
	mci_writel(IDINTEN, SDMMC_IDMAC_INT_NI |
			SDMMC_IDMAC_INT_RI | SDMMC_IDMAC_INT_TI);
}

void dw_mci_set_descriptor(u32 *descriptor)
{
	/* Set the descriptor base address */
	mci_writel(DBADDR, (u32)descriptor);
}
void dw_mci_cmd_start(u32 cmd_start)
{
	mci_writel(CMD, cmd_start);
}

int dw_mci_wait_while_busy(u32 cmd_flags)
{
	u32 status;
	u32 t1, t2;

	if (cmd_flags & SDMMC_CMD_PRV_DAT_WAIT) {
		t1 = get_sys_timer_val();
		do {
			t2 = get_sys_timer_val();
			status = mci_readl(STATUS);
			if (!(status & SDMMC_STATUS_BUSY))
				break;
		} while ((t2 - t1) < MCI_WAIT_TIME_OUT);

		if (status & SDMMC_STATUS_BUSY)
			return false;
	}

	return true;
}

void mci_send_cmd(u32 cmd, u32 arg)
{
	u32 val;
	u32 t1, t2;

	mci_writel(CMDARG, arg);
	dw_mci_wait_while_busy(cmd);
	mci_writel(CMD, SDMMC_CMD_START | cmd);

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		val = mci_readl(CMD);
		if (!(val & SDMMC_CMD_START))
			break;
	} while ((t2 - t1) < MCI_WAIT_TIME_OUT);
}

int wait_cmd_done(u32 done)
{
	u32 t1, t2;
	u32 val;

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		val = mci_readl(CMD);
		if (!(val & SDMMC_CMD_START))
			break;
	} while ((t2 - t1) < MCI_WAIT_TIME_OUT);

	t1 = get_sys_timer_val();
	do {
		t2 = get_sys_timer_val();
		val = mci_readl(MINTSTS);
		if ((val & done) == done)
			break;
	} while ((t2 - t1) < MCI_WAIT_TIME_OUT);

	if ((val & done) != done)
		return false;

	return true;
}

void dw_mci_cmdarg(u32 cmdarg)
{
	mci_writel(CMDARG, cmdarg);
}

void dw_mci_enable_dma_interface(void)
{
	u32 temp;

	temp = mci_readl(CTRL);
	temp |= SDMMC_CTRL_DMA_ENABLE;
	mci_writel(CTRL, temp);
}

void dw_mci_disable_irq(void)
{
	u32 temp;

	temp = mci_readl(INTMASK);
	temp  &= ~(SDMMC_INT_RXDR | SDMMC_INT_TXDR);
	mci_writel(INTMASK, temp);
}

void dw_mci_tmout(u32 tmout)
{
	mci_writel(TMOUT, tmout);
}

void dw_mci_bytcnt(u32 byte_cnt)
{
	mci_writel(BYTCNT, byte_cnt);
}

void dw_mci_blksz(u32 blksz)
{
	mci_writel(BLKSIZ, blksz);
}

int dw_mci_card_busy(void)
{
	u32 status;

	/*
	 * Check the busy bit which is low when DAT[3:0]
	 * (the data lines) are 0000
	 */
	status = mci_readl(STATUS);

	return !!(status & SDMMC_STATUS_BUSY);
}

void dw_mci_reset(void)
{
	int reset;

	reset = mci_readl(RST_N);
	reset &= ~(SDMMC_RST_HWACTIVE);
	mci_writel(RST_N, reset);
	delayms(1);
	reset |= SDMMC_RST_HWACTIVE;
	mci_writel(RST_N, reset);
	delayms(1);
}

int dw_mci_interrupt(struct mmc_command *cmd)
{
	u32 pending;

	pending = mci_readl(MINTSTS); /* read-only mask reg */
	mci_writel(RINTSTS, pending & 0xffff);
	if (pending & DW_MCI_ERROR_FLAGS)
		return false;
	if (pending & SDMMC_INT_CMD_DONE) {
		/* Read the response from the card (up to 16 bytes) */
		if (cmd->flags & MMC_RSP_PRESENT) {
			if (cmd->flags & MMC_RSP_136) {
				cmd->resp[3] = mci_readl(RESP0);
				cmd->resp[2] = mci_readl(RESP1);
				cmd->resp[1] = mci_readl(RESP2);
				cmd->resp[0] = mci_readl(RESP3);
			} else {
				cmd->resp[0] = mci_readl(RESP0);
				cmd->resp[1] = 0;
				cmd->resp[2] = 0;
				cmd->resp[3] = 0;
			}
		}
	}

	pending = mci_readl(IDSTS);
	if (pending & (SDMMC_IDMAC_INT_TI | SDMMC_IDMAC_INT_RI)) {
		mci_writel(IDSTS, SDMMC_IDMAC_INT_TI |
				SDMMC_IDMAC_INT_RI);
		mci_writel(IDSTS, SDMMC_IDMAC_INT_NI);
	}

	return true;
}

void dw_mci_disable_clk(void)
{
	/* disable clock */
	mci_writel(CLKENA, 0);
	mci_writel(CLKSRC, 0);

}

void dw_mci_div(u32 div)
{
	/* set clock to desired speed */
	mci_writel(CLKDIV, div);

}

void dw_mci_enable_clk(void)
{
	u32 clk_en_a;
	/* enable clock; only low power if no SDIO */
	clk_en_a = SDMMC_CLKEN_ENABLE << 0;
	clk_en_a |= SDMMC_CLKEN_LOW_PWR << 0;
	mci_writel(CLKENA, clk_en_a);
}

void dw_mci_slot_bus(u32 ctype)
{
	/* Set the current slot bus width */
	mci_writel(CTYPE, (ctype << 0));
}

void dw_mci_sdr_mode_set(void)
{
	u32 regs;

	regs = mci_readl(UHS_REG);

	/* SDR mode set */
	regs &= ~((0x1 << 0) << 16);

	mci_writel(UHS_REG, regs);

	regs = mci_readl(PWREN);
	regs |= (1 << 0);
	mci_writel(PWREN, regs);
}

int wait_dll_ready(void)
{
	u32 t1, t2;
	u32 dll_status_s0, dll_status_d0;

	t1 = get_sys_timer_val();
	while (1) {
		dll_status_s0 = __raw_readl(sdio_dllout_status_reg_spl);//a8
		dll_status_d0 = __raw_readl(sdio_dllout_status_reg_drv);//b4
		if ((dll_status_s0 & BIT(0)) && (dll_status_d0 & BIT(0)))
			break;
		t2 = get_sys_timer_val();
		if ((t2 - t1) > WAIT_TIMEOUT_MS)
			return false;
	}

	if ((dll_status_s0 & 0x6) == 0x6)
		return false;
	if ((dll_status_d0 & 0x6) == 0x6)
		return false;

	return true;
}

int sdio_cfg_init(void)
{
	bool ret = 0;

	//ret = sdio_sampling_select();
	if(ret){
		__raw_writel(0x41, reg_sdio_ctrl0);
	}else{
		__raw_writel(0x40, reg_sdio_ctrl0);
	}

	__raw_writel(SDIO_SPL1_VALUE, reg_sdio_spl1);

	return true;
}

int mci_init(void)
{
	u32 fifo_size, fifoth_val;
	/* Clear the interrupts for the host controller */
	mci_writel(RINTSTS, 0xFFFFFFFF);
	mci_writel(INTMASK, 0); /* disable all mmc interrupt first */

	/* Put in max timeout */
	mci_writel(TMOUT, 0xFFFFFFFF);

	/*
	 * FIFO threshold settings  RxMark  = fifo_size / 2 - 1,
	 *			    Tx Mark = fifo_size / 2 DMA Size = 8
	 */
	fifo_size = mci_readl(FIFOTH);
	fifo_size = 1 + ((fifo_size >> 16) & 0xfff);
	fifoth_val =
		SDMMC_SET_FIFOTH(0x2, fifo_size / 2 - 1, fifo_size / 2);
	mci_writel(FIFOTH, fifoth_val);

	/* disable clock to CIU */
	mci_writel(CLKENA, 0);
	mci_writel(CLKSRC, 0);

	/*
	 * Enable interrupts for command done, data over, data empty,
	 * receive ready and error such as transmit, receive timeout, crc error
	 */
	mci_writel(INTMASK, SDMMC_INT_CMD_DONE | SDMMC_INT_DATA_OVER |
			SDMMC_INT_TXDR | SDMMC_INT_RXDR |
			DW_MCI_ERROR_FLAGS);
	/* Enable mci interrupt */
	mci_writel(CTRL, SDMMC_CTRL_INT_ENABLE);

	return 0;
}

