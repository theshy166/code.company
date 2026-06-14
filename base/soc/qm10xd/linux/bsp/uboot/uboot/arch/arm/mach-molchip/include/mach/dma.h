#ifndef _MOLCHIP_DMA_H_
#define _MOLCHIP_DMA_H_

#define	REG_DMA_PAUSE	(0x00)
#define	DMA_FRAG_WAIT	(0x04)
#define	REQ_PEND0_EN	(0x08)
#define	REQ_PEND1_EN	(0x0C)
#define	INT_RAW_STAT	(0x10)
#define	INT_MASK_STAT	(0x14)
#define	DMA_REQ_STAT	(0x18)
#define	DMA_EN_STAT		(0x1C)
#define	DMA_DEBUG_STAT	(0x20)
#define	DMA_ARB_SEL_STAT	(0x24)
#define	START_CH_CFG_GRP1	(0x28)
#define	START_CH_CFG_GRP2	(0x2C)
#define	DMA_CH_ARPROT	(0x30)
#define	DMA_CH_AWPROT	(0x34)
#define	DMA_CH_PROT_FLAG	(0x38)
#define	DMA_GLB_PROT		(0x3C)
#define	DMA_PEND0_PROT		(0x40)
#define	DMA_PEND1_PROT		(0x44)
#define	DMA_REQID0_PROT		(0x48)
#define	DMA_REQID1_PROT		(0x4C)
#define SYNC_SEC_N_NORMAL	(0x50)
#define CNT_CH_SEL			(0x54)
#define TOTAL_TRSF_BYTE_CNT1	(0x58)
#define TOTAL_TRSF_BYTE_CNT2	(0x5C)


#define FULL_CH_PAUSE(x)	(0x1000 + 0x40 * (x))
#define FULL_CH_REQ(x)		(0x1004 + 0x40 * (x))
#define FULL_CH_CFG(x)		(0x1008 + 0x40 * (x))
#define FULL_CH_INT(x)		(0x100C + 0x40 * (x))
#define FULL_CH_SRC_ADDR(x)		(0x1010 + 0x40 * (x))
#define FULL_CH_DST_ADDR(x)		(0x1014 + 0x40 * (x))
#define FULL_CH_FRAG_LEN(x)		(0x1018 + 0x40 * (x))
#define FULL_CH_BLK_LEN(x)		(0x101C + 0x40 * (x))
#define FULL_CH_TRSC_LEN(x)		(0x1020 + 0x40 * (x))
#define FULL_CH_TRSF_STEP(x)		(0x1024 + 0x40 * (x))
#define FULL_CH_WRAP_PTR(x)		(0x1028 + 0x40 * (x))
#define FULL_CH_WRAP_TO(x)		(0x102C + 0x40 * (x))
#define FULL_CH_LLIST_PTR(x)		(0x1030 + 0x40 * (x))
#define FULL_CH_FRAG_STEP(x)		(0x1034 + 0x40 * (x))
#define FULL_CH_SRC_BLK_STEP(x)		(0x1038 + 0x40 * (x))
#define FULL_CH_DST_BLK_STEP(x)		(0x103C + 0x40 * (x))

#define REQ1_CID(x)			(0x2000 + ((x) << 2))


//DMA PAUSE register
#define DMA_PAUSE		BIT(0)
#define DMA_PAUSE_STAT		BIT(2)
#define GLB_REG_CLK_EN		BIT(16)
#define CHNL_REG_CLK_EN		BIT(17)
#define REQ_CID_CLK_EN		BIT(18)
#define INT_REG_CLK_EN		BIT(19)
#define AXI_MST_CLK_EN		BIT(20)
#define AUDIO_CNT_CLK_EN		BIT(21)
//DMA FRAG_WAIT register
#define DMA_FRAG_WAIT_SHIFT		0
#define DMA_FRAG_WAIT_MASK		0xffff
#define AXI_DST_OT_NUM_SHIFT	16
#define AXI_DST_OT_NUM_MASK		0xf
#define AXI_SRC_OT_NUM_SHIFT	16
#define AXI_SRC_OT_NUM_MASK		0xf

//FULL_CH PAUSE register
#define CH_PAUSE			BIT(0)
#define CH_PAUSE_STAT		BIT(2)
//FULL_CH_REQ register
#define CH_REQ				BIT(0)
//FULL_CH_CFG register
#define CH_EN				BIT(0)
#define LLIST_EN			BIT(4)
#define LLIST_CFG_VALID		BIT(8)
#define CH_PRIORITY_SHIFT	12
#define CH_PRIORITY_MASK	3
#define ERR_TYPE_STAT_SHIFT	20
#define ERR_TYPE_STAT_MASK	0xF
#define DONT_WAIT_BDONE		BIT(24)

#define PRIORITY_LOWEST		0
#define PRIORITY1			1
#define PRIORITY2			2
#define PRIORITY_HIGHEST	3

//FULL_CH_INIT register
#define CH_FRAGMENT_INT_EN			BIT(0)
#define CH_BLK_INT_EN				BIT(1)
#define CH_TRSC_INT_EN				BIT(2)
#define CH_LLIST_INT_EN				BIT(3)
#define CH_CFG_ERR_INT_EN			BIT(4)
#define CH_FRAGMENT_INT_RAW_STAT 	BIT(8)
#define CH_BLK_INT_RAW_STAT			BIT(9)
#define CH_TRSC_INT_RAW_STAT		BIT(10)
#define CH_LLIST_INT_RAW_STAT		BIT(11)
#define CH_CFG_ERR_INT_RAW_STAT		BIT(12)
#define CH_FRAGMENT_INT_MASK_STAT	BIT(16)
#define CH_BLK_INT_MASK_STAT		BIT(17)
#define CH_TRSC_INT_MASK_STAT		BIT(18)
#define CH_LLIST_INT_MASK_STAT		BIT(19)
#define CH_CFG_ERR_INT_MASK_STAT	BIT(20)
#define CH_FRAGMENT_INT_CLR			BIT(24)
#define CH_BLK_INT_CLR				BIT(25)
#define CH_TRSC_INT_CLR				BIT(26)
#define CH_LLIST_INT_CLR			BIT(27)
#define CH_CFG_ERR_INT_CLR			BIT(28)

#define FRAG_LEN_SHIFT				0
#define FRAG_LEN_MASK				0x1FFFF
#define BLK_LEN_REC_H_SHIFT			16
#define BLK_LEN_REC_H_MASK			0x3
#define LLIST_END					BIT(19)
#define ADDR_FIX_EN					BIT(20)
#define ADDR_FIX_SEL				BIT(21)
#define ADDR_WRAP_EN				BIT(22)
#define ADDR_WRAP_SEL				BIT(23)
#define REQ_MODE_SHIFT				24
#define REQ_MODE_MASK				3
#define SWT_MODE_SHIFT				26
#define SWT_MODE_MASK				3
#define DST_SIZE_SHIFT				28
#define DST_SIZE_MASK				3
#define SRC_SIZE_SHIFT				30
#define SRC_SIZE_MASK				3U

#define REQ_FRAGMENT				0
#define REQ_BLOCK					1
#define REQ_TRANSACTION				2
#define REQ_LINK_LIST				3

#define AHB_SIZE_BYTE				0
#define AHB_SIZE_HWORD				1
#define AHB_SIZE_WORD				2
#define AHB_SIZE_DWORD				3

#define SWT_ABCD_ABCD				0
#define SWT_ABCD_DCBA				1
#define SWT_ABCD_BADC				2
#define SWT_ABCD_DCAB				3

//FULL_CH_TRSC_LEN register
#define BLK_LEN_SHIFT				0
#define BLK_LEN_MASK				0x1FFFF
#define BLK_LEN_REC_L_SHIFT			17
#define BLK_LEN_REC_L_MASK			0x7FFF

//FULL_CH_TRSF_STEP register
#define TRSC_LEN_SHIFT				0
#define TRSC_LEN_MASK				0xFFFFFFF
#define LLIST_NODE_INT_TYPE_SHIFT	28
#define LLIST_NODE_INT_TYPE_MASK	0x7
#define LLIST_NODE_INT_EN			BIT(31)

//FULL_CH_WRAP_PTR register
#define SRC_TRSF_STEP_SHIFT			0
#define SRC_TRSF_STEP_MASK			0xFFFF
#define DST_TRSF_STEP_SHIFT			16
#define DST_TRSF_STEP_MASK			0xFFFFU

//FULL_CH_WRAP_TO register
#define WRAP_PTR_SHIFT				0
#define WRAP_PTR_MASK				0xFFFFFFF
#define CH_SRC_ADDR_SHIFT			28
#define CH_SRC_ADDR_MASK			0xF

//FULL_CH_LLIST_PTR register
#define WRAP_TO_SHIFT				0
#define WRAP_TO_MASK				0xFFFFFFF
#define CH_DST_ADDR_SHIFT			28
#define CH_DST_ADDR_MASK			0xF

//FULL_CH_FRAG_STEP register
#define DST_FRAG_SETP_SHIFT			16
#define DST_FRAG_SETP_MASK			0xFFFF

//FULL_CH_SRC_BLK_STEP register
#define CH_SRC_BLK_STEP_SHIFT		0
#define CH_SRC_BLK_STEP_MASK		0xFFFFFFF
#define LLIST_PTR_SHIFT				28
#define LLIST_PTR_MASK				0xF

//FULL_CH_DST_BLK_STEP register
#define CH_DST_BLK_STEP_SHIFT		0
#define CH_DST_BLK_STEP_MASK		0xFFFFFFF

#define DMA_CHN00					0
#define DMA_CHN01					1
#define DMA_CHN02					2
#define DMA_CHN03					3
#define DMA_CHN04					4
#define DMA_CHN05					5
#define DMA_CHN06					6
#define DMA_CHN07					7
#define DMA_CHN08					8
#define DMA_CHN09					9
#define DMA_CHN10					10
#define DMA_CHN11 				    11
#define DMA_CHN12					12
#define DMA_CHN13					13
#define DMA_CHN14					14
#define DMA_CHN15					15
#define DMA_CHN16					16
#define DMA_CHN17					17
#define DMA_CHN18					18
#define DMA_CHN19					19
#define DMA_CHN20					20
#define DMA_CHN21 				    21
#define DMA_CHN22					22
#define DMA_CHN23					23
#define DMA_CHN24					24
#define DMA_CHN25					25
#define DMA_CHN26					26
#define DMA_CHN27					27
#define DMA_CHN28					28
#define DMA_CHN29					29
#define DMA_CHN30					30
#define DMA_CHN31					31
#define	AES_RX_RID				28
#define	AES_TX_RID				29
#define HASH_RX_RID				36

#define dma_read(reg)				__raw_readl((unsigned long)(REG_DMA_BASE + (reg)))
#define dma_write(val,reg)			__raw_writel(val, (unsigned long)(REG_DMA_BASE + (reg)))

#define DATA_SWITCH_MODE		(3<<26)

#define DMA_WAIT_READY_TIME_OUT		1000
#define MAX_NODE					8
#define SRCDST_AHB_SIZE_BYTE		0x00000000
#define SRCDST_AHB_SIZE_HALFWORD	0x50000000
#define SRCDST_AHB_SIZE_WORD		0xa0000000
#define SRCDST_AHB_SIZE_DWORD		0xf0000000

#define TRANS_STEP_BYTE			0x00010001
#define TRANS_STEP_HALFWORD		0x00020002
#define TRANS_STEP_WORD			0x00040004
#define TRANS_STEP_DWORD		0x00080008

#define TRANS_MODE_FRAG			0X00000000
#define TRANS_MODE_BLOCK		0X01000000
#define TRANS_MODE_TRANS		0X02000000
#define TRANS_MODE_LLIST		0X03000000

#define WORD_MODE_ABCD				(0<<26)//ABCD => ABCD
#define WORD_MODE_DCBA				(1<<26)//ABCD => DCBA
#define WORD_MODE_BADC				(2<<26)//ABCD => BADC
#define WORD_MODE_DCAB				(3<<26)//ABCD => DCAB

#define SFC_BUFFER   REG_SPIC_BASE + 0x2000
#define PAGESIZE     0x100

typedef enum _Direction {
	DMA_MEM_TO_MEM,
	DMA_MEM_TO_DEV,
	DMA_DEV_TO_MEM,
} Direction;
typedef struct _dma_link_list_dev {
	int ch_id;
	int dma_req_cid;
	u32 ahb_size;
	u32 word_mode;
	Direction direction;
	u32 *src_addr_arr;
	u32 *dst_addr_arr;
	u32 *trsc_len_arr;
	u32 node_cnt;
} dma_link_list_dev;

typedef struct _NodeCfg {
	u32 SRC_ADDR;
	u32 DST_ADDR;
	u32 FRAG_LEN;
	u32 BLK_LEN;
	u32 TRSC_LEN;
	u32 TRSF_STEP;
	u32 WRAP_PTR;
	u32 WRAP_TO;
	u32 LLIST_PTR;
	u32 FRAG_STEP;
	u32 SRC_BLK_STEP;
	u32 DST_BLK_STEP;
} NodeCfg;

void dma_reset(void);
void dma_init(u32 chn, u32 id);
void enable_channel(u32 chn);
void dma_chn_src(u32 chn,u32 src);
void dma_chn_dst(u32 chn,u32 dst);
void dma_trsc_len(u32 chn,u32 len);
void dma_clr_enable(u32 chn);
void dma_request(u32 chn);
void dma_wait_complete(u32 chn);
u32 dma_llist_wait_complete(u32 chn, u32 timeout);
void enable_channel(u32 chn);
void init_chn_reg(u32 chn);
void dma_int_disable(u32 chn);

//uart rx dma
void dma_set_trans_len(u32 chn,u32 val);
u32  dma_get_trans_len(u32 chn);
void dma_set_src(u32 chn,u32 val);
void dma_set_dest(u32 chn,u32 val);
void dma_frag_len_reg_set(u32 chn,u32 val);
void dma_blk_len_reg_set(u32 chn,u32 val);
void uart_request_cid(u32 chn,u32 uart_addr);
void uart_int_enable(u32 chn);
void dma_set_trsf_step(u32 chn,u32 step);
void nor_dma_node_fill(u32 ch_id, u32 cnt, u32 *src, u32* dst, u32* len);
void nor_dma_node_fill_new(u32 ch_id, u32 cnt, u32 *src, u32* dst, u32* len, NodeCfg *node_out);//add NodeCfg *node_out
void dma_int_clear(u32 chn);


#endif
