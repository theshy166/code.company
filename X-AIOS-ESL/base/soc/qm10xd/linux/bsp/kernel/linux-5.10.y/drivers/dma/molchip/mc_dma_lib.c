#include <linux/interrupt.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/regmap.h>
#include <linux/dma/mc_dma64.h>
#include "../dmaengine.h"
#include "molchip.h"
#include "mc_dma_lib.h"
#include "mc_dma64_reg.h"



struct  mc_dma_chan *to_mc_dma_chan(struct dma_chan *chan){
	return container_of(chan,struct mc_dma_chan,chan);

}

struct  mc_dma *to_mc_dma(struct dma_device *dev){
	return container_of(dev,struct mc_dma,dma);
}

struct mc_dma_module *mc_dma64_prep(struct device *dev){
	struct mc_dma_module *dma;
	dma = devm_kzalloc(dev,sizeof(*dma),GFP_KERNEL);
	if(!dma){
		dev_err(dev,"kzalloc memory for mc_dma_module failed\r\n");
		return NULL;
	}
	return dma;
}
EXPORT_SYMBOL_GPL(mc_dma64_prep);

static struct mc_desc *mcc_first_active(struct mc_dma_chan *mcc){
	return to_mc_desc(mcc->active_list.next);
}

struct  mc_desc *
txd_to_mc_desc(struct dma_async_tx_descriptor *txd){
	return container_of(txd,struct mc_desc,txd);

}

struct device *chan2dev(struct dma_chan *chan){
	return &chan->dev->device;
}

void mc_dma_off(struct mc_dma *mc){
}

void mc_dma_on(struct mc_dma *mc){
}

static u32  get_fragment_len(u32 len){

	u32 fg_len;
	if(len <= LEN_BOUND)
		fg_len = FRAGMENT_S;
	else{
		fg_len = FRAGMENT_B;
	}
	return fg_len;
}

static void mc_dma_chan_reg_init(struct mc_dma* mc){
	struct mc_dma_ch_regs   *ch_regs;
	u32 *ch_ptr;
	ch_regs = (struct mc_dma_ch_regs *)mc->chan_regs;
	ch_ptr = (u32*)ch_regs;
	memset((u8*)ch_regs,0,sizeof(*ch_regs)*MC_DMA64_MAX_NR_CHANNELS);
}

static void mc_dma_common_reg_init(struct mc_dma* mc){
	dma64_clk_gate_en(mc);
	dma64_frag_wait_set(mc,DMA_FRAG_WAIT);
}

static void mcc_do_single_block(struct mc_dma_chan *mcc,
		struct mc_desc *desc){

	dma64_ch_enable(mcc);
	dma64_priority_set(mcc);
	dma64_blk_len_set(mcc,BLK_LEN_DUMMY);
	dma64_trsc_len_set(mcc,TRSC_LEN_DUMMY);
	dma64_frag_len_set(mcc,FRAG_LEN_DUMMY);
	dma64_llist_en(mcc);
	dma64_llist_ptr_set(mcc,desc);
}

static void mcc_initialize(struct mc_dma_chan *mcc){
	struct 	mc_dma_ch_regs * ch_regs;
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;

	if (test_bit(MC_DMA_IS_INITALIZED, &mcc->flags))
		return;

	set_bit(MC_DMA_IS_INITALIZED, &mcc->flags);
}

static  void mcc_dump_chan_regs(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs *ch_regs;

	ch_regs = (struct mc_dma_ch_regs*)mcc->ch_regs;

	dev_err(chan2dev(&mcc->chan),
			"  SAR: 0x%x DAR: 0x%x LLP: 0x%x CFG: 0x%x FRAG_LEN:0x%x\n",
			ch_regs->full_channel_regs.full_chn_src_addr,
			ch_regs->full_channel_regs.full_chn_dest_addr,
			ch_regs->full_channel_regs.full_chn_llist_ptr,
			ch_regs->full_channel_regs.full_chn_cfg,
			ch_regs->full_channel_regs.full_chn_frag_len);
}

static dma_cookie_t mcc_tx_submit(struct dma_async_tx_descriptor *tx){
	struct mc_desc		*desc = txd_to_mc_desc(tx);
	struct mc_dma_chan	*mcc = to_mc_dma_chan(tx->chan);
	unsigned long 		flags;
	dma_cookie_t		cookie;
	/*if you submit tx ,repeatly in one channel*/
	spin_lock_irqsave(&mcc->lock,flags);
	cookie = dma_cookie_assign(tx);
	list_add_tail(&desc->desc_node,&mcc->queue);
	spin_unlock_irqrestore(&mcc->lock,flags);

	return cookie;
}

static struct mc_desc *mcc_desc_get(struct mc_dma_chan *mcc){
	struct mc_dma 	*mc = to_mc_dma(mcc->chan.device);
	struct mc_desc 	*desc;
	dma_addr_t 	phys;
	desc = dma_pool_zalloc(mc->desc_pool,GFP_ATOMIC,&phys);
	if(!desc)
	{
		return NULL;
	}

	atomic_inc(&mcc->descs_allocated);
	INIT_LIST_HEAD(&desc->tx_list);
	dma_async_tx_descriptor_init(&desc->txd,&mcc->chan);
	desc->txd.tx_submit = mcc_tx_submit;
	desc->txd.flags = DMA_PREP_INTERRUPT;
	desc->txd.phys = phys;
	return desc;
}

static void mcc_desc_put(struct mc_dma_chan *mcc,struct mc_desc *desc){

	struct mc_dma 	*mc = to_mc_dma(mcc->chan.device);
	struct mc_desc 	*child,*_next;
	if(unlikely(!desc))
		return;

	list_for_each_entry_safe(child,_next,&desc->tx_list,desc_node){
		list_del(&child->desc_node);
		dma_pool_free(mc->desc_pool,child,child->txd.phys);
		atomic_dec(&mcc->descs_allocated);
	}

	dma_pool_free(mc->desc_pool,desc,desc->txd.phys);
	atomic_dec(&mcc->descs_allocated);
}

void channel_disable(struct mc_dma_chan *mcc){
	struct mc_dma_ch_regs  *ch_regs;

	ch_regs =(struct mc_dma_ch_regs*)mcc->ch_regs;
	/*disable the dma channel*/
	ch_regs->full_channel_regs.full_chn_cfg &= ~CH_ENABLE;
}

void clear_interrupt_all_chan(struct mc_dma *mc){
	u32 i;
	struct mc_dma_ch_regs 	*ch_regs;
	for(i = 0;i<MC_DMA64_MAX_NR_CHANNELS;i++)
	{
		ch_regs = (struct mc_dma_ch_regs*)(mc->chan_regs + i*MC_CHAN_REGS_OFFSET);
		ch_regs->full_channel_regs.full_chn_int |= CH_ALLINTRC_CLR_MASK;
	}
}

int mc_alloc_chan_resources(struct dma_chan *chan){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	struct mc_dma		*mc = to_mc_dma(chan->device);
	u32 			value;

	mutex_lock(&mcc->chan_mutex);
	value = dma64_read_reg_cfg(mcc);
	if(value & CH_ENABLE){
		dev_dbg(chan2dev(chan),"DMA channel not idle?\n");
		return  -EBUSY;
	}

	if(chan->free_flag == FREE_MAGIC_NUMBER)
		chan->free_flag = NOT_FREE_MAGIC_NUMBER;
	else if(chan->free_flag == NOT_FREE_MAGIC_NUMBER){
		dev_dbg(chan2dev(chan),"DMA channel not free\n");
		return -EPERM;
	}	
	else{
		dev_dbg(chan2dev(chan),"dma chan memory is modified in accident\n");
		return -EINVAL;
	}
	chan->validation_flag = VALIDATION_MAGIC_NUMBER;

	dma_cookie_init(chan);
	chan->validation_flag = VALIDATION_MAGIC_NUMBER;
	/*enable controller here if needed*/
	if(!mc->in_use)
	{
		/*dma chan reg init to 0*/
		mc_dma_chan_reg_init(mc);

		/*dma reset*/
		//	mc_dma_ahb_reset(mc);

		/*init common regs*/
		mc_dma_common_reg_init(mc);
	}
	mc->in_use |= mcc->mask;

	dma64_intrc_disable_all(mcc);
	mutex_unlock(&mcc->chan_mutex);
	return 0;
}

void mc_free_chan_resources(struct dma_chan *chan){

	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	struct mc_dma		*mc = to_mc_dma(chan->device);
	u32 value;
	LIST_HEAD(list);

	mutex_lock(&mcc->chan_mutex);
	BUG_ON(!list_empty(&mcc->active_list));
	BUG_ON(!list_empty(&mcc->queue));
	BUG_ON(chan->validation_flag == NOT_VALIDATION_MAGIC_NUMBER);
	BUG_ON(chan->validation_flag != NOT_VALIDATION_MAGIC_NUMBER &&
			chan->validation_flag != VALIDATION_MAGIC_NUMBER);

	if(chan->validation_flag == VALIDATION_MAGIC_NUMBER)
		chan->validation_flag = NOT_VALIDATION_MAGIC_NUMBER;

	chan->free_flag = FREE_MAGIC_NUMBER;
	value = dma64_read_reg_cfg(mcc);
	BUG_ON(value & CH_ENABLE);


	/*clear custom channel configuration*/
	memset(&mcc->dma_sconfig,0,sizeof(struct dma_slave_config));
	clear_bit(MC_DMA_IS_INITALIZED,&mcc->flags);

	dma64_intrc_disable_all(mcc);

	/*disable controller in case it was a last user*/
	mc->in_use &= ~mcc->mask;
	if(!mc->in_use)
		mc_dma_off(mc);

	mutex_unlock(&mcc->chan_mutex);
}

#if 0 
static void desc_dump(struct mc_desc* desc){

	printk("//////////////////////////////////////////////////\r\n");
	printk("desc->lli.SRC_ADDR = 0x%x\r\n",desc->lli.SRC_ADDR);
	printk("desc->lli.DEST_ADDR = 0x%x\r\n",desc->lli.DEST_ADDR);
	printk("desc->lli.FRAG_LEN =0x%x\r\n",desc->lli.FRAG_LEN);
	printk("desc->lli.BLK_LEN =0x%x\r\n",desc->lli.BLK_LEN);
	printk("desc->lli.TRANS_LEN =0x%x\r\n",desc->lli.TRANS_LEN);
	printk("desc->lli.TRSF_STEP= 0x%x\r\n",desc->lli.TRSF_STEP);
	printk("desc->lli.WRAP_PTR = 0x%x\r\n",desc->lli.WRAP_PTR);
	printk("desc->lli.WRAP_TO = 0x%x\r\n",desc->lli.WRAP_TO);
	printk("desc->lli.LLIST_PTR = 0x%x\r\n",desc->lli.LLIST_PTR);
	printk("desc->lli.FRAG_STEP = 0x%x\r\n",desc->lli.FRAG_STEP);
	printk("desc->lli.SRC_BLK_STEP = 0x%x\r\n",desc->lli.SRC_BLK_STEP);
	printk("desc->lli.DEST_BLK_STEP=0x%x\r\n",desc->lli.DEST_BLK_STEP);

	printk("////////////////////////////////////////////////////\r\n");

	printk("desc->desc_node= 0x%x\r\n",(u32)&desc->desc_node);
	printk("desc->tx_list=0x%x\r\n",(u32)&desc->tx_list);
	printk("desc->txd = 0x%x\r\n",(u32)&desc->txd);
	printk("desc->len=0x%x\r\n",desc->len);
	printk("desc->total_len=0x%x\r\n",desc->total_len);
	printk("desc->residue = 0x%x\r\n",desc->residue);


}
#endif

/*use lli  to process the mem2mem transfer*/
struct dma_async_tx_descriptor *
mc_prep_dma_memcpy(struct dma_chan *chan,dma_addr_t dest,dma_addr_t src,
		size_t len,unsigned long flags){

	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	struct mc_desc		*desc;
	u32 			frag_len;
	u32 			blk_len;

	if(unlikely(!len)){
		dev_dbg(chan2dev(chan),"%s:length is zero!\n",__func__);
		return NULL;
	}

	frag_len = get_fragment_len(len);
	blk_len = frag_len;
	desc = mcc_desc_get(mcc);
	if(!desc)
		goto err_desc_get;
	/*config src addr ,dest addr*/
	dma64_set_src(desc,src);
	dma64_set_dst(desc,dest);
	/*config transfer len*/
	dma64_set_trsc_len(desc,len);

	if(flags & DMA_PREP_INTERRUPT){
		dma64_set_llist_node_intrc_en_enable(desc,CHN_TRSC_LEN_LLIST_NODE_INT_TRSC);
	}

	/*config block len*/
	dma64_set_blk_len(desc,blk_len);
	/*config fraglen reg*/
	dma64_set_frag_len(desc,frag_len);
	/*req mode*/
	dma64_set_req_mode(desc,LINKLIST);
	/*src ahb size ,dest ahb size*/
	dma64_set_ahbsize(desc,WORD,WORD);
	/*fix addr disable*/
	dma64_fix_disable(desc);
	/*wrap disable*/
	dma64_wrap_disable(desc);
	/*trsp step*/
	dma64_set_trsf_step(desc,WORD_STEP,WORD_STEP);
	/*step config*/
	dma64_set_step(desc,0,0,0);
	/*only one list node ,so the node is end*/
	dma64_set_llist_end(desc,LLIST_END);
	desc->txd.flags = flags;
	desc->total_len = len;

	return &desc->txd;

err_desc_get:
	mcc_desc_put(mcc,desc);
	return 0;
}

/*sg_len :means sg nubmer
 * */
struct dma_async_tx_descriptor *
mc_prep_slave_sg(struct dma_chan *chan,struct scatterlist *sgl,
		unsigned int sg_len, enum dma_transfer_direction direction,
		unsigned long flags,void *context){

	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	struct dma_slave_config	*sconfig = &mcc->dma_sconfig;
	struct mc_desc		*prev;
	struct mc_desc		*first;
	struct scatterlist	*sg;
	dma_addr_t 		reg;
	u32 		reg_width;
	u32 		mem_width;
	u32 		i;
	size_t			total_len = 0;

	if(unlikely(!is_slave_direction(direction) || !sg_len))
		return NULL;

	prev = first = NULL;

	switch(direction){
		case DMA_MEM_TO_DEV:
			reg_width = sconfig->dst_addr_width;
			mem_width = sconfig->src_addr_width;
			reg = sconfig->dst_addr;
			for_each_sg(sgl,sg,sg_len,i){
				struct mc_desc	*desc;
				u32 		len,mem;
				mem = sg_dma_address(sg);
				len = sg_dma_len(sg);
				if(!len)
					goto err_len_set;
				desc =mcc_desc_get(mcc);
				if(!desc)
					goto err_desc_get;
				if(flags & DMA_CTRL_REUSE){
					mcc->desc = desc;
				}
				/*config src dest address*/
				dma64_set_src(desc,mem);
				dma64_set_dst(desc,reg);
				/*config transfer len*/
				dma64_set_trsc_len(desc,len);
				if(flags & DMA_PREP_INTERRUPT){
					dma64_set_llist_node_intrc_en_disable(desc);
					dma64_interrupt_en(mcc,CH_LLIST_INT_EN);
				}
				/*config block len*/
				dma64_set_blk_len(desc,sconfig->fragmentlen);
				/*config frag len*/
				dma64_set_frag_len(desc,sconfig->fragmentlen);
				/*req mode*/
				dma64_set_req_mode(desc,FRAGMENT);
				/*src ahb size ,dest ahb size*/
				dma64_set_ahbsize(desc,mem_width,reg_width);
				/*fix add enable*/
				dma64_fix_enable(desc,DST_FIX);
				/*wrap disable*/
				dma64_wrap_disable(desc);
				/*trsp step*/
				dma64_set_trsf_step(desc,1<<mem_width,1<<reg_width);
				/*trsp step*/
				dma64_set_step(desc,0,0,0);
				if(i < sg_len-1){
					dma64_set_llist_end(desc,NOT_LLIST_END);
				}

				if(!first){
					first = desc;
				}else{
					dma64_set_llist_ptr(prev,desc->txd.phys);
					list_add_tail(&desc->desc_node,&first->tx_list);
				}
				prev = desc;
				total_len += len;
			}
			dma64_set_llist_ptr(prev,0UL);
			dma64_set_llist_end(prev,LLIST_END);

			break;

		case DMA_DEV_TO_MEM:
			reg_width = sconfig->src_addr_width;
			mem_width = sconfig->dst_addr_width;
			reg = sconfig->src_addr;
			for_each_sg(sgl,sg,sg_len,i){
				struct mc_desc	*desc;
				u32 		len,mem;
				mem = sg_dma_address(sg);
				len = sg_dma_len(sg);
				if(!len)
					goto err_len_set;
				desc =mcc_desc_get(mcc);
				if(!desc)
					goto err_desc_get;
				if(flags & DMA_CTRL_REUSE){
					mcc->desc = desc;
					dma64_interrupt_en(mcc,CH_FRAGMENT_INT_EN);
				}
				/*config src dest address*/
				dma64_set_src(desc,reg);
				dma64_set_dst(desc,mem);
				/*config transfer len*/
				dma64_set_trsc_len(desc,len);
				if(flags & DMA_PREP_INTERRUPT){
					//dma64_set_llist_node_intrc_en_enable(desc,CHN_TRSC_LEN_LLIST_NODE_INT_TRSC);
					dma64_set_llist_node_intrc_en_disable(desc);
					dma64_interrupt_en(mcc,CH_LLIST_INT_EN);
				}
				/*config block len*/
				dma64_set_blk_len(desc,sconfig->fragmentlen);
				/*config frag len*/
				dma64_set_frag_len(desc,sconfig->fragmentlen);
				/*req mode*/
				dma64_set_req_mode(desc,FRAGMENT);
				/*src ahb size ,dest ahb size*/
				dma64_set_ahbsize(desc,reg_width,mem_width);
				/*fix add enable*/
				dma64_fix_enable(desc,SRC_FIX);
				/*wrap disable*/
				dma64_wrap_disable(desc);
				/*trsp step*/
				dma64_set_trsf_step(desc,1<<reg_width,1<<mem_width);
				/*trsp step*/
				dma64_set_step(desc,0,0,0);

				if(i < sg_len-1){
					dma64_set_llist_end(desc,NOT_LLIST_END);
				}

				if(!first){
					first = desc;
				}else{
					dma64_set_llist_ptr(prev,desc->txd.phys);
					list_add_tail(&desc->desc_node,&first->tx_list);
				}
				prev = desc;
				total_len += len;
			}
			dma64_set_llist_ptr(prev,0UL);
			dma64_set_llist_end(prev,LLIST_END);
			//		desc_dump(prev);
			break;
		default:
			return NULL;
	}
	first->total_len = total_len;
	return &first->txd;

err_desc_get:
	dev_err(chan2dev(chan),"not enough descriptors available,Direction %d\n",direction);
	mcc_desc_put(mcc,first);
	return NULL;
err_len_set:
	dev_err(chan2dev(chan),"err trans len set,trans len = 0\n");
	return NULL;
}

/**
 * mc_dma_cyclic_prep - prepare the cyclic DMA transfer
 * @chan: the DMA channel to prepare
 * @buf_addr: physical DMA address where the buffer starts
 * @buf_len: total number of bytes for the entire buffer
 * @period_len: number of bytes for each period
 * @direction: transfer direction, to or from device
 *
 * Must be called before trying to start the transfer. Returns a valid struct
 * mc_cyclic_desc if successful or an ERR_PTR(-errno) if not successful.
 */
struct mc_cyclic_desc *mc_dma_cyclic_prep(struct dma_chan *chan,
		dma_addr_t buf_addr, size_t buf_len, size_t period_len,
		enum dma_transfer_direction direction,unsigned long flags){

	struct mc_dma_chan		*mcc = to_mc_dma_chan(chan);
	struct dma_slave_config		*sconfig = &mcc->dma_sconfig;
	struct mc_cyclic_desc		*cdesc;
	struct mc_cyclic_desc		*retval = NULL;
	struct mc_desc			*desc;
	struct mc_desc			*last = NULL;
	unsigned long			was_cyclic;
	u32 				src_addr;
	u32 				dst_addr;
	u32				reg_width;
	u32 				mem_width;
	u32				periods;
	u32				i;

	if (!mcc->nollp) {
		dev_dbg(chan2dev(&mcc->chan),
				"channel doesn't support LLP transfers\n");
		return ERR_PTR(-EINVAL);
	}

	if (!list_empty(&mcc->queue) || !list_empty(&mcc->active_list)) {
		dev_dbg(chan2dev(&mcc->chan),
				"queue and/or active list are not empty\n");
		return ERR_PTR(-EBUSY);
	}

	was_cyclic = test_and_set_bit(MC_DMA_IS_CYCLIC, &mcc->flags);
	if (was_cyclic) {
		dev_dbg(chan2dev(&mcc->chan),
				"channel already prepared for cyclic DMA\n");
		return ERR_PTR(-EBUSY);
	}

	retval = ERR_PTR(-EINVAL);

	if (unlikely(!is_slave_direction(direction)))
		goto out_err;

	if (direction == DMA_MEM_TO_DEV){
		reg_width = sconfig->dst_addr_width;
		mem_width = sconfig->src_addr_width;
	}
	else{
		reg_width = sconfig->src_addr_width;
		mem_width = sconfig->dst_addr_width;
	}
	periods = buf_len / period_len;
	mcc->periods = periods;

	/* Check for unaligned periods and unaligned DMA buffer. */
	if (unlikely(period_len & ((1 << reg_width) - 1)))
		goto out_err;
	if (unlikely(buf_addr & ((1 << reg_width) - 1)))
		goto out_err;

	retval = ERR_PTR(-ENOMEM);

	cdesc = kzalloc(sizeof(struct mc_cyclic_desc), GFP_KERNEL);
	if (!cdesc)
		goto out_err;

	cdesc->desc = kzalloc(sizeof(struct mc_desc *) * periods, GFP_KERNEL);
	if (!cdesc->desc)
		goto out_err_alloc;

	for (i = 0; i < periods; i++) {
		desc = mcc_desc_get(mcc);
		if (!desc)
			goto out_err_desc_get;

		switch (direction) {
			case DMA_MEM_TO_DEV:
				/*config src dest address*/
				src_addr = buf_addr + period_len * i;
				dst_addr = sconfig->dst_addr;
				dma64_set_src(desc,src_addr);
				dma64_set_dst(desc,dst_addr);
				/*config transfer len*/
				dma64_set_trsc_len(desc,period_len);
				if(flags & DMA_PREP_INTERRUPT){
					dma64_set_llist_node_intrc_en_disable(desc);
					dma64_interrupt_en(mcc,CH_TRSC_INT_EN);
				}
				/*config block len*/
				dma64_set_blk_len(desc,sconfig->fragmentlen);
				/*config frag len*/
				dma64_set_frag_len(desc,sconfig->fragmentlen);
				/*req mode*/
				dma64_set_req_mode(desc,FRAGMENT);
				/*src ahb size ,dest ahb size*/
				dma64_set_ahbsize(desc,mem_width,reg_width);
				/*fix add enable*/
				dma64_fix_enable(desc,DST_FIX);
				/*wrap disable*/
				dma64_wrap_disable(desc);
				/*trsp step*/
				dma64_set_trsf_step(desc,1<<mem_width,1<<reg_width);
				/*trsp step*/
				dma64_set_step(desc,0,0,0);
				/*not the llist end*/
				dma64_set_llist_end(desc,NOT_LLIST_END);
				break;
			case DMA_DEV_TO_MEM:
				/*config src dest address*/
				src_addr = sconfig->src_addr; 
				dst_addr = buf_addr + period_len * i;
				dma64_set_src(desc,src_addr);
				dma64_set_dst(desc,dst_addr);
				/*config transfer len*/
				dma64_set_trsc_len(desc,period_len);
				if(flags & DMA_PREP_INTERRUPT){
					dma64_set_llist_node_intrc_en_disable(desc);
					dma64_interrupt_en(mcc,CH_TRSC_INT_EN);
				}

				/*config block len*/
				dma64_set_blk_len(desc,sconfig->fragmentlen);
				/*config frag len*/
				dma64_set_frag_len(desc,sconfig->fragmentlen);
				/*req mode*/
				dma64_set_req_mode(desc,FRAGMENT);
				/*src ahb size ,dest ahb size*/
				dma64_set_ahbsize(desc,reg_width,mem_width);
				/*fix add enable*/
				dma64_fix_enable(desc,SRC_FIX);
				/*wrap disable*/
				dma64_wrap_disable(desc);
				/*trsp step*/
				dma64_set_trsf_step(desc,1<<reg_width,1<<mem_width);
				/*trsp step*/
				dma64_set_step(desc,0,0,0);
				/*not the llist end*/
				dma64_set_llist_end(desc,NOT_LLIST_END);
				break;
			default:
				break;
		}
		mcc->buf_will_done = buf_addr;
		mcc->first_will_done = buf_addr;
		mcc->period_len = period_len;
		mcc->bwd_index = 0;

		cdesc->desc[i] = desc;

		if (last)
			dma64_set_llist_ptr(last,desc->txd.phys);
		last = desc;

	}

	/* Let's make a cyclic list */
	dma64_set_llist_ptr(last,cdesc->desc[0]->txd.phys);
	dev_dbg(chan2dev(&mcc->chan),
			"cyclic prepared buf %pad len %zu period %zu periods %d\n",
			&buf_addr, buf_len, period_len, periods);

	cdesc->periods = periods;
	mcc->cdesc = cdesc;
	mcc->chan.dma_config.priority = PRIORITY_LEVEL_3;

	return cdesc;

out_err_desc_get:
	while (i--)
		mcc_desc_put(mcc, cdesc->desc[i]);
out_err_alloc:
	kfree(cdesc);
out_err:
	clear_bit(MC_DMA_IS_CYCLIC, &mcc->flags);
	return (struct mc_cyclic_desc *)retval;
}

static void mc_dma_prep_cpy_desc(struct mc_dma_chan *mcc,
		struct mc_desc *new_desc,
		dma_addr_t dest,dma_addr_t src,
		size_t len,unsigned long flags){

	struct mc_desc		*desc = new_desc;
	u32			frag_len;
	u32			blk_len;

	if(unlikely(!len)){
		dev_dbg(chan2dev(&mcc->chan),"%s:length is zero!\n",__func__);
	}
	frag_len = get_fragment_len(len);
	blk_len = frag_len;
	/*config src dest address*/
	dma64_set_src(desc,src);
	dma64_set_dst(desc,dest);
	/*config transfer len*/
	dma64_set_trsc_len(desc,len);
	if(flags & DMA_PREP_INTERRUPT){
		//		dma64_set_llist_node_intrc_en_enable(desc,CHN_TRSC_LEN_LLIST_NODE_INT_TRSC);
		dma64_set_llist_node_intrc_en_disable(desc);
		dma64_interrupt_en(mcc,CH_LLIST_INT_EN);
	}
	/*config block len*/
	dma64_set_blk_len(desc,blk_len);
	/*config frag len*/
	dma64_set_frag_len(desc,frag_len);
	/*req mode*/
	dma64_set_req_mode(desc,LINKLIST);
	/*src ahb size ,dest ahb size*/
	dma64_set_ahbsize(desc,WORD,WORD);
	/*fix add enable*/
	dma64_fix_disable(desc);
	/*wrap disable*/
	dma64_wrap_disable(desc);
	/*trsp step*/
	dma64_set_trsf_step(desc,WORD_STEP,WORD_STEP);
	/*trsp step*/
	dma64_set_step(desc,0,0,0);
	/*only one list node ,default  the node is not the end*/
	dma64_set_llist_end(desc,NOT_LLIST_END);
	desc->txd.flags = flags;
	desc->total_len = len;
}

struct dma_async_tx_descriptor *mc_prep_dma_sg(struct dma_chan *chan,
		struct scatterlist *dst_sg,unsigned int dst_nents,
		struct scatterlist *src_sg,unsigned int src_nents,
		unsigned long flags ){

	struct mc_desc 		*first = NULL, *desc = NULL ,*prev = NULL;
	struct mc_dma_chan  	*mcc;
	size_t 			dst_avail, src_avail;
	dma_addr_t 		dst, src;
	size_t 			len;

	if (unlikely(!chan))
		return NULL;

	if (unlikely(!dst_nents || !src_nents))
		return NULL;

	if (unlikely(!dst_sg || !src_sg))
		return NULL;

	mcc = to_mc_dma_chan(chan);
	/* Get prepared for the loop */
	dst_avail = sg_dma_len(dst_sg);
	src_avail = sg_dma_len(src_sg);
	dst_nents--;
	src_nents--;

	/* Run until we are out of scatterlist entries */
	while (true) {
		/* Create the largest transaction possible */
		len = min_t(size_t, src_avail, dst_avail);
		if (len == 0)
			goto fetch;

		dst = sg_dma_address(dst_sg) + sg_dma_len(dst_sg) - dst_avail;
		src = sg_dma_address(src_sg) + sg_dma_len(src_sg) - src_avail;

		/* Allocate the link descriptor from DMA pool */
		desc = mcc_desc_get(mcc);
		if (!desc)
			goto out_err_desc_get;

		/* Prepare DMA descriptor */
		mc_dma_prep_cpy_desc(mcc, desc, dst, src, len,flags);
		desc->txd.cookie = 0;

		/* update metadata */
		dst_avail -= len;
		src_avail -= len;

		if (!first)
			first = desc;
		else{
			dma64_set_llist_ptr(prev,desc->txd.phys);
			/* Insert the link descriptor to the LD ring */
			list_add_tail(&desc->desc_node, &first->tx_list);
			//	desc_dump(prev);
		}


		prev = desc;
fetch:
		/* fetch the next dst scatterlist entry */
		if (dst_avail == 0) {
			/* no more entries: we're done */
			if (dst_nents == 0)
				break;

			/* fetch the next entry: if there are no more: done */
			dst_sg = sg_next(dst_sg);
			if (!dst_sg)
				break;

			dst_nents--;
			dst_avail = sg_dma_len(dst_sg);
		}

		/* fetch the next src scatterlist entry */
		if (src_avail == 0) {
			/* no more entries: we're done */
			if (src_nents == 0)
				break;

			/* fetch the next entry: if there are no more: done */
			src_sg = sg_next(src_sg);
			if (!src_sg)
				break;

			src_nents--;
			src_avail = sg_dma_len(src_sg);
		}
	}
	/*if the sg node 's transfer size == 0',then prev and desc may be null,but it is impossible
	 * but Coverity may scan this as FORWARD_NULL */
	if(prev){
		dma64_set_llist_ptr(prev,0UL);
	}

	if(desc){
		dma64_set_llist_end(desc,LLIST_END);
	}

	//	desc_dump(prev);
	return &first->txd;
out_err_desc_get:
	if (!first)
		return NULL;

	mcc_desc_put(mcc,first);
	return NULL;

}

int mc_config(struct dma_chan *chan,struct dma_slave_config *sconfig){

	struct mc_dma_chan 	*mcc = to_mc_dma_chan(chan);

	if(!is_slave_direction(sconfig->direction))
		return -EINVAL;

	memcpy(&mcc->dma_sconfig,sconfig,sizeof(*sconfig));

	return 0;
}

int mc_pause(struct dma_chan *chan){

	struct mc_dma_chan 	*mcc = to_mc_dma_chan(chan);
	u32 		count = 20;

	mutex_lock(&mcc->chan_mutex);
	dma64_ch_pause(mcc);
	while(!(dma64_read_reg_ch_pause(mcc)&CH_PAUSE_STATE) && count--)
		udelay(2);
	set_bit(MC_DMA_IS_PAUSED,&mcc->flags);
	mutex_unlock(&mcc->chan_mutex);

	return 0;
}

static  void mcc_chan_resume(struct mc_dma_chan *mcc){

	u32 		count = 20;

	dma64_ch_enable(mcc);
	dma64_ch_resume(mcc);
	mcc->resume_flag = 1;

	while(dma64_read_reg_ch_pause(mcc)&CH_PAUSE_STATE && count--)
		udelay(2);
	clear_bit(MC_DMA_IS_PAUSED,&mcc->flags);

}

int mc_resume(struct dma_chan *chan){

	struct mc_dma_chan	 *mcc = to_mc_dma_chan(chan);
	struct mc_dma_ch_regs	 *ch_regs;

	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;

	mutex_lock(&mcc->chan_mutex);

	if(test_bit(MC_DMA_IS_PAUSED,&mcc->flags))
	{
		mcc_chan_resume(mcc);
	}

	mutex_unlock(&mcc->chan_mutex);
	return 0;
}

void mcc_descriptor_reuse_complete(struct mc_dma_chan *mcc,struct mc_desc *desc,
		bool callback_required){
	struct dma_async_tx_descriptor *txd = &desc->txd;
	struct dmaengine_desc_callback	cb;

	if(callback_required)
		dmaengine_desc_get_callback(txd,&cb);
	else
		memset(&cb,0,sizeof(cb));

	dmaengine_desc_callback_invoke(&cb,NULL);
	clear_bit(DMA_IN_PROGRESS,&mcc->sflag);
}

static void mcc_descriptor_complete(struct mc_dma_chan *mcc,struct mc_desc *desc,
		bool callback_required){

	struct dma_async_tx_descriptor *txd = &desc->txd;
	struct dmaengine_desc_callback	cb;

	dma_cookie_complete(txd);
	if(callback_required)
		dmaengine_desc_get_callback(txd,&cb);
	else
		memset(&cb,0,sizeof(cb));

	mcc_desc_put(mcc,desc);

	dmaengine_desc_callback_invoke(&cb,NULL);
}

void mcc_start_direct(struct dma_chan *chan){

	struct mc_dma		*mc = to_mc_dma(chan->device);
	struct mc_dma_chan	*mcc;
	struct mc_dma_ch_regs 	*ch_regs;
	struct mc_desc		*desc;
	u32			 *reqcidRegAddr;
	u32 			value;

	mcc = to_mc_dma_chan(chan);
	while(test_and_set_bit(DMA_IN_PROGRESS,&mcc->sflag) == 1);
	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;
	desc = mcc->desc;
	value = dma64_read_reg_cfg(mcc);
	if(value&CH_ENABLE){
		dev_err(chan2dev(&mcc->chan),
				"%s: BUG: Attempted to start non-idle channel\n",
				__func__);
		mcc_dump_chan_regs(mcc);
		return;
	}

	value = dma64_get_trsc_len(desc);
	if(!value){
		dev_err(chan2dev(&mcc->chan),
				"BUG: Attempted to do a trsc len = 0 dma transfer\n");
		return;
	}


	if (mcc->nollp) {
		dma64_llist_en(mcc);
		dma64_llist_ptr_set(mcc,desc);
		dma64_ch_enable(mcc);

		if((mcc->dma_sconfig.direction == DMA_MEM_TO_DEV || mcc->dma_sconfig.direction== DMA_DEV_TO_MEM)&&
				(mcc->dma_sconfig.handshake == HARDWARE_HANDSHAKE)){
			reqcidRegAddr =(u32*)(mc->req_cid_regs + (mcc->dma_sconfig.slave_id)*4);
			*reqcidRegAddr = mcc->chan.chan_id+1;
		}
		else if(mcc->dma_sconfig.direction == DMA_MEM_TO_MEM ||mcc->dma_sconfig.handshake == SOFTWARE_HANDSHAKE){
			ch_regs->full_channel_regs.full_chn_req |= REQ_DMA_SET;
		}

		return;
	}
}

static void mcc_dostart(struct mc_dma_chan *mcc, struct mc_desc *first){
	struct mc_dma		*mc = to_mc_dma(mcc->chan.device);
	struct mc_dma_ch_regs 	*ch_regs;
	u32			 *reqcidRegAddr;
	u32 			value;
	u32			was_soft_llp;

	ch_regs = (struct mc_dma_ch_regs *)mcc->ch_regs;

	value = dma64_read_reg_cfg(mcc);
	if(value&CH_ENABLE){
		dev_err(chan2dev(&mcc->chan),
				"%s: BUG: Attempted to start non-idle channel\n",
				__func__);
		mcc_dump_chan_regs(mcc);
		return;
	}
	if (mcc->nollp) {
		was_soft_llp = test_and_set_bit(MC_DMA_IS_SOFT_LLP,
				&mcc->flags);
		if (was_soft_llp) {
			dev_err(chan2dev(&mcc->chan),
					"BUG: Attempted to start new LLP transfer inside ongoing one\n");
			return;
		}

		mcc_initialize(mcc);
		/* Submit first block */
		mcc_do_single_block(mcc, first);


		if((mcc->dma_sconfig.direction == DMA_MEM_TO_DEV || mcc->dma_sconfig.direction== DMA_DEV_TO_MEM)&&
				(mcc->dma_sconfig.handshake == HARDWARE_HANDSHAKE)){
			reqcidRegAddr =(u32*)(mc->req_cid_regs + (mcc->dma_sconfig.slave_id)*4);
			*reqcidRegAddr = mcc->chan.chan_id+1; 
		}
		else if(mcc->dma_sconfig.direction == DMA_MEM_TO_MEM){
			ch_regs->full_channel_regs.full_chn_req |= REQ_DMA_SET;
		}

		return;
	}
}


static void mcc_dostart_first_queued(struct mc_dma_chan *mcc){
	struct mc_desc *desc;
	unsigned long flags;

	spin_lock_irqsave(&mcc->lock,flags);
	if (list_empty(&mcc->queue)){
		spin_unlock_irqrestore(&mcc->lock,flags);
		return;
	}
	list_move(mcc->queue.next, &mcc->active_list);
	desc = mcc_first_active(mcc);
	spin_unlock_irqrestore(&mcc->lock,flags);
	mcc_dostart(mcc, desc);
}

void mcc_scan_descriptors(struct mc_dma *mc, struct mc_dma_chan *mcc){

	struct mc_desc *desc,*_desc;
	unsigned long flags;
	LIST_HEAD(list);
	/*protect between multithread*/	
	spin_lock_irqsave(&mcc->lock,flags);
	if (list_empty(&mcc->active_list)) {
		spin_unlock_irqrestore(&mcc->lock,flags);
		return;
	}
	spin_unlock_irqrestore(&mcc->lock,flags);
	/*No descriptors so far seem to be in progress, i.e
	 * this one must be done*/
	list_for_each_entry_safe(desc,_desc,&mcc->active_list,desc_node){
		mcc_descriptor_complete(mcc,desc,true);
	}
	spin_lock_irqsave(&mcc->lock,flags);
	list_splice_init(&mcc->active_list,&list);
	clear_bit(MC_DMA_IS_SOFT_LLP,&mcc->flags);
	clear_bit(DMA_IN_PROGRESS,&mcc->sflag);
	spin_unlock_irqrestore(&mcc->lock,flags);
	/*if queue has txd , the get the txd in the queue*/
	//	mcc_dostart_first_queued(mcc);
}
/**
 *mc_dma_cyclic_start - start the cyclic DMA transfer
 * @chan: the DMA channel to start
 *
 * Must be called with soft interrupts disabled. Returns zero on success or
 * -errno on failure.
 */
static int mc_dma_cyclic_start(struct dma_chan *chan){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	if (!test_bit(MC_DMA_IS_CYCLIC, &mcc->flags)) {
		dev_err(chan2dev(&mcc->chan), "missing prep for cyclic DMA\n");
		return -ENODEV;
	}

	mcc_dostart(mcc, mcc->cdesc->desc[0]);

	return 0;
}
/**
 * mc_dma_cyclic_free - free a prepared cyclic DMA transfer
 * @chan: the DMA channel to free
 */
void mc_dma_cyclic_free(struct dma_chan *chan){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	struct mc_cyclic_desc	*cdesc = mcc->cdesc;
	u32		i;

	dev_dbg(chan2dev(&mcc->chan), "%s\n", __func__);
	mutex_lock(&mcc->chan_mutex);

	if (!cdesc){
		mutex_unlock(&mcc->chan_mutex);
		return;
	}
	for (i = 0; i < cdesc->periods; i++)
		mcc_desc_put(mcc, cdesc->desc[i]);

	kfree(cdesc->desc);
	kfree(cdesc);
	mcc->cdesc = NULL;

	dma64_ch_disable(mcc);
	clear_bit(MC_DMA_IS_SOFT_LLP,&mcc->flags);
	clear_bit(MC_DMA_IS_CYCLIC, &mcc->flags);
	mutex_unlock(&mcc->chan_mutex);
}

void mc_dma_intrc_disable(struct dma_chan *chan){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	dma64_intrc_disable_all(mcc);
}

void mc_dma_chan_enable(struct dma_chan *chan){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	dma64_ch_enable(mcc);
}

u32 mc_dma_read_cfg(struct dma_chan *chan){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	return 	dma64_read_reg_cfg(mcc);
}

void mc_dma_chan_req(struct dma_chan *chan){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	dma64_send_software_request(mcc);
}

u32 mc_dma_read_dest_addr(struct dma_chan *chan){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	return dma64_read_reg_dest(mcc);
}

u32 mc_dma_read_trsc_len(struct dma_chan *chan){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	return dma64_read_trsc_len(mcc);
}

void mc_dma_write_dest_addr(struct dma_chan *chan,u32 val){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	dma64_write_reg_dest(mcc,val);
}

void mc_dma_write_trsc_len(struct dma_chan *chan,u32 val){
	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	dma64_write_trsc_len(mcc,val);
}

void mc_dma_start(struct dma_chan *chan){

	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);

	if (!test_bit(MC_DMA_IS_CYCLIC, &mcc->flags))
	{
		while(test_and_set_bit(DMA_IN_PROGRESS,&mcc->sflag) == 1);
		if (list_empty(&mcc->active_list)){
			mcc_dostart_first_queued(mcc);
		}
	}else
	{
		mc_dma_cyclic_start(chan);

	}
}

void mc_cyclic_desc_set(struct dma_chan *chan){

	struct mc_dma_chan	*mcc = to_mc_dma_chan(chan);
	dma64_llist_ptr_set(mcc,mcc->cdesc->desc[0]);
}
