#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <display_options.h>
#include <dm.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <misc.h>
#include "../../board/molchip/board/glb_reg.h"

#include "mc_aes.h"
#include "mc_aes_dma.h"


//#define AES_EFUSE_TEST
struct udevice *mc_aes_dev;

u8 plain_text[48] __attribute__((aligned(16)))= {0};
u8 cipher_text[48] __attribute__((aligned(16)))= {0};
u32 	iv[] = {0x00010203,0x04050607,0x08090a0b,0x0c0d0e0f};

u32 	key_128[] = {0x2b7e1516,0x28aed2a6,0xabf71588,0x09cf4f3c,};
u32		key_192[] = {0x8e73b0f7,0xda0e6452,0xc810f32b,0x809079e5,
					 0x62f8ead2,0x522c6b7b};
u32		key_256[] = {0x603deb10,0x15ca71be,0x2b73aef0,0x857d7781,
					 0x1f352c07,0x3b6108d7,0x2d9810a3,0x0914dff4};
u32 des_key[]= {0x01234567,0x89abcdef };
u32 des_iv[]= {0x12345678,0x90abcdef };

u32 triple_des_key[]={ 0x01234567,0x89abcdef,
						0x23456789,0xabcdef01,
						0x456789ab,0xcdef0123,};
u32 triple_des_iv[] = { 0x12345678,0x90abcdef};

static inline u32 aes_read(struct mc_aes_platdata *dd, u32 offset)
{
	u32 value = readl(dd->base + offset);
	return value;
}

static inline void aes_write(struct mc_aes_platdata *dd,
					u32 offset, u32 value)
{
	writel(value, dd->base + offset);
}

void mc_aes_enable(void)
{
  u32 val,i;

  val = readl(AP_MTX_CLK_CTRL0);
  val |= CKG_AHB_AES_EN;
  writel(val, AP_MTX_CLK_CTRL0);
  
  val = readl(CPU_SYS_RST1);
  val |= (AES_SOFT_RST);
  writel(val, CPU_SYS_RST1);
  
  for(i=0;i<0x1000;i++);
  
  val &= ~(AES_SOFT_RST);
  writel(val, CPU_SYS_RST1);
}

static inline void aes_write_mask(struct mc_aes_platdata *dd, u32 offset,
					u32 value, u32 mask)
{
	u32 val;

	val = aes_read(dd, offset);
	val &= ~mask;
	val |= value;
	aes_write(dd, offset, val);
}

static void specify_1st_iv(struct mc_aes_platdata *dd)
{
	u32 val;
	val = aes_read(dd, AES_ENCRYPT_CTRL);
	val |= CONTROL_FIRST;
	aes_write(dd, AES_ENCRYPT_CTRL, val);
}

static int aes_write_ctrl(struct mc_aes_platdata *dd)
{
	unsigned int key32;
	int i;

	key32 = dd->keylen;

	if(dd->aes_key != NULL){
		for (i = 0; i < key32; i++) {
			aes_write(dd, AES_KEY(i),
				dd->aes_key[i]);
		}
	}
	
	for(i = 0; i < dd->ivlen; i++)
		aes_write(dd, AES_IV(i),
			dd->aes_iv[i]);
	
	aes_write_mask(dd, AES_ENCRYPT_CTRL,
		EMODE(dd->emode), CONTROL_EMODE_MASK);
	
	aes_write_mask(dd, AES_ENCRYPT_CTRL,
		METHOD(dd->mode), CONTROL_MODE_MASK);
	
	specify_1st_iv(dd);
	return 0;
}

static int aes_dma_init(struct mc_aes_platdata *dd)
{
	aes_write_mask(dd, AES_DMA_TRAS_SIZE,
		TRAN_SIZE(dd->tran_size), TRAN_SIZE_MASK);

	aes_write_mask(dd, AES_EXT_DMA_CFG,
		IF_THRESHOLD(dd->if_thold), IF_THOLD_MASK);
	
	aes_write_mask(dd, AES_EXT_DMA_CFG,
		OF_THRESHOLD(dd->of_thold), OF_THOLD_MASK);		

	//AES DMA RX_ID IS 8
	aes_dma_trac(dd->src, AES_ENCRYPT_BASE + AES_RX_FIFO, AES_RX_DMA_CH, AES_RX_RID,dd->tran_size, 1);
	//AES DMA TX_ID IS 9
	aes_dma_trac(AES_ENCRYPT_BASE + AES_TX_FIFO,dd->dest, AES_TX_DMA_CH, AES_TX_RID,dd->tran_size, 0);
	aes_enable_channel(AES_RX_DMA_CH);
	aes_enable_channel(AES_TX_DMA_CH);

	return 0;
}

static int aes_decrypt(struct mc_aes_platdata *dd)
{
	u32 val;
	
	val = aes_read(dd, AES_ENCRYPT_CTRL);
	
	if(dd->decrypt)
		val |= CONTROL_DECRYPT;
	else
		val &= ~CONTROL_DECRYPT;
	aes_write(dd, AES_ENCRYPT_CTRL, val);

	return 0;
}

static int	aes_dma_en(struct mc_aes_platdata *dd, bool en)
{
	u32 val;
	val = aes_read(dd, AES_DMA_CTRL);
	if(en)	{
		val &= DMA_STOP;
		val |= DMA_EN;
	} else	{
		val &= DMA_EN;
		val |= DMA_STOP;
	}
	aes_write(dd, AES_DMA_CTRL, val);

	val = aes_read(dd, EXT_DMA_EN);
	val |= DMA_EN;
	aes_write(dd, EXT_DMA_EN, val);
	
	return 0;
}

static u32 aes_intr_src(struct mc_aes_platdata *dd)
{
	u32 val;
	val = aes_read(dd,AES_INTR_SRC);
	return val;
}

static int aes_clr_intr(struct mc_aes_platdata *dd)
{
	u32 val;
	val = CLR_STOP_INTR | CLR_ERR_INTR | CLR_DONE_INTR;
	aes_write(dd,AES_INTR_CLEAR,val);
	return 0;
}

static int aes_start(struct mc_aes_platdata *dd)
{
	u32 val;
	aes_dma_en(dd,true);
	do {
		val =  aes_intr_src(dd);
		if(val & DONE_INTR)	{
			aes_clr_intr(dd);
			break;
		}
	} while(1);
	return 0;
}

static void aes_encode(struct mc_aes_platdata *dd,u32 emode,u32 mode, u32*key, u32 *iv)
{
	dd->mode = mode;
	dd->emode = emode;
	dd->decrypt = false;

	dd->aes_iv = iv;
	dd->ivlen = 4;

	dd->aes_key = key;
	if(dd->mode == METHOD_AES_K128) {
		dd->keylen = 4;
	}
	else if(dd->mode == METHOD_AES_K192)	{
		dd->keylen = 6;
	}
	else if(dd->mode == METHOD_AES_K256){
		dd->keylen = 8;
	}
}

static void aes_decode(struct mc_aes_platdata *dd,u32 emode,u32 mode, u32*key, u32 *iv)
{
	dd->mode = mode;
	dd->emode = emode;
	dd->decrypt = true;

	dd->aes_iv = iv;
	dd->ivlen = 4;

	dd->aes_key = key;
	if(dd->mode == METHOD_AES_K128) {
		dd->keylen = 4;
	}
	else if(dd->mode == METHOD_AES_K192)	{
		dd->keylen = 6;
	}
	else if(dd->mode == METHOD_AES_K256){
		dd->keylen = 8;
	}

}


static void des_encode(struct mc_aes_platdata *dd,u32 emode, u32*key, u32 *iv)
{
	dd->mode = METHOD_DES;
	dd->emode = emode;
	dd->decrypt = false;

	dd->aes_iv = iv;
	dd->ivlen = 2;

	dd->aes_key = key;
	dd->keylen = 2;

}

static void des_decode(struct mc_aes_platdata *dd,u32 emode, u32*key, u32 *iv)
{
	dd->mode = METHOD_DES;
	dd->emode = emode;
	dd->decrypt = true;

	dd->aes_iv = iv;
	dd->ivlen = 2;

	dd->aes_key = key;
	dd->keylen = 2;
}

static void tripledes_encode(struct mc_aes_platdata *dd,u32 emode, u32*key, u32 *iv)
{
	dd->mode = METHOD_TDES;
	dd->emode = emode;
	dd->decrypt = false;

	dd->aes_iv = iv;
	dd->ivlen = 2;

	dd->aes_key = key;
	dd->keylen = 6;
}

static void tripledes_decode(struct mc_aes_platdata *dd,u32 emode, u32*key, u32 *iv)
{
	dd->mode = METHOD_TDES;
	dd->emode = emode;
	dd->decrypt = true;

	dd->aes_iv = iv;
	dd->ivlen = 2;

	dd->aes_key = key;
	dd->keylen = 6;
}

static int mc_aes_config(struct mc_aes_platdata *dd, u32 mode, u32*key, u32 *iv)
{
	dd->aes_iv = iv;
	dd->aes_key = key;

	switch(mode)
		{
			case AES_128_CBC_ENCODE:
				aes_encode(dd, EMOD_CBC, METHOD_AES_K128, key, iv);
			break;
			case AES_192_CBC_ENCODE:
				aes_encode(dd, EMOD_CBC, METHOD_AES_K192, key, iv);
			break;
			case AES_256_CBC_ENCODE:
				aes_encode(dd, EMOD_CBC, METHOD_AES_K256, key, iv);
			break;
			case AES_128_ECB_ENCODE:
				aes_encode(dd, EMOD_ECB, METHOD_AES_K128, key, iv);
			break;
			case AES_192_ECB_ENCODE:
				aes_encode(dd, EMOD_ECB, METHOD_AES_K192, key, iv);
			break;
			case AES_256_ECB_ENCODE:
				aes_encode(dd, EMOD_ECB, METHOD_AES_K256, key, iv);
			break;
			case AES_128_CTR_ENCODE:
				aes_encode(dd, EMOD_CTR, METHOD_AES_K128, key, iv);
			break;
			case AES_192_CTR_ENCODE:
				aes_encode(dd, EMOD_CTR, METHOD_AES_K192, key, iv);
			break;
			case AES_256_CTR_ENCODE:
				aes_encode(dd, EMOD_CTR, METHOD_AES_K256, key, iv);
			break;
			case AES_128_OFB_ENCODE:
				aes_encode(dd, EMOD_OFB, METHOD_AES_K128, key, iv);
			break;
			case AES_192_OFB_ENCODE:
				aes_encode(dd, EMOD_OFB, METHOD_AES_K192, key, iv);
			break;
			case AES_256_OFB_ENCODE:
				aes_encode(dd, EMOD_OFB, METHOD_AES_K256, key, iv);
			break;
			case AES_128_CFB_ENCODE:
				aes_encode(dd, EMOD_CFB, METHOD_AES_K128, key, iv);
			break;
			case AES_192_CFB_ENCODE:
				aes_encode(dd, EMOD_CFB, METHOD_AES_K192, key, iv);
			break;
			case AES_256_CFB_ENCODE:
				aes_encode(dd, EMOD_CFB, METHOD_AES_K256, key, iv);
			break;
			case DES_ECB_ENCODE:
				des_encode(dd, EMOD_ECB, key, iv);
			break;
			case DES_CBC_ENCODE:
				des_encode(dd, EMOD_CBC, key, iv);
			break;
			case DES_CFB_ENCODE:
				des_encode(dd, EMOD_CFB, key, iv);
			break;
			case DES_OFB_ENCODE:
				des_encode(dd, EMOD_OFB, key, iv);
			break;
			case TDES_ECB_ENCODE:
				tripledes_encode(dd, EMOD_ECB, key, iv);
			break;
			case TDES_CBC_ENCODE:
				tripledes_encode(dd, EMOD_CBC, key, iv);
			break;
			case TDES_CFB_ENCODE:
				tripledes_encode(dd, EMOD_CFB, key, iv);
			break;
			case TDES_OFB_ENCODE:
				tripledes_encode(dd, EMOD_OFB, key, iv);
			break;

			case AES_128_CBC_DECODE:
				aes_decode(dd, EMOD_CBC, METHOD_AES_K128, key, iv);
			break;
			case AES_192_CBC_DECODE:
				aes_decode(dd, EMOD_CBC, METHOD_AES_K192, key, iv);
			break;
			case AES_256_CBC_DECODE:
				aes_decode(dd, EMOD_CBC, METHOD_AES_K256, key, iv);
			break;
			case AES_128_ECB_DECODE:
				aes_decode(dd, EMOD_ECB, METHOD_AES_K128, key, iv);
			break;
			case AES_192_ECB_DECODE:
				aes_decode(dd, EMOD_ECB, METHOD_AES_K192, key, iv);
			break;
			case AES_256_ECB_DECODE:
				aes_decode(dd, EMOD_ECB, METHOD_AES_K256, key, iv);
			break;
			case AES_128_CTR_DECODE:
				aes_decode(dd, EMOD_CTR, METHOD_AES_K128, key, iv);
			break;
			case AES_192_CTR_DECODE:
				aes_decode(dd, EMOD_CTR, METHOD_AES_K192, key, iv);
			break;
			case AES_256_CTR_DECODE:
				aes_decode(dd, EMOD_CTR, METHOD_AES_K256, key, iv);
			break;
			case AES_128_OFB_DECODE:
				aes_decode(dd, EMOD_OFB, METHOD_AES_K128, key, iv);
			break;
			case AES_192_OFB_DECODE:
				aes_decode(dd, EMOD_OFB, METHOD_AES_K192, key, iv);
			break;
			case AES_256_OFB_DECODE:
				aes_decode(dd, EMOD_OFB, METHOD_AES_K256, key, iv);
			break;
			case AES_128_CFB_DECODE:
				aes_decode(dd, EMOD_CFB, METHOD_AES_K128, key, iv);
			break;
			case AES_192_CFB_DECODE:
				aes_decode(dd, EMOD_CFB, METHOD_AES_K192, key, iv);
			break;
			case AES_256_CFB_DECODE:
				aes_decode(dd, EMOD_CFB, METHOD_AES_K256, key, iv);
			break;
			case DES_ECB_DECODE:
				des_decode(dd, EMOD_ECB, key, iv);
			break;
			case DES_CBC_DECODE:
				des_decode(dd, EMOD_CBC, key, iv);
			break;
			case DES_CFB_DECODE:
				des_decode(dd, EMOD_CFB, key, iv);
			break;
			case DES_OFB_DECODE:
				des_decode(dd, EMOD_OFB, key, iv);
			break;
			case TDES_ECB_DECODE:
				tripledes_decode(dd, EMOD_ECB, key, iv);
			break;
			case TDES_CBC_DECODE:
				tripledes_decode(dd, EMOD_CBC, key, iv);
			break;
			case TDES_CFB_DECODE:
				tripledes_decode(dd, EMOD_CFB, key, iv);
			break;
			case TDES_OFB_DECODE:
				tripledes_decode(dd, EMOD_OFB, key, iv);
			break;
			default:
				printf("aes mode error!!!\n");
			break;

		}
	aes_write_ctrl(dd);
	aes_decrypt(dd);
	return 0;
}

static int mc_aes_start(struct mc_aes_platdata *dd, u32 *src, u32 *dst, u32 len)
{
	if(((u32)src % ALIGN_SIZE)){
		printf("the src addr is not 16byte align ,return!!!\n");
		return 0;
	}
	if(((u32)dst % ALIGN_SIZE)){
		printf("the dst addr is not 16byte align ,return!!!\n");
		return 0;
	}

	dd->if_thold = IF_THOLD_VAL;
	dd->of_thold = OF_THOLD_VAL;

	dd->src = (u32)src;
	dd->dest = (u32)dst;
	dd->tran_size =len;
	aes_dma_init(dd);
	aes_start(dd);
		
	return 0;
}

int mc_aes_call(u32 mode, u32 *key, u32 *iv,u32 *src, u32 *dst, u32 len)
{
	struct udevice *dev = mc_aes_dev;

	struct mc_aes_platdata *dd = dev_get_platdata(dev);

	mc_aes_config(dd, mode, key, iv);

	mc_aes_start(dd, src, dst, len);

	return 0;
}


u32 key1_128[] = {0x2b7e1516,0x28aed2a6,0xabf71588,0x09cf4f3c};
u32 iv1[] = {0x00010203,0x04050607,0x08090a0b,0x0c0d0e0f};
u8	plain_text1[] __attribute__((aligned(16))) = {
    0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
    0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
    0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,
    0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
    0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,
    0xe5,0xfb,0xc1,0x19,0x1a,0x0a,0x52,0xef,
    0xf6,0x9f,0x24,0x45,0xdf,0x4f,0x9b,0x17,
    0xad,0x2b,0x41,0x7b,0xe6,0x6c,0x37,0x10,
};
u8 cipher_text1[64] __attribute__((aligned(16)))= {0};


u8 aes_cbc_cipher1_k128[] __attribute__((aligned(16)))= {
    0x76, 0x49, 0xAB, 0xAC, 0x81, 0x19, 0xB2, 0x46, 
    0xCE, 0xE9, 0x8E, 0x9B, 0x12, 0xE9, 0x19, 0x7D, 
    0x50, 0x86, 0xCB, 0x9B, 0x50, 0x72, 0x19, 0xEE, 
    0x95, 0xDB, 0x11, 0x3A, 0x91, 0x76, 0x78, 0xB2, 
    0x73, 0xBE, 0xD6, 0xB8, 0xE3, 0xC1, 0x74, 0x3B, 
    0x71, 0x16, 0xE6, 0x9E, 0x22, 0x22, 0x95, 0x16, 
    0x3F, 0xF1, 0xCA, 0xA1, 0x68, 0x1F, 0xAC, 0x09, 
    0x12, 0x0E, 0xCA, 0x30, 0x75, 0x86, 0xE1, 0xA7
};

void aes_enable(void)
{
	u32 val,i;
	val = __raw_readl(AP_MTX_CLK_CTRL0);
	val |= CKG_AHB_AES_EN;
	__raw_writel(val, AP_MTX_CLK_CTRL0);
	val = __raw_readl(CPU_SYS_RST1);
	val |= AES_SOFT_RST;
	__raw_writel(val, CPU_SYS_RST1);
	for(i=0;i<0x100;i++);
	val &= ~(AES_SOFT_RST);
	__raw_writel(val, CPU_SYS_RST1);
}



static int mc_aes_ofdata_to_platdata(struct udevice *dev)
{
	struct mc_aes_platdata *plat = dev_get_platdata(dev);
	plat->base = (void *)dev_read_addr(dev);
	return 0;
}


static const struct udevice_id mc_aes_ids[] = {
	{ .compatible = "mc,aes" },
	{}
};

U_BOOT_DRIVER(mc_aes) = {
	.name = "mc,aes",
	.id = UCLASS_MISC,
	.of_match = mc_aes_ids,
	.ofdata_to_platdata = mc_aes_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct mc_aes_platdata),
};

u32 src_text[48] __attribute__((aligned(16)))= {0x11223344, 0x22334455, 0x33445566, 0x44556677, 0x55667788, 0x66778899};
u32 dst_text[48] __attribute__((aligned(16)))= {0};
u32 dst1_text[48] __attribute__((aligned(16)))= {0};

int mc_aes_use_efuse_key(u32 mode, u32 *iv,u32 *src, u32 *dst, u32 len, u8 entry_offset, u8 key_num);
static int aes_test(cmd_tbl_t *cmdtp, int flag,
		       int argc, char * const argv[])
{
	aes_enable();

	mc_aes_call(AES_128_CBC_ENCODE, key_128, iv, src_text, dst_text, 48);
	mc_aes_call(AES_128_CBC_DECODE, key_128, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_128_CBC test success!!!\n");
	}else{
		printf("AES_128_CBC test fail!!!\n");
	}

	mc_aes_call(AES_192_CBC_ENCODE, key_192, iv, src_text, dst_text, 48);
	mc_aes_call(AES_192_CBC_DECODE, key_192, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_192_CBC test success!!!\n");
	}else{
		printf("AES_192_CBC test fail!!!\n");
	}

	mc_aes_call(AES_256_CBC_ENCODE, key_256, iv, src_text, dst_text, 48);
	mc_aes_call(AES_256_CBC_DECODE, key_256, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_256_CBC test success!!!\n");
	}else{
		printf("AES_256_CBC test fail!!!\n");
	}

	mc_aes_call(AES_128_ECB_ENCODE, key_128, iv, src_text, dst_text, 48);
	mc_aes_call(AES_128_ECB_DECODE, key_128, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_128_ECB test success!!!\n");
	}else{
		printf("AES_128_ECB test fail!!!\n");
	}

	mc_aes_call(AES_192_ECB_ENCODE, key_192, iv, src_text, dst_text, 48);
	mc_aes_call(AES_192_ECB_DECODE, key_192, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_192_ECB test success!!!\n");
	}else{
		printf("AES_192_ECB test fail!!!\n");
	}

	mc_aes_call(AES_256_ECB_ENCODE, key_256, iv, src_text, dst_text, 48);
	mc_aes_call(AES_256_ECB_DECODE, key_256, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_256_ECB test success!!!\n");
	}else{
		printf("AES_256_ECB test fail!!!\n");
	}

	mc_aes_call(AES_128_CTR_ENCODE, key_128, iv, src_text, dst_text, 48);
	mc_aes_call(AES_128_CTR_DECODE, key_128, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_128_CTR test success!!!\n");
	}else{
		printf("AES_128_CTR test fail!!!\n");
	}

	mc_aes_call(AES_192_CTR_ENCODE, key_192, iv, src_text, dst_text, 48);
	mc_aes_call(AES_192_CTR_DECODE, key_192, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_192_CTR test success!!!\n");
	}else{
		printf("AES_192_CTR test fail!!!\n");
	}

	mc_aes_call(AES_256_CTR_ENCODE, key_256, iv, src_text, dst_text, 48);
	mc_aes_call(AES_256_CTR_DECODE, key_256, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_256_CTR test success!!!\n");
	}else{
		printf("AES_256_CTR test fail!!!\n");
	}

	mc_aes_call(AES_128_OFB_ENCODE, key_128, iv, src_text, dst_text, 48);
	mc_aes_call(AES_128_OFB_DECODE, key_128, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_128_OFB test success!!!\n");
	}else{
		printf("AES_128_OFB test fail!!!\n");
	}

	mc_aes_call(AES_192_OFB_ENCODE, key_192, iv, src_text, dst_text, 48);
	mc_aes_call(AES_192_OFB_DECODE, key_192, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_192_OFB test success!!!\n");
	}else{
		printf("AES_192_OFB test fail!!!\n");
	}

	mc_aes_call(AES_256_OFB_ENCODE, key_256, iv, src_text, dst_text, 48);
	mc_aes_call(AES_256_OFB_DECODE, key_256, iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_256_OFB test success!!!\n");
	}else{
		printf("AES_256_OFB test fail!!!\n");
	}

	mc_aes_call(DES_ECB_ENCODE, des_key, des_iv, src_text, dst_text, 48);
	mc_aes_call(DES_ECB_DECODE, des_key, des_iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("DES_ECB test success!!!\n");
	}else{
		printf("DES_ECB test fail!!!\n");
	}

	mc_aes_call(DES_CBC_ENCODE, des_key, des_iv, src_text, dst_text, 48);
	mc_aes_call(DES_CBC_DECODE, des_key, des_iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("DES_CBC test success!!!\n");
	}else{
		printf("DES_CBC test fail!!!\n");
	}

	mc_aes_call(DES_CFB_ENCODE, des_key, des_iv, src_text, dst_text, 48);
	mc_aes_call(DES_CFB_DECODE, des_key, des_iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("DES_CFB test success!!!\n");
	}else{
		printf("DES_CFB test fail!!!\n");
	}

	mc_aes_call(DES_OFB_ENCODE, des_key, des_iv, src_text, dst_text, 48);
	mc_aes_call(DES_OFB_DECODE, des_key, des_iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("DES_OFB test success!!!\n");
	}else{
		printf("DES_OFB test fail!!!\n");
	}

	mc_aes_call(TDES_ECB_ENCODE, triple_des_key, triple_des_iv, src_text, dst_text, 48);
	mc_aes_call(TDES_ECB_DECODE, triple_des_key, triple_des_iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("TDES_ECB test success!!!\n");
	}else{
		printf("TDES_ECB test fail!!!\n");
	}

	mc_aes_call(TDES_CBC_ENCODE, triple_des_key, triple_des_iv, src_text, dst_text, 48);
	mc_aes_call(TDES_CBC_DECODE, triple_des_key, triple_des_iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("TDES_CBC test success!!!\n");
	}else{
		printf("TDES_CBC test fail!!!\n");
	}

	mc_aes_call(TDES_CFB_ENCODE, triple_des_key, triple_des_iv, src_text, dst_text, 48);
	mc_aes_call(TDES_CFB_DECODE, triple_des_key, triple_des_iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("TDES_CFB test success!!!\n");
	}else{
		printf("TDES_CFB test fail!!!\n");
	}

	mc_aes_call(TDES_OFB_ENCODE, triple_des_key, triple_des_iv, src_text, dst_text, 48);
	mc_aes_call(TDES_OFB_DECODE, triple_des_key, triple_des_iv, dst_text, dst1_text, 48);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("TDES_OFB test success!!!\n");
	}else{
		printf("TDES_OFB test fail!!!\n");
	}

#ifdef AES_EFUSE_TEST

	//test aes use efuse key 
	mc_aes_use_efuse_key(AES_128_CBC_ENCODE, iv, src_text,dst_text, 0x30, 0, 4);

	mc_aes_use_efuse_key(AES_128_CBC_DECODE, iv, dst_text, dst1_text, 0x30, 0, 4);
	if(0 == memcmp(src_text, dst1_text, 48)){
		printf("AES_128_CBC test use efuse key success!!!\n");
	}else{
		printf("AES_128_CBC test use efuse key fail!!!\n");
	}

#endif

	return 0;

}

U_BOOT_CMD(
	test_aes, 1, 1, aes_test,
	"test the aes",
	""
);
