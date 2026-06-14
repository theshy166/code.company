#include "mci.h"
#include "type_use.h"
#include "glb_reg.h"
#include "system.h"
#include "dma.h"
#include "spi_nor_flash.h"
#include "spic_reg.h"
#include "spic.h"
#include "spi_nor_flash.h"
#include "uart_drv.h"


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
	return 0;
}

bool spi_transction_dma_read(u32 from, u32 len, u8 *read_buf)
{
	u32 ret = true, count = 0;
	u32 cnt = 0;

	if (len % 0x1000 != 0)
		cnt = 1;
	cnt += len / 0x1000;

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
		sfc_dma_wait_ready();
		from += count;
		read_buf += count;
	}

	/* clear interrupt status. */
	ret = sfc_done(SPI_FINISH);

	return ret;

}

int spi_nor_quad_write_data( u32 to, u8 *buf, u32 len)
{
	u32 count, ret;

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
		spic_addr(to, 3);
		nfc_cmd(SPINOR_OP_PP);
		nfc_set_page_size(count);
		nfc_data_length(count);
		nfc_set_tmod(1);//Transfer Mode,Transmit Only
		nfc_set_fldform(0);//Quad/Dual Data, other Serial
		nfc_set_frmform(4);//opcode + 3byte addr
		nfc_set_phase_vld(NFC_CMD_PHASE_VLD | NFC_ADDR_PHASE_VLD |
			NFC_DATA_PHASE_VLD, NFC_ENABLE);

		sfc_cmd_write();

		nfc_check_buf_p2_rdy();
		sfc_write_data(buf, count);
		write_nfc_soft_fb_p2_ack();
		sfc_access_start();
		sfc_done(SPI_FINISH);
		ret = spi_nor_wait_ready();
		if (ret == false){
			uart_puts("sfc nor nor_write_data fail\r\n");
			break;
		}
		to += count;
		buf += count;
	}

	return 0;
}


int sfc_transfer_start(u32 src, u8 *dst, u32 size)
{
	spi_enable(SPI_DISABLE);
	spi_set_dummy_byte(4);
	nfc_flowbuf_clr();
	nfc_dma_common_reg_set(NFC_ENABLE, NFC_RX_CH0_DMA_EN, 10,
		BUFFER_FLOW_READ);
	nfc_dma_rx_ch0_reg_set(1, size, 1);


	nfc_cmd_mode_reg_set(1);
	nfc_cmd_set_reg_set(src, 1, size, SPINOR_OP_READ_1_1_4,
		size);
	nfc_dev_mode_reg_set(1, 1, 4, DATA_WMODE_2, ADDR_WMODE_0);
	sfc_dma_nor_read_data(dst, size);
	sfc_access_start();

	return 0;
}
bool sfc_soft_dma_wait_ready(void)
{
	u32 ret;

	ret = soft_dma_wait_complete(DMA_CHN31);
	if (ret == true) {
		nfc_dma_clr();
	}
	return ret;
}

bool sfc_transfer_done(void)
{
	u32 ret;

	ret = sfc_soft_dma_wait_ready();
	return ret;
}
