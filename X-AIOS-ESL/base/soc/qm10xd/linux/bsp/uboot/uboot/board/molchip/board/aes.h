#ifndef _AES_H_
#define _AES_H_

#define ENCRYPT_CONTROL				0x0
#define FIFO_STATUS				0x8
#define PERR_STATUS				0xc
#define KEY0					0x10
#define KEY1					0x14
#define KEY2					0x18
#define KEY3					0x1c
#define KEY4					0x20
#define KEY5					0x24
#define KEY6					0x28
#define KEY7					0x2c
#define IV0					0x30
#define IV1					0x34
#define IV2					0x38
#define IV3					0x3c

#define DMA_SRC					0x40
#define DMA_DES					0x48
#define DMA_TRAS_SIZE				0x50
#define DMA_CTRL				0x54
#define FIFO_THOLD				0x58
#define INTR_ENABLE				0x5c
#define INTR_SRC				0x60
#define MASKED_INTR_STATUS			0x64
#define INTR_CLR				0x68

#define REVSION					0x70
#define FEATURE					0x74

#define LAST_IV0				0x80
#define LAST_IV1				0x84
#define LAST_IV2				0x88
#define LAST_IV3				0x8c
#define EXT_DMA_CFG				0x90
#define EXT_DMA_CFG1				0x94


#define AES_RX_FIFO				0x0c600098
#define AES_TX_FIFO				0x0c60009C

//ENCRYPT_CONTROL
#define CONTROL_DECRYPT				BIT(0)
#define CONTROL_MODE_MASK			GENMASK(3, 1)
#define CONTROL_EMODE_MASK			GENMASK(6, 4)
#define CONTROL_FIRST				BIT(7)
#define CONTROL_PCHK				BIT(8)

#define DMA_TXDATA_BYTE_LEN				GENMASK(31, 16)
#define DMA_RXDATA_BYTE_LEN				GENMASK(15, 0)

//FIFO STATUS
#define OF_ENTITY_MASK				GENMASK(31, 24)
#define IF_ENTITY_MASK				GENMASK(23, 16)
#define OF_FULL					BIT(3)
#define OF_EMPTY				BIT(2)
#define IF_FULL					BIT(1)
#define IF_EMPTY				BIT(0)

//PERR STATUS
#define PERR					BIT(31)
#define PERRB_MASK				GENMASK(23, 0)

//DMA_TRAS_SIZE
#define TRAN_SIZE_MASK				GENMASK(24, 0)

//DMA_CTRL
#define DMA_STOP				BIT(2)
#define DMA_EN					BIT(0)

//FIFO_THOLD
#define OF_THOLD_MASK				GENMASK(15, 8)
#define IF_THOLD_MASK				GENMASK(7, 0)

//INTR_ENABLE
#define STOP_INTR_EN				BIT(2)
#define ERR_INTR_EN				BIT(1)
#define DONE_INTR_EN				BIT(0)

//INTR_SRC
#define STOP_INTR				BIT(2)
#define ERR_INTR				BIT(1)
#define DONE_INTR				BIT(0)

//MASKED_INTR_STATUS
#define STOP_INTR_MSK				BIT(2)
#define ERR_INTR_MSK				BIT(1)
#define DONE_INTR_MSK				BIT(0)

//INTR_CLR
#define CLR_STOP_INTR				BIT(2)
#define CLR_ERR_INTR				BIT(1)
#define CLR_DONE_INTR				BIT(0)

#define AES_REG_KEY(x)				(KEY0 + ((x) * 0x04))
#define AES_REG_IV(x)				(IV0 + ((x) * 0x04))
#define EMODE(x)				((x) << 4)
#define EMOD_ECB				0
#define EMOD_CBC				1
#define EMOD_CTR				2
#define EMOD_CFB				4
#define EMOD_OFB				5

#define METHOD(x)				((x) << 1)
#define METHOD_DES				0
#define METHOD_TDES				1
#define METHOD_AES_K128				4
#define METHOD_AES_K192				5
#define METHOD_AES_K256				6

#define IF_THRESHOLD(x)				((x) << 0)
#define OF_THRESHOLD(x)				((x) << 8)
#define TRAN_SIZE(x)				((x) << 0)
#define MAX_TRAN_SIZE				0x1FFFFFF
#define FIFO_THRESHOLD				0xFF

#define AES_WAIT_READY				5000

#define aes_read(offset)			\
		__raw_readl(REG_AES_BASE + offset)

#define aes_write(offset, value)		\
		__raw_writel(value, REG_AES_BASE + offset)

bool aes_cbc_init(u32 *iv);

bool aes_cbc_decode(unsigned int src, unsigned int dest,
			u32 size);

void aes_start(unsigned int len);

void aes_enable(void);
void aes_write_iv(u32 *iv, u32 len);
void aes_config_mode(u32 mode, u32 emode);
void aes_clr_intr(void);
void aes_decrypt(bool decrypt);
u32 aes_intr_src(void);
void aes_enable_1st_iv(bool en);
void aes_threshold(void);
bool aes_done(void);
bool aes_init(void);
void aes_handle_start(u32 src ,u32 dst ,u32 len);


#endif
