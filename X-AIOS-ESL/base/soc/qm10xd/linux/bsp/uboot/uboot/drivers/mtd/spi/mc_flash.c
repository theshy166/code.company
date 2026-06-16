/*
 * SPI flash interface
 *
 * Copyright (C) 2008 Atmel Corporation
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <mach/spic.h>
#include <spi_flash.h>

#include "mc_flash.h"
#include <mach/nandflash.h>
#include <mach/hardware.h>
#include <mach/dma.h>
#include "../../../board/molchip/board/system.h"

#ifdef CONFIG_FLASH_NOR_NAND_COEXISTENCE
extern void spi_nor_cs_select(void);
#endif

NodeCfg *node_sfc_out;

#define		SFC_DMAL_INKLIST				1
#define		MAX_PRG_PERIOD					10
#define		SECT_32K_SIZE					0x8000

//#define 	REG_SYS_TIMER_BASE				 (0x1A700000)
//#define 	REG_SYS_TIMER_VALUE				 (REG_SYS_TIMER_BASE + 0x4)
#define 	get_sys_timer_val()				 __raw_readl(REG_SYS_TIMER_VALUE)

#define 	SFC_BOOT_MODE					(BIT(1))
#define 	BOOT_MODE_MASK					(0xf)

struct spi_flash_info spi_nor_ids[] = {
	{ "gd251q32", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xc86016, 0, 32 * 1024, 128,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "gd25q64",  DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xc84017, 0, 32 * 1024, 256,
		SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "gd25q128", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xc84018, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
		},
	{ "gd25q256", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xc84019, 0, 32 * 1024, 1024,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
		},
	{ "gd25q129", DUMMY_BYTE_7, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xc84318, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB | SPI_DTR_EN)
		},
	{ "gd25f256f", DUMMY_BYTE_7, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xc84319, 0, 32 * 1024, 1024,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB | SPI_DTR_EN)
		},
	{ "n25q016a", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x20bb15, 0, 32 * 1024, 64,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "n25q032",  DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x20ba16, 0, 32 * 1024, 128,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
		},
	{ "w25q16dw", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xef6015, 0, 32 * 1024, 64,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "w25q128jw", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xef4018, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
		},
	{ "w25q256jw", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xef4019, 0, 32 * 1024, 1024,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "w25q16jw", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xef4015, 0, 32 * 1024, 64,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "w25q16jw", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xef4014, 0, 32 * 1024, 64,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "w25q128jwsq", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xef6018, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "mx25v1635f", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xc22315, 0, 32 * 1024, 64,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
		},
	{ "mx25l12845g", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xc22018, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "mx25l25645g", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xc22019, 0, 32 * 1024, 1024,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB| SPI_NOR_4B_OPCODES)
	},
	{ "BY25Q128", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x684018, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "xm25qh128a", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x207018, 0, 64 * 1024, 256,
				SECT_4K | SPI_NOR_DUAL_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "xm25qh128d", DUMMY_BYTE_7, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x204018, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB | SPI_DTR_EN)
	},
	{ "xm25qu64a", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x207017, 0, 64 * 1024, 128,
				SECT_4K | SPI_NOR_DUAL_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "xm25qu64b", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x205017, 0, 64 * 1024, 128,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "xm25qu64c", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x204117, 0, 64 * 1024, 128,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "xm25qu64d", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x204017, 0, 32 * 1024, 256,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "gm25q128a", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x1c4018, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "s25fl256l", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x016019, 0, 64 * 1024, 1024,
				SECT_4K | SPI_NOR_DUAL_READ |SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB | SPI_NOR_4B_OPCODES)
	},
	{ "s25fl128l", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x016018, 0, 64 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ |SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB | SPI_NOR_4B_OPCODES)
	},
	{ "gd25lq128e", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xc86018, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "ds25m4ab", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xe54218, 0, 32 * 1024, 256,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "ds25m4ba", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xe54219, 0, 64 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "ds25q4aa", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xe53118, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "en25qh128a", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x1c7018, 0, 32 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "XM25QH256C", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0x204019, 0, 64 * 1024, 512,
				SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
	{ "FM25Q128AI3", DUMMY_BYTE_0, DUMMY_BYTE_4, SFC_SAMPLE_DELAY , INFO(0xa14018, 0, 32 * 1024, 512,
				SECT_4K |SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ |
		SPI_NOR_HAS_LOCK | SPI_NOR_HAS_TB)
	},
};

int spi_nor_read_reg(u8 opcode, u8 *buf, int len)
{

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
	sfc_done(SPI_FINISH);
	nfc_check_buf_p2_rdy();
	sfc_read_data(buf, len);
	write_nfc_soft_fb_p2_ack();

	return 0;
}

int spi_nor_write_reg(u8 opcode, u8 *buf, int len)
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
int spi_nor_wait_ready(void)
{
	u8 status = 0xff;
	u32 t1,t2;
	t1 = get_sys_timer_val();
	do {
		spi_nor_read_reg(SPINOR_OP_RDSR,&status,1);
		if((status & SR_WIP) == 0)
			break;
		t2 = get_sys_timer_val();
	}while(t2 - t1 < NOR_WAIT_READY_TIME);
	if(!(status & SR_WIP))
		return 0;
	return -EIO;
}

void spi_nor_write_enable(void)
{
	spi_nor_write_reg(SPINOR_OP_WREN,NULL,0);
}

void spi_nor_write_enable_volatile(void)
{
	spi_nor_write_reg(SPINOR_OP_WRENV,NULL,0);
}

void spi_nor_write_disable(void)
{
	 spi_nor_write_reg(SPINOR_OP_WRDI, NULL, 0);
}
int spi_nor_get_dummy(struct spi_flash *nor)
{
	u32 dummy = 0;
	switch (nor->read_proto) {
	case SNOR_PROTO_1_2_2:
	case SNOR_PROTO_2_2_2:
		dummy = (nor->read_dummy  << 1) >> 3 ;
		break;
	case SNOR_PROTO_1_4_4:
	case SNOR_PROTO_4_4_4:
		dummy = (nor->read_dummy << 2) >> 3;
		break;
	case SNOR_PROTO_1_1_2:
	case SNOR_PROTO_1_1_4:
	case SNOR_PROTO_1_1_1:
		dummy = nor->read_dummy >> 3;
	default:
		break;
	}
	return dummy;

}

void spi_nor_ops(struct spi_flash *nor, u32 start_off,u8 op_type)
{
	u32 mode;
	u32 i,proto;
	u8 addr[8];
	u32 opcode;
	u32 read_dummy;
	u32 addr_width;
	u32 frmform;

	sfc_all_clr();
	if(op_type == SPIC_OP_READ) {
		opcode = nor->read_opcode;
		proto = nor->read_proto;
		nfc_fb_incdec_mode(BUFFER_FLOW_READ);
		nfc_flowbuf_clr();
		sfc_cmd_read();
	}
	else {
		opcode = nor->program_opcode;
		proto = nor->write_proto;
		nfc_fb_incdec_mode(BUFFER_FLOW_WRITE);
		nfc_flowbuf_clr();
		sfc_cmd_write();
	}
	nfc_cmd(opcode);
	switch (proto) {
		case SNOR_PROTO_1_1_2:
			mode = WIRE_MODE_DPI;
			break;
		case SNOR_PROTO_1_1_4:
			mode = WIRE_MODE_QPI;
			break;
		case SNOR_PROTO_1_1_1:
			default:
			mode = WIRE_MODE_SPI;
			break;
	}

	addr_width = nor->addr_width;
	memset(addr,0xff,sizeof(addr));
	for(i = 0; i < addr_width; i++) {
		u32 offset = (addr_width - 1 - i) << 3;
		addr[i] = (start_off >> offset) & 0xff;
	}
	if((nor->read_dummy) && (op_type == SPIC_OP_READ))
	{
		read_dummy = spi_nor_get_dummy(nor);
		addr_width += read_dummy;
	}
	sfc_addr(addr,addr_width);
	if(addr_width == 2){
		frmform = FRMFORM_OPCODE_2ADDR;
	}else if(addr_width == 3){
		frmform = FRMFORM_OPCODE_3ADDR;
	}else{
		frmform = FRMFORM_OPCODE_4ADDR;
	}
	sfc_dev_mode_reg_set(frmform, VLD_CMD_ADDR_DATA, mode);
}

int spi_nor_read_data(struct spi_flash *nor,u32 from, u32 len,u8 *read_buf)
{
	spi_enable(SPI_DISABLE);
	nfc_dma_common_reg_set(NFC_ENABLE, NFC_RX_CH0_DMA_EN, 10,BUFFER_FLOW_READ);
	nfc_flowbuf_clr();
	nfc_dma_rx_ch0_reg_set(1, len, 1);
	spi_nor_ops(nor,from,SPIC_OP_READ);
	nfc_data_type(DATA_ONLY);
	sfc_data_length(len);
	sfc_dma_nor_read_data(read_buf, len);
	sfc_access_start();
	sfc_done(SPI_FINISH);
	sfc_dma_wait_ready();
	sfc_dma_rx0_stop();
	nfc_dma_en(NFC_RX_CH0_DMA_EN, NFC_DISABLE);
	spi_enable(SPI_ENABLE);
	return 0;
}

int spi_nor_write_data(struct spi_flash *nor,u32 to, u32 len, u8 *write_buf)
{
	spi_nor_write_enable();
	spi_nor_ops(nor,to,SPIC_OP_WRITE);
	nfc_data_type(DATA_ONLY);
	sfc_data_length(len);
	nfc_check_buf_p2_rdy();
	sfc_write_data(write_buf, len);
	write_nfc_soft_fb_p2_ack();
	sfc_access_start();
	sfc_done(SPI_FINISH);
	return 0;
}

void spi_dtr_enable(void)
{
	u32 val;

	val = spi_readl(REG_SPI_CTRLR0);
	val |= (BIT_CTRLR0_ADDR_DTR_EN | BIT_CTRLR0_DATA_DTR_EN);
	val &= (~BIT_CTRLR0_CMD_DTR_EN);

	spi_writel(REG_SPI_CTRLR0, val);
}


void spi_dtr_disable(void)
{
	u32 val;

	val = spi_readl(REG_SPI_CTRLR0);
	val &= ~(BIT_CTRLR0_ADDR_DTR_EN | BIT_CTRLR0_DATA_DTR_EN);

	spi_writel(REG_SPI_CTRLR0, val);
}

void nfc_set_dtr_enable(void)
{
	unsigned int val = 0;

	val = spi_readl(REG_NFC_DEV_MODE);
	val |= (BIT(13)|BIT(14));
	val &= ~BIT(12);
	val &= ~BIT(15);
	spi_writel(REG_NFC_DEV_MODE, val);

	val = spi_readl(REG_NFC_CMD_MODE);
	val |= BIT(18);
	spi_writel(REG_NFC_CMD_MODE,val);

	val = spi_readl(REG_NFC_CMD_SET0);
	val |= BIT(0)|BIT(1);
	spi_writel(REG_NFC_CMD_SET0,val);

	spi_writel(REG_NFC_CMD_SET2,0);
}

void spi_nor_read_setfreq(int mode)
{
	spi_enable(SPI_DISABLE);
	spi_set_baudrate(BIT_SFC_BAUDR_4);
	clk_select_spic(mode);
	spi_enable(SPI_ENABLE);
}

int spi_nor_dma_read(struct spi_flash *nor,u32 from, size_t len, void *read_buf)
{
	u32 ret = true;
	u32 cnt = 0;
	int w_page = 0x1000;
	int i = 0;
	u32 fldform, frmform, opcode, addrmode;

	if (len % w_page != 0)
		cnt = 1;
	cnt += len / w_page;
#if 0
	if(nor->flags & SPI_DTR_EN){
		/*modify clk for dtr mode*/
		spi_nor_read_setfreq(SFC_CLK_300M);
	}
#endif
	spi_enable(SPI_DISABLE);
	nfc_dma_common_reg_set(NFC_ENABLE, NFC_RX_CH0_DMA_EN, BIT_DMA_REQ_INTER_NUM,
		BUFFER_FLOW_READ);
	nfc_dma_rx_ch0_reg_set(cnt, w_page, 1);
	nfc_flowbuf_clr();
	if(nor->flags & SPI_DTR_EN){
		spi_set_dummy_byte(nor->dtr_dummy_byte);
		spi_set_baudrate(BIT_SFC_BAUDR_2);
		spi_rx_sample_delay(nor->rx_smaple_delay/2);
		spi_dtr_enable();
		if(nor->addr_width == BYTE_MODE_4)
			opcode = SPINOR_OP_READ_1_4_4_DTR_4B;
		else
			opcode = SPINOR_OP_READ_1_4_4_DTR;
		fldform = FLDFORM_SERIAL_OP_OTHER_DQ;
		frmform = FRMFORM_OPCODE_ONLY;
		addrmode = WMODE_QUAD;
	} else {
		spi_set_dummy_byte(nor->dummy_byte);
		spi_rx_sample_delay(nor->rx_smaple_delay);
		fldform = FLDFORM_DQ_DATA_OTHRE_SERIAL;
		if(nor->addr_width == BYTE_MODE_4){
			opcode = SPINOR_OP_READ_1_1_4_4B;
			frmform = FRMFORM_OPCODE_4ADDR;
			spi_set_4_bit_address_mode();	// need test in here ?
		}
		else{
			opcode = SPINOR_OP_READ_1_1_4;
			frmform = FRMFORM_OPCODE_3ADDR;
		}
		addrmode = WMODE_SINGLE;
	}
	nfc_cmd_mode_reg_set(BIT_NFC_READ_DATA);

	nfc_cmd_set_reg_set(from, cnt, w_page, opcode,w_page);
	nfc_dev_mode_reg_set(TMOD_TRANSMIT_ONLY, fldform, frmform, VLD_CMD_ADDR, addrmode);

	if(nor->flags & SPI_DTR_EN){
		nfc_set_dtr_enable();
	}
	sfc_access_start();

	/*USE DMA READ*/
	for(i=0;i<cnt-1;i++){
		sfc_dma_nor_read_data(read_buf+i*w_page, w_page);
		sfc_dma_wait_ready();
	}
	sfc_dma_nor_read_data(read_buf+i*w_page, len-w_page*i);
	sfc_dma_wait_ready();
	ret = sfc_done(SPI_FINISH);
	sfc_dma_rx0_stop();
	nfc_dma_en(NFC_RX_CH0_DMA_EN, NFC_DISABLE);
	spi_enable(SPI_DISABLE);
	if(nor->flags & SPI_DTR_EN){
		spi_dtr_disable();
		spi_set_baudrate(BIT_SFC_BAUDR_4);
		spi_rx_sample_delay(nor->rx_smaple_delay);
		//spi_nor_read_setfreq(SFC_CLK_400M);
	}
	spi_set_dummy_byte(DUMMY_BYTE_0);

	if(ret)
		return 0;
	else
		return 1;
}

void spi_dma_linklist_read_start(struct spi_flash *nor, u32 from, u32 len, u8 *read_buf, NodeCfg *node_sfc_out)
{
	u32 cnt = 0;
	u32 *src, *dst, *len_arr;
	u32 i;
	int w_page = SFC_BUFFER_SIZE;
	u32 fldform, frmform, opcode, addrmode;

	if (len % w_page != 0){
		cnt = 1;
	}
	cnt += len / w_page;
	src = (u32 *)memalign(ARCH_DMA_MINALIGN, sizeof(u32)*cnt);
	dst = (u32 *)memalign(ARCH_DMA_MINALIGN, sizeof(u32)*cnt);
	len_arr = (u32 *)memalign(ARCH_DMA_MINALIGN, sizeof(u32)*cnt);
	if((src == NULL) | (dst == NULL) | (len_arr == NULL)){
		printf("smalloc fail in linklist read!\r\n");
		return;
	}
#if 0
	if(nor->flags & SPI_DTR_EN){
		/*modify clk for dtr mode*/
		spi_nor_read_setfreq(SFC_CLK_300M);
	}
#endif
	spi_enable(SPI_DISABLE);
	nfc_dma_common_reg_set(NFC_ENABLE, NFC_RX_CH0_DMA_EN, BIT_DMA_REQ_INTER_NUM,
		BUFFER_FLOW_READ);
	nfc_dma_rx_ch0_reg_set(cnt, w_page, 1);
	nfc_flowbuf_clr();
	if(nor->flags & SPI_DTR_EN){
		spi_set_dummy_byte(nor->dtr_dummy_byte);
		spi_set_baudrate(BIT_SFC_BAUDR_2);
		spi_rx_sample_delay(nor->rx_smaple_delay/2);
		spi_dtr_enable();
		if(nor->addr_width == BYTE_MODE_4)
			opcode = SPINOR_OP_READ_1_4_4_DTR_4B;
		else
			opcode = SPINOR_OP_READ_1_4_4_DTR;
		fldform = FLDFORM_SERIAL_OP_OTHER_DQ;
		frmform = FRMFORM_OPCODE_ONLY;
		addrmode = WMODE_QUAD;
	} else {
		spi_set_dummy_byte(nor->dummy_byte);
		spi_rx_sample_delay(nor->rx_smaple_delay);
		fldform = FLDFORM_DQ_DATA_OTHRE_SERIAL;
		if(nor->addr_width == BYTE_MODE_4){
			opcode = SPINOR_OP_READ_1_1_4_4B;
			frmform = FRMFORM_OPCODE_4ADDR;
			spi_set_4_bit_address_mode();	// need test in here ?
		}
		else{
			opcode = SPINOR_OP_READ_1_1_4;
			frmform = FRMFORM_OPCODE_3ADDR;
		}
		addrmode = WMODE_SINGLE;
	}
	nfc_cmd_mode_reg_set(BIT_NFC_READ_DATA);
	nfc_cmd_set_reg_set(from, cnt, w_page, opcode,w_page);
	nfc_dev_mode_reg_set(TMOD_TRANSMIT_ONLY, fldform, frmform, VLD_CMD_ADDR, addrmode);
	if(nor->flags & SPI_DTR_EN){
		nfc_set_dtr_enable();
	}

	/*config dma*/
	for(i=0;i<cnt;i++)
		src[i] = SFC_BUFFER;
	for(i=0;i<cnt;i++)
		dst[i] = (u32)(read_buf+(w_page*i));
	for(i=0;i<(cnt-1);i++)
		len_arr[i] = w_page;

	len_arr[i] = len-w_page*i;

	node_sfc_out = (NodeCfg *)memalign(64, ((sizeof(struct _NodeCfg)*cnt)));
	nor_dma_node_fill_new(DMA_CHN24, cnt, src, dst, len_arr, node_sfc_out);

	free(dst);
	free(src);
	free(len_arr);

	/* sfc start */
	sfc_access_start();
}

int spi_dma_linklist_read_stop(NodeCfg *node_sfc_out)
{
	u32 ret = true;

	if(true != dma_llist_wait_complete(DMA_CHN24,DMA_SFC_WAIT_DONE_TIME_OUT)) {
		printf("sfc dma list wait time out!\r\n");
	}
	free(node_sfc_out);
	ret = sfc_done(SPI_FINISH);//wait done

	sfc_dma_rx0_stop();
	nfc_dma_en(NFC_RX_CH0_DMA_EN, NFC_DISABLE);
	spi_enable(SPI_DISABLE);
	spi_set_dummy_byte(DUMMY_BYTE_0);
	spi_dtr_disable();
	spi_set_baudrate(BIT_SFC_BAUDR_4);
	spi_rx_sample_delay(SFC_SAMPLE_DELAY);
//	spi_nor_read_setfreq(SFC_CLK_400M);

	dma_int_clear(DMA_CHN24);
	dma_int_disable(DMA_CHN24);
	init_chn_reg(DMA_CHN24);

	return ret;
}

static volatile int continue_read_wait = 1;
int spi_nor_read(struct spi_flash *nor,u32 from, size_t len, void *read_buf)
{
	u32 count;
#ifndef CONFIG_MOLCHIP_FASTBOOT
	u32 length;

	length = len;
#endif
#ifdef CONFIG_FLASH_NOR_NAND_COEXISTENCE
	spi_nor_cs_select();
#endif

	if(nor->flags & SPI_NOR_QUAD_READ){
#ifdef SFC_DMAL_INKLIST
		spi_dma_linklist_read_start(nor,from,len, (u8 *)read_buf, node_sfc_out);
		if (continue_read_wait)
			spi_dma_linklist_read_stop(node_sfc_out);
#else
		spi_nor_dma_read(nor,from,len, (u8 *)read_buf);
#endif
	}else{
		while(len) {
			if(len > 4096) {
				len -= 4096;
				count = 4096;
			}
			else {
				count = len;
				len = 0;
			}
			spi_nor_read_data(nor,from,count, (u8 *)read_buf);

			read_buf += count;
			from += count;
		}
	}

	if (continue_read_wait)
		flush_cache((ulong)read_buf, len);

	#ifndef CONFIG_MOLCHIP_FASTBOOT
	printf("READ:%x\n",length);
	#endif
	return 0;
}

int spi_nor_program(struct spi_flash *nor,u32 to, size_t len, const void *write_buf)
{
	u32 count;
	int ret=0;
	u32 length;
	u32 period_count=0;
	u32 idx=0;
	u32 period_len=len/MAX_PRG_PERIOD;

	length = len;
#ifdef CONFIG_FLASH_NOR_NAND_COEXISTENCE
	spi_nor_cs_select();
#endif
	while(len) {
		if(len > 256) {
			len -= 256;
			count = 256;
		}
		else {
			count = len;
			len = 0;
		}
		spi_nor_write_data(nor,to,count, (u8 *)write_buf);

		ret = spi_nor_wait_ready();
		if(ret)
			goto prg_err;
		write_buf += count;
		to += count;
		period_count += count;
		if(period_count >  period_len){
			printf("$");
			period_count = 0;
			idx++;
		}
	}
	for(;idx<MAX_PRG_PERIOD;idx++){
		printf("$");
	}
	printf("\n");
	printf("PRG:%x %x\n",length,idx);

prg_err:
	return len;
}

void spi_nor_erase_sector(struct spi_flash *flash,u8* buf)
{
	spi_nor_write_enable();

	spi_nor_write_reg(flash->erase_opcode,buf,flash->addr_width);

}

int spi_nor_erase(struct spi_flash *flash,u32 to,size_t len)
{
	u32 ret=0;
	u32 i;
	u32 addr;
	u8 buf[4];

#ifdef CONFIG_FLASH_NOR_NAND_COEXISTENCE
	spi_nor_cs_select();
#endif
	for(i=0;i < len;){
		addr = to + i;
		if(flash->addr_width == 4) {
			buf[3] = addr & 0xff;
			buf[2] = (addr >> 8) & 0xff;
			buf[1] = (addr >> 16) & 0xff;
			buf[0] = (addr >> 24) & 0xff;
		} else {
			buf[2] = addr & 0xff;
			buf[1] = (addr >> 8) & 0xff;
			buf[0] = (addr >> 16) & 0xff;
		}

		spi_nor_erase_sector(flash,buf);

		ret = spi_nor_wait_ready();
		if(ret)
			goto erase_err;

		i += flash->sector_size;

	}

erase_err:
	printf("ERS:%x\n",i);
	return ret;

}

struct spi_flash_info *spi_nor_read_id(void)
{
	int tmp;
	u8 id[SPI_NOR_MAX_ID_LEN] = {0,};
	struct spi_flash_info	*info;

	spi_nor_read_reg(SPINOR_OP_RDID,id,SPI_NOR_MAX_ID_LEN);

	for(tmp =0; tmp < ARRAY_SIZE(spi_nor_ids) ;tmp ++){
		info = &spi_nor_ids[tmp];
		if(info->id_len){
			if(!memcmp(info->id, id, info->id_len)){
				flash_size = info->sector_size * info->n_sectors;
				return info;
			}
		}
	}

	printf("SF: unrecognized JEDEC id bytes: %02x, %02x, %02x\n",
			   id[0], id[1], id[2]);
	return ERR_PTR(-ENODEV);

}

u8 spi_nor_convert_opcode(u8 opcode, const u8 table[][2], u32 size)
{
	u32 i;

	for (i = 0; i < size; i++)
		if (table[i][0] == opcode)
			return table[i][1];

	/* No conversion found, keep input op code. */
	return opcode;
}

int spi_quad_enable(struct spi_flash *nor)
{
	u8 quad_en = CR_QUAD_EN_SPAN;
	int ret;
	u8 val;

	spi_nor_write_enable();

	ret = spi_nor_write_reg(SPINOR_OP_WRSR2,&quad_en,1);
	if (ret < 0){
		printf("write status register 1 fail\n");
	}

	ret = spi_nor_wait_ready();
	if (ret < 0){
		printf("erase status err\n");
	}

	/* read back and check it */
	ret = spi_nor_read_reg(SPINOR_OP_RDSR2,&val,1);
	if (!(ret == 0 && (val & CR_QUAD_EN_SPAN))) {
		printf("Quad bit not set\n");
		return -EINVAL;
	}

	return 0;
}


int spi_mx_quad_enable(struct spi_flash *nor)
{
	u8 quad_en = SR_QUAD_EN_MX;
	int ret;
	u8 val;

	spi_nor_write_enable();

	ret = spi_nor_write_reg(SPINOR_OP_WRSR,&quad_en,1);
	if (ret < 0){
		printf("write status register fail\n");
	}

	ret = spi_nor_wait_ready();
	if (ret < 0){
		printf("erase status err\n");
	}

	/* read back and check it */
	ret = spi_nor_read_reg(SPINOR_OP_RDSR,&val,1);
	if (!(ret == 0 && (val & SR_QUAD_EN_MX))) {
		printf("Quad bit not set\n");
		return -EINVAL;
	}

	return 0;
}

int spi_spansion_quad_enable(struct spi_flash *nor)
{
	u8 addr[4] = {0x0, BIT(1), 0x60, 0x78};
	int ret;
	u8 val;

	spi_nor_write_enable_volatile();

	ret = spi_nor_write_reg(0x1, addr, 4);
	if (ret < 0){
		printf("write status register fail\n");
	}

	ret = spi_nor_wait_ready();
	if (ret < 0){
		printf("erase status err\n");
	}

	/* read back and check it */
	ret = spi_nor_read_reg(SPINOR_OP_RDCR1,&val,1);

	if (!(ret == 0 && (val & BIT(1)))) {
		printf("Quad bit not set\n");
		return -EINVAL;
	}

	return 0;
}

int spi_giga_quad_enable(struct spi_flash *nor)
{
	u32 quad_en = Giga_QUAD_EN;
	int ret;
	u8 val;

	spi_nor_write_enable();

	ret = spi_nor_write_reg(SPINOR_OP_WRSR,(u8 *)&quad_en,2);
	if (ret < 0){
		printf("write status register 1 fail\n");
	}

	ret = spi_nor_wait_ready();
	if (ret < 0){
		printf("erase status err\n");
	}

	/* read back and check it */
	ret = spi_nor_read_reg(SPINOR_OP_RDSR2,&val,1);
	if (!(ret == 0 && (val & CR_QUAD_EN_SPAN))) {
		printf("Quad bit not set\n");
		return -EINVAL;
	}

	return 0;
}

void spi_nor_set_read_settings(struct spi_nor_read_command *read,
			  u8 num_mode_clocks,
			  u8 num_wait_states,
			  u8 opcode,
			  enum spi_nor_protocol proto)
{
	read->num_mode_clocks = num_mode_clocks;
	read->num_wait_states = num_wait_states;
	read->opcode = opcode;
	read->proto = proto;
}
void spi_nor_set_pp_settings(struct spi_nor_pp_command *pp,
		  u8 opcode,
		  enum spi_nor_protocol proto)
{
  pp->opcode = opcode;
  pp->proto = proto;
}

int spi_nor_init_params(struct spi_flash *nor,
			       struct spi_flash_info *info,
			       struct spi_nor_flash_parameter *params)
{
	/* Set legacy flash parameters as default. */
	memset(params, 0, sizeof(*params));

	/* Set SPI NOR sizes. */
	params->size = info->sector_size * info->n_sectors;
	params->page_size = info->page_size;

	/* (Fast) Read settings. */
	params->hwcaps.mask |= SNOR_HWCAPS_READ;
	spi_nor_set_read_settings(&params->reads[SNOR_CMD_READ],
				  0, 0, SPINOR_OP_READ,
				  SNOR_PROTO_1_1_1);

	if (!(info->flags & SPI_NOR_NO_FR)) {
		params->hwcaps.mask |= SNOR_HWCAPS_READ_FAST;
		spi_nor_set_read_settings(&params->reads[SNOR_CMD_READ_FAST],
					  0, 8, SPINOR_OP_READ_FAST,
					  SNOR_PROTO_1_1_1);
	}

	if (info->flags & SPI_NOR_DUAL_READ) {
		params->hwcaps.mask |= SNOR_HWCAPS_READ_1_1_2;
		spi_nor_set_read_settings(&params->reads[SNOR_CMD_READ_1_1_2],
					  0, 8, SPINOR_OP_READ_1_1_2,
					  SNOR_PROTO_1_1_2);
	}

	if (info->flags & SPI_NOR_QUAD_READ) {
		params->hwcaps.mask |= SNOR_HWCAPS_READ_1_1_4;
		spi_nor_set_read_settings(&params->reads[SNOR_CMD_READ_1_1_4],
					  0, 8, SPINOR_OP_READ_1_1_4,
					  SNOR_PROTO_1_1_4);
	}

	/* Page Program settings. */
	params->hwcaps.mask |= SNOR_HWCAPS_PP;
	spi_nor_set_pp_settings(&params->page_programs[SNOR_CMD_PP],
				SPINOR_OP_PP, SNOR_PROTO_1_1_1);
#if 1
	params->hwcaps.mask |= SNOR_HWCAPS_PP_QUAD;
	spi_nor_set_pp_settings(&params->page_programs[SNOR_CMD_PP_1_1_4],
				SPINOR_OP_PP_1_1_4, SNOR_PROTO_1_1_4);
#endif
	/* Select the procedure to set the Quad Enable bit. */
	if (params->hwcaps.mask & (SNOR_HWCAPS_READ_QUAD |
				   SNOR_HWCAPS_PP_QUAD)) {
        switch (JEDEC_MFR(info)) {
       case SNOR_MFR_MX:
            params->quad_enable = spi_mx_quad_enable;
            break;
		case SNOR_MFR_SPANSION:
			params->quad_enable = spi_spansion_quad_enable;
			break;
		case SNOR_MFR_GIGADEVICE:
			params->quad_enable = spi_giga_quad_enable;
			break;
		case SNOR_MFR_EON:
			params->quad_enable = NULL;
			break;
        default:
            /* Kept only for backward compatibility purpose. */
            params->quad_enable = spi_quad_enable;
            break;
        }
    }

	return 0;

}

int spi_nor_hwcaps2cmd(u32 hwcaps, const int table[][2], u32 size)
{
   u32 i;

   for (i = 0; i < size; i++)
	   if (table[i][0] == (int)hwcaps)
		   return table[i][1];

   return -EINVAL;
}

int spi_nor_hwcaps_read2cmd(u32 hwcaps)
{
   static const int hwcaps_read2cmd[][2] = {
	   { SNOR_HWCAPS_READ,	   SNOR_CMD_READ },
	   { SNOR_HWCAPS_READ_FAST,    SNOR_CMD_READ_FAST },
	   { SNOR_HWCAPS_READ_1_1_1_DTR,   SNOR_CMD_READ_1_1_1_DTR },
	   { SNOR_HWCAPS_READ_1_1_2,   SNOR_CMD_READ_1_1_2 },
	   { SNOR_HWCAPS_READ_1_2_2,   SNOR_CMD_READ_1_2_2 },
	   { SNOR_HWCAPS_READ_2_2_2,   SNOR_CMD_READ_2_2_2 },
	   { SNOR_HWCAPS_READ_1_2_2_DTR,   SNOR_CMD_READ_1_2_2_DTR },
	   { SNOR_HWCAPS_READ_1_1_4,   SNOR_CMD_READ_1_1_4 },
	   { SNOR_HWCAPS_READ_1_4_4,   SNOR_CMD_READ_1_4_4 },
	   { SNOR_HWCAPS_READ_4_4_4,   SNOR_CMD_READ_4_4_4 },
	   { SNOR_HWCAPS_READ_1_4_4_DTR,   SNOR_CMD_READ_1_4_4_DTR },
	   { SNOR_HWCAPS_READ_1_1_8,   SNOR_CMD_READ_1_1_8 },
	   { SNOR_HWCAPS_READ_1_8_8,   SNOR_CMD_READ_1_8_8 },
	   { SNOR_HWCAPS_READ_8_8_8,   SNOR_CMD_READ_8_8_8 },
	   { SNOR_HWCAPS_READ_1_8_8_DTR,   SNOR_CMD_READ_1_8_8_DTR },
   };

   return spi_nor_hwcaps2cmd(hwcaps, hwcaps_read2cmd,
				 ARRAY_SIZE(hwcaps_read2cmd));
}

int spi_nor_hwcaps_pp2cmd(u32 hwcaps)
{
   static const int hwcaps_pp2cmd[][2] = {
	   { SNOR_HWCAPS_PP,	   SNOR_CMD_PP },
	   { SNOR_HWCAPS_PP_1_1_4,	   SNOR_CMD_PP_1_1_4 },
	   { SNOR_HWCAPS_PP_1_4_4,	   SNOR_CMD_PP_1_4_4 },
	   { SNOR_HWCAPS_PP_4_4_4,	   SNOR_CMD_PP_4_4_4 },
	   { SNOR_HWCAPS_PP_1_1_8,	   SNOR_CMD_PP_1_1_8 },
	   { SNOR_HWCAPS_PP_1_8_8,	   SNOR_CMD_PP_1_8_8 },
	   { SNOR_HWCAPS_PP_8_8_8,	   SNOR_CMD_PP_8_8_8 },
   };

   return spi_nor_hwcaps2cmd(hwcaps, hwcaps_pp2cmd,
				 ARRAY_SIZE(hwcaps_pp2cmd));
}

int spi_nor_select_read(struct spi_flash *nor,
				  const struct spi_nor_flash_parameter *params,
				  u32 shared_hwcaps)
{

   const struct spi_nor_read_command *read;
   int cmd, best_match = fls(shared_hwcaps & SNOR_HWCAPS_READ_MASK) - 1;

   if (best_match < 0)
	   return -EINVAL;

   cmd = spi_nor_hwcaps_read2cmd(BIT(best_match));
   if (cmd < 0)
	   return -EINVAL;

   read = &params->reads[cmd];
   nor->read_opcode = read->opcode;
   nor->read_proto = read->proto;

   /*
	* In the spi-nor framework, we don't need to make the difference
	* between mode clock cycles and wait state clock cycles.
	* Indeed, the value of the mode clock cycles is used by a QSPI
	* flash memory to know whether it should enter or leave its 0-4-4
	* (Continuous Read / XIP) mode.
	* eXecution In Place is out of the scope of the mtd sub-system.
	* Hence we choose to merge both mode and wait state clock cycles
	* into the so called dummy clock cycles.
	*/
   nor->read_dummy = read->num_mode_clocks + read->num_wait_states;
   return 0;
}

int spi_nor_select_pp(struct spi_flash *nor,
				const struct spi_nor_flash_parameter *params,
				u32 shared_hwcaps)
{
   int cmd, best_match = fls(shared_hwcaps & SNOR_HWCAPS_PP_MASK) - 1;
   const struct spi_nor_pp_command *pp;

   if (best_match < 0)
	   return -EINVAL;

   cmd = spi_nor_hwcaps_pp2cmd(BIT(best_match));
   if (cmd < 0)
	   return -EINVAL;

   pp = &params->page_programs[cmd];
   nor->program_opcode = pp->opcode;
   nor->write_proto = pp->proto;
   return 0;
}

int spi_nor_select_erase(struct spi_flash *nor,
			   const struct spi_flash_info *info)
{
	if(4*1024 == info->sector_size)
		nor->erase_opcode = SPINOR_OP_BE_4K;
	else if(32*1024 == info->sector_size)
		nor->erase_opcode = SPINOR_OP_BE_32K;
	else if(64*1024 == info->sector_size)
		nor->erase_opcode = SPINOR_OP_SE;
	else
	{
		return -EIO;
	}

   nor->erase_size = info->sector_size;
   return 0;
}

/* Enable/disable 4-byte addressing mode. */
static int set_4byte(struct spi_flash_info *info, bool enable)
{
	int status;
	bool need_wren = false;
	u8 cmd;

	switch (JEDEC_MFR(info)) {
	case SNOR_MFR_MICRON:
		/* Some Micron need WREN command; all will accept it */
		need_wren = true;
		/* fall through */
	case SNOR_MFR_MACRONIX:
	case SNOR_MFR_GIGADEVICE:
	case SNOR_MFR_WINBOND:
		if (need_wren)
			spi_nor_write_enable();

		cmd = enable ? SPINOR_OP_EN4B : SPINOR_OP_EX4B;
		status = spi_nor_write_reg(cmd, NULL, 0);
		if (need_wren)
			spi_nor_write_disable();

		if (!status && !enable &&
		    JEDEC_MFR(info) == SNOR_MFR_WINBOND) {
			/*
			 * On Winbond W25Q256FV, leaving 4byte mode causes
			 * the Extended Address Register to be set to 1, so all
			 * 3-byte-address reads come from the second 16M.
			 * We must clear the register to enable normal behavior.
			 */
			spi_nor_write_enable();
			cmd = 0;
			spi_nor_write_reg(SPINOR_OP_WREAR, &cmd, 1);
			spi_nor_write_disable();
		}

		return status;
	default:
		/* Spansion style */
		cmd = enable << 7;
		return spi_nor_write_reg(SPINOR_OP_BRWR, &cmd, 1);
	}
}

int spi_nor_init(struct spi_flash *nor)
{
	int err;

	if (nor->quad_enable) {
		err = nor->quad_enable(nor);
		if (err) {
			printf("quad mode not supported\n");
			return err;
		}
	}

	return 0;
}
int spi_nor_setup(struct spi_flash *nor, const struct spi_flash_info *info,
			const struct spi_nor_flash_parameter *params,
			const struct spi_nor_hwcaps *hwcaps)
{
   u32 ignored_mask, shared_mask;
   bool enable_quad_io;
   int err;
   /*
	* Keep only the hardware capabilities supported by both the SPI
	* controller and the SPI flash memory.
	*/
   shared_mask = hwcaps->mask & params->hwcaps.mask;

   /* SPI n-n-n protocols are not supported yet. */
   ignored_mask = (SNOR_HWCAPS_READ_2_2_2 |
		   SNOR_HWCAPS_READ_4_4_4 |
		   SNOR_HWCAPS_READ_8_8_8 |
		   SNOR_HWCAPS_PP_4_4_4 |
		   SNOR_HWCAPS_PP_8_8_8);
   if (shared_mask & ignored_mask) {
	   printf("SPI n-n-n protocols are not supported yet.\n");
	   shared_mask &= ~ignored_mask;
   }

   /* Select the (Fast) Read command. */
   err = spi_nor_select_read(nor, params, shared_mask);
   if (err) {
	   printf("can't select read settings supported by both the SPI controller and memory.\n");
	   return err;
   }

   /* Select the Page Program command. */
   err = spi_nor_select_pp(nor, params, shared_mask);
   if (err) {
	   printf(
		   "can't select write settings supported by both the SPI controller and memory.\n");
	   return err;
   }

   /* Select the Sector Erase command. */
   err = spi_nor_select_erase(nor, info);
   if (err) {
	   printk(
		   "can't select erase settings supported by both the SPI controller and memory.\n");
	   return err;
   }

   /* Enable Quad I/O if needed. */
   enable_quad_io = (spi_nor_get_protocol_width(nor->read_proto) == 4 ||
			 spi_nor_get_protocol_width(nor->write_proto) == 4);
   if (enable_quad_io && params->quad_enable)
	   nor->quad_enable = params->quad_enable;
   else
	   nor->quad_enable = NULL;

   return 0;
}

static u8 spi_nor_convert_3to4_read(u8 opcode)
{
	static const u8 spi_nor_3to4_read[][2] = {
		{ SPINOR_OP_READ,	SPINOR_OP_READ_4B },
		{ SPINOR_OP_READ_FAST,	SPINOR_OP_READ_FAST_4B },
		{ SPINOR_OP_READ_1_1_2,	SPINOR_OP_READ_1_1_2_4B },
		{ SPINOR_OP_READ_1_1_4,	SPINOR_OP_READ_1_1_4_4B },

	};

	return spi_nor_convert_opcode(opcode, spi_nor_3to4_read,
				      ARRAY_SIZE(spi_nor_3to4_read));
}

static u8 spi_nor_convert_3to4_program(u8 opcode)
{
	static const u8 spi_nor_3to4_program[][2] = {
		{ SPINOR_OP_PP,		SPINOR_OP_PP_4B },
		{ SPINOR_OP_PP_1_1_4,	SPINOR_OP_PP_1_1_4_4B },
	};

	return spi_nor_convert_opcode(opcode, spi_nor_3to4_program,
				      ARRAY_SIZE(spi_nor_3to4_program));
}

static u8 spi_nor_convert_3to4_erase(u8 opcode)
{
	static const u8 spi_nor_3to4_erase[][2] = {
		{ SPINOR_OP_BE_4K,	SPINOR_OP_BE_4K_4B },
		{ SPINOR_OP_BE_32K,	SPINOR_OP_BE_32K_4B },
		{ SPINOR_OP_SE,		SPINOR_OP_SE_4B },
	};

	return spi_nor_convert_opcode(opcode, spi_nor_3to4_erase,
				      ARRAY_SIZE(spi_nor_3to4_erase));
}

static void spi_nor_set_4byte_opcodes(struct spi_flash *nor)
{

	nor->read_opcode = spi_nor_convert_3to4_read(nor->read_opcode);
	nor->program_opcode = spi_nor_convert_3to4_program(nor->program_opcode);
	nor->erase_opcode = spi_nor_convert_3to4_erase(nor->erase_opcode);

}

int spi_nor_scan(struct spi_flash *nor, const char *name,
		 const struct spi_nor_hwcaps *hwcaps)
{
	struct spi_flash_info *info=NULL;
	struct spi_nor_flash_parameter params;
	int ret;

#ifdef CONFIG_FLASH_NOR_NAND_COEXISTENCE
	spi_nor_cs_select();
#endif
	/* Reset SPI protocol for all commands. */
	nor->reg_proto = SNOR_PROTO_1_1_1;
	nor->read_proto = SNOR_PROTO_1_1_1;
	nor->write_proto = SNOR_PROTO_1_1_1;

	/* Try to auto-detect if chip name wasn't specified or not found */
	if (!info)
		info = spi_nor_read_id();

	spi_nor_read_setfreq(SFC_CLK_400M);

	if(IS_ERR_OR_NULL(info))
		return -ENOENT;

	/* Parse the Serial Flash Discoverable Parameters table. */
	ret = spi_nor_init_params(nor, info, &params);
	if (ret)
		return ret;

	nor->page_size = params.page_size;
	nor->size = info->sector_size * info->n_sectors;
	nor->sector_size = info->sector_size;
	nor->flags = info->flags;
	nor->dummy_byte = info->dummy_byte;
	nor->dtr_dummy_byte = info->dtr_dummy_byte;
	nor->rx_smaple_delay = info->rx_sample_delay;

	/* If we weren't instantiated by DT, default to fast-read */
	params.hwcaps.mask |= SNOR_HWCAPS_READ_FAST;

	/* Some devices cannot do fast-read, no matter what DT tells us */
	if (info->flags & SPI_NOR_NO_FR)
		params.hwcaps.mask &= ~SNOR_HWCAPS_READ_FAST;

	/*
	 * Configure the SPI memory:
	 * - select op codes for (Fast) Read, Page Program and Sector Erase.
	 * - set the number of dummy cycles (mode cycles + wait states).
	 * - set the SPI protocols for register and memory accesses.
	 * - set the Quad Enable bit if needed (required by SPI x-y-4 protos).
	 */
	ret = spi_nor_setup(nor, info, &params, hwcaps);
	if (ret)
		return ret;

	if (nor->addr_width) {
			/* already configured from SFDP */
	} else if (info->addr_width) {
		nor->addr_width = info->addr_width;
	} else if (nor->size > 0x1000000) {
		/* enable 4-byte addressing if the device exceeds 16MiB */
		nor->addr_width = 4;
		if (info->flags & SPI_NOR_4B_OPCODES)
			spi_nor_set_4byte_opcodes(nor);

	} else {
		nor->addr_width = 3;
	}

	if (nor->addr_width > SPI_NOR_MAX_ADDR_WIDTH) {
		printf("address width is too large: %u\n",
		nor->addr_width);
		return -EINVAL;
	}

	/* Send all the required SPI flash commands to initialize device */
	ret = spi_nor_init(nor);
	if (ret)
		return ret;
	if (nor->addr_width == 4 && !(nor->flags & SPI_NOR_4B_OPCODES)) {
		set_4byte(info, true);
	}
	#ifndef CONFIG_MOLCHIP_FASTBOOT
	printf("nor->read_opcode:%x,nor->write_opcode:%x,nor->erase_opcode:%x\n", nor->read_opcode,nor->program_opcode,nor->erase_opcode);
	#endif
	return 0;
}

struct spi_flash *spi_nor_flash(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
	struct spi_flash *flash;
	int ret;

	const struct spi_nor_hwcaps hwcaps = {
		.mask = SNOR_HWCAPS_READ |
			 SNOR_HWCAPS_READ_FAST |
			 SNOR_HWCAPS_READ_1_1_2 |
			 SNOR_HWCAPS_READ_1_1_4 |
			// SNOR_HWCAPS_PP_1_1_4 |  does not supported for defact
			SNOR_HWCAPS_PP,
	};

	flash = malloc(sizeof(*flash));
	memset(flash,0,sizeof(*flash));

	ret = spi_nor_scan(flash,NULL,&hwcaps);

	if(ret)
		goto err;

	flash->read = spi_nor_read;
	flash->write = spi_nor_program;
	flash->erase = spi_nor_erase;

	return flash;

	err:
		kfree(flash);
		return NULL;

}

struct spi_flash *spi_nand_flash(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
	struct spi_flash *flash;
	int ret;
	printf("spi_nand_boot\n");
	flash = malloc(sizeof(*flash));
	memset(flash,0,sizeof(*flash));
	ret = spi_nand_scan(flash);

	if(ret)
		goto err;

	flash->read = spi_nand_read;
	flash->write = spi_nand_program;
	flash->erase = spi_nand_erase;

	return flash;

	err:
		kfree(flash);

		return NULL;

}

u8 nvm_boot_device(void)
{
	unsigned int val;
	val = get_strap_status();
	val >>= 1;
	if (val == 0) return BOOT_DEVICE_NOR;
	else if (val == 1) return BOOT_DEVICE_NAND;
	else if (val == 2) return BOOT_DEVICE_USB;
	else if (val == 3) return BOOT_DEVICE_SDIO1;
	else return BOOT_DEVICE_NONE;
}

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
	u8 bootmod;
	struct spi_flash *flash;
	bootmod = nvm_boot_device();
	//bootmod = BOOT_DEVICE_NOR;
	board_spic_init();

	switch(bootmod)
	{
		case BOOT_DEVICE_NOR:
			flash = spi_nor_flash(bus,cs,max_hz,spi_mode);
			break;

		case BOOT_DEVICE_NAND:
			flash = spi_nand_flash(bus,cs,max_hz,spi_mode);
			break;
		default :
			flash = spi_nor_flash(bus,cs,max_hz,spi_mode);
			break;
	}
	return flash;
}


void spi_flash_free(struct spi_flash *flash)
{

	kfree(flash);
}

int spi_nor_read_mode(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *cmd;
	if (argc < 2)
		goto usage;

	cmd = argv[1];

	if (strcmp(cmd, "cont_nw") == 0) {
		continue_read_wait = 0;
	} else if (strcmp(cmd, "cont_w") == 0) {
		continue_read_wait = 1;
	}

	return 0;

usage:
	return CMD_RET_USAGE;
}

U_BOOT_CMD(
        sfrm, 2, 1, spi_nor_read_mode,
		"SPI Nor flash read mode",
		"sfrm cont_w	- set spi nor flash continue mode\n"
		"				  and wait finish\n"
		"sfrm cont_nw    - set spi nor flash continue mode\n"
		"				  and no wait finish'\n"
);

int spi_nor_cont_read_done(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u8 *read_buf;
	u32 len,buf_addr;

	buf_addr = simple_strtoul(argv[1], NULL, 16);
	read_buf = (u8 *)buf_addr;
	len = (int)simple_strtoul(argv[2], NULL, 8);

	if (true == spi_dma_linklist_read_stop(node_sfc_out)) {
		flush_cache((ulong)read_buf, len);
	} else {
		printf("spi nor flash wait failed\n");
		return -1;
	}

	return 0;
}

U_BOOT_CMD(
        sfrd, 3, 1, spi_nor_cont_read_done,
        "spi nor flash continue read wait finish",
        "spi nor flash continue read wait finish"
);

