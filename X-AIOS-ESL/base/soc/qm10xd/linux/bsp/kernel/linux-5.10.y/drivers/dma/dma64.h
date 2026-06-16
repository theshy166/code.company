#ifndef _DMA_MOLCHIP_H
#define _DMA_MOLCHIP_H

#define DMA64_BUSWIDTHS	\
	BIT(DMA_SLAVE_BUSWIDTH_UNDEFINED)	|\
	BIT(DMA_SLAVE_BUSWIDTH_1_BYTE)		|\
	BIT(DMA_SLAVE_BUSWIDTH_2_BYTES)		|\
	BIT(DMA_SLAVE_BUSWIDTH_4_BYTES)		|\
	BIT(DMA_SLAVE_BUSWIDTH_8_BYTES)

enum dma64_req_mode{
	FRAGMENT = 0,
	BLOCK,
	TRANSACTION,
	LINKLIST
};

enum dma64_swt_mode{
	ABCD2ABCD = 0,
	ABCD2DCBA,
	ABCD2BADC,
	ABCD2DCAB
};

enum dma64_slave_id{
	MC_HASH = 28,
};

//DMA_SOFT_RET Register
#define	DMA64_SOFT_RST_OFFSET           0x20 /* Pause Register */
#define DMA64_DMA_SOFT_RST              (1<<23)
#define DMA64_DMA_SOFT_RST_VALUE_HIGH   (1<<23)
#define DMA64_DMA_SOFT_RST_VALUE_LOW    (0<<23)

#define	DMA64_PAUSE_REG		0x0	/* Pause Register */
#define	DMA64_FRAG_WAIT_REG	0x4	/* Frag wait Register */
#define	DMA64_RAW_REG		0x10	/* Status Register */
#define	DMA64_MASK_REG		0x14	/* Mask Register */
#define	DMA64_STAT_REG		0x20	/* State Register */

#define DMA_BLK_MAXLEN_MSK		GENMASK(16,0)
#define DMA_FRAG_MAXLEN_MSK		GENMASK(16,0)

//DMA_PAUSE Register Bits
#define AUDIO_CNT_CLK_EN	(1 << 21)
#define AXI_MST_CLK_EN		(1 << 20)
#define INT_REG_CLK_EN		(1 << 19)
#define REQ_CID_CLK_EN		(1 << 18)
#define CHNL_REG_CLK_EN		(1 << 17)
#define GLB_REG_CLK_EN		(1 << 16)
#define DMA_PAUSE			(1 << 0)

#define DMA_CLK_ALL_EN	\
	AUDIO_CNT_CLK_EN |\
	AXI_MST_CLK_EN	 |\
	INT_REG_CLK_EN	 |\
	REQ_CID_CLK_EN	 |\
	CHNL_REG_CLK_EN	 |\
	GLB_REG_CLK_EN

//DMA64_FRAG_WAIT Register Bits
#define DMA_FRAG_WAIT_MASK		((1 << 16) - 1)
#define DMA_FRAG_WAIT(x)	((x) & DMA_FRAG_WAIT_MASK)

//DMA_STAT Register Bits
#define DMA_BUSY	(1 << 20)

#define DMA64_CH_PAUSE_REG	0x0
#define DMA64_CH_REQ_REG	0x4
#define DMA64_CH_CFG_REG	0x8
#define DMA64_CH_INT_REG	0xC
#define DMA64_CH_SADDR_REG	0x10
#define DMA64_CH_DADDR_REG	0x14
#define DMA64_CH_FRAGL_REG	0x18
#define DMA64_CH_BLKL_REG	0x1C
#define DMA64_CH_TRSCL_REG	0x20
#define DMA64_CH_TRSFS_REG	0x24
#define DMA64_CH_WRAPP_REG	0x28
#define DMA64_CH_WRAPT_REG	0x2C
#define DMA64_CH_LLISTP_REG	0x30
#define DMA64_CH_FRAGS_REG	0x34
#define DMA64_CH_SBLKS_REG	0x38
#define DMA64_CH_DBLKS_REG	0x3C




#define DMA64_CH_LLIST_PTR_L	0x30
#define DMA64_CH_LLIST_PTR_H	0x38

#define TRSC_LEN_MASK			0xfffffff;
#define CHN_TRSC_LEN_LLIST_NODE_INT_EN			(0x1<<31)
#define BLK_LEN_MASK					0x1ffff;
#define FRAG_LEN_MASK					0x1ffff;
#define REQ_MODE_MASK					0x3
#define REQ_MODE_SHIFT					24
#define SRC_DST_AHB_SIZE_MASK				0xf
#define DEST_AHB_SIZE_SHIFT				28
#define SRC_AHB_SIZE_SHIFT				30
#define ADDR_FIX_EN_SHIFT				20
#define DST_FIX						1
#define SRC_FIX					    	0
#define ADDR_WRAP_EN_SHIFT				22
#define TRSF_STEP_MASK					0xffffffff
#define LLIST_END_SHIFT					19
#define ADDR_FIX_SEL_SHIFT				21


//FULL_CH_PAUSE Register Bits
#define DMA64C_PAUSE	(0x1<<0)

//FULL_CH_REQ Register Bits
#define DMA64C_REQ	(0x1<<0)

//FULL_CH_CFG Register Bits
#define DMA64C_LLIST_EN	(0x1<<4)
#define DMA64C_EN	(0x1<<0)

//FULL_CH_INT Register Bits
#define CHN_CFG_ERR_INT_CLR				(0x1<<28)
#define CHN_LINKLIST_INT_CLR			(0x1<<27)
#define CHN_TRSC_INT_CLR				(0x1<<26)
#define CHN_BLK_INT_CLR					(0x1<<25)
#define CHN_FRAGMENT_INT_CLR			(0x1<<24)

#define CHN_CFG_ERR_INT_MASK			(0x1<<20)
#define CHN_LINKLIST_INT_MASK			(0x1<<19)
#define CHN_TRSC_INT_MASK				(0x1<<18)
#define CHN_BLK_INT_MASK				(0x1<<17)
#define CHN_FRAGMENT_INT_MASK			(0x1<<16)


#define DMA64C_CFG_ERR_INT_EN	(0x1<<4)
#define DMA64C_LLIST_INT_EN		(0x1<<3)
#define DMA64C_TRSC_INT_EN		(0x1<<2)
#define DMA64C_BLK_INT_EN		(0x1<<1)
#define DMA64C_FRAGMENT_INT_EN	(0x1<<0)

//FULL_CH_SRC_ADDR Register Bits
#define DMA64C_SRC_ADDR_L(x)	(x)

//FULL_CH_DST_ADDR Register Bits
#define DMA64C_DST_ADDR_L(x)	(x)

//FULL_CH_FRAG_LEN Register Bits
#define DMA64C_SRC_WIDTH_OFFSET	30
#define DMA64C_SRC_WIDTH(x)		((x) << DMA64C_SRC_WIDTH_OFFSET)
#define DMA64C_DST_WIDTH_OFFSET	28
#define DMA64C_DST_WIDTH(x)		((x) << DMA64C_DST_WIDTH_OFFSET)
#define DMA64C_DST_BLK_STEP_SPILT_OFFSET	27
#define DMA64C_DST_BLK_STEP_SPILT(x)		((x) << DMA64C_DST_BLK_STEP_SPILT_OFFSET)
#define DMA64C_SWP_MODE_OFFSET	26
#define DMA64C_SWP_MODE(x)	((x) << DMA64C_SWP_MODE_OFFSET)
#define DMA64C_REQ_MODE_OFFSET	24
#define DMA64C_REQ_MOD(x)	((x) << DMA64C_REQ_MODE_OFFSET)
#define DMA64C_ADDR_WRAP_SEL_OFFSET	23
#define DMA64C_ADDR_WRAP_SEL(x)	((x) << DMA64C_ADDR_WRAP_SEL_OFFSET)
#define DMA64C_ADDR_WRAP_EN_OFFSET	22
#define DMA64C_ADDR_WRAP(x)		((x) << DMA64C_ADDR_WRAP_EN_OFFSET)
#define DMA64C_ADDR_FIX_SEL_OFFSET	21
#define DMA64C_ADDR_FIX_SEL(x)	((x) << DMA64C_ADDR_FIX_SEL_OFFSET)
#define DMA64C_ADDR_FIX_EN_OFFSET	20
#define DMA64C_ADDR_FIX(x)			((x) << DMA64C_ADDR_FIX_EN_OFFSET)
#define DMA64C_LLIST_END_OFFSET		19
#define DMA64C_LLIST_MOD(x)			((x) << DMA64C_LLIST_END_OFFSET)
#define DMA64C_FRAG_LEN_MASK	((1<<17)-1)
#define DMA64C_FRAG_LEN(x)	((x) & DMA64C_FRAG_LEN_MASK)

//FULL_CH_BLK_LEN Register Bits
#define DMA64C_BLK_LEN_MASK		((1<<17)-1)
#define DMA64C_BLK_LEN(x)		((x)&DMA64C_BLK_LEN_MASK)

//FULL_CH_TRSC_LEN Register Bits
#define DMA64C_LLIST_NODE_INT_EN	(0x1<<31)
#define DMA64C_LLIST_NODE_INT_TRSC	(0x1<<30)
#define DMA64C_LLIST_NODE_INT_BLK	(0x1<<29)
#define DMA64C_LLIST_NODE_INT_FRAG	(0x1<<28)
#define DMA64C_TRSC_LEN_MASK	((1 << 28) - 1)
#define DMA64C_TRSC_LEN(x)	((x) & DMA64C_TRSC_LEN_MASK)

//FULL_CH_TRSF_STEP Register Bits
#define	DMA64C_DST_TRSF_STEP_OFFSET	16
#define	DMA64C_DST_TRSF_STEP_MASK	0xffff0000
#define	DMA64C_SRC_TRSF_STEP_MASK	0xffff
#define	DMA64C_DST_TRSF_STEP(x)	((x << DMA64C_DST_TRSF_STEP_OFFSET)&DMA64C_DST_TRSF_STEP_MASK)
#define	DMA64C_SRC_TRSF_STEP(x)	((x)&DMA64C_SRC_TRSF_STEP_MASK)


//FULL_CH_WRAP_PTR Register Bits
#define	DMA64C_SRC_ADDR_H_OFFSET	28
#define	DMA64C_WRAP_PTR_MASK		0xfffffff
#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
#define	DMA64C_SRC_ADDR_H(x)		(((x >> 32) & 0xF) << DMA64C_SRC_ADDR_H_OFFSET)
#else
#define	DMA64C_SRC_ADDR_H(x)		(0)
#endif
#define	DMA64C_WRAP_PTR(x)			((x)&DMA64C_WRAP_PTR_MASK)


//FULL_CH_WRAP_TO Register Bits
#define	DMA64C_DST_ADDR_H_OFFSET	28
#define	DMA64C_WRAP_TO_MASK		0xfffffff
#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
#define	DMA64C_DST_ADDR_H(x)		(((x >> 32) & 0xF) << DMA64C_DST_ADDR_H_OFFSET)
#else
#define	DMA64C_DST_ADDR_H(x)		(0)
#endif
#define	DMA64C_WRAP_TO(x)			((x)&DMA64C_WRAP_TO_MASK)

//FULL_CH_LLIST_PTR Register Bits
#define	DMA64C_LLIST_PTR_L(x)		(x)

//FULL_CH_FRAG_STEP Register Bits
#define	DMA64C_DST_FRAG_STEP(x)		((x) << 16)
#define	DMA64C_SRC_FRAG_STEP(x)		(x)

//FULL_CH_SRC_BLK_STEP Register Bits
#define	DMA64C_LLIST_PTR_H_OFFSET	28
#define	DMA64C_SRC_BLK_STEP_MASK	0xfffffff
#ifdef CONFIG_ARCH_DMA_ADDR_T_64BIT
#define	DMA64C_LLIST_PTR_H(x)		(((x >> 32) & 0xF) << DMA64C_LLIST_PTR_H_OFFSET)
#else
#define	DMA64C_LLIST_PTR_H(x)		(0)
#endif

#define	DMA64C_SRC_BLK_STEP(x)		((x) & DMA64C_SRC_BLK_STEP_MASK)

//FULL_CH_DST_BLK_STEP Register Bits
#define	DMA64C_DST_BLK_STEP_MASK	0xfffffff
#define	DMA64C_DST_BLK_STEP(x)		((x) & DMA64C_DST_BLK_STEP_MASK)

#define	DMA64_CH_REGS_BASE	0x1000	/* Channel registers base address */
#define	ch_regs(x)	(DMA64_CH_REGS_BASE + (x) * 0x40) /* Channel x base addr */

#define	DMA64_REQ_CID_REGS_BASE	0x2000	/* req cid registers base address */
#define	cid_regs(x)	(DMA64_REQ_CID_REGS_BASE + (x)*4) /* Channel x base addr */

//DMA split config Bits
#define DMA64_SRC_SIZE  0x2  /* 4 bytes (1 bytes if set 0) */
#define DMA64_DST_SIZE	0x2  /* 4 bytes (1 bytes if set 0) */

/* various constants */
#define ENABLE			1
#define DISABLE			0

#define DST			1
#define SRC			0

#define LLIST_END			1
#define LLIST_NOT_END		0

#define DMA64_FRAG_BTSIZE_MAX	DMA64C_FRAG_LEN_MASK
#define DMA64_BLK_BTSIZE_MAX	DMA64C_BLK_LEN_MASK
#define DMA64_TRSC_BTSIZE_MAX	DMA64C_TRSC_LEN_MASK

/* Hardware descriptor for Linked LIst transfers */
struct dma64_lli{
	unsigned int	saddr_l;
	unsigned int	daddr_l;
	unsigned int	fragl;
	unsigned int	blkl;
	unsigned int	trscl;
	unsigned int	trsfs;
	unsigned int  	wrapp; //[31:28] high src addr [27:0] wrapp
	unsigned int	wrapt; //[31:28] high dest addr [27:0] wrapt
	unsigned int	llistp_l;
	unsigned int	frags;
	unsigned int	sblks; //[31:28] high llist ptr [27:0] src blk step
	unsigned int	dblks;
} __attribute__((aligned(16)));

struct dma64_desc {
	/* FIRST values the hardware uses */
	struct dma64_lli			lli;

	/* THEN values for driver housekeeping */
	struct list_head		tx_list;
	struct dma_async_tx_descriptor	txd;
	struct list_head		desc_node;
	size_t				len;
	size_t				total_len;

};

static inline struct dma64_desc *
txd_to_dma64_desc(struct dma_async_tx_descriptor *txd)
{
	return container_of(txd, struct dma64_desc, txd);
}

/*--  Channels  --------------------------------------------------------*/

/**
 * dma64c_status - information bits stored in channel status flag
 *
 * Manipulated with atomic operations.
 */
enum dma64c_status {
	DMA64C_IS_ERROR = 0,
	DMA64C_IS_PAUSED = 1,
	DMA64C_IS_CYCLIC = 24,
};

/**
 * struct at_dma_chan - internal representation of an Atmel HDMAC channel
 * @chan_common: common dmaengine channel object members
 * @device: parent device
 * @ch_regs: memory mapped register base
 * @status: transmit status information from irq/prep* functions
 *                to tasklet (use atomic operations)
 * @tasklet: bottom half to finish transaction work
 * @save_cfg: configuration register that is saved on suspend/resume cycle
 * @save_dscr: for cyclic operations, preserve next descriptor address in
 *             the cyclic list on suspend/resume cycle
 * @dma_sconfig: configuration for slave transfers, passed via
 * .device_config
 * @lock: serializes enqueue/dequeue operations to descriptors lists
 * @active_list: list of descriptors dmaengine is being running on
 * @queue: list of descriptors ready to be submitted to engine
 * @free_list: list of descriptors usable by the channel
 * @descs_allocated: records the actual size of the descriptor pool
 */
struct dma64_chan {
	struct dma_chan		chan_common;
	struct dma64		*device;
	void __iomem		*ch_regs;
	unsigned long		status;
	struct tasklet_struct	tasklet;
	u32			save_cfg;
	u32			save_dscr;
	struct dma_slave_config dma_sconfig;
	struct dma64_desc		*cyclic_desc;

	spinlock_t		lock;
	spinlock_t      dma_int_count_lock;

	/* these other elements are all protected by lock */
	struct list_head	active_list;
	struct list_head	queue;
	struct list_head	free_list;
	unsigned int		descs_allocated;
};

#define channel_readl(dma64c, name)		\
	__raw_readl((dma64c)->ch_regs + DMA64_CH_##name)
#define channel_writel(dma64c, name, val)	\
	__raw_writel((val), dma64c->ch_regs + DMA64_CH_##name)

static inline struct dma64_chan *to_dma64_chan(struct dma_chan *chan)
{
	return container_of(chan, struct dma64_chan, chan_common);
}

/*
 * Fix sconfig's bus width according to at_hdmac.
 * 1 byte -> 0, 2 bytes -> 1, 4 bytes -> 2.
 */
static inline u8 convert_buswidth(enum dma_slave_buswidth addr_width)
{
	switch (addr_width) {
	case DMA_SLAVE_BUSWIDTH_2_BYTES:
		return 1;
	case DMA_SLAVE_BUSWIDTH_4_BYTES:
		return 2;
	case DMA_SLAVE_BUSWIDTH_8_BYTES:
		return 3;
	default:
		/* For 1 byte width or fallback */
		return 0;
	}
}


/**
 * dma64c_is_enabled - test if given channel is enabled
 * @atchan: channel we want to test status
 */
static inline int dma64_chan_is_enabled(struct dma64_chan *dma64c)
{
	return (channel_readl(dma64c, CFG_REG) & DMA64C_EN);
}

/**
 * dma64_chan_is_paused - test channel pause/resume status
 * @dma64c: channel we want to test status
 */
static inline int dma64_chan_is_paused(struct dma64_chan *dma64c)
{
	return test_bit(DMA64C_IS_PAUSED, &dma64c->status);
}

/**
 * dma64_chan_is_cyclic - test if given channel has cyclic property set
 * @dma64c: channel we want to test status
 */
static inline int dma64_chan_is_cyclic(struct dma64_chan *dma64c)
{
	return test_bit(DMA64C_IS_CYCLIC, &dma64c->status);
}

struct dma64{
	struct dma_device	dma64_common;

	void __iomem *regs;

	struct clk 	*clk;

	/* channels */
	unsigned short all_chan_mask;
	struct dma64_chan *chan;

	struct dma64_data *pdata;

	struct dma_pool		*desc_pool;

	struct regmap *cpu_sysy_base;
	
};

#define dma64_cid_readl(dma64, offset)			\
	__raw_readl((dma64)->regs + offset)
#define dma64_cid_writel(dma64, offset, val)		\
	__raw_writel((val), (dma64)->regs + offset)


#define dma64_readl(dma64, name)			\
	__raw_readl((dma64)->regs + DMA64_##name)
#define dma64_writel(dma64, name, val)		\
	__raw_writel((val), (dma64)->regs + DMA64_##name)

static inline struct dma64 *to_dma64(struct dma_device *ddev)
{
	return container_of(ddev, struct dma64, dma64_common);
}


/**
 * struct dma64_data - Controller configuration parameters
 * @nr_channels: Number of channels supported by hardware (max 8)
 * @is_nollp: The device channels does not support multi block transfers.
 * @chan_allocation_order: Allocate channels starting from 0 or 7
 * @chan_priority: Set channel priority increasing from 0 to 7 or 7 to 0.
 * @fragment_size: Maximum fragment_size supported by the controller
 * @nr_masters: Number of AHB masters supported by the controller
 * @data_width: Maximum data width supported by hardware per AHB master
 *		(in bytes, power of 2)
 */
struct dma64_data {
	u32	nr_channels;
	bool		is_nollp;
#define CHAN_ALLOCATION_ASCENDING	0	/* 0 to 31 */
#define CHAN_ALLOCATION_DESCENDING	1	/* 31 to 0 */
	unsigned int	chan_allocation_order;
	unsigned char	nr_masters;
};

/*--  Helper functions  ------------------------------------------------*/

static struct device *chan2dev(struct dma_chan *chan)
{
	return &chan->dev->device;
}

#if defined(VERBOSE_DEBUG)
#if 0
static void vdbg_dump_regs(struct dma64_chan *dma64c)
{

	dev_info(chan2dev(&dma64c->chan_common),
		"channel %u : pause = 0x%x, req = 0x%x cfg = 0x%x int = 0x%x  \
		0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		dma64c->chan_common.chan_id,
		channel_readl(dma64c, PAUSE_REG),
		channel_readl(dma64c, REQ_REG),
		channel_readl(dma64c, CFG_REG),
		channel_readl(dma64c, INT_REG),
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
}
#endif
#else
//static void vdbg_dump_regs(struct dma64_chan *dma64c) {}
#endif

static void dma64_dump_lli(struct dma64_chan *dma64c, struct dma64_lli *lli)
{
	dev_info(chan2dev(&dma64c->chan_common),
		"channel lli %u : 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		dma64c->chan_common.chan_id,
		lli->saddr_l,
		lli->daddr_l,
		lli->fragl,
		lli->blkl,
		lli->trscl,
		lli->trsfs,
		lli->wrapp,
		lli->wrapt,
		lli->llistp_l,
		lli->frags,
		lli->sblks,
		lli->dblks);
}


/**
 * set_desc_eol - set end-of-link to descriptor so it will end transfer
 * @desc: descriptor, signle or at the end of a chain, to end chain on
 */
static void set_desc_eol(struct dma64_desc *desc)
{
	desc->lli.fragl |= DMA64C_LLIST_MOD(LLIST_END);
}
#endif

