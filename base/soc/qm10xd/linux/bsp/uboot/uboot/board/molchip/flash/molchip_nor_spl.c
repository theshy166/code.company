#include <common.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/spic.h>
#include <mach/uart_drv.h>
#include <mach/spl.h>
#include <mach/norflash.h>
#include <malloc.h>
#include "../board/system.h"
#include "../board/cpu_sched.h"
#include "../board/sched_internal.h"

struct norflash_info *info=NULL;

void spi_nor_set_winbond_adr_3mod(void);
void spi_enable_qe(void);
void spi_nor_set_ds_extern(void);
void spi_nor_set_ds_25m4ba_extern(void);
void spi_nor_set_gd256_extern(void);
void spi_nor_set_qe_enable_gd(void);
void spi_nor_set_status_fudan(void);

struct norflash_info spi_nor_ids[] = {
	{ "gd251q32", NULL, INFO(0xc86016, 0, 32 * 1024, 128)  },
	{ "gd25q64", spi_nor_set_qe_enable_gd, INFO(0xc84017, 0, 32 * 1024, 256) },
	{ "gd25q128", spi_nor_set_qe_enable_gd, INFO(0xc84018, 0, 32 * 1024, 512) },
	{ "gd25q256e", spi_nor_set_gd256_extern, INFO(0xc84019, 0, 32 * 1024, 1024) },
	{ "gd25lq128e", spi_enable_qe, INFO(0xc86018, 0, 32 * 1024, 512) },
	{ "gd25f128f", NULL, INFO(0xc84318, 0, 32 * 1024, 128) },
	{ "gd25f256f", spi_nor_set_gd256_extern, INFO(0xc84319, 0, 32 * 1024, 1024) },
	{ "n25q016a", NULL, INFO(0x20bb15, 0, 32 * 1024, 64)	},
	{ "n25q032", NULL,  INFO(0x20ba16, 0, 32 * 1024, 128)  },
	{ "w25q16dw", NULL, INFO(0xef6015, 0, 32 * 1024, 64)	},
	{ "w25q128jw", NULL, INFO(0xef4018, 0, 32 * 1024, 512) },
	{ "w25q128jwsq", NULL, INFO(0xef6018, 0, 32 * 1024, 512) },
	{ "w25q256jw", spi_nor_set_winbond_adr_3mod , INFO(SNOR_W25Q256_ID, 0, 32 * 1024, 1024) },
	{ "w25q16jw", NULL, INFO(0xef4015, 0, 32 * 1024, 64)	},
	{ "w25q16jw", NULL, INFO(0xef4014, 0, 32 * 1024, 64)	},
//	{ "mx25v1635f", NULL, INFO(0xc22315, 0, 32 * 1024, 64) },
	{ "mx25v1635f", NULL, INFO(0xc22315, 0, 32 * 1024, 64) },
	{ "mx25l12845g", NULL, INFO(0xc22018, 0, 32 * 1024, 512) },
	{ "mx25l25645g", NULL, INFO(0xc22019, 0, 32 * 1024, 1024) },
	{ "by25q128as", NULL, INFO(0x684018, 0, 32 * 1024, 512) },
	{ "xm25qh128a", NULL, INFO(0x207018, 0, 32 * 1024, 512)},
	{ "xm25qh128c", NULL, INFO(0x204018, 0, 32 * 1024, 512)},
	{ "xm25qu64a", NULL, INFO(0x207017, 0, 32 * 1024, 256)},
	{ "xm25qu64b", NULL, INFO(0x205017, 0, 32 * 1024, 256)},
	{ "xm25qu64c", NULL, INFO(0x204117, 0, 32 * 1024, 256)},
	{ "xm25qu64d", NULL, INFO(0x204017, 0, 32 * 1024, 256)},
	{ "gm25q128a", NULL, INFO(0x1c4018, 0, 32 * 1024, 512)},
	{ "s25fl256l", NULL, INFO(0x016019, 0, 32 * 1024, 1024) },
	{ "s25fl128l", NULL, INFO(0x016018, 0, 32 * 1024, 512) },
	{ "ds25m4ab", spi_nor_set_ds_extern, INFO(0xe54218, 0, 32 * 1024, 256) },
	{ "ds25m4ba", spi_nor_set_ds_25m4ba_extern, INFO(0xe54219, 0, 64 * 1024, 512) },
	{ "ds25q4aa", spi_nor_set_ds_extern, INFO(0xe53118, 0, 32*1024, 512 )},
	{ "en25qh128", NULL, INFO(0x1c7018, 0, 32 * 1024, 512) },
	{ "XM25QH256C", NULL, INFO(0x204019, 0, 32 * 1024, 512)},
	{ "FM25Q128AI3", spi_nor_set_status_fudan, INFO(0xa14018, 0, 32 * 1024, 512)},
};

int spi_nor_read_reg(u8 opcode, u8 *buf, int len)
{
	u32 ret;

	nfc_fb_incdec_mode(BUFFER_FLOW_READ);

	nfc_flowbuf_clr();
	nfc_all_clr();

	nfc_data_type(DATA_ONLY);
	nfc_set_page_size(len);
	nfc_cmd_type(0);
	nfc_data_length(len);
	nfc_cmd(opcode);
	nfc_set_tmod(1);
	nfc_set_fldform(0);
	nfc_set_frmform(1);
	nfc_set_phase_vld(NFC_DATA_PHASE_VLD | NFC_CMD_PHASE_VLD, NFC_ENABLE);
	nfc_read_en();

	sfc_access_start();
	ret = sfc_done(SPI_FINISH);
	if (ret == false)
		uart_puts("NRF\r\n"); /* nor flash regsiter read fail. */

	nfc_check_buf_p2_rdy();
	sfc_read_data(buf, len);
	write_nfc_soft_fb_p2_ack();
	
	return 0;
}

int spi_nor_write_reg(u8 opcode, u8 *buf)//06//c7
{
	nfc_flowbuf_clr();
	nfc_all_clr();
	sfc_cmd_write();
	nfc_cmd(opcode);
	sfc_data_type(DATA_NONE);
	nfc_set_tmod(TMOD_TRANSMIT_ONLY);//Transfer Mode,Transmit Only
	nfc_set_fldform(FLDFORM_ALL_SERIAL);//all Serial
	nfc_set_frmform(FRMFORM_OPCODE_ONLY);//opcode
	if(buf){
		sfc_write_reg(*buf);
		nfc_set_phase_vld(NFC_CMD_PHASE_VLD | NFC_DATA_PHASE_VLD | NFC_REG_PHASE_VLD, NFC_ENABLE);
	}else{
		nfc_set_phase_vld(NFC_CMD_PHASE_VLD, NFC_ENABLE);
	}
	nfc_set_cmd_wmode(WMODE_SINGLE);
	nfc_set_data_wmode(WMODE_SINGLE);
	nfc_set_addr_wmode(WMODE_SINGLE);
	sfc_access_start();


	sfc_done(SPI_FINISH);
	

	return 0;
}

bool spi_nor_wait_ready(void)
{
	u8 status = 0xff;
	u32 t1, t2;

	t1 = get_sys_timer_val();
	do {
		spi_nor_read_reg(SPINOR_OP_RDSR, &status, 1);
		if ((status & SR_WIP) == 0)
			return 0;
		t2 = get_sys_timer_val();
	} while (t2 - t1 < NOR_WAIT_READY_TIME);

	return 1;
}

bool spi_nor_erase_block(u32 opcode, u32 block)//0xd8//52/
{
	
	nfc_fb_incdec_mode(BUFFER_FLOW_WRITE);
	nfc_flowbuf_clr();
	nfc_all_clr();

	spic_addr(block, 3);
	nfc_cmd(opcode);
	nfc_set_tmod(1);//Transfer Mode,Transmit Only
	nfc_set_fldform(0);//all Serial
	nfc_set_frmform(4);//opcode + 3byte addr
	nfc_set_phase_vld(NFC_CMD_PHASE_VLD | NFC_ADDR_PHASE_VLD, NFC_ENABLE);

	sfc_cmd_write();
	sfc_access_start();
	sfc_done(SPI_FINISH);

	return 0;

}


void spi_nor_write_enable(void)
{
	spi_nor_write_reg(SPINOR_OP_WREN, NULL);

}

bool spi_nor_read_data(u32 from, u32 len,u8 *read_buf)
{
	u32 ret = true, count = 0;

	while (len) {
		nfc_fb_incdec_mode(BUFFER_FLOW_READ);
		nfc_flowbuf_clr();
		nfc_all_clr();

		if (len > SFC_BUF_SIZE) {
			count = SFC_BUF_SIZE;
			len -= SFC_BUF_SIZE;
		} else {
			count = len;
			len = 0;
		}

		nfc_data_type(DATA_ONLY);
		spic_addr(from, 3);
		nfc_set_page_size(count);
		nfc_cmd(SPINOR_OP_READ);//opcode
		nfc_data_length(count);

		nfc_set_tmod(1);//Transfer Mode,Transmit Only
		nfc_set_fldform(0);//Quad/Dual Data, other Serial
		nfc_set_frmform(4);//opcode + 3byte addr
		nfc_set_phase_vld(NFC_DATA_PHASE_VLD | NFC_CMD_PHASE_VLD |
			NFC_ADDR_PHASE_VLD, NFC_ENABLE);
		nfc_set_cmd_wmode(WMODE_SINGLE);
		nfc_set_data_wmode(WMODE_SINGLE);
		nfc_set_addr_wmode(WMODE_SINGLE);

		nfc_read_en();
		sfc_access_start();
		ret = sfc_done(SPI_FINISH);
		if(false == ret )
			break;
		nfc_check_buf_p2_rdy();
		sfc_read_data(read_buf, count);
		write_nfc_soft_fb_p2_ack();
		read_buf += count;
		from += count;
	}

	return ret;

}

int spi_transaction_dma_read(u32 from, u32 len, u8 *read_buf)
{
	u32 ret = true, count = 0;
	u32 cnt = 0;

	if (len % 0x1000 != 0)
		cnt = 1;
	cnt += len / 0x1000;


	spi_enable(SPI_DISABLE);
	spi_set_dummy_byte(4);

	nfc_dma_common_reg_set(NFC_ENABLE, NFC_RX_CH0_DMA_EN, 10,
		BUFFER_FLOW_READ);
	nfc_flowbuf_clr();
	nfc_dma_rx_ch0_reg_set(cnt, SFC_BUF_SIZE, 1);
	while (len) {
		if (len > SFC_BUF_SIZE) {
			count = SFC_BUF_SIZE;
			len -= SFC_BUF_SIZE;
		} else {
			count = len;
			len = 0;
		}
		nfc_cmd_mode_reg_set(1);
		nfc_cmd_set_reg_set(from, 1, count, SPINOR_OP_READ_1_1_4,
			count);
		nfc_dev_mode_reg_set(1, 1, 4, DATA_WMODE_2, ADDR_WMODE_0);
		sfc_dma_nor_read_data(read_buf, count);
		sfc_access_start();
		ret = sfc_done(SPI_FINISH);
		sfc_dma_wait_ready();
		from += count;
		read_buf += count;
	}

	sfc_dma_rx0_stop();
	nfc_dma_en(NFC_RX_CH0_DMA_EN, NFC_DISABLE);
	spi_enable(SPI_DISABLE);
	spi_set_dummy_byte(0);

	return ret;

}


#if SFC_CONFIG
int sfc_config(u32 from, u32 len, u8 *read_buf, struct boot_header *header)
{
	unsigned int w_req_num;

	if ((len & 0xFF) == 0)
		w_req_num = ((len & 0xFFFFFF00) >> 8);
	else
		w_req_num = ((len & 0xFFFFFF00) >> 8) + 1;

	nfc_dma_common_reg_set(NFC_ENABLE, NFC_RX_CH0_DMA_EN, 10,
		BUFFER_FLOW_READ);
	nfc_dma_rx_ch0_reg_set(w_req_num, W_PAGESIZE, 0);
	nfc_access_start_reg_set(0, 1);

	nfc_cmd_mode_reg_set(1);
	nfc_cmd_set_reg_set(from, w_req_num, W_PAGESIZE, SPINOR_OP_READ_1_1_4,
		W_PAGESIZE);
	nfc_dev_mode_reg_set(1, 1, 4, DATA_WMODE_2, ADDR_WMODE_0);
	sfc_dma_node_fill(read_buf);

//nfc_dma_common_reg_set(NFC_DISABLE, NFC_RX_CH0_DMA_EN, 10, BUFFER_FLOW_READ);
//nfc_dma_rx_ch0_reg_set(w_req_num, W_PAGESIZE, 0);
	return 0;
}
#endif

int spi_nor_write_data(u32 to, u32 len, u8 *write_buf)
{
	u32 count;
	flash_size = info->sector_size * info->n_sectors;
	while(len) {
		spi_nor_write_enable();
		nfc_fb_incdec_mode(BUFFER_FLOW_WRITE);
		nfc_flowbuf_clr();
		nfc_all_clr();

		if(len > 256) {
			len -= 256;
			count = 256;
		}
		else {
			count = len;
			len = 0;
		}
		nfc_data_type(DATA_ONLY);
		if(flash_size > FLASH_SIZE_16M){
			spi_set_4_bit_address_mode();
			spic_addr(to, BYTE_MODE_4);
			nfc_cmd(SPINOR_OP_PP_4B);
		}else{
			spic_addr(to, BYTE_MODE_3);
			nfc_cmd(SPINOR_OP_PP);
		}
		nfc_set_page_size(count);
		nfc_data_length(count);
		nfc_set_tmod(1);//Transfer Mode,Transmit Only
		nfc_set_fldform(0);//Quad/Dual Data, other Serial
		nfc_set_frmform(4);//opcode + 3byte addr
		nfc_set_phase_vld(NFC_CMD_PHASE_VLD | NFC_ADDR_PHASE_VLD |
			NFC_DATA_PHASE_VLD, NFC_ENABLE);

		sfc_cmd_write();

		nfc_check_buf_p2_rdy();
		sfc_write_data(write_buf, count);
		write_nfc_soft_fb_p2_ack();
		sfc_access_start();
		sfc_done(SPI_FINISH);
		spi_nor_wait_ready();
		
		to += count;
		write_buf += count;
	}

	return 0;
}

void spi_nor_erase_sector(u32 addr,u8 type)
{
	spi_nor_write_enable();
	switch(type)
	{
		case Sct_Erase_4K:
			spi_nor_erase_block(SPINOR_OP_BE_4K, addr);
			break;
		case Sct_Erase_32K:
			spi_nor_erase_block(SPINOR_OP_BE_32K, addr);
			break;
		case Sct_Erase_64K:
			spi_nor_erase_block(SPINOR_OP_SE, addr);
			break;
	}
	spi_nor_wait_ready();
}

void spi_nor_reset(void)
{
	spi_nor_write_reg(SPINOR_EN_RESET, NULL);
	spi_nor_write_reg(SPINOR_RESET_DEVICE, NULL);
	spi_nor_wait_ready();
}

void spi_nor_set_winbond_adr_3mod(void)
{
	u8 reg;

	spi_nor_read_reg(SPINOR_OP_RDSR3,&reg,1);
	reg &= 0xfd;
	spi_nor_write_reg(SPINOR_OP_WRSR3, &reg);

	return;
}


void spi_enable_qe(void)
{
	spi_nor_write_enable();
	nfc_fb_incdec_mode(BUFFER_FLOW_WRITE);
	nfc_flowbuf_clr();
	nfc_all_clr();
	spic_addr(BIT_SPINOR_LATENCY_CODE, 2);
	nfc_cmd(SPINOR_OP_WRSR);
	nfc_set_tmod(1);
	nfc_set_fldform(0);
	nfc_set_frmform(3);
	nfc_set_phase_vld(NFC_CMD_PHASE_VLD | NFC_ADDR_PHASE_VLD, NFC_ENABLE);
	sfc_cmd_write();
	sfc_access_start();
	sfc_done(SPI_FINISH);
	spi_nor_wait_ready();
	mc_udelay(10);
}

int spi_nor_set_DS_Qual_enable(u8 opcode, u8 *buf, int len)
{
	spi_nor_write_enable();
	nfc_fb_incdec_mode(BUFFER_FLOW_WRITE);
	nfc_flowbuf_clr();
	nfc_all_clr();

	nfc_cmd(opcode);
	sfc_cmd_write();
	nfc_data_type(DATA_ONLY);
	if(len){
		sfc_data_length(len);
		sfc_dev_mode_reg_set(FRMFORM_OPCODE_ONLY, VLD_CMD_DATA, WIRE_MODE_SPI);
		nfc_check_buf_p2_rdy();
		sfc_write_data(buf, len);
		write_nfc_soft_fb_p2_ack();
	}else{
		sfc_dev_mode_reg_set(FRMFORM_OPCODE_ONLY, VLD_CMD, WIRE_MODE_SPI);
	}
	sfc_access_start();
	sfc_done(SPI_FINISH);
	spi_nor_wait_ready();
	return 0;
}


int spi_nor_set_DS_3byte_addr_mode(u8 opcode, u8 *buf, int len)
{
	spi_nor_write_enable();
	nfc_fb_incdec_mode(BUFFER_FLOW_WRITE);
	nfc_flowbuf_clr();
	nfc_all_clr();

	nfc_cmd(opcode);
	sfc_cmd_write();
	nfc_data_type(DATA_ONLY);
	if(len){
		sfc_data_length(len);
		sfc_dev_mode_reg_set(FRMFORM_OPCODE_ONLY, VLD_CMD_DATA, WIRE_MODE_SPI);
		nfc_check_buf_p2_rdy();
		sfc_write_data(buf, len);
		write_nfc_soft_fb_p2_ack();
	}else{
		sfc_dev_mode_reg_set(FRMFORM_OPCODE_ONLY, VLD_CMD, WIRE_MODE_SPI);
	}
	sfc_access_start();
	sfc_done(SPI_FINISH);
	spi_nor_wait_ready();
	return 0;
}

void spi_nor_set_ds_extern(void)
{
    u8 quad_en = BIT(1);//quad/bl0/bl1/bl2
    spi_nor_write_enable();
    spi_nor_set_DS_Qual_enable(SPINOR_OP_WRSR2_2,&quad_en,1);
}

void spi_nor_set_ds_25m4ba_extern(void)
{
	u8 quad_en = BIT_SPINOR_QUAD_ENABLE;
	u8 reg;

	spi_nor_read_reg(SPINOR_OP_RDSR3,&reg,1);
	reg &= 0xfd;
	spi_nor_set_DS_3byte_addr_mode(SPINOR_OP_WRSR3, &reg, 1);
	spi_nor_set_DS_3byte_addr_mode(SPINOR_OP_EX4B, NULL, 0);
	spi_nor_set_DS_Qual_enable(SPINOR_OP_WRSR2_2,&quad_en,1);
}

void spi_nor_set_gd256_extern(void)
{
	u8 reg;

	spi_nor_read_reg(SPINOR_OP_RDSR2_2,&reg,1);
	reg |= BIT_SPINOR_QUAD_ENABLE;
	spi_nor_write_enable();
	spi_nor_write_reg(SPINOR_OP_WRSR2_2, &reg);

	spi_nor_read_reg(SPINOR_OP_RDSR3,&reg,1);
	reg &= ~(BIT_SPINOR_LATENCY_CODE);
	spi_nor_write_enable();
	spi_nor_write_reg(SPINOR_OP_WRSR3, &reg);
	spi_nor_write_reg(SPINOR_OP_EX4B, NULL);
}


void spi_nor_set_qe_enable_gd(void)
{
	u8 reg;

	spi_nor_read_reg(SPINOR_OP_RDSR2_2,&reg,1);
	if(!(reg & BIT_SPINOR_QUAD_ENABLE)) {
		reg |= BIT_SPINOR_QUAD_ENABLE;
		spi_nor_write_enable();
		spi_nor_write_reg(SPINOR_OP_WRSR2_2, &reg);
	}
}

void spi_nor_set_status_fudan(void)
{
    u8 reg;

    spi_nor_read_reg(SPINOR_OP_RDSR2_2,&reg,1);
    if (!(reg & BIT_SPINOR_QUAD_ENABLE)) {
        reg |= BIT_SPINOR_QUAD_ENABLE;
        spi_nor_write_enable();
        spi_nor_write_reg(SPINOR_OP_WRSR2_2, &reg);
    }

    spi_nor_read_reg(SPINOR_OP_RDSR,&reg,1);
    if (reg & (SR_BP0|SR_BP1|SR_BP2))
    {
        reg &= ~(SR_BP0|SR_BP1|SR_BP2);
        spi_nor_write_enable();
        spi_nor_write_reg(SPINOR_OP_WRSR, &reg);
    }
}

int spi_nor_read_id(void)
{
	int tmp;

	u8 id[SPI_NOR_MAX_ID_LEN] = {0};

	spi_nor_read_reg(SPINOR_OP_RDID,id,SPI_NOR_MAX_ID_LEN);

	for(tmp =0; tmp < ARRAY_SIZE(spi_nor_ids) ;tmp++){
		info = &spi_nor_ids[tmp];
		if(info->id_len){
			if(!memcmp(info->id, id, info->id_len)) {
				if(info->spi_nor_feature)
					info->spi_nor_feature();
				return 0;
			}
		}
	}

	return -EIO;
}

int spi_nor_write_with_addr(u8 opcode, u8 *buf, int len)
{

	nfc_fb_incdec_mode(BUFFER_FLOW_WRITE);
	nfc_flowbuf_clr();
	nfc_all_clr();

	nfc_cmd(opcode);
	sfc_cmd_write();
	nfc_data_type(DATA_ONLY);
	if(len){
		sfc_data_length(len);
		sfc_dev_mode_reg_set(FRMFORM_OPCODE_ONLY, VLD_CMD_DATA, WIRE_MODE_SPI);
		nfc_check_buf_p2_rdy();
		sfc_write_data(buf, len);
		write_nfc_soft_fb_p2_ack();
	}else{
		sfc_dev_mode_reg_set(FRMFORM_OPCODE_ONLY, VLD_CMD, WIRE_MODE_SPI);
	}
	sfc_access_start();
	sfc_done(SPI_FINISH);

	return 0;
}

int spi_nor_erase(u32 to,u32 len)
{
	u32 ret=0;
	u32 i;
	u32 addr;
	u8 erase_type;
	u8 buf[4];
	u8 opcode;
	flash_size = info->sector_size * info->n_sectors;

	for(i=0;i < len;){
		addr = to + i;

		if(4*1024 == info->sector_size){
			erase_type = Sct_Erase_4K;
			opcode = SPINOR_OP_BE_4K_4B;
		}
		else if(32*1024 == info->sector_size){
			erase_type = Sct_Erase_32K;
			opcode = SPINOR_OP_BE_4B;
		}
		else if(64*1024 == info->sector_size){
			erase_type = Sct_Erase_64K;
			opcode = SPINOR_OP_BE_4B;
		}
		else
		{
			erase_type = Sct_Erase_Invalid;
			uart_puts("invalid sector size!\n");
			return -EIO;
		}

		if(flash_size > FLASH_SIZE_16M){
			spi_nor_write_enable();
			spi_nor_write_with_addr(SPINOR_OP_EN4B, NULL, CMD_DTAT_LEN_0);
			buf[3] = addr & 0xff;
			buf[2] = (addr >> 8) & 0xff;
			buf[1] = (addr >> 16) & 0xff;
			buf[0] = (addr >> 24) & 0xff;
			spi_nor_write_with_addr(opcode,buf,CMD_DTAT_LEN_4);
		}
		else{
			spi_nor_erase_sector(addr,erase_type);
		}
		ret = spi_nor_wait_ready();
		if(ret)
			goto erase_err;

		i += info->sector_size;
	}
erase_err:
	return ret;

}

void spi_nor_reset_dummy_data(void)
{
	spi_enable(SPI_DISABLE);
	spi_set_dummy_byte(0);
}

int spi_nor_program_image(u32 dest, u32 len, u8 *write_buf)
{
	int ret=0;

	if(!spi_nor_read_id())
	{
		ret = spi_nor_erase(dest,len);

		if(ret)
			goto prg_err;

		ret = spi_nor_write_data(dest,len,write_buf);

		if(ret)
			goto prg_err;

	}
	else
		return -ENODEV;
	return ret;
prg_err:
	return ret;
}
volatile unsigned int trc_base =0 ;

int sched_nor_load_image(struct boot_image_info * info)
{

	bool ret;
	struct boot_header *header_info;
	unsigned char *header_addr = NULL ;
	unsigned int base = 0;

	uart_puts("SPL Nor Load!\n");

#ifdef CONFIG_SPL_BUILD
		header_addr = smalloc(HEADER_LEN);
		if(header_addr ==NULL)
			uart_puts("malloc header fail\r\n");
#endif
	base =info->nvm_base;
	spi_nor_reset_dummy_data();

	ret = spi_nor_read_data(base, HEADER_LEN, (unsigned char*)(header_addr));
	
	if (false == ret) {
		uart_puts("read head fail\r\n");
		return false;
	}
	
	header_info = (struct boot_header *)(header_addr);

	if (header_info->magic_num != MAGICE_DATA) 
		uart_puts("read head data fail\r\n");
	
	if((header_info->cpu_sched)){
		ret = spi_cpu_sched_boot(header_info);
	}else{
		ret = spi_transaction_dma_read(base + HEADER_LEN, header_info->length, (u8 *)(header_info->addr));
	}

	if (false == ret) {
			uart_puts("sfc load image fail\r\n");
			return false;
	}
	if(info->type == LOAD_nIMAGE){
		memcpy((unsigned int *)header_info->image_dst, (unsigned int *)header_info->image_src, header_info->image_len);
	}


	return true;
}
