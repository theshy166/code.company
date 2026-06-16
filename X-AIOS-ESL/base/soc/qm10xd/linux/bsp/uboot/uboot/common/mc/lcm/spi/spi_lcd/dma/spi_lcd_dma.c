// SPDX-License-Identifier: GPL-2.0-only
/*
 * Special handling for DW DMA core
 *
 * Copyright (c) 2009, 2014 Intel Corporation.
 */

#include "data_type.h"
#include "debug.h"
#include "spi_lcd_dma.h"
//#include <fy_comm_lcdmgr.h>
#if 0
#ifndef __KERNEL__
#define phys_addr_t unsigned long
#define dma_addr_t  unsigned long
#endif

typedef struct __spi_lcd_dma_{
	uint32_t init;
	phys_addr_t dma_addr;
	uint32_t dma_chan_busy;
	spi_dma_callback_func stDmaCallback;
	//struct dma_chan * txchan;
}spi_lcd_dma_s;

static spi_lcd_dma_s stSpiLcdDmaContext = {0, 0, 0};

static spi_lcd_dma_s *get_spi_lcd_context(void){
	return &stSpiLcdDmaContext;
}

static void spi_lcd_dma_tx_done(void *arg)
{
	spi_lcd_dma_s *pContext = get_spi_lcd_context();
	if(pContext->stDmaCallback){
		pContext->stDmaCallback();
	}
}

static int spi_lcd_dma_config_tx(spi_lcd_dma_s *pContext)
{
        int rtn = SUCCESS;
#if defined(__KERNEL__)


	struct dma_slave_config txconf;

	memset(&txconf, 0, sizeof(txconf));
	txconf.direction = DMA_MEM_TO_DEV;
	txconf.dst_addr = pContext->dma_addr;
	txconf.dst_maxburst = 0x10;//dws->txburst;
	txconf.src_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;//DMA_SLAVE_BUSWIDTH_2_BYTES;//DMA_SLAVE_BUSWIDTH_1_BYTE;
	txconf.dst_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;//DMA_SLAVE_BUSWIDTH_2_BYTES;//DMA_SLAVE_BUSWIDTH_1_BYTE;
	txconf.device_fc = false;
	txconf.slave_id = 19;

	return dmaengine_slave_config(pContext->txchan, &txconf);
#endif
    return rtn;
}

static int spi_lcd_dma_submit_tx(spi_lcd_dma_s *pContext, dma_addr_t buf_addr,
		size_t buf_len)
{
    int rtn = SUCCESS;
#if defined(__KERNEL__)

	struct dma_async_tx_descriptor *txdesc;
	dma_cookie_t cookie;
#if 1
	struct scatterlist txsg;
	sg_init_table(&txsg, 1);
	sg_dma_address(&txsg) = buf_addr;
	sg_dma_len(&txsg) = buf_len;

	txdesc = dmaengine_prep_slave_sg(pContext->txchan, &txsg, 1,
			DMA_MEM_TO_DEV, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
#else
	txdesc = dmaengine_prep_dma_cyclic(dws->txchan, buf_addr, buf_len,
			period_len, DMA_MEM_TO_DEV, DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
#endif

	if (!txdesc)
		return -ENOMEM;


	//printk("sg_dma_address(&txsg) @@:0x%llx\n", sg_dma_address(&txsg));
	txdesc->callback = spi_lcd_dma_tx_done;
	txdesc->callback_param = pContext;

	cookie = dmaengine_submit(txdesc);
	rtn = dma_submit_error(cookie);
	if (rtn) {
		dmaengine_terminate_sync(pContext->txchan);
		return rtn;
	}
#endif

	return 0;
}

static int spi_lcd_dma_transfer_all(spi_lcd_dma_s *pContext,
				   unsigned long long phyaddr, unsigned len)
{
    int rtn = SUCCESS;
#if defined(__KERNEL__)

	//printk("phyaddr @@:0x%llx\n", phyaddr);
	/* Submit the DMA Tx transfer */
	rtn = spi_lcd_dma_submit_tx(pContext, phyaddr, len);
    EXIT_IF_FAIL(ret, NULL, "spi_lcd_dma_submit_tx failed");

	/* Submit the DMA Rx transfer if required */
	dma_async_issue_pending(pContext->txchan);

    EXIT:

    debug_fun(1, __FUNCTION__, __LINE__, "end");
#endif

	return rtn;
}

int spi_lcd_dma_setup(uint64_t dma_base_addr, spi_dma_callback_func stDmaCallback)
{
    int rtn = SUCCESS;
#if defined(__KERNEL__)

	spi_lcd_dma_s *pContext = get_spi_lcd_context();
	dma_cap_mask_t mask;

    debug_fun(1, __FUNCTION__, __LINE__, "start");
    EXIT_IF_ZERO(dma_base_addr, NULL, "dma_base_addr is null \n");
    CHECK_PTR(stDmaCallback, NULL, "stDmaCallback is null \n");

	if(pContext->init == 1) {
        rtn = FAIL_RE_INIT;
		EXIT_IF_FAIL(rtn, NULL, "spi dma already setup \n");
	}
	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);
	pContext->dma_addr = dma_base_addr;

	pContext->txchan = dma_request_channel(mask,NULL,NULL);
    EXIT_IF_ZERO(pContext->txchan, NULL, "spi dma tx request failed \n");


	/* Setup DMA channels */
	rtn = spi_lcd_dma_config_tx(pContext);
    EXIT_IF_FAIL(rtn, NULL, "spi dma config tx failed \n");

	pContext->stDmaCallback = stDmaCallback;
	pContext->init = 1;

    EXIT:

    debug_fun(1, __FUNCTION__, __LINE__, "end");
#endif
	return rtn;
}

int spi_lcd_dma_transfer(unsigned long long phyaddr, unsigned len)
{
	int rtn = SUCCESS;
#if defined(__KERNEL__)

	spi_lcd_dma_s *pContext = get_spi_lcd_context();

    debug_fun(1, __FUNCTION__, __LINE__, "start");

    EXIT_IF_ZERO(phyaddr,NULL, "tx_buf is null \n");
    EXIT_IF_ZERO(len, NULL, "len is null \n");
    CHECK_PTR(pContext, NULL, "pContext is null \n");

	rtn = spi_lcd_dma_transfer_all(pContext, phyaddr, len);

    EXIT:

    debug_fun(1, __FUNCTION__, __LINE__, "end");
#endif

	return rtn;
}

void spi_lcd_dma_release(void)
{
    int32_t rtn = SUCCESS;
#if defined(__KERNEL__)
	spi_lcd_dma_s *pContext = get_spi_lcd_context();

    debug_fun(1, __FUNCTION__, __LINE__, "start");

    CHECK_PTR(pContext, NULL, "pContext is null \n");

	dmaengine_terminate_sync(pContext->txchan);
	dma_release_channel(pContext->txchan);

	pContext->init = 0;

    EXIT:

    debug_fun(1, __FUNCTION__, __LINE__, "end");
#endif
    return ;
}
#endif