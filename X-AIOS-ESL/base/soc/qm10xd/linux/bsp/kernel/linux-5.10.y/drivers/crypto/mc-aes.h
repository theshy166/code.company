#ifndef __MC_AES_REGS_H__
#define __MC_AES_REGS_H__

#define AES_ENCRYPT_CTRL			0x00
#define AES_CTRL_DECRYPT			(1 << 0)
#define AES_CTRL_ENCRYPT			(0 << 0)
#define AES_CTRL_METHOD_MASK		(0x7 << 1)
#define AES_CTRL_METHOD_OFFSET		1
#define AES_METHOD_DES				(0 << AES_CTRL_METHOD_OFFSET)
#define AES_METHOD_TDES				(1 << AES_CTRL_METHOD_OFFSET)
#define AES_METHOD_AES_K128			(4 << AES_CTRL_METHOD_OFFSET)
#define AES_METHOD_AES_K192			(5 << AES_CTRL_METHOD_OFFSET)
#define AES_METHOD_AES_K256			(6 << AES_CTRL_METHOD_OFFSET)

#define AES_CTRL_EMODE_MASK			(0x07 << 4)
#define AES_CTRL_EMODE_OFFSET		4
#define AES_EMODE_ECB				(0 << AES_CTRL_EMODE_OFFSET)
#define AES_EMODE_CBC				(1 << AES_CTRL_EMODE_OFFSET)
#define AES_EMODE_CTR				(2 << AES_CTRL_EMODE_OFFSET)
#define AES_EMODE_CFB				(4 << AES_CTRL_EMODE_OFFSET)
#define AES_EMODE_OFB				(5 << AES_CTRL_EMODE_OFFSET)

#define AES_CTRL_FIRST				(1 << 7)
#define AES_CTRL_PCHK				(1 << 8)

#define	AES_FIFO_STATUS				0x08
#define AES_STS_IF_EMPTY			(0 << 0)
#define AES_STS_IF_FULL				(1 << 0)
#define AES_STS_OF_EMPTY			(1 << 1)
#define	AES_STS_OF_FULL				(1 << 3)
#define AES_STS_IF_ENTITY_MASK		(0xFF << 16)
#define AES_STS_IF_ENTITY_OFFSET	16
#define AES_STS_OF_ENTITY_MASK		(0xFF << 24)
#define AES_STS_OF_ENTITY_OFFSET	24

#define AES_PEER_STATUS				0x0c
#define AES_PERR					(1 << 31)
#define AES_PERR_B(x)				(1 << (x))

#define AES_KEY(x)					(0x10 + ((x) << 2))
#define AES_IV(x)					(0x30 + ((x) << 2))
#define AES_DMA_SRC					0x40
#define AES_DMA_SRC_HIGH			0x44
#define AES_DMA_DST					0x48
#define AES_DMA_DST_HIGH			0x4c

#define AES_DMA_TRAS_SIZE			0x50
#define AES_DMA_TRAS_SIZE_MASK		(0x1FFFFF)

#define AES_DMA_CTRL				0x54
#define AES_DMA_EN					(1 << 0)
#define AES_DMA_STOP				(1 << 2)

#define AES_FIFO_THRESHOLD			0x58
#define AES_OUTFIFO_THOLD_OFFSET	(8)
#define AES_OUTFIFO_THOLD_MASK		(0xFF << 8)
#define AES_INFIFO_THOLD_OFFSET		(0)
#define AES_INFIFO_THOLD_MASK		(0xFF)

#define AES_INTR_ENABLE				0x5c
#define AES_INTR_ENABLE_DONE		(1 << 0)
#define AES_INTR_ENABLE_ERR			(1 << 1)
#define AES_INTR_ENABLE_STOP		(1 << 2)

#define AES_INTR_SRC				0x60
#define AES_INTR_SRC_DONE			(1 << 0)
#define AES_INTR_SRC_ERR			(1 << 1)
#define AES_INTR_SRC_STOP			(1 << 2)

#define AES_INTR_MASKED				0x64
#define AES_INTR_MASKED_DONE		(1 << 0)
#define AES_INTR_MASKED_ERR			(1 << 1)
#define AES_INTR_MASKED_STOP		(1 << 2)

#define AES_INTR_CLEAR				0x68
#define AES_INTR_CLEAR_DONE			(1 << 0)
#define AES_INTR_CLEAR_ERR			(1 << 1)
#define AES_INTR_CLEAR_STOP			(1 << 2)

#define AES_HW_VERSION  			0x70
#define AES_HW_FEATURE				0x74

#define LAST_IV(x)					(0x80 + ((x) << 2))
#define AES_EXI_DMA_EN				0x90
#define AES_EXI_DMA_REQ				0x94
#define AES_REQ_BYTE_NUM_PERCENT	0x10


#define AES_RX_FIFO					0x98
#define AES_TX_FIFO					0x9C

#define MC_AES_PRIORITY	300

#define MC_AES_BUFFER_ORDER	2
#define MC_AES_BUFFER_SIZE	(PAGE_SIZE << MC_AES_BUFFER_ORDER)

#define CFB8_BLOCK_SIZE		1
#define SIZE_IN_WORDS(x)	((x) >> 2)

/* AES flags */
#define AES_FLAGS_ENCRYPT	AES_CTRL_DECRYPT


#define AES_FLAGS_BUSY		BIT(3)


#define AES_FLAGS_PERSISTENT	AES_FLAGS_BUSY

#define MC_AES_QUEUE_LENGTH	50

#define MC_AES_DMA_THRESHOLD		0xFF


struct mc_aes_dev;


typedef int (*mc_aes_fn_t)(struct mc_aes_dev *);


struct mc_aes_base_ctx {
	struct mc_aes_dev	*dd;
	mc_aes_fn_t		start;
	int			keylen;
	u32			key[AES_KEYSIZE_256 / sizeof(u32)];
	u16			block_size;
	bool			is_aead;
};

struct mc_aes_ctx {
	struct mc_aes_base_ctx	base;
};

struct mc_aes_reqctx {
	unsigned long		mode;
};

struct mc_aes_dma {
	struct dma_chan		*chan;
	struct scatterlist	*sg;
	int			nents;
	unsigned int		remainder;
	unsigned int		sg_len;
};

struct mc_aes_dev {
	struct list_head	list;
	unsigned long		phys_base;
	void __iomem		*io_base;
	struct regmap *cpu_sysy_base;

	struct crypto_async_request	*areq;
	struct mc_aes_base_ctx	*ctx;
	int				 aes_clk_ctrl_reg;
	int 			aes_clk_ctrl_bit;
	int				aes_reset_reg;
	int				aes_sw_reset_bit;
	mc_aes_fn_t		resume;

	struct device		*dev;
	struct clk		*iclk;
	int			irq;
	int tx_dma_id;
	int rx_dma_id;

	unsigned long		flags;
	unsigned int 		cipher_key_flags;
	unsigned int 		cipher_key_buf_phys;
	spinlock_t		lock;
	struct crypto_queue	queue;

	struct tasklet_struct	done_task;
	struct tasklet_struct	queue_task;

	size_t			total;
	size_t			datalen;
	u32			*data;

	struct mc_aes_dma	src;
	struct mc_aes_dma	dst;

	size_t			buflen;
	void			*buf;
	struct scatterlist	aligned_sg;
	struct scatterlist	*real_dst;

	u32	  efuse_key;
};

struct mc_aes_drv {
	struct list_head	dev_list;
	spinlock_t		lock;
};

#endif /* __MC_AES_REGS_H__ */
