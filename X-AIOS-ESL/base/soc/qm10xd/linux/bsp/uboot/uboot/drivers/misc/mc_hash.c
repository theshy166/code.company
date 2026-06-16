#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <display_options.h>
#include <dm.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <misc.h>

#include "../../board/molchip/board/glb_reg.h"
#include "mc_hash.h"
#include "mc_hash_dma.h"


#define hash_writel(v, x)		\
	__raw_writel(v, (unsigned long)(REG_HASH_BASE + x))
#define hash_readl(x)			\
	__raw_readl((unsigned long)(REG_HASH_BASE + x))

#define 	get_sys_time_val()				 __raw_readl(REG_SYS_TIME_VALUE)

#define HASH_SHA1			0
#define HASH_SHA256			1
#define HASH_SHA384			2
#define HASH_SHA512			3

void hash_enable(void)
{
    unsigned int val,i;

    val = __raw_readl(CEN_PERI_CLK_CTRL);
    val &= ~CKG_HASH_EN;
    __raw_writel(val, CEN_PERI_CLK_CTRL);

    val = __raw_readl(CEN_PERI_APB_CLK_CTRL);
    val &= ~CKG_APB_HASH_EN;
    __raw_writel(val, CEN_PERI_APB_CLK_CTRL);

    //udelay(1);
    for(i=0;i<0x1000;i++);
    val = __raw_readl(AP_PERI_SOFT_RST0);
    val |= HASH_SOFT_RST;
    __raw_writel(val, AP_PERI_SOFT_RST0);

    val = __raw_readl(CKG_HASH_CFG);
    val &= (~0x3);
    __raw_writel(val, CKG_HASH_CFG);

    val |= 2;
    __raw_writel(val, CKG_HASH_CFG);

    //udelay(1);
    for(i=0;i<0x1000;i++);

    val = __raw_readl(CEN_PERI_CLK_CTRL);
    val |= CKG_HASH_EN;
    __raw_writel(val, CEN_PERI_CLK_CTRL);

    val = __raw_readl(CEN_PERI_APB_CLK_CTRL);
    val |= CKG_APB_HASH_EN;
    __raw_writel(val, CEN_PERI_APB_CLK_CTRL);

    val = __raw_readl(AP_PERI_SOFT_RST0);
    val &= ~HASH_SOFT_RST;
    __raw_writel(val, AP_PERI_SOFT_RST0);
}

void hash_mode(unsigned int mode)
{
    unsigned int val;

    val = hash_readl(HASH_CFG);
    val &= ~HASH_MODE_MASK;
    val |= mode & HASH_MODE_MASK;
    hash_writel(val, HASH_CFG);
}

void hash_msg_len(unsigned int len)
{
    hash_writel(len, HASH_MSG_LEN_CFG0);
}

void hash_start(void)
{
    unsigned int val,i;

    val = hash_readl(HASH_CFG);
    val &= ~HASH_START;
    hash_writel(val, HASH_CFG);

    //udelay(1);
    for(i=0;i<0x1000;i++);

    val |= HASH_START;
    hash_writel(val, HASH_CFG);
}

int hash_cfg(unsigned int chn, unsigned int src, unsigned int len)
{
    hash_dma_init(chn);
    hash_mode(HASH_SHA256);
    hash_msg_len(len);
    len = (((len + 3) >> 2) << 2);
    hash_enable_channel(chn);
    hash_dma_chn_src(chn, (src));
    hash_dma_chn_dst(chn, REG_HASH_BASE + HASH_FIFO);
    hash_dma_trsc_len(chn, len);
    hash_writel(0x80*4, MEM_INFO_CFG2);
    return true;
}

void hash_dma_enable(void)
{
    unsigned int i,value;
    
    //DMA enable
    value = __raw_readl(CPU_SYS_AHB_BASE + HASH_DMA_CTRL);
    value |=HASH_DMA_EN;
    __raw_writel(value,CPU_SYS_AHB_BASE + HASH_DMA_CTRL);

    //DMA reset
    value = __raw_readl(CPU_SYS_AHB_BASE + HASH_DMA_REG);
    value |= HASH_DMA_RST;
    raw_writel(value,CPU_SYS_AHB_BASE + HASH_DMA_REG);
    
    for(i=0; i< 0x100; i++);
    
    value &= ~(HASH_DMA_RST);
    __raw_writel(value,CPU_SYS_AHB_BASE + HASH_DMA_REG);
    
}

void read_result(unsigned int *result, unsigned int mode)
{
    unsigned int len, i;
    
    if (mode == HASH_SHA1) {
    	len = ((160 >> 3) >> 2);
    	for (i = 0; i < len; i++) {
    		result[i] = hash_swab32(hash_readl(HASH_H0 + i*4));
    	}
    } else if (mode == HASH_SHA256) {
    	len = ((256 >> 3) >> 2);
    	for (i = 0; i < len; i++) {
    		result[i] = hash_swab32(hash_readl(HASH_H0 + i*4));
    	}
    } else if (mode == HASH_SHA512) {
    	len = ((512 >> 3) >> 2);
    	for (i = 0; i < len; i += 2) {
    		result[i] = hash_swab32(hash_readl(HASH_H0 + (i + 1)*4));
    		result[i + 1] = hash_swab32(hash_readl(HASH_H0 + i*4));
    	}
    } else if (mode == HASH_SHA384) {
    	len = ((384 >> 3) >> 2);
    	for (i = 0; i < len; i += 2) {
    		result[i] = hash_swab32(hash_readl(HASH_H0 + (i + 1)*4));
    		result[i + 1] = hash_swab32(hash_readl(HASH_H0 + i*4));
    	}
    }
}
//extern void pts_init(void);
//extern unsigned long long pts_get_value(void);
//volatile unsigned int  hash_satrt_timer=0;
//volatile unsigned int hash_end_timer=0;

int mc_hw_sha(const unsigned char *indata, unsigned char *outdata, int datalen)
{
    unsigned int len,v,val,t1,t2;
    unsigned int status = INT_STATUS;
    unsigned int outtime = HASH_WAIT_COMPLETE_TIME_OUT;
	//pts_init();

    hash_enable();
    hash_mode(HASH_SHA256);
    hash_msg_len(datalen);
    
    hash_dma_enable();
    hash_dma_init(DMA_CHN30); //dma
   // printf("begain start hash\r\n");
   // hash_satrt_timer= pts_get_value();
    len = (((datalen + 3) >> 2) << 2);
    hash_enable_channel(DMA_CHN30);
    hash_dma_chn_src(DMA_CHN30, (u32)((unsigned long)(indata)));
    hash_dma_chn_dst(DMA_CHN30, REG_HASH_BASE + HASH_FIFO);
    hash_dma_trsc_len(DMA_CHN30, len);
    
    hash_writel(HASH_DMA_REQ_BYTE_NUM, MEM_INFO_CFG2);

    hash_start();

    t1 = get_sys_time_val();
      do {
        t2 = get_sys_time_val();
          v = hash_readl(HASH_INT);
          if((v & status) == status) {

          val= hash_readl(HASH_INT);
          val |= HASH_INT_CLR;
          hash_writel(val, HASH_INT);

          val= hash_readl(HASH_INT);
          val &= (~HASH_INT_CLR);
          hash_writel(val, HASH_INT);
             goto hash_complete;
           }
         } while(t2 - t1 < outtime);
        return -1;
hash_complete:
		//hash_end_timer= pts_get_value();
    read_result((unsigned int *)outdata,HASH_SHA256);
		//printf("hash satrt_timer = 0x%x\n",hash_satrt_timer);
	//printf("hash end_timer = 0x%x\n",hash_end_timer);
    return 0;
}

static void print_usage(void)
{
    printf("Usage:HW_sha_test indata indata-addr indata-size(bytes) hashvalue_addr\n");
    printf("example:\n");
    printf("1.tftp 0x41000000 test_data.bin\n");//tftp test_data.bin to 0x41000000(mmc read & sf read)
    printf("2.HW_sha_test test_data.bin 0x41000000 sizeof(test_data.bin) 0x42000000\n");//test the data
}

static int hash_test(cmd_tbl_t *cmdtp, int flag,
		       int argc, char * const argv[])
{
    HW_sha hash;
    unsigned int i;
    unsigned int *result;
    int src_len;
    unsigned int src_addr = 0,dst_addr = 0;

    if (argc != 5) {
      printf("please set data addr and size first!!\n");
      print_usage();
      return 0;
    }

    hash = &mc_hw_sha;
    src_addr = simple_strtoul(argv[2], NULL, 10);
    dst_addr = simple_strtoul(argv[4], NULL, 10);
    src_len = simple_strtoul(argv[3], NULL, 10);

    //decode
    if(hash((unsigned char *)src_addr,(unsigned char *)dst_addr,src_len) != 0) {
      printf("HW_sha_test no-done!\n");
    } else {
      printf("HW_sha_test done!\n");
    }

    //printf result
    result = (unsigned int *)dst_addr;
    for(i = 0;i < 8;i++) {
      printf("result[%d] = 0x%x\n",i,result[i]);
    }

    return 0;
}

U_BOOT_CMD(
	HW_sha_test, 5, 1, hash_test,
	"test the HW sha",
	""
);


