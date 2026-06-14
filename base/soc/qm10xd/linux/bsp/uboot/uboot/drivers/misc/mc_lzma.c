#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <display_options.h>
#include <dm.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <misc.h>

#include "../../board/molchip/board/glb_reg.h"

#include "mc_lzma.h"

volatile unsigned int  lzma_satrt_timer=0;
volatile unsigned int lzma_end_timer=0;
extern void pts_init(void);
extern unsigned long long pts_get_value(void);

void lzma_enable(void)
{
	unsigned int reg_val,i;

	reg_val = reg_readl(IOE_CFG);
	reg_val &= ~CKG_LZMA_CORE_EN;
	reg_val &= ~CKG_IOE_PUB_EN;
	reg_writel(reg_val, IOE_CFG);

	reg_val = reg_readl(IOE_CFG);
	reg_val |= IOE_LZMA_CORE_SOFT_RST;
	reg_val |=  CKG_IOE_PUB_SOFT_RST;
	reg_writel(reg_val, IOE_CFG);
	

	for(i=0;i<0x1000;i++);
	//lzma clk sel
	reg_val = reg_readl(IOE_CFG);
	reg_val |= ( ((4)) << 12);
	reg_writel(reg_val, IOE_CFG);
	

	for(i=0;i<0x1000;i++);

	reg_val = reg_readl(IOE_CFG);
	reg_val |= CKG_LZMA_CORE_EN;
	reg_val |= CKG_IOE_PUB_EN;
	reg_writel(reg_val, IOE_CFG);

	reg_val = reg_readl(IOE_CFG);
	reg_val &= ~IOE_LZMA_CORE_SOFT_RST;
	reg_val &= ~CKG_IOE_PUB_SOFT_RST;
	reg_writel(reg_val, IOE_CFG);
}
void lzma_start(void)
{
	unsigned int val ,i;

	val = reg_readl(DECODE_CTRL0);
	val |= 1;
	reg_writel(val, DECODE_CTRL0);
	

	for(i=0;i<0x1000;i++);

	val = reg_readl(DECODE_CTRL0);
	val &= (~1);
	reg_writel(val, DECODE_CTRL0);
}

bool lzma_done(unsigned int timer)
{
	unsigned int val, v;// t1, t2;
	unsigned int status = INT_DONE;

	//t1 = get_sys_timer_val();
	do {
		//t2 = get_sys_timer_val();
		v = reg_readl(INT_DONE_STATUS);
		if ((v & status) == status) {
			val = reg_readl(INT_DONE_CLEAR);
			val |= LZMA_DONE_CLEAR;
			reg_writel(val, INT_DONE_CLEAR);

			val = reg_readl(INT_DONE_CLEAR);
			val &= ~LZMA_DONE_CLEAR;
			reg_writel(val, INT_DONE_CLEAR);
			lzma_end_timer= pts_get_value();

			goto lzma_complete;
		}
	} while (1);

	return false;
lzma_complete:
	return true;
}


int mc_hw_lzma(const unsigned int *indata, unsigned int *outdata, int zip_len, int timer_out)
{
	
	unsigned int reg_val,i;
	bool ret ;
	

	lzma_enable();

	reg_val = reg_readl(LZMA_DMA_CTRL);
	reg_val &= ~ BIT(0);
	reg_val |= 0x30;
	reg_writel(reg_val, LZMA_DMA_CTRL);


	reg_val = reg_readl(DECODE_CTRL0);
	reg_val |= (1 << 8);
	reg_writel(reg_val, DECODE_CTRL0);

	for(i=0;i<0x1000;i++);
	reg_val = reg_readl(DECODE_CTRL0);
	reg_val &= ~(1 << 8);
	reg_writel(reg_val, DECODE_CTRL0);

	reg_writel(((unsigned int)indata) >> 3, SRC_START_ADDR);
	reg_writel((((unsigned int)indata) >> 3) + 0x40 - 1, SRC_END_ADDR);
	reg_writel(((unsigned int)outdata) >> 3, DES_BASE_ADDR);
	reg_writel(zip_len, SRC_FILE_LENGTH);
	reg_writel(0xFFFFFFFF, INT_DONE_MASK);
	
	lzma_start();
	lzma_satrt_timer= pts_get_value();
	ret = lzma_done(timer_out);
	 
	if(ret == false){
		printf("lzma not done\r\n");
		return ret;
	}
	

	return true;
}
static int mc_lzma_test(cmd_tbl_t *cmdtp, int flag,
		       int argc, char * const argv[])
{
	unsigned int lzma_src_addr,lzma_dst_addr,lzma_zip_len,timer;
	hw_lzma  mc_lzma;
	printf("%d\r\n",argc);
    printf("%s\r\n",argv[0]);
	printf("%s\r\n",argv[1]);
	printf("%s\r\n",argv[2]);
	printf("%s\r\n",argv[3]);
    lzma_src_addr = simple_strtoul(argv[1], NULL, 10);
    lzma_dst_addr = simple_strtoul(argv[2], NULL, 10);
    lzma_zip_len = simple_strtoul(argv[3], NULL, 10);
	timer = simple_strtoul(argv[4], NULL, 10);
	printf("%x\r\n",lzma_src_addr);
	printf("%x\r\n",lzma_dst_addr);
	printf("%x\r\n",lzma_zip_len);
	mc_lzma =&mc_hw_lzma;
	//pts_init();
    
	if(mc_lzma((unsigned int *)lzma_src_addr,(unsigned int *)lzma_dst_addr,lzma_zip_len,timer) == 0)
		printf("lzma test fail \r\n");
	
	
	printf("lzma 11satrt_timer = 0x%x\n",lzma_satrt_timer);
	printf("lzma 11end_timer = 0x%x\n",lzma_end_timer);
	return 0;
}




U_BOOT_CMD(
	test_lzma, 5, 0, mc_lzma_test,
	"tlz",
	"ttz"
);
