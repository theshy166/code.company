#include "mc_dma_lib.h"
#include "mc_app_dma64.h"
#include <linux/module.h>
#include <linux/dmaengine.h>
#include <linux/dmapool.h>
#include <linux/pm_runtime.h>
#include <linux/dma/mc_dma64.h>
#include <linux/workqueue.h>
#include <linux/serial.h>
#include <asm/bitops.h>
#include "utility.h"
#include "mc_dma64_reg.h"
#define MC_DMA_BUSWIDTHS	\
	BIT(DMA_SLAVE_BUSWIDTH_UNDEFINED)	|\
	BIT(DMA_SLAVE_BUSWIDTH_1_BYTE)		|\
	BIT(DMA_SLAVE_BUSWIDTH_2_BYTES)		|\
	BIT(DMA_SLAVE_BUSWIDTH_4_BYTES)		|\
	BIT(DMA_SLAVE_BUSWIDTH_8_BYTES)


#define	INTERRUPT_MISS_DEBUG		0
void	mcc_interrupt_miss_debug(struct mc_dma *mc,struct mc_dma_chan *mcc){

	struct 	mc_dma_ch_regs *ch_regs;
	u32 	dest_addr_current = 0;
	u32 	src_addr_current = 0;
	u32 	period_len = mcc->period_len;
	u32 	buf_will_done = mcc->buf_will_done;
	u32 	buf_addr_diff_abs = 0;
	u32 	bwd_index = mcc->bwd_index;

	ch_regs =(struct mc_dma_ch_regs *)mcc->ch_regs;

	if(mcc->dma_sconfig.direction != DMA_MEM_TO_DEV &&
			mcc->dma_sconfig.direction != DMA_DEV_TO_MEM)
	{
		dev_err(chan2dev(&mcc->chan),"dma direction is wrong\r\n");
		return ;
	}

	if(mcc->dma_sconfig.direction == DMA_MEM_TO_DEV){
		src_addr_current = ch_regs->full_channel_regs.full_chn_src_addr;
		buf_addr_diff_abs = abs(src_addr_current - buf_will_done);
		dev_vdbg(chan2dev(&mcc->chan)," c 0x%x,w 0x%x\r\n",src_addr_current,buf_will_done);

	}else{

		dest_addr_current = ch_regs->full_channel_regs.full_chn_dest_addr;
		buf_addr_diff_abs = abs(dest_addr_current - buf_will_done);
		dev_vdbg(chan2dev(&mcc->chan)," c 0x%x,w 0x%x\r\n",dest_addr_current,buf_will_done);
	}


	if((buf_addr_diff_abs >= period_len && buf_addr_diff_abs <= period_len*2)
			||(buf_addr_diff_abs >= period_len*(mcc->periods - 2) && buf_addr_diff_abs <=period_len*(mcc->periods -1))){

		mcc->bd_index = bwd_index;
		if(bwd_index == mcc->periods -1)
			mcc->bwd_index = 0;
		else
			mcc->bwd_index++;

		if(mcc->dma_sconfig.direction == DMA_MEM_TO_DEV){
			mcc->buf_will_done = src_addr_current - (src_addr_current & (period_len-1));
		}else{
			mcc->buf_will_done = dest_addr_current - (dest_addr_current & (period_len-1));
		}

		dev_vdbg(chan2dev(&mcc->chan),"%d\r\n",mcc->bd_index);
		dev_vdbg(chan2dev(&mcc->chan),"0x%x,%d\r\n",buf_addr_diff_abs,mcc->bwd_index);
	}
	else
	{
		mcc->intrc_error++;
		if(mcc->dma_sconfig.direction == DMA_MEM_TO_DEV){
			mcc->buf_will_done = src_addr_current - (src_addr_current & (period_len-1));
		}else{
			mcc->buf_will_done = dest_addr_current - (dest_addr_current & (period_len-1));
		}
		mcc->bwd_index = (mcc->buf_will_done - mcc->first_will_done)/period_len;
		dev_err(chan2dev(&mcc->chan),"dma audio intrc miss\r\n");
	}


}

void mcc_cyclic_debug(struct mc_dma_chan *mcc){
	u32 value;
	if(mcc->dma_sconfig.direction == DMA_MEM_TO_DEV){
		 value = dma64_read_reg_src(mcc);
			dev_vdbg(chan2dev(&mcc->chan),"0x%x,%d\r\n",value,mcc->bd_index);
	}else{
		 value = dma64_read_reg_dest(mcc);
			dev_vdbg(chan2dev(&mcc->chan),"0x%x,%d\r\n",value,mcc->bd_index);
	}
}

static void mcc_handle_cyclic_index(struct mc_dma *mc, struct mc_dma_chan *mcc,
		u32 status_trans){
#if (INTERRUPT_MISS_DEBUG == 0)
	u32 	bwd_index = mcc->bwd_index;
	mcc->bd_index = bwd_index;
	if(bwd_index == mcc->periods -1)
		mcc->bwd_index = 0;
	else
		mcc->bwd_index++;
#else
//	mcc_interrupt_miss_debug(mc,mcc);
	mcc_cyclic_debug(mcc);

#endif
	if(mcc->resume_flag == 1)
	{
		mcc->bwd_index = 0;
		mcc->resume_flag = 0;
		return;
	}
	if (status_trans) {
		void (*callback)(void *param);
		void *callback_param;
		callback = mcc->cdesc->period_callback;
		callback_param = mcc->cdesc->period_callback_param;

		if (callback)
			callback(callback_param);
	}
}

void mc_dma_intrc_tasklet(long unsigned int data){
	struct 	mc_dma_chan *mcc;
	struct 	mc_dma *mc;
	u32 	status_trans_bits;
	u32 	status_llist_bits;
	u32	channel;
	mc = (struct mc_dma*)data;
	status_trans_bits = mc->trans_bits;
	status_llist_bits = mc->llist_bits;
	mc->trans_bits = 0;
	mc->llist_bits = 0;
	while(status_trans_bits){
		channel = fls(status_trans_bits);
		if(!channel)
			break;
		channel--;
		mcc = &mc->chan[channel];
		if(test_bit(MC_DMA_IS_CYCLIC,&mcc->flags)){
			mcc_handle_cyclic_index(mc,mcc,status_trans_bits&(1<<channel));
		}
		mcc->status_trans = 0;
		status_trans_bits &= ~(1<<channel);
	}
	while(status_llist_bits){
		channel = fls(status_llist_bits);
		if(!channel)
			break;
		channel--;
		mcc = &mc->chan[channel];
		mcc->status_llist = 0;
		mcc_scan_descriptors(mc,mcc);
		status_llist_bits &= ~(1<<channel);
	}
}

static void mcc_dma_fragm_process(struct mc_dma_chan *mcc){
	struct mc_desc *desc;
	desc = mcc->desc;
	mcc->status_fragm = 0;
	mcc_descriptor_reuse_complete(mcc,desc,true);
}

static void mcc_dma_llist_process(struct mc_dma_chan *mcc){
	struct mc_desc*desc;
	desc = mcc->desc;
	mcc->status_llist = 0;
	mcc_descriptor_reuse_complete(mcc,desc,true);
}

#if 0
void mcc_dma_llist_process_tasklet(long unsigned int data){
	struct  mc_dma *mc = (struct mc_dma*)data;
	struct 	mc_dma_chan *mcc;
	mcc =&mc->chan[mc->ch_uart_tx];
	mcc_dma_llist_process(mcc);
}
#endif

static u32 mc_is_uart_channel(u32 *ch_uart, u32 channel)
{
	u32 i;

	for (i = 0; i < UART_NR_MAX; i++)
		if (ch_uart[i] == channel)
			return 1;

	return 0;
}

irqreturn_t mc_dma64_interrupt(int irq,void *dev_id){
	u32 	value;
	u32 	channel;
	struct 	mc_dma *mc = dev_id;
	struct 	mc_dma_common_reg* comr;
	struct 	mc_dma_ch_regs* ch_regs;
	struct 	mc_dma_chan *mcc;
	void 	__iomem* ch_regs_base;
	u32 is_uart_tx_ch = 0;
	u32 need_tasklet_scheule = 0;

	comr = (struct mc_dma_common_reg*)mc->common_regs;
	ch_regs_base = mc->chan_regs;

	while(dma64_get_int_mask_status_reg(comr)){
		value = dma64_get_int_mask_status_reg(comr);
		channel = fls(value);
		channel--;
		ch_regs = (struct mc_dma_ch_regs*)(ch_regs_base + channel*MC_CHAN_REGS_OFFSET);
		value = dma64_get_full_chn_int(ch_regs);
		/*check if we have any interrupts from the channel*/
		if(!(value&0x1f0000))
			continue;
		/*try to clear the interrupts*/
		if(value & 0x40000){
			value |= CHN_TRANSACTION_CLR;
			dma64_clear_int(ch_regs,value);
		}else if(value & 0x80000){
			value |= CHN_LINKLIST_CLR;
			dma64_clear_int(ch_regs,value);
		}else if(value & 0x10000){
			value |= CHN_FRAGMENT_INT_CLR;
			dma64_clear_int(ch_regs,value);
		}else if(value & 0x20000){
			value |= CHN_BLK_INT_CLR;
			dma64_clear_int(ch_regs,value);
		}else if(value & 0x100000){
			value |= CHN_CFG_ERR_INT_CLR;
			dma64_clear_int(ch_regs,value);
			mc->err_counter++;
			continue;
		}
		mcc = &mc->chan[channel];
		mcc->status_fragm = value&(1<<16);
		mcc->status_trans = value&(1<<18);
		mcc->status_llist = value&(1<<19);

		if (mc_is_uart_channel(mc->ch_uart_tx, channel)) {
			is_uart_tx_ch = 1;
			if (mcc->status_llist){
				mcc_dma_llist_process(mcc);
				//tasklet_schedule(&mc->tasklet2);
			}
		}

		if(mc_is_uart_channel(mc->ch_uart_rx, channel)) {
			if(mcc->status_fragm){
				mcc_dma_fragm_process(mcc);
			}
		}

		if (mcc->status_trans) {
			mc->trans_bits |= 1 << channel;
			need_tasklet_scheule = 1;
		} else if (mcc->status_llist) {
			if (!is_uart_tx_ch) {
				mc->llist_bits |= 1 << channel;
				need_tasklet_scheule = 1;
			}
		}
	}

	if (need_tasklet_scheule)
		tasklet_schedule(&mc->tasklet);

	return IRQ_HANDLED;
}

int mc_dma64_probe(struct mc_dma64_chip *chip){
	struct mc_dma64_data 	*pdata;
	struct mc_dma		*mc;
	int			err;
	int 			i;

	mc = devm_kzalloc(chip->dev,sizeof(*mc),GFP_KERNEL);
	if(!mc)
		return -ENOMEM;
	mc->pdata = devm_kzalloc(chip->dev,sizeof(*mc->pdata),GFP_KERNEL);
	if(!mc->pdata)
		return -ENOMEM;

	mc->common_regs = chip->common_regs;
	mc->chan_regs = chip->chan_regs;
	mc->req_cid_regs = chip->req_cid_regs;
	mc->in_use = 0;
	mc->trans_bits = 0;
	mc->llist_bits = 0;
	mc->err_counter = 0;
	for(i = 0;i < UART_NR_MAX;i++){
		mc->ch_uart_tx[i] = DMA_INVALID_CH_MAGIC_NUMBER;
		mc->ch_uart_rx[i] = DMA_INVALID_CH_MAGIC_NUMBER;
	}
	chip->mc = mc;

	if(!chip->pdata){
		/*pdata get from here not form dts file,and fill with default value*/
		pdata = mc->pdata;
		pdata->chan_allocation_order = CHAN_ALLOCATION_ASCENDING;
		pdata->nr_channels = MC_DMA64_MAX_NR_CHANNELS;

	}else if(chip->pdata->nr_channels > MC_DMA64_MAX_NR_CHANNELS){
		err = -EINVAL;
		goto err_pdata;
	}
	else{
		memcpy(mc->pdata,chip->pdata,sizeof(*mc->pdata));
		pdata = mc->pdata;
	}

	mc->chan = devm_kcalloc(chip->dev,pdata->nr_channels,sizeof(*mc->chan),GFP_KERNEL);
	if(!mc->chan){
		err = -ENOMEM;
		goto err_pdata;
	}


	/*force dma off*/
	mc_dma_off(mc);

	/*create a pool of consistent memory blocks for hardware descriptor*/
	mc->desc_pool = dmam_pool_create("mc_dmac64_desc_pool",chip->dev,
			sizeof(struct mc_desc),8,0);

	if(!mc->desc_pool){
		dev_err(chip->dev,"No memory for descriptors dma pool\r\n");
		err = -ENOMEM;
		goto err_pdata;
	}
	tasklet_init(&mc->tasklet, mc_dma_intrc_tasklet, (unsigned long)mc);
	//	tasklet_init(&mc->tasklet2, mcc_dma_llist_process_tasklet, (long unsigned int)mc);
	err = request_irq(chip->irq,mc_dma64_interrupt,IRQF_SHARED,"mc_dma64",mc);
	if(err)
		goto err_pdata;

	INIT_LIST_HEAD(&mc->dma.channels);
	for(i = pdata->nr_channels-1;i >= 0 ;i--){

		struct mc_dma_chan	*mcc = &mc->chan[i];
		mcc->chan.device = &mc->dma;
		mcc->chan.chan_id = i;
		mcc->chan.free_flag = FREE_MAGIC_NUMBER;
		mcc->chan.validation_flag = NOT_VALIDATION_MAGIC_NUMBER;

		/*TODE check if we need cookie*/
		dma_cookie_init(&mcc->chan);
		if(pdata->chan_allocation_order==CHAN_ALLOCATION_ASCENDING)
			list_add_tail(&mcc->chan.device_node,
					&mc->dma.channels);
		else
			list_add(&mcc->chan.device_node,&mc->dma.channels);

		mcc->ch_regs = mc->chan_regs + i*MC_CHAN_REGS_OFFSET;
		mutex_init(&mcc->chan_mutex);
		spin_lock_init(&mcc->lock);

		mcc->mask = 1<<i;

		INIT_LIST_HEAD(&mcc->active_list);
		INIT_LIST_HEAD(&mcc->queue);
		/*disable channel*/
		channel_disable(mcc);

		/*if llp is support*/
		mcc->nollp = true;
		mcc->sflag = 0;
		mcc->resume_flag = 0;
		atomic_set(&mcc->descs_allocated, 0);
	}

	/*clear all interrupts on all channels*/
	clear_interrupt_all_chan(mc);

	/*set capabilities*/
	dma_cap_set(DMA_SLAVE,mc->dma.cap_mask);
	dma_cap_set(DMA_SG,mc->dma.cap_mask);
	dma_cap_set(DMA_CYCLIC,mc->dma.cap_mask);
	dma_cap_set(DMA_MEMCPY,mc->dma.cap_mask);


	mc->dma.dev = chip->dev;
	mc->dma.device_alloc_chan_resources = mc_alloc_chan_resources;
	mc->dma.device_free_chan_resources = mc_free_chan_resources;

	mc->dma.device_prep_dma_memcpy = mc_prep_dma_memcpy;
	mc->dma.device_prep_slave_sg = mc_prep_slave_sg;
	mc->dma.device_prep_dma_sg = mc_prep_dma_sg;
	mc->dma.device_prep_dma_cyclic = mc_dma_cyclic_prep;

	mc->dma.device_config = mc_config;
	mc->dma.device_pause = mc_pause;
	mc->dma.device_resume = mc_resume;
	mc->dma.device_issue_pending = mc_dma_start;
	mc->dma.device_cyclic_free = mc_dma_cyclic_free;
	mc->dma.device_intrc_disable= mc_dma_intrc_disable;
	mc->dma.device_chan_enable = mc_dma_chan_enable;
	mc->dma.device_chan_req = mc_dma_chan_req;
	mc->dma.device_chan_read_dest_addr = mc_dma_read_dest_addr;
	mc->dma.device_chan_read_trsc_len = mc_dma_read_trsc_len;
	mc->dma.device_chan_write_dest_addr = mc_dma_write_dest_addr;
	mc->dma.device_chan_write_trsc_len = mc_dma_write_trsc_len;
	mc->dma.device_chan_read_ch_cfg = mc_dma_read_cfg;
	mc->dma.device_start_direct = mcc_start_direct;
	mc->dma.device_chan_cyclic_desc_set = mc_cyclic_desc_set;
	/*dma capabilities*/

	mc->dma.src_addr_widths = MC_DMA_BUSWIDTHS;
	mc->dma.dst_addr_widths = MC_DMA_BUSWIDTHS;
	mc->dma.directions = BIT(DMA_DEV_TO_MEM)|BIT(DMA_MEM_TO_DEV)|
		BIT(DMA_MEM_TO_MEM);

	mc->dma.residue_granularity = DMA_RESIDUE_GRANULARITY_DESCRIPTOR;

	err = dma_async_device_register(&mc->dma);
	if(err)
		goto err_dma_register;
	dev_info(chip->dev,"Molchip DMA Controller,%d channels\n",pdata->nr_channels);

	return 0;


err_dma_register:
	free_irq(chip->irq,mc);

err_pdata:
	return err;

}
EXPORT_SYMBOL_GPL(mc_dma64_probe);

int mc_dma_remove(struct mc_dma64_chip *chip){
	struct mc_dma		*mc = chip->mc;
	struct mc_dma_chan	*mcc, *_mcc;

	pm_runtime_get_sync(chip->dev);
	mc_dma_off(mc);
	dma_async_device_unregister(&mc->dma);

	tasklet_kill(&mc->tasklet);
	free_irq(chip->irq,mc);
	list_for_each_entry_safe(mcc,_mcc,&mc->dma.channels,
			chan.device_node){
		list_del(&mcc->chan.device_node);
		channel_disable(mcc);
	}
	pm_runtime_put_sync_suspend(chip->dev);
	return 0;
}
EXPORT_SYMBOL_GPL(mc_dma_remove);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("molchip dma64 core dirver");

