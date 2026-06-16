/*
 * Platform driver for molchip  DMA64 Controller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/clk.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_dma.h>
#include <linux/mini_clock/clock.h>
#include <linux/delay.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>

#include "dma64.h"
#include "dmaengine.h"

#define DRV_NAME	"dma64"

#define DMA64_MAX	1

struct dma64	*dma64_device[DMA64_MAX];

/*
 * Initial number of descriptors to allocate for each channel. This could
 * be increased during dma usage.
 */
static unsigned int init_nr_desc_per_channel = 64;
module_param(init_nr_desc_per_channel, uint, 0644);
MODULE_PARM_DESC(init_nr_desc_per_channel,
		 "initial descriptors per channel (default: 64)");

 /* prototypes */
 static dma_cookie_t dma64_tx_submit(struct dma_async_tx_descriptor *tx);
 static void dma64_issue_pending(struct dma_chan *chan);

dma_addr_t get_dma_ch_dst_addr(int dma_id,unsigned int chn)
{
	 struct dma64  *dma64;
	 struct dma64_chan *dma64c;
	 u32 addr_l;
	 u32 addr_h;
	 dma_addr_t dst_addr;

	 if(unlikely((dma_id >= DMA64_MAX) || (chn >= 32))){
	  printk(KERN_ERR "parameter error\n");
	  return -1;
	 }

	 dma64 = dma64_device[dma_id];
	 dma64c = &dma64->chan[chn];
	 //  vdbg_dump_regs(dma64c);

	 addr_l = channel_readl(dma64c,DADDR_REG);
	 addr_h = channel_readl(dma64c,WRAPT_REG);
#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
	addr_h >>= DMA64C_DST_ADDR_H_OFFSET;
	addr_h &= 0xF;
	dst_addr =((dma_addr_t)addr_h << 32) | (dma_addr_t)addr_l;
#else
	dst_addr = (dma_addr_t)addr_l;
#endif
	 return dst_addr;
}
 
EXPORT_SYMBOL(get_dma_ch_dst_addr);

 dma_addr_t get_dma_ch_src_addr(int dma_id,unsigned int chn)
 {
	  struct dma64	*dma64;
	  struct dma64_chan *dma64c;
	  u32 addr_l;
	  u32 addr_h;
	  dma_addr_t src_addr;
 
	  if(unlikely((dma_id >= DMA64_MAX) || (chn >= 32))){
	   printk(KERN_ERR "parameter error\n");
	   return -1;
	  }
 
	  dma64 = dma64_device[dma_id];
	  dma64c = &dma64->chan[chn];
	  //  vdbg_dump_regs(dma64c);
 
	  addr_l = channel_readl(dma64c,SADDR_REG);
	  addr_h = channel_readl(dma64c,WRAPP_REG);
#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
	 addr_h >>= DMA64C_SRC_ADDR_H_OFFSET;
	 addr_h &= 0xF;
	 src_addr = ((dma_addr_t)addr_h << 32) | (dma_addr_t)addr_l;
#else
	 src_addr = (dma_addr_t)addr_l;
#endif
	  return src_addr;
 }
  
 EXPORT_SYMBOL(get_dma_ch_src_addr);

size_t get_dma_ch_interrupt_dst_addr(int dma_id,unsigned int chn)
{
    struct dma64  *dma64;
    struct dma64_chan *dma64c;
    unsigned long     flags;
    unsigned long dma_int_hw_pos;

    if(unlikely((dma_id >= DMA64_MAX) || (chn >= 32))){
        printk(KERN_ERR "parameter error\n");
        return -1;
    }
    
    dma64 = dma64_device[dma_id];
    dma64c = &dma64->chan[chn];
    //printk(KERN_ERR "dma64c->chan_common.dma_int_count %d\n",dma64c->chan_common.dma_int_count);
    spin_lock_irqsave(&dma64c->dma_int_count_lock, flags);
    dma_int_hw_pos = dma64c->chan_common.dma_cycle_buf_len_count;
    spin_unlock_irqrestore(&dma64c->dma_int_count_lock, flags);

    return dma_int_hw_pos;
}
  
EXPORT_SYMBOL(get_dma_ch_interrupt_dst_addr);

 static inline unsigned int dma64_get_xfer_width(dma_addr_t src, dma_addr_t dst,
						 size_t len)
 {
	 unsigned int width;

	 if(!((src | dst  | len) & 7))
	 	 width = 3;
	 else if (!((src | dst  | len) & 3))
		 width = 2;
	 else if (!((src | dst | len) & 1))
		 width = 1;
	 else
		 width = 0;
 
	 return width;
 }
 
static struct dma64_desc *dma64c_first_active(struct dma64_chan *dma64c)
{
	 return list_first_entry(&dma64c->active_list,
				 struct dma64_desc, desc_node);
}
 
/*static struct dma64_desc *dma64c_first_queued(struct dma64_chan *dma64c)
{
	return list_first_entry(&dma64c->queue,
				struct dma64_desc, desc_node);
}*/

/**
 * dma64c_alloc_descriptor - allocate and return an initialized descriptor
 * @chan: the channel to allocate descriptors for
 * @gfp_flags: GFP allocation flags
 *
 * Note: The ack-bit is positioned in the descriptor flag at creation time
 *       to make initial allocation more convenient. This bit will be cleared
 *       and control will be given to client at usage time (during
 *       preparation functions).
 */
static struct dma64_desc *dma64c_alloc_descriptor(struct dma_chan *chan,
					    gfp_t gfp_flags)
{
	struct dma64_desc	*desc = NULL;
	struct dma64	*dma64 = to_dma64(chan->device);
	dma_addr_t phys;

	desc = dma_pool_alloc(dma64->desc_pool, gfp_flags, &phys);
	if (desc) {
		memset(desc, 0, sizeof(struct dma64_desc));
		INIT_LIST_HEAD(&desc->tx_list);
		dma_async_tx_descriptor_init(&desc->txd, chan);
		/* txd.flags will be overwritten in prep functions */
		desc->txd.flags = DMA_CTRL_ACK;
		desc->txd.tx_submit = dma64_tx_submit;
		desc->txd.phys = phys;
	}

	return desc;
}

/**
 * dma64c_desc_get - get an unused descriptor from free_list
 * @dma64c: channel we want a new descriptor for
 */
static struct dma64_desc *dma64c_desc_get(struct dma64_chan *dma64c)
{
	struct dma64_desc *desc, *_desc;
	struct dma64_desc *ret = NULL;
	unsigned long flags;
	unsigned int i = 0;
	LIST_HEAD(tmp_list);

	spin_lock_irqsave(&dma64c->lock, flags);
	list_for_each_entry_safe(desc, _desc, &dma64c->free_list, desc_node) {
		i++;
		if (async_tx_test_ack(&desc->txd)) {
			list_del(&desc->desc_node);
			ret = desc;
			break;
		}
	}
	spin_unlock_irqrestore(&dma64c->lock, flags);

	/* no more descriptor available in initial pool: create one more */
	if (!ret) {
		ret = dma64c_alloc_descriptor(&dma64c->chan_common, GFP_ATOMIC);
		if (ret) {
			spin_lock_irqsave(&dma64c->lock, flags);
			dma64c->descs_allocated++;
			spin_unlock_irqrestore(&dma64c->lock, flags);
		} else {
			dev_err(chan2dev(&dma64c->chan_common),
					"not enough descriptors available\n");
		}
	}

	return ret;
}

/**
 * dma64c_desc_put - move a descriptor, including any children, to the free list
 * @dma64c: channel we work on
 * @desc: descriptor, at the head of a chain, to move to free list
 */
static void dma64c_desc_put(struct dma64_chan *dma64c, struct dma64_desc *desc)
{
	if (desc) {
		unsigned long flags;

		spin_lock_irqsave(&dma64c->lock, flags);
		list_splice_init(&desc->tx_list, &dma64c->free_list);
		list_add(&desc->desc_node, &dma64c->free_list);
		spin_unlock_irqrestore(&dma64c->lock, flags);
	}
}

/**
 * dma64c_desc_chain - build chain adding a descriptor
 * @first: address of first descriptor of the chain
 * @prev: address of previous descriptor of the chain
 * @desc: descriptor to queue
 *
 * Called from prep_* functions
 */
static void dma64c_desc_chain(struct dma64_desc **first, struct dma64_desc **prev,
			   struct dma64_desc *desc)
{
	if (!(*first)) {
		*first = desc;
	} else {
		/* inform the HW lli about chaining */
		(*prev)->lli.llistp_l = DMA64C_LLIST_PTR_L(desc->txd.phys);
		(*prev)->lli.sblks |= DMA64C_LLIST_PTR_H(desc->txd.phys);
		/* insert the link descriptor to the LD ring */
		list_add_tail(&desc->desc_node,
				&(*first)->tx_list);
	}
	*prev = desc;
}

 void dma64_fix_enable(struct dma64_desc *desc,u32 val){
	desc->lli.fragl |= 1<<ADDR_FIX_EN_SHIFT;
	if(val == DST_FIX)
		desc->lli.fragl |= 1<<ADDR_FIX_SEL_SHIFT;
	else if(val == SRC_FIX)
		desc->lli.fragl &= ~(1<<ADDR_FIX_SEL_SHIFT);
}
 void dma64_set_llist_end(struct dma64_desc *desc,u32 val){
	if(val == LLIST_END)
		desc->lli.fragl |= 1<<LLIST_END_SHIFT;
	else
		desc->lli.fragl &= ~(1<<LLIST_END_SHIFT);
}

/**
* dma64c_replenish_dummynode - replenish the DMA dummy node config
* @dma64c: the channel we want to start
* @first: first descriptor in the list we want to begin with
*
* Called with dma64c->lock held and bh disabled
*/
static void dma64c_replenish_dummynode(struct dma64_chan *dma64c, struct dma64_desc *first)
{
	struct dma_slave_config *sconfig = &dma64c->dma_sconfig;
 
	if(sconfig->direction == DMA_SPILT_TO_I2S_AUDIO) {
		channel_writel(dma64c, FRAGL_REG, DMA64C_REQ_MOD(LINKLIST));
	}
}

/**
 * dma64c_dostart - starts the DMA engine for real
 * @dma64c: the channel we want to start
 * @first: first descriptor in the list we want to begin with
 *
 * Called with dma64c->lock held and bh disabled
 */
static void dma64c_dostart(struct dma64_chan *dma64c, struct dma64_desc *first)
{
	/* ASSERT:  channel is idle */
	if (dma64_chan_is_enabled(dma64c)) {
	   dev_err(chan2dev(&dma64c->chan_common),
		   "BUG: Attempted to start non-idle channel\n");
	   dev_err(chan2dev(&dma64c->chan_common),
		   "  channel: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		   channel_readl(dma64c, SADDR_REG),
		   channel_readl(dma64c, DADDR_REG),
		   channel_readl(dma64c, FRAGL_REG),
		   channel_readl(dma64c, BLKL_REG),
		   channel_readl(dma64c, TRSCL_REG),
		   channel_readl(dma64c, TRSFS_REG),
		   channel_readl(dma64c, WRAPP_REG),
		   channel_readl(dma64c, WRAPT_REG),
		   channel_readl(dma64c, LLISTP_REG),
		   channel_readl(dma64c, FRAGS_REG),
		   channel_readl(dma64c, SBLKS_REG),
		   channel_readl(dma64c, DBLKS_REG));

	   /* The tasklet will hopefully advance the queue... */
	   return;
	}

	channel_writel(dma64c, SADDR_REG, 0),
	channel_writel(dma64c, DADDR_REG, 0),
	channel_writel(dma64c, FRAGL_REG, 0),
	channel_writel(dma64c, BLKL_REG,  0),
	channel_writel(dma64c, TRSCL_REG, 0),
	channel_writel(dma64c, TRSFS_REG, 0),
	channel_writel(dma64c, WRAPP_REG, 0),
	channel_writel(dma64c, WRAPT_REG, 0),
	channel_writel(dma64c, LLISTP_REG, DMA64C_LLIST_PTR_L(first->txd.phys));
	channel_writel(dma64c, FRAGS_REG, 0),
	channel_writel(dma64c, SBLKS_REG, DMA64C_LLIST_PTR_H(first->txd.phys));
	channel_writel(dma64c, DBLKS_REG, 0);

	//vdbg_dump_regs(dma64c);

	dma64c_replenish_dummynode(dma64c,first);
  
	channel_writel(dma64c, CFG_REG, DMA64C_LLIST_EN | DMA64C_EN);


}

/*
 * dma64c_get_desc_by_cookie - get the descriptor of a cookie
 * @dma64c: the DMA channel
 * @cookie: the cookie to get the descriptor for
 */
static struct dma64_desc *dma64c_get_desc_by_cookie(struct dma64_chan *dma64c,
						dma_cookie_t cookie)
{
	struct dma64_desc *desc, *_desc;

	list_for_each_entry_safe(desc, _desc, &dma64c->queue, desc_node) {
		if (desc->txd.cookie == cookie)
			return desc;
	}

	list_for_each_entry_safe(desc, _desc, &dma64c->active_list, desc_node) {
		if (desc->txd.cookie == cookie)
			return desc;
	}

	return NULL;
}

/**
 * dma64c_get_bytes_left - get the number of bytes residue for a cookie
 * @chan: DMA channel
 * @cookie: transaction identifier to check status of
 */
static int dma64c_get_bytes_left(struct dma_chan *chan, dma_cookie_t cookie)
{
	struct dma64_chan      *dma64c = to_dma64_chan(chan);
	struct dma64_desc *desc_first = dma64c_first_active(dma64c);
	struct dma64_desc *desc;
	int ret;

	/*
	 * If the cookie doesn't match to the currently running transfer then
	 * we can return the total length of the associated DMA transfer,
	 * because it is still queued.
	 */
	desc = dma64c_get_desc_by_cookie(dma64c, cookie);
	if (desc == NULL)
		return -EINVAL;
	else if (desc != desc_first)
		return desc->total_len;

	/*
	 * 1.cookie matches to the currently running transfer 
	 * 2.hardware not support to calc the running transfer data left
	 * 3.so mask as DMA64_TRSC_BTSIZE_MAX 
	 */
	ret = DMA64_TRSC_BTSIZE_MAX;

	return ret;

}

/**
 * dma64c_chain_complete - finish work for one transaction chain
 * @atchan: channel we work on
 * @desc: descriptor at the head of the chain we want do complete
 *
 * Called with atchan->lock held and bh disabled */
static void
dma64c_chain_complete(struct dma64_chan *dma64c, struct dma64_desc *desc)
{
	struct dma_async_tx_descriptor	*txd = &desc->txd;

	/* mark the descriptor as complete for non cyclic cases only */
	if (!dma64_chan_is_cyclic(dma64c))
		dma_cookie_complete(txd);

	/* move children to free_list */
	list_splice_init(&desc->tx_list, &dma64c->free_list);
	/* move myself to free_list */
	list_move(&desc->desc_node, &dma64c->free_list);

	dma_descriptor_unmap(txd);
	/* for cyclic transfers,
	 * no need to replay callback function while stopping */
	if (!dma64_chan_is_cyclic(dma64c)) {
		/*
		 * The API requires that no submissions are done from a
		 * callback, so we don't need to drop the lock here
		 */
		dmaengine_desc_get_callback_invoke(txd, NULL);
	}

	dma_run_dependencies(txd);
}

static void dma64c_dostart_first_queued(struct dma64_chan *dma64c)
{
	struct dma64_desc *desc=NULL;

	if(list_empty(&dma64c->queue))
		return;

	list_move(dma64c->queue.next, &dma64c->active_list);
	desc = dma64c_first_active(dma64c);
	if(desc){
		dma64c_dostart(dma64c,desc);
	}
}

/**
 * dma64c_complete_all - finish work for all transactions
 * @dma64c: channel to complete transactions for
 *
 * Eventually submit queued descriptors if any
 *
 * Assume channel is idle while calling this function
 * Called with dma64c->lock held and bh disabled
 */
static void dma64c_complete_all(struct dma64_chan *dma64c)
{
	struct dma64_desc *desc, *_desc;
	LIST_HEAD(list);

	/*
	 * Submit queued descriptors ASAP, i.e. before we go through
	 * the completed ones.
	 */
	/* empty active_list now it is completed */
	list_splice_init(&dma64c->active_list, &list);
	/*move first queue to active list,then do start*/
	dma64c_dostart_first_queued(dma64c);

	list_for_each_entry_safe(desc, _desc, &list, desc_node)
		dma64c_chain_complete(dma64c, desc);
}


/**
 * dma64_advance_work - at the end of a transaction, move forward
 * @dma64c: channel where the transaction ended
 *
 * Called with dma64c->lock held and bh disabled
 */
static void dma64_advance_work(struct dma64_chan *dma64c)
{
	if (dma64_chan_is_enabled(dma64c))
		return;

	if (dma64_chan_is_cyclic(dma64c)){
		dma64c_dostart(dma64c,dma64c->cyclic_desc);
		return;
	}

	if (list_empty(&dma64c->active_list))
		dma64c_dostart_first_queued(dma64c);
}

/**
 * dma64c_handle_error - handle errors reported by DMA controller
 * @atchan: channel where error occurs
 *
 * Called with dma64c->lock held and bh disabled
 */
static void dma64c_handle_error(struct dma64_chan *dma64c)
{
	struct dma64_desc *bad_desc;
	struct dma64_desc *child;

	/*
	 * The descriptor currently at the head of the active list is
	 * broked. Since we don't have any way to report errors, we'll
	 * just have to scream loudly and try to carry on.
	 */
	bad_desc = dma64c_first_active(dma64c);
	list_del_init(&bad_desc->desc_node);

	/* As we are stopped, take advantage to push queued descriptors
	 * in active_list */
	list_splice_init(&dma64c->queue, dma64c->active_list.prev);

	/* Try to restart the controller */
	if (!list_empty(&dma64c->active_list))
		dma64c_dostart(dma64c, dma64c_first_active(dma64c));

	/*
	 * KERN_CRITICAL may seem harsh, but since this only happens
	 * when someone submits a bad physical address in a
	 * descriptor, we should consider ourselves lucky that the
	 * controller flagged an error instead of scribbling over
	 * random memory locations.
	 */
	dev_crit(chan2dev(&dma64c->chan_common),
			"Bad descriptor submitted for DMA!\n");
	dev_crit(chan2dev(&dma64c->chan_common),
			"  cookie: %d\n", bad_desc->txd.cookie);
	dma64_dump_lli(dma64c, &bad_desc->lli);
	list_for_each_entry(child, &bad_desc->tx_list, desc_node)
		dma64_dump_lli(dma64c, &child->lli);

	/* Pretend the descriptor completed successfully */
	dma64c_chain_complete(dma64c, bad_desc);
}

/**
 * dma64c_handle_cyclic - at the end of a period, run callback function
 * @atchan: channel used for cyclic operations
 *
 * Called with atchan->lock held and bh disabled
 */
static void dma64c_handle_cyclic(struct dma64_chan *dma64c)
{
	struct dma64_desc			*desc = dma64c->cyclic_desc;
	struct dma_async_tx_descriptor	*txd = &desc->txd;

	dmaengine_desc_get_callback_invoke(txd, NULL);
}

#if 0
/**
 * dma64c_fill_cid - Fill hardware cid register
 */
static void dma64c_fill_cid(struct dma_chan *chan)
{
	 struct dma64_chan	*dma64c = to_dma64_chan(chan);
	 struct dma64		*dma64 = to_dma64(chan->device);

	 dma64_cid_writel(dma64,cid_regs(dma64c->dma_sconfig.slave_id),
	  dma64c->chan_common.chan_id + 1);
}
#endif

/*--  IRQ & Tasklet  ---------------------------------------------------*/

static void dma64c_tasklet(unsigned long data)
{
	struct dma64_chan *dma64c = (struct dma64_chan *)data;
	unsigned long flags;

	spin_lock_irqsave(&dma64c->lock, flags);
	if (test_and_clear_bit(DMA64C_IS_ERROR, &dma64c->status))
		dma64c_handle_error(dma64c);
	else if (dma64_chan_is_cyclic(dma64c))
		dma64c_handle_cyclic(dma64c);
	else
		dma64c_complete_all(dma64c);

	spin_unlock_irqrestore(&dma64c->lock, flags);
}

static irqreturn_t dma64_interrupt(int irq, void *dev_id)
{
	struct dma64		*dma64_common = (struct dma64 *)dev_id;
	struct dma64_chan	*dma64c;
	u32 channel;
	u32			mask, raw;
	int			ret = IRQ_NONE;

	do {
		mask = dma64_readl(dma64_common, MASK_REG);

		if (!mask)
			break;

		channel = fls(mask);
		channel--;

		dma64c = &dma64_common->chan[channel];

		if( (dma64c->chan_common.dma_cycle_buf_len > 0) && (dma64c->chan_common.dma_cycle_period_len > 0) ){
			dma64c->chan_common.dma_cycle_buf_len_count+=dma64c->chan_common.dma_cycle_period_len;
			if(dma64c->chan_common.dma_cycle_buf_len_count > (dma64c->chan_common.dma_cycle_buf_len) ) {
			    dma64c->chan_common.dma_cycle_buf_len_count = 0;
			    dma64c->chan_common.dma_cycle_buf_len_count+=dma64c->chan_common.dma_cycle_period_len;
			}
		}
        
		raw = channel_readl(dma64c,INT_REG);

		/*try to clear the interrupts*/
		if(raw & CHN_LINKLIST_INT_MASK){
			raw |= CHN_LINKLIST_INT_CLR;
		}else if(raw & CHN_TRSC_INT_MASK){
			raw |= CHN_TRSC_INT_CLR;
		}else if(raw & CHN_BLK_INT_MASK){
			raw |= CHN_BLK_INT_CLR;
		}else if(raw & CHN_FRAGMENT_INT_CLR){
			raw |= CHN_FRAGMENT_INT_CLR;
		}else{
			raw |= CHN_CFG_ERR_INT_CLR;
			/* Give information to tasklet */
			set_bit(DMA64C_IS_ERROR, &dma64c->status);
		}
		channel_writel(dma64c,INT_REG,raw);

		if(channel >= 24)  //chan 24 -31 high proirity
			tasklet_hi_schedule(&dma64c->tasklet);
		else
			tasklet_schedule(&dma64c->tasklet);
		ret = IRQ_HANDLED;
	} while (mask);


	return ret;

}

/*--  DMA Engine API  --------------------------------------------------*/

/**
 * dma64_tx_submit - add the prepared descriptor(s) to the queue
 * @desc: descriptor at the head of the transaction chain
 *
 * Queue chain if DMA engine is working already
 *
 * Cookie increment and adding to active_list or queue must be atomic
 */
static dma_cookie_t dma64_tx_submit(struct dma_async_tx_descriptor *tx)
{
	struct dma64_desc		*desc = txd_to_dma64_desc(tx);
	struct dma64_chan	*dma64c = to_dma64_chan(tx->chan);
	dma_cookie_t		cookie;
	unsigned long		flags;

	spin_lock_irqsave(&dma64c->lock, flags);
	cookie = dma_cookie_assign(tx);

	/*
	 * REVISIT: We should attempt to chain as many descriptors as
	 * possible, perhaps even appending to those already submitted
	 * for DMA. But this is hard to do in a race-free manner.
	 */

	list_add_tail(&desc->desc_node, &dma64c->queue);
	spin_unlock_irqrestore(&dma64c->lock, flags);

	return cookie;
}

static struct dma_async_tx_descriptor *
dma64_prep_dma_memcpy(struct dma_chan *chan, dma_addr_t dest, dma_addr_t src,
		size_t len, unsigned long flags)
{
	struct dma64_chan		*dma64c = to_dma64_chan(chan);
	struct dma64_desc		*desc = NULL;
	unsigned int		src_width;
	unsigned int		dst_width;
	u32 		ctrl;

	if (unlikely(len == 0))
		return NULL;

	ctrl = DMA64C_SWP_MODE(ABCD2ABCD) |
		DMA64C_REQ_MOD(LINKLIST) |
		DMA64C_ADDR_WRAP(DISABLE)|
		DMA64C_ADDR_FIX(DISABLE);

	if (unlikely(len > DMA64_TRSC_BTSIZE_MAX)){
		len = DMA64_TRSC_BTSIZE_MAX;
		dev_err(chan2dev(&dma64c->chan_common),
			"trsction len exceed 256M, skip!\n");
	}

	/* take care for the alignment */
	src_width = dst_width = dma64_get_xfer_width(src, dest, len);

	ctrl |= DMA64C_SRC_WIDTH(src_width) | DMA64C_DST_WIDTH(dst_width);

	/* create the transfer */
	desc = dma64c_desc_get(dma64c);
	if (!desc)
		goto err_desc_get;

	/* Prepare DMA descriptor */
	desc->lli.saddr_l = (unsigned int)DMA64C_SRC_ADDR_L(src);
	desc->lli.daddr_l = (unsigned int)DMA64C_DST_ADDR_L(dest);

	desc->lli.wrapp = (unsigned int)DMA64C_SRC_ADDR_H(src);
	desc->lli.wrapt = (unsigned int)DMA64C_DST_ADDR_H(dest);

	ctrl |= DMA64C_FRAG_LEN(1 << src_width);
	desc->lli.fragl = ctrl;

	desc->lli.blkl = DMA64C_BLK_LEN(1 << src_width);
	desc->lli.trscl = DMA64C_TRSC_LEN(len);

	desc->lli.trsfs = DMA64C_DST_TRSF_STEP(1 << src_width) |
		DMA64C_SRC_TRSF_STEP(1 << src_width);

	desc->lli.frags = DMA64C_DST_FRAG_STEP(0) |
		DMA64C_SRC_FRAG_STEP(0);
	desc->lli.frags = DMA64C_SRC_FRAG_STEP(0);
	desc->lli.dblks = DMA64C_DST_BLK_STEP(0);

	desc->txd.cookie = 0;
	desc->len = len;
	desc->total_len = len;
	desc->txd.flags = flags;

	/* set end-of-link to the last link descriptor of list*/
	set_desc_eol(desc);

	if(flags & DMA_PREP_INTERRUPT)
		channel_writel(dma64c,INT_REG,
			channel_readl(dma64c,INT_REG)|DMA64C_LLIST_INT_EN);

	channel_writel(dma64c, REQ_REG, DMA64C_REQ);

	return &desc->txd;

err_desc_get:
	dma64c_desc_put(dma64c, desc);
	return NULL;

}

 void dma64c_slave_sg_fill_cid(struct dma_chan *chan)
{
	struct dma64_chan	*dma64c = to_dma64_chan(chan);
	struct dma64		*dma64 = to_dma64(chan->device);
	dma64_cid_writel(dma64,cid_regs(dma64c->dma_sconfig.slave_id),
		dma64c->chan_common.chan_id + 1);
}

#if 0
/**
 * dma64c_check_expand_values
 * Check for same expand parameters that dmaengine not support
 */
static int
dma64c_check_expand_param(struct dma64_desc* desc, unsigned long flags)
{
	 unsigned long  mode;
	 mode = (flags >> 16) & 0xffff;
	 
	 switch(mode){
	   case ABCD2ABCD:
        desc->lli.fragl |= DMA64C_SWP_MODE(ABCD2ABCD);
        break;
	   case ABCD2DCBA:
        desc->lli.fragl |= DMA64C_SWP_MODE(ABCD2DCBA);
        break;
	   case ABCD2BADC:
        desc->lli.fragl |= DMA64C_SWP_MODE(ABCD2BADC);
        break;
	   case ABCD2DCAB:
        desc->lli.fragl |= DMA64C_SWP_MODE(ABCD2DCAB);
        break;
	   default:
        break;  //default  ABCD2ABCD
	 }
  
	 return 0;

}

/**
 * dma64c_set_switch_mode
 * set slave swt mode
 */
static int
dma64c_set_switch_mode(unsigned int *ctrl,struct dma_slave_config *sconfig)
{
	 unsigned int slave_id=0;

	 slave_id = sconfig->slave_id;
	 
	 switch(slave_id){
	   case MC_HASH:
        *ctrl |= DMA64C_SWP_MODE(ABCD2DCBA);
        break;
	   default:
        break;  //default  ABCD2ABCD
	 }
  
	 return 0;
}

struct dma_async_tx_descriptor *
dma64_prep_slave_sg(struct dma_chan *chan,struct scatterlist *sgl,
    unsigned int sg_len, enum dma_transfer_direction direction,
    unsigned long flags,void *context){
	 struct dma64_chan *dma64c = to_dma64_chan(chan);
	 struct dma_slave_config *sconfig = &dma64c->dma_sconfig;
	 struct dma64_desc   *first = NULL;
	 struct dma64_desc   *prev = NULL;
	 struct scatterlist  *sg;
	 u32     reg_width;
	 u32     i;
	 u32     ctrl;
	 size_t      total_len = 0;

	 if(unlikely(!is_slave_direction(direction) || !sg_len))
      goto err_out;

	 if (sconfig->direction == DMA_MEM_TO_DEV)
      reg_width = convert_buswidth(sconfig->dst_addr_width);
	 else
      reg_width = convert_buswidth(sconfig->src_addr_width);

    /* prepare common FRAG CRTL value */
	 ctrl = DMA64C_SWP_MODE(ABCD2ABCD) |
        DMA64C_LLIST_MOD(FRAGMENT) |
        DMA64C_ADDR_WRAP(DISABLE) |
        DMA64C_SRC_WIDTH(reg_width) |
        DMA64C_DST_WIDTH(reg_width) |
        DMA64C_LLIST_MOD(LLIST_NOT_END);

	 dma64c_set_switch_mode(&ctrl,sconfig);

	 prev = first = NULL;
	 switch(direction){
      case DMA_MEM_TO_DEV:
        for_each_sg(sgl,sg,sg_len,i){
          struct dma64_desc *desc;
          size_t      len;
          dma_addr_t    mem;
          mem = sg_dma_address(sg);
          len = sg_dma_len(sg);
          if(!len)
            goto err_len_set;
          desc =dma64c_desc_get(dma64c);
          if(!desc)
            goto err_desc_get;
          
          desc->lli.saddr_l = (unsigned int)DMA64C_SRC_ADDR_L(mem);
          desc->lli.wrapp = (unsigned int)DMA64C_SRC_ADDR_H(mem);
          desc->lli.daddr_l = (unsigned int)DMA64C_DST_ADDR_L(sconfig->dst_addr);
          desc->lli.wrapt = (unsigned int)DMA64C_DST_ADDR_H(sconfig->dst_addr);
          
          ctrl |= DMA64C_ADDR_FIX(ENABLE);
          ctrl |= DMA64C_ADDR_FIX_SEL(DST);
          ctrl |= DMA64C_FRAG_LEN(sconfig->dst_maxburst << reg_width);
          desc->lli.fragl = ctrl;
          desc->lli.blkl = DMA64C_BLK_LEN(sconfig->dst_maxburst << reg_width);
          desc->lli.trscl = DMA64C_TRSC_LEN(len);
          desc->lli.trsfs = DMA64C_DST_TRSF_STEP(1 << reg_width) | DMA64C_SRC_TRSF_STEP(1 << reg_width);
          desc->lli.frags = DMA64C_DST_FRAG_STEP(0) | DMA64C_SRC_FRAG_STEP(0);
          desc->lli.sblks = DMA64C_SRC_BLK_STEP(0);
          desc->lli.dblks = DMA64C_DST_BLK_STEP(0);

          dma64c_check_expand_param(desc,flags);

          desc->txd.cookie = 0;
          desc->len = len;

          dma64c_desc_chain(&first, &prev, desc);
          
          total_len += len;
        }
        break;
      case DMA_DEV_TO_MEM:
        for_each_sg(sgl,sg,sg_len,i){
          struct dma64_desc *desc;
          size_t      len;
          dma_addr_t    mem;
          mem = sg_dma_address(sg);
          len = sg_dma_len(sg);
          if(!len)
            goto err_len_set;
          desc =dma64c_desc_get(dma64c);
          if(!desc)
            goto err_desc_get;
          desc->lli.saddr_l = (unsigned int)DMA64C_SRC_ADDR_L(sconfig->src_addr);
          desc->lli.wrapp = (unsigned int)DMA64C_SRC_ADDR_H(sconfig->src_addr);
          desc->lli.daddr_l = (unsigned int)DMA64C_DST_ADDR_L(mem);
          desc->lli.wrapt = (unsigned int)DMA64C_DST_ADDR_H(mem);
          
          ctrl |= DMA64C_ADDR_FIX(ENABLE);
          ctrl |= DMA64C_ADDR_FIX_SEL(SRC);
          ctrl |= DMA64C_FRAG_LEN(sconfig->src_maxburst << reg_width);
          desc->lli.fragl = ctrl;
          desc->lli.blkl = DMA64C_BLK_LEN(sconfig->src_maxburst << reg_width);
          desc->lli.trscl = DMA64C_TRSC_LEN(len);
          desc->lli.trsfs = DMA64C_DST_TRSF_STEP(1 << reg_width) | DMA64C_SRC_TRSF_STEP(1 << reg_width);
          desc->lli.frags = DMA64C_DST_FRAG_STEP(0) | DMA64C_SRC_FRAG_STEP(0);
          desc->lli.sblks = DMA64C_SRC_FRAG_STEP(0);
          desc->lli.dblks = DMA64C_DST_BLK_STEP(0);

          dma64c_check_expand_param(desc,flags);

          desc->txd.cookie = 0;
          desc->len = len;

          dma64c_desc_chain(&first, &prev, desc);

          total_len += len;
        }
        break;
      default:
        goto err_out;
    }
    /* lets make a non-cyclic list */
    prev->lli.fragl |= DMA64C_LLIST_MOD(LLIST_END);
    /* First descriptor of the chain embedds additional information */
    first->txd.cookie = -EBUSY;
    first->total_len = total_len;

    if(flags & DMA_PREP_INTERRUPT)
      channel_writel(dma64c,INT_REG,
        channel_readl(dma64c,INT_REG)|DMA64C_TRSC_INT_EN );

    dma64c_fill_cid(chan);

    dma64c->cyclic_desc = first;

    return &first->txd;

err_desc_get:
	 dev_err(chan2dev(chan),"not enough descriptors available,Direction %d\n",direction);
	 dma64c_desc_put(dma64c,first);
err_len_set:
	 dev_err(chan2dev(chan),"err trans len set,trans len = 0\n");
err_out:
	 return NULL;

}
#endif
#if 1
struct dma_async_tx_descriptor *
dma64_prep_slave_sg(struct dma_chan *chan,struct scatterlist *sgl,
		unsigned int sg_len, enum dma_transfer_direction direction,
		unsigned long flags,void *context){
	struct dma64_chan	*mcc = to_dma64_chan(chan);
	struct dma_slave_config *sconfig = &mcc->dma_sconfig;
	struct dma64_desc		*prev;
	struct dma64_desc		*first;
	struct scatterlist	*sg;
	dma_addr_t		reg;
	u32 		reg_width;
	u32 		mem_width;
	u32 		i;
	size_t			total_len = 0;
	if(unlikely(!is_slave_direction(direction) || !sg_len))
		return NULL;
	//printk("sg_len is 0x%08x \r\n",sg_len);
	prev = first = NULL;
	switch(direction){
		case DMA_MEM_TO_DEV:
			reg_width = sconfig->dst_addr_width;
			mem_width = sconfig->src_addr_width;
			reg = sconfig->dst_addr;
			for_each_sg(sgl,sg,sg_len,i){
				struct dma64_desc	*desc;
				unsigned long long		len,mem;
				//printk("mem to dev \r\n");
				mem = sg_dma_address(sg);
				len = sg_dma_len(sg);
				if(!len)
					goto err_len_set;
				desc =dma64c_desc_get(mcc);
				if(!desc)
					goto err_desc_get;
				desc->lli.saddr_l = (unsigned int)DMA64C_SRC_ADDR_L(mem);
				//desc->lli.wrapp = (unsigned int)DMA64C_SRC_ADDR_H(mem);
				desc->lli.daddr_l = (unsigned int)DMA64C_DST_ADDR_L(reg);
				//desc->lli.wrapt = (unsigned int)DMA64C_DST_ADDR_H(reg);
				desc->lli.trscl &= ~TRSC_LEN_MASK;
				desc->lli.trscl |=len;
				if(flags & DMA_PREP_INTERRUPT){
					desc->lli.trscl &= ~CHN_TRSC_LEN_LLIST_NODE_INT_EN;
					channel_writel(mcc,INT_REG,
								channel_readl(mcc,INT_REG)|DMA64C_LLIST_INT_EN);
				}
				desc->lli.blkl &= ~BLK_LEN_MASK;
				desc->lli.blkl |= DMA64C_FRAG_LEN(sconfig->dst_maxburst);
				desc->lli.fragl &= ~FRAG_LEN_MASK;
				desc->lli.fragl |= DMA64C_FRAG_LEN(sconfig->dst_maxburst);
				desc->lli.fragl |= DMA64C_SWP_MODE(ABCD2DCBA);
				desc->lli.fragl &= ~(REQ_MODE_MASK<<REQ_MODE_SHIFT);
				desc->lli.fragl |= DMA64C_REQ_MOD(FRAGMENT);
				desc->lli.fragl &=~(SRC_DST_AHB_SIZE_MASK<<DEST_AHB_SIZE_SHIFT);
				desc->lli.fragl |= (((mem_width/2) << SRC_AHB_SIZE_SHIFT) | ((reg_width/2) << DEST_AHB_SIZE_SHIFT));
				dma64_fix_enable(desc,DST_FIX);
				desc->lli.fragl &= ~(1<<ADDR_WRAP_EN_SHIFT);
				desc->lli.trsfs &= ~TRSF_STEP_MASK;
				desc->lli.trsfs |= (DMA64C_DST_TRSF_STEP(reg_width) | DMA64C_SRC_TRSF_STEP(mem_width));
				desc->lli.frags = 0;
				desc->lli.sblks = 0;
				desc->lli.dblks = 0;
				if(i < sg_len-1){
					desc->lli.fragl &= ~(1<<LLIST_END_SHIFT);
				}

				total_len += len;
				dma64c_desc_chain(&first, &prev, desc);
			}
			break;
		case DMA_DEV_TO_MEM:
			reg_width = sconfig->src_addr_width;
			mem_width = sconfig->dst_addr_width;
			reg = sconfig->src_addr;
			for_each_sg(sgl,sg,sg_len,i){
				//printk("dev to mem \r\n");
				struct dma64_desc	*desc;
				unsigned long long			len,mem;
				mem = sg_dma_address(sg);
				len = sg_dma_len(sg);
				if(!len)
					goto err_len_set;
				desc =dma64c_desc_get(mcc);
				if(!desc)
					goto err_desc_get;
				desc->lli.saddr_l = (unsigned int)DMA64C_SRC_ADDR_L(reg);
				//desc->lli.wrapp = (unsigned int)DMA64C_SRC_ADDR_H(mem);
				desc->lli.daddr_l = (unsigned int)DMA64C_DST_ADDR_L(mem);
				//desc->lli.wrapt = (unsigned int)DMA64C_DST_ADDR_H(reg);
				desc->lli.trscl &= ~TRSC_LEN_MASK;
				desc->lli.trscl |=len;
				if(flags & DMA_PREP_INTERRUPT){
					desc->lli.trscl &= ~CHN_TRSC_LEN_LLIST_NODE_INT_EN;
					channel_writel(mcc,INT_REG,
								channel_readl(mcc,INT_REG)|DMA64C_LLIST_INT_EN);
				}
				desc->lli.blkl &= ~BLK_LEN_MASK;
				desc->lli.blkl |= DMA64C_FRAG_LEN(sconfig->dst_maxburst);
				desc->lli.fragl &= ~FRAG_LEN_MASK;
				desc->lli.fragl |= DMA64C_FRAG_LEN(sconfig->dst_maxburst);
				desc->lli.fragl |= DMA64C_SWP_MODE(ABCD2DCBA);
				desc->lli.fragl &= ~(REQ_MODE_MASK<<REQ_MODE_SHIFT);
				desc->lli.fragl |= DMA64C_REQ_MOD(FRAGMENT);
				desc->lli.fragl &=~(SRC_DST_AHB_SIZE_MASK<<DEST_AHB_SIZE_SHIFT);
				desc->lli.fragl |= (((mem_width/2) << DEST_AHB_SIZE_SHIFT) | ((reg_width/2) <<  SRC_AHB_SIZE_SHIFT));
				dma64_fix_enable(desc,SRC_FIX);
				desc->lli.fragl &= ~(1<<ADDR_WRAP_EN_SHIFT);
				desc->lli.trsfs &= ~TRSF_STEP_MASK;
				desc->lli.trsfs |= (DMA64C_DST_TRSF_STEP(mem_width) | DMA64C_SRC_TRSF_STEP(reg_width));
				desc->lli.frags = 0;
				desc->lli.sblks = 0;
				desc->lli.dblks = 0;
				if(i < sg_len-1){
					desc->lli.fragl &= ~(1<<LLIST_END_SHIFT);
				}

				total_len += len;
				dma64c_desc_chain(&first, &prev, desc);
			}
			break;
		default:
			return NULL;
	}
	dma64_set_llist_end(prev,LLIST_END);
	dma64c_slave_sg_fill_cid(chan);
	first->txd.cookie = -EBUSY;
	first->total_len = total_len;
	first->txd.flags = flags;
	mcc->cyclic_desc = first;
	return &first->txd;
err_desc_get:
	dev_err(chan2dev(chan),"not enough descriptors available,Direction %d\n",direction);
	dma64c_desc_put(mcc,first);
	return NULL;
err_len_set:
	dev_err(chan2dev(chan),"err trans len set,trans len = 0\n");
	return NULL;
}
#endif

/**
 * dma64_prep_dma_sg - prepare memory to memory scather-gather operation
 * @chan: the channel to prepare operation on
 * @dst_sg: destination scatterlist
 * @dst_nents: number of destination scatterlist entries
 * @src_sg: source scatterlist
 * @src_nents: number of source scatterlist entries
 * @flags: tx descriptor status flags
 */
static struct dma_async_tx_descriptor *
dma64_prep_dma_sg(struct dma_chan *chan,
		struct scatterlist *dst_sg, unsigned int dst_nents,
		struct scatterlist *src_sg, unsigned int src_nents,
		unsigned long flags)
{
	struct dma64_chan		*dma64c = to_dma64_chan(chan);
	struct dma64_desc		*first = NULL;
	struct dma64_desc		*prev = NULL;
	struct dma64_desc		*desc = NULL;
	unsigned int		src_width;
	unsigned int		dst_width;
	u32			ctrl;
	size_t			dst_len = 0, src_len = 0;
	dma_addr_t		dst = 0, src = 0;
	size_t			len = 0, total_len = 0;

	if (unlikely(dst_nents == 0 || src_nents == 0))
		return NULL;

	if (unlikely(dst_sg == NULL || src_sg == NULL))
		return NULL;

	ctrl = DMA64C_SWP_MODE(ABCD2ABCD) |
		DMA64C_REQ_MOD(LINKLIST) |
		DMA64C_ADDR_WRAP(DISABLE)|
		DMA64C_ADDR_FIX(DISABLE) |
		DMA64C_LLIST_MOD(LLIST_NOT_END);
	/* Run until we are out of scatterlist entries */
	while (true) {

		/* prepare the next transfer */
		if (dst_len == 0) {

			/* no more destination scatterlist entries */
			if (!dst_sg || !dst_nents)
				break;

			dst = sg_dma_address(dst_sg);
			dst_len = sg_dma_len(dst_sg);

			dst_sg = sg_next(dst_sg);
			dst_nents--;
		}

		if (src_len == 0) {

			/* no more source scatterlist entries */
			if (!src_sg || !src_nents)
				break;

			src = sg_dma_address(src_sg);
			src_len = sg_dma_len(src_sg);

			src_sg = sg_next(src_sg);
			src_nents--;
		}

		len = min_t(size_t, src_len, dst_len);
		if (len == 0)
			continue;

		/*
		 * The number of transfers to set up refer to the source width
		 * that depends on the alignment.
		 */
		if (len > DMA64_TRSC_BTSIZE_MAX) {
			len = DMA64C_TRSC_LEN(DMA64_TRSC_BTSIZE_MAX);
			dev_info(chan2dev(chan),"warning...dma64 exceed max transaction length!\n");
		}

		/* take care for the alignment */
		src_width = dst_width = dma64_get_xfer_width(src, dst, len);

		ctrl |= DMA64C_SRC_WIDTH(src_width) | DMA64C_DST_WIDTH(dst_width);		

		/* create the transfer */
		desc = dma64c_desc_get(dma64c);
		if (!desc)
			goto err_desc_get;

		/* Prepare DMA descriptor */
		desc->lli.saddr_l = (unsigned int)DMA64C_SRC_ADDR_L(src);
		desc->lli.daddr_l = (unsigned int)DMA64C_DST_ADDR_L(dst);

		desc->lli.wrapp = (unsigned int)DMA64C_SRC_ADDR_H(src);
		desc->lli.wrapt = (unsigned int)DMA64C_DST_ADDR_H(dst);

		//ctrl |= DMA64C_FRAG_LEN(1 << src_width);
		ctrl |= DMA64C_FRAG_LEN(len & DMA_FRAG_MAXLEN_MSK);
		desc->lli.fragl = ctrl;

		//desc->lli.blkl = DMA64C_BLK_LEN(1 << src_width);
		desc->lli.blkl = DMA64C_BLK_LEN(len & DMA_BLK_MAXLEN_MSK);
		desc->lli.trscl = DMA64C_TRSC_LEN(len);

		desc->lli.trsfs = DMA64C_DST_TRSF_STEP(1 << src_width) |
			DMA64C_SRC_TRSF_STEP(1 << src_width);

		desc->lli.frags = DMA64C_DST_FRAG_STEP(0) | 
			DMA64C_SRC_FRAG_STEP(0);
		desc->lli.frags = DMA64C_SRC_FRAG_STEP(0);
		desc->lli.dblks = DMA64C_DST_BLK_STEP(0);

		desc->txd.cookie = 0;
		desc->len = len;

		dma64c_desc_chain(&first, &prev, desc);

		/* update the lengths and addresses for the next loop cycle */
		dst_len -= len;
		src_len -= len;
		dst += len;
		src += len;

		total_len += len;
	}
	/* First descriptor of the chain embedds additional information */
	first->txd.cookie = -EBUSY;
	first->total_len = total_len;

	/* set end-of-link to the last link descriptor of list*/
	set_desc_eol(desc);
	
	if(flags & DMA_PREP_INTERRUPT)
		channel_writel(dma64c,INT_REG,
			channel_readl(dma64c,INT_REG)|DMA64C_LLIST_INT_EN);

	channel_writel(dma64c, REQ_REG, DMA64C_REQ);		
	
	first->txd.flags = flags; /* client is in control of this ack */
	
	return &first->txd;
	
err_desc_get:
	dma64c_desc_put(dma64c, first);
	return NULL;

}

/**
 * dma64c_cyclic_check_values
 * Check for too big/unaligned periods and unaligned DMA buffer
 */
static int
dma64c_cyclic_check_values(unsigned int reg_width, dma_addr_t buf_addr,
		size_t period_len)
{
	if (period_len > DMA64_TRSC_BTSIZE_MAX)
		goto err_out;
	if (unlikely(period_len & ((1 << reg_width) - 1)))
		goto err_out;
	if (unlikely(buf_addr & ((1 << reg_width) - 1)))
		goto err_out;

	return 0;

err_out:
	return -EINVAL;
}

/**
 * dma64_cyclic_fill_desc - Fill one period descriptor
 */
static int
dma64c_cyclic_fill_desc(struct dma_chan *chan, struct dma64_desc *desc,
		unsigned int period_index, dma_addr_t buf_addr,
		unsigned int reg_width, size_t period_len,
		enum dma_transfer_direction direction)
{
	struct dma64_chan	*dma64c = to_dma64_chan(chan);
	struct dma_slave_config *sconfig = &dma64c->dma_sconfig;
	dma_addr_t addr = buf_addr + (period_len * period_index);
	u32 		ctrl;
	u32     ctrl_trsfs;
	u32     ctrl_dblks;

	/* prepare common FRAG CRTL value */
	ctrl = DMA64C_SWP_MODE(ABCD2ABCD) |
			DMA64C_LLIST_MOD(FRAGMENT) |
			DMA64C_ADDR_WRAP(DISABLE) |
			DMA64C_SRC_WIDTH(reg_width) |
			DMA64C_DST_WIDTH(reg_width) |
			DMA64C_LLIST_MOD(LLIST_NOT_END);

	switch (direction) {
	case DMA_MEM_TO_DEV:
		desc->lli.saddr_l = (unsigned int)DMA64C_SRC_ADDR_L(addr);
		desc->lli.wrapp = (unsigned int)DMA64C_SRC_ADDR_H(addr);
		desc->lli.daddr_l = (unsigned int)DMA64C_DST_ADDR_L(sconfig->dst_addr);
		desc->lli.wrapt = (unsigned int)DMA64C_DST_ADDR_H(sconfig->dst_addr);

		ctrl |= DMA64C_ADDR_FIX(ENABLE);
		ctrl |= DMA64C_ADDR_FIX_SEL(DST);
		ctrl |= DMA64C_FRAG_LEN(sconfig->dst_maxburst << reg_width);
		desc->lli.fragl = ctrl;
		desc->lli.blkl = DMA64C_BLK_LEN(sconfig->dst_maxburst << reg_width);
		desc->lli.trscl = DMA64C_TRSC_LEN(period_len);
		desc->lli.trsfs = DMA64C_DST_TRSF_STEP(1 << reg_width) | DMA64C_SRC_TRSF_STEP(1 << reg_width);
		desc->lli.frags = DMA64C_DST_FRAG_STEP(0) | DMA64C_SRC_FRAG_STEP(0);
		desc->lli.frags = DMA64C_SRC_FRAG_STEP(0);
		desc->lli.dblks = DMA64C_DST_BLK_STEP(0);


		desc->len = period_len;
		break;

	case DMA_DEV_TO_MEM:
		desc->lli.saddr_l = (unsigned int)DMA64C_SRC_ADDR_L(sconfig->src_addr);
		desc->lli.wrapp = (unsigned int)DMA64C_SRC_ADDR_H(sconfig->src_addr);
		desc->lli.daddr_l = (unsigned int)DMA64C_DST_ADDR_L(addr);
		desc->lli.wrapt = (unsigned int)DMA64C_DST_ADDR_H(addr);

		ctrl |= DMA64C_ADDR_FIX(ENABLE);
		ctrl |= DMA64C_ADDR_FIX_SEL(SRC);
		ctrl |= DMA64C_FRAG_LEN(sconfig->src_maxburst << reg_width);
		desc->lli.fragl = ctrl;
		desc->lli.blkl = DMA64C_BLK_LEN(sconfig->src_maxburst << reg_width);
		desc->lli.trscl = DMA64C_TRSC_LEN(period_len);
		desc->lli.trsfs = DMA64C_DST_TRSF_STEP(1 << reg_width) | DMA64C_SRC_TRSF_STEP(1 << reg_width);
		desc->lli.frags = DMA64C_DST_FRAG_STEP(0) | DMA64C_SRC_FRAG_STEP(0);
		desc->lli.frags = DMA64C_SRC_FRAG_STEP(0);
		desc->lli.dblks = DMA64C_DST_BLK_STEP(0);
		desc->len = period_len;
		break;

	case DMA_SPILT_TO_I2S_AUDIO://rx
		desc->lli.saddr_l = (unsigned int)DMA64C_SRC_ADDR_L(sconfig->src_addr);
		desc->lli.wrapp = (unsigned int)DMA64C_SRC_ADDR_H(sconfig->src_addr);
		desc->lli.daddr_l = (unsigned int)DMA64C_DST_ADDR_L(addr);
		desc->lli.wrapt = (unsigned int)DMA64C_DST_ADDR_H(addr);

		ctrl |= DMA64C_ADDR_FIX(ENABLE);
		ctrl |= DMA64C_ADDR_FIX_SEL(SRC);
		ctrl |= DMA64C_FRAG_LEN(sconfig->src_maxburst);
		ctrl &= ~ DMA64C_DST_WIDTH(0x3);//set 0
		ctrl |= DMA64C_DST_WIDTH(sconfig->split_dst_width>>1);
		desc->lli.fragl = ctrl;
		desc->lli.blkl = DMA64C_BLK_LEN(sconfig->src_maxburst);
		desc->lli.trscl = DMA64C_TRSC_LEN( (sconfig->split_num * sconfig->trsf_step) );
		ctrl_trsfs = (1<<reg_width); //sampling points bytes
		ctrl_trsfs |= (sconfig->trsf_step<<16);
		desc->lli.trsfs = ctrl_trsfs;
		desc->lli.frags = DMA64C_DST_FRAG_STEP(0) | DMA64C_SRC_FRAG_STEP(0);
		desc->lli.frags = DMA64C_SRC_FRAG_STEP(0);
		ctrl_dblks = DMA64C_DST_BLK_STEP_SPILT(1);
		ctrl_dblks |= ( (sconfig->split_num * sconfig->trsf_step) - sconfig->split_dst_width );
		desc->lli.dblks = ctrl_dblks;
		desc->len = period_len;
		break; 

	default:
		return -EINVAL;
	}

	return 0;
}

/**
 * dma64c_cyclic_fill_cid - Fill hardware cid register
 */
static void dma64c_cyclic_fill_cid(struct dma_chan *chan)
{
	struct dma64_chan	*dma64c = to_dma64_chan(chan);
	struct dma64		*dma64 = to_dma64(chan->device);

	dma64_cid_writel(dma64,cid_regs(dma64c->dma_sconfig.slave_id),
		dma64c->chan_common.chan_id + 1);
}

/**
 * dma64c_cyclic_clear_cid - clear hardware cid register
 */
static void dma64c_chan_clear_cid(struct dma_chan *chan)
{
	struct dma64_chan	*dma64c = to_dma64_chan(chan);
	struct dma64		*dma64 = to_dma64(chan->device);

	dma64_cid_writel(dma64,cid_regs(dma64c->dma_sconfig.slave_id),0);
}

/**
 * dma64_prep_dma_cyclic - prepare the cyclic DMA transfer
 * @chan: the DMA channel to prepare
 * @buf_addr: physical DMA address where the buffer starts
 * @buf_len: total number of bytes for the entire buffer
 * @period_len: number of bytes for each period
 * @direction: transfer direction, to or from device
 * @flags: tx descriptor status flags
 */
static struct dma_async_tx_descriptor *
dma64_prep_dma_cyclic(struct dma_chan *chan, dma_addr_t buf_addr, size_t buf_len,
		size_t period_len, enum dma_transfer_direction direction,
		unsigned long flags)
{
	struct dma64_chan	*dma64c = to_dma64_chan(chan);
	struct dma_slave_config *sconfig = &dma64c->dma_sconfig;
	struct dma64_desc		*first = NULL;
	struct dma64_desc		*prev = NULL;
	unsigned long		was_cyclic;
	unsigned int		reg_width;
	unsigned int		periods = buf_len / period_len;
	unsigned int		i;

	if (unlikely(!buf_len || !period_len)) {
		return NULL;
	}

	was_cyclic = test_and_set_bit(DMA64C_IS_CYCLIC, &dma64c->status);
	if (was_cyclic) {
		return NULL;
	}

	if (unlikely(!is_slave_direction(direction)))
		goto err_out;

	if (sconfig->direction == DMA_MEM_TO_DEV)
		reg_width = convert_buswidth(sconfig->dst_addr_width);
	else
		reg_width = convert_buswidth(sconfig->src_addr_width);

	/* Check for too big/unaligned periods and unaligned DMA buffer */
	if (dma64c_cyclic_check_values(reg_width, buf_addr, period_len))
		goto err_out;

	/* build cyclic linked list */
	for (i = 0; i < periods; i++) {
		struct dma64_desc	*desc;

		desc = dma64c_desc_get(dma64c);
		if (!desc)
			goto err_desc_get;

		if (dma64c_cyclic_fill_desc(chan, desc, i, buf_addr,
						 reg_width, period_len, direction))
			goto err_desc_get;

		dma64c_desc_chain(&first, &prev, desc);
	}
	dma64c->chan_common.dma_cycle_buf_len_count = 0;
	dma64c->chan_common.dma_cycle_period_len = period_len;
	dma64c->chan_common.dma_cycle_buf_len = buf_len;

	if(flags & DMA_PREP_FENCE){
		prev->lli.fragl |= DMA64C_LLIST_MOD(LLIST_END);
	} else {
		/* lets make a cyclic list */
		prev->lli.llistp_l = DMA64C_LLIST_PTR_L(first->txd.phys);
		prev->lli.sblks = DMA64C_LLIST_PTR_H(first->txd.phys);
	}

	/* First descriptor of the chain embedds additional information */
	first->txd.cookie = -EBUSY;
	first->total_len = buf_len;

	if(flags & DMA_PREP_INTERRUPT)
		channel_writel(dma64c,INT_REG,
			channel_readl(dma64c,INT_REG)|DMA64C_TRSC_INT_EN );

	dma64c_cyclic_fill_cid(chan);

	dma64c->cyclic_desc = first;

	return &first->txd;

err_desc_get:
	dev_err(chan2dev(chan), "not enough descriptors available\n");
	dma64c_desc_put(dma64c, first);
err_out:
	clear_bit(DMA64C_IS_CYCLIC, &dma64c->status);
	return NULL;
}

static int dma64_config(struct dma_chan *chan,
			  struct dma_slave_config *sconfig)
{
	struct dma64_chan	*dma64c = to_dma64_chan(chan);

	/* Check if it is chan is configured for slave transfers */
	if(!is_slave_direction(sconfig->direction))
		return -EINVAL;

	memcpy(&dma64c->dma_sconfig, sconfig, sizeof(*sconfig));

	return 0;
}

static int dma64_terminate_all(struct dma_chan *chan)
{
  struct dma64_chan  *dma64c = to_dma64_chan(chan);
  struct dma64_desc	  *desc, *_desc;
  unsigned long 	  flags;

  LIST_HEAD(list);

  /*
   * This is only called when something went wrong elsewhere, so
   * we don't really care about the data. Just disable the
   * channel. We still have to poll the channel enable bit due
   * to AHB/HSB limitations.
   */
  spin_lock_irqsave(&dma64c->lock, flags);

  /* disabling channel: must also remove suspend state */
  channel_writel(dma64c, CFG_REG, 0);

  /* clear channel cid */
  dma64c_chan_clear_cid(chan);

  /* confirm that this channel is disabled */
  while (dma64_chan_is_enabled(dma64c))
	  cpu_relax();

  /* active_list entries will end up before queued entries */
  list_splice_init(&dma64c->queue, &list);
  list_splice_init(&dma64c->active_list, &list);

  /* Flush all pending and queued descriptors */
  list_for_each_entry_safe(desc, _desc, &list, desc_node)
	  dma64c_chain_complete(dma64c, desc);

  clear_bit(DMA64C_IS_PAUSED, &dma64c->status);
  /* if channel dedicated to cyclic operations, free it */
  clear_bit(DMA64C_IS_CYCLIC, &dma64c->status);

  spin_unlock_irqrestore(&dma64c->lock, flags);

  return 0;
}

/**
 * dma64_tx_status - poll for transaction completion
 * @chan: DMA channel
 * @cookie: transaction identifier to check status of
 * @txstate: if not %NULL updated with transaction state
 *
 * If @txstate is passed in, upon return it reflect the driver
 * internal state and can be used with dma_async_is_complete() to check
 * the status of multiple cookies without re-checking hardware state.
 */
static enum dma_status
dma64_tx_status(struct dma_chan *chan,
		dma_cookie_t cookie,
		struct dma_tx_state *txstate)
{
	struct dma64_chan	*dma64c = to_dma64_chan(chan);
	unsigned long		flags;
	enum dma_status		ret;
	int bytes = 0;

	ret = dma_cookie_status(chan, cookie, txstate);
	if (ret == DMA_COMPLETE)
		return ret;
	/*
	 * There's no point calculating the residue if there's
	 * no txstate to store the value.
	 */
	if (!txstate)
		return DMA_ERROR;

	spin_lock_irqsave(&dma64c->lock, flags);

	/*  Get number of bytes left in the active transactions */
	bytes = dma64c_get_bytes_left(chan, cookie);

	spin_unlock_irqrestore(&dma64c->lock, flags);

	if (unlikely(bytes < 0)) {
		return DMA_ERROR;
	} else {
		dma_set_residue(txstate, bytes);
	}

	return ret;
}


/**
 * dma64_issue_pending - try to finish work
 * @chan: target DMA channel
 */
static void dma64_issue_pending(struct dma_chan *chan)
{
	struct dma64_chan	*dma64c = to_dma64_chan(chan);
	unsigned long		flags;

	spin_lock_irqsave(&dma64c->lock, flags);
	dma64_advance_work(dma64c);
	spin_unlock_irqrestore(&dma64c->lock, flags);
}

/**
 * dma64_alloc_chan_resources - allocate resources for DMA channel
 * @chan: allocate descriptor resources for this channel
 * @client: current client requesting the channel be ready for requests
 *
 * return - the number of allocated descriptors
 */
static int dma64_alloc_chan_resources(struct dma_chan *chan)
{
	struct dma64_chan *dma64c = to_dma64_chan(chan);
	struct dma64		*dma64_common = to_dma64(chan->device);
	struct dma64_desc		*desc;
	unsigned long		flags;
	int i;
	LIST_HEAD(tmp_list);

	/* ASSERT:  channel is idle */
	if (dma64_chan_is_enabled(dma64c)) {
		return -EIO;
	}

	/* have we already been set up?
	 * reconfigure channel but no need to reallocate descriptors */
	if (!list_empty(&dma64c->free_list))
		return dma64c->descs_allocated;

	/* Allocate initial pool of descriptors */
	for (i = 0; i < init_nr_desc_per_channel; i++) {
		desc = dma64c_alloc_descriptor(chan, GFP_KERNEL);
		if (!desc) {
			dev_err(dma64_common->dma64_common.dev,
				"Only %d initial descriptors\n", i);
			break;
		}
		list_add_tail(&desc->desc_node, &tmp_list);
	}

	spin_lock_irqsave(&dma64c->lock, flags);
	dma64c->descs_allocated = i;
	list_splice(&tmp_list, &dma64c->free_list);
	dma_cookie_init(chan);
	spin_unlock_irqrestore(&dma64c->lock, flags);

	return dma64c->descs_allocated;
}

void dma64_free_chan_resources(struct dma_chan *chan)
{
	struct dma64_chan	*dma64c = to_dma64_chan(chan);
	struct dma64		*dma64 = to_dma64(chan->device);
	struct dma64_desc		*desc, *_desc;
	LIST_HEAD(list);

	/* ASSERT:  channel is idle */
	BUG_ON(!list_empty(&dma64c->active_list));
	BUG_ON(!list_empty(&dma64c->queue));
	BUG_ON(dma64_chan_is_enabled(dma64c));

	/* cyclic desc move to free list */
	if(dma64_chan_is_cyclic(dma64c))
		dma64c_desc_put(dma64c,dma64c->cyclic_desc);

	list_for_each_entry_safe(desc, _desc, &dma64c->free_list, desc_node) {
		list_del(&desc->desc_node);
		/* free link descriptor */
		dma_pool_free(dma64->desc_pool, desc, desc->txd.phys);
	}
	list_splice_init(&dma64c->free_list, &list);
	dma64c->descs_allocated = 0;
	dma64c->status = 0;

}

static struct dma_chan* dma64_of_xlate(struct of_phandle_args *dma_spec,
					struct of_dma *ofdma){
	return NULL;
}

/**
 * dma64_on - enable DMA controller
 * @dma64_common: the DMA64 device
 */
static void dma64_on(struct dma64 *dma64_common)
{
	unsigned int reg;
	dma64_writel(dma64_common,PAUSE_REG,DMA_CLK_ALL_EN);
	reg = dma64_readl(dma64_common,FRAG_WAIT_REG);
	reg &= ~DMA_FRAG_WAIT_MASK;
	reg |= DMA_FRAG_WAIT(0);
	dma64_writel(dma64_common,FRAG_WAIT_REG,reg);
}

/**
 * dma64_reset - reset DMA controller
 * @dma64_common: the DMA64 device
 */
static void dma64_reset(struct dma64 *dma64_common)
{
    regmap_update_bits(dma64_common->cpu_sysy_base, DMA64_SOFT_RST_OFFSET, DMA64_DMA_SOFT_RST, DMA64_DMA_SOFT_RST_VALUE_HIGH);
    udelay(2);
    regmap_update_bits(dma64_common->cpu_sysy_base, DMA64_SOFT_RST_OFFSET, DMA64_DMA_SOFT_RST, DMA64_DMA_SOFT_RST_VALUE_LOW);
    
    printk("Molchip DMA 64-bit Soft Reset\n");
}

static int dma64_probe(struct platform_device *pdev)
{
	struct dma64 	*dma64;
	struct device *dev = &pdev->dev;
	struct resource 	*mem;
	struct device_node *np = pdev->dev.of_node;
	struct dma64_data *pdata = dev_get_platdata(dev);
	int i;
	int	irq;
	int err;
	static int dma_id = 0;
	pdata = devm_kzalloc(dev,sizeof(*pdata),GFP_KERNEL);
	if(!pdata) {
		return -ENOMEM;
	}

	of_property_read_u32(np, "dma-channels", &(pdata->nr_channels));
	of_property_read_u32(np, "chan_allocation_order", &(pdata->chan_allocation_order));

	dma64 = devm_kzalloc(dev,sizeof(*dma64),GFP_KERNEL);
	if(!dma64) {
		err = -ENOMEM;
		goto err_exit;
	}

	dma64_device[dma_id] = dma64;
	dma_id++;
	if(dma_id > DMA64_MAX){
		err = -ENXIO;
		goto err_exit;

	}

	dma64->pdata = pdata;

	dma64->chan = devm_kzalloc(dev,(pdata->nr_channels) * sizeof(*dma64->chan),GFP_KERNEL);
	if(!dma64->chan){
		err = -ENOMEM;
		goto err_exit;
	}

	mem = platform_get_resource(pdev,IORESOURCE_MEM,0);
	dma64->regs = devm_ioremap_resource(dev,mem);
	if(IS_ERR(dma64->regs)){
		err = PTR_ERR(dma64->regs);
		goto err_exit;
	}

	dma64->cpu_sysy_base = syscon_regmap_lookup_by_phandle(np,"molchip,syscon-cpu-reg");
		if (IS_ERR(dma64->cpu_sysy_base)){
		err = PTR_ERR(dma64->cpu_sysy_base);
		goto err_exit;
	}

  /* Initializing the clock */
	err = clk_mini_enable(CKG_DMA0_EN);
	if(err)
		goto err_exit;

	/* soft reset DMA controller */
	dma64_reset(dma64);

	irq = platform_get_irq(pdev,0);
	if(irq < 0){
		err = -ENOMEM;
		goto err_irq;
	}

	err = devm_request_irq(dev, irq, &dma64_interrupt, 0,
						   dev_name(dev), dma64);
	if (err)
		goto err_irq;

        irq = platform_get_irq(pdev, 1);
	if(irq < 0){
		err = -ENOMEM;
		goto err_irq;
	}

	err = devm_request_irq(dev, irq, &dma64_interrupt, 0,
						"dma_ch31-24", dma64);
	if (err)
		goto err_irq;

	platform_set_drvdata(pdev,dma64);

	/*create a pool of consistent memory blocks for hardware descriptor*/
	dma64->desc_pool = dmam_pool_create("dmac64_desc_pool",dev,
			sizeof(struct dma64_desc),8,0);

	if(!dma64->desc_pool){
		dev_err(dev,"No memory for descriptors dma pool\r\n");
		err = -ENOMEM;
		goto err_desc_pool_create;
	}

	INIT_LIST_HEAD(&dma64->dma64_common.channels);
	for (i = 0; i < pdata->nr_channels; i++) {
		struct dma64_chan	*chan = &dma64->chan[i];
		chan->chan_common.device = &dma64->dma64_common;
		dma_cookie_init(&chan->chan_common);
		if(pdata->chan_allocation_order==CHAN_ALLOCATION_ASCENDING)
			list_add_tail(&chan->chan_common.device_node,
				&dma64->dma64_common.channels);
		else
			list_add(&chan->chan_common.device_node,
				&dma64->dma64_common.channels);

		chan->ch_regs = dma64->regs + ch_regs(i);
		spin_lock_init(&chan->lock);
		spin_lock_init(&chan->dma_int_count_lock);
		INIT_LIST_HEAD(&chan->active_list);
		INIT_LIST_HEAD(&chan->queue);
		INIT_LIST_HEAD(&chan->free_list);

		tasklet_init(&chan->tasklet, dma64c_tasklet, (unsigned long)chan);
	}


	/*set capabilities*/
	dma_cap_set(DMA_SG,dma64->dma64_common.cap_mask);
	dma_cap_set(DMA_SLAVE,dma64->dma64_common.cap_mask);
	dma_cap_set(DMA_CYCLIC,dma64->dma64_common.cap_mask);
	dma_cap_set(DMA_MEMCPY,dma64->dma64_common.cap_mask);
	dma64->dma64_common.dev = dev;
	dma64->dma64_common.device_alloc_chan_resources = dma64_alloc_chan_resources;
	dma64->dma64_common.device_free_chan_resources = dma64_free_chan_resources;

	dma64->dma64_common.device_prep_dma_memcpy = dma64_prep_dma_memcpy;
	dma64->dma64_common.device_prep_slave_sg = dma64_prep_slave_sg;
	dma64->dma64_common.device_prep_dma_cyclic = dma64_prep_dma_cyclic;
	dma64->dma64_common.device_prep_dma_sg = dma64_prep_dma_sg;

	dma64->dma64_common.device_config = dma64_config;
	dma64->dma64_common.device_tx_status = dma64_tx_status;
	dma64->dma64_common.device_issue_pending = dma64_issue_pending;
	dma64->dma64_common.device_terminate_all = dma64_terminate_all;


#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
	err = dma_coerce_mask_and_coherent(&pdev->dev,DMA_BIT_MASK(64));
#else
	err = dma_coerce_mask_and_coherent(&pdev->dev,DMA_BIT_MASK(32));
#endif
	if(err)
		goto err_corece_mask_coherent;

	dma64->dma64_common.src_addr_widths = DMA64_BUSWIDTHS;
	dma64->dma64_common.dst_addr_widths = DMA64_BUSWIDTHS;
	dma64->dma64_common.directions = BIT(DMA_DEV_TO_MEM)|BIT(DMA_MEM_TO_DEV)|
		BIT(DMA_MEM_TO_MEM);

	dma64->dma64_common.residue_granularity = DMA_RESIDUE_GRANULARITY_DESCRIPTOR;

	dma_async_device_register(&dma64->dma64_common);

	if(pdev->dev.of_node){
		err = of_dma_controller_register(pdev->dev.of_node,dma64_of_xlate,dma64);
		if(err){
			dev_err(&pdev->dev,"could not register of_dma_controller\n");
			goto err_of_dma_controller_register;
		}
	}

	/* Turn on DMA controller */
	dma64_on(dma64);

	dev_info(dev, "Found Molchip integrated DMA 64-bit\n");
	return 0;

err_of_dma_controller_register:
	dma_async_device_unregister(&dma64->dma64_common);
err_corece_mask_coherent:
	dma_pool_destroy(dma64->desc_pool);
err_desc_pool_create:
	free_irq(platform_get_irq(pdev, 0), dma64);
err_irq:
	clk_mini_disable(CKG_DMA0_EN);
err_exit:
	return err;
}



static int dma64_remove(struct platform_device *pdev)
{
	return 0;
}

static void dma64_shutdown(struct platform_device *pdev)
{

}

static const struct of_device_id mc_dma_of_id_table[] ={
	{.compatible = "molchip,mc-dma64"},
	{}
};

#ifdef CONFIG_PM_SLEEP
static int molchip_dma64_suspend(struct device *dev)
{
	struct dma64 	*dma64_common = dev_get_drvdata(dev);
	u32				mask;
	do{
		mask = dma64_readl(dma64_common, STAT_REG);
	}while(mask & DMA_BUSY);

	mask = dma64_readl(dma64_common, PAUSE_REG);
	mask |= DMA_PAUSE;
	dma64_writel(dma64_common, PAUSE_REG,mask);

	clk_mini_disable(CKG_DMA0_EN);
	printk("--%s--\n",__func__);
	return 0;
}

static int molchip_dma64_resume(struct device *dev)
{
	struct dma64 	*dma64_common = dev_get_drvdata(dev);
	u32				mask;
	
	clk_mini_enable(CKG_DMA0_EN);

	mask = dma64_readl(dma64_common, PAUSE_REG);
	mask &= ~DMA_PAUSE;
	dma64_writel(dma64_common, PAUSE_REG,mask);

	printk("--%s--\n",__func__);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(molchip_dma64_pm_ops,
			 molchip_dma64_suspend, molchip_dma64_resume);


static struct platform_driver molchip_driver = {
	.probe		= dma64_probe,
	.remove		= dma64_remove,
	.shutdown	= dma64_shutdown,
	.driver		={
		.name	=DRV_NAME,
		.of_match_table = of_match_ptr(mc_dma_of_id_table),
		.pm	= &molchip_dma64_pm_ops,  
	},

};

static int __init molchip_init(void)
{
	return platform_driver_register(&molchip_driver);
}
subsys_initcall(molchip_init);

static void __exit molchip_exit(void)
{
	platform_driver_unregister(&molchip_driver);
}
module_exit(molchip_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("molchip DMA64 controller");
MODULE_ALIAS("platform:" DRV_NAME);

