/*
 * (C) Copyright 2020 Molchip technology
 *
 * SD_BOOT
 *
 */

#include <common.h>
#include <mach/hardware.h>
#include <mach/spl.h>
#include <mach/systimer.h>
#include <mach/uart_drv.h>
#include "mach/sd_mmc.h"
#include "../board/system.h"

int block_cmd_data(u32 addr, u32 size);

const unsigned freqs[4] = { 400000, 300000, 200000, 100000 };
unsigned int clk_emmc_2x[4] = {24000000, 200000000, 297000000, 400000000};
u8 ext_csd[512] = {0};
u8 bw_ext_csd[512] = {0};
#if 0

int emmc_go_idle(void)
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

int emmc_go_pre_idle(void)
{
	int err;
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_GO_IDLE_STATE;
	cmd.arg = 0xf0f0f0f0;
	cmd.flags = MMC_RSP_NONE | MMC_CMD_BC;

	err = mmc_start_request(&cmd);

	return err;
}

int emmc_send_op_cond(u32 ocr, u32 *rocr)
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

int emmc_send_cxd_native(u32 arg, u32 *cxd, int opcode)
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

int emmc_send_cid(u32 *cid)
{
	return emmc_send_cxd_native(0, cid, MMC_ALL_SEND_CID);
}

int emmc_send_relative_addr(unsigned int *rca)
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

int emmc_send_csd(u32 rca, u32 *csd)
{

	return emmc_send_cxd_native(rca << 16,	csd,
				MMC_SEND_CSD);
}

int emmc_select_card(u32 rca)
{
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_SELECT_CARD;

	cmd.arg = rca << 16;
	cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;

	return mmc_start_request(&cmd);
}
#endif

/*
 * NOTE: void *buf, caller for the buf is required to use DMA-capable
 * buffer or on-stack buffer (with some overhead in callee).
 */
int
emmc_send_cxd_data(	u32 opcode, void *buf, unsigned len)
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
	sdhci_set_adma_addr((u32)(unsigned long)buf);

	return mmc_start_request(&cmd);

}

int emmc_get_ext_csd(u8 **new_ext_csd)
{
	int err;

	if (!new_ext_csd)
		return false;

	err = emmc_send_cxd_data(MMC_SEND_EXT_CSD, new_ext_csd,
				512);
	if (err == false)
		return err;
	err = block_cmd_data((u32)new_ext_csd, 512);
	return err;
}

int emmc_switch(u8 set, u8 index, u8 value)
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

int emmc_blk_mq_issue_rw_rq(u32 req, u32 addr, u32 sector, u32 len)
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
		//stop.opcode = MMC_STOP_TRANSMISSION;
		readcmd = MMC_READ_MULTIPLE_BLOCK;
		writecmd = MMC_WRITE_MULTIPLE_BLOCK;
	} else {
		readcmd = MMC_READ_SINGLE_BLOCK;
		writecmd = MMC_WRITE_BLOCK;
	}

	if (rq_data_dir(req) == READ) {
		data.flags = MMC_DATA_READ;
		cmd.opcode = readcmd;
		//stop.flags = MMC_RSP_R1 | MMC_CMD_AC;
	} else {
		data.flags = MMC_DATA_WRITE;
		cmd.opcode = writecmd;
		//stop.flags = MMC_RSP_R1B | MMC_CMD_AC;
	}

	err = mmc_start_request(&cmd);
	if (err == 1)
		return 1;

	if (stop.opcode)
		err =  mmc_start_request(&stop);

	return err;
}
#if 0
int init_emmc(u32 *rca, u32 *csd)
{
	int err;
	u32 ocr, ocr_avail;
	u32 cid[4] = {0};

	ocr_avail = MMC_VDD_165_195;

	emmc_go_idle();
	err = emmc_send_op_cond(0, &ocr);
	if (err)
		return false;
	if(!(ocr & ocr_avail))
		return false;

	emmc_go_idle();

	err = emmc_send_op_cond(ocr | (1 << 30), &ocr);
	if (err)
		return false;
	err = emmc_send_cid(cid);
	if(err)
		return false;
	err = emmc_send_relative_addr(rca);
	if (err)
		return false;
	err = emmc_send_csd(*rca, csd);
	if (err)
		return false;
	err = emmc_select_card(*rca);
	if (err)
		return false;

	return true;
}
#else
int (*init_emmc)(u32 *, u32 *) = (int (*)(u32 *, u32 *))(0x8d3c);
#endif
void get_rom_version(void)
{
	u32 rom_id;

	rom_id = ((*(volatile unsigned int*)(0x188))>>16)&0xf;
	if(rom_id)
		init_emmc = (int (*)(u32 *, u32 *))(0x8e70);
	else
		init_emmc = (int (*)(u32 *, u32 *))(0x8d3c);
}

int mmc_send_status(int rca, int timeout)
{
	int err, retries = 5;
	struct mmc_command cmd = {0};

	cmd.opcode = MMC_SEND_STATUS;
	cmd.arg = rca << 16;
	cmd.flags = MMC_RSP_R1;
	while (1) {
		err = mmc_start_request(&cmd);
		if (!err) {
			if ((cmd.resp[0] & MMC_STATUS_RDY_FOR_DATA) &&
			    (cmd.resp[0] & MMC_STATUS_CURR_STATE) !=
			     MMC_STATE_PRG)
				break;
			else if (cmd.resp[0] & MMC_STATUS_MASK) {
				return false;
			}
		} else if (--retries < 0)
			return err;
		if (timeout-- <= 0)
			break;
		delayms(1);
	}
	if (timeout <= 0) {
		return false;
	}
	return true;
}

int emmc_switch_part(unsigned int part_num)
{
	int ret;

	ret = emmc_switch(EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONFIG,
			(part_num & EXT_CSD_PART_CONFIG_ACC_MASK));

	return ret;
}

int switch_hsxx_mode(u32 rca, u32 clock)
{
	int err;
	u16 tmp;
	int timing;

	emmc_clk_div(400000);

	err = emmc_switch(EXT_CSD_CMD_SET_NORMAL, 
			EXT_CSD_HS_TIMING, EXT_CSD_HI_SPD);
	if (err == false)
		goto ERR;
	
	err = emmc_switch(EXT_CSD_CMD_SET_NORMAL, 
			EXT_CSD_BUS_WIDTH, EXT_CSD_BUS_WIDTH_8_DUAL_DS);
	if (err == false)
		goto ERR;
	
	timing = EXT_CSD_SPD_HS400 |
	     1 << EXT_CSD_DRV_STR_SHIFT;
	err = emmc_switch(EXT_CSD_CMD_SET_NORMAL, 
			EXT_CSD_HS_TIMING, timing);
	if (err == false)
		goto ERR;

	sdhci_set_bus_width(MMC_BUS_WIDTH_8);

	tmp = sdhci_readb(SDHCI_HOST_CONTROL);
	tmp |= (SDHCI_CTRL_HISPD);
	sdhci_writeb(tmp , SDHCI_HOST_CONTROL);

	tmp = sdhci_readw(SDHCI_HOST_CONTROL2);
	tmp |= (SDHCI_CTRL_HS400);
	sdhci_writew(tmp , SDHCI_HOST_CONTROL2);
	mc_phy_set_ios_post(1);
	emmc_clk_div(clock);

	err = mmc_send_status(rca,1000);
	if (err == false)
		goto ERR;
	err = emmc_get_ext_csd((u8 **)&bw_ext_csd);
	if (err == false) {
		uart_puts("emmc get cmd8 error!\n");
		return false;
	}
	err = !((ext_csd[EXT_CSD_PARTITION_SUPPORT] ==
			bw_ext_csd[EXT_CSD_PARTITION_SUPPORT]) &&
		(ext_csd[EXT_CSD_ERASED_MEM_CONT] ==
			bw_ext_csd[EXT_CSD_ERASED_MEM_CONT]) &&
		(ext_csd[EXT_CSD_REV] ==
			bw_ext_csd[EXT_CSD_REV]) &&
		(ext_csd[EXT_CSD_STRUCTURE] ==
			bw_ext_csd[EXT_CSD_STRUCTURE]) &&
		(ext_csd[EXT_CSD_CARD_TYPE] ==
			bw_ext_csd[EXT_CSD_CARD_TYPE]));
	if (err)
		goto ERR;

	return true;

ERR:
	return false;
}

int  sdhci_data_tran_done(u32 addr)
{
	u32 val;
	u32 start_addr;
	val = sdhci_readl(SDHCI_INT_STATUS);
	if(val & SDHCI_INT_ERROR){
		return false;
	}
	if (val & SDHCI_INT_RESPONSE) {
		if(val & SDHCI_INT_DATA_END) {
			sdhci_writel(val, SDHCI_INT_STATUS);
			return true;
		}  else {
			if(val & SDHCI_INT_DMA_END){
				start_addr = sdhci_readl(SDHCI_ADMA_ADDRESS);
				sdhci_writel(SDHCI_INT_DMA_END, SDHCI_INT_STATUS);
				if (!(start_addr & (EMMC_SDMA_SIZE_LIMIT - 1)))
					start_addr = addr;
				sdhci_writel(start_addr, SDHCI_ADMA_ADDRESS);
			}
		}
	}
	return false;
}

int block_cmd_data(u32 addr, u32 size)
{
	int ret = false;
	u32 cross, mask;
	if (size > EMMC_SDMA_SIZE_LIMIT) {
		uart_puts("size not support\n");
		hang();
	}
	mask = (EMMC_SDMA_SIZE_LIMIT - 1);
	cross = (addr & mask) + size;
	if (cross > EMMC_SDMA_SIZE_LIMIT)
		addr = (addr + size) & ~mask;
	do {
		ret = sdhci_data_tran_done(addr);
		if (ret == true)
			break;
	}while(1);
	return ret;
}

int mmc_init_card(void)
{
	u32 i;
	u32 rca = 1;
	u32 csd[4] = {0};
	int err = false;

	mmc_clk_set();
	mmc_phy_init();
	emmc_tm_clk_set();
	conflict_disable();


	for (i = 0; i < ARRAY_SIZE(freqs); i++) {
		sdhci_init(0, freqs[i]);
		get_rom_version();
		err = init_emmc(&rca, csd);
		if(err == true)
			break;
	}
	if(err == false) {
		uart_puts("emmc init fail!!!\n");
		return false;
	}

	sdhci_set_ios(TRANS_DATA_FREQS);
	err = emmc_get_ext_csd((u8 **)&ext_csd);
	if (err == false) {
		uart_puts("emmc read cmd8 error\n");
		return false;
	}
	err = switch_hsxx_mode(rca, HS400_DATA_FREQS);
	if (err == false) {
		uart_puts("emmc switch hs mode error\n");
		return false;
	}
	return err;
}

int emmc_switch_part_acc(u8 acc)
{
	u8 boot_partition;
	int err = false;
	boot_partition = ext_csd[EXT_CSD_PART_CONFIG];
	boot_partition &= EXT_CSD_PART_CONFIG_BOOT_MASK;
	boot_partition |= (acc & EXT_CSD_PART_CONFIG_ACC_MASK);

	err = emmc_switch(EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_PART_CONFIG, boot_partition);

	return err;

}

void pre_sched_boot(struct boot_header *header_info)
{
	u32 addr, len, granu;
	u32 i, cnt;
	addr = header_info->aes_src;;
	len = header_info->decode_length;
	granu = header_info->granularity;
	
	cnt = len / granu;
	for (i = 0; i <= cnt; i++) {
		
		writel(ENTRY_BAD_ADDRESS, addr);
		addr += granu;
	}
}

int sched_emmc_load_image(struct boot_image_info * info)
{
	int err = false;
	u32 size, load_address;
	u32 sector;
	struct boot_header *header_info;
	u8 acc = 0;
/**
Information Definition
*/

	if (!(info->flag & BIT_HEADER)) {		
		uart_puts("invalid image\n");
		return false;
	}

	if (info->name == pre_rolling || info->name == rtt) {
		acc = EXT_CSD_PART_CONFIG_ACC_BOOT1;
	}
	err = emmc_switch_part_acc(acc);
	if (err == false)
		return false;

	size = ALIGN(info->size, 512);
	sector = ALIGN(info->nvm_base, 512) >> 9;
	load_address = info->mem_base - HEADER_LEN;

	err = emmc_blk_mq_issue_rw_rq(READ, load_address, sector, HEADER_LEN);
	if(err == false) {
		uart_puts("emmc_blk_mq rq error!\n");
		return false;
	}
	err = block_cmd_data(load_address, HEADER_LEN);
	if(err == false) {
		uart_puts("mmc read header error!!!\n");
		return false;
	}

	header_info = (struct boot_header *)(load_address);
	load_address = load_address + HEADER_LEN;
	sector = sector + (HEADER_LEN >> 9);
	if (size > HEADER_LEN) {
		size = size - HEADER_LEN;
	} else {
		uart_puts("size too short!\n");
		return false;
	}
	
	if (header_info->cpu_sched) {
		pre_sched_boot(header_info);
		load_address = header_info->aes_src;
		size = ALIGN(header_info->decode_length, 512);
	}
	
	err = emmc_blk_mq_issue_rw_rq(READ, load_address, sector, size);
	if(err == false) {
		uart_puts("emmc_blk_mq rq error!\n");
		return false;
	}

	if (header_info->cpu_sched) {
		err = spi_cpu_sched_boot(header_info);
	} else {
		err = block_cmd_data(load_address, size);
	}
	if(err == false) {
		uart_puts("mmc read data error!!!\n");
		return false;
	}

	if(info->type == LOAD_nIMAGE)
	{
		memcpy((unsigned int *)header_info->image_dst, (unsigned int *)header_info->image_src, header_info->image_len);
	}

	return true;
}

