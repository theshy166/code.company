/*
 * (C) Copyright 2019-2020
 * Stelian Pop <junbao.zhang@molchip.com>
 * Lead Tech Design <www.molchip.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef __ASM_ARM_ARCH_SPIC_H__
#define __ASM_ARM_ARCH_SPIC_H__

#if defined(CONFIG_TARGET_TS01)
# include <mach/spic_r1p0.h>
#elif defined(CONFIG_TARGET_FY01)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_FY02)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_FY02B)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_FY02P)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_FY10P)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_FY00)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_FY10D)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_FY10DS)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_FY12)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_XC01)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_LT00)
# include <mach/spic_r2p0.h>
#elif defined(CONFIG_TARGET_MC3302)
# include <mach/spic_r2p0.h>
#else
# error "Unsupported Molchip SPIC Version"
#endif

#define FLASH_SIZE_16M 		0x1000000
extern u32 flash_size;

#define BYTE_MODE_3				3
#define BYTE_MODE_4				4

#define CMD_DTAT_LEN_4			4
#define CMD_DTAT_LEN_0			0

void enalbe_spic(u32 enable);
void reset_spic(void);
void spi_enable(u32 enable);
void spi_disalble_slave(u32 date);
void spi_set_polarity(u32 polarity);
void spi_set_phase(u32 phase);
void spi_cs_holdtime(unsigned int data);
void spi_set_dummy_byte(u8 dummy_byte);
void nfc_all_clr(void);
void nfc_dev_mode_clr(void);
void spi_set_frame_format(u32 format);
void spi_set_data_size(u32 size);
void spi_set_transfer_mode(u32 mode);
void spi_set_read_data_num(u32 num);
void spi_set_baudrate(u32 baudrate);
void spi_set_imr(unsigned int data);
void spi_disable_irq(u32 irq);
void spi_enable_irq(u32 irq);
u32 spi_get_status(void);
void spi_enalble_port(u32 port);
void spi_disalble_port(u32 port);
void spi_rx_sample_delay(u32 delay);
void spi_timing(u32 value);
void spi_set_field_format(u8 fld_form);
void spi_set_frame_form(u8 frm_form);
void spi_set_addr_mode(u8 addr_mode);
void clk_select_spic(int clk);
void sfc_cmd_write(void);
void sfc_cmd_read(void);
void sfc_ecc_enable(bool en);
void sfc_data_type(u32 type);
void sfc_addr_length(u32 len);
void sfc_spare_length(u32 len);
void sfc_data_length(u32 len);
void sfc_cmd(u32 cmd);
void ecc_encode(u32 encode);
bool get_ecc_sts(u32 num);
void sfc_addr(u8 *addr, u32 len);
void sfc_ecc_pos(u32 pos);
void sfc_dma_mode(void);
void sfc_write_spare(u8 *src, u32 len, u32 count);
void sfc_fill_spare(void);
void sfc_read_spare(u8 *dest, u32 len, u32 count);
void sfc_ecc_enc_mode(u32 encode);
void  sfc_spare_ecc_byte(u32 ecc);
void sfc_ecc_cor_mode(void);
void sfc_ecc_mode(u32 mode);
void nfc_addr4(unsigned int data);
void nfc_addr_length(unsigned int len);
void nfc_dma_common_clr(void);
void nfc_dma_en(unsigned int ch, unsigned char enable);
void nfc_fb_incdec_mode(unsigned char mode);
void nfc_clear_ecc(void);
void nfc_dma_reg_inter_num(unsigned char cycle);
void nfc_dma_rx_ch0_start(unsigned int num, unsigned int len,
	unsigned char run);
void spic_addr(unsigned int addr, unsigned int len);
void nfc_data_type(unsigned int type);
void nfc_read_en(void);
void nfc_set_page_size(unsigned int page_size);
void nfc_cmd_type(unsigned int type);
void nfc_ccmd_datalength(unsigned int len);
void nfc_data_length(unsigned int len);
void nfc_cmd(unsigned char opcode);
void nfc_set_tmod(unsigned char mode);
void nfc_set_fldform(unsigned char mode);
void nfc_set_frmform(unsigned char mode);
void nfc_set_cmd_wmode(unsigned char wire);
void nfc_set_data_wmode(unsigned char wire);
void nfc_set_phase_vld(unsigned int bits, unsigned char vld);
void nfc_check_buf_p2_rdy(void);
void ecc_sts_clr(void);
void sfc_start(void);
bool sfc_done(u32 finish);
void spi_set_4_bit_address_mode(void);
void nfc_set_addr_wmode(u8 wire);
void access_start_clr(void);
void access_start_set(u32 able, u32 flowbuf);
void dma_rx0_cfg1(u32 interrupt);
void nfc_cmd_mode_clr(void);
void nfc_cmd_set_clr(void);
void nfc_dev_mode_clr(void);
void nfc_dma_common_reg_set(u32 able, u32 ch, u8 pclk_cycle, u32 mode);
void nfc_dma_rx_ch0_cfg1_reg_set(u32 interrupt);
void nfc_dma_rx_ch0_reg_set(u32 num, u32 len, u8 run);
void nfc_access_start_reg_set(u32 able, u32 flowbuf);
void nfc_cmd_mode_reg_set(u32 type);
void nfc_cmd_set_reg_set(u32 from, u32 page_num, u32 page_size,
	u32 opcode, u32 ccmd_pagesize);
void nfc_dev_mode_reg_set(u32 tmod, u32 fldform, u32 frmform, u32 data,
	u32 addr);
void write_nfc_soft_fb_p2_ack(void);
void nfc_flowbuf_clr(void);
void sfc_access_start(void);
void sfc_flowbuf_clr(void);
void sfc_dev_mode_reg_set(u32 frmform, u32 vld, u32 nbits);
int sfc_start_exec(u8 dma);
u32 sfc_read_reg(void);
void sfc_write_reg(u32 cfg);
bool sfc_get_ecc_sts(u32 num);
void sfc_ecc_clr(void);
void sfc_ecc_page_size(u32 size);
void sfc_spare_len_1k(u32 len);
void sfc_dma_tx0_enable(void);
void sfc_dma_rx0_enable(void);
void sfc_dma_disable(void);
void sfc_dma_tx0_start(u32 len);
void sfc_dma_tx0_stop(void);
void sfc_dma_rx0_start(u32 len);
void sfc_dma_rx0_stop(void);
void sfc_dma_rx1_stop(void);
void sfc_dma_tx1_stop(void);

void sfc_read_ecc(u8 *dest, u32 len);
void sfc_ecc_erase_check_eb(u32 enable, u32 len);
int sfc_start_ecc_encode(void);
int sfc_start_ecc_decode(void);
void sfc_write_data_region0(u8 *src, u32 len);
void sfc_write_spare_region0(u8 *src, u32 len);
void nfc_dma_clr(void);
void sfc_fbuf2_ack(void);
void sfc_fbuf0_ack(void);
int sfc_wait_fbuf2_ready(void);
int sfc_wait_fbuf0_ready(void);

void sfc_all_clr(void);
//void sfc_cmd(u32 cmd);
//void sfc_addr(u8 *addr,u32 len);
//void sfc_cmd_read(void);
//void sfc_data_type(u32 type);
//void sfc_access_start(void);
void sfc_read_data(u8 *dest,u32 len);
//void sfc_cmd_write(void);
//void sfc_ecc_mode(u32 mode);
//void sfc_ecc_pos(u32 pos);
//void  sfc_spare_ecc_byte(u32 ecc);
//void sfc_ecc_enable(bool en);
//void sfc_dma_mode(void);
//void sfc_data_length(u32 len);
void sfc_write_data(u8 *src,u32 len);
void sfc_dma_read_data(u8 *dest,u32 len);
void sfc_dma_wait_ready(void);
void sfc_dma_write_data(u8 *src,u32 len);
void sfc_dma_write_spare(u8 *src,u32 len);
void sfc_deal_spare(u8 *dest, u8 *src_addr, u32 len, u32 count);
int spi_nand_calculate_ecc(u32 size);

void sfc_xfc_mode(u32 mode);
//void sfc_spare_length(u32 len);
//void sfc_write_spare(u8 *src,u32 len,u32 count);
//void sfc_read_spare(u8 *dest,u32 len,u32 count);
//void sfc_ecc_enc_mode(void);
//bool get_ecc_sts(u32 num);

void board_spic_init(void);
void board_spi_init(void);
int mc_spi_xfer(u32 base, u8 *tx_buf, u8 *rx_buf, u32 len, u8 is_slave);

int mc_spi_slave_xfer(u32 id, u8 *tx_buf, u8 *rx_buf, u32 len);
void board_spi_slave_init(u32 id);

#endif /* __ASM_ARM_ARCH_HARDWARE_H__ */

