#ifndef __MOLCHIP_DWC_ETH_QOS_H__
#define __MOLCHIP_DWC_ETH_QOS_H__

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <errno.h>
#include <memalign.h>
#include <miiphy.h>
#include <net.h>
#include <netdev.h>
#include <phy.h>
#include <reset.h>
#include <wait_bit.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>
/* Core registers */

#define EQOS_MAC_REGS_BASE 0x000
struct qos_mac_regs {
	uint32_t configuration;				/* 0x000 */
	uint32_t unused_004[(0x070 - 0x004) / 4];	/* 0x004 */
	uint32_t q0_tx_flow_ctrl;			/* 0x070 */
	uint32_t unused_070[(0x090 - 0x074) / 4];	/* 0x074 */
	uint32_t rx_flow_ctrl;				/* 0x090 */
	uint32_t unused_094;				/* 0x094 */
	uint32_t txq_prty_map0;				/* 0x098 */
	uint32_t unused_09c;				/* 0x09c */
	uint32_t rxq_ctrl0;				/* 0x0a0 */
	uint32_t unused_0a4;				/* 0x0a4 */
	uint32_t rxq_ctrl2;				/* 0x0a8 */
	uint32_t unused_0ac[(0x0dc - 0x0ac) / 4];	/* 0x0ac */
	uint32_t us_tic_counter;			/* 0x0dc */
	uint32_t unused_0e0[(0x11c - 0x0e0) / 4];	/* 0x0e0 */
	uint32_t hw_feature0;				/* 0x11c */
	uint32_t hw_feature1;				/* 0x120 */
	uint32_t hw_feature2;				/* 0x124 */
	uint32_t unused_128[(0x200 - 0x128) / 4];	/* 0x128 */
	uint32_t mdio_address;				/* 0x200 */
	uint32_t mdio_data;				/* 0x204 */
	uint32_t unused_208[(0x300 - 0x208) / 4];	/* 0x208 */
	uint32_t address0_high;				/* 0x300 */
	uint32_t address0_low;				/* 0x304 */
};

#define EQOS_MAC_CONFIGURATION_GPSLCE			BIT(23)
#define EQOS_MAC_CONFIGURATION_CST			BIT(21)
#define EQOS_MAC_CONFIGURATION_ACS			BIT(20)
#define EQOS_MAC_CONFIGURATION_WD			BIT(19)
#define EQOS_MAC_CONFIGURATION_JD			BIT(17)
#define EQOS_MAC_CONFIGURATION_JE			BIT(16)
#define EQOS_MAC_CONFIGURATION_PS			BIT(15)
#define EQOS_MAC_CONFIGURATION_FES			BIT(14)
#define EQOS_MAC_CONFIGURATION_DM			BIT(13)
#define EQOS_MAC_CONFIGURATION_TE			BIT(1)
#define EQOS_MAC_CONFIGURATION_RE			BIT(0)

#define EQOS_MAC_Q0_TX_FLOW_CTRL_PT_SHIFT		16
#define EQOS_MAC_Q0_TX_FLOW_CTRL_PT_MASK		0xffff
#define EQOS_MAC_Q0_TX_FLOW_CTRL_TFE			BIT(1)

#define EQOS_MAC_RX_FLOW_CTRL_RFE			BIT(0)

#define EQOS_MAC_TXQ_PRTY_MAP0_PSTQ0_SHIFT		0
#define EQOS_MAC_TXQ_PRTY_MAP0_PSTQ0_MASK		0xff

#define EQOS_MAC_RXQ_CTRL0_RXQ0EN_SHIFT			0
#define EQOS_MAC_RXQ_CTRL0_RXQ0EN_MASK			3
#define EQOS_MAC_RXQ_CTRL0_RXQ0EN_NOT_ENABLED		0
#define EQOS_MAC_RXQ_CTRL0_RXQ0EN_ENABLED_DCB		2

#define EQOS_MAC_RXQ_CTRL2_PSRQ0_SHIFT			0
#define EQOS_MAC_RXQ_CTRL2_PSRQ0_MASK			0xff

#define EQOS_MAC_HW_FEATURE1_TXFIFOSIZE_SHIFT		6
#define EQOS_MAC_HW_FEATURE1_TXFIFOSIZE_MASK		0x1f
#define EQOS_MAC_HW_FEATURE1_RXFIFOSIZE_SHIFT		0
#define EQOS_MAC_HW_FEATURE1_RXFIFOSIZE_MASK		0x1f

#define EQOS_MAC_MDIO_ADDRESS_PA_SHIFT			21
#define EQOS_MAC_MDIO_ADDRESS_RDA_SHIFT			16
#define EQOS_MAC_MDIO_ADDRESS_CR_SHIFT			8
#define EQOS_MAC_MDIO_ADDRESS_CR_20_35			2
#define EQOS_MAC_MDIO_ADDRESS_CR_35_60			3
#define EQOS_MAC_MDIO_ADDRESS_CR_60_100			0
#define EQOS_MAC_MDIO_ADDRESS_CR_100_150		1
#define EQOS_MAC_MDIO_ADDRESS_CR_150_250		4
#define EQOS_MAC_MDIO_ADDRESS_CR_250_300		5

#define EQOS_MAC_MDIO_ADDRESS_SKAP			BIT(4)
#define EQOS_MAC_MDIO_ADDRESS_GOC_SHIFT			2
#define EQOS_MAC_MDIO_ADDRESS_GOC_READ			3
#define EQOS_MAC_MDIO_ADDRESS_GOC_WRITE			1
#define EQOS_MAC_MDIO_ADDRESS_C45E			BIT(1)
#define EQOS_MAC_MDIO_ADDRESS_GB			BIT(0)

#define EQOS_MAC_MDIO_DATA_GD_MASK			0xffff

#define EQOS_MTL_REGS_BASE 0xd00
struct qos_mtl_regs {
	uint32_t txq0_operation_mode;			/* 0xd00 */
	uint32_t unused_d04;				/* 0xd04 */
	uint32_t txq0_debug;				/* 0xd08 */
	uint32_t unused_d0c[(0xd18 - 0xd0c) / 4];	/* 0xd0c */
	uint32_t txq0_quantum_weight;			/* 0xd18 */
	uint32_t unused_d1c[(0xd30 - 0xd1c) / 4];	/* 0xd1c */
	uint32_t rxq0_operation_mode;			/* 0xd30 */
	uint32_t unused_d34;				/* 0xd34 */
	uint32_t rxq0_debug;				/* 0xd38 */
};

#define EQOS_MTL_TXQ0_OPERATION_MODE_TQS_SHIFT		16
#define EQOS_MTL_TXQ0_OPERATION_MODE_TQS_MASK		0x1ff
#define EQOS_MTL_TXQ0_OPERATION_MODE_TXQEN_SHIFT	2
#define EQOS_MTL_TXQ0_OPERATION_MODE_TXQEN_MASK		3
#define EQOS_MTL_TXQ0_OPERATION_MODE_TXQEN_ENABLED	2
#define EQOS_MTL_TXQ0_OPERATION_MODE_TSF		BIT(1)
#define EQOS_MTL_TXQ0_OPERATION_MODE_FTQ		BIT(0)

#define EQOS_MTL_TXQ0_DEBUG_TXQSTS			BIT(4)
#define EQOS_MTL_TXQ0_DEBUG_TRCSTS_SHIFT		1
#define EQOS_MTL_TXQ0_DEBUG_TRCSTS_MASK			3

#define EQOS_MTL_RXQ0_OPERATION_MODE_RQS_SHIFT		20
#define EQOS_MTL_RXQ0_OPERATION_MODE_RQS_MASK		0x3ff
#define EQOS_MTL_RXQ0_OPERATION_MODE_RFD_SHIFT		14
#define EQOS_MTL_RXQ0_OPERATION_MODE_RFD_MASK		0x3f
#define EQOS_MTL_RXQ0_OPERATION_MODE_RFA_SHIFT		8
#define EQOS_MTL_RXQ0_OPERATION_MODE_RFA_MASK		0x3f
#define EQOS_MTL_RXQ0_OPERATION_MODE_EHFC		BIT(7)
#define EQOS_MTL_RXQ0_OPERATION_MODE_RSF		BIT(5)

#define EQOS_MTL_RXQ0_DEBUG_PRXQ_SHIFT			16
#define EQOS_MTL_RXQ0_DEBUG_PRXQ_MASK			0x7fff
#define EQOS_MTL_RXQ0_DEBUG_RXQSTS_SHIFT		4
#define EQOS_MTL_RXQ0_DEBUG_RXQSTS_MASK			3

#define EQOS_DMA_REGS_BASE 0x1000
struct qos_dma_regs {
	uint32_t mode;					/* 0x1000 */
	uint32_t sysbus_mode;				/* 0x1004 */
	uint32_t unused_1008[(0x1100 - 0x1008) / 4];	/* 0x1008 */
	uint32_t ch0_control;				/* 0x1100 */
	uint32_t ch0_tx_control;			/* 0x1104 */
	uint32_t ch0_rx_control;			/* 0x1108 */
	uint32_t unused_110c;				/* 0x110c */
	uint32_t ch0_txdesc_list_haddress;		/* 0x1110 */
	uint32_t ch0_txdesc_list_address;		/* 0x1114 */
	uint32_t ch0_rxdesc_list_haddress;		/* 0x1118 */
	uint32_t ch0_rxdesc_list_address;		/* 0x111c */
	uint32_t ch0_txdesc_tail_pointer;		/* 0x1120 */
	uint32_t unused_1124;				/* 0x1124 */
	uint32_t ch0_rxdesc_tail_pointer;		/* 0x1128 */
	uint32_t ch0_txdesc_ring_length;		/* 0x112c */
	uint32_t ch0_rxdesc_ring_length;		/* 0x1130 */
};

#define EQOS_DMA_MODE_SWR				BIT(0)

#define EQOS_DMA_SYSBUS_MODE_RD_OSR_LMT_SHIFT		16
#define EQOS_DMA_SYSBUS_MODE_RD_OSR_LMT_MASK		0xf
#define EQOS_DMA_SYSBUS_MODE_EAME			BIT(11)
#define EQOS_DMA_SYSBUS_MODE_BLEN16			BIT(3)
#define EQOS_DMA_SYSBUS_MODE_BLEN8			BIT(2)
#define EQOS_DMA_SYSBUS_MODE_BLEN4			BIT(1)

#define EQOS_DMA_CH0_CONTROL_PBLX8			BIT(16)

#define EQOS_DMA_CH0_TX_CONTROL_TXPBL_SHIFT		16
#define EQOS_DMA_CH0_TX_CONTROL_TXPBL_MASK		0x3f
#define EQOS_DMA_CH0_TX_CONTROL_OSP			BIT(4)
#define EQOS_DMA_CH0_TX_CONTROL_ST			BIT(0)

#define EQOS_DMA_CH0_RX_CONTROL_RXPBL_SHIFT		16
#define EQOS_DMA_CH0_RX_CONTROL_RXPBL_MASK		0x3f
#define EQOS_DMA_CH0_RX_CONTROL_RBSZ_SHIFT		1
#define EQOS_DMA_CH0_RX_CONTROL_RBSZ_MASK		0x3fff
#define EQOS_DMA_CH0_RX_CONTROL_SR			BIT(0)


#define EQOS_SDMEMCOMPPADCTRL_PAD_E_INPUT_OR_E_PWRD	BIT(31)

#define EQOS_AUTO_CAL_CONFIG_START			BIT(31)
#define EQOS_AUTO_CAL_CONFIG_ENABLE			BIT(29)

#define EQOS_AUTO_CAL_STATUS_ACTIVE			BIT(31)

/* Descriptors */

#define EQOS_DESCRIPTOR_WORDS	4
#define EQOS_DESCRIPTOR_SIZE	(EQOS_DESCRIPTOR_WORDS * 4)
/* We assume ARCH_DMA_MINALIGN >= 16; 16 is the EQOS HW minimum */
#define EQOS_DESCRIPTOR_ALIGN	ARCH_DMA_MINALIGN
#define EQOS_DESCRIPTORS_TX	4
#define EQOS_DESCRIPTORS_RX	4
#define EQOS_DESCRIPTORS_NUM	(EQOS_DESCRIPTORS_TX + EQOS_DESCRIPTORS_RX)
#define EQOS_DESCRIPTORS_SIZE	ALIGN(EQOS_DESCRIPTORS_NUM * \
				      EQOS_DESCRIPTOR_SIZE, ARCH_DMA_MINALIGN)
#define EQOS_BUFFER_ALIGN	ARCH_DMA_MINALIGN
#define EQOS_MAX_PACKET_SIZE	ALIGN(1568, ARCH_DMA_MINALIGN)
#define EQOS_RX_BUFFER_SIZE	(EQOS_DESCRIPTORS_RX * EQOS_MAX_PACKET_SIZE)

#define MDIO_WRITE_CNT 2
/*
 * Warn if the cache-line size is larger than the descriptor size. In such
 * cases the driver will likely fail because the CPU needs to flush the cache
 * when requeuing RX buffers, therefore descriptors written by the hardware
 * may be discarded. Architectures with full IO coherence, such as x86, do not
 * experience this issue, and hence are excluded from this condition.
 *
 * This can be fixed by defining CONFIG_SYS_NONCACHED_MEMORY which will cause
 * the driver to allocate descriptors from a pool of non-cached memory.
 */
#ifdef MMU_DCACHE_EN
#define EQOS_DESC_CACHE_DBG
#endif

#ifdef EQOS_DESC_CACHE_DBG
#if EQOS_DESCRIPTOR_SIZE < ARCH_DMA_MINALIGN
#if !defined(CONFIG_SYS_NONCACHED_MEMORY) && \
	!defined(CONFIG_SYS_DCACHE_OFF) && !defined(CONFIG_X86)
#warning Cache line size is larger than descriptor size
#endif
#endif
#endif

#if defined(CONFIG_GMAC_MOLCHIP_FDT)

#define	INTERNAL_PHY		0
#define	EXTERNAL_PHY		1

#define	CHIP_PLATFORM		1
#define	HAPS_PLATFORM		2

enum{
	MII_INTF = 0,
	RGMII_INTF = 1,
	RMII_INTF = 4,
};

struct dwceqos_reg_cfg{
	union {
		phys_addr_t base;
		struct {
			u32 base_low;
			u32 base_high;
		}s;
	} reg_base;
	u32 reg_offset;
	u32 bit_offset;
	u32 bit_mask;
	u32 bit_val;
};
#else

#define	INTERNAL_PHY		0
#define	EXTERNAL_PHY		1

#endif


struct qos_desc {
	u32 des0;
	u32 des1;
	u32 des2;
	u32 des3;
};

#define EQOS_DESC3_OWN		BIT(31)
#define EQOS_DESC3_FD		BIT(29)
#define EQOS_DESC3_LD		BIT(28)
#define EQOS_DESC3_BUF1V	BIT(24)

struct qos_config {
	bool reg_access_always_ok;
};

struct dpll_addr_map {
	fdt_addr_t txdpll_base;
	fdt_addr_t rxdpll_base;
	fdt_addr_t negrxdpll_base;
	fdt_addr_t eco0_base;
	fdt_addr_t eco1_base;
	bool dll_addr_is_fixup;
};
struct record_eth {
	struct udevice *dev;
	int index;
};

struct gmac_cap {
	bool has_txrx_delay;
	u32 dpll_version;
};

struct qos_priv {
	struct udevice *dev;
	const struct qos_config *config;
	fdt_addr_t regs;
	struct qos_mac_regs *mac_regs;
	struct qos_mtl_regs *mtl_regs;
	struct qos_dma_regs *dma_regs;
	struct clk clk_master_bus;
	struct clk clk_rx;
	struct clk clk_ptp_ref;
	struct clk clk_tx;
	struct clk clk_slave_bus;
	struct mii_dev *mii;
	struct phy_device *phy;
	void *descs;
	struct qos_desc *tx_descs;
	struct qos_desc *rx_descs;
	int tx_desc_idx, rx_desc_idx;
	void *tx_dma_buf;
	void *rx_dma_buf;
	void *rx_pkt;
	bool started;
	bool reg_access_ok;
	u32	phy_interface;
	int	phy_addr;
	struct gmac_cap gmac_cap;
	struct dpll_addr_map dpll_addr_map;
	int	tx_delay;
	int	rx_delay;
	int	negrx_delay;
	int	ctrl_id;
#if defined(CONFIG_GMAC_MOLCHIP_FDT)
	/*gmac soc cfg*/
	unsigned int apahb_base;
	unsigned int apapb_base;
	unsigned int glapb_base;
	struct dwceqos_reg_cfg gmac_axi_en; 
	struct dwceqos_reg_cfg gmac_csr_en;
	struct dwceqos_reg_cfg phy_type_sel; 
	struct dwceqos_reg_cfg rmii_pad_sel;
	struct dwceqos_reg_cfg phy_intf_sel;
	struct dwceqos_reg_cfg ref_en_cfg;
	struct dwceqos_reg_cfg ephy_soc_sel;
	struct dwceqos_reg_cfg soft_reset_cfg;
	struct dwceqos_reg_cfg tx_en;
	struct dwceqos_reg_cfg rx_en;
	struct dwceqos_reg_cfg rmii_en;

	uint32_t platform;
#endif
	uint32_t phy_type;
	int ephy_test_pattern;
	int autoneg;
	int speed;
	int duplex;
};

struct phy_dpll_iteam {
	u32 phy_id;
	int ctrl_id;
	int tx_delay;
	int rx_delay;
	int negrx_delay;
};

/**dpll config***/
#define REG_GMAC_TXDLL_CTRL0 (eqos->dpll_addr_map.txdpll_base + 0x00)
#define REG_GMAC_TXDLL_CTRL1 (eqos->dpll_addr_map.txdpll_base + 0x04)
#define REG_GMAC_TXDLL_CTRL2 (eqos->dpll_addr_map.txdpll_base + 0x08)
#define REG_GMAC_TXDLL_CTRL3 (eqos->dpll_addr_map.txdpll_base + 0x0c)

#define REG_GMAC_RXDLL_CTRL0 (eqos->dpll_addr_map.rxdpll_base + 0x00)
#define REG_GMAC_RXDLL_CTRL1 (eqos->dpll_addr_map.rxdpll_base + 0x04)
#define REG_GMAC_RXDLL_CTRL2 (eqos->dpll_addr_map.rxdpll_base + 0x08)
#define REG_GMAC_RXDLL_CTRL3 (eqos->dpll_addr_map.rxdpll_base + 0x0c)


#define REG_GMAC_RXDLL_ECO0 (eqos->dpll_addr_map.eco0_base)
#define REG_GMAC_RXDLL_ECO1 (eqos->dpll_addr_map.eco1_base)

#endif /* __MOLCHIP_DWC_ETH_QOS_H__ */
