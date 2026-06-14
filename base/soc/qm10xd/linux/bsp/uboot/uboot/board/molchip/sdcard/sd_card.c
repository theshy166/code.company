/*
 * (C) Copyright 2020 Molchip technology
 *
 * SD_BOOT
 *
 */
#include <common.h>
#include <mach/systimer.h>
#include <mach/sd_host.h>
#include <mach/sd_card.h>

int unstuff_bits(u32 *resp, u32 start, u32 size)
{
	const u32 __mask = (size < 32 ? 1 << size : 0) - 1;
	const int __off = 3 - ((start) / 32);
	const int __shft = (start) & 31;
	u32 __res;

	__res = resp[__off] >> __shft;
	if (size + __shft > 32)
		__res |= resp[__off-1] << ((32 - __shft) % 32);
	__res &= __mask;

	return __res;
}

int mmc_go_idle(void)
{
	int err;
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_GO_IDLE_STATE;
	cmd.arg = 0;
	cmd.flags = MMC_RSP_NONE | MMC_CMD_BC;

	err = dw_mci_start_request(&cmd);

	delayms(1);

	return err;
}

int mmc_send_if_cond(u32 ocr)
{
	struct mmc_command cmd = {0};
	int err;
	const u8 test_pattern = 0xAA;
	u8 result_pattern;

	/*
	 * To support SD 2.0 cards, we must always invoke SD_SEND_IF_COND
	 * before SD_APP_OP_COND. This command will harmlessly fail for
	 * SD 1.0 cards.
	 */
	cmd.opcode = SD_SEND_IF_COND;
	cmd.arg = ((ocr & 0xFF8000) != 0) << 8 | test_pattern;
	cmd.flags = MMC_RSP_R7 | MMC_CMD_BCR;

	err = dw_mci_start_request(&cmd);
	if (err == false)
		return false;

	result_pattern = cmd.resp[0] & 0xFF;

	if (result_pattern != test_pattern)
		return false;

	return true;
}

int mmc_app_cmd(u32 rca)
{
	int err;
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_APP_CMD;

	cmd.arg = rca << 16;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

	err = dw_mci_start_request(&cmd);
	if (err == false)
		return false;

	/* Check that card supported application commands */
	if (!(cmd.resp[0] & R1_APP_CMD))
		return false;

	return true;
}

int mmc_wait_for_app_cmd(u32 rca,
			struct mmc_command *cmd, int retries)
{
	int i, err;

	if (retries < 0)
		retries = MMC_CMD_RETRIES;
	/*
	 * We have to resend MMC_APP_CMD for each attempt so
	 * we cannot use the retries field in mmc_command.
	 */
	for (i = 0; i <= retries; i++) {
		err = mmc_app_cmd(rca);
		if (err == false) {
			/* no point in retrying; no APP commands allowed */
			continue;
		}
		memset(cmd->resp, 0, sizeof(cmd->resp));
		cmd->data = NULL;
		err = dw_mci_start_request(cmd);
		if (err == true)
			break;
	}

	return err;
}

int mmc_send_app_op_cond(u32 ocr, u32 *rocr)
{
	struct mmc_command cmd = {0};
	int i, err = true;

	cmd.opcode = SD_APP_OP_COND;
	cmd.arg = ocr;
	cmd.flags = MMC_RSP_R3 | MMC_CMD_BCR;

	for (i = 500; i; i--) {
		err = mmc_wait_for_app_cmd((u32)NULL, &cmd, MMC_CMD_RETRIES);
		if (err == false)
			break;

		/* if we're just probing, do a single pass */
		if (ocr == 0)
			break;

		/* otherwise wait until reset completes */
		if (cmd.resp[0] & MMC_CARD_BUSY)
			break;

		delayms(1);
	}

	*rocr = cmd.resp[0];

	return err;
}

int mmc_send_cxd_native(u32 arg, u32 *cxd, int opcode)
{
	int err;
	struct mmc_command cmd = {0};

	cmd.opcode = opcode;
	cmd.arg = arg;
	cmd.flags = MMC_RSP_R2 | MMC_CMD_AC;

	err = dw_mci_start_request(&cmd);
	if (err == false)
		return false;

	memcpy(cxd, cmd.resp, sizeof(u32) * 4);

	return true;
}

int mmc_send_cid(u32 *cid)
{
	return mmc_send_cxd_native(0, cid, MMC_ALL_SEND_CID);
}

int mmc_send_relative_addr(unsigned int *rca)
{
	int err;
	struct mmc_command cmd = {0};

	cmd.opcode = SD_SEND_RELATIVE_ADDR;
	cmd.arg = 0;
	cmd.flags = MMC_RSP_R6 | MMC_CMD_BCR;

	err = dw_mci_start_request(&cmd);
	if (err == false)
		return false;

	*rca = cmd.resp[0] >> 16;

	return true;
}

int mmc_send_csd(u32 rca, u32 *csd)
{

	return mmc_send_cxd_native(rca << 16, csd,
		MMC_SEND_CSD);
}

int mmc_select_card(u32 rca)
{
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_SELECT_CARD;

	cmd.arg = rca << 16;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

	return dw_mci_start_request(&cmd);
}

int mmc_dw_set_blocklen(u32 rca, unsigned int blocklen)
{
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_SET_BLOCKLEN;
	cmd.arg = blocklen;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

	return dw_mci_start_request(&cmd);
}

int mmc_app_set_bus_width(u32 rca, int width)
{
	struct mmc_command cmd = {0};

	cmd.opcode = SD_APP_SET_BUS_WIDTH;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

	switch (width) {
	case MMC_BUS_WIDTH_1:
		cmd.arg = SD_BUS_WIDTH_1;
		break;
	case MMC_BUS_WIDTH_4:
		cmd.arg = SD_BUS_WIDTH_4;
		break;
	default:
		return -1;
	}

	return mmc_wait_for_app_cmd(rca, &cmd, MMC_CMD_RETRIES);
}

int mmc_blk_mq_issue_rw_rq(u32 req, u32 addr, u32 sector, u32 len)
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

	err = dw_mci_start_request(&cmd);
	if (err == false)
		return false;

	if (stop.opcode)
		err =  dw_mci_start_request(&stop);

	return err;
}

int init_sdcard(u32 *rca, u32 *csd)
{
	int err;
	u32 ocr, ocr_avail;
	u32 cid[4] = {0};

	ocr_avail = MMC_VDD_32_33 | MMC_VDD_33_34;

	mmc_go_idle();
	mmc_send_if_cond(ocr_avail);
	err = mmc_send_app_op_cond(0, &ocr);
	if (err == false)
		return false;
	if (!(ocr & ocr_avail))
		return false;

	mmc_go_idle();
	err = mmc_send_if_cond(ocr_avail);
	if (err == true)
		ocr |= SD_OCR_CCS;

	err = mmc_send_app_op_cond(ocr, &ocr);
	if (err == false)
		return false;
	err = mmc_send_cid(cid);
	if (err == false)
		return false;
	err = mmc_send_relative_addr(rca);
	if (err == false)
		return false;
	err = mmc_send_csd(*rca, csd);
	if (err == false)
		return false;
	err = mmc_select_card(*rca);
	if (err == false)
		return false;
	err = mmc_dw_set_blocklen(*rca, 512);
	if (err == false)
		return false;
	err = mmc_app_set_bus_width(*rca, MMC_BUS_WIDTH_1);
	if (err == false)
		return false;

	return true;
}

