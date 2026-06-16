#include "mci.h"
#include "type_use.h"
#include "glb_reg.h"
#include "system.h"
#include "aes.h"
#include "secure.h"
#include "dma.h"
#include "efuse_drv.h"
#include "uart_drv.h"

bool aes_cbc_init(u32 *iv)
{
	bool ret;

	aes_enable();
	aes_write_iv(iv, 4);
	ret = tran_aes_key();
	if (ret == false)
		return false;

	aes_config_mode(METHOD_AES_K128, EMOD_CBC);
	aes_threshold();
	aes_enable_1st_iv(true);
	return true;
}

bool aes_cbc_decode_init(void)
{
	bool ret = true;

	unsigned int aes_iv[4] = {0, 0, 0, 0};

	ret = get_aes_iv(&aes_iv[0]);
	if ((ret == false) || !(aes_iv[0]
		|| aes_iv[1] || aes_iv[2] || aes_iv[3])){
		uart_puts("iv get fail\r\n");
		return false;
	}

	ret = aes_cbc_init(&aes_iv[0]);
	if (ret == false)
		return false;

	return true;
}

bool aes_init(void)
{
	bool ret;

	ret = aes_cbc_decode_init();
	if (ret == false)
		uart_puts("AES INIT FAIL\r\n");


	return ret;
}

void aes_inter_dma_init(unsigned int src, unsigned int dest, u32 size)
{
	aes_write(DMA_SRC, src);
	aes_write(DMA_DES, dest);
	size &= TRAN_SIZE_MASK;
	aes_write(DMA_TRAS_SIZE, size);
}

void aes_inter_dma_en(bool en)
{
	u32 val;

	val = aes_read(DMA_CTRL);
	if (en) {
		val &= ~DMA_STOP;
		val |= DMA_EN;
	} else {
		val &= ~DMA_EN;
		val |= DMA_STOP;
	}
	aes_write(DMA_CTRL, val);
}

bool aes_inter_dma_start(void)
{
	u32 val;
	u32 t1, t2;

	aes_inter_dma_en(true);
	t1 = get_sys_timer_val();
	do {
		val =	aes_intr_src();
		if (val & DONE_INTR) {
			aes_clr_intr();
			break;
		}
		t2 = get_sys_timer_val();
	} while ((t2 - t1) < AES_WAIT_READY);
	if (val & DONE_INTR)
		return true;
	return false;
}

bool aes_done(void)
{
	u32 val;

	do {
		val = aes_intr_src();
		if (val & DONE_INTR) {
			aes_clr_intr();
			break;
		}

	} while (1);
	if (val & DONE_INTR)
		return true;
	return false;
}

bool aes_cbc_decode(unsigned int src, unsigned int dest,
			 u32 size)
{
	bool ret;

	ret = aes_cbc_decode_init();
	if (ret == false)
		return false;
	
	aes_decrypt(true);
	
	size = ROUNDUP(size, 16);
	
	if (size > MAX_TRAN_SIZE)
		return false;
	
	aes_inter_dma_init(src, dest, size);
	ret = aes_inter_dma_start();
	return ret;
}
volatile  unsigned int aes_start_cnt =0;

void aes_handle_start(u32 src ,u32 dst ,u32 len){


	aes_dma_trac(src, AES_RX_FIFO, 2, AES_RX_RID,len, 1);	 
	aes_dma_trac(AES_TX_FIFO,dst, 3, AES_TX_RID,len, 0);
	enable_channel(2);
	enable_channel(3);
	if(aes_start_cnt == 0){
		aes_start(len);
	}
	aes_start_cnt++;
}
