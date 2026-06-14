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
#include <mach/sd_host.h>
#include <mach/sd_card.h>

static u32 csd_struct = 0;
static const unsigned int freqs[] = { 400000, 300000, 200000, 100000 };

int sdio_init(void *data)
{
	u32 i;
	int err = false;
	u32 rca = 0;
	u32 csd[4] = {0};
	u32 *boot_mode = data;

	mci_selection(*boot_mode);
	err = dw_mci_init();
	if (err == false) {
		return false;
	}

	for (i = 0; i < ARRAY_SIZE(freqs); i++) {
		dw_mci_hw_reset();
		dw_mci_set_ios(freqs[i], MMC_BUS_WIDTH_1);
		err = init_sdcard(&rca, csd);
		if (err == true)
			break;
	}
	if (err == false) {
		return false;
	}

	csd_struct = unstuff_bits(csd, 126, 2);
	dw_mci_set_ios(TRANS_DATA_FREQS, MMC_BUS_WIDTH_1);

	return true;
}

int sdio_write(unsigned int offset, unsigned char *buf, unsigned int size)
{
	int err;
	unsigned int write_len = size;

	if((offset & 0x1ff) || (((unsigned int)buf) & 0x3)){
		printf("Address not aligned!\n");
		return false;
	}

	if (csd_struct)
		offset >>= 9;

	write_len = ALIGN(write_len, 512);
	err = mmc_blk_mq_issue_rw_rq(WRITE, (u32)buf, (u32)offset, write_len);

	return err;
}

int sdio_read(unsigned int offset, unsigned char *buf, unsigned int size)
{
	int err;
	unsigned int read_len = size;

	if((offset & 0x1ff) || (((unsigned int)buf) & 0x3)){
		printf("Address not aligned!\n");
		return false;
	}

	if (csd_struct)
		offset >>= 9;

	read_len = ALIGN(read_len, 512);
	err = mmc_blk_mq_issue_rw_rq(READ, (u32)buf, (u32)offset, read_len);

	return err;
}

int spl_sdio0_load_image(struct spl_image_info *spl_image)
{
	int err;
	u32 boot_mode = BOOT_DEVICE_SDIO0;
	err = sdio_init(&boot_mode);
	if(err == false){
		printf("sdio0 init fail!!!\n");
		return 1;
	}

	err = sdio_read(0x2000, (unsigned char*)UBOOT_LOAD_ADDR, UBOOT_MAX_SIZE);
	if(err == false){
		return 1;
	}

	return 0;
}

int spl_sdio1_load_image(struct spl_image_info *spl_image)
{
	int err;
	u32 boot_mode = BOOT_DEVICE_SDIO1;
	err = sdio_init(&boot_mode);
	if(err == false){
		printf("sdio1 init fail!!!\n");
		return 1;
	}

	err = sdio_read(0x2000, (unsigned char*)UBOOT_LOAD_ADDR, UBOOT_MAX_SIZE);
	if(err == false){
		return 1;
	}

	return 0;
}

int sdl_sdio1_program_image(u32 dest, u32 len, u8 *addr)
{
	int err;
	u32 boot_mode = BOOT_DEVICE_SDIO1;

	err = sdio_init(&boot_mode);
	if(err == false){
		printf("sdio1 init fail!!!\n");
		return 1;
	}

	err = sdio_write(dest, addr, len);
	if(err == true){
		return 0;
	} else {
		return 1;
	}
}

