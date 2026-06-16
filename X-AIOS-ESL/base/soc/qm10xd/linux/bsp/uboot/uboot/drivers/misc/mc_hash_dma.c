
#include "mc_hash_dma.h"


#define __raw_readb(addr)	\
	(*(volatile unsigned char *) (addr))

#define __raw_writeb(b, addr)	\
	((*(volatile unsigned char *) (addr)) = (unsigned int)(b))

#define __raw_readl(addr)	\
	(*(volatile unsigned int *) (addr))

#define __raw_writel(b, addr)	\
	((*(volatile unsigned int *) (addr)) = (unsigned int)(b))

#define false			(0)
#define true			(1)


  /*
  static void dma_enable(bool en)
  {
    
    u32 v;
    v = __raw_readl(REG_AHB_PERI_CLK_CTRL);
    if(en)    
      v |= CKG_DMA_EN;  
    else    
      v &= ~(CKG_DMA_EN);
    __raw_writel(v, REG_AHB_PERI_CLK_CTRL);
  }
  static void dma_reset(void)
  {
    u32 v,i;
    v = __raw_readl(REG_AP_AHB_PERI_SOFT_RST);
    v |= DMA_SOFT_RST;
    __raw_writel(v, REG_AP_AHB_PERI_SOFT_RST);
    for(i = 0; i< 0x100;i++);
    v &= ~DMA_SOFT_RST;
    __raw_writel(v, REG_AP_AHB_PERI_SOFT_RST);
  }
  */
  void hash_init_chn_reg(unsigned int chn)
  {
    unsigned int i;
    unsigned int reg;
    reg = FULL_CH_PAUSE(chn);
    for(i = 0; i < 0x10; i++) {
      dma_write(0,reg);
      reg += 4;   
    }
  }
  void hash_enable_channel(unsigned int chn)
  {
    dma_write(CH_EN,FULL_CH_CFG(chn));
  }
   void dma_frag_wait(unsigned int wait)
  {
    unsigned int v;
    v = dma_read(DMA_FRAG_WAIT);
    v &= ~(DMA_FRAG_WAIT_MASK << DMA_FRAG_WAIT_SHIFT);
    wait &= DMA_FRAG_WAIT_MASK;
    v |= (wait << DMA_FRAG_WAIT_SHIFT);
    dma_write(v,DMA_FRAG_WAIT);
  }
   void dma_chn_priority(unsigned int chn,unsigned int pri)
  {
    unsigned int v;
    v = dma_read(FULL_CH_CFG(chn));
    v &= ~(CH_PRIORITY_MASK << CH_PRIORITY_SHIFT);
    pri &= CH_PRIORITY_MASK;
    v |= (pri << CH_PRIORITY_SHIFT);
    dma_write(v,FULL_CH_CFG(chn));
  }
  void hash_dma_chn_src(unsigned int chn,unsigned int src)
  {
    dma_write(src,FULL_CH_SRC_ADDR(chn));
  }
  void dma_chn_src_high(unsigned int chn,unsigned int src)
  {
    unsigned int v;
    v = dma_read(FULL_CH_WRAP_PTR(chn));
    v &= ~(CH_SRC_ADDR_MASK << CH_SRC_ADDR_SHIFT);
    v |= (src << CH_SRC_ADDR_SHIFT);
    dma_write(v,FULL_CH_WRAP_PTR(chn));
  }
  
  void hash_dma_chn_dst(unsigned int chn,unsigned int dst)
  {
    dma_write(dst,FULL_CH_DST_ADDR(chn));
  }
  
  void hash_dma_trsc_len(unsigned int chn,unsigned int len)
  {
    unsigned int v;
    v = dma_read(FULL_CH_TRSC_LEN(chn));
    v &= ~(TRSC_LEN_MASK << TRSC_LEN_SHFIT);
    len &= TRSC_LEN_MASK;
    v |= (len << TRSC_LEN_SHFIT);
    dma_write(v,FULL_CH_TRSC_LEN(chn));
    
    v = dma_read(FULL_CH_BLK_LEN(chn));
    v &= ~(BLK_LEN_MASK << BLK_LEN_SHFIT);
    if(len < BLK_LEN_MASK){
      len &= BLK_LEN_MASK;
      v |= (BLK_LEN_MASK<< BLK_LEN_SHFIT);
    }else{
  
      v |= (0x80*4 << BLK_LEN_SHFIT);
    }
    dma_write(v,FULL_CH_BLK_LEN(chn));
    
    v = dma_read(FULL_CH_FRAG_LEN(chn));
    v &= ~(FRAG_LEN_MASK << FRAG_LEN_SHIFT);
    //if(len < FRAG_LEN_MASK){
    if(len < 0x80){
  
      len &= FRAG_LEN_MASK;
      v |= (len << FRAG_LEN_SHIFT);
    }else{
  
      v |= (0x80*4 << FRAG_LEN_SHIFT);
    }
    dma_write(v,FULL_CH_FRAG_LEN(chn));
  }
  
  
   void dma_req_mode(unsigned int chn,unsigned int mode)
  {
    unsigned int v;
    v = dma_read(FULL_CH_FRAG_LEN(chn));
    v &= ~(REQ_MODE_MASK << REQ_MODE_SHFIT);
    mode &= REQ_MODE_MASK;
    v |= (mode << REQ_MODE_SHFIT);
    dma_write(v,FULL_CH_FRAG_LEN(chn));
  }
   void dma_switch_mode(unsigned int chn,unsigned int mode)
  {
    unsigned int v;
    v = dma_read(FULL_CH_FRAG_LEN(chn));
    v &= ~(SWT_MODE_MASK << SWT_MODE_SHIFT);
    mode &= SWT_MODE_MASK;
    v |= (mode << SWT_MODE_SHIFT);
    dma_write(v,FULL_CH_FRAG_LEN(chn));
  }
  
   void dma_ahb_size(unsigned int chn,unsigned int size)
  {
    unsigned int v;
    v = dma_read(FULL_CH_FRAG_LEN(chn));
    v &= ~(DST_SIZE_MASK << DST_SIZE_SHFIT);
    v &= ~(SRC_SIZE_MASK << SRC_SIZE_SHIFT);
    size &= DST_SIZE_MASK;
    v |= (size << DST_SIZE_SHFIT) | (size << SRC_SIZE_SHIFT);
    dma_write(v,FULL_CH_FRAG_LEN(chn));
  }
   void dma_trsf_step(unsigned int chn,unsigned int step)
  {
    unsigned int v;
    v = dma_read(FULL_CH_TRSF_STEP(chn));
    v &= ~(DST_TRSF_STEP_MASK << DST_TRSF_STEP_SHIFT);
    v &= ~(SRC_TRSF_STEP_MASK << SRC_TRSF_STEP_SHFIT);
    step &= SRC_TRSF_STEP_MASK;
    v |= (step << SRC_TRSF_STEP_SHFIT) | (step << DST_TRSF_STEP_SHIFT);
    dma_write(v,FULL_CH_TRSF_STEP(chn));
  }
   void dma_wrap_enable(unsigned int chn,unsigned char en)
  {
    unsigned int v;
    v = dma_read(FULL_CH_FRAG_LEN(chn));
    v &= ~(ADDR_WRAP_EN);
    if(en)
      v |= ADDR_WRAP_EN;
    dma_write(v,FULL_CH_FRAG_LEN(chn));
  }
   void dma_fix_enable(unsigned int chn,unsigned char en)
  {
    unsigned int v;
    v = dma_read(FULL_CH_FRAG_LEN(chn));
    v &= ~(ADDR_FIX_EN);
    if(en)
      v |= ADDR_FIX_EN|ADDR_FIX_SEL;
    dma_write(v,FULL_CH_FRAG_LEN(chn));
  }
  void hash_dma_int_enable(unsigned int chn)
  {
    unsigned int v;
    v = dma_read(FULL_CH_INT(chn));
    v |= CH_CFG_ERR_INT_EN | CH_LLIST_INT_EN | CH_TRSC_INT_EN |
        CH_BLK_INT_EN | CH_FRAGMENT_INT_EN;
    dma_write(v,FULL_CH_INT(chn));
  }

  
   void dma_clk_enable(void)
  {
    unsigned int v;
    v = dma_read(REG_DMA_PAUSE);
    v |= (GLB_REG_CLK_EN | CHNL_REG_CLK_EN | 
        REQ_CID_CLK_EN | INT_REG_CLK_EN | 
        AXI_MST_CLK_EN | AUDIO_CNT_CLK_EN);
    dma_write(v,REG_DMA_PAUSE);
  }
  void hash_dma_int_clear(unsigned int chn)
  {
    unsigned int v;
    v = dma_read(FULL_CH_INT(chn));
    v |= CH_CFG_ERR_INT_CLR | CH_LLIST_INT_CLR | CH_TRSC_INT_CLR |
        CH_BLK_INT_CLR | CH_FRAGMENT_INT_CLR;
    dma_write(v,FULL_CH_INT(chn));
  }

  
  void hash_dma_wait_complete(unsigned int chn, unsigned int timeout)
  {
    unsigned int v;
    //u32 t1,t2;
    unsigned int status = CH_TRSC_INT_MASK_STAT | CH_BLK_INT_MASK_STAT |
         CH_FRAGMENT_INT_MASK_STAT;
  //  t1 = get_sys_timer_val();
    do {
      //t2 = get_sys_timer_val();
      v = dma_read(FULL_CH_INT(chn));
      if((v & status) == status) {
        hash_dma_int_clear(chn);
        break;
      }
    }while(1);
  }
  
  void hash_spic_request_cid(unsigned int chn)
  {
    chn = chn + 1;
    dma_write(chn,REQ1_CID(SPIC_TX_RID));
    dma_write(chn,REQ1_CID(SPIC_RX_RID));
  }
  
  void hash_request_cid(unsigned int chn)
  {
    chn = chn + 1;
    dma_write(chn, REQ1_CID(HASH_RX_RID));
  }
  
  /*
  void dma_init(void)
  {
    dma_enable(true);
    dma_reset();
  }
  */
  void hash_dma_init(unsigned int chn)
  {
    hash_init_chn_reg(chn);
    dma_clk_enable();
    dma_frag_wait(0);
    hash_enable_channel(chn);
    hash_dma_int_enable(chn);
    dma_chn_priority(chn, PRIORITY_LOWEST);
    dma_req_mode(chn, REQ_FRAGEMENT);
    dma_switch_mode(chn, SWT_ABCD_DCBA);
    dma_ahb_size(chn, AHB_SIZE_WORD);
    dma_trsf_step(chn, 4);
    dma_fix_enable(chn, true);
    dma_wrap_enable(chn, false);
    hash_request_cid(chn);
  }
  
  void dma_init_chnel(unsigned int chn)
  {
    hash_init_chn_reg(chn);
    dma_clk_enable();
    dma_frag_wait(0);
    hash_enable_channel(chn);
    hash_dma_int_enable(chn);
    dma_chn_priority(chn,PRIORITY_LOWEST);
    dma_req_mode(chn,REQ_TRANSACTION);
    dma_switch_mode(chn,SWT_ABCD_ABCD);
    dma_ahb_size(chn,AHB_SIZE_WORD);
    dma_trsf_step(chn,4);
    dma_fix_enable(chn,false);
    dma_wrap_enable(chn,false);
    hash_spic_request_cid(chn);
  
  }

