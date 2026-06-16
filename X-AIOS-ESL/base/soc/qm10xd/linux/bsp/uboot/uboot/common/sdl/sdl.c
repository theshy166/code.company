/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/clk.h>
#include <mach/spl.h>
#include <mach/uart_drv.h>
#include <mach/uart.h>
#include <mach/spic.h>
#include <mach/sd_card.h>
#include <mach/norflash.h>
#include <mach/nandflash.h>
#include <mach/download.h>
#include <mach/board.h>
#include <mach/dma.h>
#include <mach/ddr.h>
#include <nand.h>
#include "../../board/molchip/board/system.h"
#ifdef CONFIG_SDL_MMC_SUPPORT
#include "mach/sd_mmc.h"
#endif
#include <mach/usb_boot.h>


#ifdef CONFIG_ARM64
void hang(void)
{
	while(1);
}
#endif

void s_init(void)
{
        //timer clk init
        molchip_systimer_clk_enable();

        //uart0 clk init
        molchip_uart_clk_enable(REG_UART0_BASE);

        //uart1 clk init
        molchip_uart_clk_enable(REG_UART1_BASE);

	//spic clk enable
	molchip_spic_clk_enable();

#ifndef CONFIG_TARGET_TS01
	//dma clk enable
	molchip_dma_clk_enable();
#endif
}
/*    This function prepares the hardware for
 *    execution from system RAM (DRAM, DDR...) As system RAM may not
 *    be available yet, , board_init_f() must use the current GD to
 *    store any data which must be passed on to later stages. These
 *    data include the relocation destination, the future stack, and
 *    the future GD location.
 *
 *    bss data not init
 */
void board_init_f(ulong dummy)
{
        /* CPU/BUS CLK config if needed*/

		/* hardware init */
        uart_hw_init();

     	board_spic_init();
}

#ifdef CONFIG_SDL_NAND_SUPPORT
int spi_nand_program_image(u32 dest, u32 len, u8 *buf)
{
	nand_erase_options_t opts;
	int ret;
	struct mtd_info *mtd;
	u32 actual;
	loff_t lim;
	struct spi_nand_info *nand_info;
	struct nand_chip *chip ;
	u32 length = (64<<11);
	u8 *buf_tmp;

	buf_tmp = buf + 0x500000;

	mtd = get_nand_dev_by_index(0);
	chip = mtd_to_nand(mtd);

	if(len <length){
		length = mtd->erasesize ;
		memset(buf_tmp, 0xff, length);
		memcpy(buf_tmp, buf, len);
	}
	else{
		if(dest == 0){
			length = mtd->erasesize + UBOOT_MAX_SIZE;
			memset(buf_tmp, 0xff, length);
			memcpy(buf_tmp, buf, (32<<10));
			memcpy(buf_tmp + mtd->erasesize, buf+(32<<10), len-(32<<10));
		}
		else{
			memset(buf_tmp, 0xff, len);
			memcpy(buf_tmp, buf, len);
			length = len;
		}
	}

	if(dest == 0)
	{
		nand_info = (struct spi_nand_info *)(buf_tmp + NAND_INFO_OFFSET);
		memset(nand_info, 0,sizeof(struct spi_nand_info));
		nfc_fb_incdec_mode(FB_INCDEC_WRITE);
		sfc_write_data((u8 *)buf_tmp, 512);
		spi_nand_calculate_ecc(512);
		sfc_read_ecc(nand_info->ecc, 44);
		nand_info->main_size = 2048;
		if(mtd->writesize == 4096)
			nand_info->oob_size = mtd->oobsize/2;
		else
			nand_info->oob_size = mtd->oobsize;
		nand_info->sector_num = nand_info->main_size >> 10;
		nand_info->block_page = mtd->erasesize / mtd->writesize;
		if (chip->options & SPINAND_ECC_MODE_24_bit) {
			nand_info->ecc_mode = 1;
			nand_info->ecc_pos = nand_info->oob_size / nand_info->sector_num - 42;
			nand_info->spare_size = nand_info->ecc_pos;
		} else {
			nand_info->ecc_mode = 0;
			nand_info->ecc_pos = nand_info->oob_size / nand_info->sector_num - 14;
			nand_info->spare_size = nand_info->ecc_pos;
		}

	//	memcpy(buf_tmp + mtd->erasesize, buf_tmp, (32<<10));
	}
	//spi_nand_write_feature(REG_BLOCK_LOCK, 0);
	lim = chip->chipsize;
	memset(&opts, 0, sizeof(opts));
	opts.offset = dest;
	opts.length = length;
	opts.quiet = 1;
	ret = nand_erase_opts(mtd, &opts);
	if (ret){
		return ret;
	}

	if((mtd->writesize == 4096) && (dest == 0)){
		int temp_len = 32*1024;
		ret = nand_write_skip_bad(mtd, dest, (size_t *)&temp_len, &actual,
				  lim, buf_tmp, WITH_WR_VERIFY);
		if(ret){
			return ret;
		}

		temp_len = length - mtd->erasesize;

		ret = nand_write_skip_bad(mtd, dest+256*1024, (size_t *)&temp_len, &actual,
			lim, (buf_tmp + 256*1024) , WITH_WR_VERIFY);
		if(ret){
			return ret;
		}

	}
	else{
		ret = nand_write_skip_bad(mtd, dest, (size_t *)&length, &actual,
				lim, buf_tmp, WITH_WR_VERIFY);
		if(ret){
			return ret;
		}
	}
	return 0;

}
#endif

void board_init_r(gd_t *gd, ulong dest_addr)
{
	u32 uart_port = 0;
	u32 nvm_mod;
	struct down_info *d_info;
	u32 ram_downaddr,nvm_downaddr;
	u32 i=0;
	int ret = 0;
	unsigned int strap_sts;

	strap_sts = get_strap_status();

	nvm_mod = nvm_boot_device();
	timer_enable();
	mem_malloc_init(SPL_BASE_ADDR, (64<<10));
	gd->flags |= GD_FLG_FULL_MALLOC_INIT;

#ifdef CONFIG_SDL_NAND_SUPPORT
		nand_init();
#endif

	if(strap_sts & 0x1){ //uart download
		uart_port = uart_identify();
		uart_preboot(uart_port);
	}else{
		usb_identify();
		usb_preboot();
	}

	while(1)
	{
		if(strap_sts & 0x1){ //uart download
			uart_boot(uart_port);
		}
		else{
			usb_boot();
			if(strap_sts & 0x2) //boot1 0: nor 1:nand
				nvm_mod = BOOT_DEVICE_NAND ;
			else
				nvm_mod = BOOT_DEVICE_NOR;
		}

		d_info = get_cur_downinfo();

		switch(nvm_mod)
		{
			case BOOT_DEVICE_NAND:
			#ifdef CONFIG_SDL_NAND_SUPPORT
				ram_downaddr = get_downmap_info(d_info,IMGRAMADDR_CMD);
				nvm_downaddr = get_downmap_info(d_info,IMG_NANDADDR_CMD);

				ret = spi_nand_program_image(nvm_downaddr,d_info->total_len,(u8*)(unsigned long)ram_downaddr);
				//mark,just for debug
				*((volatile u32 *)(unsigned long)(0x47FFFF00+i * 4)) = d_info->imageid | (i<< 16);
				*((volatile u32 *)(unsigned long)(0x47FFFF20+i * 4)) = nvm_downaddr;
				*((volatile u32 *)(unsigned long)(0x47FFFF40+i * 4)) = d_info->total_len;
				*((volatile u32 *)(unsigned long)(0x47FFFF60+i * 4)) = ram_downaddr;
				i++;
			#endif
				break;
			case BOOT_DEVICE_NOR:
				ram_downaddr = get_downmap_info(d_info,IMGRAMADDR_CMD);
				nvm_downaddr = get_downmap_info(d_info,IMG_NORADDR_CMD);
	
				ret = spi_nor_program_image(nvm_downaddr,d_info->total_len,(u8*)(unsigned long)ram_downaddr);

				//mark,just for debug
				*((volatile u32 *)(unsigned long)(0x47FFFF00+i * 4)) = d_info->imageid | (i<< 16);
				*((volatile u32 *)(unsigned long)(0x47FFFF20+i * 4)) = nvm_downaddr;
				*((volatile u32 *)(unsigned long)(0x47FFFF40+i * 4)) = d_info->total_len;
				*((volatile u32 *)(unsigned long)(0x47FFFF60+i * 4)) = ram_downaddr;
				i++;
				break;
			case BOOT_DEVICE_SDIO1:
				ram_downaddr = get_downmap_info(d_info,IMGRAMADDR_CMD);
				nvm_downaddr = get_downmap_info(d_info,IMG_SDIO1ADDR_CMD);
			
				ret = sdl_sdio1_program_image(nvm_downaddr,d_info->total_len,(u8*)(unsigned long)ram_downaddr);

				//mark,just for debug
				*((volatile u32 *)(unsigned long)(0x47FFFF00+i * 4)) = d_info->imageid | (i<< 16);
				*((volatile u32 *)(unsigned long)(0x47FFFF20+i * 4)) = nvm_downaddr;
				*((volatile u32 *)(unsigned long)(0x47FFFF40+i * 4)) = d_info->total_len;
				*((volatile u32 *)(unsigned long)(0x47FFFF60+i * 4)) = ram_downaddr;
				i++;
				break;
		}

        d_info->comp_status = ret;
	}
}

