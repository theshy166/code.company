/*
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef _MC_AES_H_
#define _MC_AES_H_

#define AES_ENCRYPT_BASE			0x0c600000

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

#define AES_KEY(x)					(0x10 + ((x) * 0x04))
#define AES_IV(x)					(0x30 + ((x) * 0x04))
#define AES_DMA_SRC					0x40
#define AES_DMA_DST					0x48

#define AES_DMA_TRAS_SIZE			0x50
#define AES_DMA_TRAS_SIZE_MASK		(0x1FFFFF)

#define AES_DMA_CTRL				0x54
#define AES_DMA_EN					(1 << 0)
#define AES_DMA_STOP				(1 << 2)
#define AES_KEY_CIPHER				(1 << 4)

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
#define EXT_DMA_EN				0x90
#define AES_EXT_DMA_CFG			0x94
#define AES_RX_FIFO				0x98
#define AES_TX_FIFO				0x9C

#define LAST_IV(x)					(0x80 + ((x) << 2))
#define AES_KEY_CIPHER_DST			0x100

#define MC_AES_PRIORITY	300

#define MC_AES_BUFFER_ORDER	2
#define MC_AES_BUFFER_SIZE	(PAGE_SIZE << MC_AES_BUFFER_ORDER)

#define CFB8_BLOCK_SIZE		1
#define SIZE_IN_WORDS(x)	((x) >> 2)

/* AES flags */
#define AES_FLAGS_DECRYPT_KEY		BIT(1)
#define AES_FLAGS_CIPHER_KEY		BIT(2)
#define AES_FLAGS_BUSY		BIT(3)


#define AES_FLAGS_PERSISTENT	AES_FLAGS_BUSY

#define MC_AES_QUEUE_LENGTH	50

#define MC_AES_DMA_THRESHOLD		0xFF

#define MC_AES_IF_THRESHOLD		0x40
#define MC_AES_OF_THRESHOLD		0x40

#define AES_RX_RID			8
#define AES_TX_RID			9
#define AES_RX_DMA_CH			2
#define AES_TX_DMA_CH			3
#define ALIGN_SIZE			16
#define IF_THOLD_VAL			0x10
#define OF_THOLD_VAL			0x10

struct mc_aes_platdata {
	void __iomem *base;
	unsigned long		phys_base;
	u32					decrypt;
	unsigned long		emode;
	unsigned long 		mode;
	u32 	if_thold;
	u32		of_thold;
	u32 	src;
	u32		dest;
	u32		tran_size;
	int		keylen;
	int		ivlen;
	u32		*aes_key;
	u32		*aes_iv;
};

///ENCRYPT_CONTROL
#define CONTROL_DECRYPT				BIT(0)
#define CONTROL_MODE_MASK			GENMASK(3,1)
#define CONTROL_EMODE_MASK			GENMASK(6,4)
#define CONTROL_FIRST				BIT(7)
#define CONTROL_PCHK				BIT(8)

//FIFO STATUS
#define OF_ENTITY_MASK				GENMASK(31,24)
#define IF_ENTITY_MASK				GENMASK(23,16)
#define OF_FULL						BIT(3)
#define OF_EMPTY					BIT(2)
#define IF_FULL						BIT(1)
#define IF_EMPTY					BIT(0)
//PERR STATUS
#define PERR						BIT(31)
#define PERRB_MASK					GENMASK(23,0)
//DMA_TRAS_SIZE
#define TRAN_SIZE_MASK				GENMASK(11,0)
//DMA_CTRL
#define DMA_STOP					BIT(2)
#define DMA_EN						BIT(0)

//FIFO_THOLD
#define OF_THOLD_MASK				GENMASK(31,16)
#define IF_THOLD_MASK				GENMASK(15,0)
//INTR_ENABLE
#define STOP_INTR_EN				BIT(2)
#define ERR_INTR_EN					BIT(1)
#define DONE_INTR_EN				BIT(0)
//INTR_SRC
#define STOP_INTR					BIT(2)
#define ERR_INTR					BIT(1)
#define DONE_INTR					BIT(0)
//MASKED_INTR_STATUS
#define STOP_INTR_MSK				BIT(2)
#define ERR_INTR_MSK				BIT(1)
#define DONE_INTR_MSK				BIT(0)
//INTR_CLR
#define CLR_STOP_INTR				BIT(2)
#define CLR_ERR_INTR				BIT(1)
#define CLR_DONE_INTR				BIT(0)

#define EMODE(x)					((x) << 4)
#define EMOD_ECB					0
#define EMOD_CBC					1
#define EMOD_CTR					2
#define EMOD_CFB					4
#define EMOD_OFB					5

#define METHOD(x)					((x) << 1)
#define METHOD_DES					0
#define METHOD_TDES					1
#define METHOD_AES_K128				4
#define METHOD_AES_K192				5
#define METHOD_AES_K256				6

#define AES_BLK_MASK				0xFFFFFFF0
#define DES_BLK_MASK				0xFFFFFFF8

#define IF_THRESHOLD(x)				((x) << 0)
#define OF_THRESHOLD(x)				((x) << 16)
#define TRAN_SIZE(x)				((x) << 0)

enum {
	AES_128_CBC_ENCODE,
	AES_192_CBC_ENCODE,
	AES_256_CBC_ENCODE,
	AES_128_ECB_ENCODE,
	AES_192_ECB_ENCODE,
	AES_256_ECB_ENCODE,
	AES_128_CTR_ENCODE,
	AES_192_CTR_ENCODE,
	AES_256_CTR_ENCODE,
	AES_128_OFB_ENCODE,
	AES_192_OFB_ENCODE,
	AES_256_OFB_ENCODE,
	AES_128_CFB_ENCODE,
	AES_192_CFB_ENCODE,
	AES_256_CFB_ENCODE,

	DES_ECB_ENCODE,
	DES_CBC_ENCODE,
	DES_CFB_ENCODE,
	DES_OFB_ENCODE,
	TDES_ECB_ENCODE,
	TDES_CBC_ENCODE,
	TDES_CFB_ENCODE,
	TDES_OFB_ENCODE,

	AES_128_CBC_DECODE,
	AES_192_CBC_DECODE,
	AES_256_CBC_DECODE,
	AES_128_ECB_DECODE,
	AES_192_ECB_DECODE,
	AES_256_ECB_DECODE,
	AES_128_CTR_DECODE,
	AES_192_CTR_DECODE,
	AES_256_CTR_DECODE,
	AES_128_OFB_DECODE,
	AES_192_OFB_DECODE,
	AES_256_OFB_DECODE,
	AES_128_CFB_DECODE,
	AES_192_CFB_DECODE,
	AES_256_CFB_DECODE,

	DES_ECB_DECODE,
	DES_CBC_DECODE,
	DES_CFB_DECODE,
	DES_OFB_DECODE,
	TDES_ECB_DECODE,
	TDES_CBC_DECODE,
	TDES_CFB_DECODE,
	TDES_OFB_DECODE,

};

/*in, out, len*/
typedef int (*HW_aes)(const unsigned char*, unsigned char*, int);

int mc_hw_aes(const unsigned char *indata, unsigned char *outdata, int datalen);
int mc_aes_call(u32 mode, u32 *key, u32 *iv,u32 *src, u32 *dst, u32 len);

#endif


