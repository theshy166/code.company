/*  Synopsys DWC Ethernet Quality-of-Service v4.10a linux driver
 *
 *  This is a driver for the Synopsys DWC Ethernet QoS IP version 4.10a (GMAC).
 *  This version introduced a lot of changes which breaks backwards
 *  compatibility the non-QoS IP from Synopsys (used in the ST Micro drivers).
 *  Some fields differ between version 4.00a and 4.10a, mainly the interrupt
 *  bit fields. The driver could be made compatible with 4.00, if all relevant
 *  HW erratas are handled.
 *
 *  The GMAC is highly configurable at synthesis time. This driver has been
 *  developed for a subset of the total available feature set. Currently
 *  it supports:
 *  - TSO
 *  - Checksum offload for RX and TX.
 *  - Energy efficient ethernet.
 *  - GMII phy interface.
 *  - The statistics module.
 *  - Single RX and TX queue.
 *
 *  Copyright (C) 2015 Axis Communications AB.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms and conditions of the GNU General Public License,
 *  version 2, as published by the Free Software Foundation.
 */

#include <linux/clk.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ethtool.h>
#include <linux/stat.h>
#include <linux/types.h>

#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>

#include <linux/phy.h>
#include <linux/mii.h>
#include <linux/dma-mapping.h>
#include <linux/vmalloc.h>

#include <linux/device.h>
#include <linux/bitrev.h>
#include <linux/crc32.h>

#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/clocksource.h>
#include <linux/net_tstamp.h>
#include <linux/pm_runtime.h>
#include <linux/of_net.h>
#include <linux/of_address.h>
#include <linux/of_mdio.h>
#include <linux/of_platform.h>
#include <linux/timer.h>
#include <linux/tcp.h>
#include <linux/gpio.h>

#include <linux/mini_clock/clock.h>

#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/interrupt.h>
#include <linux/mii.h>
#include <linux/phylink.h>
#include <linux/net_tstamp.h>
#include <asm/io.h>

#include <linux/pm_wakeirq.h>



#define GMAC_QUICK_CLK 1

#define DRIVER_NAME			"dwceqos"
#define DRIVER_DESCRIPTION		"Synopsys DWC Ethernet QoS driver"
#define DRIVER_VERSION			"0.9"

#define	CHIP_PLATFORM		1
#define	HAPS_PLATFORM		2

#define	INTERNAL_PHY		0
#define	EXTERNAL_PHY		1


#define MDIO_RETRY_CNT 5
#define MDIO_RETRY_DELAY 64
#define MDIO_DATA_MASK GENMASK(15, 0)
#define TSO_EN	true
#define DWCEQOS_MSG_DEFAULT	(NETIF_MSG_DRV | NETIF_MSG_PROBE | \
	NETIF_MSG_LINK | NETIF_MSG_IFDOWN | NETIF_MSG_IFUP)

#define DWCEQOS_TX_TIMEOUT 5 /* Seconds */

#define DWCEQOS_LPI_TIMER_MIN      8
#define DWCEQOS_LPI_TIMER_MAX      ((1 << 20) - 1)

#define DWCEQOS_RX_BUF_SIZE 2048

#define DWCEQOS_RX_DCNT 1024
#define DWCEQOS_TX_DCNT 1024

#define DWCEQOS_HASH_TABLE_SIZE 64

/* The size field in the DMA descriptor is 14 bits */
#define BYTES_PER_DMA_DESC 16376

#define BIT_CKG_GMAC_RMII_PAD_SEL	1
#define BIT_CEN_APB_ANA_EPHY_REF_EN	1
#define BIT_CEN_APB_ANA_EPHY_SRST	1
#define BIT_AHB_EPHY_CFG_REF_EN	3
#define BIT_AXI_EMAC_EN	1


/* Hardware registers */
#define START_MAC_REG_OFFSET    0x0000
#define MAX_MAC_REG_OFFSET      0x0bd0
#define START_MTL_REG_OFFSET    0x0c00
#define MAX_MTL_REG_OFFSET      0x0d7c
#define START_DMA_REG_OFFSET    0x1000
#define MAX_DMA_REG_OFFSET      0x117C

#define REG_SPACE_SIZE          0x1800

/* DMA */
#define REG_DWCEQOS_DMA_MODE             0x1000
#define REG_DWCEQOS_DMA_SYSBUS_MODE      0x1004
#define REG_DWCEQOS_DMA_IS               0x1008
#define REG_DWCEQOS_DMA_DEBUG_ST0        0x100c

/* DMA channel registers */
#define REG_DWCEQOS_DMA_CH0_CTRL         0x1100
#define REG_DWCEQOS_DMA_CH0_TX_CTRL      0x1104
#define REG_DWCEQOS_DMA_CH0_RX_CTRL      0x1108
#define REG_DWCEQOS_DMA_CH0_TXDESC_LIST  0x1114
#define REG_DWCEQOS_DMA_CH0_RXDESC_LIST  0x111c
#define REG_DWCEQOS_DMA_CH0_TXDESC_TAIL  0x1120
#define REG_DWCEQOS_DMA_CH0_RXDESC_TAIL  0x1128
#define REG_DWCEQOS_DMA_CH0_TXDESC_LEN   0x112c
#define REG_DWCEQOS_DMA_CH0_RXDESC_LEN   0x1130
#define REG_DWCEQOS_DMA_CH0_IE           0x1134
#define REG_DWCEQOS_DMA_CH0_CUR_TXDESC   0x1144
#define REG_DWCEQOS_DMA_CH0_CUR_RXDESC   0x114c
#define REG_DWCEQOS_DMA_CH0_CUR_TXBUF    0x1154
#define REG_DWCEQOS_DMA_CH0_CUR_RXBUG    0x115c
#define REG_DWCEQOS_DMA_CH0_STA          0x1160

#define REG_DWCEQOS_DMA_RX_WATHCDOG          0x1138

#define DWCEQOS_DMA_MODE_TXPR            BIT(11)
#define DWCEQOS_DMA_MODE_DA              BIT(1)

#define DWCEQOS_DMA_SYSBUS_MODE_EN_LPI   BIT(31)
#define DWCEQOS_DMA_SYSBUS_MODE_FB       BIT(0)
#define DWCEQOS_DMA_SYSBUS_MODE_AAL      BIT(12)

#define DWCEQOS_DMA_SYSBUS_MODE_RD_OSR_LIMIT(x) \
	(((x) << 16) & 0x000F0000)
#define DWCEQOS_DMA_SYSBUS_MODE_RD_OSR_LIMIT_DEFAULT    3
#define DWCEQOS_DMA_SYSBUS_MODE_RD_OSR_LIMIT_MASK       GENMASK(19, 16)

#define DWCEQOS_DMA_SYSBUS_MODE_WR_OSR_LIMIT(x) \
	(((x) << 24) & 0x0F000000)
#define DWCEQOS_DMA_SYSBUS_MODE_WR_OSR_LIMIT_DEFAULT    3
#define DWCEQOS_DMA_SYSBUS_MODE_WR_OSR_LIMIT_MASK       GENMASK(27, 24)

#define DWCEQOS_DMA_SYSBUS_MODE_BURST_MASK GENMASK(7, 1)
#define DWCEQOS_DMA_SYSBUS_MODE_BURST(x) \
	(((x) << 1) & DWCEQOS_DMA_SYSBUS_MODE_BURST_MASK)
#define DWCEQOS_DMA_SYSBUS_MODE_BURST_DEFAULT   GENMASK(3, 1)

#define DWCEQOS_DMA_CH_CTRL_PBLX8       BIT(16)
#define DWCEQOS_DMA_CH_CTRL_DSL(x)      ((x) << 18)

#define DWCEQOS_DMA_CH_CTRL_PBL(x)       ((x) << 16)
#define DWCEQOS_DMA_CH_CTRL_START         BIT(0)
#define DWCEQOS_DMA_CH_RX_CTRL_BUFSIZE(x)   ((x) << 1)
#define DWCEQOS_DMA_CH_TX_OSP            BIT(4)
#define DWCEQOS_DMA_CH_TX_TSE            BIT(12)

#define DWCEQOS_DMA_CH0_IE_NIE           BIT(15)
#define DWCEQOS_DMA_CH0_IE_AIE           BIT(14)
#define DWCEQOS_DMA_CH0_IE_RIE           BIT(6)
#define DWCEQOS_DMA_CH0_IE_TIE           BIT(0)
#define DWCEQOS_DMA_CH0_IE_FBEE          BIT(12)
#define DWCEQOS_DMA_CH0_IE_RBUE          BIT(7)

#define DWCEQOS_DMA_IS_DC0IS             BIT(0)
#define DWCEQOS_DMA_IS_MTLIS             BIT(16)
#define DWCEQOS_DMA_IS_MACIS             BIT(17)

#define DWCEQOS_DMA_CH0_IS_TI            BIT(0)
#define DWCEQOS_DMA_CH0_IS_RI            BIT(6)
#define DWCEQOS_DMA_CH0_IS_RBU           BIT(7)
#define DWCEQOS_DMA_CH0_IS_FBE           BIT(12)
#define DWCEQOS_DMA_CH0_IS_CDE           BIT(13)
#define DWCEQOS_DMA_CH0_IS_AIS           BIT(14)

#define DWCEQOS_DMA_CH0_IS_TEB           GENMASK(18, 16)
#define DWCEQOS_DMA_CH0_IS_TX_ERR_READ   BIT(16)
#define DWCEQOS_DMA_CH0_IS_TX_ERR_DESCR  BIT(17)

#define DWCEQOS_DMA_CH0_IS_REB           GENMASK(21, 19)
#define DWCEQOS_DMA_CH0_IS_RX_ERR_READ   BIT(19)
#define DWCEQOS_DMA_CH0_IS_RX_ERR_DESCR  BIT(20)

/* DMA descriptor bits for RX normal descriptor (read format) */
#define DWCEQOS_DMA_RDES3_OWN     BIT(31)
#define DWCEQOS_DMA_RDES3_INTE    BIT(30)
#define DWCEQOS_DMA_RDES3_BUF2V   BIT(25)
#define DWCEQOS_DMA_RDES3_BUF1V   BIT(24)

/* DMA descriptor bits for RX normal descriptor (write back format) */
#define DWCEQOS_DMA_RDES1_IPCE    BIT(7)
#define DWCEQOS_DMA_RDES3_ES      BIT(15)
#define DWCEQOS_DMA_RDES3_E_JT    BIT(14)
#define DWCEQOS_DMA_RDES3_PL(x)   ((x) & 0x7fff)
#define DWCEQOS_DMA_RDES1_PT      0x00000007
#define DWCEQOS_DMA_RDES1_PT_UDP  BIT(0)
#define DWCEQOS_DMA_RDES1_PT_TCP  BIT(1)
#define DWCEQOS_DMA_RDES1_PT_ICMP 0x00000003

/* DMA descriptor bits for TX normal descriptor (read format) */
#define DWCEQOS_DMA_TDES2_IOC     BIT(31)
#define DWCEQOS_DMA_TDES3_OWN     BIT(31)
#define DWCEQOS_DMA_TDES3_CTXT    BIT(30)
#define DWCEQOS_DMA_TDES3_FD      BIT(29)
#define DWCEQOS_DMA_TDES3_LD      BIT(28)
#define DWCEQOS_DMA_TDES3_CIPH    BIT(16)
#define DWCEQOS_DMA_TDES3_CIPP    BIT(17)
#define DWCEQOS_DMA_TDES3_CA      0x00030000
#define DWCEQOS_DMA_TDES3_TSE     BIT(18)
#define DWCEQOS_DMA_DES3_THL(x)   ((x) << 19)
#define DWCEQOS_DMA_DES2_B2L(x)   ((x) << 16)

#define DWCEQOS_DMA_TDES3_TCMSSV    BIT(26)

/* DMA channel states */
#define DMA_CH_STOPPED   0
#define DMA_TX_CH_SUSPENDED 6
#define DMA_RX_CH_SUSPENDED 4

#define DMA_GET_TX_STATE_CH0(status0) ((status0 & 0xF000) >> 12)
#define DMA_GET_RX_STATE_CH0(status0) ((status0 & 0x0F00) >> 8)

/* MTL */
#define REG_DWCEQOS_MTL_OPER             0x0c00
#define REG_DWCEQOS_MTL_DEBUG_ST         0x0c0c
#define REG_DWCEQOS_MTL_TXQ0_DEBUG_ST    0x0d08
#define DWCEQOS_MTL_TXQ0_TRCSTS	GENMASK(2, 1)
#define MTL_GET_TXQ0_STATE(status0) ((status0 & DWCEQOS_MTL_TXQ0_TRCSTS) >> 1)
#define DWCEQOS_MTL_TXQ0_TRCSTS_IDLE 0x0
#define DWCEQOS_MTL_TXQ0_TRCSTS_READ 0x1
#define DWCEQOS_MTL_TXQ0_TRCSTS_PENDING 0x2
#define DWCEQOS_MTL_TXQ0_TRCSTS_FLUSH 0x3

#define DWCEQOS_MTL_TXQ0_TXQSTS	BIT(4)

#define REG_DWCEQOS_MTL_RXQ0_DEBUG_ST    0x0d38
#define DWCEQOS_MTL_RXQ0_RXQSTS	GENMASK(5, 4)
#define MTL_GET_RXQ0_RXQSTS(status0) ((status0 & DWCEQOS_MTL_RXQ0_RXQSTS) >> 4)
#define DWCEQOS_MTL_RXQ0_RXQSTS_EMPTY 0x0
#define DWCEQOS_MTL_TXQ0_TRCSTS_BELOW 0x1
#define DWCEQOS_MTL_TXQ0_TRCSTS_ABOVE 0x2
#define DWCEQOS_MTL_TXQ0_TRCSTS_FULL 0x3

#define DWCEQOS_MTL_RXQ0_PRXQ	GENMASK(29, 16)
#define MTL_GET_RXQ0_PRXQ(status0) ((status0 & DWCEQOS_MTL_RXQ0_PRXQ) >> 16)

#define REG_DWCEQOS_MTL_IS               0x0c20
#define REG_DWCEQOS_MTL_TXQ0_OPER        0x0d00
#define REG_DWCEQOS_MTL_RXQ0_OPER        0x0d30
#define REG_DWCEQOS_MTL_RXQ0_MIS_CNT     0x0d34
#define REG_DWCEQOS_MTL_RXQ0_CTRL         0x0d3c

#define REG_DWCEQOS_MTL_Q0_ISCTRL         0x0d2c

#define DWCEQOS_MTL_SCHALG_STRICT        0x00000060

#define DWCEQOS_MTL_TXQ_TXQEN            BIT(3)
#define DWCEQOS_MTL_TXQ_TSF              BIT(1)
#define DWCEQOS_MTL_TXQ_FTQ              BIT(0)
#define DWCEQOS_MTL_TXQ_TTC512           0x00000070

#define DWCEQOS_MTL_TXQ_SIZE(x)          ((((x) - 256) & 0xff00) << 8)

#define DWCEQOS_MTL_RXQ_SIZE(x)          ((((x) - 256) & 0xff00) << 12)
#define DWCEQOS_MTL_RXQ_EHFC             BIT(7)
#define DWCEQOS_MTL_RXQ_DIS_TCP_EF       BIT(6)
#define DWCEQOS_MTL_RXQ_FEP              BIT(4)
#define DWCEQOS_MTL_RXQ_FUP              BIT(3)
#define DWCEQOS_MTL_RXQ_RSF              BIT(5)
#define DWCEQOS_MTL_RXQ_RTC32            BIT(0)
#define DWCEQOS_MTL_RXQ_RTC128           GENMASK(1, 0)

/* MAC */
#define REG_DWCEQOS_MAC_CFG              0x0000
#define REG_DWCEQOS_MAC_EXT_CFG          0x0004
#define REG_DWCEQOS_MAC_PKT_FILT         0x0008
#define REG_DWCEQOS_MAC_WD_TO            0x000c
#define REG_DWCEQOS_HASTABLE_LO          0x0010
#define REG_DWCEQOS_HASTABLE_HI          0x0014
#define REG_DWCEQOS_MAC_IS               0x00b0
#define REG_DWCEQOS_MAC_IE               0x00b4
#define REG_DWCEQOS_MAC_STAT             0x00b8
#define REG_DWCEQOS_MAC_MDIO_ADDR        0x0200
#define REG_DWCEQOS_MAC_MDIO_DATA        0x0204
#define REG_DWCEQOS_MAC_MAC_ADDR0_HI     0x0300
#define REG_DWCEQOS_MAC_MAC_ADDR0_LO     0x0304
#define REG_DWCEQOS_MAC_RXQ0_CTRL0       0x00a0
#define REG_DWCEQOS_MAC_HW_FEATURE0      0x011c
#define REG_DWCEQOS_MAC_HW_FEATURE1      0x0120
#define REG_DWCEQOS_MAC_HW_FEATURE2      0x0124
#define REG_DWCEQOS_MAC_HASHTABLE_LO     0x0010
#define REG_DWCEQOS_MAC_HASHTABLE_HI     0x0014
#define REG_DWCEQOS_MAC_LPI_CTRL_STATUS  0x00d0
#define REG_DWCEQOS_MAC_LPI_TIMERS_CTRL  0x00d4
#define REG_DWCEQOS_MAC_LPI_ENTRY_TIMER  0x00d8
#define REG_DWCEQOS_MAC_1US_TIC_COUNTER  0x00dc
#define REG_DWCEQOS_MAC_RX_FLOW_CTRL     0x0090
#define REG_DWCEQOS_MAC_PMT_CTRL_STATUS     0x00C0

#define REG_DWCEQOS_MAC_Q0_TX_FLOW	 0x0070

#define GMAC_HW_FEAT_MGKSEL		BIT(7)
#define GMAC_HW_FEAT_RWKSEL		BIT(6)


#define DWCEQOS_MAC_CFG_ACS              BIT(20)
#define DWCEQOS_MAC_CFG_JD               BIT(17)
#define DWCEQOS_MAC_CFG_JE               BIT(16)
#define DWCEQOS_MAC_CFG_PS               BIT(15)
#define DWCEQOS_MAC_CFG_FES              BIT(14)
#define DWCEQOS_MAC_CFG_DM               BIT(13)
#define DWCEQOS_MAC_CFG_DO               BIT(10)
#define DWCEQOS_MAC_CFG_DCRS             BIT(9)
#define DWCEQOS_MAC_CFG_TE               BIT(1)
#define DWCEQOS_MAC_CFG_IPC              BIT(27)
#define DWCEQOS_MAC_CFG_RE               BIT(0)

#define DWCEQOS_ADDR_HIGH(reg)           (0x00000300 + (reg * 8))
#define DWCEQOS_ADDR_LOW(reg)            (0x00000304 + (reg * 8))

#define DWCEQOS_MAC_IS_PMTIS_INT           BIT(4)
#define DWCEQOS_MAC_IS_LPI_INT           BIT(5)
#define DWCEQOS_MAC_IS_MMC_INT           BIT(8)
#define DWCEQOS_MAC_IS_RXIPIS_INT        BIT(11)
#define DWCEQOS_MAC_RXQ_EN               BIT(1)
#define DWCEQOS_MAC_MAC_ADDR_HI_EN       BIT(31)
#define DWCEQOS_MAC_PKT_FILT_RA          BIT(31)
#define DWCEQOS_MAC_PKT_FILT_HPF         BIT(10)
#define DWCEQOS_MAC_PKT_FILT_SAF         BIT(9)
#define DWCEQOS_MAC_PKT_FILT_SAIF        BIT(8)
#define DWCEQOS_MAC_PKT_FILT_DBF         BIT(5)
#define DWCEQOS_MAC_PKT_FILT_PM          BIT(4)
#define DWCEQOS_MAC_PKT_FILT_DAIF        BIT(3)
#define DWCEQOS_MAC_PKT_FILT_HMC         BIT(2)
#define DWCEQOS_MAC_PKT_FILT_HUC         BIT(1)
#define DWCEQOS_MAC_PKT_FILT_PR          BIT(0)

#define DWCEQOS_MAC_MDIO_ADDR_CR(x)      (((x & 15)) << 8)
#define DWCEQOS_MAC_MDIO_ADDR_CR_20      2
#define DWCEQOS_MAC_MDIO_ADDR_CR_35      3
#define DWCEQOS_MAC_MDIO_ADDR_CR_60      0
#define DWCEQOS_MAC_MDIO_ADDR_CR_100     1
#define DWCEQOS_MAC_MDIO_ADDR_CR_150     4
#define DWCEQOS_MAC_MDIO_ADDR_CR_250     5
#define DWCEQOS_MAC_MDIO_ADDR_GOC_READ   0x0000000c
#define DWCEQOS_MAC_MDIO_ADDR_GOC_WRITE  BIT(2)
#define DWCEQOS_MAC_MDIO_ADDR_GB         BIT(0)

#define DWCEQOS_MAC_LPI_CTRL_STATUS_TLPIEN  BIT(0)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_TLPIEX  BIT(1)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_RLPIEN  BIT(2)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_RLPIEX  BIT(3)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_TLPIST  BIT(8)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_RLPIST  BIT(9)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_LPIEN   BIT(16)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_PLS     BIT(17)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_PLSEN   BIT(18)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_LIPTXA  BIT(19)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_LPITE   BIT(20)
#define DWCEQOS_MAC_LPI_CTRL_STATUS_LPITCSE BIT(21)

#define DWCEQOS_MAC_1US_TIC_COUNTER_VAL(x)  ((x) & GENMASK(11, 0))

#define DWCEQOS_LPI_CTRL_ENABLE_EEE      (DWCEQOS_MAC_LPI_CTRL_STATUS_LPITE | \
					  DWCEQOS_MAC_LPI_CTRL_STATUS_LIPTXA | \
					  DWCEQOS_MAC_LPI_CTRL_STATUS_LPIEN)

#define DWCEQOS_MAC_RX_FLOW_CTRL_RFE BIT(0)

#define DWCEQOS_MAC_Q0_TX_FLOW_TFE   BIT(1)
#define DWCEQOS_MAC_Q0_TX_FLOW_PT(time)	((time) << 16)
#define DWCEQOS_MAC_Q0_TX_FLOW_PLT_4_SLOTS (0 << 4)

/* Features */
#define DWCEQOS_MAC_HW_FEATURE0_RXCOESEL BIT(16)
#define DWCEQOS_MAC_HW_FEATURE0_TXCOESEL BIT(14)
#define DWCEQOS_MAC_HW_FEATURE0_HDSEL    BIT(2)
#define DWCEQOS_MAC_HW_FEATURE0_EEESEL   BIT(13)
#define DWCEQOS_MAC_HW_FEATURE0_GMIISEL  BIT(1)
#define DWCEQOS_MAC_HW_FEATURE0_MIISEL   BIT(0)

#define DWCEQOS_MAC_HW_FEATURE1_TSOEN    BIT(18)
#define DWCEQOS_MAC_HW_FEATURE1_TXFIFOSIZE(x) ((128 << ((x) & 0x7c0)) >> 6)
#define DWCEQOS_MAC_HW_FEATURE1_RXFIFOSIZE(x)  (128 << ((x) & 0x1f))

#define DWCEQOS_MAX_PERFECT_ADDRESSES(feature1) \
	(1 + (((feature1) & 0x1fc0000) >> 18))

#define DWCEQOS_MDIO_PHYADDR(x)     (((x) & 0x1f) << 21)
#define DWCEQOS_MDIO_PHYREG(x)      (((x) & 0x1f) << 16)

#define DWCEQOS_DMA_MODE_SWR            BIT(0)

#define DWCEQOS_DWCEQOS_RX_BUF_SIZE 2048

/* Mac Management Counters */
#define REG_DWCEQOS_MMC_CTRL             0x0700
#define REG_DWCEQOS_MMC_RXIRQ            0x0704
#define REG_DWCEQOS_MMC_TXIRQ            0x0708
#define REG_DWCEQOS_MMC_RXIRQMASK        0x070c
#define REG_DWCEQOS_MMC_TXIRQMASK        0x0710

#define DWCEQOS_MMC_CTRL_CNTRST          BIT(0)
#define DWCEQOS_MMC_CTRL_RSTONRD         BIT(2)

#define DWC_MMC_TXLPITRANSCNTR           0x07F0
#define DWC_MMC_TXLPIUSCNTR              0x07EC
#define DWC_MMC_TXOVERSIZE_G             0x0778
#define DWC_MMC_TXVLANPACKETS_G          0x0774
#define DWC_MMC_TXPAUSEPACKETS           0x0770
#define DWC_MMC_TXEXCESSDEF              0x076C
#define DWC_MMC_TXPACKETCOUNT_G          0x0768
#define DWC_MMC_TXOCTETCOUNT_G           0x0764
#define DWC_MMC_TXCARRIERERROR           0x0760
#define DWC_MMC_TXEXCESSCOL              0x075C
#define DWC_MMC_TXLATECOL                0x0758
#define DWC_MMC_TXDEFERRED               0x0754
#define DWC_MMC_TXMULTICOL_G             0x0750
#define DWC_MMC_TXSINGLECOL_G            0x074C
#define DWC_MMC_TXUNDERFLOWERROR         0x0748
#define DWC_MMC_TXBROADCASTPACKETS_GB    0x0744
#define DWC_MMC_TXMULTICASTPACKETS_GB    0x0740
#define DWC_MMC_TXUNICASTPACKETS_GB      0x073C
#define DWC_MMC_TX1024TOMAXOCTETS_GB     0x0738
#define DWC_MMC_TX512TO1023OCTETS_GB     0x0734
#define DWC_MMC_TX256TO511OCTETS_GB      0x0730
#define DWC_MMC_TX128TO255OCTETS_GB      0x072C
#define DWC_MMC_TX65TO127OCTETS_GB       0x0728
#define DWC_MMC_TX64OCTETS_GB            0x0724
#define DWC_MMC_TXMULTICASTPACKETS_G     0x0720
#define DWC_MMC_TXBROADCASTPACKETS_G     0x071C
#define DWC_MMC_TXPACKETCOUNT_GB         0x0718
#define DWC_MMC_TXOCTETCOUNT_GB          0x0714

#define DWC_MMC_RXLPITRANSCNTR           0x07F8
#define DWC_MMC_RXLPIUSCNTR              0x07F4
#define DWC_MMC_RXCTRLPACKETS_G          0x07E4
#define DWC_MMC_RXRCVERROR               0x07E0
#define DWC_MMC_RXWATCHDOG               0x07DC
#define DWC_MMC_RXVLANPACKETS_GB         0x07D8
#define DWC_MMC_RXFIFOOVERFLOW           0x07D4
#define DWC_MMC_RXPAUSEPACKETS           0x07D0
#define DWC_MMC_RXOUTOFRANGETYPE         0x07CC
#define DWC_MMC_RXLENGTHERROR            0x07C8
#define DWC_MMC_RXUNICASTPACKETS_G       0x07C4
#define DWC_MMC_RX1024TOMAXOCTETS_GB     0x07C0
#define DWC_MMC_RX512TO1023OCTETS_GB     0x07BC
#define DWC_MMC_RX256TO511OCTETS_GB      0x07B8
#define DWC_MMC_RX128TO255OCTETS_GB      0x07B4
#define DWC_MMC_RX65TO127OCTETS_GB       0x07B0
#define DWC_MMC_RX64OCTETS_GB            0x07AC
#define DWC_MMC_RXOVERSIZE_G             0x07A8
#define DWC_MMC_RXUNDERSIZE_G            0x07A4
#define DWC_MMC_RXJABBERERROR            0x07A0
#define DWC_MMC_RXRUNTERROR              0x079C
#define DWC_MMC_RXALIGNMENTERROR         0x0798
#define DWC_MMC_RXCRCERROR               0x0794
#define DWC_MMC_RXMULTICASTPACKETS_G     0x0790
#define DWC_MMC_RXBROADCASTPACKETS_G     0x078C
#define DWC_MMC_RXOCTETCOUNT_G           0x0788
#define DWC_MMC_RXOCTETCOUNT_GB          0x0784
#define DWC_MMC_RXPACKETCOUNT_GB         0x0780

#define REG_DWCEQOS_IPC_RXMASK      	 0x0800
#define REG_DWCEQOS_IPC_RXINT      	 	 0x0808
#define	REG_DWCEQOS_IPC_RXICMPERO		 0x0884
#define	REG_DWCEQOS_IPC_RXICMPGO		 0x0880
#define	REG_DWCEQOS_IPC_RXTCPERO 		 0x087c
#define	REG_DWCEQOS_IPC_RXTCPGO			 0x0878
#define	REG_DWCEQOS_IPC_RXUDPERO		 0x0874
#define	REG_DWCEQOS_IPC_RXUDPO			 0x0870
#define	REG_DWCEQOS_IPC_RXIPV6NOPAYO	 0x086c
#define	REG_DWCEQOS_IPC_RXIPV6HERO		 0x0868
#define	REG_DWCEQOS_IPC_RXIPV6GO		 0x0864
#define	REG_DWCEQOS_IPC_RXIPV4UDSBLO	 0x0860
#define	REG_DWCEQOS_IPC_RXIPV4FRAO		 0x085c
#define	REG_DWCEQOS_IPC_RXIPV4NOPAYO	 0x0858
#define	REG_DWCEQOS_IPC_RXIPV4HERO		 0x0854
#define	REG_DWCEQOS_IPC_RXIPV4GO		 0x0850
#define	REG_DWCEQOS_IPC_RXICMPERP		 0x0844
#define	REG_DWCEQOS_IPC_RXICMPGP		 0x0840
#define	REG_DWCEQOS_IPC_RXTCPERP		 0x083c
#define	REG_DWCEQOS_IPC_RXTCPGP			 0x0838
#define	REG_DWCEQOS_IPC_RXUDPERP		 0x0834
#define	REG_DWCEQOS_IPC_RXUDPGP			 0x0830
#define	REG_DWCEQOS_IPC_RXIPV6NOPAYP	 0x082c
#define	REG_DWCEQOS_IPC_RXIPV6HERP		 0x0828
#define	REG_DWCEQOS_IPC_RXIPV6GP		 0x0824
#define	REG_DWCEQOS_IPC_RXIPV4UDSBLP	 0x0820
#define	REG_DWCEQOS_IPC_RXIPV4FRAGP		 0x081c
#define	REG_DWCEQOS_IPC_RXIPV4NOPAYP	 0x0818
#define	REG_DWCEQOS_IPC_RXIPV4HERP		 0x0814
#define	REG_DWCEQOS_IPC_RXIPV4P			 0x0810

/* For Internal PHY(100T) */
#define	REG_ANA_REG			 0x90
#define	REG_ANA_PHY_CLK			 BIT(11)

/* For External GPHY */
#define MII_PAGE		0x1f
#define MII_PHYSR_PAGE		0xa43
#define MII_PHYSR		0x1a
#define MII_PHYSR_CABLE	BIT(13)

#define GPHY_CABLE_TIME_OUT	20
#define GPHY_LINK_TIME_OUT	300

#define REG_DWCEQOS_DMA_CH0_CTRL_MSS(val) ((u32)(val) & GENMASK(13,0))
//#define TCP_FRAG_SIZE 4096
#define TCP_FRAG_SIZE 16376

/**dpll config***/
#define REG_GMAC_TXDLL_CTRL0 ((void __iomem *)(lp->dpll_addr_map.txdpll_base + 0x00))
#define REG_GMAC_TXDLL_CTRL1 ((void __iomem *)(lp->dpll_addr_map.txdpll_base + 0x04))
#define REG_GMAC_TXDLL_CTRL2 ((void __iomem *)(lp->dpll_addr_map.txdpll_base + 0x08))
#define REG_GMAC_TXDLL_CTRL3 ((void __iomem *)(lp->dpll_addr_map.txdpll_base + 0x0c))

#define REG_GMAC_RXDLL_CTRL0 ((void __iomem *)(lp->dpll_addr_map.rxdpll_base + 0x00))
#define REG_GMAC_RXDLL_CTRL1 ((void __iomem *)(lp->dpll_addr_map.rxdpll_base + 0x04))
#define REG_GMAC_RXDLL_CTRL2 ((void __iomem *)(lp->dpll_addr_map.rxdpll_base + 0x08))
#define REG_GMAC_RXDLL_CTRL3 ((void __iomem *)(lp->dpll_addr_map.rxdpll_base + 0x0c))


#define REG_GMAC_RXDLL_ECO0 ((void __iomem *)(lp->dpll_addr_map.eco0_base))
#define REG_GMAC_RXDLL_ECO1 ((void __iomem *)(lp->dpll_addr_map.eco1_base))

/*because gmac0 and gmac1 use two eco regs Interleaved*/
static void __iomem * default_eco0_base = NULL;
static void __iomem * default_eco1_base = NULL;
#define dll_fixup_addr (0x34400400)

#define FH_GMAC_PHY_INTERNAL_V2 0x46480000

static int debug = -1;
module_param(debug, int, 0);
MODULE_PARM_DESC(debug, "DWC_eth_qos debug level (0=none,...,16=all)");

/* DMA ring descriptor. These are used as support descriptors for the HW DMA */
struct ring_desc {
	struct sk_buff *skb;
	dma_addr_t mapping;
	size_t len;
};

/* DMA hardware descriptor */
struct dwceqos_dma_desc {
	u32	des0;
	u32	des1;
	u32	des2;
	u32	des3;
}__attribute__((__aligned__(32)));

struct dwceqos_mmc_counters {
	__u64 txlpitranscntr;
	__u64 txpiuscntr;
	__u64 txoversize_g;
	__u64 txvlanpackets_g;
	__u64 txpausepackets;
	__u64 txexcessdef;
	__u64 txpacketcount_g;
	__u64 txoctetcount_g;
	__u64 txcarriererror;
	__u64 txexcesscol;
	__u64 txlatecol;
	__u64 txdeferred;
	__u64 txmulticol_g;
	__u64 txsinglecol_g;
	__u64 txunderflowerror;
	__u64 txbroadcastpackets_gb;
	__u64 txmulticastpackets_gb;
	__u64 txunicastpackets_gb;
	__u64 tx1024tomaxoctets_gb;
	__u64 tx512to1023octets_gb;
	__u64 tx256to511octets_gb;
	__u64 tx128to255octets_gb;
	__u64 tx65to127octets_gb;
	__u64 tx64octets_gb;
	__u64 txmulticastpackets_g;
	__u64 txbroadcastpackets_g;
	__u64 txpacketcount_gb;
	__u64 txoctetcount_gb;

	__u64 rxlpitranscntr;
	__u64 rxlpiuscntr;
	__u64 rxctrlpackets_g;
	__u64 rxrcverror;
	__u64 rxwatchdog;
	__u64 rxvlanpackets_gb;
	__u64 rxfifooverflow;
	__u64 rxpausepackets;
	__u64 rxoutofrangetype;
	__u64 rxlengtherror;
	__u64 rxunicastpackets_g;
	__u64 rx1024tomaxoctets_gb;
	__u64 rx512to1023octets_gb;
	__u64 rx256to511octets_gb;
	__u64 rx128to255octets_gb;
	__u64 rx65to127octets_gb;
	__u64 rx64octets_gb;
	__u64 rxoversize_g;
	__u64 rxundersize_g;
	__u64 rxjabbererror;
	__u64 rxrunterror;
	__u64 rxalignmenterror;
	__u64 rxcrcerror;
	__u64 rxmulticastpackets_g;
	__u64 rxbroadcastpackets_g;
	__u64 rxoctetcount_g;
	__u64 rxoctetcount_gb;
	__u64 rxpacketcount_gb;

	__u64 rxicmpero;
	__u64 rxicmpgo;
	__u64 rxtcpero;
	__u64 rxtcpgo;
	__u64 rxudpero;
	__u64 rxudpo;
	__u64 rxipv6nopayo;
	__u64 rxipv6hero;
	__u64 rxipv6go;
	__u64 rxipv4udsblo;
	__u64 rxipv4frao;
	__u64 rxipv4nopayo;
	__u64 rxipv4hero;
	__u64 rxipv4go;
	__u64 rxicmperp;
	__u64 rxicmpgp;
	__u64 rxtcperp;
	__u64 rxtcpgp;
	__u64 rxudperp;
	__u64 rxudpgp;
	__u64 rxipv6nopayp;
	__u64 rxipv6herp;
	__u64 rxipv6gp;
	__u64 rxipv4udsblp;
	__u64 rxipv4fragp;
	__u64 rxipv4nopayp;
	__u64 rxipv4herp;
	__u64 rxipv4p;

};

/* Ethtool statistics */

struct dwceqos_stat {
	const char stat_name[ETH_GSTRING_LEN];
	int   offset;
};

#define STAT_ITEM(name, var) \
	{\
		name,\
		offsetof(struct dwceqos_mmc_counters, var),\
	}

static const struct dwceqos_stat dwceqos_ethtool_stats[] = {

	STAT_ITEM("tx_bytes_gb",txoctetcount_gb),
	STAT_ITEM("tx_bytes_g",txoctetcount_g),
	STAT_ITEM("tx_pkts_gb",txpacketcount_gb),
	STAT_ITEM("tx_pkts_g",txpacketcount_g),
	STAT_ITEM("tx_unicst_pkts_gb",txunicastpackets_gb),
	STAT_ITEM("tx_excessive_collision_pkts",txexcesscol),
	STAT_ITEM("tx_late_collision_pkts",txlatecol),
	STAT_ITEM("tx_deferred_pkts_g",txdeferred),
	STAT_ITEM("tx_multiple_collision_pkts_g",txmulticol_g),
	STAT_ITEM("tx_single_collision_pkts_g",txsinglecol_g),
	STAT_ITEM("tx_lpi_microsecond_timer",txpiuscntr),
	STAT_ITEM("tx_Osize_pkts_g",txoversize_g),
	STAT_ITEM("tx_vlan_pkts_g",txvlanpackets_g),
	STAT_ITEM("tx_broadcast_pkts_gb",txbroadcastpackets_gb),
	STAT_ITEM("tx_broadcast_pkts_g",txbroadcastpackets_g),
	STAT_ITEM("tx_multicast_pkts_gb",txmulticastpackets_gb),
	STAT_ITEM("tx_multicast_pkts_g",txmulticastpackets_g),
	STAT_ITEM("tx_up_to_64_byte_pkts_gb",tx64octets_gb),
	STAT_ITEM("tx_65_to_127_byte_pkts_gb",tx65to127octets_gb),
	STAT_ITEM("tx_128_to_255_byte_pkts_gb",tx128to255octets_gb),
	STAT_ITEM("tx_256_to_511_byte_pkts_gb",tx256to511octets_gb),
	STAT_ITEM("tx_512_to_1023_byte_pkts_gb",tx512to1023octets_gb),
	STAT_ITEM("tx_1024_to_maxsize_pkts_gb",tx1024tomaxoctets_gb),


	STAT_ITEM("tx_pause_pkts",txpausepackets),
	STAT_ITEM("tx_underflow_errors",txunderflowerror),
	STAT_ITEM("tx_carrier_error_pkts",txcarriererror),
	STAT_ITEM("tx_excessive_defer_error_pkts",txexcessdef),
	STAT_ITEM("tx_low_power_idle_count",txlpitranscntr),
	STAT_ITEM("rx_bytes_gb",rxoctetcount_gb),
	STAT_ITEM("rx_bytes_g",rxoctetcount_g),
	STAT_ITEM("rx_pkts_gb",rxpacketcount_gb),
	STAT_ITEM("rx_unicast_pkts_g",rxunicastpackets_g),
	STAT_ITEM("rx_broadcast_pkts_g",rxbroadcastpackets_g),
	STAT_ITEM("rx_multicast_pkts_g",rxmulticastpackets_g),
	STAT_ITEM("rx_oversize_pkts_g",rxoversize_g),
	STAT_ITEM("rx_undersize_pkts_g",rxundersize_g),
	STAT_ITEM("rx_control_pkts_g",rxctrlpackets_g),
	STAT_ITEM("rx_vlan_pkts_gb",rxvlanpackets_gb),
	STAT_ITEM("rx_up_to_64_byte_pkts_gb",rx64octets_gb),
	STAT_ITEM("rx_65_to_127_byte_pkts_gb",rx65to127octets_gb),
	STAT_ITEM("rx_128_to_255_byte_pkts_gb",rx128to255octets_gb),
	STAT_ITEM("rx_256_to_511_byte_pkts_gb",rx256to511octets_gb),
	STAT_ITEM("rx_512_to_1023_byte_pkts_gb",rx512to1023octets_gb),
	STAT_ITEM("rx_1024_to_maxsize_pkts_gb",rx1024tomaxoctets_gb),
	STAT_ITEM("rx_pause_pkts",rxpausepackets),
	STAT_ITEM("rx_out_of_range_pkts",rxoutofrangetype),
	STAT_ITEM("rx_fifo_overflow_pkts",rxfifooverflow),
	STAT_ITEM("rx_jabber_error_pkts",rxjabbererror),
	STAT_ITEM("rx_runt_error_pkts",rxrunterror),
	STAT_ITEM("rx_alignment_error_pkts",rxalignmenterror),
	STAT_ITEM("rx_crc_error_pkts",rxcrcerror),
	STAT_ITEM("rx_length_error_pkts",rxlengtherror),
	STAT_ITEM("rx_receive_error_pkts",rxrcverror),
	STAT_ITEM("rx_watchdog_error_pkts",rxwatchdog),
	STAT_ITEM("rx_low_power_idle_count",rxlpitranscntr),
	STAT_ITEM("rx_lpi_microsecond_timer",rxlpiuscntr),
	STAT_ITEM("rx_imcp_error_bytes",rxicmpero),
	STAT_ITEM("rx_imcp_bytes_g",rxicmpgo),
	STAT_ITEM("rx_tcp_error_bytes",rxtcpero),
	STAT_ITEM("rx_tcp_bytes_g",rxtcpgo),
	STAT_ITEM("rx_udp_error_bytes",rxudpero),
	STAT_ITEM("rx_udp_bytes_g",rxudpo),
	STAT_ITEM("rx_ipv6_no_payload_bytes",rxipv6nopayo),
	STAT_ITEM("rx_ipv6_header_error_bytes",rxipv6hero),
	STAT_ITEM("rx_ipv6_bytes_g",rxipv6go),
	STAT_ITEM("rx_ipv4_udp_checksum_dis_bytes_g",rxipv4udsblo),
	STAT_ITEM("rx_ipv4_fragmented_bytes_g",rxipv4frao),
	STAT_ITEM("rx_ipv4_no_payload_bytes",rxipv4nopayo),
	STAT_ITEM("rx_ipv4_header_error_bytes",rxipv4hero),
	STAT_ITEM("rx_ipv4_bytes_g",rxipv4go),
	STAT_ITEM("rx_imcp_error_pkts",rxicmperp),
	STAT_ITEM("rx_imcp_pkts_g",rxicmpgp),
	STAT_ITEM("rx_tcp_error_pkts",rxtcperp),
	STAT_ITEM("rx_tcp_pkts_g",rxtcpgp),
	STAT_ITEM("rx_udp_error_pkts",rxudperp),
	STAT_ITEM("rx_udp_pkts_g",rxudpgp),
	STAT_ITEM("rx_ipv6_no_payload_pkts",rxipv6nopayp),
	STAT_ITEM("rx_ipv6_header_error_pkts",rxipv6herp),
	STAT_ITEM("rx_ipv6_pkts_g",rxipv6gp),
	STAT_ITEM("rx_ipv4_udp_checksum_dis_pkts_g",rxipv4udsblp),
	STAT_ITEM("rx_ipv4_fragmented_pkts_g",rxipv4fragp),
	STAT_ITEM("rx_ipv4_no_payload_pkts",rxipv4nopayp),
	STAT_ITEM("rx_ipv4_header_error_pkts",rxipv4herp),
	STAT_ITEM("rx_ipv4_pkts_g",rxipv4p),

};

/* Configuration of AXI bus parameters.
 * These values depend on the parameters set on the MAC core as well
 * as the AXI interconnect.
 */
struct dwceqos_bus_cfg {
	/* Enable AXI low-power interface. */
	bool en_lpi;
	/* enable clk rmii out inv */
	bool en_rmii_out_inv;
	/* Limit on number of outstanding AXI write requests. */
	u32 write_requests;
	/* Limit on number of outstanding AXI read requests. */
	u32 read_requests;
	/* Bitmap of allowed AXI burst lengths, 4-256 beats. */
	u32 burst_map;
	/* DMA Programmable burst length*/
	u32 tx_pbl;
	u32 rx_pbl;
	u32 rx_gro;
};

struct dwceqos_flowcontrol {
	int autoneg;
	int rx;
	int rx_current;
	int tx;
	int tx_current;
};

struct dwceqos_reg_cfg{
	u32 reg_offset;
	u32 bit_offset;
	u32 bit_mask;
};

struct reg_cfg_val{
	u32 reg_offset;
	u32 bit_offset;
	u32 bit_mask;
	u32 bit_val;
};

enum{
	MII_INTF = 0,
	RGMII_INTF = 1,
	RMII_INTF = 4,
};
struct dpll_addr_map {
	void __iomem * txdpll_base;
	void __iomem * rxdpll_base;
	void __iomem * negrxdpll_base;
	void __iomem * eco0_base;
	void __iomem * eco1_base;
	bool dll_addr_is_fixup;
};

struct gmac_cap {
        bool has_txrx_delay;
        u32 dpll_version;
};

struct net_local {
	void __iomem *baseaddr;
	struct regmap *apahb_base;
	struct regmap *apapb_base;
	struct regmap *glapb_base;
	struct regmap *ana;
	struct regmap *pinconf;
	struct clk *apahb_gate;
	struct clk *apapb_gate;
	struct clk *apb_pclk;
	struct clk *tx_en;
	struct clk *rx_en;
	struct clk *rmii_en;
	struct clk *ephy_ref_en;
	u32 default_apb_pclk;

	struct device_node *phy_node;
	struct net_device *ndev;
	struct platform_device *pdev;

	u32 msg_enable;

	struct tasklet_struct tx_bdreclaim_tasklet;
	struct workqueue_struct *txtimeout_handler_wq;
	struct work_struct txtimeout_reinit;

	unsigned int platform;
	unsigned int phy_type;
	phy_interface_t phy_interface;
	struct mii_bus *mii_bus;

	unsigned int link;
	unsigned int speed;
	unsigned int duplex;

	struct napi_struct napi;

	/* DMA Descriptor Areas */
	struct ring_desc *rx_skb;
	struct ring_desc *tx_skb;

	struct dwceqos_dma_desc *tx_descs;
	struct dwceqos_dma_desc *rx_descs;

	/* DMA Mapped Descriptor areas*/
	dma_addr_t tx_descs_addr;
	dma_addr_t rx_descs_addr;
	dma_addr_t tx_descs_tail_addr;
	dma_addr_t rx_descs_tail_addr;

	size_t tx_free;
	size_t tx_next;
	size_t rx_cur;
	size_t tx_cur;

	/* Spinlocks for accessing DMA Descriptors */
	spinlock_t tx_lock;

	/* Spinlock for register read-modify-writes. */
	spinlock_t hw_lock;

	u32 feature0;
	u32 feature1;
	u32 feature2;

	struct dwceqos_bus_cfg bus_cfg;
	bool en_tx_lpi_clockgating;

	int eee_enabled;
	int eee_active;
	int csr_val;
	u32 gso_size;

	struct dwceqos_mmc_counters mmc_counters;
	/* Protect the mmc_counter updates. */
	spinlock_t stats_lock;
	u32 mmc_rx_counters_mask;
	u32 mmc_tx_counters_mask;
	u32 mmc_ipc_counters_mask;

	struct dwceqos_flowcontrol flowcontrol;

	/* 0x00-0xff,high half byte is board_type
	 * low half byte is controller number
	 * 0x10---fy10p
	 *
	 */
	int ctrl_id;
	/* Tracks the intermediate state of phy started but hardware
	 * init not finished yet.
	 */
	bool phy_defer;
	bool mac_suspend;
	u32 rx_irq_watchdog;
	u32 rx_irq_watchdog_old;
	int tx_delay;
	int rx_delay;
	int	negrx_delay;
	struct dpll_addr_map dpll_addr_map;
	struct gmac_cap gmac_cap;

	//remote wakeup
	int pmt;
	int wolopts;
	int wol_irq;
	int lpi_irq;
	int irq_wake;
	unsigned int pmt_remote_wake_up;
	unsigned int pmt_magic_frame;

};

/* MAC PMT bitmap */
enum power_event {
	pointer_reset =	0x80000000,
	global_unicast = 0x00000200,
	wake_up_rx_frame = 0x00000040,
	magic_frame = 0x00000020,
	wake_up_frame_en = 0x00000004,
	magic_pkt_en = 0x00000002,
	power_down = 0x00000001,
};

struct dwceqos_drv_data {
	int	(*clk_init) (struct net_local *lp);
	int	(*ctl_reset) (struct net_local *lp);
};

static void dwceqos_read_mmc_counters(struct net_local *lp, u32 rx_mask,
				      u32 tx_mask, u32 lpc_mask);

static void dwceqos_set_umac_addr(struct net_local *lp, unsigned char *addr,
				  unsigned int reg_n);
static int dwceqos_stop(struct net_device *ndev);
static int dwceqos_open(struct net_device *ndev);
static void dwceqos_tx_poll_demand(struct net_local *lp);

static void dwceqos_set_rx_flowcontrol(struct net_local *lp, bool enable);
static void dwceqos_set_tx_flowcontrol(struct net_local *lp, bool enable);

static void dwceqos_reset_state(struct net_local *lp);

#define dwceqos_read(lp, reg)						\
	readl_relaxed(((void __iomem *)((lp)->baseaddr)) + (reg))
#define dwceqos_write(lp, reg, val)					\
	writel_relaxed((val), ((void __iomem *)((lp)->baseaddr)) + (reg))

static void dwceqos_reset_state(struct net_local *lp)
{
	lp->link    = 0;
	lp->speed   = 0;
	lp->duplex  = DUPLEX_UNKNOWN;
	lp->flowcontrol.rx_current = 0;
	lp->flowcontrol.tx_current = 0;
	lp->eee_active = 0;
	lp->eee_enabled = 0;
}

static void print_descriptor(struct net_local *lp, int index, int tx)
{
	struct dwceqos_dma_desc *dd;

	if (tx)
		dd = (struct dwceqos_dma_desc *)&lp->tx_descs[index];
	else
		dd = (struct dwceqos_dma_desc *)&lp->rx_descs[index];

        if (tx)
                printk(KERN_DEBUG "%s DMA Descriptor #%d@%p--%x Contents:\n", tx ? "TX" : "RX",
                        index, dd, (lp->tx_descs_addr + index * sizeof(struct dwceqos_dma_desc)));
        else
                printk(KERN_DEBUG "%s DMA Descriptor #%d@%p--%x Contents:\n", tx ? "TX" : "RX",
                        index, dd, (lp->rx_descs_addr + index * sizeof(struct dwceqos_dma_desc)));

	pr_info("0x%08x 0x%08x 0x%08x 0x%08x\n", dd->des0, dd->des1, dd->des2,
		dd->des3);
}

static void print_status(struct net_local *lp)
{
	size_t desci, i;

	pr_info("tx_free %zu, tx_cur %zu, tx_next %zu\n", lp->tx_free,
		lp->tx_cur, lp->tx_next);

	print_descriptor(lp, lp->rx_cur, 0);

	for (desci = (lp->tx_cur - 10) % DWCEQOS_TX_DCNT, i = 0;
		 i < DWCEQOS_TX_DCNT;
		 ++i) {
		print_descriptor(lp, desci, 1);
		desci = (desci + 1) % DWCEQOS_TX_DCNT;
	}

	pr_info("DMA_DMA_IE:          0x%08x\n",
		dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_IE));
	pr_info("DMA_Debug_Status0:          0x%08x\n",
		dwceqos_read(lp, REG_DWCEQOS_DMA_DEBUG_ST0));
	pr_info("DMA_CH0_Status:             0x%08x\n",
		dwceqos_read(lp, REG_DWCEQOS_DMA_IS));
	pr_info("DMA_CH0_Current_App_TxDesc: 0x%08x\n",
		dwceqos_read(lp, 0x1144));
	pr_info("DMA_CH0_Current_App_TxBuff: 0x%08x\n",
		dwceqos_read(lp, 0x1154));
	pr_info("MTL_Debug_Status:      0x%08x\n",
		dwceqos_read(lp, REG_DWCEQOS_MTL_DEBUG_ST));
	pr_info("MTL_TXQ0_Debug_Status: 0x%08x\n",
		dwceqos_read(lp, REG_DWCEQOS_MTL_TXQ0_DEBUG_ST));
	pr_info("MTL_RXQ0_Debug_Status: 0x%08x\n",
		dwceqos_read(lp, REG_DWCEQOS_MTL_RXQ0_DEBUG_ST));
	pr_info("Current TX DMA: 0x%08x, RX DMA: 0x%08x\n",
		dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_CUR_TXDESC),
		dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_CUR_RXDESC));
	pr_info("Current TX H DMA: 0x%08x, RX H DMA: 0x%08x\n",
		dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_TXDESC_LIST),
		dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_RXDESC_LIST));
	pr_info("Current TX T DMA: 0x%08x, RX H DMA: 0x%08x\n",
		dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_TXDESC_TAIL),
		dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_RXDESC_TAIL));
}

static void dwceqos_mdio_set_csr(struct net_local *lp)
{

	unsigned long rate;
#if (GMAC_QUICK_CLK == 0) 
	if (lp->apb_pclk)
		rate = clk_get_rate(lp->apb_pclk);
	else
		rate = lp->default_apb_pclk;
#else

	rate = clk_mini_get_rate(CKG_MUX_GLB_APB);
	if(rate <= 0)
		rate = lp->default_apb_pclk;

#endif
	dev_info(&lp->pdev->dev, "mdio set csr rate (%ld)\n", rate);

	if (rate <= 20000000)
		lp->csr_val = DWCEQOS_MAC_MDIO_ADDR_CR_20;
	else if (rate <= 35000000)
		lp->csr_val = DWCEQOS_MAC_MDIO_ADDR_CR_35;
	else if (rate <= 60000000)
		lp->csr_val = DWCEQOS_MAC_MDIO_ADDR_CR_60;
	else if (rate <= 100000000)
		lp->csr_val = DWCEQOS_MAC_MDIO_ADDR_CR_100;
	else if (rate <= 150000000)
		lp->csr_val = DWCEQOS_MAC_MDIO_ADDR_CR_150;
	else if (rate <= 250000000)
		lp->csr_val = DWCEQOS_MAC_MDIO_ADDR_CR_250;
}

/* Simple MDIO functions implementing mii_bus */
static int dwceqos_mdio_read(struct mii_bus *bus, int mii_id, int phyreg)
{
	struct net_local *lp = bus->priv;
	u32 regval;
	int i;
	int data;

	regval = DWCEQOS_MDIO_PHYADDR(mii_id) |
		DWCEQOS_MDIO_PHYREG(phyreg) |
		DWCEQOS_MAC_MDIO_ADDR_CR(lp->csr_val) |
		DWCEQOS_MAC_MDIO_ADDR_GB |
		DWCEQOS_MAC_MDIO_ADDR_GOC_READ;
	dwceqos_write(lp, REG_DWCEQOS_MAC_MDIO_ADDR, regval);

	for (i = 0; i < MDIO_RETRY_CNT; ++i) {
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		usleep_range(MDIO_RETRY_DELAY, MDIO_RETRY_DELAY << 1);
		#else
		udelay(60);
		#endif
		if (!(dwceqos_read(lp, REG_DWCEQOS_MAC_MDIO_ADDR) &
		      DWCEQOS_MAC_MDIO_ADDR_GB))
			break;
	}

	data = dwceqos_read(lp, REG_DWCEQOS_MAC_MDIO_DATA);
	if (i == MDIO_RETRY_CNT) {
		netdev_warn(lp->ndev, "MDIO read timed out\n");
		data = MDIO_DATA_MASK;
	}
	pr_debug("mdio rd addr %x val%x\n", phyreg, (u32)(data & MDIO_DATA_MASK));

	return data & MDIO_DATA_MASK;
}

static int dwceqos_mdio_write(struct mii_bus *bus, int mii_id, int phyreg,
			      u16 value)
{
	struct net_local *lp = bus->priv;
	struct net_device * net_dev = lp->ndev;
	struct phy_device *phydev = net_dev->phydev;
	struct phy_driver *phydrv = phydev->drv;
	u32 regval;
	u32 phy_id = phydev->phy_id;
	int i;

	pr_debug("mdio wr addr %x val%x\n", phyreg, value);
	if(lp->phy_interface == PHY_INTERFACE_MODE_RGMII) {
		if ((phyreg == 0) && (value & BMCR_PDOWN)) {
			pr_debug("Skip PHY PD For RGMII\n");
			return 0;
		}
	}
	if(lp->phy_type == INTERNAL_PHY) {
		if((phyreg == 0) && (value & (1 << 15))) {
			if(phy_id == FH_GMAC_PHY_INTERNAL_V2) {
				phydrv->config_init(phydev);
				return 0;
			}

		}
	}
	dwceqos_write(lp, REG_DWCEQOS_MAC_MDIO_DATA, value);
	if ((lp->phy_interface == PHY_INTERFACE_MODE_RMII) ||
		(lp->phy_interface == PHY_INTERFACE_MODE_MII))
		dwceqos_write(lp, REG_DWCEQOS_MAC_MDIO_DATA, value);
	regval = DWCEQOS_MDIO_PHYADDR(mii_id) |
		DWCEQOS_MDIO_PHYREG(phyreg) |
		DWCEQOS_MAC_MDIO_ADDR_CR(lp->csr_val) |
		DWCEQOS_MAC_MDIO_ADDR_GB |
		DWCEQOS_MAC_MDIO_ADDR_GOC_WRITE;
	dwceqos_write(lp, REG_DWCEQOS_MAC_MDIO_ADDR, regval);
	for (i = 0; i < MDIO_RETRY_CNT; ++i) {
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		usleep_range(MDIO_RETRY_DELAY, MDIO_RETRY_DELAY << 1);
		#else
		udelay(60);
		#endif
		if (!(dwceqos_read(lp, REG_DWCEQOS_MAC_MDIO_ADDR) &
		      DWCEQOS_MAC_MDIO_ADDR_GB))
			break;
	}
	if (i == MDIO_RETRY_CNT)
		netdev_warn(lp->ndev, "MDIO write timed out\n");

	return 0;
}

static int dwceqos_ioctl(struct net_device *ndev, struct ifreq *rq, int cmd)
{
	struct net_local *lp = netdev_priv(ndev);
	struct phy_device *phydev = ndev->phydev;

	if (!netif_running(ndev))
		return -EINVAL;

	if (!phydev)
		return -ENODEV;

	switch (cmd) {
	case SIOCGMIIPHY:
	case SIOCGMIIREG:
	case SIOCSMIIREG:
		return phy_mii_ioctl(phydev, rq, cmd);
	default:
		dev_info(&lp->pdev->dev, "ioctl %X not implemented.\n", cmd);
		return -EOPNOTSUPP;
	}
}

static void dwceqos_drain_tx_dma(struct net_local *lp);
static void dwceqos_drain_txq(struct net_local *lp);
static void dwceqos_drain_rxq(struct net_local *lp);
void dweqos_disable_rx_dma(struct net_local *lp);
static void dweqos_disable_mac(struct net_local *lp);
static void dweqos_disable_tx_dma(struct net_local *lp);
static void dweqos_disable_mac_re(struct net_local *lp);
//static void dweqos_disable_mac_te(struct net_local *lp);

#define DELAY_COUNT 10000
static void dwceqos_suspend(struct net_local *lp)
{
	unsigned long flags;

	int count = DELAY_COUNT;
	lp->mac_suspend = true;

	printk(KERN_ERR "suspend mac\n");

	dwceqos_drain_tx_dma(lp);
        //disable the transimit DMA
	spin_lock_irqsave(&lp->hw_lock, flags);
        dweqos_disable_tx_dma(lp);
        dweqos_disable_tx_dma(lp);
	dweqos_disable_mac_re(lp);
	spin_unlock_irqrestore(&lp->hw_lock, flags);

	dwceqos_drain_tx_dma(lp);

	pr_info("be REG_DWCEQOS_DMA_CH0_TX_CTRL: 0x%08x\n",
                dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_TX_CTRL));
	pr_info("be MTL_TXQ0_Debug_Status: 0x%08x\n",
                dwceqos_read(lp, REG_DWCEQOS_MTL_TXQ0_DEBUG_ST));
        pr_info("be MTL_RXQ0_Debug_Status: 0x%08x\n",
               dwceqos_read(lp, REG_DWCEQOS_MTL_RXQ0_DEBUG_ST));

        pr_info("be DMA_Debug_Status0:          0x%08x\n",
                dwceqos_read(lp, REG_DWCEQOS_DMA_DEBUG_ST0));
	//wait for previouse frame or flush tx fifo
	while( (dwceqos_read(lp, REG_DWCEQOS_MTL_TXQ0_OPER) & BIT(0)) && count --) {
		udelay(1000);
	}
	if( count <= 0) {
		printk(KERN_ERR "wait for tx fifo fulsh timeout!\n");
	}

	//printk(KERN_DEBUG "wait for tx fifo fulsh:%d\n",count);
	//flush mtl tx fifo
	dwceqos_write(lp, REG_DWCEQOS_MTL_TXQ0_OPER, (dwceqos_read(lp,REG_DWCEQOS_MTL_TXQ0_OPER) | BIT(0)));

	count = DELAY_COUNT;
	while( (dwceqos_read(lp, REG_DWCEQOS_MTL_TXQ0_OPER) & BIT(0)) && count -- ) {
		udelay(1000);
	}
	if( count <= 0) {
		printk(KERN_ERR "wait for tx fifo fulsh finish timeout!\n");
	}

	//printk(KERN_DEBUG "wait for tx fifo fulsh finish:%d\n",count);

	//wait for previouse frame
        dwceqos_drain_txq(lp);

	spin_lock_irqsave(&lp->hw_lock, flags);
        //disable mac transmitter and receiver
        dweqos_disable_mac(lp);
	spin_unlock_irqrestore(&lp->hw_lock, flags);

        dwceqos_drain_rxq(lp);

	spin_lock_irqsave(&lp->hw_lock, flags);
        dweqos_disable_rx_dma(lp);
	spin_unlock_irqrestore(&lp->hw_lock, flags);

	pr_info("MTL_TXQ0_Debug_Status: 0x%08x\n",
                dwceqos_read(lp, REG_DWCEQOS_MTL_TXQ0_DEBUG_ST));
        pr_info("MTL_RXQ0_Debug_Status: 0x%08x\n",
               dwceqos_read(lp, REG_DWCEQOS_MTL_RXQ0_DEBUG_ST));

        pr_info("DMA_Debug_Status0:          0x%08x\n",
                dwceqos_read(lp, REG_DWCEQOS_DMA_DEBUG_ST0));
	printk(KERN_ERR "suspend mac done\n");

	return;
}

static void dwceqos_link_down(struct net_local *lp)
{
	u32 regval;
	unsigned long flags;
	//struct net_device * net_dev = lp->ndev;
	//struct phy_device *phydev = net_dev->phydev;
	//struct phy_driver *phydrv = phydev->drv;

	//if (lp->phy_type == INTERNAL_PHY)
	//	phydrv->brd_link_down_cb(phydev);
	/* Indicate link down to the LPI state machine */
	spin_lock_irqsave(&lp->hw_lock, flags);
	regval = dwceqos_read(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS);
	regval &= ~DWCEQOS_MAC_LPI_CTRL_STATUS_PLS;
	dwceqos_write(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS, regval);
	spin_unlock_irqrestore(&lp->hw_lock, flags);

	dwceqos_suspend(lp);

}

static void dweqos_enable_dmas(struct net_local *lp)
{
        u32 reg;
        reg = dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_RX_CTRL);
        reg |= DWCEQOS_DMA_CH_CTRL_START;
        dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_RX_CTRL, reg);

        reg = dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_TX_CTRL);
        reg |= DWCEQOS_DMA_CH_CTRL_START;
        dwceqos_write(lp,REG_DWCEQOS_DMA_CH0_TX_CTRL, reg);

}
static void dweqos_enable_mac(struct net_local *lp)
{
        u32 reg;

        reg = dwceqos_read(lp, REG_DWCEQOS_MAC_CFG);
        reg |= (DWCEQOS_MAC_CFG_TE | DWCEQOS_MAC_CFG_RE);
        dwceqos_write(lp, REG_DWCEQOS_MAC_CFG, reg);


}

static void dwceqos_resume(struct net_local *lp)
{
	lp->mac_suspend = false;
	printk(KERN_ERR "resume mac\n");
        dweqos_enable_dmas(lp);
	dweqos_enable_mac(lp);
	printk(KERN_ERR "resume mac done\n");

	return;
}
static void dwceqos_link_up(struct net_local *lp)
{
	struct net_device *ndev = lp->ndev;
	u32 regval;
	unsigned long flags;
	//struct phy_device *phydev = ndev->phydev;
	//struct phy_driver *phydrv = phydev->drv;

	//if (lp->phy_type == INTERNAL_PHY)
	//	phydrv->brd_link_up_cb(phydev);

	/* Indicate link up to the LPI state machine */
	spin_lock_irqsave(&lp->hw_lock, flags);
	regval = dwceqos_read(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS);
	regval |= DWCEQOS_MAC_LPI_CTRL_STATUS_PLS;
	dwceqos_write(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS, regval);
	spin_unlock_irqrestore(&lp->hw_lock, flags);

	mdelay(100);
	 if(lp->phy_interface == PHY_INTERFACE_MODE_RGMII) {
		if (0x0 == (dwceqos_read(lp, 0x00f8) & BIT(19))) {
			printk(KERN_DEBUG "gmac get phy status err!\n");
		}
	 }

	spin_lock_irqsave(&lp->hw_lock, flags);
	dwceqos_resume(lp);
	spin_unlock_irqrestore(&lp->hw_lock, flags);

	if (!(lp->feature0 & DWCEQOS_MAC_HW_FEATURE0_EEESEL))
		return;

	lp->eee_active = !phy_init_eee(ndev->phydev, 0);

	/* Check for changed EEE capability */
	if (!lp->eee_active && lp->eee_enabled) {
		lp->eee_enabled = 0;

		spin_lock_irqsave(&lp->hw_lock, flags);
		regval = dwceqos_read(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS);
		regval &= ~DWCEQOS_LPI_CTRL_ENABLE_EEE;
		dwceqos_write(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS, regval);
		spin_unlock_irqrestore(&lp->hw_lock, flags);
	}
}

static void dwceqos_set_speed(struct net_local *lp)
{
	struct net_device *ndev = lp->ndev;
	struct phy_device *phydev = ndev->phydev;
	u32 regval;

	regval = dwceqos_read(lp, REG_DWCEQOS_MAC_CFG);
	regval &= ~(DWCEQOS_MAC_CFG_PS | DWCEQOS_MAC_CFG_FES |
		    DWCEQOS_MAC_CFG_DM);

	if (phydev->duplex)
		regval |= DWCEQOS_MAC_CFG_DM;
	if (phydev->speed == SPEED_10) {
		regval |= DWCEQOS_MAC_CFG_PS;
	} else if (phydev->speed == SPEED_100) {
		regval |= DWCEQOS_MAC_CFG_PS |
			DWCEQOS_MAC_CFG_FES;
	} else if (phydev->speed != SPEED_1000) {
		netdev_err(lp->ndev,
			   "unknown PHY speed %d\n",
			   phydev->speed);
		return;
	}

	dwceqos_write(lp, REG_DWCEQOS_MAC_CFG, regval);
}

static void dwceqos_adjust_link(struct net_device *ndev)
{
	struct net_local *lp = netdev_priv(ndev);
	struct phy_device *phydev = ndev->phydev;
	int status_change = 0;

	if (lp->phy_defer)
		return;

	if (phydev->link) {
		if ((lp->speed != phydev->speed) ||
		    (lp->duplex != phydev->duplex)) {

			if(lp->mac_suspend == false) {
				printk(KERN_ERR "missing mac down.\n");
				//dwceqos_suspend(lp);
				dwceqos_link_down(lp);
			}

			dwceqos_set_speed(lp);

			lp->speed = phydev->speed;
			lp->duplex = phydev->duplex;
			status_change = 1;
		}

		if (lp->flowcontrol.autoneg) {
			lp->flowcontrol.rx = phydev->pause ||
					     phydev->asym_pause;
			lp->flowcontrol.tx = phydev->pause ||
					     phydev->asym_pause;
		}

		if (lp->flowcontrol.rx != lp->flowcontrol.rx_current) {
			if (netif_msg_link(lp))
				netdev_dbg(ndev, "set rx flow to %d\n",
					   lp->flowcontrol.rx);
			dwceqos_set_rx_flowcontrol(lp, lp->flowcontrol.rx);
			lp->flowcontrol.rx_current = lp->flowcontrol.rx;
		}
		if (lp->flowcontrol.tx != lp->flowcontrol.tx_current) {
			if (netif_msg_link(lp))
				netdev_dbg(ndev, "set tx flow to %d\n",
					   lp->flowcontrol.tx);
			dwceqos_set_tx_flowcontrol(lp, lp->flowcontrol.tx);
			lp->flowcontrol.tx_current = lp->flowcontrol.tx;
		}
	}

	if (phydev->link != lp->link) {
		lp->link = phydev->link;
		status_change = 1;
	}

	if (status_change) {
		if (phydev->link) {
			netif_trans_update(lp->ndev);
			dwceqos_link_up(lp);
		} else {
			dwceqos_link_down(lp);
		}
		phy_print_status(phydev);
	}
}

static void dwceqos_mphy_clk_reset(struct net_device *ndev)
{
	struct net_local *lp = netdev_priv(ndev);

	if (lp->ana) {
		#if 0
		/* Reset PHY CLK for Internal PHY(100T) */
		regmap_update_bits(lp->ana, REG_ANA_REG,
					REG_ANA_PHY_CLK, (u32)(~REG_ANA_PHY_CLK));
		mdelay(100);
		regmap_update_bits(lp->ana, REG_ANA_REG,
					REG_ANA_PHY_CLK, REG_ANA_PHY_CLK);
		mdelay(20);
		#endif
	} else {
		netdev_dbg(lp->ndev, "ANA reg hasn't io remap\n");
	}
}

static __u64 dwceqos_mmc_read_status(struct net_device *ndev)
{
	struct net_local *lp = netdev_priv(ndev);

	lp->mmc_counters.rxcrcerror +=
			dwceqos_read(lp, DWC_MMC_RXCRCERROR);

	return lp->mmc_counters.rxcrcerror;
}

static int dwceqos_mii_probe(struct net_device *ndev)
{
	struct net_local *lp = netdev_priv(ndev);
	struct phy_device *phydev = NULL;

	if (lp->phy_node) {
		phydev = of_phy_connect(lp->ndev,
					lp->phy_node,
					&dwceqos_adjust_link,
					0,
					lp->phy_interface);

		if (!phydev) {
			netdev_err(ndev, "no PHY found\n");
			return -1;
		}
		if((lp->phy_interface == PHY_INTERFACE_MODE_MII) ||
			(lp->phy_interface == PHY_INTERFACE_MODE_RMII)) {
			if(lp->phy_type == INTERNAL_PHY){
				phydev->clk_reset = &dwceqos_mphy_clk_reset;
				phydev->mmc_read_status = &dwceqos_mmc_read_status;
			}
		}
		else
			phydev->clk_reset = NULL;
	} else {
		netdev_err(ndev, "no PHY configured\n");
		return -ENODEV;
	}

	if (netif_msg_probe(lp))
		phy_attached_info(phydev);

	//phydev->supported &= PHY_GBIT_FEATURES | SUPPORTED_Pause |
	//		     SUPPORTED_Asym_Pause;
	linkmode_copy(phydev->supported, PHY_GBIT_FEATURES);
	//linkmode_and(phydev->supported, PHY_GBIT_FEATURES);
	//linkmode_set_bit(ETHTOOL_LINK_MODE_Pause_BIT, phydev->supported);
	//linkmode_set_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT, phydev->supported);
	//linkmode_set_bit(ETHTOOL_LINK_MODE_Pause_BIT, phydev->advertising);
	//linkmode_set_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT, phydev->advertising);
	//linkmode_and(ETHTOOL_LINK_MODE_Pause_BIT, phydev->supported);
	//linkmode_and(ETHTOOL_LINK_MODE_Asym_Pause_BIT, phydev->supported);
	//linkmode_and(ETHTOOL_LINK_MODE_Pause_BIT, phydev->advertising);
	//linkmode_and(ETHTOOL_LINK_MODE_Asym_Pause_BIT, phydev->advertising);

	lp->link    = 0;
	lp->speed   = 0;
	lp->duplex  = DUPLEX_UNKNOWN;
	lp->flowcontrol.autoneg = AUTONEG_ENABLE;

	return 0;
}

static void dwceqos_alloc_rxring_desc(struct net_local *lp, int index)
{
	struct sk_buff *new_skb;
	dma_addr_t new_skb_baddr = 0;

	new_skb = netdev_alloc_skb(lp->ndev, DWCEQOS_RX_BUF_SIZE);
	if (!new_skb) {
		netdev_err(lp->ndev, "alloc_skb error for desc %d\n", index);
		goto err_out;
	}
	new_skb_baddr = dma_map_single(lp->ndev->dev.parent,
				       new_skb->data, DWCEQOS_RX_BUF_SIZE,
				       DMA_FROM_DEVICE);
	if (dma_mapping_error(lp->ndev->dev.parent, new_skb_baddr)) {
		netdev_err(lp->ndev, "DMA map error\n");
		dev_kfree_skb(new_skb);
		new_skb = NULL;
		goto err_out;
	}
	lp->rx_descs[index].des0 = new_skb_baddr;
	lp->rx_descs[index].des1 = 0;
	lp->rx_descs[index].des2 = 0;


	if (lp->rx_irq_watchdog) {
		lp->rx_descs[index].des3 = DWCEQOS_DMA_RDES3_BUF1V |
					DWCEQOS_DMA_RDES3_OWN;
	} else {
		lp->rx_descs[index].des3 = DWCEQOS_DMA_RDES3_INTE |
					DWCEQOS_DMA_RDES3_BUF1V |
					DWCEQOS_DMA_RDES3_OWN;
	}
	lp->rx_skb[index].mapping = new_skb_baddr;
	lp->rx_skb[index].len = DWCEQOS_RX_BUF_SIZE;

err_out:
	lp->rx_skb[index].skb = new_skb;
}

static void dwceqos_clean_rings(struct net_local *lp)
{
	int i;

	if (lp->rx_skb) {
		for (i = 0; i < DWCEQOS_RX_DCNT; i++) {
			if (lp->rx_skb[i].skb) {
				dma_unmap_single(lp->ndev->dev.parent,
						 lp->rx_skb[i].mapping,
						 lp->rx_skb[i].len,
						 DMA_FROM_DEVICE);

				dev_kfree_skb(lp->rx_skb[i].skb);
				lp->rx_skb[i].skb = NULL;
				lp->rx_skb[i].mapping = 0;
			}
		}
	}

	if (lp->tx_skb) {
		for (i = 0; i < DWCEQOS_TX_DCNT; i++) {
			if (lp->tx_skb[i].skb) {
				dev_kfree_skb(lp->tx_skb[i].skb);
				lp->tx_skb[i].skb = NULL;
			}
			if (lp->tx_skb[i].mapping) {
				dma_unmap_single(lp->ndev->dev.parent,
						 lp->tx_skb[i].mapping,
						 lp->tx_skb[i].len,
						 DMA_TO_DEVICE);
				lp->tx_skb[i].mapping = 0;
			}
		}
	}
}

static void dwceqos_descriptor_free(struct net_local *lp)
{
	int size;

	dwceqos_clean_rings(lp);

	kfree(lp->tx_skb);
	lp->tx_skb = NULL;
	kfree(lp->rx_skb);
	lp->rx_skb = NULL;

	size = DWCEQOS_RX_DCNT * sizeof(struct dwceqos_dma_desc);
	if (lp->rx_descs) {
		dma_free_coherent(lp->ndev->dev.parent, size,
				  (void *)(lp->rx_descs), lp->rx_descs_addr);
		lp->rx_descs = NULL;
	}

	size = DWCEQOS_TX_DCNT * sizeof(struct dwceqos_dma_desc);
	if (lp->tx_descs) {
		dma_free_coherent(lp->ndev->dev.parent, size,
				  (void *)(lp->tx_descs), lp->tx_descs_addr);
		lp->tx_descs = NULL;
	}
}

static int dwceqos_descriptor_init(struct net_local *lp)
{
	int size;
	u32 i;

	lp->gso_size = 0;

	lp->tx_skb = NULL;
	lp->rx_skb = NULL;
	lp->rx_descs = NULL;
	lp->tx_descs = NULL;

	/* Reset the DMA indexes */
	lp->rx_cur = 0;
	lp->tx_cur = 0;
	lp->tx_next = 0;
	lp->tx_free = DWCEQOS_TX_DCNT;

	/* Allocate Ring descriptors */
	size = DWCEQOS_RX_DCNT * sizeof(struct ring_desc);
	lp->rx_skb = kzalloc(size, GFP_KERNEL);
	if (!lp->rx_skb)
		goto err_out;

	size = DWCEQOS_TX_DCNT * sizeof(struct ring_desc);
	lp->tx_skb = kzalloc(size, GFP_KERNEL);
	if (!lp->tx_skb)
		goto err_out;

	/* Allocate DMA descriptors */
	size = DWCEQOS_RX_DCNT * sizeof(struct dwceqos_dma_desc);
	lp->rx_descs = dma_alloc_coherent(lp->ndev->dev.parent, size,
			&lp->rx_descs_addr, GFP_KERNEL);
	if (!lp->rx_descs)
		goto err_out;
	lp->rx_descs_tail_addr = lp->rx_descs_addr +
		sizeof(struct dwceqos_dma_desc) * DWCEQOS_RX_DCNT;

	size = DWCEQOS_TX_DCNT * sizeof(struct dwceqos_dma_desc);
	lp->tx_descs = dma_alloc_coherent(lp->ndev->dev.parent, size,
			&lp->tx_descs_addr, GFP_KERNEL);
	if (!lp->tx_descs)
		goto err_out;
	lp->tx_descs_tail_addr = lp->tx_descs_addr +
		sizeof(struct dwceqos_dma_desc) * DWCEQOS_TX_DCNT;

	/* Initialize RX Ring Descriptors and buffers */
	for (i = 0; i < DWCEQOS_RX_DCNT; ++i) {
		dwceqos_alloc_rxring_desc(lp, i);
		if (!(lp->rx_skb[lp->rx_cur].skb))
			goto err_out;
	}

	/* Initialize TX Descriptors */
	for (i = 0; i < DWCEQOS_TX_DCNT; ++i) {
		lp->tx_descs[i].des0 = 0;
		lp->tx_descs[i].des1 = 0;
		lp->tx_descs[i].des2 = 0;
		lp->tx_descs[i].des3 = 0;
	}

	/* Make descriptor writes visible to the DMA. */
	wmb();

	return 0;

err_out:
	dwceqos_descriptor_free(lp);
	return -ENOMEM;
}

static int dwceqos_packet_avail(struct net_local *lp)
{
	return !(lp->rx_descs[lp->rx_cur].des3 & DWCEQOS_DMA_RDES3_OWN);
}

static void dwceqos_get_hwfeatures(struct net_local *lp)
{
	lp->feature0 = dwceqos_read(lp, REG_DWCEQOS_MAC_HW_FEATURE0);
	lp->feature1 = dwceqos_read(lp, REG_DWCEQOS_MAC_HW_FEATURE1);
	lp->feature2 = dwceqos_read(lp, REG_DWCEQOS_MAC_HW_FEATURE2);
}

static void dwceqos_dma_enable_txirq(struct net_local *lp)
{
	u32 regval;
	unsigned long flags;

	spin_lock_irqsave(&lp->hw_lock, flags);
	regval = dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_IE);
	regval |= DWCEQOS_DMA_CH0_IE_TIE;
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_IE, regval);
	spin_unlock_irqrestore(&lp->hw_lock, flags);
}

static void dwceqos_dma_disable_txirq(struct net_local *lp)
{
	u32 regval;
	unsigned long flags;

	spin_lock_irqsave(&lp->hw_lock, flags);
	regval = dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_IE);
	regval &= ~DWCEQOS_DMA_CH0_IE_TIE;
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_IE, regval);
	spin_unlock_irqrestore(&lp->hw_lock, flags);
}

static void dwceqos_dma_enable_rxirq(struct net_local *lp)
{
	u32 regval;
	unsigned long flags;

	spin_lock_irqsave(&lp->hw_lock, flags);
	regval = dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_IE);
	regval |= DWCEQOS_DMA_CH0_IE_RIE;
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_IE, regval);
	spin_unlock_irqrestore(&lp->hw_lock, flags);
}

static void dwceqos_dma_disable_rxirq(struct net_local *lp)
{
	u32 regval;
	unsigned long flags;

	spin_lock_irqsave(&lp->hw_lock, flags);
	regval = dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_IE);
	regval &= ~DWCEQOS_DMA_CH0_IE_RIE;
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_IE, regval);
	spin_unlock_irqrestore(&lp->hw_lock, flags);
}

static void dwceqos_disable_all_irq(struct net_local *lp)
{
	unsigned long flags;

	spin_lock_irqsave(&lp->hw_lock, flags);
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_IE, 0);
	dwceqos_write(lp, REG_DWCEQOS_MAC_IE, 0);
	dwceqos_write(lp, REG_DWCEQOS_MMC_RXIRQMASK, ~0u);
	dwceqos_write(lp, REG_DWCEQOS_MMC_TXIRQMASK, ~0u);
	dwceqos_write(lp, REG_DWCEQOS_IPC_RXMASK, ~0u);
	spin_unlock_irqrestore(&lp->hw_lock, flags);
}

static void dwceqos_enable_mmc_interrupt(struct net_local *lp)
{
	dwceqos_write(lp, REG_DWCEQOS_MMC_RXIRQMASK, 0);
	dwceqos_write(lp, REG_DWCEQOS_MMC_TXIRQMASK, 0);
}

static int dwceqos_mii_init(struct net_local *lp)
{
	int ret = -ENXIO;
	struct resource res;
	struct device_node *mdionode;

	mdionode = of_get_child_by_name(lp->pdev->dev.of_node, "mdio");

	if (!mdionode)
		return 0;

	lp->mii_bus = mdiobus_alloc();
	if (!lp->mii_bus) {
		ret = -ENOMEM;
		goto err_out;
	}

	lp->mii_bus->name  = "DWCEQOS MII bus";
	lp->mii_bus->read  = &dwceqos_mdio_read;
	lp->mii_bus->write = &dwceqos_mdio_write;
	lp->mii_bus->priv = lp;
	lp->mii_bus->parent = &lp->pdev->dev;

	of_address_to_resource(lp->pdev->dev.of_node, 0, &res);
	snprintf(lp->mii_bus->id, MII_BUS_ID_SIZE, "%.8llx",
		 (unsigned long long)res.start);
	if (of_mdiobus_register(lp->mii_bus, mdionode))
		goto err_out_free_mdiobus;

	return 0;

err_out_free_mdiobus:
	mdiobus_free(lp->mii_bus);
err_out:
	of_node_put(mdionode);
	return ret;
}

/* DMA reset. When issued also resets all MTL and MAC registers as well */
static void dwceqos_reset_hw(struct net_local *lp)
{
	/* Wait (at most) 0.5 seconds for DMA reset*/
	int i = 5000;
	u32 reg;

	/* Force gigabit to guarantee a TX clock for GMII. */
	reg = dwceqos_read(lp, REG_DWCEQOS_MAC_CFG);
	reg &= ~(DWCEQOS_MAC_CFG_PS | DWCEQOS_MAC_CFG_FES);
	reg |= DWCEQOS_MAC_CFG_DM;
	dwceqos_write(lp, REG_DWCEQOS_MAC_CFG, reg);
	mutex_lock(&lp->ndev->phydev->lock);
	dwceqos_write(lp, REG_DWCEQOS_DMA_MODE, DWCEQOS_DMA_MODE_SWR);

	do {
		udelay(100);
		i--;
		reg = dwceqos_read(lp, REG_DWCEQOS_DMA_MODE);
	} while ((reg & DWCEQOS_DMA_MODE_SWR) && i);
	mutex_unlock(&lp->ndev->phydev->lock);
	/* We might experience a timeout if the chip clock mux is broken */
	if (!i)
		netdev_err(lp->ndev, "DMA reset timed out!\n");
}

static void dwceqos_fatal_bus_error(struct net_local *lp, u32 dma_status)
{
	if (dma_status & DWCEQOS_DMA_CH0_IS_TEB) {
		netdev_err(lp->ndev, "txdma bus error %s %s (status=%08x)\n",
			   dma_status & DWCEQOS_DMA_CH0_IS_TX_ERR_READ ?
				"read" : "write",
			   dma_status & DWCEQOS_DMA_CH0_IS_TX_ERR_DESCR ?
				"descr" : "data",
			   dma_status);

		print_status(lp);
	}
	if (dma_status & DWCEQOS_DMA_CH0_IS_REB) {
		netdev_err(lp->ndev, "rxdma bus error %s %s (status=%08x)\n",
			   dma_status & DWCEQOS_DMA_CH0_IS_RX_ERR_READ ?
				"read" : "write",
			   dma_status & DWCEQOS_DMA_CH0_IS_RX_ERR_DESCR ?
				"descr" : "data",
			   dma_status);

		print_status(lp);
	}
}

static void dwceqos_mmc_interrupt(struct net_local *lp)
{
	unsigned long flags;

	spin_lock_irqsave(&lp->stats_lock, flags);

	/* A latched mmc interrupt can not be masked, we must read
	 *  all the counters with an interrupt pending.
	 */
	dwceqos_read_mmc_counters(lp,
				  dwceqos_read(lp, REG_DWCEQOS_MMC_RXIRQ),
				  dwceqos_read(lp, REG_DWCEQOS_MMC_TXIRQ),
				  dwceqos_read(lp, REG_DWCEQOS_IPC_RXINT));

	spin_unlock_irqrestore(&lp->stats_lock, flags);
}

static void dwceqos_mac_interrupt(struct net_local *lp)
{
	u32 cause;

	cause = dwceqos_read(lp, REG_DWCEQOS_MAC_IS);

	if (cause & DWCEQOS_MAC_IS_MMC_INT)
		dwceqos_mmc_interrupt(lp);
	if (cause & DWCEQOS_MAC_IS_PMTIS_INT) {
		printk(KERN_ERR "get pmt status:0x%x\n",dwceqos_read(lp, REG_DWCEQOS_MAC_PMT_CTRL_STATUS));
	}
}
static void dwmac4_pmt(struct net_local *lp, unsigned long mode)
{
    unsigned int pmt = 0;

    if (mode & WAKE_MAGIC) {
            pr_debug("GMAC: WOL Magic frame\n");
            pmt |= power_down | magic_pkt_en;
    }
    if (mode & WAKE_UCAST) {
            pr_debug("GMAC: WOL on global unicast\n");
            pmt |= power_down | global_unicast | wake_up_frame_en;
    }

    if (pmt) {
            /* The receiver must be enabled for WOL before powering down */

		u32 reg;
        reg = dwceqos_read(lp, REG_DWCEQOS_MAC_CFG);
        reg |= (DWCEQOS_MAC_CFG_TE | DWCEQOS_MAC_CFG_RE);
        dwceqos_write(lp, REG_DWCEQOS_MAC_CFG, reg);
    }
	printk(KERN_ERR "set pmt:%x\n",pmt);
   // writel(pmt, ioaddr + GMAC_PMT);
   dwceqos_write(lp, REG_DWCEQOS_MAC_PMT_CTRL_STATUS, pmt);
   printk(KERN_ERR "aat pmt:%x\n",dwceqos_read(lp, REG_DWCEQOS_MAC_PMT_CTRL_STATUS));
}
static irqreturn_t wol_interrupt(int irq, void *dev_id)
{
	struct net_device *ndev = dev_id;
	struct net_local *lp = netdev_priv(ndev);
	printk(KERN_ERR "get wol----------\n");
	printk(KERN_ERR "wol_interrupt pmt status:0x%x\n",dwceqos_read(lp, REG_DWCEQOS_MAC_PMT_CTRL_STATUS));
	if (lp->irq_wake)
		pm_wakeup_event(&(lp->pdev->dev), 0);

	return IRQ_HANDLED;
}

static irqreturn_t dwceqos_interrupt(int irq, void *dev_id)
{
	struct net_device *ndev = dev_id;
	struct net_local *lp = netdev_priv(ndev);

	u32 cause;
	u32 dma_status;
	irqreturn_t ret = IRQ_NONE;

	//if (lp->irq_wake)
	//	pm_wakeup_event(&(lp->pdev->dev), 0);


	cause = dwceqos_read(lp, REG_DWCEQOS_DMA_IS);
	/* DMA Channel 0 Interrupt */
	if (cause & DWCEQOS_DMA_IS_DC0IS) {
		dma_status = dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_STA);

		/* Transmit Interrupt */
		if (dma_status & DWCEQOS_DMA_CH0_IS_TI) {
			tasklet_schedule(&lp->tx_bdreclaim_tasklet);
			dwceqos_dma_disable_txirq(lp);
		}

		/* Receive Interrupt */
		if (dma_status & DWCEQOS_DMA_CH0_IS_RI) {
			/* Disable RX IRQs */
			dwceqos_dma_disable_rxirq(lp);
			napi_schedule(&lp->napi);
		}

		/* Fatal Bus Error interrupt */
		if (unlikely(dma_status & DWCEQOS_DMA_CH0_IS_FBE)) {
			dwceqos_fatal_bus_error(lp, dma_status);

			/* errata 9000831707 */
			dma_status |= DWCEQOS_DMA_CH0_IS_TEB |
				      DWCEQOS_DMA_CH0_IS_REB;
		}

		/* Ack all DMA Channel 0 IRQs */
		dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_STA, dma_status);
		ret = IRQ_HANDLED;
	}

	if (cause & DWCEQOS_DMA_IS_MTLIS) {
		u32 val = dwceqos_read(lp, REG_DWCEQOS_MTL_Q0_ISCTRL);

		dwceqos_write(lp, REG_DWCEQOS_MTL_Q0_ISCTRL, val);
		ret = IRQ_HANDLED;
	}

	if (cause & DWCEQOS_DMA_IS_MACIS) {
		dwceqos_mac_interrupt(lp);
		ret = IRQ_HANDLED;
	}

	return ret;
}

static void dwceqos_set_rx_flowcontrol(struct net_local *lp, bool enable)
{
	u32 regval;
	unsigned long flags;

	spin_lock_irqsave(&lp->hw_lock, flags);

	regval = dwceqos_read(lp, REG_DWCEQOS_MAC_RX_FLOW_CTRL);
	if (enable)
		regval |= DWCEQOS_MAC_RX_FLOW_CTRL_RFE;
	else
		regval &= ~DWCEQOS_MAC_RX_FLOW_CTRL_RFE;
	dwceqos_write(lp, REG_DWCEQOS_MAC_RX_FLOW_CTRL, regval);

	spin_unlock_irqrestore(&lp->hw_lock, flags);
}

static void dwceqos_set_tx_flowcontrol(struct net_local *lp, bool enable)
{
	u32 regval;
	unsigned long flags;

	spin_lock_irqsave(&lp->hw_lock, flags);

	/* MTL flow control */
	regval = dwceqos_read(lp, REG_DWCEQOS_MTL_RXQ0_OPER);
	if (enable)
		regval |= DWCEQOS_MTL_RXQ_EHFC;
	else
		regval &= ~DWCEQOS_MTL_RXQ_EHFC;

	dwceqos_write(lp, REG_DWCEQOS_MTL_RXQ0_OPER, regval);

	/* MAC flow control */
	regval = dwceqos_read(lp, REG_DWCEQOS_MAC_Q0_TX_FLOW);
	if (enable)
		regval |= DWCEQOS_MAC_Q0_TX_FLOW_TFE;
	else
		regval &= ~DWCEQOS_MAC_Q0_TX_FLOW_TFE;
	dwceqos_write(lp, REG_DWCEQOS_MAC_Q0_TX_FLOW, regval);

	spin_unlock_irqrestore(&lp->hw_lock, flags);
}

static void dwceqos_configure_flow_control(struct net_local *lp)
{
	u32 regval;
	unsigned long flags;
	int RQS, RFD, RFA;

	spin_lock_irqsave(&lp->hw_lock, flags);

	regval = dwceqos_read(lp, REG_DWCEQOS_MTL_RXQ0_OPER);

	/* The queue size is in units of 256 bytes. We want 512 bytes units for
	 * the threshold fields.
	 */
	RQS = ((regval >> 20) & 0x3FF) + 1;
	RQS /= 2;

	/* The thresholds are relative to a full queue, with a bias
	 * of 1 KiByte below full.
	 */
	RFD = RQS / 2 - 2;
	RFA = RQS / 8 - 2;

	regval = (regval & 0xFFF000FF) | (RFD << 14) | (RFA << 8);

	if (RFD >= 0 && RFA >= 0) {
		dwceqos_write(lp, REG_DWCEQOS_MTL_RXQ0_OPER, regval);
	} else {
		netdev_warn(lp->ndev,
			    "FIFO too small for flow control.");
	}

	regval = DWCEQOS_MAC_Q0_TX_FLOW_PT(256) |
		 DWCEQOS_MAC_Q0_TX_FLOW_PLT_4_SLOTS;

	dwceqos_write(lp, REG_DWCEQOS_MAC_Q0_TX_FLOW, regval);

	spin_unlock_irqrestore(&lp->hw_lock, flags);
}

static void dwceqos_configure_clock(struct net_local *lp)
{

	unsigned long rate_mhz = 0;
#if (GMAC_QUICK_CLK == 0)
	if (lp->apb_pclk)
		rate_mhz = clk_get_rate(lp->apb_pclk) / 1000000;
	else
		rate_mhz = lp->default_apb_pclk / 1000000;
#else
	rate_mhz = clk_mini_get_rate(CKG_MUX_GLB_APB)/ 1000000;

	if(rate_mhz == 0)
		rate_mhz = lp->default_apb_pclk / 1000000;
#endif
	BUG_ON(!rate_mhz);
	dwceqos_write(lp,
		      REG_DWCEQOS_MAC_1US_TIC_COUNTER,
		      DWCEQOS_MAC_1US_TIC_COUNTER_VAL(rate_mhz - 1));
}

static void dwceqos_configure_bus(struct net_local *lp)
{
	u32 sysbus_reg;

	/* N.B. We do not support the Fixed Burst mode because it
	 * opens a race window by making HW access to DMA descriptors
	 * non-atomic.
	 */

	sysbus_reg = DWCEQOS_DMA_SYSBUS_MODE_AAL;

	if (lp->bus_cfg.en_lpi)
		sysbus_reg |= DWCEQOS_DMA_SYSBUS_MODE_EN_LPI;

	if (lp->bus_cfg.burst_map)
		sysbus_reg |= DWCEQOS_DMA_SYSBUS_MODE_BURST(
			lp->bus_cfg.burst_map);
	else
		sysbus_reg |= DWCEQOS_DMA_SYSBUS_MODE_BURST(
			DWCEQOS_DMA_SYSBUS_MODE_BURST_DEFAULT);

	if (lp->bus_cfg.read_requests)
		sysbus_reg |= DWCEQOS_DMA_SYSBUS_MODE_RD_OSR_LIMIT(
			lp->bus_cfg.read_requests - 1);
	else
		sysbus_reg |= DWCEQOS_DMA_SYSBUS_MODE_RD_OSR_LIMIT(
			DWCEQOS_DMA_SYSBUS_MODE_RD_OSR_LIMIT_DEFAULT);

	if (lp->bus_cfg.write_requests)
		sysbus_reg |= DWCEQOS_DMA_SYSBUS_MODE_WR_OSR_LIMIT(
			lp->bus_cfg.write_requests - 1);
	else
		sysbus_reg |= DWCEQOS_DMA_SYSBUS_MODE_WR_OSR_LIMIT(
			DWCEQOS_DMA_SYSBUS_MODE_WR_OSR_LIMIT_DEFAULT);
	//set outstanding to max
	sysbus_reg |= (0xf<<24)|(0xf<<16);
	//disable axi low power interface
        sysbus_reg &= ~BIT(31);

	if (netif_msg_hw(lp))
		netdev_dbg(lp->ndev, "SysbusMode %#X\n", sysbus_reg);

	dwceqos_write(lp, REG_DWCEQOS_DMA_SYSBUS_MODE, sysbus_reg);
}

static void dwceqos_init_hw(struct net_local *lp)
{
	struct net_device *ndev = lp->ndev;
	struct device	*dev = &lp->pdev->dev;
	u32 regval, buswidth, dma_skip;
	int ret;

	/* Software reset */
	dwceqos_reset_hw(lp);
	/* MPHY CLK reset */
	if((lp->phy_interface == PHY_INTERFACE_MODE_MII) ||
		(lp->phy_interface == PHY_INTERFACE_MODE_RMII)) {
		if(lp->phy_type == INTERNAL_PHY){
			mutex_lock(&ndev->phydev->lock);
			dwceqos_mphy_clk_reset(ndev);
			mutex_unlock(&ndev->phydev->lock);
		}
	}
	dwceqos_configure_bus(lp);

	/* Probe data bus width, 32/64/128 bits. */
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_TXDESC_TAIL, 0xF);
	regval = dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_TXDESC_TAIL);
	buswidth = (regval ^ 0xF) + 1;

	/* Cache-align dma descriptors. */
	dma_skip = (sizeof(struct dwceqos_dma_desc) - 16) / buswidth;
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_CTRL,
		      DWCEQOS_DMA_CH_CTRL_DSL(dma_skip) |
		      DWCEQOS_DMA_CH_CTRL_PBLX8);

	/* Initialize DMA Channel 0 */
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_TXDESC_LEN, DWCEQOS_TX_DCNT - 1);
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_RXDESC_LEN, DWCEQOS_RX_DCNT - 1);
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_TXDESC_LIST,
		      (u32)lp->tx_descs_addr);
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_RXDESC_LIST,
		      (u32)lp->rx_descs_addr);

	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_TXDESC_TAIL,
		      lp->tx_descs_tail_addr);
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_RXDESC_TAIL,
		      lp->rx_descs_tail_addr);
	dwceqos_write(lp, REG_DWCEQOS_DMA_RX_WATHCDOG, lp->rx_irq_watchdog);

	if (lp->bus_cfg.tx_pbl)
		regval = DWCEQOS_DMA_CH_CTRL_PBL(lp->bus_cfg.tx_pbl);
	else
		regval = DWCEQOS_DMA_CH_CTRL_PBL(2);
#ifdef TSO_EN
	/* Enable TSO if the HW support it */
	if (lp->feature1 & DWCEQOS_MAC_HW_FEATURE1_TSOEN)
		regval |= DWCEQOS_DMA_CH_TX_TSE;
#endif
	//enable OSP
	regval |= DWCEQOS_DMA_CH_TX_OSP;

	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_TX_CTRL, regval);

	if (lp->bus_cfg.rx_pbl)
		regval = DWCEQOS_DMA_CH_CTRL_PBL(lp->bus_cfg.rx_pbl);
	else
		regval = DWCEQOS_DMA_CH_CTRL_PBL(2);

	regval |= DWCEQOS_DMA_CH_RX_CTRL_BUFSIZE(DWCEQOS_DWCEQOS_RX_BUF_SIZE);
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_RX_CTRL, regval);

	//enable rx packet flush
	regval |= BIT(31);
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_RX_CTRL, regval);

	/* Initialize MTL Queues */
	regval = DWCEQOS_MTL_SCHALG_STRICT;
	dwceqos_write(lp, REG_DWCEQOS_MTL_OPER, regval);

	regval = DWCEQOS_MTL_TXQ_SIZE(
			DWCEQOS_MAC_HW_FEATURE1_TXFIFOSIZE(lp->feature1)) |
		DWCEQOS_MTL_TXQ_TXQEN | DWCEQOS_MTL_TXQ_TSF |
		DWCEQOS_MTL_TXQ_TTC512;
	dwceqos_write(lp, REG_DWCEQOS_MTL_TXQ0_OPER, regval);

	regval = DWCEQOS_MTL_RXQ_SIZE(
			DWCEQOS_MAC_HW_FEATURE1_RXFIFOSIZE(lp->feature1)) | DWCEQOS_MTL_RXQ_RTC128;

	dwceqos_write(lp, REG_DWCEQOS_MTL_RXQ0_OPER, regval);

	dwceqos_configure_flow_control(lp);
       //tttttttttttt disable forward error package aaaaaaaaaaaaaaaa
       dwceqos_write(lp, REG_DWCEQOS_MTL_RXQ0_OPER, (dwceqos_read(lp, REG_DWCEQOS_MTL_RXQ0_OPER) & ~BIT(4)));


	/* Initialize MAC */
	dwceqos_set_umac_addr(lp, lp->ndev->dev_addr, 0);

	lp->eee_enabled = 0;

	dwceqos_configure_clock(lp);

	/* MMC counters */

	/* probe implemented counters */
	dwceqos_write(lp, REG_DWCEQOS_MMC_RXIRQMASK, ~0u);
	dwceqos_write(lp, REG_DWCEQOS_MMC_TXIRQMASK, ~0u);
	dwceqos_write(lp, REG_DWCEQOS_IPC_RXMASK, ~0u);
	lp->mmc_rx_counters_mask = dwceqos_read(lp, REG_DWCEQOS_MMC_RXIRQMASK);
	lp->mmc_tx_counters_mask = dwceqos_read(lp, REG_DWCEQOS_MMC_TXIRQMASK);
	lp->mmc_ipc_counters_mask = dwceqos_read(lp, REG_DWCEQOS_MMC_TXIRQMASK);

	dwceqos_write(lp, REG_DWCEQOS_MMC_CTRL, DWCEQOS_MMC_CTRL_CNTRST |
		DWCEQOS_MMC_CTRL_RSTONRD);
	dwceqos_enable_mmc_interrupt(lp);
	dwceqos_write(lp, REG_DWCEQOS_IPC_RXMASK, 0);

	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_IE, 0);
	dwceqos_write(lp, REG_DWCEQOS_MAC_IE, 0);

	if(lp->wol_irq) {
		//the inner pmt irq and sperate pmt irq is mutexed???
		//dwceqos_write(lp, REG_DWCEQOS_MAC_IE, BIT(4));
	}

	dwceqos_write(lp, REG_DWCEQOS_MAC_CFG, DWCEQOS_MAC_CFG_IPC |
		DWCEQOS_MAC_CFG_DM);

	if(lp->phy_interface == PHY_INTERFACE_MODE_RGMII) {
		//set mac_phy_intf_ctrl TC, transmit info to phy
		dwceqos_write(lp, 0x00f8, dwceqos_read(lp, 0x00f8) | BIT(0));
	}
	//disable receive own ,set DCRS and back-off limit
	dwceqos_write(lp, REG_DWCEQOS_MAC_CFG, dwceqos_read(lp, REG_DWCEQOS_MAC_CFG)| DWCEQOS_MAC_CFG_DO |DWCEQOS_MAC_CFG_DCRS | BIT(6));

	/*enable rx watchdog timeout*/
	dwceqos_write(lp, REG_DWCEQOS_MAC_WD_TO, BIT(8));

	/* Enable RX Q EN*/
	regval = dwceqos_read(lp, REG_DWCEQOS_MAC_RXQ0_CTRL0);
	dwceqos_write(lp, REG_DWCEQOS_MAC_RXQ0_CTRL0,
		      regval | 0x2);

	lp->phy_defer = false;
	lp->mac_suspend = true;
	mutex_lock(&ndev->phydev->lock);
	ret = phy_read_status(ndev->phydev);
	if (ret)
		dev_err(dev, "phy read status error %d\n", ret);
	else
		dwceqos_adjust_link(lp->ndev);
	mutex_unlock(&ndev->phydev->lock);
}

static void dwceqos_tx_reclaim(unsigned long data)
{
	struct net_device *ndev = (struct net_device *)data;
	struct net_local *lp = netdev_priv(ndev);
	unsigned int tx_bytes = 0;
	unsigned int tx_packets = 0;

	spin_lock(&lp->tx_lock);

	while (lp->tx_free < DWCEQOS_TX_DCNT) {
		struct dwceqos_dma_desc *dd = &lp->tx_descs[lp->tx_cur];
		struct ring_desc *rd = &lp->tx_skb[lp->tx_cur];

		/* Descriptor still being held by DMA ? */
		if (dd->des3 & DWCEQOS_DMA_TDES3_OWN)
			break;

		if (rd->mapping)
			dma_unmap_single(ndev->dev.parent, rd->mapping, rd->len,
					 DMA_TO_DEVICE);

		if (unlikely(rd->skb)) {
			++tx_packets;
			tx_bytes += rd->skb->len;
			dev_consume_skb_any(rd->skb);
		}

		rd->skb = NULL;
		rd->mapping = 0;
		lp->tx_free++;
		lp->tx_cur = (lp->tx_cur + 1) % DWCEQOS_TX_DCNT;

		if ((dd->des3 & DWCEQOS_DMA_TDES3_LD) &&
		    (dd->des3 & DWCEQOS_DMA_RDES3_ES)) {
			if (netif_msg_tx_err(lp))
				netdev_err(ndev, "TX Error, TDES3 = 0x%x\n",
					   dd->des3);
			if (netif_msg_hw(lp))
				print_status(lp);
		}
	}
	spin_unlock(&lp->tx_lock);

	netdev_completed_queue(ndev, tx_packets, tx_bytes);

	dwceqos_dma_enable_txirq(lp);
	netif_wake_queue(ndev);
}

static int dwceqos_rx(struct net_local *lp, int budget)
{
	dma_addr_t new_skb_baddr = 0;
	u32 n_descs = 0;
	struct sk_buff *skb;
	struct dwceqos_dma_desc *dd;
	struct sk_buff *new_skb;
	u32 len;

	while (n_descs < budget) {
		if (!dwceqos_packet_avail(lp))
			break;

		new_skb = netdev_alloc_skb(lp->ndev, DWCEQOS_RX_BUF_SIZE);
		if (!new_skb) {
			netdev_err(lp->ndev, "no memory for new sk_buff\n");
			break;
		}

		/* Get dma handle of skb->data */
		new_skb_baddr = (u32)dma_map_single(lp->ndev->dev.parent,
					new_skb->data,
					DWCEQOS_RX_BUF_SIZE,
					DMA_FROM_DEVICE);
		if (dma_mapping_error(lp->ndev->dev.parent, new_skb_baddr)) {
			netdev_err(lp->ndev, "DMA map error\n");
			dev_kfree_skb(new_skb);
			break;
		}

		/* Read descriptor data after reading owner bit. */
		dma_rmb();

		dd = &lp->rx_descs[lp->rx_cur];
		len = DWCEQOS_DMA_RDES3_PL(dd->des3);
		skb = lp->rx_skb[lp->rx_cur].skb;

		/* Unmap old buffer */
		dma_unmap_single(lp->ndev->dev.parent,
				 lp->rx_skb[lp->rx_cur].mapping,
				 lp->rx_skb[lp->rx_cur].len, DMA_FROM_DEVICE);

		/* Discard packet on reception error or bad checksum */
		if ((dd->des3 & DWCEQOS_DMA_RDES3_ES) ||
		    (dd->des1 & DWCEQOS_DMA_RDES1_IPCE)) {
			dev_kfree_skb(skb);
			skb = NULL;
		} else {
			skb_put(skb, len);
			skb->protocol = eth_type_trans(skb, lp->ndev);
			switch (dd->des1 & DWCEQOS_DMA_RDES1_PT) {
			case DWCEQOS_DMA_RDES1_PT_UDP:
			case DWCEQOS_DMA_RDES1_PT_TCP:
			case DWCEQOS_DMA_RDES1_PT_ICMP:
				skb->ip_summed = CHECKSUM_UNNECESSARY;
				break;
			default:
				skb->ip_summed = CHECKSUM_NONE;
				break;
			}
		}

		if (unlikely(!skb)) {
			if (netif_msg_rx_err(lp))
				netdev_dbg(lp->ndev, "rx error: des3=%X\n",
					   lp->rx_descs[lp->rx_cur].des3);
		} else {
			if (lp->bus_cfg.rx_gro)
				napi_gro_receive(&lp->napi, skb);
			else
				netif_receive_skb(skb);
		}

		lp->rx_descs[lp->rx_cur].des0 = new_skb_baddr;
		lp->rx_descs[lp->rx_cur].des1 = 0;
		lp->rx_descs[lp->rx_cur].des2 = 0;
		/* The DMA must observe des0/1/2 written before des3. */
		wmb();
		if (lp->rx_irq_watchdog) {
			lp->rx_descs[lp->rx_cur].des3 = DWCEQOS_DMA_RDES3_OWN  |
							DWCEQOS_DMA_RDES3_BUF1V;
		} else {
			lp->rx_descs[lp->rx_cur].des3 = DWCEQOS_DMA_RDES3_INTE |
							DWCEQOS_DMA_RDES3_OWN  |
							DWCEQOS_DMA_RDES3_BUF1V;
		}
		lp->rx_skb[lp->rx_cur].mapping = new_skb_baddr;
		lp->rx_skb[lp->rx_cur].len = DWCEQOS_RX_BUF_SIZE;
		lp->rx_skb[lp->rx_cur].skb = new_skb;

		n_descs++;
		lp->rx_cur = (lp->rx_cur + 1) % DWCEQOS_RX_DCNT;
	}

	/* Make sure any ownership update is written to the descriptors before
	 * DMA wakeup.
	 */
	wmb();

	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_STA, DWCEQOS_DMA_CH0_IS_RI);
	/* Wake up RX by writing tail pointer */
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_RXDESC_TAIL,
		      lp->rx_descs_tail_addr);

	return n_descs;
}

static int dwceqos_rx_poll(struct napi_struct *napi, int budget)
{
	struct net_local *lp = container_of(napi, struct net_local, napi);
	int work_done = 0;

	work_done = dwceqos_rx(lp, budget - work_done);
	if (!dwceqos_packet_avail(lp) && work_done < budget) {
		napi_complete(napi);
		dwceqos_dma_enable_rxirq(lp);
	} else {
		work_done = budget;
	}

	return work_done;
}

/* Reinitialize function if a TX timed out */
static void dwceqos_reinit_for_txtimeout(struct work_struct *data)
{
	struct net_local *lp = container_of(data, struct net_local,
		txtimeout_reinit);

	netdev_err(lp->ndev, "transmit timeout %d s, resetting...\n",
		   DWCEQOS_TX_TIMEOUT);

	if (netif_msg_hw(lp))
		print_status(lp);

	rtnl_lock();
	dwceqos_stop(lp->ndev);
	dwceqos_open(lp->ndev);
	rtnl_unlock();
}

/* DT Probing function called by main probe */
static inline int dwceqos_probe_config_dt(struct platform_device *pdev)
{
	struct net_device *ndev;
	struct net_local *lp;
	const void *mac_address;
	struct dwceqos_bus_cfg *bus_cfg;
	struct device_node *np = pdev->dev.of_node;

	ndev = platform_get_drvdata(pdev);
	lp = netdev_priv(ndev);
	bus_cfg = &lp->bus_cfg;

	/* Set the MAC address. */
	mac_address = of_get_mac_address(pdev->dev.of_node);
	if (mac_address)
		ether_addr_copy(ndev->dev_addr, mac_address);

	/*rx_irq_watchdog set*/
	of_property_read_u32(np, "rx_irq_watchdog", &lp->rx_irq_watchdog);
	lp->rx_irq_watchdog_old = lp->rx_irq_watchdog = 0x80;

	/* These are all optional parameters */
	lp->en_tx_lpi_clockgating =  of_property_read_bool(np,
		"snps,en-tx-lpi-clockgating");
	bus_cfg->en_lpi = of_property_read_bool(np, "snps,en-lpi");
	bus_cfg->en_rmii_out_inv = of_property_read_bool(np, "snps,en-rmii-inv");
	of_property_read_u32(np, "snps,write-requests",
			     &bus_cfg->write_requests);
	of_property_read_u32(np, "snps,read-requests", &bus_cfg->read_requests);
	of_property_read_u32(np, "snps,burst-map", &bus_cfg->burst_map);
	of_property_read_u32(np, "snps,txpbl", &bus_cfg->tx_pbl);
	of_property_read_u32(np, "snps,rxpbl", &bus_cfg->rx_pbl);
	of_property_read_u32(np, "snps,rxgro", &bus_cfg->rx_gro);
	netdev_dbg(ndev, "BusCfg: lpi:%u inv:%u wr:%u rr:%u bm:%X rxpbl:%u txpbl:%d rxgro:%d\n",
		   bus_cfg->en_lpi,
		   bus_cfg->en_rmii_out_inv,
		   bus_cfg->write_requests,
		   bus_cfg->read_requests,
		   bus_cfg->burst_map,
		   bus_cfg->rx_pbl,
		   bus_cfg->tx_pbl,
		   bus_cfg->rx_gro);

	return 0;
}

static int dwceqos_gphy_wait_link(struct net_device *ndev)
{
	struct phy_device *phydev = ndev->phydev;
	int reg;
	int link_time;
	int cable_time = 0;

retry_cable:
	if (phy_write(phydev, MII_PAGE, MII_PHYSR_PAGE) < 0)
		return -1;
	if (phy_read(phydev, MII_PHYSR) & MII_PHYSR_CABLE) {
		for (link_time = 0; link_time < GPHY_LINK_TIME_OUT; link_time++) {
			if (phy_write(phydev, MII_PAGE, 0) < 0)
				return -1;
			/* Do a fake read */
			reg = phy_read(phydev, MII_BMSR);
			if (reg < 0)
				return reg;
			/* Read link and autonegotiation status */
			reg = phy_read(phydev, MII_BMSR);
			if (reg < 0)
				return reg;

			if (reg & BMSR_LSTATUS) {
				netdev_dbg(ndev, "Wait Link Done %d\n", link_time);
				return 1;
			}

			if (phy_write(phydev, MII_PAGE, MII_PHYSR_PAGE) < 0)
				return -1;

			if (!(phy_read(phydev, MII_PHYSR) & MII_PHYSR_CABLE)) {
				netdev_dbg(ndev, "Cable Plug Out %d\n", link_time);
				cable_time = 0;
				goto retry_cable;
			}
			msleep(100);
		}
		netdev_info(ndev, "Wait Link Time Out\n");
	} else {
		msleep(100);
		if (++cable_time < GPHY_CABLE_TIME_OUT)
			goto retry_cable;
		netdev_info(ndev, "No Cable\n");
	}

	/* restore to default page 0 */
	if (phy_write(phydev, MII_PAGE, 0) < 0)
		return -1;

	return 0;
}

static int dwceqos_open(struct net_device *ndev)
{
	struct net_local *lp = netdev_priv(ndev);
	struct phy_driver *phydrv = to_phy_driver(ndev->phydev->mdio.dev.driver);
	int res;
	dwceqos_reset_state(lp);
	res = dwceqos_descriptor_init(lp);
	if (res) {
		netdev_err(ndev, "Unable to allocate DMA memory, rc %d\n", res);
		return res;
	}
	netdev_reset_queue(ndev);

	if(lp->phy_interface == PHY_INTERFACE_MODE_RGMII) {
		if(phydrv->phy_id == 0x001cc916){  //RTL8211F phy id
			res = dwceqos_gphy_wait_link(ndev);
			if (res < 0)
				netdev_err(ndev, "Wait phy link fail %d\n", res);
		}
	}

	/* The dwceqos reset state machine requires all phy clocks to complete,
	 * hence the unusual init order with phy_start first.
	 */
	lp->phy_defer = true;
	phy_start(ndev->phydev);
	dwceqos_init_hw(lp);
	napi_enable(&lp->napi);

	netif_start_queue(ndev);
	tasklet_enable(&lp->tx_bdreclaim_tasklet);

	/* Enable Interrupts -- do this only after we enable NAPI and the
	 * tasklet.
	 */
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_IE,
		      DWCEQOS_DMA_CH0_IE_NIE |
		      DWCEQOS_DMA_CH0_IE_RIE | DWCEQOS_DMA_CH0_IE_TIE |
		      DWCEQOS_DMA_CH0_IE_AIE |
		      DWCEQOS_DMA_CH0_IE_FBEE);

//		dwmac4_pmt(lp,lp->wolopts);
//		lp->irq_wake = 1;

	return 0;
}
#define MTL_DRAIN
#ifdef MTL_DRAIN

#if 0
static bool dweqos_is_rx_dma_suspended(struct net_local *lp)
{
	u32 reg;

	reg = dwceqos_read(lp, REG_DWCEQOS_DMA_DEBUG_ST0);
	reg = DMA_GET_RX_STATE_CH0(reg);

	return ((reg == DMA_RX_CH_SUSPENDED) || (reg == DMA_CH_STOPPED));
}

static void dwceqos_drain_rx_dma(struct net_local *lp)
{
	/* Wait for all pending RX buffers to be sent. Upper limit based
	 * on max frame size on a 10 Mbit link.
	 */
	size_t limit = (DWCEQOS_RX_DCNT * 1250) / 100;

	while (!dweqos_is_rx_dma_suspended(lp) && limit--)
		usleep_range(100, 200);

	if (!dweqos_is_rx_dma_suspended(lp))
		netdev_info(lp->ndev, "Drain RX DMA Fail REG_DWCEQOS_DMA_DEBUG_ST0 0x%x\n",
			dwceqos_read(lp, REG_DWCEQOS_DMA_DEBUG_ST0));
}
#endif
static bool dweqos_is_rxq_suspended(struct net_local *lp)
{
	u32 reg;

	reg = dwceqos_read(lp, REG_DWCEQOS_MTL_RXQ0_DEBUG_ST);

	return (MTL_GET_RXQ0_RXQSTS(reg) == DWCEQOS_MTL_RXQ0_RXQSTS_EMPTY) &&
		(!MTL_GET_RXQ0_PRXQ(reg));
}

static void dwceqos_drain_rxq(struct net_local *lp)
{
	/* Wait for all pending RX Queue buffers to be sent.
	 * Upper limit based on max frame size on a 10 Mbit link.
	 */
	size_t limit = (DWCEQOS_RX_DCNT * 1250) / 100;

	while (!dweqos_is_rxq_suspended(lp) && limit--)
		usleep_range(100, 200);

	if (!dweqos_is_rxq_suspended(lp))
		netdev_info(lp->ndev, " MTL_RXQ0_DEBUG_ST 0x%08x REG_DWCEQOS_MAC_CFG 0x%08x\n",
			dwceqos_read(lp, REG_DWCEQOS_MTL_RXQ0_DEBUG_ST),
			dwceqos_read(lp, REG_DWCEQOS_MAC_CFG));
}

static bool dweqos_is_txq_suspended(struct net_local *lp)
{
	u32 reg;

	reg = dwceqos_read(lp, REG_DWCEQOS_MTL_TXQ0_DEBUG_ST);

	return (MTL_GET_TXQ0_STATE(reg) != DWCEQOS_MTL_TXQ0_TRCSTS_READ) &&
		(!(reg & DWCEQOS_MTL_TXQ0_TXQSTS));
}

static void dwceqos_drain_txq(struct net_local *lp)
{
	/* Wait for all pending TX Queue buffers to be sent.
	 * Upper limit based on max frame size on a 10 Mbit link.
	 */
	size_t limit = (DWCEQOS_TX_DCNT * 1250) / 100;

	while (!dweqos_is_txq_suspended(lp) && limit--)
		usleep_range(100, 200);
	if (!dweqos_is_txq_suspended(lp))
		netdev_info(lp->ndev, "Drain TXQ Fail TXQ0_DEBUG_ST 0x%08x\n",
			dwceqos_read(lp, REG_DWCEQOS_MTL_TXQ0_DEBUG_ST));
}
#endif

static bool dweqos_is_tx_dma_suspended(struct net_local *lp)
{
	u32 reg;

	reg = dwceqos_read(lp, REG_DWCEQOS_DMA_DEBUG_ST0);
	reg = DMA_GET_TX_STATE_CH0(reg);

	return ((reg == DMA_TX_CH_SUSPENDED) || (reg == DMA_CH_STOPPED));
}


static void dwceqos_drain_tx_dma(struct net_local *lp)
{
	/* Wait for all pending TX buffers to be sent. Upper limit based
	 * on max frame size on a 10 Mbit link.
	 */
	size_t limit = (DWCEQOS_TX_DCNT * 1250) / 100;

	while (!dweqos_is_tx_dma_suspended(lp) && limit--)
		usleep_range(100, 200);

	if (!dweqos_is_tx_dma_suspended(lp))
			netdev_info(lp->ndev, "Drain TX DMA Fail REG_DWCEQOS_DMA_DEBUG_ST0 0x%x\n",
				dwceqos_read(lp, REG_DWCEQOS_DMA_DEBUG_ST0));

}

static void dweqos_disable_tx_dma(struct net_local *lp)
{
	u32 reg;

	reg = dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_TX_CTRL);
	reg &= ~DWCEQOS_DMA_CH_CTRL_START;
	dwceqos_write(lp,REG_DWCEQOS_DMA_CH0_TX_CTRL, reg);
}

void dweqos_disable_rx_dma(struct net_local *lp)
{
	u32 reg;

	reg = dwceqos_read(lp, REG_DWCEQOS_DMA_CH0_RX_CTRL);
	reg &= ~DWCEQOS_DMA_CH_CTRL_START;
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_RX_CTRL, reg);
}

static void dweqos_disable_mac(struct net_local *lp)
{
	u32 reg;

	reg = dwceqos_read(lp, REG_DWCEQOS_MAC_CFG);
	reg &= ~(DWCEQOS_MAC_CFG_TE | DWCEQOS_MAC_CFG_RE);
	dwceqos_write(lp, REG_DWCEQOS_MAC_CFG, reg);
}
static void dweqos_disable_mac_re(struct net_local *lp)
{
	u32 reg;

	reg = dwceqos_read(lp, REG_DWCEQOS_MAC_CFG);
	reg &= ~DWCEQOS_MAC_CFG_RE;
	dwceqos_write(lp, REG_DWCEQOS_MAC_CFG, reg);
}

static int dwceqos_stop(struct net_device *ndev)
{
	struct net_local *lp = netdev_priv(ndev);

	dwceqos_disable_all_irq(lp);
	tasklet_disable(&lp->tx_bdreclaim_tasklet);
	napi_disable(&lp->napi);

	/* Stop all tx before we drain the tx dma. */
	netif_tx_lock_bh(lp->ndev);
	netif_stop_queue(ndev);
	netif_tx_unlock_bh(lp->ndev);

	dwceqos_drain_tx_dma(lp);
	dweqos_disable_tx_dma(lp);
	dweqos_disable_mac(lp);
	dweqos_disable_rx_dma(lp);
	mdelay(100);
	dwceqos_reset_hw(lp);
	phy_stop(ndev->phydev);

	dwceqos_descriptor_free(lp);

	return 0;
}

static void dwceqos_dmadesc_set_ctx(struct net_local *lp,
				    unsigned short gso_size)
{
	struct dwceqos_dma_desc *dd = &lp->tx_descs[lp->tx_next];

	dd->des0 = 0;
	dd->des1 = 0;
	dd->des2 = gso_size;
	dd->des3 = DWCEQOS_DMA_TDES3_CTXT | DWCEQOS_DMA_TDES3_TCMSSV;

	lp->tx_next = (lp->tx_next + 1) % DWCEQOS_TX_DCNT;
}

static void dwceqos_tx_poll_demand(struct net_local *lp)
{
	dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_TXDESC_TAIL,
		      lp->tx_descs_tail_addr);
}

struct dwceqos_tx {
	size_t nr_descriptors;
	size_t initial_descriptor;
	size_t last_descriptor;
	size_t prev_gso_size;
	size_t network_header_len;
};

static void dwceqos_tx_prepare(struct sk_buff *skb, struct net_local *lp,
			       struct dwceqos_tx *tx)
{
	size_t n = 1;
	size_t i;

	if (skb_is_gso(skb) && skb_shinfo(skb)->gso_size != lp->gso_size)
		++n;

	tx->network_header_len = skb_transport_offset(skb);
	if (skb_is_gso(skb))
		tx->network_header_len += tcp_hdrlen(skb);

	if (skb_is_gso(skb) && (skb_headlen(skb) - tx->network_header_len) ) {
		n +=  ( (skb_headlen(skb) - tx->network_header_len)  - 1 ) / (TCP_FRAG_SIZE);
	}

	for (i = 0; i < skb_shinfo(skb)->nr_frags; ++i) {
		skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

		n +=  (skb_frag_size(frag) + BYTES_PER_DMA_DESC - 1) /
		       BYTES_PER_DMA_DESC;
	}

	tx->nr_descriptors = n;
	tx->initial_descriptor = lp->tx_next;
	tx->last_descriptor = lp->tx_next;
	tx->prev_gso_size = lp->gso_size;
}
static int dwceqos_tx_linear(struct sk_buff *skb, struct net_local *lp,
			     struct dwceqos_tx *tx)
{
	struct ring_desc *rd;
	struct dwceqos_dma_desc *dd;
	size_t payload_len = 0;
	dma_addr_t dma_handle;

	size_t consumed_size;
	
	if (skb_is_gso(skb) && skb_shinfo(skb)->gso_size != lp->gso_size) {
		dwceqos_dmadesc_set_ctx(lp, skb_shinfo(skb)->gso_size);
		lp->gso_size = skb_shinfo(skb)->gso_size;
	}

	dma_handle = dma_map_single(lp->ndev->dev.parent, skb->data,
				    skb_headlen(skb), DMA_TO_DEVICE);

	if (dma_mapping_error(lp->ndev->dev.parent, dma_handle)) {
		netdev_err(lp->ndev, "TX DMA Mapping error\n");
		return -ENOMEM;
	}

	rd = &lp->tx_skb[lp->tx_next];
	dd = &lp->tx_descs[lp->tx_next];

	rd->skb = NULL;
	rd->len = skb_headlen(skb);
	rd->mapping = dma_handle;

	/* Set up DMA Descriptor */
	dd->des0 = dma_handle;

	if (skb_is_gso(skb)) {
		payload_len = skb_headlen(skb) - tx->network_header_len;

		if (payload_len)
			dd->des1 = dma_handle + tx->network_header_len;
		if (payload_len <= TCP_FRAG_SIZE) {
			dd->des2 = tx->network_header_len |
				DWCEQOS_DMA_DES2_B2L(payload_len);
			dd->des3 = DWCEQOS_DMA_TDES3_TSE |
				DWCEQOS_DMA_DES3_THL((tcp_hdrlen(skb) / 4)) |
				(skb->len - tx->network_header_len);
		} else {

			dd->des2 = tx->network_header_len |
				DWCEQOS_DMA_DES2_B2L(TCP_FRAG_SIZE);
			dd->des3 = DWCEQOS_DMA_TDES3_TSE |
				DWCEQOS_DMA_DES3_THL((tcp_hdrlen(skb) / 4)) |
				(skb->len - tx->network_header_len);

			consumed_size = TCP_FRAG_SIZE;
		}


	} else {
		dd->des1 = 0;
		dd->des2 = skb_headlen(skb);
		dd->des3 = skb->len;

		switch (skb->ip_summed) {
		case CHECKSUM_PARTIAL:
			dd->des3 |= DWCEQOS_DMA_TDES3_CA;
		case CHECKSUM_NONE:
		case CHECKSUM_UNNECESSARY:
		case CHECKSUM_COMPLETE:
		default:
			break;
		}
	}

	dd->des3 |= DWCEQOS_DMA_TDES3_FD;
	if (lp->tx_next  != tx->initial_descriptor)
		dd->des3 |= DWCEQOS_DMA_TDES3_OWN;

	tx->last_descriptor = lp->tx_next;
	lp->tx_next = (lp->tx_next + 1) % DWCEQOS_TX_DCNT;

	//if the linear data is too long, split it
	if ((payload_len > TCP_FRAG_SIZE) && skb_is_gso(skb) ) {

		do {
			size_t dma_size = min_t(size_t, TCP_FRAG_SIZE,
						payload_len - consumed_size);

			dd = &lp->tx_descs[lp->tx_next];

			// Set DMA Descriptor fields
			dd->des0 = dma_handle + tx->network_header_len + consumed_size;
			dd->des1 = 0;
			dd->des2 = dma_size;

			dd->des3 = (skb->len - tx->network_header_len);

			dd->des3 |= DWCEQOS_DMA_TDES3_OWN;

			tx->last_descriptor = lp->tx_next;
			lp->tx_next = (lp->tx_next + 1) % DWCEQOS_TX_DCNT;
			consumed_size += dma_size;
		
		} while(consumed_size < payload_len);

	}

	return 0;
}

static int dwceqos_tx_frags(struct sk_buff *skb, struct net_local *lp,
			    struct dwceqos_tx *tx)
{
	struct ring_desc *rd = NULL;
	struct dwceqos_dma_desc *dd;
	dma_addr_t dma_handle;
	size_t i;

	/* Setup more ring and DMA descriptor if the packet is fragmented */
	for (i = 0; i < skb_shinfo(skb)->nr_frags; ++i) {
		skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
		size_t frag_size;
		size_t consumed_size;

		/* Map DMA Area */
		dma_handle = skb_frag_dma_map(lp->ndev->dev.parent, frag, 0,
					      skb_frag_size(frag),
					      DMA_TO_DEVICE);
		if (dma_mapping_error(lp->ndev->dev.parent, dma_handle)) {
			netdev_err(lp->ndev, "DMA Mapping error\n");
			return -ENOMEM;
		}

		/* order-3 fragments span more than one descriptor. */
		frag_size = skb_frag_size(frag);
		consumed_size = 0;
		while (consumed_size < frag_size) {
			size_t dma_size = min_t(size_t, 16376,
						frag_size - consumed_size);

			rd = &lp->tx_skb[lp->tx_next];
			memset(rd, 0, sizeof(*rd));

			dd = &lp->tx_descs[lp->tx_next];

			/* Set DMA Descriptor fields */
			dd->des0 = dma_handle + consumed_size;
			dd->des1 = 0;
			dd->des2 = dma_size;

			if (skb_is_gso(skb))
				dd->des3 = (skb->len - tx->network_header_len);
			else
				dd->des3 = skb->len;

			dd->des3 |= DWCEQOS_DMA_TDES3_OWN;

			tx->last_descriptor = lp->tx_next;
			lp->tx_next = (lp->tx_next + 1) % DWCEQOS_TX_DCNT;
			consumed_size += dma_size;
		}

		rd->len = skb_frag_size(frag);
		rd->mapping = dma_handle;
	}

	return 0;
}

static void dwceqos_tx_finalize(struct sk_buff *skb, struct net_local *lp,
				struct dwceqos_tx *tx)
{
	lp->tx_descs[tx->last_descriptor].des3 |= DWCEQOS_DMA_TDES3_LD;
	lp->tx_descs[tx->last_descriptor].des2 |= DWCEQOS_DMA_TDES2_IOC;

	lp->tx_skb[tx->last_descriptor].skb = skb;

	/* Make all descriptor updates visible to the DMA before setting the
	 * owner bit.
	 */
	wmb();

	lp->tx_descs[tx->initial_descriptor].des3 |= DWCEQOS_DMA_TDES3_OWN;

	/* Make the owner bit visible before TX wakeup. */
	wmb();

	dwceqos_tx_poll_demand(lp);
}

static void dwceqos_tx_rollback(struct net_local *lp, struct dwceqos_tx *tx)
{
	size_t i = tx->initial_descriptor;

	while (i != lp->tx_next) {
		if (lp->tx_skb[i].mapping)
			dma_unmap_single(lp->ndev->dev.parent,
					 lp->tx_skb[i].mapping,
					 lp->tx_skb[i].len,
					 DMA_TO_DEVICE);

		lp->tx_skb[i].mapping = 0;
		lp->tx_skb[i].skb = NULL;

		memset(&lp->tx_descs[i], 0, sizeof(lp->tx_descs[i]));

		i = (i + 1) % DWCEQOS_TX_DCNT;
	}

	lp->tx_next = tx->initial_descriptor;
	lp->gso_size = tx->prev_gso_size;
}

static int dwceqos_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	struct net_local *lp = netdev_priv(ndev);
	struct dwceqos_tx trans;
	int err;

	dwceqos_tx_prepare(skb, lp, &trans);
	if (lp->tx_free < trans.nr_descriptors) {

		netif_stop_queue(ndev);
		return NETDEV_TX_BUSY;
	}

	err = dwceqos_tx_linear(skb, lp, &trans);
	if (err)
		goto tx_error;

	err = dwceqos_tx_frags(skb, lp, &trans);
	if (err)
		goto tx_error;
	

	WARN_ON(lp->tx_next !=
		((trans.initial_descriptor + trans.nr_descriptors) %
		 DWCEQOS_TX_DCNT));

	spin_lock_bh(&lp->tx_lock);
	lp->tx_free -= trans.nr_descriptors;
	dwceqos_tx_finalize(skb, lp, &trans);
	netdev_sent_queue(ndev, skb->len);
	spin_unlock_bh(&lp->tx_lock);

	netif_trans_update(ndev);

	return 0;

tx_error:

	dwceqos_tx_rollback(lp, &trans);
	dev_kfree_skb(skb);

	return 0;
}

/* Set MAC address and then update HW accordingly */
static int dwceqos_set_mac_address(struct net_device *ndev, void *addr)
{
	struct net_local *lp = netdev_priv(ndev);
	struct sockaddr *hwaddr = (struct sockaddr *)addr;

	if (netif_running(ndev))
		return -EBUSY;

	if (!is_valid_ether_addr(hwaddr->sa_data))
		return -EADDRNOTAVAIL;

	memcpy(ndev->dev_addr, hwaddr->sa_data, ndev->addr_len);

	dwceqos_set_umac_addr(lp, lp->ndev->dev_addr, 0);
	return 0;
}

static void dwceqos_tx_timeout(struct net_device *ndev, unsigned int txqueue)
{
	struct net_local *lp = netdev_priv(ndev);

	queue_work(lp->txtimeout_handler_wq, &lp->txtimeout_reinit);
}

static void dwceqos_set_umac_addr(struct net_local *lp, unsigned char *addr,
				  unsigned int reg_n)
{
	unsigned long data;

	data = (addr[5] << 8) | addr[4];
	dwceqos_write(lp, DWCEQOS_ADDR_HIGH(reg_n),
		      data | DWCEQOS_MAC_MAC_ADDR_HI_EN);
	data = (addr[3] << 24) | (addr[2] << 16) | (addr[1] << 8) | addr[0];
	dwceqos_write(lp, DWCEQOS_ADDR_LOW(reg_n), data);
}

static void dwceqos_disable_umac_addr(struct net_local *lp, unsigned int reg_n)
{
	/* Do not disable MAC address 0 */
	if (reg_n != 0)
		dwceqos_write(lp, DWCEQOS_ADDR_HIGH(reg_n), 0);
}

static void dwceqos_set_rx_mode(struct net_device *ndev)
{
	struct net_local *lp = netdev_priv(ndev);
	u32 regval = 0;
	u32 mc_filter[2];
	int reg = 1;
	struct netdev_hw_addr *ha;
	unsigned int max_mac_addr;

	max_mac_addr = DWCEQOS_MAX_PERFECT_ADDRESSES(lp->feature1);

	if (ndev->flags & IFF_PROMISC) {
		regval = DWCEQOS_MAC_PKT_FILT_PR;
	} else if (((netdev_mc_count(ndev) > DWCEQOS_HASH_TABLE_SIZE) ||
				(ndev->flags & IFF_ALLMULTI))) {
		regval = DWCEQOS_MAC_PKT_FILT_PM;
		dwceqos_write(lp, REG_DWCEQOS_HASTABLE_LO, 0xffffffff);
		dwceqos_write(lp, REG_DWCEQOS_HASTABLE_HI, 0xffffffff);
	} else if (!netdev_mc_empty(ndev)) {
		regval = DWCEQOS_MAC_PKT_FILT_HMC;
		memset(mc_filter, 0, sizeof(mc_filter));
		netdev_for_each_mc_addr(ha, ndev) {
			/* The upper 6 bits of the calculated CRC are used to
			 * index the contens of the hash table
			 */
			int bit_nr = bitrev32(~crc32_le(~0, ha->addr, 6)) >> 26;
			/* The most significant bit determines the register
			 * to use (H/L) while the other 5 bits determine
			 * the bit within the register.
			 */
			mc_filter[bit_nr >> 5] |= 1 << (bit_nr & 31);
		}
		dwceqos_write(lp, REG_DWCEQOS_HASTABLE_LO, mc_filter[0]);
		dwceqos_write(lp, REG_DWCEQOS_HASTABLE_HI, mc_filter[1]);
	}
	if (netdev_uc_count(ndev) > max_mac_addr) {
		regval |= DWCEQOS_MAC_PKT_FILT_PR;
	} else {
		netdev_for_each_uc_addr(ha, ndev) {
			dwceqos_set_umac_addr(lp, ha->addr, reg);
			reg++;
		}
		for (; reg < DWCEQOS_MAX_PERFECT_ADDRESSES(lp->feature1); reg++)
			dwceqos_disable_umac_addr(lp, reg);
	}
	dwceqos_write(lp, REG_DWCEQOS_MAC_PKT_FILT, regval);
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void dwceqos_poll_controller(struct net_device *ndev)
{
	disable_irq(ndev->irq);
	dwceqos_interrupt(ndev->irq, ndev);
	enable_irq(ndev->irq);
}
#endif

static void dwceqos_read_mmc_counters(struct net_local *lp, u32 rx_mask,
				      u32 tx_mask, u32 lpc_mask)
{
	if (tx_mask & BIT(27))
		lp->mmc_counters.txlpitranscntr +=
			dwceqos_read(lp, DWC_MMC_TXLPITRANSCNTR);
	if (tx_mask & BIT(26))
		lp->mmc_counters.txpiuscntr +=
			dwceqos_read(lp, DWC_MMC_TXLPIUSCNTR);
	if (tx_mask & BIT(25))
		lp->mmc_counters.txoversize_g +=
			dwceqos_read(lp, DWC_MMC_TXOVERSIZE_G);
	if (tx_mask & BIT(24))
		lp->mmc_counters.txvlanpackets_g +=
			dwceqos_read(lp, DWC_MMC_TXVLANPACKETS_G);
	if (tx_mask & BIT(23))
		lp->mmc_counters.txpausepackets +=
			dwceqos_read(lp, DWC_MMC_TXPAUSEPACKETS);
	if (tx_mask & BIT(22))
		lp->mmc_counters.txexcessdef +=
			dwceqos_read(lp, DWC_MMC_TXEXCESSDEF);
	if (tx_mask & BIT(21))
		lp->mmc_counters.txpacketcount_g +=
			dwceqos_read(lp, DWC_MMC_TXPACKETCOUNT_G);
	if (tx_mask & BIT(20))
		lp->mmc_counters.txoctetcount_g +=
			dwceqos_read(lp, DWC_MMC_TXOCTETCOUNT_G);
	if (tx_mask & BIT(19))
		lp->mmc_counters.txcarriererror +=
			dwceqos_read(lp, DWC_MMC_TXCARRIERERROR);
	if (tx_mask & BIT(18))
		lp->mmc_counters.txexcesscol +=
			dwceqos_read(lp, DWC_MMC_TXEXCESSCOL);
	if (tx_mask & BIT(17))
		lp->mmc_counters.txlatecol +=
			dwceqos_read(lp, DWC_MMC_TXLATECOL);
	if (tx_mask & BIT(16))
		lp->mmc_counters.txdeferred +=
			dwceqos_read(lp, DWC_MMC_TXDEFERRED);
	if (tx_mask & BIT(15))
		lp->mmc_counters.txmulticol_g +=
			dwceqos_read(lp, DWC_MMC_TXMULTICOL_G);
	if (tx_mask & BIT(14))
		lp->mmc_counters.txsinglecol_g +=
			dwceqos_read(lp, DWC_MMC_TXSINGLECOL_G);
	if (tx_mask & BIT(13))
		lp->mmc_counters.txunderflowerror +=
			dwceqos_read(lp, DWC_MMC_TXUNDERFLOWERROR);
	if (tx_mask & BIT(12))
		lp->mmc_counters.txbroadcastpackets_gb +=
			dwceqos_read(lp, DWC_MMC_TXBROADCASTPACKETS_GB);
	if (tx_mask & BIT(11))
		lp->mmc_counters.txmulticastpackets_gb +=
			dwceqos_read(lp, DWC_MMC_TXMULTICASTPACKETS_GB);
	if (tx_mask & BIT(10))
		lp->mmc_counters.txunicastpackets_gb +=
			dwceqos_read(lp, DWC_MMC_TXUNICASTPACKETS_GB);
	if (tx_mask & BIT(9))
		lp->mmc_counters.tx1024tomaxoctets_gb +=
			dwceqos_read(lp, DWC_MMC_TX1024TOMAXOCTETS_GB);
	if (tx_mask & BIT(8))
		lp->mmc_counters.tx512to1023octets_gb +=
			dwceqos_read(lp, DWC_MMC_TX512TO1023OCTETS_GB);
	if (tx_mask & BIT(7))
		lp->mmc_counters.tx256to511octets_gb +=
			dwceqos_read(lp, DWC_MMC_TX256TO511OCTETS_GB);
	if (tx_mask & BIT(6))
		lp->mmc_counters.tx128to255octets_gb +=
			dwceqos_read(lp, DWC_MMC_TX128TO255OCTETS_GB);
	if (tx_mask & BIT(5))
		lp->mmc_counters.tx65to127octets_gb +=
			dwceqos_read(lp, DWC_MMC_TX65TO127OCTETS_GB);
	if (tx_mask & BIT(4))
		lp->mmc_counters.tx64octets_gb +=
			dwceqos_read(lp, DWC_MMC_TX64OCTETS_GB);
	if (tx_mask & BIT(3))
		lp->mmc_counters.txmulticastpackets_g +=
			dwceqos_read(lp, DWC_MMC_TXMULTICASTPACKETS_G);
	if (tx_mask & BIT(2))
		lp->mmc_counters.txbroadcastpackets_g +=
			dwceqos_read(lp, DWC_MMC_TXBROADCASTPACKETS_G);
	if (tx_mask & BIT(1))
		lp->mmc_counters.txpacketcount_gb +=
			dwceqos_read(lp, DWC_MMC_TXPACKETCOUNT_GB);
	if (tx_mask & BIT(0))
		lp->mmc_counters.txoctetcount_gb +=
			dwceqos_read(lp, DWC_MMC_TXOCTETCOUNT_GB);

	if (rx_mask & BIT(27))
		lp->mmc_counters.rxlpitranscntr +=
			dwceqos_read(lp, DWC_MMC_RXLPITRANSCNTR);
	if (rx_mask & BIT(26))
		lp->mmc_counters.rxlpiuscntr +=
			dwceqos_read(lp, DWC_MMC_RXLPIUSCNTR);
	if (rx_mask & BIT(25))
		lp->mmc_counters.rxctrlpackets_g +=
			dwceqos_read(lp, DWC_MMC_RXCTRLPACKETS_G);
	if (rx_mask & BIT(24))
		lp->mmc_counters.rxrcverror +=
			dwceqos_read(lp, DWC_MMC_RXRCVERROR);
	if (rx_mask & BIT(23))
		lp->mmc_counters.rxwatchdog +=
			dwceqos_read(lp, DWC_MMC_RXWATCHDOG);
	if (rx_mask & BIT(22))
		lp->mmc_counters.rxvlanpackets_gb +=
			dwceqos_read(lp, DWC_MMC_RXVLANPACKETS_GB);
	if (rx_mask & BIT(21))
		lp->mmc_counters.rxfifooverflow +=
			dwceqos_read(lp, DWC_MMC_RXFIFOOVERFLOW);
	if (rx_mask & BIT(20))
		lp->mmc_counters.rxpausepackets +=
			dwceqos_read(lp, DWC_MMC_RXPAUSEPACKETS);
	if (rx_mask & BIT(19))
		lp->mmc_counters.rxoutofrangetype +=
			dwceqos_read(lp, DWC_MMC_RXOUTOFRANGETYPE);
	if (rx_mask & BIT(18))
		lp->mmc_counters.rxlengtherror +=
			dwceqos_read(lp, DWC_MMC_RXLENGTHERROR);
	if (rx_mask & BIT(17))
		lp->mmc_counters.rxunicastpackets_g +=
			dwceqos_read(lp, DWC_MMC_RXUNICASTPACKETS_G);
	if (rx_mask & BIT(16))
		lp->mmc_counters.rx1024tomaxoctets_gb +=
			dwceqos_read(lp, DWC_MMC_RX1024TOMAXOCTETS_GB);
	if (rx_mask & BIT(15))
		lp->mmc_counters.rx512to1023octets_gb +=
			dwceqos_read(lp, DWC_MMC_RX512TO1023OCTETS_GB);
	if (rx_mask & BIT(14))
		lp->mmc_counters.rx256to511octets_gb +=
			dwceqos_read(lp, DWC_MMC_RX256TO511OCTETS_GB);
	if (rx_mask & BIT(13))
		lp->mmc_counters.rx128to255octets_gb +=
			dwceqos_read(lp, DWC_MMC_RX128TO255OCTETS_GB);
	if (rx_mask & BIT(12))
		lp->mmc_counters.rx65to127octets_gb +=
			dwceqos_read(lp, DWC_MMC_RX65TO127OCTETS_GB);
	if (rx_mask & BIT(11))
		lp->mmc_counters.rx64octets_gb +=
			dwceqos_read(lp, DWC_MMC_RX64OCTETS_GB);
	if (rx_mask & BIT(10))
		lp->mmc_counters.rxoversize_g +=
			dwceqos_read(lp, DWC_MMC_RXOVERSIZE_G);
	if (rx_mask & BIT(9))
		lp->mmc_counters.rxundersize_g +=
			dwceqos_read(lp, DWC_MMC_RXUNDERSIZE_G);
	if (rx_mask & BIT(8))
		lp->mmc_counters.rxjabbererror +=
			dwceqos_read(lp, DWC_MMC_RXJABBERERROR);
	if (rx_mask & BIT(7))
		lp->mmc_counters.rxrunterror +=
			dwceqos_read(lp, DWC_MMC_RXRUNTERROR);
	if (rx_mask & BIT(6))
		lp->mmc_counters.rxalignmenterror +=
			dwceqos_read(lp, DWC_MMC_RXALIGNMENTERROR);
	if (rx_mask & BIT(5))
		lp->mmc_counters.rxcrcerror +=
			dwceqos_read(lp, DWC_MMC_RXCRCERROR);
	if (rx_mask & BIT(4))
		lp->mmc_counters.rxmulticastpackets_g +=
			dwceqos_read(lp, DWC_MMC_RXMULTICASTPACKETS_G);
	if (rx_mask & BIT(3))
		lp->mmc_counters.rxbroadcastpackets_g +=
			dwceqos_read(lp, DWC_MMC_RXBROADCASTPACKETS_G);
	if (rx_mask & BIT(2))
		lp->mmc_counters.rxoctetcount_g +=
			dwceqos_read(lp, DWC_MMC_RXOCTETCOUNT_G);
	if (rx_mask & BIT(1))
		lp->mmc_counters.rxoctetcount_gb +=
			dwceqos_read(lp, DWC_MMC_RXOCTETCOUNT_GB);
	if (rx_mask & BIT(0))
		lp->mmc_counters.rxpacketcount_gb +=
			dwceqos_read(lp, DWC_MMC_RXPACKETCOUNT_GB);

	if (lpc_mask & BIT(29))
		lp->mmc_counters.rxicmpero +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXICMPERO);
	if (lpc_mask & BIT(28))
		lp->mmc_counters.rxicmpgo +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXICMPGO);
	if (lpc_mask & BIT(27))
		lp->mmc_counters.rxtcpero +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXTCPERO);
	if (lpc_mask & BIT(26))
		lp->mmc_counters.rxtcpgo +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXTCPGO);
	if (lpc_mask & BIT(25))
		lp->mmc_counters.rxudpero +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXUDPERO);
	if (lpc_mask & BIT(24))
		lp->mmc_counters.rxudpo +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXUDPO);
	if (lpc_mask & BIT(23))
		lp->mmc_counters.rxipv6nopayo +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV6NOPAYO);
	if (lpc_mask & BIT(22))
		lp->mmc_counters.rxipv6hero +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV6HERO);
	if (lpc_mask & BIT(21))
		lp->mmc_counters.rxipv6go +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV6GO);
	if (lpc_mask & BIT(20))
		lp->mmc_counters.rxipv4udsblo +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV4UDSBLO);
	if (lpc_mask & BIT(19))
		lp->mmc_counters.rxipv4frao +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV4FRAO);
	if (lpc_mask & BIT(18))
		lp->mmc_counters.rxipv4nopayo +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV4NOPAYO);
	if (lpc_mask & BIT(17))
		lp->mmc_counters.rxipv4hero +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV4HERO);
	if (lpc_mask & BIT(16))
		lp->mmc_counters.rxipv4go +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV4GO);
	if (lpc_mask & BIT(13))
		lp->mmc_counters.rxicmperp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXICMPERP);
	if (lpc_mask & BIT(12))
		lp->mmc_counters.rxicmpgp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXICMPGP);
	if (lpc_mask & BIT(11))
		lp->mmc_counters.rxtcperp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXTCPERP);
	if (lpc_mask & BIT(10))
		lp->mmc_counters.rxtcpgp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXTCPGP);
	if (lpc_mask & BIT(9))
		lp->mmc_counters.rxudperp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXUDPERP);
	if (lpc_mask & BIT(8))
		lp->mmc_counters.rxudpgp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXUDPGP);
	if (lpc_mask & BIT(7))
		lp->mmc_counters.rxipv6nopayp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV6NOPAYP);
	if (lpc_mask & BIT(6))
		lp->mmc_counters.rxipv6herp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV6HERP);
	if (lpc_mask & BIT(5))
		lp->mmc_counters.rxipv6gp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV6GP);
	if (lpc_mask & BIT(4))
		lp->mmc_counters.rxipv4udsblp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV4UDSBLP);
	if (lpc_mask & BIT(3))
		lp->mmc_counters.rxipv4fragp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV4FRAGP);
	if (lpc_mask & BIT(2))
		lp->mmc_counters.rxipv4nopayp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV4NOPAYP);
	if (lpc_mask & BIT(1))
		lp->mmc_counters.rxipv4herp +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV4HERP);
	if (lpc_mask & BIT(0))
		lp->mmc_counters.rxipv4p +=
			dwceqos_read(lp, REG_DWCEQOS_IPC_RXIPV4P);
}

static void 
dwceqos_get_stats64(struct net_device *ndev, struct rtnl_link_stats64 *s) 
{
	unsigned long flags;
	struct net_local *lp = netdev_priv(ndev);
	struct dwceqos_mmc_counters *hwstats = &lp->mmc_counters;

	spin_lock_irqsave(&lp->stats_lock, flags);
	dwceqos_read_mmc_counters(lp, lp->mmc_rx_counters_mask,
				  lp->mmc_tx_counters_mask,
				  lp->mmc_ipc_counters_mask);
	spin_unlock_irqrestore(&lp->stats_lock, flags);

	s->rx_packets = hwstats->rxpacketcount_gb;
	s->rx_bytes = hwstats->rxoctetcount_gb;
	s->rx_errors = hwstats->rxpacketcount_gb -
		hwstats->rxbroadcastpackets_g -
		hwstats->rxmulticastpackets_g -
		hwstats->rxunicastpackets_g;
	s->multicast = hwstats->rxmulticastpackets_g;
	s->rx_length_errors = hwstats->rxlengtherror;
	s->rx_crc_errors = hwstats->rxcrcerror;
	s->rx_fifo_errors = hwstats->rxfifooverflow;

	s->tx_packets = hwstats->txpacketcount_gb;
	s->tx_bytes = hwstats->txoctetcount_gb;

	if (lp->mmc_tx_counters_mask & BIT(21))
		s->tx_errors = hwstats->txpacketcount_gb -
			hwstats->txpacketcount_g;
	else
		s->tx_errors = hwstats->txunderflowerror +
			hwstats->txcarriererror;


}

static void
dwceqos_get_drvinfo(struct net_device *ndev, struct ethtool_drvinfo *ed)
{
	const struct net_local *lp = netdev_priv(ndev);
	struct device	*dev = &lp->pdev->dev;
	int src_strlen = strlen(lp->pdev->dev.driver->name);

	if (src_strlen < sizeof(ed->driver))
		strcpy(ed->driver, lp->pdev->dev.driver->name);
	else
		dev_err(dev, "driver name string length error %d\n", src_strlen);
	strcpy(ed->version, DRIVER_VERSION);
}

static void dwceqos_get_pauseparam(struct net_device *ndev,
				   struct ethtool_pauseparam *pp)
{
	const struct net_local *lp = netdev_priv(ndev);

	pp->autoneg = lp->flowcontrol.autoneg;
	pp->tx_pause = lp->flowcontrol.tx;
	pp->rx_pause = lp->flowcontrol.rx;
}

static int dwceqos_set_pauseparam(struct net_device *ndev,
				  struct ethtool_pauseparam *pp)
{
	struct net_local *lp = netdev_priv(ndev);
	int ret = 0;

	lp->flowcontrol.autoneg = pp->autoneg;
	if (pp->autoneg) {
		//ndev->phydev->advertising |= ADVERTISED_Pause;
		linkmode_set_bit(ETHTOOL_LINK_MODE_Pause_BIT, ndev->phydev->advertising);
		//ndev->phydev->advertising |= ADVERTISED_Asym_Pause;
		linkmode_set_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT, ndev->phydev->advertising);
	} else {
		//ndev->phydev->advertising &= ~ADVERTISED_Pause;
		linkmode_clear_bit(ETHTOOL_LINK_MODE_Pause_BIT, ndev->phydev->advertising);
		//ndev->phydev->advertising &= ~ADVERTISED_Asym_Pause;
		linkmode_clear_bit(ETHTOOL_LINK_MODE_Asym_Pause_BIT, ndev->phydev->advertising);
		lp->flowcontrol.rx = pp->rx_pause;
		lp->flowcontrol.tx = pp->tx_pause;
	}

	if (netif_running(ndev))
		ret = phy_start_aneg(ndev->phydev);

	return ret;
}

static void dwceqos_get_strings(struct net_device *ndev, u32 stringset,
				u8 *data)
{
	size_t i;

	if (stringset != ETH_SS_STATS)
		return;

	for (i = 0; i < ARRAY_SIZE(dwceqos_ethtool_stats); ++i) {
		memcpy(data, dwceqos_ethtool_stats[i].stat_name,
		       ETH_GSTRING_LEN);
		data += ETH_GSTRING_LEN;
	}
}

static void dwceqos_get_ethtool_stats(struct net_device *ndev,
				      struct ethtool_stats *stats, u64 *data)
{
	struct net_local *lp = netdev_priv(ndev);
	unsigned long flags;
	size_t i;
	u8 *mmcstat = (u8 *)&lp->mmc_counters;

	spin_lock_irqsave(&lp->stats_lock, flags);
	dwceqos_read_mmc_counters(lp, lp->mmc_rx_counters_mask,
				  lp->mmc_tx_counters_mask,
				  lp->mmc_ipc_counters_mask);
	spin_unlock_irqrestore(&lp->stats_lock, flags);

	for (i = 0; i < ARRAY_SIZE(dwceqos_ethtool_stats); ++i) {
		memcpy(data,
		       mmcstat + dwceqos_ethtool_stats[i].offset,
		       sizeof(u64));
		data++;
	}
}

static int dwceqos_get_sset_count(struct net_device *ndev, int sset)
{
	if (sset == ETH_SS_STATS)
		return ARRAY_SIZE(dwceqos_ethtool_stats);

	return -EOPNOTSUPP;
}

static void dwceqos_get_regs(struct net_device *dev, struct ethtool_regs *regs,
			     void *space)
{
	const struct net_local *lp = netdev_priv(dev);
	u32 *reg_space = (u32 *)space;
	int reg_offset;
	int reg_ix = 0;

	/* MAC registers */
	for (reg_offset = START_MAC_REG_OFFSET;
		reg_offset <= MAX_DMA_REG_OFFSET; reg_offset += 4) {
		reg_space[reg_ix] = dwceqos_read(lp, reg_offset);
		reg_ix++;
	}
	/* MTL registers */
	for (reg_offset = START_MTL_REG_OFFSET;
		reg_offset <= MAX_MTL_REG_OFFSET; reg_offset += 4) {
		reg_space[reg_ix] = dwceqos_read(lp, reg_offset);
		reg_ix++;
	}

	/* DMA registers */
	for (reg_offset = START_DMA_REG_OFFSET;
		reg_offset <= MAX_DMA_REG_OFFSET; reg_offset += 4) {
		reg_space[reg_ix] = dwceqos_read(lp, reg_offset);
		reg_ix++;
	}

	BUG_ON(4 * reg_ix > REG_SPACE_SIZE);
}

static int dwceqos_get_regs_len(struct net_device *dev)
{
	return REG_SPACE_SIZE;
}

static inline const char *dwceqos_get_rx_lpi_state(u32 lpi_ctrl)
{
	return (lpi_ctrl & DWCEQOS_MAC_LPI_CTRL_STATUS_RLPIST) ? "on" : "off";
}

static inline const char *dwceqos_get_tx_lpi_state(u32 lpi_ctrl)
{
	return (lpi_ctrl & DWCEQOS_MAC_LPI_CTRL_STATUS_TLPIST) ? "on" : "off";
}

static int dwceqos_get_eee(struct net_device *ndev, struct ethtool_eee *edata)
{
	struct net_local *lp = netdev_priv(ndev);
	u32 lpi_status;
	u32 lpi_enabled;

	if (!(lp->feature0 & DWCEQOS_MAC_HW_FEATURE0_EEESEL))
		return -EOPNOTSUPP;

	edata->eee_active  = lp->eee_active;
	edata->eee_enabled = lp->eee_enabled;
	edata->tx_lpi_timer = dwceqos_read(lp, REG_DWCEQOS_MAC_LPI_ENTRY_TIMER);
	lpi_status = dwceqos_read(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS);
	lpi_enabled = !!(lpi_status & DWCEQOS_MAC_LPI_CTRL_STATUS_LIPTXA);
	edata->tx_lpi_enabled = lpi_enabled;

	if (netif_msg_hw(lp)) {
		u32 regval;

		regval = dwceqos_read(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS);

		netdev_info(lp->ndev, "MAC LPI State: RX:%s TX:%s\n",
			    dwceqos_get_rx_lpi_state(regval),
			    dwceqos_get_tx_lpi_state(regval));
	}

	return phy_ethtool_get_eee(ndev->phydev, edata);
}

static int dwceqos_set_eee(struct net_device *ndev, struct ethtool_eee *edata)
{
	struct net_local *lp = netdev_priv(ndev);
	u32 regval;
	unsigned long flags;

	if (!(lp->feature0 & DWCEQOS_MAC_HW_FEATURE0_EEESEL))
		return -EOPNOTSUPP;

	if (edata->eee_enabled && !lp->eee_active)
		return -EOPNOTSUPP;

	if (edata->tx_lpi_enabled) {
		if (edata->tx_lpi_timer < DWCEQOS_LPI_TIMER_MIN ||
		    edata->tx_lpi_timer > DWCEQOS_LPI_TIMER_MAX)
			return -EINVAL;
	}

	lp->eee_enabled = edata->eee_enabled;

	if (edata->eee_enabled && edata->tx_lpi_enabled) {
		dwceqos_write(lp, REG_DWCEQOS_MAC_LPI_ENTRY_TIMER,
			      edata->tx_lpi_timer);

		spin_lock_irqsave(&lp->hw_lock, flags);
		regval = dwceqos_read(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS);
		regval |= DWCEQOS_LPI_CTRL_ENABLE_EEE;
		if (lp->en_tx_lpi_clockgating)
			regval |= DWCEQOS_MAC_LPI_CTRL_STATUS_LPITCSE;
		dwceqos_write(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS, regval);
		spin_unlock_irqrestore(&lp->hw_lock, flags);
	} else {
		spin_lock_irqsave(&lp->hw_lock, flags);
		regval = dwceqos_read(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS);
		regval &= ~DWCEQOS_LPI_CTRL_ENABLE_EEE;
		dwceqos_write(lp, REG_DWCEQOS_MAC_LPI_CTRL_STATUS, regval);
		spin_unlock_irqrestore(&lp->hw_lock, flags);
	}

	return phy_ethtool_set_eee(ndev->phydev, edata);
}

static u32 dwceqos_get_msglevel(struct net_device *ndev)
{
	const struct net_local *lp = netdev_priv(ndev);

	return lp->msg_enable;
}

static void dwceqos_set_msglevel(struct net_device *ndev, u32 msglevel)
{
	struct net_local *lp = netdev_priv(ndev);

	lp->msg_enable = msglevel;
}
#define MTU_MINIMUN 100
static int dwceqos_net_change_mtu(struct net_device *dev, int new_mtu)
{

	if (new_mtu < MTU_MINIMUN || new_mtu > ETH_DATA_LEN)
		return -EINVAL;

	dev->mtu = new_mtu;

	return 0;
}
/* Currently only support WOL through Magic packet. */
static void stmmac_get_wol(struct net_device *dev, struct ethtool_wolinfo *wol)
{
	struct net_local *lp = netdev_priv(dev);

	if (!lp->pmt) {
		printk(KERN_ERR "%s not support wol\n",dev->name);
	}		

	if (device_can_wakeup(&lp->pdev->dev)) {
		wol->supported = WAKE_MAGIC | WAKE_UCAST;
		if ( !lp->pmt_magic_frame)
			wol->supported &= ~WAKE_MAGIC;
		wol->wolopts = lp->wolopts;
	}
}

static int stmmac_set_wol(struct net_device *dev, struct ethtool_wolinfo *wol)
{
	struct net_local *lp = netdev_priv(dev);
	u32 support = WAKE_MAGIC | WAKE_UCAST;

	if (!device_can_wakeup(&lp->pdev->dev))
		return -EOPNOTSUPP;

	if (!lp->pmt) {
		printk(KERN_ERR "%s not support wol\n",dev->name);

		return 0;
	}

	/* By default almost all GMAC devices support the WoL via
	 * magic frame but we can disable it if the HW capability
	 * register shows no support for pmt_magic_frame. */
	if (!lp->pmt_magic_frame)
		wol->wolopts &= ~WAKE_MAGIC;

	if (wol->wolopts & ~support)
		return -EINVAL;

	if (wol->wolopts) {
		pr_info("stmmac: wakeup enable\n");
		device_set_wakeup_enable(&lp->pdev->dev, 1);
		enable_irq_wake(lp->wol_irq);
	} else {
		device_set_wakeup_enable(&lp->pdev->dev, 0);
		disable_irq_wake(lp->wol_irq);
	}

	lp->wolopts = wol->wolopts;

	return 0;
}


static const struct ethtool_ops dwceqos_ethtool_ops = {
	.get_drvinfo    = dwceqos_get_drvinfo,
	.get_link       = ethtool_op_get_link,
	.get_pauseparam = dwceqos_get_pauseparam,
	.set_pauseparam = dwceqos_set_pauseparam,
	.get_strings    = dwceqos_get_strings,
	.get_ethtool_stats = dwceqos_get_ethtool_stats,
	.get_sset_count = dwceqos_get_sset_count,
	.get_regs       = dwceqos_get_regs,
	.get_regs_len   = dwceqos_get_regs_len,
	.get_eee        = dwceqos_get_eee,
	.set_eee        = dwceqos_set_eee,
	.get_msglevel   = dwceqos_get_msglevel,
	.set_msglevel   = dwceqos_set_msglevel,
	.get_link_ksettings = phy_ethtool_get_link_ksettings,
	.set_link_ksettings = phy_ethtool_set_link_ksettings,
	.get_wol = stmmac_get_wol,
	.set_wol = stmmac_set_wol,
};

static const struct net_device_ops netdev_ops = {
	.ndo_open		= dwceqos_open,
	.ndo_stop		= dwceqos_stop,
	.ndo_start_xmit		= dwceqos_start_xmit,
	.ndo_set_rx_mode	= dwceqos_set_rx_mode,
	.ndo_set_mac_address	= dwceqos_set_mac_address,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= dwceqos_poll_controller,
#endif
	.ndo_do_ioctl		= dwceqos_ioctl,
	.ndo_tx_timeout		= dwceqos_tx_timeout,
	.ndo_get_stats64	= dwceqos_get_stats64,
	.ndo_change_mtu         = dwceqos_net_change_mtu,
	//.ndo_change_mtu         = eth_change_mtu,
};

static int dwceqos_haps_apclk_init(struct net_local *lp)
{
	struct device_node *np = lp->pdev->dev.of_node;

	lp->apb_pclk = NULL;

	return of_property_read_u32(np, "molchip,haps-apclk",
			     &lp->default_apb_pclk);
}

#ifndef CONFIG_MOLCHIP_FASTBOOT
static int dwceqos_chip_apclk_init(struct net_local *lp)
{
	int ret = 0;

#if (GMAC_QUICK_CLK == 0)
	struct device	*dev = &lp->pdev->dev;

	lp->apb_pclk = devm_clk_get(dev, "apb_pclk");
	if (IS_ERR(lp->apb_pclk)) {
		dev_err(dev, "apb_pclk clock not found.\n");
		lp->apb_pclk = NULL;
		return PTR_ERR(lp->apb_pclk);
	}
#endif
	return ret;
}
#endif

//#define REG_GMAC0_TXDLL_CTRL0 (lp->gmac_dpll_base)

struct phy_dpll_iteam {
	u32 phy_id;
	int ctrl_id;
	int tx_delay;
	int rx_delay;
	int	negrx_delay;
};

struct phy_dpll_iteam gmac_dpll_config_table[] = {
	/*
	{0x1cc916,0x0,0x2e,0x23,0xa},
	{0x1cc916,0x1,0x2e,0x26,0xa}
	*/
};

static int scan_dpll_table(struct net_local *lp)
{
	int i = 0;
	struct phy_dpll_iteam *temp;
	struct net_device * net_dev = lp->ndev;
	struct device *dev = &lp->pdev->dev;
	u32 phy_id = net_dev->phydev->phy_id;
	
	if (lp->ctrl_id < 0) {
		dev_err(dev, "error gmac ctrl id\n");
		return -1;
	}

	for (i = 0; i < ARRAY_SIZE(gmac_dpll_config_table); i ++) {
		temp = &gmac_dpll_config_table[i];
		if ( (temp->phy_id == phy_id) && (temp->ctrl_id == lp->ctrl_id) ) {
			lp->tx_delay = temp->tx_delay;
			lp->rx_delay = temp->rx_delay;
			lp->negrx_delay = temp->negrx_delay;
			dev_dbg(dev, "\nfind phy_id:0x%x ctrl_id:0x%x, tx_delay:0x%x,rx_delay:0x%x,negrx_delay:0x%x\n",
				phy_id, lp->ctrl_id, lp->tx_delay, lp->rx_delay, lp->negrx_delay);
			return 0;
		}
		dev_dbg(dev, "\nphy_id:0x%x ctrl_id:0x%x, tx_delay:0x%x,rx_delay:0x%x,negrx_delay:0x%x\n",
		temp->phy_id, temp->ctrl_id, temp->tx_delay, temp->rx_delay, temp->negrx_delay);
	}

	return -2;
}


/*dpll V2*/

static int bit_modify_rdata(u32 *val, u32 bit_idx, u32 bit_mask, u32 bit_val)
{
        *val = (*val & (~(bit_mask << bit_idx)));
        *val = (*val | (bit_val << bit_idx));
        printk(KERN_ERR "set val:0x%x\n",*val);
        return 0;
}

static int config_txdll(struct net_local *lp, u8 val)
{
        u32 rdata;
        rdata = readl(lp->dpll_addr_map.txdpll_base);
        printk(KERN_ERR "tx set=0x%08x\n",rdata);

        bit_modify_rdata(&rdata, 9, 1, 0);
        writel(rdata,lp->dpll_addr_map.txdpll_base);
        bit_modify_rdata(&rdata, 11, 1, 1);
        writel(rdata,lp->dpll_addr_map.txdpll_base);
        bit_modify_rdata(&rdata, 0, 0xff, val);
        writel(rdata,lp->dpll_addr_map.txdpll_base);
        bit_modify_rdata(&rdata, 8, 0x1, 1);
        writel(rdata,lp->dpll_addr_map.txdpll_base);
        bit_modify_rdata(&rdata, 8, 0x1, 0);
        writel(rdata,lp->dpll_addr_map.txdpll_base);
        bit_modify_rdata(&rdata, 11, 1, 0);
        writel(rdata,lp->dpll_addr_map.txdpll_base);
        bit_modify_rdata(&rdata, 9, 1, 1);
        writel(rdata,lp->dpll_addr_map.txdpll_base);

        return 0;
}

static int config_rxdll(struct net_local *lp, u8 val)
{
        u32 rdata;
        rdata = readl(lp->dpll_addr_map.rxdpll_base);
        printk(KERN_ERR "rx set=0x%08x\n",rdata);

        bit_modify_rdata(&rdata, 21, 1, 0);
        writel(rdata,lp->dpll_addr_map.rxdpll_base);
        bit_modify_rdata(&rdata, 23, 1, 1);
        writel(rdata,lp->dpll_addr_map.rxdpll_base);
        bit_modify_rdata(&rdata, 12, 0xff, val);
        writel(rdata,lp->dpll_addr_map.rxdpll_base);
        bit_modify_rdata(&rdata, 20, 0x1, 1);
        writel(rdata,lp->dpll_addr_map.rxdpll_base);
        bit_modify_rdata(&rdata, 20, 0x1, 0);
        writel(rdata,lp->dpll_addr_map.rxdpll_base);
        bit_modify_rdata(&rdata, 23, 1, 0);
        writel(rdata,lp->dpll_addr_map.rxdpll_base);
        bit_modify_rdata(&rdata, 21, 1, 1);
        writel(rdata,lp->dpll_addr_map.rxdpll_base);

        return 0;
}

static int config_rxdll_neg(struct net_local *lp, u8 val)
{
        u32 rdata;
        rdata = readl(lp->dpll_addr_map.negrxdpll_base);
        printk(KERN_ERR "rxneg set=0x%08x\n",rdata);

        bit_modify_rdata(&rdata, 9, 1, 0);
        writel(rdata,lp->dpll_addr_map.negrxdpll_base);
        bit_modify_rdata(&rdata, 11, 1, 1);
        writel(rdata,lp->dpll_addr_map.negrxdpll_base);
        bit_modify_rdata(&rdata, 0, 0xff, val);
        writel(rdata,lp->dpll_addr_map.negrxdpll_base);
        bit_modify_rdata(&rdata, 8, 0x1, 1);
        writel(rdata,lp->dpll_addr_map.negrxdpll_base);
        bit_modify_rdata(&rdata, 8, 0x1, 0);
        writel(rdata,lp->dpll_addr_map.negrxdpll_base);
        bit_modify_rdata(&rdata, 11, 1, 0);
        writel(rdata,lp->dpll_addr_map.negrxdpll_base);
        bit_modify_rdata(&rdata, 9, 1, 1);
        writel(rdata,lp->dpll_addr_map.negrxdpll_base);

        return 0;
}

static int config_dpll_v2(struct net_local *lp)
{
	struct net_device *ndev = lp->ndev;
	printk(KERN_ERR "\n ctrl_id:0x%x, tx_delay:0x%x,rx_delay:0x%x,negrx_delay:0x%x\n",
		lp->ctrl_id, lp->tx_delay, lp->rx_delay, lp->negrx_delay);
	if ((lp->dpll_addr_map.txdpll_base != NULL) && (lp->tx_delay >= 0) 
		&& (lp->tx_delay <= 0xff)) {
		
		config_txdll(lp, lp->tx_delay);

	} else {
		printk(KERN_ERR "%s skip gmac txdelay config\n",ndev->name);
	}

	if ((lp->dpll_addr_map.rxdpll_base != NULL) && (lp->rx_delay >= 0) 
		&& (lp->rx_delay <= 0xff)) {
		
		config_rxdll(lp, lp->rx_delay);

	} else {
		printk(KERN_ERR "%s skip gmac rxdelay config\n",ndev->name);
	}
	
	if(lp->dpll_addr_map.negrxdpll_base != NULL){
			if( (lp->negrx_delay >= 0) && (lp->negrx_delay <= 0xff)) {
				config_rxdll_neg(lp, lp->negrx_delay);
			} else {
				printk(KERN_ERR "%s skip gmac negrx_delay config\n",ndev->name);
			}
	}


	return 0;
}


/*************bit ops*************/
/*****bit_check_and_modify <set_reg> <bit_idx(0-31)> <check_reg> <bit_check_idx(0-31)****/

static int bit_check_and_modify(unsigned int* modify_val, int bit_idx,
	unsigned int * check_reg, int bit_check_idx)
{

	if (*check_reg & (0x1 << bit_check_idx)) {
			//set bit
			*modify_val |= (0x1 << bit_idx);
	} else {
			//clr bit
			*modify_val &= ~(0x1 << bit_idx);
	}

	return 0;
}
static int config_eth_rxnegdelay(struct net_local *lp, int value)
{
	struct net_device *ndev = lp->ndev;
	unsigned int rdata0 = 0x0;
	unsigned int rdata1 = 0x0;
	unsigned int checkval = 0x0;
	
	value &= GENMASK(7, 0);
	rdata0 = readl(REG_GMAC_RXDLL_ECO1);
	rdata1 = readl(REG_GMAC_RXDLL_ECO0);
	
	if (lp->ctrl_id == 0) {
		checkval = readl(REG_GMAC_RXDLL_CTRL0);
		rdata0 &= ~(0xff << 4);
		rdata0 |= (value << 4);
		bit_check_and_modify(&rdata1,0,&checkval,4);
		bit_check_and_modify(&rdata0,0,&checkval,3);
		bit_check_and_modify(&rdata1,1,&checkval,2);
		bit_check_and_modify(&rdata0,1,&checkval,1);
		bit_check_and_modify(&rdata1,2,&checkval,0);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);
		printk("\n ctrl_id:0x%x rdata0:0x%x, rdata1:0x%x\n",lp->ctrl_id, rdata0, rdata1);
		//0x6a04
		checkval=0x6a04;
		bit_check_and_modify(&rdata0,2,&checkval,11);
		bit_check_and_modify(&rdata1,3,&checkval,12);
		bit_check_and_modify(&rdata0,3,&checkval,13);
		bit_check_and_modify(&rdata1,4,&checkval,14);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);
		//0x4a04
		checkval=0x4a04;
		bit_check_and_modify(&rdata0,2,&checkval,11);
		bit_check_and_modify(&rdata1,3,&checkval,12);
		bit_check_and_modify(&rdata0,3,&checkval,13);
		bit_check_and_modify(&rdata1,4,&checkval,14);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);
	} else if (lp->ctrl_id == 1) {
		checkval = readl(REG_GMAC_RXDLL_CTRL0);
		rdata0 &= ~(0xff << 16);
		rdata0 |= (value << 16);
		bit_check_and_modify(&rdata1,5,&checkval,4);
		bit_check_and_modify(&rdata0,12,&checkval,3);
		bit_check_and_modify(&rdata1,6,&checkval,2);
		bit_check_and_modify(&rdata0,13,&checkval,1);
		bit_check_and_modify(&rdata1,7,&checkval,0);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);
		printk("\n ctrl_id:0x%x rdata0:0x%x, rdata1:0x%x\n",lp->ctrl_id, rdata0, rdata1);
		//0x6a04
		checkval=0x6a04;
		bit_check_and_modify(&rdata0,14,&checkval,11);
		bit_check_and_modify(&rdata1,8,&checkval,12);
		bit_check_and_modify(&rdata0,15,&checkval,13);
		bit_check_and_modify(&rdata1,9,&checkval,14);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);
		//0x4a04
		checkval=0x4a04;
		bit_check_and_modify(&rdata0,14,&checkval,11);
		bit_check_and_modify(&rdata1,8,&checkval,12);
		bit_check_and_modify(&rdata0,15,&checkval,13);
		bit_check_and_modify(&rdata1,9,&checkval,14);
		writel(rdata0,REG_GMAC_RXDLL_ECO1);
		writel(rdata1,REG_GMAC_RXDLL_ECO0);

	} else {
		printk("error gmac ctrl_id,name:%s\n!!!\n",ndev->name);
	}

	return 0;
}

static int config_dpll(struct net_local *lp)
{
	struct net_device *ndev = lp->ndev;
	struct device *dev = &lp->pdev->dev;
	printk("\n ctrl_id:0x%x, tx_delay:0x%x,rx_delay:0x%x,negrx_delay:0x%x\n",
		lp->ctrl_id, lp->tx_delay, lp->rx_delay, lp->negrx_delay);
#if 0	
	if (lp->gmac_dpll_base == NULL) {
		dev_err(dev, "err config gmac tx/rx delay\n");
		return -1;
	}
#endif
	if ((lp->dpll_addr_map.txdpll_base != NULL) && (lp->tx_delay >= 0)
		&& (lp->tx_delay <= 0xff)) {

		writel(0x19,REG_GMAC_TXDLL_CTRL0);
		writel(lp->tx_delay,REG_GMAC_TXDLL_CTRL2);
		writel(0x6a04,REG_GMAC_TXDLL_CTRL1);
		writel(0x4a04,REG_GMAC_TXDLL_CTRL1);
	} else {
		printk("%s skip gmac txdelay config\n",ndev->name);
	}

	if ((lp->dpll_addr_map.rxdpll_base != NULL) && (lp->rx_delay >= 0)
		&& (lp->rx_delay <= 0xff)) {

		writel(0x19,REG_GMAC_RXDLL_CTRL0);

		if (lp->dpll_addr_map.dll_addr_is_fixup == true) {
			if(lp->ctrl_id == 0x0){
				void __iomem * tmp = devm_ioremap(dev,dll_fixup_addr,4);
				writel(lp->rx_delay, tmp);
			} else {
				writel(lp->rx_delay,REG_GMAC_RXDLL_CTRL2);
			}
		} else {
			writel(lp->rx_delay,REG_GMAC_RXDLL_CTRL2);
		}
		writel(0x6a04,REG_GMAC_RXDLL_CTRL1);
		writel(0x4a04,REG_GMAC_RXDLL_CTRL1);

	} else {
		printk("%s skip gmac rxdelay config\n",ndev->name);
	}

	if((lp->dpll_addr_map.eco0_base != NULL) && 
		(lp->dpll_addr_map.eco1_base != NULL)){
			if( (lp->negrx_delay >= 0) && (lp->negrx_delay <= 0xff)) {
				config_eth_rxnegdelay(lp, lp->negrx_delay);
			} else {
				printk("%s skip gmac negrx_delay config\n",ndev->name);
			}
	}

	return 0;
}

static int _pltfr_rmii_out_phase_inv(struct device *dev)
{
	struct platform_device * pdev = to_platform_device(dev);
	struct net_device * ndev = platform_get_drvdata(pdev);
	struct net_local *lp = netdev_priv(ndev);
	struct dwceqos_reg_cfg inv_sel;
	int num;
	int ret = 0;

	if (lp->bus_cfg.en_rmii_out_inv) {
		num = device_property_read_u32_array(dev, "snps,inv-sel", NULL, 0);
		if ((num <= 0) || (num > sizeof(struct dwceqos_reg_cfg)/sizeof(u32))) {
			dev_err(dev, "Fail to get inv-sel num\n");
			return -ENODEV;
		}

		ret = device_property_read_u32_array(dev, "snps,inv-sel", (u32*)&inv_sel, num);
		if (ret < 0) {
			dev_err(dev, "Fail to get inv-sel\n");
			return ret;
		}

		regmap_update_bits(lp->apahb_base, inv_sel.reg_offset,
						inv_sel.bit_mask << inv_sel.bit_offset, 0x1 << inv_sel.bit_offset);
	}

	return 0;
}

static int _pltfr_clk_enable(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	int ret = 0;

	ret = clk_mini_enable(CKG_AXI_EMAC_EN);
	if(ret) {
		dev_err(&ndev->dev, "Unable to enable CKG_AXI_GMAC0_EN clock.\n");
		goto err;
	}

	ret = clk_mini_enable(CKG_APB_GMAC0_CSR_EN);
	if(ret) {
		dev_err(&ndev->dev, "Unable to enable CKG_APB_GMAC0_CSR_EN clock.\n");
		goto err;
	}

	ret = clk_mini_enable(CKG_GMAC0_TX_EN);
	if(ret) {
		dev_err(&ndev->dev, "Unable to enable CKG_GMAC0_TX_EN clock.\n");
		goto err;
	}

	ret = clk_mini_enable(CKG_APB_EPHY_PHYCFG_EN);
	if(ret) {
		dev_err(&ndev->dev, "Unable to enable APB_EPHY_PHYCFG_EN clock.\n");
		goto err;
	}

	if (!device_may_wakeup(dev)) {
		ret = clk_mini_enable(CKG_GMAC0_RX_EN);
		if(ret) {
			dev_err(&ndev->dev, "Unable to enable CKG_GMAC0_RX_EN clock.\n");
			goto err;
		}

		ret = clk_mini_enable(CKG_GMAC0_RMII_EN);
		if(ret) {
			dev_err(&ndev->dev, "Unable to enable CKG_GMAC0_RMII_EN clock.\n");
			goto err;
		}

		ret = clk_mini_enable(CKG_EPHY0_REF_EN);
		if(ret) {
			dev_err(&ndev->dev, "Unable to enable CKG_EPHY0_REF_EN clock.\n");
			goto err;
		}
	}

err:
	return ret;

}

static int _pltfr_clk_disable(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	int ret = 0;

	ret = clk_mini_disable(CKG_AXI_EMAC_EN);
	if (ret) {
		dev_err(&ndev->dev, "clk axi emac error\n");
		goto err;
	}
	ret = clk_mini_disable(CKG_APB_GMAC0_CSR_EN);
	if (ret) {
		dev_err(&ndev->dev, "clk gmac csr error\n");
		goto err;
	}
	ret = clk_mini_disable(CKG_GMAC0_TX_EN);
	if (ret) {
		dev_err(&ndev->dev, "clk gmac tx error\n");
		goto err;
	}

	ret = clk_mini_disable(CKG_APB_EPHY_PHYCFG_EN);
	if (ret) {
		dev_err(&ndev->dev, "clk ephycfg error\n");
		goto err;
	}

	if (!device_may_wakeup(dev)) {
		ret = clk_mini_disable(CKG_GMAC0_RX_EN);
		if (ret) {
			dev_err(&ndev->dev, "clk gmac rx error\n");
			goto err;
		}

		ret = clk_mini_disable(CKG_GMAC0_RMII_EN);
		if (ret) {
			dev_err(&ndev->dev, "clk gmac rmii error\n");
			goto err;
		}

		ret = clk_mini_disable(CKG_EPHY0_REF_EN);
		if (ret) {
			dev_err(&ndev->dev, "clk ref error\n");
			goto err;
		}
	}

err:

	return ret;
}

static int dwceqos_clk_init(struct net_local *lp)
{
    int ret = 0;
    struct device   *dev = &lp->pdev->dev;
    const char *phy_mode_str = NULL;
	struct dwceqos_reg_cfg phy_type_sel;
	struct dwceqos_reg_cfg rmii_pad_sel;
	struct dwceqos_reg_cfg phy_intf_sel;
	struct dwceqos_reg_cfg ref_en_cfg;
	struct dwceqos_reg_cfg soft_reset_cfg;
	struct dwceqos_reg_cfg axi_emac_en_cfg;
	struct dwceqos_reg_cfg ana_reset_cfg;
	int num;

    ret = of_property_read_string(dev->of_node, "phy-mode", &phy_mode_str);
    if (ret < 0) {
		dev_err(dev, "invalid PHY mode property\n");
		return ret;
    }

	num = device_property_read_u32_array(dev, "snps,ephy-type-sel", NULL, 0);
	if ((num <= 0) || (num > sizeof(struct dwceqos_reg_cfg)/sizeof(u32))) {
		dev_err(dev, "Fail to get ephy-type-sel num\n");
		return -ENODEV;
	}
	
	ret = device_property_read_u32_array(dev, "snps,ephy-type-sel", (u32*)&phy_type_sel, num);
	if (ret < 0) {
		dev_err(dev, "Fail to get ephy-type-sel\n");
		return ret;
	}

	num = device_property_read_u32_array(dev, "snps,rmii-pad-sel", NULL, 0);
	if ((num <= 0) || (num > sizeof(struct dwceqos_reg_cfg)/sizeof(u32))) {
		dev_err(dev, "Fail to get rmii-pad-sel num\n");
		return -ENODEV;
	}
	
	ret = device_property_read_u32_array(dev, "snps,rmii-pad-sel", (u32*)&rmii_pad_sel, num);
	if (ret < 0) {
		dev_err(dev, "Fail to get rmii-pad-sel\n");
		return ret;
	}

	num = device_property_read_u32_array(dev, "snps,phy-intf-sel", NULL, 0);
	if ((num <= 0) || (num > sizeof(struct dwceqos_reg_cfg)/sizeof(u32))) {
		dev_err(dev, "Fail to get phy-intf-sel num\n");
		return -ENODEV;
	}
	
	ret = device_property_read_u32_array(dev, "snps,phy-intf-sel", (u32*)&phy_intf_sel, num);
	if (ret < 0) {
		dev_err(dev, "Fail to get phy-intf-sel\n");
		return ret;
	}

	num = device_property_read_u32_array(dev, "snps,ephy-ref-en", NULL, 0);
	if ((num <= 0) || (num > sizeof(struct dwceqos_reg_cfg)/sizeof(u32))) {
		dev_err(dev, "Fail to get phy-intf-sel num\n");
		return -ENODEV;
	}
	
	ret = device_property_read_u32_array(dev, "snps,ephy-ref-en", (u32*)&ref_en_cfg, num);
	if (ret < 0) {
		dev_err(dev, "Fail to get phy-intf-sel\n");
		return ret;
	}

	num = device_property_read_u32_array(dev, "snps,ephy-soft-reset", NULL, 0);
	if ((num <= 0) || (num > sizeof(struct dwceqos_reg_cfg)/sizeof(u32))) {
		dev_err(dev, "Fail to get ephy-soft-reset num\n");
		return -ENODEV;
	}

	ret = device_property_read_u32_array(dev, "snps,ephy-soft-reset", (u32*)&soft_reset_cfg, num);
	if (ret < 0) {
		dev_err(dev, "Fail to get ephy-soft-reset\n");
		return ret;
	}

	num = device_property_read_u32_array(dev, "snps,ana-soft-reset", NULL, 0);
	if ((num <= 0) || (num > sizeof(struct dwceqos_reg_cfg)/sizeof(u32))) {
		dev_err(dev, "Fail to get ana-soft-reset num\n");
		return -ENODEV;
	}

	ret = device_property_read_u32_array(dev, "snps,ana-soft-reset", (u32*)&soft_reset_cfg, num);
	if (ret < 0) {
		dev_err(dev, "Fail to get ana-soft-reset\n");
		return ret;
	}


	num = device_property_read_u32_array(dev, "snps,axi-emac-en", NULL, 0);
	if ((num <= 0) || (num > sizeof(struct dwceqos_reg_cfg)/sizeof(u32))) {
		dev_err(dev, "Fail to get axi-emac-en num\n");
		return -ENODEV;
	}

	ret = device_property_read_u32_array(dev, "snps,axi-emac-en", (u32*)&axi_emac_en_cfg, num);
	if (ret < 0) {
		dev_err(dev, "Fail to get axi-emac-en\n");
		return ret;
	}

	if (!strcasecmp(phy_mode_str, "rgmii")) {

		regmap_update_bits(lp->apahb_base, rmii_pad_sel.reg_offset,
			rmii_pad_sel.bit_mask << rmii_pad_sel.bit_offset, ~(rmii_pad_sel.bit_mask << rmii_pad_sel.bit_offset));
		regmap_update_bits(lp->apahb_base, phy_intf_sel.reg_offset,
			phy_intf_sel.bit_mask << phy_intf_sel.bit_offset, RGMII_INTF << phy_intf_sel.bit_offset);

		regmap_update_bits(lp->glapb_base, ref_en_cfg.reg_offset,
			ref_en_cfg.bit_mask << ref_en_cfg.bit_offset, BIT_CEN_APB_ANA_EPHY_REF_EN << ref_en_cfg.bit_offset);
			
	} else if (!strcasecmp(phy_mode_str, "rmii")) {
		regmap_update_bits(lp->apahb_base, rmii_pad_sel.reg_offset,
			rmii_pad_sel.bit_mask << rmii_pad_sel.bit_offset, BIT_CKG_GMAC_RMII_PAD_SEL << rmii_pad_sel.bit_offset);
		regmap_update_bits(lp->apahb_base, phy_intf_sel.reg_offset,
			phy_intf_sel.bit_mask << phy_intf_sel.bit_offset, RMII_INTF << phy_intf_sel.bit_offset);
		// clk cfg
		regmap_update_bits(lp->apahb_base, ref_en_cfg.reg_offset,
			ref_en_cfg.bit_mask << ref_en_cfg.bit_offset, BIT_AHB_EPHY_CFG_REF_EN << ref_en_cfg.bit_offset);
		regmap_update_bits(lp->apahb_base, axi_emac_en_cfg.reg_offset,
			axi_emac_en_cfg.bit_mask << axi_emac_en_cfg.bit_offset, BIT_AXI_EMAC_EN << axi_emac_en_cfg.bit_offset);


	} else if (!strcasecmp(phy_mode_str, "mii")) {
		regmap_update_bits(lp->apahb_base, rmii_pad_sel.reg_offset,
			rmii_pad_sel.bit_mask << rmii_pad_sel.bit_offset, BIT_CKG_GMAC_RMII_PAD_SEL << rmii_pad_sel.bit_offset);
		regmap_update_bits(lp->apahb_base, phy_intf_sel.reg_offset,
			phy_intf_sel.bit_mask << phy_intf_sel.bit_offset, MII_INTF << phy_intf_sel.bit_offset);
	
		regmap_update_bits(lp->apahb_base, ref_en_cfg.reg_offset,
			ref_en_cfg.bit_mask << ref_en_cfg.bit_offset, BIT_AHB_EPHY_CFG_REF_EN << ref_en_cfg.bit_offset);
		regmap_update_bits(lp->apahb_base, axi_emac_en_cfg.reg_offset,
			axi_emac_en_cfg.bit_mask << axi_emac_en_cfg.bit_offset, BIT_AXI_EMAC_EN << axi_emac_en_cfg.bit_offset);

	} else {
		dev_err(dev, "unknow PHY mode string\n");
		return -ENXIO;
	}

	if(lp->phy_type == EXTERNAL_PHY) {
		/*
		if (lp->ctrl_id == 0x10) {
		regmap_update_bits(lp->apahb_base, phy_type_sel.reg_offset,
			phy_type_sel.bit_mask << phy_type_sel.bit_offset, 0x1 << phy_type_sel.bit_offset);
		} else {
		regmap_update_bits(lp->apahb_base, phy_type_sel.reg_offset,
			phy_type_sel.bit_mask << phy_type_sel.bit_offset, 0x2 << phy_type_sel.bit_offset);
		}
		regmap_update_bits(lp->glapb_base, ref_en_cfg.reg_offset,
					ref_en_cfg.bit_mask << ref_en_cfg.bit_offset, BIT_CEN_APB_ANA_EPHY_REF_EN << ref_en_cfg.bit_offset);
		*/
		regmap_update_bits(lp->apahb_base, phy_type_sel.reg_offset,
			phy_type_sel.bit_mask << phy_type_sel.bit_offset, 0x1 << phy_type_sel.bit_offset);
		if(lp->platform  == CHIP_PLATFORM) {
			/* EPHY Soft reset */
			regmap_update_bits(lp->apahb_base, soft_reset_cfg.reg_offset,
				soft_reset_cfg.bit_mask << soft_reset_cfg.bit_offset, BIT_CEN_APB_ANA_EPHY_SRST << soft_reset_cfg.bit_offset);
			msleep(50);
			regmap_update_bits(lp->apahb_base, soft_reset_cfg.reg_offset,
				soft_reset_cfg.bit_mask << soft_reset_cfg.bit_offset, ~(BIT_CEN_APB_ANA_EPHY_SRST << soft_reset_cfg.bit_offset));
			msleep(100);
		#if 0
			//ephy1-rstn ==> gpio3_5
			gpio_rstn = 8*3+5;
			gpio_request(gpio_rstn,"ephy0_rstn");
			
			gpio_direction_output(gpio_rstn, 1);

			gpio_set_value(gpio_rstn,0);
			msleep(50);
			gpio_set_value(gpio_rstn,1);
			msleep(100);
			gpio_free(gpio_rstn);
		#endif
		printk(KERN_DEBUG "soft reset ephy done\n");
		}
		else {
		/* Haps EPHY Soft reset */
            regmap_update_bits(lp->apahb_base, soft_reset_cfg.reg_offset,
                    soft_reset_cfg.bit_mask << soft_reset_cfg.bit_offset, BIT_CEN_APB_ANA_EPHY_SRST << soft_reset_cfg.bit_offset);
            msleep(50);
            regmap_update_bits(lp->apahb_base, soft_reset_cfg.reg_offset,
                    soft_reset_cfg.bit_mask << soft_reset_cfg.bit_offset, ~(BIT_CEN_APB_ANA_EPHY_SRST << soft_reset_cfg.bit_offset));
            msleep(100);
			printk(KERN_DEBUG "haps soft reset ephy done\n");
		}

	} else {
	/*
		regmap_update_bits(lp->apahb_base, phy_type_sel.reg_offset,
				phy_type_sel.bit_mask << phy_type_sel.bit_offset, INTERNAL_PHY << phy_type_sel.bit_offset);
		regmap_update_bits(lp->glapb_base, ref_en_cfg.reg_offset,
				ref_en_cfg.bit_mask << ref_en_cfg.bit_offset, ~(BIT_CEN_APB_ANA_EPHY_REF_EN << ref_en_cfg.bit_offset));
	*/

		/*internal fh phy*/
		regmap_update_bits(lp->apahb_base, phy_type_sel.reg_offset,
			phy_type_sel.bit_mask << phy_type_sel.bit_offset, 0x0 << phy_type_sel.bit_offset);

		/* EPHY Soft reset */
		regmap_update_bits(lp->apahb_base, ana_reset_cfg.reg_offset,
			ana_reset_cfg.bit_mask << ana_reset_cfg.bit_offset, BIT_CEN_APB_ANA_EPHY_SRST << ana_reset_cfg.bit_offset);
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		msleep(50);
		#else
		udelay(100);
		#endif
		regmap_update_bits(lp->apahb_base, ana_reset_cfg.reg_offset,
			ana_reset_cfg.bit_mask << ana_reset_cfg.bit_offset, ~(BIT_CEN_APB_ANA_EPHY_SRST << ana_reset_cfg.bit_offset));
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		msleep(100);
		dev_info(dev, "internal phy soft reset done!\n");
		#else
		udelay(100);
		#endif

	}

    return ret;
}
static const struct of_device_id dwceq_of_match[] = {
	{ .compatible = "snps,dwc-qos-ethernet-4.10", },
	{ .compatible = "molchip,dwc-qos-ethernet-4.10", },
	{}
};
MODULE_DEVICE_TABLE(of, dwceq_of_match);

static int dwceqos_hw_io_init(struct net_local *lp)
{
	int ret = -ENXIO;
	struct resource *r_mem = NULL;
	struct device	*dev = &lp->pdev->dev;

	r_mem = platform_get_resource(lp->pdev, IORESOURCE_MEM, 0);
	if (!r_mem) {
		dev_err(dev, "no IO 0 resource defined.\n");
		return ret;
	}
	lp->ndev->base_addr = r_mem->start;
	lp->baseaddr = devm_ioremap_resource(dev, r_mem);
	if (IS_ERR(lp->baseaddr)) {
		dev_err(dev, "failed to map baseaddress 0.\n");
		ret = PTR_ERR(lp->baseaddr);
		return ret;
	}

	if (lp->gmac_cap.has_txrx_delay == 1) {
		r_mem = platform_get_resource(lp->pdev, IORESOURCE_MEM, 1);
		if (!r_mem) {
			dev_err(dev, "no IO 1 resource defined.\n");
			lp->dpll_addr_map.txdpll_base = NULL;
			//return ret;
		} else {
			lp->dpll_addr_map.txdpll_base = devm_ioremap_resource(dev, r_mem);
			if (IS_ERR(lp->dpll_addr_map.txdpll_base)) {
				dev_err(dev, "failed to map txdpll_base.1\n");
				ret = PTR_ERR(lp->dpll_addr_map.txdpll_base);
				lp->dpll_addr_map.txdpll_base = NULL;
				//return ret;
			}
			
		}
		r_mem = platform_get_resource(lp->pdev, IORESOURCE_MEM, 2);
		if (!r_mem) {
			dev_err(dev, "no IO 2 resource defined.\n");
			lp->dpll_addr_map.rxdpll_base = NULL;
			//return ret;
		} else {
			lp->dpll_addr_map.rxdpll_base = devm_ioremap_resource(dev, r_mem);
			if (IS_ERR(lp->dpll_addr_map.rxdpll_base)) {
				dev_err(dev, "failed to map rxdpll_base.2\n");
				ret = PTR_ERR(lp->dpll_addr_map.rxdpll_base);
				lp->dpll_addr_map.rxdpll_base = NULL;
				//return ret;
			}
			
		}

		if(lp->gmac_cap.dpll_version == 1) {
			r_mem = platform_get_resource(lp->pdev, IORESOURCE_MEM, 3);
			if (!r_mem) {
				dev_err(dev, "no IO 3 resource defined.\n");
				lp->dpll_addr_map.eco0_base = NULL;
				//return ret;
			} else {
				lp->dpll_addr_map.eco0_base = devm_ioremap_resource(dev, r_mem);
				if (IS_ERR(lp->dpll_addr_map.eco0_base)) {
					dev_dbg(dev, "failed to map eco0_base.3\n");
					ret = PTR_ERR(lp->dpll_addr_map.eco0_base);
					//lp->dpll_addr_map.eco0_base = NULL;
					lp->dpll_addr_map.eco0_base = default_eco0_base;
					//return ret;
				} else {
					default_eco0_base = lp->dpll_addr_map.eco0_base;
				}
			}
			r_mem = platform_get_resource(lp->pdev, IORESOURCE_MEM, 4);
			if (!r_mem) {
				dev_err(dev, "no IO 4 resource defined.\n");
				lp->dpll_addr_map.eco1_base = NULL;
				//return ret;
			} else {
				lp->dpll_addr_map.eco1_base = devm_ioremap_resource(dev, r_mem);
				if (IS_ERR(lp->dpll_addr_map.eco1_base)) {
					dev_dbg(dev, "failed to map eco1_base.4\n");
					ret = PTR_ERR(lp->dpll_addr_map.eco1_base);
					//lp->dpll_addr_map.eco1_base = NULL;
					lp->dpll_addr_map.eco1_base = default_eco1_base;
					//return ret;
				} else {
					default_eco1_base = lp->dpll_addr_map.eco1_base;
				}
			}
			
			lp->dpll_addr_map.dll_addr_is_fixup = of_property_read_bool(dev->of_node, "dll_addr_is_fixup");
		}else if (lp->gmac_cap.dpll_version == 2) {
			r_mem = platform_get_resource(lp->pdev, IORESOURCE_MEM, 3);
			if (!r_mem) {
				dev_err(dev, "no IO 3 resource defined.\n");
				lp->dpll_addr_map.negrxdpll_base = NULL;
				//return ret;
			} else {
				lp->dpll_addr_map.negrxdpll_base = devm_ioremap_resource(dev, r_mem);
				if (IS_ERR(lp->dpll_addr_map.negrxdpll_base)) {
					dev_err(dev, "failed to map negrxdpll_base.3 replaced by rxdpll_base\n");
					ret = PTR_ERR(lp->dpll_addr_map.negrxdpll_base);
					lp->dpll_addr_map.negrxdpll_base = lp->dpll_addr_map.rxdpll_base;
					//return ret;
				}
				
			}


		}

	}
	
	lp->apahb_base = syscon_regmap_lookup_by_phandle(dev->of_node,
						"molchip,syscon-ap-ahb");
	if (IS_ERR(lp->apahb_base)) {
		dev_err(dev, "failed to map apahb_base\n");
		return PTR_ERR(lp->apahb_base);
	}
	/*
	lp->apapb_base = syscon_regmap_lookup_by_phandle(dev->of_node,
						"molchip,syscon-ap-apb");
	if (IS_ERR(lp->apapb_base)) {
		dev_err(dev, "failed to map apapb_base\n");
		return PTR_ERR(lp->apapb_base);
	}

	lp->glapb_base = syscon_regmap_lookup_by_phandle(dev->of_node,
						"molchip,syscon-gl-apb");
	if (IS_ERR(lp->glapb_base)) {
		dev_err(dev, "failed to map glapb_base\n");
		return PTR_ERR(lp->glapb_base);
	}
	*/

	of_property_read_u32(dev->of_node,
					 "phy-type", &lp->phy_type);
	of_property_read_u32(dev->of_node,
					 "platform", &lp->platform);

	if(lp->platform == CHIP_PLATFORM){
	    lp->ana = syscon_regmap_lookup_by_phandle(dev->of_node, "molchip,syscon-ana");
		if (IS_ERR(lp->ana) && (lp->phy_type == INTERNAL_PHY)) {
			dev_err(dev, "failed to map ana\n");
			return PTR_ERR(lp->ana);
		}
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		dwceqos_chip_apclk_init(lp);
		#endif
	}
	else
		dwceqos_haps_apclk_init(lp);

	dwceqos_haps_apclk_init(lp);
	dwceqos_clk_init(lp);

	return 0;
}
static ssize_t rx_irq_watchdog_show(struct device *dev,
                                   struct device_attribute *attr, char *buf)
{
        ssize_t rc = 0;
        struct platform_device * pdev = to_platform_device(dev);
        struct net_device * net_dev = platform_get_drvdata(pdev);
        struct net_local * lp = netdev_priv(net_dev);

        snprintf(buf,32,"%d\n",lp->rx_irq_watchdog);
        rc = strlen(buf);
        return rc;
}

static void reopen_net_device(struct net_device *ndev)
{
	rtnl_lock();
	dwceqos_stop(ndev);
	dwceqos_open(ndev);
	rtnl_unlock();

	return;
}

static ssize_t rx_irq_watchdog_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct platform_device * pdev = to_platform_device(dev);
	struct net_device * net_dev = platform_get_drvdata(pdev);
	struct net_local * lp = netdev_priv(net_dev);

	lp->rx_irq_watchdog = (u8)simple_strtoull(buf,'\0',16);

	if ( (lp->rx_irq_watchdog && lp->rx_irq_watchdog_old)  == 0 ) {

		reopen_net_device(lp->ndev);
		printk(KERN_INFO "switch rx_irq_watchdog[0---close,>0---open]:%d\n",lp->rx_irq_watchdog);

	} else {
		dwceqos_write(lp, REG_DWCEQOS_DMA_RX_WATHCDOG, lp->rx_irq_watchdog);
	}


	lp->rx_irq_watchdog_old = lp->rx_irq_watchdog;
	printk(KERN_INFO "rx_irq_watchdog[0x0~0xff]:%d\n",lp->rx_irq_watchdog);

	return count;
}


static DEVICE_ATTR(rx_irq_watchdog, S_IRUGO|S_IWUSR, rx_irq_watchdog_show, rx_irq_watchdog_store);

static struct attribute *device_attrs[] = {
	&dev_attr_rx_irq_watchdog.attr,
	NULL
};

static const struct attribute_group device_attr_group = {
	.attrs = device_attrs,
};

static int dwceqos_probe(struct platform_device *pdev)
{
	struct net_device *ndev;
	struct net_local *lp;
	int ret = -ENXIO;
	struct device   *dev = &pdev->dev;
	char int_name[20];

	ndev = alloc_etherdev(sizeof(*lp));
	if (!ndev) {
		dev_err(&pdev->dev, "etherdev allocation failed.\n");
		return ret;
	}

	SET_NETDEV_DEV(ndev, &pdev->dev);

	lp = netdev_priv(ndev);
	lp->ndev = ndev;
	lp->pdev = pdev;
	lp->msg_enable = netif_msg_init(debug, DWCEQOS_MSG_DEFAULT);

	spin_lock_init(&lp->tx_lock);
	spin_lock_init(&lp->hw_lock);
	spin_lock_init(&lp->stats_lock);

	ret = of_property_read_u32(dev->of_node, "ctrl_id", &lp->ctrl_id);
	if (ret < 0) {
			lp->ctrl_id = -1;
			dev_err(dev, "invalid ctrl_id\n");
	}

	lp->gmac_cap.has_txrx_delay = of_property_read_bool(dev->of_node, "has_txrx_delay");
	if (ret < 0) {
		lp->gmac_cap.has_txrx_delay = 0;
	}
	
	if (lp->gmac_cap.has_txrx_delay == 1) {

		ret = of_property_read_u32(dev->of_node, "dpll_version", &(lp->gmac_cap.dpll_version));
		if (ret < 0) {
				lp->gmac_cap.dpll_version = 1;
		}
		printk(KERN_ERR "miku has_txrx_delay:%d, dpll_version:%d\n",lp->gmac_cap.has_txrx_delay, lp->gmac_cap.dpll_version);
		
        	ret = of_property_read_u32(dev->of_node, "tx_delay", &lp->tx_delay);
        	if (ret < 0) {
                	lp->tx_delay = -2;
                	dev_err(dev, "invalid tx_delay\n");
        	}

        	ret = of_property_read_u32(dev->of_node, "rx_delay", &lp->rx_delay);
		if (ret < 0) {
			lp->rx_delay = -2;
			dev_err(dev, "invalid rx_delay\n");
		}

		ret = of_property_read_u32(dev->of_node, "negrx_delay", &lp->negrx_delay);
		if (ret < 0) {
			lp->negrx_delay = -2;
			dev_err(dev, "invalid negrx_delay\n");
		}
	}

#if (GMAC_QUICK_CLK == 0)
	lp->apahb_gate = devm_clk_get(&pdev->dev, "gmac_clk_eb");
	if (IS_ERR(lp->apahb_gate)) {
		dev_err(&pdev->dev, "gmac_clk_eb clock not found.\n");
		ret = PTR_ERR(lp->apahb_gate);
		goto err_out_free_netdev;
	}

	ret = clk_prepare_enable(lp->apahb_gate);
	if (ret) {
		dev_err(&pdev->dev, "Unable to enable gmac_clk_eb clock.\n");
		goto err_out_free_netdev;
	}

	lp->apapb_gate = devm_clk_get(&pdev->dev, "gmac_clk_csr");
	if (IS_ERR(lp->apapb_gate)) {
		dev_err(&pdev->dev, "gmac_clk_csr clock not found.\n");
		ret = PTR_ERR(lp->apapb_gate);
		goto err_out_clk_dis_apahb_gate;
	}

	ret = clk_prepare_enable(lp->apapb_gate);
	if (ret) {
		dev_err(&pdev->dev, "Unable to enable gmac_clk_csr clock.\n");
		goto err_out_clk_dis_apahb_gate;
	}


	lp->tx_en = devm_clk_get(&pdev->dev, "tx_en");
	if (IS_ERR(lp->tx_en)) {
		dev_err(&pdev->dev, "tx_en clock not found.\n");
		//ret = PTR_ERR(lp->tx_en);
		//goto err_out_clk_dis_apahb_gate;
	} else {
		ret = clk_prepare_enable(lp->tx_en);
		if (ret) {
			dev_err(&pdev->dev, "Unable to enable tx_en clock.\n");
			//goto err_out_clk_dis_apahb_gate;
		}
	}

	lp->rx_en = devm_clk_get(&pdev->dev, "rx_en");
	if (IS_ERR(lp->rx_en)) {
		dev_err(&pdev->dev, "rx_en clock not found.\n");
		//ret = PTR_ERR(lp->rx_en);
		//goto err_out_clk_dis_apahb_gate;
	} else {
		ret = clk_prepare_enable(lp->rx_en);
		if (ret) {
			dev_err(&pdev->dev, "Unable to enable rx_en clock.\n");
			//goto err_out_clk_dis_apahb_gate;
		}
	}

	lp->rmii_en = devm_clk_get(&pdev->dev, "rmii_en");
	if (IS_ERR(lp->rmii_en)) {
		dev_err(&pdev->dev, "rx_en clock not found.\n");
		//ret = PTR_ERR(lp->rmii_en);
		//goto err_out_clk_dis_apahb_gate;
	} else {
		ret = clk_prepare_enable(lp->rmii_en);
		if (ret) {
			dev_err(&pdev->dev, "Unable to enable rx_en clock.\n");
			//goto err_out_clk_dis_apahb_gate;
		}
	}

	lp->ephy_ref_en = devm_clk_get(&pdev->dev, "ephy_ref_en");
	if (IS_ERR(lp->ephy_ref_en)) {
		dev_err(&pdev->dev, "ephy_ref_en clock not found.\n");
		//ret = PTR_ERR(lp->ephy_ref_en);
		//goto err_out_clk_dis_apahb_gate;
	} else {
		ret = clk_prepare_enable(lp->ephy_ref_en);
		if (ret) {
			dev_err(&pdev->dev, "Unable to enable ephy_ref_en clock.\n");
			goto err_out_clk_dis_apahb_gate;
		}
	}
#else
	dev_info(&pdev->dev, "gmac quick clk\n");

	ret = _pltfr_clk_enable(dev);
	if (ret) {
		dev_err(&pdev->dev, "mini clk init fail!\n");
		goto err_out_free_netdev;
	}
#endif
	ret = dwceqos_hw_io_init(lp);
	if (ret)
		goto err_out_clk_dis_apapb_gate;

	ndev->irq = platform_get_irq(pdev, 0);
	ndev->watchdog_timeo = DWCEQOS_TX_TIMEOUT * HZ;
	ndev->netdev_ops = &netdev_ops;
	ndev->ethtool_ops = &dwceqos_ethtool_ops;

	lp->lpi_irq = platform_get_irq(pdev, 1);
	if(lp->lpi_irq < 0)
		printk(KERN_ERR "cant get lpi_irq\n");
	lp->wol_irq = platform_get_irq(pdev, 2);
	if(lp->wol_irq < 0)
		printk(KERN_ERR "cant get wol_irq\n");
	#ifndef CONFIG_MOLCHIP_FASTBOOT
	printk(KERN_ERR "miku lpiirq:%d wolirq:%d commonirq:%d \n",lp->lpi_irq,lp->wol_irq,ndev->irq);
	#endif

	dwceqos_get_hwfeatures(lp);
	dwceqos_mdio_set_csr(lp);

	ndev->hw_features = NETIF_F_SG;
	ndev->hw_features |= NETIF_F_HIGHDMA;

#ifdef TSO_EN
	if (lp->feature1 & DWCEQOS_MAC_HW_FEATURE1_TSOEN)
		ndev->hw_features |= NETIF_F_TSO | NETIF_F_TSO6;
#endif
	if (lp->feature0 & DWCEQOS_MAC_HW_FEATURE0_TXCOESEL)
		ndev->hw_features |= NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM;

	if (lp->feature0 & DWCEQOS_MAC_HW_FEATURE0_RXCOESEL)
		ndev->hw_features |= NETIF_F_RXCSUM;

	ndev->features = ndev->hw_features;

	lp->phy_node = of_parse_phandle(lp->pdev->dev.of_node,
						"phy-handle", 0);
	if (!lp->phy_node && of_phy_is_fixed_link(lp->pdev->dev.of_node)) {
		ret = of_phy_register_fixed_link(lp->pdev->dev.of_node);
		if (ret < 0) {
			dev_err(&pdev->dev, "invalid fixed-link");
			goto err_out_clk_dis_apapb_gate;
		}

		lp->phy_node = of_node_get(lp->pdev->dev.of_node);
	}

	ret = of_get_phy_mode(lp->pdev->dev.of_node, &lp->phy_interface);
	if (ret < 0) {
		dev_err(&lp->pdev->dev, "error in getting phy i/f\n");
		goto err_out_deregister_fixed_link;
	}

	//lp->phy_interface = ret;

	ret = dwceqos_mii_init(lp);
	if (ret) {
		dev_err(&lp->pdev->dev, "error in dwceqos_mii_init\n");
		goto err_out_deregister_fixed_link;
	}

	ret = dwceqos_mii_probe(ndev);
	if (ret != 0) {
		netdev_err(ndev, "mii_probe fail.\n");
		ret = -ENXIO;
		goto err_out_deregister_fixed_link;
	}

	dwceqos_set_umac_addr(lp, lp->ndev->dev_addr, 0);

	tasklet_init(&lp->tx_bdreclaim_tasklet, dwceqos_tx_reclaim,
		     (unsigned long)ndev);
	tasklet_disable(&lp->tx_bdreclaim_tasklet);

	lp->txtimeout_handler_wq = alloc_workqueue(DRIVER_NAME,
						   WQ_MEM_RECLAIM, 0);
	INIT_WORK(&lp->txtimeout_reinit, dwceqos_reinit_for_txtimeout);

	platform_set_drvdata(pdev, ndev);
	ret = dwceqos_probe_config_dt(pdev);
	if (ret) {
		dev_err(&lp->pdev->dev, "Unable to retrieve DT, error %d\n",
			ret);
		goto err_out_deregister_fixed_link;
	}

	dev_info(&lp->pdev->dev, "pdev->id %d, baseaddr 0x%08lx, irq %d\n",
		 pdev->id, ndev->base_addr, ndev->irq);

	ret = devm_request_irq(&pdev->dev, ndev->irq, &dwceqos_interrupt, 0,
			       ndev->name, ndev);
	if (ret) {
		dev_err(&lp->pdev->dev, "Unable to request IRQ %d, error %d\n",
			ndev->irq, ret);
		goto err_out_deregister_fixed_link;
	}

	ret = _pltfr_rmii_out_phase_inv(dev);
	if (ret) {
		dev_err(&lp->pdev->dev, "set clk inv fail %d\n",
			ret);
		goto err_out_deregister_fixed_link;
	}

	if (netif_msg_probe(lp))
		netdev_dbg(ndev, "net_local@%p\n", lp);

	netif_napi_add(ndev, &lp->napi, dwceqos_rx_poll, NAPI_POLL_WEIGHT);

	ret = register_netdev(ndev);
	if (ret) {
		dev_err(&pdev->dev, "Cannot register net device, aborting.\n");
		goto err_out_deregister_fixed_link;
	}
	ret =  sysfs_create_group(&pdev->dev.kobj, &device_attr_group);
	if (ret) {

		dev_err(&pdev->dev, "sysfs_create_group aborting.\n");
	}

	//wakeup irq
	if(lp->wol_irq > 0) {
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		printk(KERN_ERR "mikukkkk:%s\n",ndev->name);
		#endif
		sprintf(int_name, "%s:%s", "eth0", "wol");
		int_name[19] = '\0';
		//ret = devm_request_irq(&pdev->dev, lp->wol_irq, &dwceqos_interrupt, IRQF_SHARED,
		//			   int_name, ndev);
		ret = devm_request_irq(&pdev->dev, lp->wol_irq, &wol_interrupt, IRQF_EARLY_RESUME,
					   ndev->name, ndev);
		if (ret) {
			dev_err(&lp->pdev->dev, "Unable to request IRQ %d, error %d\n",
				lp->wol_irq, ret);
			//goto err_out_deregister_fixed_link;
		}

			lp->pmt_remote_wake_up = (lp->feature0 & GMAC_HW_FEAT_RWKSEL) >> 6;
			lp->pmt_magic_frame = (lp->feature0 & GMAC_HW_FEAT_MGKSEL) >> 7;


			//enable wakeup
			lp->wolopts= WAKE_MAGIC;// | WAKE_UCAST;
			lp->pmt = 1;
			lp->ndev->wol_enabled = 1;
			//mac4_pmt(lp,lp->wolopts);
		//lp->irq_wake = 1;
	}
	

	if ( (lp->tx_delay < -1) || (lp->rx_delay < -1) ) {
		ret = scan_dpll_table(lp);
		if (ret < 0) {
			dev_err(dev, "cannot find scan dpll table entry:%d\n",ret);
		}
	}
	if (lp->gmac_cap.dpll_version == 2) {
		config_dpll_v2(lp);
	} else if (lp->gmac_cap.dpll_version == 1) {
		config_dpll(lp);
	}

	device_init_wakeup(dev, true);
	dev_pm_set_wake_irq(dev, lp->wol_irq);
	return 0;

err_out_deregister_fixed_link:
	if (of_phy_is_fixed_link(pdev->dev.of_node))
		of_phy_deregister_fixed_link(pdev->dev.of_node);
#if (GMAC_QUICK_CLK == 0)
err_out_clk_dis_apapb_gate:
	clk_disable_unprepare(lp->apapb_gate);
//err_out_clk_dis_apahb_gate:
	clk_disable_unprepare(lp->apahb_gate);
#else
err_out_clk_dis_apapb_gate:
//err_out_clk_dis_apahb_gate:
	ret = _pltfr_clk_disable(dev);
	if (ret)
		dev_err(&pdev->dev, "dwceqos clk error\n");
#endif
err_out_free_netdev:
	of_node_put(lp->phy_node);
	free_netdev(ndev);
	platform_set_drvdata(pdev, NULL);
	return ret;
}

static int dwceqos_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct net_local *lp;
	sysfs_remove_group(&pdev->dev.kobj, &device_attr_group);
	if (ndev) {
		lp = netdev_priv(ndev);

		if (ndev->phydev) {
			phy_disconnect(ndev->phydev);
			if (of_phy_is_fixed_link(pdev->dev.of_node))
				of_phy_deregister_fixed_link(pdev->dev.of_node);
		}
		mdiobus_unregister(lp->mii_bus);
		mdiobus_free(lp->mii_bus);

		unregister_netdev(ndev);

		clk_disable_unprepare(lp->apahb_gate);
		clk_disable_unprepare(lp->apapb_gate);

		free_netdev(ndev);
	}

	return 0;
}

static int __maybe_unused dwceqos_clk_disabled(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	int ret;

#if (GMAC_QUICK_CLK == 0)
	struct net_local *lp = netdev_priv(ndev);

	clk_disable_unprepare(lp->ephy_ref_en);
	clk_disable_unprepare(lp->tx_en);
	clk_disable_unprepare(lp->rx_en);
	clk_disable_unprepare(lp->rmii_en);
	clk_disable_unprepare(lp->apahb_gate);
	clk_disable_unprepare(lp->apapb_gate);
#else

	ret = _pltfr_clk_disable(dev);
	if (ret) {
		dev_err(&ndev->dev, "dwceqos clk error\n");
		return ret;
	}
#endif
	printk(KERN_ERR "%s\n", __func__);
	return 0;
}

static int __maybe_unused dwceqos_clk_enable(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	int ret;

#if (GMAC_QUICK_CLK == 0)
	struct net_local *lp = netdev_priv(ndev);

	clk_prepare_enable(lp->apahb_gate);
	clk_prepare_enable(lp->apapb_gate);
	clk_prepare_enable(lp->tx_en);
	clk_prepare_enable(lp->rx_en);
	clk_prepare_enable(lp->rmii_en);
	clk_prepare_enable(lp->ephy_ref_en);
#else
	ret = _pltfr_clk_enable(dev);
	if (ret) {
		dev_err(&ndev->dev, "dwceqos clk error\n");
		return ret;
	}
#endif

	return 0;
}



/**
 * dwceqos_pltfr_suspend
 * @dev: device pointer
 * Description: this function is invoked when suspend the driver and it direcly
 * call the main suspend function and then, if required, on some platform, it
 * can call an exit helper.
 */
static int __maybe_unused dwceqos_pltfr_suspend(struct device *dev)
{
	int ret;
	struct net_device *ndev = dev_get_drvdata(dev);
	struct net_local *lp = netdev_priv(ndev);
	printk(KERN_ERR "%s\n",__func__);

	rtnl_lock();
	if (!device_may_wakeup(&lp->pdev->dev)) {
		lp->ndev->wol_enabled = 0;
	} else {
		lp->ndev->wol_enabled = 1;
	}
	rtnl_unlock();

	if (!ndev || !netif_running(ndev)) {
		rtnl_lock();
		phy_stop(ndev->phydev);
		//clk disabled
		ret = dwceqos_clk_disabled(dev);
		rtnl_unlock();

	} else {

		rtnl_lock();


		dwceqos_disable_all_irq(lp);


		tasklet_disable(&lp->tx_bdreclaim_tasklet);
		napi_disable(&lp->napi);
		netif_tx_lock_bh(ndev);
		netif_device_detach(ndev);
		netif_tx_unlock_bh(ndev);
		dwceqos_suspend(lp);
#if 1
		/* The remote wake-up is not support when phy_stop is invoked */
		phy_stop(ndev->phydev);

		//clk disabled
		ret = dwceqos_clk_disabled(dev);
#endif
		device_set_wakeup_enable(&lp->pdev->dev, 1);
		enable_irq_wake(lp->wol_irq);



		printk("device_may_wakeup is %d, lp->pmt:%d\n", device_may_wakeup(&lp->pdev->dev), lp->pmt);

		/* Enable Power down mode by programming the PMT regs */
		if (device_may_wakeup(&lp->pdev->dev) && lp->pmt) {
			dwmac4_pmt(lp, lp->wolopts);
			lp->irq_wake = 1;
		} else {

		}

		rtnl_unlock();


	}

	printk(KERN_ERR "%s done\n",__func__);
	return ret;
}
static void dwceqos_reset_queues_param(struct net_local *lp)
{

	lp->gso_size = 0;

	//lp->tx_skb = NULL;
	//lp->rx_skb = NULL;
	//lp->rx_descs = NULL;
	//lp->tx_descs = NULL;

	/* Reset the DMA indexes */
	lp->rx_cur = 0;
	lp->tx_cur = 0;
	lp->tx_next = 0;
	lp->tx_free = DWCEQOS_TX_DCNT;

	netdev_tx_reset_queue(netdev_get_tx_queue(lp->ndev, 0));

}

static void dwceqos_free_tx_skbufs(struct net_local *lp)
{
	dwceqos_clean_rings(lp);
}
static void dwceqos_clear_descriptors(struct net_local *lp)
{
	int i =0;
	/* Initialize RX Ring Descriptors and buffers */
	for (i = 0; i < DWCEQOS_RX_DCNT; ++i) {
		dwceqos_alloc_rxring_desc(lp, i);
		if (!(lp->rx_skb[lp->rx_cur].skb)) {
			printk(KERN_ERR "alloc_rxring error \n");
		}
			//goto err_out;
	}

	/* Initialize TX Descriptors */
	for (i = 0; i < DWCEQOS_TX_DCNT; ++i) {
		lp->tx_descs[i].des0 = 0;
		lp->tx_descs[i].des1 = 0;
		lp->tx_descs[i].des2 = 0;
		lp->tx_descs[i].des3 = 0;
	}

	/* Make descriptor writes visible to the DMA. */
	wmb();


}
static void dwceqos_hw_setup(struct net_local *lp)
{
	dwceqos_init_hw(lp);
}
static void dwceqos_set_rx_mode2(struct net_local *lp)
{
	return;
}
static void dwceqos_enable_all_queues(struct net_local *lp)
{
	napi_enable(&lp->napi);

}
//netif_device_attach(ndev);


/**
 * dwceqos_pltfr_resume
 * @dev: device pointer
 * Description: this function is invoked when resume the driver before calling
 * the main resume function, on some platforms, it can call own init helper
 * if required.
 */
static int __maybe_unused dwceqos_pltfr_resume(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	struct net_local *lp = netdev_priv(ndev);

	printk(KERN_ERR "%s\n",__func__);

	if (!ndev || !netif_running(ndev)) {
		rtnl_lock();
		dwceqos_clk_enable(dev);
		rtnl_unlock();
	} else {
		rtnl_lock();

	/* Power Down bit, into the PM register, is cleared
	 * automatically as soon as a magic packet or a Wake-up frame
	 * is received. Anyway, it's better to manually clear
	 * this bit because it can generate problems while resuming
	 * from another devices (e.g. serial console).
	 */
	if (device_may_wakeup(&lp->pdev->dev) && lp->pmt) {

		dwmac4_pmt(lp,0);

		lp->irq_wake = 0;
	} else {

	}

		dwceqos_clk_enable(dev);

		dwceqos_reset_state(lp);
		dwceqos_reset_queues_param(lp);
		dwceqos_free_tx_skbufs(lp);
		dwceqos_clear_descriptors(lp);

		lp->phy_defer = true;
		phy_start(ndev->phydev);
		dwceqos_hw_setup(lp);
		dwceqos_set_rx_mode2(lp);
		dwceqos_enable_all_queues(lp);

		netif_tx_lock_bh(ndev);
		netif_device_attach(ndev);
		netif_tx_unlock_bh(ndev);

		netif_start_queue(ndev);
		tasklet_enable(&lp->tx_bdreclaim_tasklet);

		/* Enable Interrupts -- do this only after we enable NAPI and the
		 * tasklet.
		 */
		dwceqos_write(lp, REG_DWCEQOS_DMA_CH0_IE,
				  DWCEQOS_DMA_CH0_IE_NIE |
				  DWCEQOS_DMA_CH0_IE_RIE | DWCEQOS_DMA_CH0_IE_TIE |
				  DWCEQOS_DMA_CH0_IE_AIE |
				  DWCEQOS_DMA_CH0_IE_FBEE);
		rtnl_unlock();

	}

	printk(KERN_ERR "%s done\n",__func__);
	return 0;
}

static int __maybe_unused dwceqos_runtime_suspend(struct device *dev)
{
	printk(KERN_DEBUG "%s\n",__func__);

	return dwceqos_clk_disabled(dev);
}

static int __maybe_unused dwceqos_runtime_resume(struct device *dev)
{
	printk(KERN_DEBUG "%s\n",__func__);

	return dwceqos_clk_enable(dev);
}

static int __maybe_unused dwceqos_pltfr_noirq_suspend(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	struct net_local *lp = netdev_priv(ndev);
	struct platform_device *pdev = lp->pdev;
	int ret;
	printk(KERN_DEBUG "%s\n",__func__);

	if (!netif_running(ndev))
		return 0;

	if (!device_may_wakeup(&(pdev->dev))) {
		/* Disable clock in case of PWM is off */
		dwceqos_clk_disabled(dev);

		ret = pm_runtime_force_suspend(dev);
		if (ret)
			return ret;
	}

	return 0;
}

static int __maybe_unused dwceqos_pltfr_noirq_resume(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);
	struct net_local *lp = netdev_priv(ndev);
	struct platform_device *pdev = lp->pdev;
	int ret;
	printk(KERN_DEBUG "%s\n",__func__);

	if (!netif_running(ndev))
		return 0;

	if (!device_may_wakeup(&(pdev->dev))) {
		/* enable the clk previously disabled */
		ret = pm_runtime_force_resume(dev);
		if (ret)
			return ret;

		ret = dwceqos_clk_enable(dev);
		if (ret < 0) {
			netdev_warn(ndev,
				    "failed to enable PTP reference clock: %pe\n",
				    ERR_PTR(ret));
			return ret;
		}
	}

	return 0;
}

const struct dev_pm_ops dwceqos_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(dwceqos_pltfr_suspend, dwceqos_pltfr_resume)
	//SET_RUNTIME_PM_OPS(dwceqos_runtime_suspend, dwceqos_runtime_resume, NULL)
	//SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(dwceqos_pltfr_noirq_suspend, dwceqos_pltfr_noirq_resume)
};

static struct platform_driver dwceqos_driver = {
	.probe   = dwceqos_probe,
	.remove  = dwceqos_remove,
	.driver  = {
		.name  = DRIVER_NAME,
		.pm = &dwceqos_pm_ops,
		.of_match_table = dwceq_of_match,
	},
};

module_platform_driver(dwceqos_driver);

MODULE_DESCRIPTION("DWC Ethernet QoS v4.10a driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Andreas Irestaal <andreas.irestal@axis.com>");
MODULE_AUTHOR("Lars Persson <lars.persson@axis.com>");
