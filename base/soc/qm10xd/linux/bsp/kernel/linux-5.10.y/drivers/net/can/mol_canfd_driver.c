#include <linux/clk.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/skbuff.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/can/dev.h>
#include <linux/can/error.h>
#include <linux/can/led.h>
#include <linux/pm_runtime.h>
#include <linux/of_device.h>

#include <linux/mini_clock/clock.h>
#include <linux/pinctrl/consumer.h>

/* Standard module information, edit as appropriate */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("mol/ipms can controller driver");

#define DRIVER_NAME "ipms_canfd"

// enable debug output
static int debug_on_off = 1;
//module_param(debug_on_off,int,1);
MODULE_VERSION("1.1");

/*ipms canfd debug information*/
#define DEV_PRINT(fmt,args...) printk(KERN_DEBUG "IPMS_CANFD:CAN DEVICE is %s " fmt,ndev->name,  ## args);

#undef PDEBUG
#define IPMS_CANFD_DEBUG
#ifdef IPMS_CANFD_DEBUG
#ifdef	__KERNEL__
#define PDEBUG(fmt,args...) if(debug_on_off){printk( KERN_DEBUG "IPMS_CANFD:" fmt,## args);}
#else
#define PDEBUG(fmt,args...) fprintf( stderr, fmt, ## args)
#endif
#else
#define PDEBUG(fmt,args...) 
#endif

#define CANFD_MASK	0x01
#define CAN_MASK	0x00
#define TX_MAX		1
#define CAN_DEFAULT_FREQ	(24000000)

/* CAN registers set */
enum canfd_device_reg {
	CANFD_RUBF_OFFSET			=	0x00,/*Receive Buffer Registers 0x00-0x4f(not finished)*/
	CANFD_RUBF_ID_OFFSET		=	0x00,
	CANFD_RBUF_CTL_OFFSET		=	0x04,
	CANFD_RBUF_DATA_OFFSET		=	0x08,
	
	CANFD_TBUF_OFFSET			=	0x50,	/*Transmit Buffer Registers 0x50-0x97(not finished)*/
	CANFD_TBUF_ID_OFFSET		=	0x50,
	CANFD_TBUF_CTL_OFFSET		=	0x54,
	CANFD_TBUF_DATA_OFFSET		=	0x58,
	
	CANFD_TTS_OFFSET			=	0x98,	/*Transmission Time Stamp 0x98-0x9f(not finished)*/	
	
	CANFD_CFG_STAT_OFFSET		=	0xa0,
	CANFD_TCMD_OFFSET			=	0xa1,
	CANFD_TCTRL_OFFSET			=	0xa2,
	CANFD_RCTRL_OFFSET			=	0xa3,
	CANFD_RTIE_OFFSET			=	0xa4,
	CANFD_RTIF_OFFSET			=	0xa5,
	CANFD_ERRINT_OFFSET			=	0xa6,
	CANFD_LIMIT_OFFSET			=	0xa7,
	CANFD_S_SEG_1_OFFSET		=	0xa8,
	CANFD_S_SEG_2_OFFSET		=	0xa9,
	CANFD_S_SJW_OFFSET			=	0xaa,
	CANFD_S_PRESC_OFFSET		=	0xab,

	CANFD_F_SEG_1_OFFSET		=	0xac,
	CANFD_F_SEG_2_OFFSET		=	0xad,
	CANFD_F_SJW_OFFSET			=	0xae,
	CANFD_F_PRESC_OFFSET		=	0xaf,
	
	CANFD_EALCAP_OFFSET			=	0xb0,
	CANFD_RECNT_OFFSET			=	0xb2,
	CANFD_TECNT_OFFSET			=	0xb3,

	CANFD_ACFCTRL_OFFSET		= 0xb4,
	CANFD_ACF_EN_OFFSET			= 0xb6,
	CANFD_ACODE_ID_OFFSET		= 0xb8,
	CANFD_ACODE_MASK_OFFSET		= 0xb8,
};

enum canfd_reg_bitchange{
	CAN_FD_SET_RST_MASK 		=	0x80,  	/*Set Reset Bit*/	
	CAN_FD_OFF_RST_MASK			=	0x7f,	/*Reset Off Bit*/
	
	CAN_FD_SET_FULLCAN_MASK		=	0x10,	/*set TTTBM as 1->full TTCAN mode*/
	CAN_FD_OFF_FULLCAN_MASK		=	0xef,	/*set TTTBM as 0->separate PTB and STB mode*/

	CAN_FD_SET_FIFO_MASK		=	0x20,	/*set TSMODE as 1->FIFO mode*/
	CAN_FD_OFF_FIFO_MASK		=	0xdf,	/*set TSMODE as 0->Priority mode*/
	
	CAN_FD_SET_TSONE_MASK		=	0x04,	
	CAN_FD_OFF_TSONE_MASK		=	0xfb,
	
	CAN_FD_SET_TSALL_MASK		=	0x02,
	CAN_FD_OFF_TSALL_MASK		=	0xfd,
	
	CAN_FD_LBMEMOD_MASK			=	0x40,	/*set loop back mode, external*/
	CAN_FD_LBMIMOD_MASK			=	0x20,
	
	CAN_FD_SET_BUSOFF_MASK		=	0x01,
	CAN_FD_OFF_BUSOFF_MASK		=	0xfe,
	
	CAN_FD_SET_TTSEN_MASK		=	0x80,	/*set ttsen, tts update enable*/
	CAN_FD_SET_BRS_MASK			=	0x10,	/*can fd Bit Rate Switch mask*/
	CAN_FD_OFF_BRS_MASK			=	0xef,	
	CAN_FD_SET_EDL_MASK			=	0x20,	/*Extended Data Length*/
	CAN_FD_OFF_EDL_MASK			=	0xdf,

	CAN_FD_SET_DLC_MASK			=	0x0f,	
	CAN_FD_SET_TENEXT_MASK		=	0x40,
	CAN_FD_SET_IDE_MASK			=	0x80,
	CAN_FD_OFF_IDE_MASK			=	0x7f,
	CAN_FD_SET_RTR_MASK			=	0x40,
	CAN_FD_OFF_RTR_MASK			=	0xbf,
	
	CAN_FD_INTR_ALL_MASK		=	0xff, 	/*all interrupts enable mask*/
	CAN_FD_SET_RIE_MASK			=	0x80,
	CAN_FD_OFF_RIE_MASK			=	0x7f,
	CAN_FD_SET_RFIE_MASK		=	0x20,
	CAN_FD_OFF_RFIE_MASK		=	0xdf,
	CAN_FD_SET_RAFIE_MASK		=	0x10,
	CAN_FD_OFF_RAFIE_MASK		=	0xef,
	CAN_FD_SET_EIE_MASK			=	0x02,
	CAN_FD_OFF_EIE_MASK			=	0xfd,	
	
	CAN_FD_TASCTIVE_MASK		=	0x02,
	CAN_FD_RASCTIVE_MASK		=	0x04,
	
	CAN_FD_SET_TBSEL_MASK		=	0x80,	/*message writen in STB*/
	CAN_FD_OFF_TBSEL_MASK		=	0x7f,	/*message writen in PTB*/	
	
	CAN_FD_SET_STBY_MASK		=	0x20,
	CAN_FD_OFF_STBY_MASK		=	0xdf,
	
	CAN_FD_SET_TPE_MASK			=	0x10,	/*Transmit primary enable*/
	
	CAN_FD_SET_TPA_MASK			=	0x08,
	
	CAN_FD_SET_SACK_MASK		=	0x80,
	CAN_FD_SET_RREL_MASK		=	0x10,

	CAN_FD_RSTAT_NOT_EMPTY_MASK	=	0x03,
	
	CAN_FD_SET_RIF_MASK			=	0x80,
	CAN_FD_OFF_RIF_MASK			=	0x7f,
	CAN_FD_SET_RAFIF_MASK		=	0x10,
	CAN_FD_SET_RFIF_MASK		=	0x20,

	CAN_FD_SET_TPIF_MASK		=	0x08,	/*Transmission Primary Interrupt Flag*/
	CAN_FD_SET_TSIF_MASK		=	0x04,
	CAN_FD_SET_EIF_MASK			=	0x02,
	CAN_FD_SET_AIF_MASK			=	0x01,

	
	CAN_FD_SET_EWARN_MASK		=	0x80,
	CAN_FD_SET_EPASS_MASK		=	0x40,
	CAN_FD_SET_EPIE_MASK		=	0x20,
	CAN_FD_SET_EPIF_MASK		=	0x10,
	CAN_FD_SET_ALIE_MASK		=	0x08,
	CAN_FD_SET_ALIF_MASK		=	0x04,
	CAN_FD_SET_BEIE_MASK		=	0x02,
	CAN_FD_SET_BEIF_MASK		=	0x01,
	CAN_FD_OFF_EPIE_MASK		=	0xdf,
	CAN_FD_OFF_BEIE_MASK		=	0xfd,
	
	CAN_FD_SET_AFWL_MASK		=	0x40,
	CAN_FD_SET_EWL_MASK			=	0x0b,
	
	CAN_FD_SET_KOER_MASK		=	0xe0,
	CAN_FD_SET_BIT_ERROR_MASK	=	0x20,
	CAN_FD_SET_FORM_ERROR_MASK	=	0x40,
	CAN_FD_SET_STUFF_ERROR_MASK	=	0x60,
	CAN_FD_SET_ACK_ERROR_MASK	=	0x80,
	CAN_FD_SET_CRC_ERROR_MASK	=	0xa0,
	CAN_FD_SET_OTH_ERROR_MASK	=	0xc0,

	CAN_FD_ACODE_SEL_MASK			= 0x20,
	CAN_FD_OFF_ACF_ADDR_MASK		= 0xf,
	CAN_FD_ACODE_MASK				= 0x1fffffff,
};

static int canfd_device_driver_bittime_configuration(struct net_device *ndev);

#define BITS_OF_BITTIMING_REG 		8		/*seg1,seg2,sjw,prescaler all have 8 bits*/
#define FBITS_IN_BITTIMING_STR		32		/*in can_bittiming strucure every field has 32 bits---->u32*/
#define SEG_1_SHIFT	 				0
#define SEG_2_SHIFT	 				8
#define SJW_SHIFT	 				16
#define PRESC_SHIFT	 				24
 
#define TTSEN_8_32_SHIFT			24		/*TTSEN bit used for 32 bit register read or write*/
#define RTR_32_8_SHIFT				24

/*transmit mode*/
#define XMIT_FULL					0
#define XMIT_SEP_FIFO				1
#define XMIT_SEP_PRIO				2
#define XMIT_PTB_MODE				3

struct ipms_platform_data{
	u8 can_or_canfd;
};

struct ipms_acf {
	u32 id;
	u32 acode_mask;
	u32 acode_id;
};

struct IPMS_canfd_priv {
	struct can_priv can;
	unsigned int tx_head;
	unsigned int tx_tail;
	unsigned int tx_max;
	struct napi_struct napi;
	u32 (*read_reg)(const struct IPMS_canfd_priv *priv, enum canfd_device_reg reg);
	void (*write_reg)(const struct IPMS_canfd_priv *priv, enum canfd_device_reg reg, u32 val);
	struct device *dev;
	void __iomem *reg_base;
	unsigned long irq_flags;
	struct clk *bus_clk;
	struct clk *can_clk;
	unsigned int tx_mode;
	unsigned int tbsel; /*tbsel=0 choose PTB,tbsel=1 choose STB*/
	u8 can_or_canfd;
};

/* CAN Bittiming constants */
/*Bittiming setting from IPMS DS. */
/*CANFD*/
static struct can_bittiming_const canfd_bittiming_const = {
	.name = DRIVER_NAME,
	.tseg1_min = 2,
	.tseg1_max = 17,
	.tseg2_min = 1,
	.tseg2_max = 8,
	.sjw_max = 8,
	.brp_min = 1,
	.brp_max = 256,
	.brp_inc = 1,

};

/* CAN Data Bittiming constants */
/*CAN2.0*/
//TODO: t_seg2 for CAN FD nominal is differently defined
static struct can_bittiming_const canfd_data_bittiming_const = {
	.name = DRIVER_NAME,
	.tseg1_min = 2,
	.tseg1_max = 65,
	.tseg2_min = 1,
	.tseg2_max = 8,
	.sjw_max = 16,
	.brp_min = 1,
	.brp_max = 512,
	.brp_inc = 1,
};

static const struct ipms_platform_data ipms_def_fd = {
	.can_or_canfd	= CANFD_MASK,	
};

static const struct ipms_platform_data ipms_def_nonfd = {
	.can_or_canfd	= CAN_MASK,	
};

static const struct of_device_id canfd_of_match[] = {
	{ .compatible = "ipms,can", .data = &ipms_def_nonfd, },
	{ .compatible = "ipms,canfd", .data = &ipms_def_fd, },
	{ .compatible = "molchip,can", .data = &ipms_def_nonfd, },
	{ /* end of list */ },
};
MODULE_DEVICE_TABLE(of, canfd_of_match);

static void canfd_write_reg_le(const struct IPMS_canfd_priv *priv,enum canfd_device_reg reg,u32 val){
	iowrite32(val, priv->reg_base + reg);
}

static u32 canfd_read_reg_le(const struct IPMS_canfd_priv *priv, enum canfd_device_reg reg){
	return ioread32(priv->reg_base + reg);
}

static void canfd_reigister_set_bit(const struct IPMS_canfd_priv *priv,enum canfd_device_reg reg,
			enum canfd_reg_bitchange set_mask){
	u8 temp=0;
	temp = ioread8(priv->reg_base+reg);
	iowrite8(temp|set_mask,priv->reg_base+reg);
}

static void canfd_reigister_off_bit(const struct IPMS_canfd_priv *priv,enum canfd_device_reg reg,
			enum canfd_reg_bitchange set_mask){
	u8 temp=0;
	temp = ioread8(priv->reg_base+reg);
	iowrite8(temp&set_mask,priv->reg_base+reg);
}

static int canfd_device_driver_bittime_configuration(struct net_device *ndev){

	struct IPMS_canfd_priv *priv= netdev_priv(ndev);
	struct can_bittiming *bt = &priv->can.bittiming;
	struct can_bittiming *dbt = &priv->can.data_bittiming;
	u32 reset_test,bittiming_temp,data_bittiming_temp;
	
	
	reset_test=ioread8(priv->reg_base+CANFD_CFG_STAT_OFFSET);
	if(!(reset_test&CAN_FD_SET_RST_MASK)){
		netdev_alert(ndev, "Not in reset mode, cannot set bit timing\n");
		return -EPERM;
	}
	bittiming_temp=((bt->phase_seg1+bt->prop_seg+bt->sjw-2)<<SEG_1_SHIFT)|((bt->phase_seg2-1)<<SEG_2_SHIFT)|
						((bt->sjw-1)<<SJW_SHIFT)|((bt->brp-1)<<PRESC_SHIFT);

	if( (((int)(bt->phase_seg1+bt->prop_seg+1)-2)<0) || (((int)(bt->phase_seg2)-1)<0) || (((int)(bt->sjw)-1)<0) ||
				(((int)(bt->brp)-1)<0) ){
		PDEBUG("slow bittime configuration is incorrect\n");
		return -EINVAL;
	}
	
	PDEBUG( "bittiming_temp value is %08x\n",bittiming_temp);
	priv->write_reg(priv,CANFD_S_SEG_1_OFFSET,bittiming_temp);
	if(priv->can_or_canfd==CANFD_MASK){	
		data_bittiming_temp=((dbt->phase_seg1+dbt->prop_seg+1-2)<<SEG_1_SHIFT)|((dbt->phase_seg2-1)<<SEG_2_SHIFT)|
				((dbt->sjw-1)<<SJW_SHIFT)|((dbt->brp-1)<<PRESC_SHIFT);
		PDEBUG("test phase_seg1 is %d\n",dbt->phase_seg1);
		if( (((int)(dbt->phase_seg1+dbt->prop_seg+1)-2)<0) || (((int)(dbt->phase_seg2)-1)<0) ||
				(((int)(dbt->sjw)-1)<0) || (((int)(dbt->brp)-1)<0) ){
			PDEBUG("fast bittime configuration is incorrect\n");
			return -EINVAL;
		}
		priv->write_reg(priv,CANFD_F_SEG_1_OFFSET,data_bittiming_temp);

	}
	/*reset off*/
	canfd_reigister_off_bit(priv,CANFD_CFG_STAT_OFFSET,CAN_FD_OFF_RST_MASK);
	
	/*print configured slow and fast bit rate*/
	PDEBUG( "Slow bit rate configuration: %08x\n",priv->read_reg(priv,CANFD_S_SEG_1_OFFSET));
	PDEBUG( "Fast bit rate configuration: %08x\n",priv->read_reg(priv,CANFD_F_SEG_1_OFFSET));
	return 0;
}

static int canfd_device_set_acf(struct net_device *ndev, struct ipms_acf * acf)
{
	struct IPMS_canfd_priv *priv= netdev_priv(ndev);
	u8 reset_test, val;

	reset_test=ioread8(priv->reg_base + CANFD_CFG_STAT_OFFSET);
	if(!(reset_test & CAN_FD_SET_RST_MASK)){
		printk(KERN_WARNING "can not int reset mode\n");
		return -EPERM;
	}

	val = ioread8(priv->reg_base + CANFD_ACFCTRL_OFFSET);
	val &= ~(CAN_FD_OFF_ACF_ADDR_MASK | CAN_FD_ACODE_SEL_MASK);
	val |= acf->id;
	iowrite8(val, priv->reg_base + CANFD_ACFCTRL_OFFSET);
	priv->write_reg(priv, CANFD_ACODE_ID_OFFSET, (acf->acode_id & CAN_FD_ACODE_MASK));

	val = ioread8(priv->reg_base + CANFD_ACFCTRL_OFFSET);
	val |= CAN_FD_ACODE_SEL_MASK;
	iowrite8(val, priv->reg_base + CANFD_ACFCTRL_OFFSET);
	priv->write_reg(priv, CANFD_ACODE_MASK_OFFSET, (acf->acode_mask & CAN_FD_ACODE_MASK));
	return 0;

}

static void canfd_device_acf_enable(struct net_device *ndev, int id, bool en)
{
	struct IPMS_canfd_priv *priv= netdev_priv(ndev);
	u8 val;

	val = ioread8(priv->reg_base + CANFD_ACF_EN_OFFSET);

	if (en == true)
		val |= (1 << id);
	else
		val &= ~(1 << id);

	iowrite8(val, priv->reg_base + CANFD_ACF_EN_OFFSET);
}


int canfd_get_freebuffer(struct IPMS_canfd_priv *priv){
	/*get next transmit buffer. If all buffers are full, return -1*/
	canfd_reigister_set_bit(priv,CANFD_TCTRL_OFFSET,CAN_FD_SET_TENEXT_MASK);
	if(ioread8(priv->reg_base+CANFD_TCTRL_OFFSET)&CAN_FD_SET_TENEXT_MASK){
		return -1;
		
	}
	return 0;
	
}

static void canfd_tx_interrupt(struct net_device *ndev,u8 isr){
	struct IPMS_canfd_priv *priv= netdev_priv(ndev);
	
	//PDEBUG( "canfd tx interrupt\n");
	/*wait till transmission of the PTB or STB finished*/
	while(isr & (CAN_FD_SET_TPIF_MASK|CAN_FD_SET_TSIF_MASK)){
		if(isr & CAN_FD_SET_TPIF_MASK)
			canfd_reigister_set_bit(priv,CANFD_RTIF_OFFSET,CAN_FD_SET_TPIF_MASK);
		
		if(isr & CAN_FD_SET_TSIF_MASK)
			canfd_reigister_set_bit(priv,CANFD_RTIF_OFFSET,CAN_FD_SET_TSIF_MASK);
		//TODO: time out error		
		isr = ioread8(priv->reg_base+CANFD_RTIF_OFFSET);	
	}
	netif_wake_queue(ndev);
}

/*
 * Return: 1 on success and 0 on failure.
 */
static int can_rx(struct net_device *ndev){
	struct IPMS_canfd_priv *priv= netdev_priv(ndev);
	struct net_device_stats *stats = &ndev->stats;
	struct can_frame *cf;
	struct sk_buff *skb;
	
	u32 can_id;
	u8	dlc,control,rx_status;
	int i;

	PDEBUG("start can_rx interrrupt\n");	
	
	rx_status=ioread8(priv->reg_base+CANFD_RCTRL_OFFSET);
	if(rx_status & CAN_FD_RSTAT_NOT_EMPTY_MASK){
		control=ioread8(priv->reg_base+CANFD_RBUF_CTL_OFFSET);
		can_id=priv->read_reg(priv,CANFD_RUBF_ID_OFFSET);
		dlc=ioread8(priv->reg_base+CANFD_RBUF_CTL_OFFSET)&CAN_FD_SET_DLC_MASK;
		
		skb	= alloc_can_skb(ndev,(struct can_frame **)&cf);
		
		if(!skb){
			stats->rx_dropped++;
			return 0;
			
		}	
        /*read  CAN data length*/
		cf->can_dlc=get_can_dlc(dlc);
		PDEBUG("DLC in can_rx is %02x",dlc);	
		/*change the CANFD id into socketcan id format*/
		if(control&CAN_FD_SET_IDE_MASK){
			/*extended format*/
			cf->can_id=can_id;
			cf->can_id |=CAN_EFF_FLAG ;
			PDEBUG("ID Extension\n");	
		/*bit 29,error message not defined here*/
		//TODO:error message 
		}
		else{
			cf->can_id=can_id;
			cf->can_id &=(~CAN_EFF_FLAG) ;
			PDEBUG("without ID Extension\n");	
		}
		
		if(control&CAN_FD_SET_RTR_MASK)
			cf->can_id |=CAN_RTR_FLAG ;
		
		/* Data*/
		if(!(control&CAN_FD_SET_RTR_MASK)){
			*((u32*)(cf->data+0))= priv->read_reg(priv,CANFD_RBUF_DATA_OFFSET);
			*((u32*)(cf->data+4))= priv->read_reg(priv,CANFD_RBUF_DATA_OFFSET+4);
		}
		
		/*point to next packet and will save to buffer*/	
		/*reset RREL to release RB slot, next RBUF will be updated. Then reset receive interrupt flag(we can not reset interrupt flag here)*/
		canfd_reigister_set_bit(priv,CANFD_RCTRL_OFFSET,CAN_FD_SET_RREL_MASK);
		
		/*do some statistics*/
		stats->rx_bytes += can_dlc2len(cf->can_dlc);
		stats->rx_packets++;
		i=netif_receive_skb(skb);
		if (i != NET_RX_SUCCESS) {
			// TODO: error handling
			PDEBUG( "test in receive interrupt. skb->dev->stats.rx_bytes is %ld.\n",skb->dev->stats.rx_bytes);
			PDEBUG( "netif_receive_skb return value is %d\n",i);
        }

		return 1;
	}
	return 0;
}

/*
 * Return: 1 on success and 0 on failure.
 */
static int canfd_rx(struct net_device *ndev){
	struct IPMS_canfd_priv *priv= netdev_priv(ndev);
	struct net_device_stats *stats = &ndev->stats;
	struct canfd_frame *cf;
	struct sk_buff *skb;
	
	u32 can_id;
	u8	dlc,control,rx_status;
	int i;
	
	/*since here we start canfd_rx interrrupt*/
	PDEBUG("start canfd_rx interrrupt\n");
	
	/*rx control register has 32 bits--->u32 ctl will be used later*/	
	rx_status=ioread8(priv->reg_base+CANFD_RCTRL_OFFSET);
	if(rx_status & CAN_FD_RSTAT_NOT_EMPTY_MASK){
		control=ioread8(priv->reg_base+CANFD_RBUF_CTL_OFFSET);
		can_id=priv->read_reg(priv,CANFD_RUBF_ID_OFFSET);
		dlc=ioread8(priv->reg_base+CANFD_RBUF_CTL_OFFSET)&CAN_FD_SET_DLC_MASK;

		if(control&CAN_FD_SET_EDL_MASK)
			/* allocate sk_buffer for canfd frame */
			skb = alloc_canfd_skb(ndev,&cf);
		else
			/* allocate sk_buffer for can frame */
			skb	= alloc_can_skb(ndev,(struct can_frame **)&cf);
		
		if(!skb){
			stats->rx_dropped++;
			return 0;
			
		}	
	/*change the CANFD or CAN2.0 data into socketcan data format*/
	
		if(control&CAN_FD_SET_EDL_MASK)
			cf->len=can_dlc2len(dlc);
		else
			cf->len=get_can_dlc(dlc);
		
	/*change the CANFD id into socketcan id format*/
		if(control&CAN_FD_SET_EDL_MASK){
			/*extended format*/
			cf->can_id=can_id;
			if(control & CAN_FD_SET_IDE_MASK)
				cf->can_id |= CAN_EFF_FLAG;
			else
				cf->can_id &= (~CAN_EFF_FLAG);
		/*bit 29,error message not defined here*/
			
		}
		else{
			cf->can_id=can_id;
			if(control & CAN_FD_SET_IDE_MASK)
				cf->can_id |= CAN_EFF_FLAG;
			else
				cf->can_id &= (~CAN_EFF_FLAG);
	/*deal with RTR in can2.0*/
			if(control&CAN_FD_SET_RTR_MASK)
				cf->can_id |=CAN_RTR_FLAG ;
			
		}
		
		/* Data*/
		/*CANFD frames handed over to SKB*/
		if(control&CAN_FD_SET_EDL_MASK){
			for (i = 0; i < cf->len; i += 4) {
				PDEBUG( "CAN FD original DATA 0x%08x\n",priv->read_reg(priv,CANFD_RBUF_DATA_OFFSET+i));
				*((u32 *)(cf->data+i)) = priv->read_reg(priv,CANFD_RBUF_DATA_OFFSET+i);
			}
		}
		else{
		/*skb reads the received datas, if the RTR bit not set.*/
			if(!(control&CAN_FD_SET_RTR_MASK)){
				*((u32*)(cf->data+0))= priv->read_reg(priv,CANFD_RBUF_DATA_OFFSET);
				*((u32*)(cf->data+4))= priv->read_reg(priv,CANFD_RBUF_DATA_OFFSET+4);
			}	
		}

		/*point to next packet and will save to buffer*/
		/*reset RREL to release RB slot, next RBUF will be updated. Then reset receive interrupt flag(we can not reset interrupt flag here)*/
		
		canfd_reigister_set_bit(priv,CANFD_RCTRL_OFFSET,CAN_FD_SET_RREL_MASK);
		//canfd_reigister_set_bit(priv,CANFD_RTIF_OFFSET,CAN_FD_SET_RIF_MASK);
		stats->rx_bytes += cf->len;
		stats->rx_packets++;
		i=netif_receive_skb(skb);
		//netif_receive_skb(skb);
		if (i != NET_RX_SUCCESS) {
			// TODO: error handling
			PDEBUG( "test in receive interrupt. skb->dev->stats.rx_bytes is %ld.\n",skb->dev->stats.rx_bytes);
			PDEBUG( "netif_receive_skb return value is %d\n",i);
		}

		return 1;
	}
	return 0;
}


static int canfd_rx_poll(struct napi_struct *napi, int quota){
	struct net_device *ndev = napi->dev;
	struct IPMS_canfd_priv *priv= netdev_priv(ndev);
	int work_done = 0;
	u8 rx_status=0,control=0;
	
	
	control=ioread8(priv->reg_base + CANFD_RBUF_CTL_OFFSET);
	rx_status = ioread8(priv->reg_base + CANFD_RCTRL_OFFSET);
	//canfd_reigister_set_bit(priv,CANFD_RTIF_OFFSET,CAN_FD_SET_RIF_MASK);
	while((rx_status&CAN_FD_RSTAT_NOT_EMPTY_MASK)&&(work_done < quota)){
		( control & CAN_FD_SET_EDL_MASK) ? (work_done+=canfd_rx(ndev)) : (work_done+=can_rx(ndev));
		rx_status = ioread8(priv->reg_base + CANFD_RCTRL_OFFSET);
		PDEBUG( "rx-status is 0x%02x\n",rx_status);
		PDEBUG( "rx-status is 0x%08x\n",ioread32(priv->reg_base + CANFD_CFG_STAT_OFFSET));
	}

	//if (work_done < 10) {
		napi_complete(napi);
		canfd_reigister_set_bit(priv,CANFD_RTIE_OFFSET,	CAN_FD_SET_RIE_MASK);
		//canfd_reigister_set_bit(priv,CANFD_RTIE_OFFSET,	CAN_FD_SET_RFIE_MASK);
		//canfd_reigister_set_bit(priv,CANFD_RTIE_OFFSET,	CAN_FD_SET_RAFIE_MASK);
	//}
	return work_done;	
}

static void canfd_rxfull_interrupt(struct net_device *ndev,u8 isr){
	struct IPMS_canfd_priv *priv= netdev_priv(ndev);
	
	if(isr & CAN_FD_SET_RAFIF_MASK)
		canfd_reigister_set_bit(priv,CANFD_RTIF_OFFSET,CAN_FD_SET_RAFIF_MASK);

}

/*Mode could be chosen, by reading corresponding registers.*/
/*This part could also be removed, when only one mode is used*/
static int set_canfd_xmit_mode(struct net_device *ndev){
	struct IPMS_canfd_priv *priv = netdev_priv(ndev);
	switch(priv->tx_mode){
		case XMIT_FULL:
			canfd_reigister_set_bit(priv,CANFD_TCTRL_OFFSET,CAN_FD_SET_FULLCAN_MASK);
			PDEBUG( "Full can mode\n");
			break;
		
		case XMIT_SEP_FIFO:
			canfd_reigister_off_bit(priv,CANFD_TCTRL_OFFSET,CAN_FD_OFF_FULLCAN_MASK);
			canfd_reigister_set_bit(priv,CANFD_TCTRL_OFFSET,CAN_FD_SET_FIFO_MASK);
			canfd_reigister_off_bit(priv,CANFD_TCMD_OFFSET,CAN_FD_SET_TBSEL_MASK);
			PDEBUG( "FIFO mode\n");		
		
			break;
		
		case XMIT_SEP_PRIO:
			canfd_reigister_off_bit(priv,CANFD_TCTRL_OFFSET,CAN_FD_OFF_FULLCAN_MASK);
			canfd_reigister_off_bit(priv,CANFD_TCTRL_OFFSET,CAN_FD_OFF_FIFO_MASK);
			canfd_reigister_off_bit(priv,CANFD_TCMD_OFFSET,CAN_FD_SET_TBSEL_MASK);
			PDEBUG( "Priority mode\n");		
		
			break;		
		case XMIT_PTB_MODE:
			canfd_reigister_off_bit(priv,CANFD_TCMD_OFFSET,CAN_FD_OFF_TBSEL_MASK);/*PTB is used here*/
			PDEBUG( "PTB mode\n");		
		
			break;
		default:	
			break;
	}	
	return 0;
}

/*
 * netdev_tx_t (*ndo_start_xmit)(struct sk_buff *skb,
 *                               struct net_device *dev);
 *	Called when a packet needs to be transmitted.
 *	Must return NETDEV_TX_OK , NETDEV_TX_BUSY.
 *        (can also return NETDEV_TX_LOCKED iff NETIF_F_LLTX)
 *	Required can not be NULL.
*/
static int canfd_driver_start_xmit(struct sk_buff *skb, struct net_device *ndev){
	u32 ttsen,id,ctl,addr_off;
	int i;
	struct IPMS_canfd_priv *priv = netdev_priv(ndev);
	struct canfd_frame *cf = (struct canfd_frame *)skb->data;
	struct net_device_stats *stats = &ndev->stats;

	switch(priv->tx_mode){
		case XMIT_FULL:
		
			return -1;
			break;
		
		case XMIT_PTB_MODE:
			PDEBUG( "PTB XMIT Mode\n");
			/*set transmit register*/
			set_canfd_xmit_mode(ndev);
			
			/*select TBSEL, frame writen in PTB */
			//canfd_reigister_set_bit(priv,CANFD_TCMD_OFFSET,CAN_FD_SET_TBSEL_MASK);

			canfd_reigister_off_bit(priv,CANFD_TCMD_OFFSET,CAN_FD_OFF_STBY_MASK);/*standby mode off*/
			PDEBUG( "CFG_STAT is %02x after STBY off\n",ioread8(priv->reg_base+CANFD_CFG_STAT_OFFSET));
			PDEBUG( "TCMD is %02x after STBY off\n",ioread8(priv->reg_base+CANFD_TCMD_OFFSET));
			
			if(cf->can_id&CAN_EFF_FLAG){
				/*extended îd length, CAN_EFF_FLAG is set in the msb*/
				id =(cf->can_id & CAN_EFF_MASK);
				ttsen=0<<TTSEN_8_32_SHIFT;
				//ttsen=CAN_FD_SET_TTSEN_MASK<<TTSEN_8_32_SHIFT;
				id|=ttsen;	
			}else{
				/*standard id*/
				id =(cf->can_id & CAN_SFF_MASK);
				ttsen=0<<TTSEN_8_32_SHIFT;
				//ttsen=CAN_FD_SET_TTSEN_MASK<<TTSEN_8_32_SHIFT;
				id|=ttsen;	  
			}
			
			PDEBUG("cf->can_id=%x10",cf->can_id);
			
			ctl=can_len2dlc(cf->len);
			/*transmit can fd frame*/
			if(priv->can_or_canfd==CANFD_MASK){
				if (can_is_canfd_skb(skb)) {
					if(cf->can_id&CAN_EFF_FLAG)
						ctl |= CAN_FD_SET_IDE_MASK;	/*IDE=1 if IDenfitier Extension*/
					else
						ctl	&=CAN_FD_OFF_IDE_MASK;/*IDE=0*/
					
					if (cf->flags & CANFD_BRS)		/*CANFD_BRS=0x01*/
						ctl |= CAN_FD_SET_BRS_MASK;
					ctl |= CAN_FD_SET_EDL_MASK;
			
					addr_off=CANFD_TBUF_DATA_OFFSET;
					for(i=0;i<cf->len;i+=4){
						PDEBUG( "CAN FD original DATA 0x%08x\n",priv->read_reg(priv,addr_off));
				
						priv->write_reg(priv,addr_off,*((u32 *)(cf->data+i)));
						PDEBUG("data in transmit register is:0x%08x\n",ioread32(priv->reg_base+addr_off));
						PDEBUG( "CAN FD updated DATA 0x%08x\n",*((u32 *) (cf->data+i)));
						addr_off+=4;
					}
			
					PDEBUG( "TCMD is 0x%02x after writing data into resiger\n",
							ioread8(priv->reg_base+CANFD_TCMD_OFFSET));
					PDEBUG( "Interrupt flag register is 0x%02x after writing data into resiger\n",
							ioread8(priv->reg_base+CANFD_RTIF_OFFSET));
				}	
			//can_put_echo_skb(skb, ndev, 0);/*have no idea with this statement*/
			/*temp=canfd_get_freebuffer(priv);
			if(temp==-1){
				netif_stop_queue(ndev);	
			}
			*/
				/* Transmit can 2.0 frame under canfd mode*/
				else{
					ctl &= CAN_FD_OFF_EDL_MASK;//set Extended Data  Length 0, force BRS to be set 0 ;
					ctl &= CAN_FD_OFF_BRS_MASK;
					if(cf->can_id&CAN_EFF_FLAG)
						ctl |= CAN_FD_SET_IDE_MASK;	/*IDE=1 if IDenfitier Extension*/
					else
						ctl &= CAN_FD_OFF_IDE_MASK;/*IDE=0*/
					
					if(cf->can_id & CAN_RTR_FLAG){
						ctl |= CAN_FD_SET_RTR_MASK;
						priv->write_reg(priv,CANFD_TBUF_ID_OFFSET, id);
						priv->write_reg(priv,CANFD_TBUF_CTL_OFFSET,ctl);
					}
					else{
						ctl &= CAN_FD_OFF_RTR_MASK;
						addr_off=CANFD_TBUF_DATA_OFFSET;
						PDEBUG( "CAN original DATA 0x%08x\n",priv->read_reg(priv,addr_off));
						priv->write_reg(priv,addr_off,*((u32 *)(cf->data+0)));
						PDEBUG( "CAN updated DATA 0x%08x\n",priv->read_reg(priv,addr_off));
						PDEBUG( "CAN original DATA 0x%08x\n",priv->read_reg(priv,addr_off+4));
						priv->write_reg(priv,addr_off+4,*((u32 *)(cf->data+4)));
						PDEBUG( "CAN updated DATA 0x%08x\n",priv->read_reg(priv,addr_off+4));

						PDEBUG( "TCMD is 0x%02x after writing data into resiger\n",
								ioread8(priv->reg_base+CANFD_TCMD_OFFSET));
						PDEBUG( "Interrupt flag register is 0x%02x after writing data into resiger\n",
								ioread8(priv->reg_base+CANFD_RTIF_OFFSET));
					}

				}
				priv->write_reg(priv,CANFD_TBUF_ID_OFFSET, id);
				priv->write_reg(priv,CANFD_TBUF_CTL_OFFSET,ctl);
			
				PDEBUG( "ID 0x%08x\n",priv->read_reg(priv,CANFD_TBUF_ID_OFFSET));
				PDEBUG( "Control 0x%08x\n",priv->read_reg(priv,CANFD_TBUF_CTL_OFFSET));
			
				addr_off=CANFD_TBUF_DATA_OFFSET;
				PDEBUG( "TCMD is 0x%02x before set TPE=1\n",ioread8(priv->reg_base+CANFD_TCMD_OFFSET));
		
			}
			/*deal with can 2.0 frame, when the driver can only be compatible with can 2.0 tranceiver	*/	
			else{
				/*transmit can2.0 frame*/
				PDEBUG("Sending can2.0 frame\n");
				
				ctl &= CAN_FD_OFF_EDL_MASK;//set Extended Data  Length 0, force BRS to be set 0 ;
				ctl &= CAN_FD_OFF_BRS_MASK;
				if(cf->can_id&CAN_EFF_FLAG)
					ctl |= CAN_FD_SET_IDE_MASK;	/*IDE=1 if IDenfitier Extension*/
				else
					ctl &= CAN_FD_OFF_IDE_MASK;/*IDE=0*/
				
				if(cf->can_id & CAN_RTR_FLAG){
					ctl |= CAN_FD_SET_RTR_MASK;
					priv->write_reg(priv,CANFD_TBUF_ID_OFFSET, id);
					priv->write_reg(priv,CANFD_TBUF_CTL_OFFSET,ctl);
				}
				else{
					ctl &= CAN_FD_OFF_RTR_MASK;
				
					priv->write_reg(priv,CANFD_TBUF_ID_OFFSET, id);
					priv->write_reg(priv,CANFD_TBUF_CTL_OFFSET,ctl);
					
					PDEBUG( "ID 0x%08x\n",priv->read_reg(priv,CANFD_TBUF_ID_OFFSET));
					PDEBUG( "Control 0x%08x\n",priv->read_reg(priv,CANFD_TBUF_CTL_OFFSET));
			
					addr_off=CANFD_TBUF_DATA_OFFSET;
					PDEBUG( "CAN original DATA 0x%08x\n",priv->read_reg(priv,addr_off));
					priv->write_reg(priv,addr_off,*((u32 *)(cf->data+0)));
					PDEBUG( "CAN updated DATA 0x%08x\n",priv->read_reg(priv,addr_off));
					PDEBUG( "CAN original DATA 0x%08x\n",priv->read_reg(priv,addr_off+4));
					priv->write_reg(priv,addr_off+4,*((u32 *)(cf->data+4)));
					PDEBUG( "CAN updated DATA 0x%08x\n",priv->read_reg(priv,addr_off+4));

					PDEBUG( "TCMD is 0x%02x after writing data into resiger\n",
							ioread8(priv->reg_base+CANFD_TCMD_OFFSET));
					PDEBUG( "Interrupt flag register is 0x%02x after writing data into resiger\n",
							ioread8(priv->reg_base+CANFD_RTIF_OFFSET));
			
				}

			}	
			/*set TPE to transmit data. update statistic*/
			canfd_reigister_set_bit(priv,CANFD_TCMD_OFFSET,CAN_FD_SET_TPE_MASK);
			stats->tx_bytes += cf->len;
			//canfd_reigister_set_bit(priv,CANFD_RCTRL_OFFSET,CAN_FD_SET_SACK_MASK);/*set SACK---->loop back external*/
			
			/*stop transmit queue*/
			netif_stop_queue(ndev);	
			
			//canfd_reigister_set_bit(priv,CANFD_TCMD_OFFSET,CAN_FD_SET_TPA_MASK);
			//canfd_reigister_set_bit(priv,CANFD_RTIF_OFFSET,0x08);
			//PDEBUG( "TPA is %d\n",ioread8(priv->reg_base+CANFD_TCMD_OFFSET)&CAN_FD_SET_TPA_MASK>>3);
			//netif_stop_queue(ndev);	
			/*send interrupt flag debug*/
			PDEBUG( "Interrupt flag register is 0x%02x. TX ends\n",
						ioread8(priv->reg_base+CANFD_RTIF_OFFSET));
			PDEBUG( "Interrupt enable register is 0x%02x. TX ends\n",
						ioread8(priv->reg_base+CANFD_RTIE_OFFSET));

			PDEBUG( "Transmission status is %d. TX ends\n",
					ioread8(priv->reg_base+CANFD_CFG_STAT_OFFSET)&CAN_FD_TASCTIVE_MASK>>1);
			
			PDEBUG( "Receive buffer:Data first 8 bytes 0x%08x. TX ends\n",
					ioread32(priv->reg_base+CANFD_RBUF_DATA_OFFSET));
			PDEBUG( "TCMD is 0x%02x. TX ends\n",
					ioread8(priv->reg_base+CANFD_TCMD_OFFSET));
			PDEBUG( "Receive buffer:id 0x%08x. TX ends\n",
					ioread32(priv->reg_base+CANFD_RUBF_ID_OFFSET));
			PDEBUG( "Receive buffer:Control 0x%08x. TX ends\n",
					ioread32(priv->reg_base+CANFD_RBUF_CTL_OFFSET));
			PDEBUG( "Receive buffer:Data first 8 bytes 0x%08x. TX ends\n",
					ioread32(priv->reg_base+CANFD_RBUF_DATA_OFFSET));
			PDEBUG( "Interrupt flag register is 0x%02x. TX ends\n",
					ioread8(priv->reg_base+CANFD_RTIF_OFFSET));
			break;
		default:
			break;	
	}
	return NETDEV_TX_OK;
}

static int set_reset_mode(struct net_device *ndev){
	struct IPMS_canfd_priv *priv = netdev_priv(ndev);

	/*Situation for time out still not defined*/
	u8 ret;
	//unsigned long timeout;
	ret=ioread8(priv->reg_base + CANFD_CFG_STAT_OFFSET);
	ret|=CAN_FD_SET_RST_MASK;
	iowrite8(ret, priv->reg_base + CANFD_CFG_STAT_OFFSET);

	return 0;
}

static int canfd_driver_stop(struct net_device *ndev)
{
	struct IPMS_canfd_priv *priv = netdev_priv(ndev);

	PDEBUG( "can stops here\n");
	netif_stop_queue(ndev);
	napi_disable(&priv->napi);
	free_irq(ndev->irq, ndev);
	close_candev(ndev);
	set_reset_mode(ndev);
	return 0;
}

static enum can_state get_of_chip_status(struct net_device *ndev){
	struct IPMS_canfd_priv *priv = netdev_priv(ndev);
	u8  can_stat,eir;
	can_stat=ioread8(priv->reg_base+CANFD_CFG_STAT_OFFSET);
	eir = ioread8(priv->reg_base + CANFD_ERRINT_OFFSET);

	if(can_stat & CAN_FD_SET_BUSOFF_MASK)
		return CAN_STATE_BUS_OFF;
	if((eir & CAN_FD_SET_EPASS_MASK) && ~(can_stat & CAN_FD_SET_BUSOFF_MASK))
		return CAN_STATE_ERROR_PASSIVE;
	if(eir & CAN_FD_SET_EWARN_MASK && ~(eir & CAN_FD_SET_EPASS_MASK))
		return CAN_STATE_ERROR_WARNING;
	if(~(eir & CAN_FD_SET_EPASS_MASK))
		return CAN_STATE_ERROR_ACTIVE;
}

/*Error Interrupt Routine*/
static void canfd_error_interrupt(struct net_device *ndev, u8 isr, u8 eir){
	struct IPMS_canfd_priv *priv = netdev_priv(ndev);
	struct net_device_stats *stats = &ndev->stats;
	struct can_frame *cf;
	struct sk_buff *skb;
	u8  koer,recnt=0,tecnt=0,can_stat=0;
	
	DEV_PRINT("in Error interrupt:")
	skb = alloc_can_err_skb(ndev, &cf);
	
	koer = ioread8(priv->reg_base +CANFD_EALCAP_OFFSET)&CAN_FD_SET_KOER_MASK;
	recnt= ioread8(priv->reg_base +CANFD_RECNT_OFFSET);
	tecnt= ioread8(priv->reg_base +CANFD_TECNT_OFFSET);	

	//test
	//if(recnt>40 || tecnt>40) canfd_reigister_set_bit(priv,CANFD_CFG_STAT_OFFSET,CAN_FD_SET_BUSOFF_MASK);
	
	/*read CAN Status*/
	can_stat=ioread8(priv->reg_base+CANFD_CFG_STAT_OFFSET);
	
	PDEBUG("canfd_error_interrupt starts since here\n");
	PDEBUG("At the beginning of error interrupt, isr is 0x%02x and eir is 0x%02x\n",isr,eir);
	PDEBUG("canfd error counter: RECNT is 0x%02x, TECNT is 0x%02x\n",ioread8(priv->reg_base+CANFD_RECNT_OFFSET),
				ioread8(priv->reg_base+CANFD_TECNT_OFFSET));
	PDEBUG("KOER is :0x%02x at the beginning\n",koer);
	PDEBUG("canfd is %s the beginning\n",(can_stat & CAN_FD_SET_BUSOFF_MASK)?"busoff":"not busoff");
	/*define the status of can, when error exists*/

	/*BUS-OFF->active error mode*/
	if( (isr & CAN_FD_SET_EIF_MASK) && (priv->can.state==CAN_STATE_BUS_OFF)){
		//TODO: restart functions should be finished
		//can_restart_now(ndev);  //not sure,because dev.c describes->only can be used if automatic
		//restart is disabled and the device is in the bus-off state. How to know if it could be automatically called?
		priv->can.state=get_of_chip_status(ndev);
	}

	/*bus off*/
	/*if status is BUSOFF*/
	if(can_stat & CAN_FD_SET_BUSOFF_MASK){
		PDEBUG("CFG_STAT(BUSOFF bit) in EIF branch: 0x%02x\n",ioread8(priv->reg_base+CANFD_CFG_STAT_OFFSET));
		PDEBUG("BUSOFF exists\n");
		/*abortion of not transmitted frame*/
		//canfd_reigister_set_bit(priv,CANFD_TCMD_OFFSET,CAN_FD_SET_TPA_MASK);
		/*statistics*/
		priv->can.state=get_of_chip_status(ndev);
		priv->can.can_stats.bus_off++;
		/*reset canfd ip core*/
		//test with reset busoff status
		canfd_reigister_set_bit(priv,CANFD_CFG_STAT_OFFSET,CAN_FD_SET_BUSOFF_MASK);
		//canfd_reigister_set_bit(priv,CANFD_CFG_STAT_OFFSET,CAN_FD_SET_RST_MASK);

		/*tell netif layer,no more packets*/
		//TODO:configure the can to bus-off status?
		can_bus_off(ndev);
		if (skb)
			cf->can_id |= CAN_ERR_BUSOFF;
	}else if((eir & CAN_FD_SET_EPASS_MASK) && ~(can_stat & CAN_FD_SET_BUSOFF_MASK)){
		/*Error Passive mode*/
		/*EPASS is either 0 or 1. Hypothetically, when transceiver is in BUSOFF mode, bit EPASS stays unchanged, i.e. "1" */
		PDEBUG("Error Interrupt in Error Passive mode\n");
		priv->can.state = get_of_chip_status(ndev);
		priv->can.can_stats.error_passive++;
		if(skb){
			cf->can_id |= CAN_ERR_CRTL;
			cf->data[1] |= (recnt>127)?CAN_ERR_CRTL_RX_PASSIVE:0;
			cf->data[1] |= (tecnt>127)?CAN_ERR_CRTL_TX_PASSIVE:0;
			cf->data[6]=tecnt;
			cf->data[7]=recnt;
		}
	}else if(eir & CAN_FD_SET_EWARN_MASK && ~(eir & CAN_FD_SET_EPASS_MASK)){
		/*Counter over Error limitation*/

		PDEBUG("Error Warning\n");
		priv->can.state =get_of_chip_status(ndev);
		priv->can.can_stats.error_warning++;
		if(skb){
			/*Bosch specification indicates that Error count value greater than 96 is heavily disturbed.*/
			/*EWL=11*/
			cf->can_id |= CAN_ERR_CRTL;
			cf->data[1] |= (recnt>95)?CAN_ERR_CRTL_RX_WARNING:0;
			cf->data[1] |= (tecnt>95)?CAN_ERR_CRTL_TX_WARNING:0;
			cf->data[6]=tecnt;
			cf->data[7]=recnt;
		}
	}

	PDEBUG("CFG_STAT(BUSOFF bit) before protocol errors: 0x%02x\n",ioread8(priv->reg_base+CANFD_CFG_STAT_OFFSET));
	/* Check for in protocol defined error interrupt */
	if(eir & CAN_FD_SET_BEIF_MASK){
		PDEBUG("protocol defined error exists\n");
		PDEBUG("KOER is :0x%02x in protocol error \n",ioread8(priv->reg_base +CANFD_EALCAP_OFFSET)&CAN_FD_SET_KOER_MASK);
		if(skb)
			cf->can_id |= CAN_ERR_BUSERROR  | CAN_ERR_PROT;
		/*BIT error interrupt*/	
		if(koer == CAN_FD_SET_BIT_ERROR_MASK){
			PDEBUG("BIT error exists\n");
			stats->tx_errors++;
			if (skb) {
				cf->can_id |= CAN_ERR_PROT;
				cf->data[2] = CAN_ERR_PROT_BIT;
			}
		}

		/*FORM error interrupt*/	
		if(koer == CAN_FD_SET_FORM_ERROR_MASK){
			PDEBUG("FORM error exists\n");
			stats->rx_errors++;
			if (skb) {
				cf->can_id |= CAN_ERR_PROT;
				cf->data[2] = CAN_ERR_PROT_FORM;
			}
		}

		/*STUFF error interrupt*/
		if(koer == CAN_FD_SET_STUFF_ERROR_MASK){
			PDEBUG("STUFF error exists\n");
			stats->rx_errors++;
			if (skb) {
				cf->can_id |= CAN_ERR_PROT;
				cf->data[3] = CAN_ERR_PROT_STUFF;
			}
		}

		/*ACK error interrupt*/	
		if(koer == CAN_FD_SET_ACK_ERROR_MASK){
			PDEBUG("ACK error exists\n");
			/*test with abortion*/
			//canfd_reigister_set_bit(priv,CANFD_TCMD_OFFSET,CAN_FD_SET_TPA_MASK);
			stats->tx_errors++;
			if (skb) {
				cf->can_id |= CAN_ERR_PROT;
				cf->data[2] = CAN_ERR_PROT_LOC_ACK;
			}			
		}

		/*CRC error interrupt*/	
		if(koer == CAN_FD_SET_CRC_ERROR_MASK){
			PDEBUG("CRC error exists\n");
			stats->rx_errors++;
			if (skb) {
				cf->can_id |= CAN_ERR_PROT;
				cf->data[2] = CAN_ERR_PROT_LOC_CRC_SEQ ;
			}
		}
		priv->can.can_stats.bus_error++;
	}
	//canfd_reigister_set_bit(priv,CANFD_TCMD_OFFSET,CAN_FD_SET_TPA_MASK);
	//priv->can.state = TODO_GET_STATUS_OF_CHIP(struct net_device *ndev);

	if (skb) {
		stats->rx_packets++;
		stats->rx_bytes += cf->can_dlc;
		netif_rx(skb);
	}
	/*interrupt enable*/
	//canfd_reigister_set_bit(priv,CANFD_ERRINT_OFFSET,CAN_FD_SET_EPIE_MASK|CAN_FD_SET_BEIE_MASK);
	//canfd_reigister_set_bit(priv,CANFD_RTIE_OFFSET,CAN_FD_SET_EIE_MASK);
	PDEBUG("canfd error counter: RECNT is 0x%02x, TECNT is 0x%02x\n",ioread8(priv->reg_base+CANFD_RECNT_OFFSET),
			ioread8(priv->reg_base+CANFD_TECNT_OFFSET));
	PDEBUG("canfd_error_interrupt ends\n");
}

static irqreturn_t canfd_interrupt(int irq, void *dev_id){
	struct net_device *ndev = (struct net_device *)dev_id;
	struct IPMS_canfd_priv *priv = netdev_priv(ndev);
	u8 isr,eir;
	u8 isr_handled, eir_handled;
	/*read the value of interrupt status register and error interrupt register*/
	isr = ioread8(priv->reg_base + CANFD_RTIF_OFFSET);
	eir = ioread8(priv->reg_base + CANFD_ERRINT_OFFSET);
	

	isr_handled = 0;
	eir_handled = 0;
	
	/* Check for Tx interrupt and Processing it */
	if (isr & (CAN_FD_SET_TPIF_MASK|CAN_FD_SET_TSIF_MASK)){
		canfd_tx_interrupt(ndev, isr);	
		isr_handled |= (CAN_FD_SET_TPIF_MASK|CAN_FD_SET_TSIF_MASK);
	}
	
	if (isr & (CAN_FD_SET_RAFIF_MASK|CAN_FD_SET_RFIF_MASK) ) {
		canfd_rxfull_interrupt(ndev,isr);
		isr_handled |= (CAN_FD_SET_RAFIF_MASK|CAN_FD_SET_RFIF_MASK);
	}
	
	/*	Check Rx interrupt and Processing the receive interrupt routine*/
	if (isr & CAN_FD_SET_RIF_MASK){
		canfd_reigister_off_bit(priv,CANFD_RTIE_OFFSET,	CAN_FD_OFF_RIE_MASK);
		canfd_reigister_set_bit(priv,CANFD_RTIF_OFFSET,CAN_FD_SET_RIF_MASK);
		napi_schedule(&priv->napi);
		isr_handled |= CAN_FD_SET_RIF_MASK;
	
	}
	//test
	PDEBUG("RTIE before error routine is 0x%02x\n",ioread8(priv->reg_base+CANFD_RTIE_OFFSET));
	// if (~( (isr&CAN_FD_SET_EIF_MASK) || (eir & CAN_FD_SET_EPIF_MASK ) || (eir & CAN_FD_SET_BEIF_MASK))) {
	// 		return IRQ_NONE;
	// 	}
	if ( (isr & CAN_FD_SET_EIF_MASK) | (eir & (CAN_FD_SET_EPIF_MASK | CAN_FD_SET_BEIF_MASK))){
	//if (isr & CAN_FD_SET_EIF_MASK){
		PDEBUG("Error Error Error\n");
		//canfd_reigister_off_bit(priv,CANFD_ERRINT_OFFSET,CAN_FD_OFF_EPIE_MASK&CAN_FD_OFF_BEIE_MASK);
		//canfd_reigister_off_bit(priv,CANFD_RTIE_OFFSET,CAN_FD_OFF_EIE_MASK);
		
		/*reset EPIF and BEIF. Reset EIF. What if during error interrupt, new interrupt appears?*/
		canfd_reigister_set_bit(priv,CANFD_ERRINT_OFFSET,eir & (CAN_FD_SET_EPIF_MASK|CAN_FD_SET_BEIF_MASK));
		canfd_reigister_set_bit(priv,CANFD_RTIF_OFFSET,isr & CAN_FD_SET_EIF_MASK);		
		
		canfd_error_interrupt(ndev,isr,eir);
		isr_handled |= CAN_FD_SET_EIF_MASK;
		eir_handled |=(CAN_FD_SET_EPIF_MASK | CAN_FD_SET_BEIF_MASK);
	}
	if ((isr_handled == 0) && (eir_handled == 0)) {
		DEV_PRINT("unhandled interrupt!");
		return IRQ_NONE;
	}
	DEV_PRINT("ISR: %02X/%02X\n", isr, isr_handled);
	DEV_PRINT("EIR: %02X/%02X\n", eir, eir_handled);
	DEV_PRINT("canfd error counter: RECNT is 0x%02x, TECNT is 0x%02x\n",ioread8(priv->reg_base+CANFD_RECNT_OFFSET),
				ioread8(priv->reg_base+CANFD_TECNT_OFFSET));
	return IRQ_HANDLED;
}

static int canfd_chip_start(struct net_device *ndev){

	struct IPMS_canfd_priv *priv = netdev_priv(ndev);
	int temp;
	unsigned int ret;

	temp=set_reset_mode(ndev);
	if (temp < 0){
		PDEBUG( "set reset mod failed\n");
		return temp;
	}
	temp=canfd_device_driver_bittime_configuration(ndev);/*set S_seg_1 S_Seg_2 S_SJW S_PRESC */
	if (temp < 0){
		PDEBUG( "bittime setting failed\n");
		return temp;
	}
	canfd_reigister_off_bit(priv,CANFD_CFG_STAT_OFFSET,CAN_FD_OFF_RST_MASK);

	/*set Almost Full Warning Limit*/
	canfd_reigister_set_bit(priv,CANFD_LIMIT_OFFSET,CAN_FD_SET_AFWL_MASK);
	
	/*Programmable Error Warning Limit = (EWL+1)*8. Set EWL=11->Error Warning=96 */
	canfd_reigister_set_bit(priv,CANFD_LIMIT_OFFSET,CAN_FD_SET_EWL_MASK);
	/*the start state*/
	priv->can.state=CAN_STATE_ERROR_ACTIVE;
	/*interrupts enable*/
	iowrite8(CAN_FD_INTR_ALL_MASK,priv->reg_base + CANFD_RTIE_OFFSET);	
	/*Error Interrupts enable(Error Passive and Bus Error)*/
	canfd_reigister_set_bit(priv,CANFD_ERRINT_OFFSET,CAN_FD_SET_EPIE_MASK);
	canfd_reigister_set_bit(priv,CANFD_ERRINT_OFFSET,CAN_FD_SET_BEIE_MASK);
	/*check the mode,later on*/

	/*set mode*/
	ret=ioread8(priv->reg_base + CANFD_CFG_STAT_OFFSET);
	if (priv->can.ctrlmode == CAN_CTRLMODE_LOOPBACK) {
		ret |= CAN_FD_LBMEMOD_MASK;
	}
	iowrite8(ret, priv->reg_base + CANFD_CFG_STAT_OFFSET);

	return 0;
	
}

static int canfd_driver_open(struct net_device *ndev){
	struct IPMS_canfd_priv *priv = netdev_priv(ndev);
	int ret;
	
	/*test bittiming*/
	struct can_bittiming *bt = &priv->can.bittiming;
	struct can_bittiming *dbt = &priv->can.data_bittiming;
	
	PDEBUG( "slow bit rate setting is phase_seg1 0x%08x phase_seg2 0x%08x sjw 0x%08x prescaler 0x%02x prop_seg is 0x%02x\n",
				bt->phase_seg1,bt->phase_seg2,bt->sjw,bt->brp,bt->prop_seg);
	PDEBUG( "fast bit rate setting is phase_seg1 0x%08x  phase_seg2 0x%08x  sjw 0x%08x  prescaler 0x%02x prop_seg is 0x%02x\n",
				dbt->phase_seg1,dbt->phase_seg2,dbt->sjw,dbt->brp,dbt->prop_seg);
	
	/*request irq*/
	ret = request_irq(ndev->irq, canfd_interrupt, IRQF_SHARED, ndev->name, ndev);
	if (ret < 0) {
		netdev_err(ndev, "irq allocation for CAN failed\n");
		goto err;
	}
	PDEBUG( "request_irq finished\n");
	/* Set chip into reset mode */
	ret = set_reset_mode(ndev);
	if (ret < 0) {
		netdev_err(ndev, "mode resetting failed!\n");
		goto err_irq;
	}
	PDEBUG( "set_reset_mod finished\n");
	
	ret = open_candev(ndev);
	if (ret)
		goto err_irq;
	PDEBUG( "open_candev finished\n");
	
	ret = canfd_chip_start(ndev);
	if (ret < 0) {
		netdev_err(ndev, "canfd_chip_start failed!\n");
		goto err_candev;
	}
	PDEBUG( "canfd_chip_start finished\n");
	napi_enable(&priv->napi);
	netif_start_queue(ndev);

	PDEBUG( "canfd_driver_open success\n");
	return 0;
	
//TODO: handle with errors
err_candev:
	PDEBUG("Error with starting canfd chip\n");
err_irq:
	PDEBUG("Error during opening can device\n");
	free_irq(ndev->irq, ndev);
err:
	PDEBUG("Error during requesting irq\n");

	return ret;
}		

static const struct net_device_ops canfd_netdev_ops = {
	.ndo_open	= canfd_driver_open,
	.ndo_stop	= canfd_driver_stop,
	.ndo_start_xmit	= canfd_driver_start_xmit,
	.ndo_change_mtu	= can_change_mtu,
};

static ssize_t accept_filter_store(struct device *dev,
       struct device_attribute *attr, const char *buf, size_t size)
{
	char * ptr;
	int en;
	struct ipms_acf acf;
	struct net_device *ndev = NULL;
	struct IPMS_canfd_priv *priv = NULL;

	ndev = dev_get_drvdata(dev);
	if (!ndev)
		return -EINVAL;

	priv = netdev_priv(ndev);
	if (!priv)
		return -EINVAL;

	if (!buf || size > PAGE_SIZE)
		return -EINVAL;

	ptr = strchr(buf, 'N');
	if (ptr)
		acf.id = simple_strtol(ptr + 2, NULL, 10);
	ptr = strchr(buf, 'E');
	if (ptr)
		en = simple_strtol(ptr + 2, NULL, 10);
	ptr = strchr(buf, 'I');
	if (ptr)
		acf.acode_id = simple_strtol(ptr + 2, NULL, 0);
	ptr = strchr(buf, 'M');
	if (ptr)
		acf.acode_mask = simple_strtol(ptr + 2, NULL, 0);

	if (en) {
		canfd_device_acf_enable(ndev, acf.id, true);
	} else {
		canfd_device_acf_enable(ndev, acf.id, false);
	}

	canfd_device_set_acf(ndev, &acf);

	return size;
}
static ssize_t accept_filter_show(struct device *dev,
       struct device_attribute *attr, char *buf)
{
	return 0;
}


static DEVICE_ATTR(accept_filter, 0644, accept_filter_show, accept_filter_store);

static int canfd_driver_probe(struct platform_device *pdev){
	struct net_device *ndev;
	struct IPMS_canfd_priv *priv;
	void __iomem *addr = NULL;
	int ret, tx_max;
	struct resource *res; 
	const struct of_device_id *match;
	resource_size_t size;
	unsigned int freq;

	/*get virtual address*/
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	/*get the size of resource*/
	size = resource_size(res);

	/*ioremap: project the physical address(registers' addresses) to memory. return value is the virtual address in mem.*/
	addr =	devm_ioremap_resource(&pdev->dev,res); 
	if (IS_ERR(addr)) {
		ret = PTR_ERR(addr);
		goto err;
	}

	/* Create a CAN device instance */
	tx_max=TX_MAX;
	ndev = alloc_candev(sizeof(struct IPMS_canfd_priv), tx_max);
	if (!ndev) {
		ret = -ENOMEM;
		goto err;
	}
	/*get the private part in net_device*/
	priv = netdev_priv(ndev);

	/*find compatible device in match_table*/
	match = of_match_node(canfd_of_match, pdev->dev.of_node);
	if(match && match->data){
		const struct ipms_platform_data *data = match->data;
		priv->can_or_canfd=data->can_or_canfd;
		PDEBUG("priv->can_or_canfd is %d, driver type: %s \n",priv->can_or_canfd ,priv->can_or_canfd?"CANFD":"CAN");
	}

	/*basic device strucure embedded in higher-level representation of device*/
	priv->dev = &pdev->dev;

	/*get the bittiming table, which shows bit-time-setting limitation	in user space*/
	priv->can.bittiming_const = &canfd_bittiming_const;
	priv->can.data_bittiming_const = &canfd_data_bittiming_const;

	/*in user space the execution mode can be choosen */
	if(priv->can_or_canfd==CANFD_MASK)
		priv->can.ctrlmode_supported =CAN_CTRLMODE_LOOPBACK|CAN_CTRLMODE_FD;
	else
		//TODO: still have problem here 
		priv->can.ctrlmode_supported=CAN_CTRLMODE_LOOPBACK;

	/*basic device informations*/	
	priv->reg_base = addr;
	priv->tx_max = tx_max;
	priv->tx_head = 0;
	priv->tx_tail = 0;

	priv->tx_mode=XMIT_PTB_MODE;//test

	priv->write_reg = canfd_write_reg_le;
	priv->read_reg = canfd_read_reg_le;

	/*set bittime, sys clock*/
	if (of_property_read_u32(pdev->dev.of_node, "clock-freq", &freq))
		freq = CAN_DEFAULT_FREQ;

	priv->can.clock.freq = freq;

	clk_mini_enable(CKG_APB_CAN_EN);
	clk_mini_enable(CKG_CAN_EN);
	clk_mini_set_rate(CKG_MUX_CAN, freq);

	/*Get IRQ for the device*/
	ndev->irq = platform_get_irq(pdev,0);
	ndev->flags |= IFF_ECHO;	/*local echo is supported*/
	
	platform_set_drvdata(pdev, ndev);
	SET_NETDEV_DEV(ndev, &pdev->dev);
	ndev->netdev_ops = &canfd_netdev_ops;
	
	netif_napi_add(ndev,&priv->napi,canfd_rx_poll,16);//test budget=16
	
	ret =register_candev(ndev);
	if (ret) {
		dev_err(&pdev->dev, "fail to register failed (err=%d)\n", ret);
		goto err;
	}

	device_create_file(priv->dev, &dev_attr_accept_filter);

	// PDEBUG("end of probe\n");
	return 0;
err:
	PDEBUG("error in probe function");
	if (addr)
		devm_iounmap(&pdev->dev, addr);

	return ret;
}

static int canfd_driver_remove(struct platform_device *pdev){
    struct net_device *ndev = platform_get_drvdata(pdev);
	struct IPMS_canfd_priv *priv=netdev_priv(ndev);
	
	unregister_candev(ndev);
	netif_napi_del(&priv->napi);
	free_candev(ndev);
	clk_mini_disable(CKG_CAN_EN);
	clk_mini_disable(CKG_APB_CAN_EN);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int mol_canfd_suspend(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	int ret = 0;

	if (netif_running(ndev)) {
		netif_stop_queue(ndev);
		netif_device_detach(ndev);
		set_reset_mode(ndev);
	}

	clk_mini_disable(CKG_CAN_EN);
	clk_mini_disable(CKG_APB_CAN_EN);

	ret = pinctrl_pm_select_sleep_state(dev);
	if (ret) {
		dev_err(dev, "suspend pinctrl err %d\n", ret);
	}

	return ret;
}

static int mol_canfd_resume(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	int ret;

	clk_mini_enable(CKG_CAN_EN);
	clk_mini_enable(CKG_APB_CAN_EN);

	ret = pinctrl_select_default_state(dev);
	if (ret) {
		dev_err(dev, "resume pinctrl err %d\n", ret);
		return ret;
	}
	if (netif_running(ndev)) {
		ret = canfd_chip_start(ndev);
		if (ret) {
			dev_err(dev, "mol_canfd_chip_start failed on resume\n");
			return ret;
		}

		netif_device_attach(ndev);
		netif_start_queue(ndev);
	}

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(mol_canfd_pm_ops, mol_canfd_suspend, mol_canfd_resume);


// ************************************************************

static struct platform_driver can_driver = {
	.probe          = canfd_driver_probe,
	.remove         = canfd_driver_remove,
	.driver = {
		.name  = DRIVER_NAME,
		.of_match_table	= canfd_of_match,
		.pm	= &mol_canfd_pm_ops,
	},
};

int canfd_driver_init(void){
	platform_driver_register(&can_driver);
	return 0;
}

void canfd_driver_exit(void){
	platform_driver_unregister(&can_driver);
	return;
}

module_init(canfd_driver_init);
module_exit(canfd_driver_exit);
