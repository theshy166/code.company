#include <mach/sd_reg.h>
#include <mach/sd_host.h>

u32 descriptor[DW_MCI_DESC_RING_NUM << 2] = {0}; //max 64k

void mci_selection(u32 boot_mode)
{
	mci_addr_selection(boot_mode);
	mci_drive_strength(boot_mode);
	mci_pull_up(boot_mode);
	mci_io_mux(boot_mode);
}

int dw_mci_start_command(struct mmc_command *cmd, u32 cmd_flags)
{
	int err, done;

	dw_mci_cmdarg(cmd->arg);
	err = dw_mci_wait_while_busy(cmd_flags);
	if (err == false)
		return false;

	dw_mci_cmd_start(cmd_flags | SDMMC_CMD_START);

	done = SDMMC_INT_CMD_DONE;
	if (cmd->data)
		done |= SDMMC_INT_DATA_OVER;
	err = wait_cmd_done(done);
	if (err == false)
		return false;

	err = dw_mci_interrupt(cmd);
	if (err == false)
		return false;

	return true;
}

u32 dw_mci_prepare_command(struct mmc_command *cmd)
{
	u32 cmdr;

	cmdr = cmd->opcode;

	if (cmd->opcode == MMC_STOP_TRANSMISSION ||
		cmd->opcode == MMC_GO_IDLE_STATE ||
		cmd->opcode == MMC_GO_INACTIVE_STATE)
		cmdr |= SDMMC_CMD_STOP;
	else if (cmd->opcode != MMC_SEND_STATUS && cmd->data)
		cmdr |= SDMMC_CMD_PRV_DAT_WAIT;

	if (cmd->flags & MMC_RSP_PRESENT) {
		/* We expect a response, so set this bit */
		cmdr |= SDMMC_CMD_RESP_EXP;
		if (cmd->flags & MMC_RSP_136)
			cmdr |= SDMMC_CMD_RESP_LONG;
	}

	if (cmd->flags & MMC_RSP_CRC)
		cmdr |= SDMMC_CMD_RESP_CRC;

	if (cmd->data) {
		cmdr |= SDMMC_CMD_DAT_EXP;
		if (cmd->data->flags & MMC_DATA_WRITE)
			cmdr |= SDMMC_CMD_DAT_WR;
	}
	cmdr |= SDMMC_CMD_USE_HOLD_REG;

	return cmdr;
}

void dw_mci_prepare_desc32(u32 mem_addr,
	 u32 length)
{
	unsigned int desc_len;
	struct idmac_desc *desc_first, *desc_last, *desc;

	desc_first = desc_last = desc = (struct idmac_desc *)descriptor;

	for (; length ; desc++) {
		desc_len = (length <= DW_MCI_DESC_DATA_LENGTH) ?
			length : DW_MCI_DESC_DATA_LENGTH;

		length -= desc_len;

		/*
		 * Set the OWN bit and disable interrupts
		 * for this descriptor
		 */
		desc->des0 = IDMAC_DES0_OWN | IDMAC_DES0_DIC | IDMAC_DES0_CH;

		/* Buffer length */
		IDMAC_SET_BUFFER1_SIZE(desc, desc_len);

		/* Physical address to DMA to/from */
		desc->des2 = mem_addr;
		/* Update physical address for the next desc */
		mem_addr += desc_len;

		/* Save pointer to the last descriptor */
		desc_last = desc;
	}

	/* Set first descriptor */
	desc_first->des0 |= IDMAC_DES0_FD;

	/* Set last descriptor */
	desc_last->des0 &= ~(IDMAC_DES0_CH | IDMAC_DES0_DIC);
	desc_last->des0 |= IDMAC_DES0_LD;

}

void dw_mci_idmac_start_dma(u32 addr, unsigned int len)
{

	dw_mci_prepare_desc32(addr, len);

	/* Make sure to reset DMA in case we did PIO before this */
	dw_mci_ctrl_reset(SDMMC_CTRL_DMA_RESET);
	dw_mci_idmac_reset();

	/* Select IDMAC interface */
	dw_mci_select_idmac_interface();


	/* Enable the IDMAC */
	dw_mci_deable_idmac();

	/* Start it running */
	dw_mci_start_run();
}

int dw_mci_submit_data_dma(u32 address, u32 length)
{
	/* Enable the DMA interface */
	dw_mci_enable_dma_interface();

	/* Disable RX/TX IRQs, let DMA handle it */
	dw_mci_disable_irq();

	dw_mci_idmac_start_dma(address, length);

	return true;
}

int dw_mci_start_request(struct mmc_command *cmd)
{
	u32 cmdflags;
	u32 byte_cnt;
	struct mmc_data *data;

	data = cmd->data;
	if (data) {
		byte_cnt = data->blksz * data->blocks;
		dw_mci_tmout(0xFFFFFFFF);
		dw_mci_bytcnt(byte_cnt);
		dw_mci_blksz(data->blksz);
		dw_mci_submit_data_dma(data->address, byte_cnt);
	}

	cmdflags = dw_mci_prepare_command(cmd);

	if (cmd->opcode == MMC_GO_IDLE_STATE)
		cmdflags |= SDMMC_CMD_INIT;

	return dw_mci_start_command(cmd, cmdflags);

}

void dw_mci_setup_bus(u32 clock, u32 ctype)
{
	u32 div;
	u32 sdmmc_cmd_bits = SDMMC_CMD_UPD_CLK | SDMMC_CMD_PRV_DAT_WAIT;

	div = BUS_HZ / clock;
	if (BUS_HZ % clock && BUS_HZ > clock)
		/*
		 * move the + 1 after the divide to prevent
		 * over-clocking the card.
		 */
		div += 1;

	div = (clock != BUS_HZ) ? DIV_ROUND_UP(div, 2) : 0;

	dw_mci_disable_clk();

	/* inform CIU */
	mci_send_cmd(sdmmc_cmd_bits, 0);

	dw_mci_div(div);

	/* inform CIU */
	mci_send_cmd(sdmmc_cmd_bits, 0);

	dw_mci_enable_clk();

	/* inform CIU */
	mci_send_cmd(sdmmc_cmd_bits, 0);

	dw_mci_slot_bus(ctype);

}

void dw_mci_set_ios(u32 clock, u32 bus_width)
{
	u32 ctype;

	switch (bus_width) {
	case MMC_BUS_WIDTH_4:
		ctype = SDMMC_CTYPE_4BIT;
		break;
	case MMC_BUS_WIDTH_8:
		ctype = SDMMC_CTYPE_8BIT;
		break;
	default:
		/* set default 1 bit mode */
		ctype = SDMMC_CTYPE_1BIT;
	}

	dw_mci_sdr_mode_set();

	/* Adjust clock / bus width after power is up */
	dw_mci_setup_bus(clock, ctype);
}

void dw_mci_hw_reset(void)
{


	dw_mci_idmac_reset();

	if (!dw_mci_ctrl_reset(SDMMC_CTRL_DMA_RESET |
				SDMMC_CTRL_FIFO_RESET))
		return;

	/*
	 * According to eMMC spec, card reset procedure:
	 * tRstW >= 1us:	RST_n pulse width
	 * tRSCA >= 200us: RST_n to Command time
	 * tRSTH >= 1us:	RST_n high period
	 */
	dw_mci_reset();
}

int dw_mci_init_dma(void)
{
	int i;
	struct idmac_desc *p, *desc;

	memset(descriptor, 0, sizeof(descriptor));
	desc = (struct idmac_desc *)descriptor;
	/* Forward link the descriptor list */
	for (i = 0, p = desc;
		i < DW_MCI_DESC_RING_NUM - 1;
		i++, p++) {
		p->des3 = (u32)(desc + (i + 1));
		p->des0 = 0;
		p->des1 = 0;
	}

	/* Set the last descriptor as the end-of-ring descriptor */
	p->des3 = (u32)desc;
	p->des0 = IDMAC_DES0_ER;

	dw_mci_idmac_reset();
	dw_mci_mask_interrupts();
	dw_mci_set_descriptor(descriptor);

	return true;

}

int dw_mci_init(void)
{
	mci_clk_enable();
	mci_ctl_reset();
	if (sdio_cfg_init() == false)
		return false;

	/* Reset all blocks */
	if (!dw_mci_ctrl_reset(SDMMC_CTRL_ALL_RESET_FLAGS))
		return false;

	dw_mci_init_dma();
	mci_init();

	return true;
}




