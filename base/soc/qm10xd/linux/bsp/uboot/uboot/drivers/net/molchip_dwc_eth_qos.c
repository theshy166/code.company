/*
 * Copyright (c) 2016, NVIDIA CORPORATION.
 *
 * SPDX-License-Identifier: GPL-2.0
 *
 * Portions based on U-Boot's rtl8169.c.
 */

/*
 * This driver supports the Synopsys Designware Ethernet QOS (Quality Of
 * Service) IP block. The IP supports multiple options for bus type, clocking/
 * reset structure, and feature list.
 *
 * The driver is written such that generic core logic is kept separate from
 * configuration-specific logic. Code that interacts with configuration-
 * specific resources is split out into separate functions to avoid polluting
 * common code. If/when this driver is enhanced to support multiple
 * configurations, the core code should be adapted to call all configuration-
 * specific functions through function pointers, with the definition of those
 * function pointers being supplied by struct udevice_id eqos_ids[]'s .data
 * field.
 *
 * The following configurations are currently supported:
 * tegra186:
 *    NVIDIA's Tegra186 chip. This configuration uses an AXI master/DMA bus, an
 *    AHB slave/register bus, contains the DMA, MTL, and MAC sub-blocks, and
 *    supports a single RGMII PHY. This configuration also has SW control over
 *    all clock and reset signals to the HW block.
 */
#include "molchip_dwc_eth_qos.h"

struct phy_dpll_iteam gmac_dpll_config_table[] = {
/*
        {0x1cc916,0x0,0x2e,0x23,0xa},
        {0x1cc916,0x1,0x2e,0x26,0xa}
        */
};

struct record_eth record_eths[4];
static int scan_dpll_table(struct udevice *dev)
{
	int i = 0;
	struct phy_dpll_iteam *temp;
	struct qos_priv *eqos = dev_get_priv(dev);
	u32 phy_id = 0xffffffff;
	int r = get_phy_id(eqos->mii, eqos->phy_addr, 0, &phy_id);
	debug("\n%s: return:%d phy_id:0x%x\n", __func__, r,phy_id);
	if (r != 0 || ((phy_id & 0x1fffffff) == 0x1fffffff)) {
		printf("\nfailed find phyid by c22\n");
		return -1;
	}

	for (i = 0; i < ARRAY_SIZE(gmac_dpll_config_table); i ++) {
		temp = &gmac_dpll_config_table[i];
		if ( (temp->phy_id == phy_id) && (temp->ctrl_id == eqos->ctrl_id) ) {
			eqos->tx_delay = temp->tx_delay;
			eqos->rx_delay = temp->rx_delay;
			eqos->negrx_delay = temp->negrx_delay;
			printf("\nfind phy_id:0x%x ctrl_id:0x%x, tx_delay:0x%x,rx_delay:0x%x,negrx_delay:0x%x\n",
			phy_id, eqos->ctrl_id, eqos->tx_delay, eqos->rx_delay,eqos->negrx_delay);
			return 0;
		}
		debug("\nphy_id:0x%x ctrl_id:0x%x, tx_delay:0x%x,rx_delay:0x%x,negrx_delay:0x%x\n",
		temp->phy_id, temp->ctrl_id, temp->tx_delay, temp->rx_delay,temp->negrx_delay);
	}

	return -2;
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


static int config_eth_rxnegdelay(struct udevice *dev, int value)
{
	struct qos_priv *eqos = dev_get_priv(dev);
	unsigned int rdata0 = 0x0;
	unsigned int rdata1 = 0x0;
	unsigned int checkval = 0x0;
	
	value &= GENMASK(7, 0);
	rdata0 = readl(REG_GMAC_RXDLL_ECO1);
	rdata1 = readl(REG_GMAC_RXDLL_ECO0);
	
	if (eqos->ctrl_id == 0) {
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
	} else if (eqos->ctrl_id == 1) {
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
		printf("error gmac ctrl_id!!!\n");
	}

	return 0;
}

/*V2*/

static int bit_modify_rdata(u32 *val, u32 bit_idx, u32 bit_mask, u32 bit_val)
{
	*val = (*val & (~(bit_mask << bit_idx)));
	*val = (*val | (bit_val << bit_idx));
	printf("set val:0x%x\n",*val);
	return 0;
}
static int config_txdll(struct udevice *dev, u8 val)
{
	u32 rdata;
	struct qos_priv *eqos = dev_get_priv(dev);
	rdata = readl(eqos->dpll_addr_map.txdpll_base);
	printf("tx set=0x%08x\n",rdata);
	
	bit_modify_rdata(&rdata, 9, 1, 0);
	writel(rdata,eqos->dpll_addr_map.txdpll_base);
	bit_modify_rdata(&rdata, 11, 1, 1);
	writel(rdata,eqos->dpll_addr_map.txdpll_base);
	bit_modify_rdata(&rdata, 0, 0xff, val);
	writel(rdata,eqos->dpll_addr_map.txdpll_base);
	bit_modify_rdata(&rdata, 8, 0x1, 1);
	writel(rdata,eqos->dpll_addr_map.txdpll_base);
	bit_modify_rdata(&rdata, 8, 0x1, 0);
	writel(rdata,eqos->dpll_addr_map.txdpll_base);
	bit_modify_rdata(&rdata, 11, 1, 0);
	writel(rdata,eqos->dpll_addr_map.txdpll_base);
	bit_modify_rdata(&rdata, 9, 1, 1);
	writel(rdata,eqos->dpll_addr_map.txdpll_base);
	
	return 0;
}
static int config_rxdll(struct udevice *dev, u8 val)
{
	u32 rdata;
	struct qos_priv *eqos = dev_get_priv(dev);
	rdata = readl(eqos->dpll_addr_map.rxdpll_base);
	printf("rx set=0x%08x\n",rdata);

	bit_modify_rdata(&rdata, 21, 1, 0);
	writel(rdata,eqos->dpll_addr_map.rxdpll_base);
	bit_modify_rdata(&rdata, 23, 1, 1);
	writel(rdata,eqos->dpll_addr_map.rxdpll_base);
	bit_modify_rdata(&rdata, 12, 0xff, val);
	writel(rdata,eqos->dpll_addr_map.rxdpll_base);
	bit_modify_rdata(&rdata, 20, 0x1, 1);
	writel(rdata,eqos->dpll_addr_map.rxdpll_base);
	bit_modify_rdata(&rdata, 20, 0x1, 0);
	writel(rdata,eqos->dpll_addr_map.rxdpll_base);
	bit_modify_rdata(&rdata, 23, 1, 0);
	writel(rdata,eqos->dpll_addr_map.rxdpll_base);
	bit_modify_rdata(&rdata, 21, 1, 1);
	writel(rdata,eqos->dpll_addr_map.rxdpll_base);

	return 0;
}
static int config_rxdll_neg(struct udevice *dev, u8 val)
{
	u32 rdata;
	struct qos_priv *eqos = dev_get_priv(dev);
	rdata = readl(eqos->dpll_addr_map.negrxdpll_base);
	printf("rxneg set=0x%08x\n",rdata);
	
	bit_modify_rdata(&rdata, 9, 1, 0);
	writel(rdata,eqos->dpll_addr_map.negrxdpll_base);
	bit_modify_rdata(&rdata, 11, 1, 1);
	writel(rdata,eqos->dpll_addr_map.negrxdpll_base);
	bit_modify_rdata(&rdata, 0, 0xff, val);
	writel(rdata,eqos->dpll_addr_map.negrxdpll_base);
	bit_modify_rdata(&rdata, 8, 0x1, 1);
	writel(rdata,eqos->dpll_addr_map.negrxdpll_base);
	bit_modify_rdata(&rdata, 8, 0x1, 0);
	writel(rdata,eqos->dpll_addr_map.negrxdpll_base);
	bit_modify_rdata(&rdata, 11, 1, 0);
	writel(rdata,eqos->dpll_addr_map.negrxdpll_base);
	bit_modify_rdata(&rdata, 9, 1, 1);
	writel(rdata,eqos->dpll_addr_map.negrxdpll_base);

	return 0;
}

static int config_dpll_v2(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);

	debug("\n%s: config GMAC dpll ctrl_id:0x%x,tx_delay:0x%x,rx_delay:0x%x\n",
			__func__, eqos->ctrl_id, eqos->tx_delay, eqos->rx_delay);

	if ((eqos->dpll_addr_map.txdpll_base != FDT_ADDR_T_NONE) && (eqos->tx_delay >= 0) 
		&& (eqos->tx_delay <= 0xff)) {
		config_txdll(dev, eqos->tx_delay);
	} else {
		printf("%s skip gmac txdelay config\n",dev->name);
	}

	if ((eqos->dpll_addr_map.rxdpll_base != FDT_ADDR_T_NONE) && (eqos->rx_delay >= 0) 
		&& (eqos->rx_delay <= 0xff)) {
		config_rxdll(dev,eqos->rx_delay);
	} else {
		printf("%s skip gmac rxdelay config\n",dev->name);
	}

	if(eqos->dpll_addr_map.negrxdpll_base != FDT_ADDR_T_NONE){
			if( (eqos->negrx_delay >= 0) && (eqos->negrx_delay <= 0xff)) {
				config_rxdll_neg(dev, eqos->negrx_delay);
			} else {
				printf("%s skip gmac negrx_delay config\n",dev->name);
			}
			
	}
	return 0;
}

static int config_dpll(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);


	debug("\n%s: config GMAC dpll ctrl_id:0x%x,tx_delay:0x%x,rx_delay:0x%x\n",
			__func__, eqos->ctrl_id, eqos->tx_delay, eqos->rx_delay);
	debug("\n%s: config GMAC dpll txdpll_base:0x%lx,rxdpll_base:0x%lx,eco0_base:0x%lx,eco1_base:0x%lx\n",
			__func__, eqos->dpll_addr_map.txdpll_base, eqos->dpll_addr_map.rxdpll_base,
			eqos->dpll_addr_map.eco0_base, eqos->dpll_addr_map.eco1_base);

	if ((eqos->dpll_addr_map.txdpll_base != FDT_ADDR_T_NONE) && (eqos->tx_delay >= 0) 
		&& (eqos->tx_delay <= 0xff)) {
		
		writel(0x19,REG_GMAC_TXDLL_CTRL0);
		writel(eqos->tx_delay,REG_GMAC_TXDLL_CTRL2);
		writel(0x6a04,REG_GMAC_TXDLL_CTRL1);
		writel(0x4a04,REG_GMAC_TXDLL_CTRL1);
	} else {
		printf("%s skip gmac txdelay config\n",dev->name);
	}

	if ((eqos->dpll_addr_map.rxdpll_base != FDT_ADDR_T_NONE) && (eqos->rx_delay >= 0) 
		&& (eqos->rx_delay <= 0xff)) {
		
		writel(0x19,REG_GMAC_RXDLL_CTRL0);
		if (eqos->dpll_addr_map.dll_addr_is_fixup == true)
			writel(eqos->rx_delay, 0x34400400);
		else
			writel(eqos->rx_delay,REG_GMAC_RXDLL_CTRL2);
		writel(0x6a04,REG_GMAC_RXDLL_CTRL1);
		writel(0x4a04,REG_GMAC_RXDLL_CTRL1);

	} else {
		printf("%s skip gmac rxdelay config\n",dev->name);
	}

	if((eqos->dpll_addr_map.eco0_base != FDT_ADDR_T_NONE) && 
		(eqos->dpll_addr_map.eco1_base != FDT_ADDR_T_NONE)){
			if( (eqos->negrx_delay >= 0) && (eqos->negrx_delay <= 0xff)) {
				config_eth_rxnegdelay(dev, eqos->negrx_delay);
			} else {
				printf("%s skip gmac negrx_delay config\n",dev->name);
			}
			
		}


	return 0;
}
#if defined(CONFIG_GMAC_MOLCHIP_FDT)
static void qos_update_bits(unsigned int base_regs, unsigned int offset_reg,unsigned int mask,unsigned int val)
{
	writel((readl(base_regs + offset_reg) & (~mask)) | val ,
		(base_regs + offset_reg));
}
#if 0
/*true- internal phy   false-external phy*/
static bool qos_internal_phy_sel(unsigned int base_regs, unsigned int offset_reg,unsigned int bit_mask,unsigned int bit_offset)
{
	unsigned int reg_val;
	reg_val = (readl(base_regs + offset_reg) & (bit_mask << bit_offset)) >> bit_offset;

	if(reg_val == INTERNAL_PHY)
		return true;
	else
		return false;
	 
}
#endif
static int qos_lowlevel_init(struct qos_priv *eqos)
{

#if  defined(CONFIG_TARGET_XC01)

	if ( (eqos->phy_type == EXTERNAL_PHY) && (eqos->phy_interface == PHY_INTERFACE_MODE_RGMII)) {
		//rgmii !!!!!! is not compatible	
		printf("enable rgmii ephy ref clk\n");
		writel((readl(0x2c100004) | BIT(25)), 0x2c100004);
	}

	if (eqos->phy_type == INTERNAL_PHY) {
		printf("enable config ephy\n");
		writel(readl(0x1c0001a0) | BIT(3), 0x1c0001a0);
		writel((readl(0x1f000300) & ~BIT(0)), 0x1f000300);
	}

#endif
	//CKG_AXI_GMAC_en
	qos_update_bits(eqos->gmac_axi_en.reg_base.base, eqos->gmac_axi_en.reg_offset,
		eqos->gmac_axi_en.bit_mask << eqos->gmac_axi_en.bit_offset, eqos->gmac_axi_en.bit_val << eqos->gmac_axi_en.bit_offset);
	//tx_en
	qos_update_bits(eqos->tx_en.reg_base.base, eqos->tx_en.reg_offset,
		eqos->tx_en.bit_mask << eqos->tx_en.bit_offset, eqos->tx_en.bit_val << eqos->tx_en.bit_offset);
	//rx_en
	qos_update_bits(eqos->rx_en.reg_base.base, eqos->rx_en.reg_offset,
		eqos->rx_en.bit_mask << eqos->rx_en.bit_offset, eqos->rx_en.bit_val << eqos->rx_en.bit_offset);
	//rmii_en
	qos_update_bits(eqos->rmii_en.reg_base.base, eqos->rmii_en.reg_offset,
		eqos->rmii_en.bit_mask << eqos->rmii_en.bit_offset, eqos->rmii_en.bit_val << eqos->rmii_en.bit_offset);
	//CKG_CSR_GMAC_EN
	qos_update_bits(eqos->gmac_csr_en.reg_base.base, eqos->gmac_csr_en.reg_offset,
		eqos->gmac_csr_en.bit_mask << eqos->gmac_csr_en.bit_offset, eqos->gmac_csr_en.bit_val << eqos->gmac_csr_en.bit_offset);
	//ckg_gmac_rmii_pad_sel
	qos_update_bits(eqos->rmii_pad_sel.reg_base.base, eqos->rmii_pad_sel.reg_offset,
		eqos->rmii_pad_sel.bit_mask << eqos->rmii_pad_sel.bit_offset, eqos->rmii_pad_sel.bit_val << eqos->rmii_pad_sel.bit_offset);
	//gmac_phy_intf_sel_i
	qos_update_bits(eqos->phy_intf_sel.reg_base.base, eqos->phy_intf_sel.reg_offset,
		eqos->phy_intf_sel.bit_mask << eqos->phy_intf_sel.bit_offset, eqos->phy_intf_sel.bit_val << eqos->phy_intf_sel.bit_offset);
	//ephy_external_sel 0x0:125M 0x1:50M 0x2:10M
	qos_update_bits(eqos->phy_type_sel.reg_base.base, eqos->phy_type_sel.reg_offset,
		eqos->phy_type_sel.bit_mask << eqos->phy_type_sel.bit_offset, eqos->phy_type_sel.bit_val << eqos->phy_type_sel.bit_offset);
	//CKG_EPHY_REF_EN
	qos_update_bits( eqos->ref_en_cfg.reg_base.base, eqos->ref_en_cfg.reg_offset,
		eqos->ref_en_cfg.bit_mask << eqos->ref_en_cfg.bit_offset, eqos->ref_en_cfg.bit_val << eqos->ref_en_cfg.bit_offset);

	if(eqos->phy_type == EXTERNAL_PHY) {

		if(eqos->platform  == CHIP_PLATFORM) {
			/* EPHY Soft reset */
			qos_update_bits(eqos->soft_reset_cfg.reg_base.base, eqos->soft_reset_cfg.reg_offset,
				eqos->soft_reset_cfg.bit_mask << eqos->soft_reset_cfg.bit_offset, 1 << eqos->soft_reset_cfg.bit_offset);
			mdelay(50);
			qos_update_bits(eqos->soft_reset_cfg.reg_base.base, eqos->soft_reset_cfg.reg_offset,
				eqos->soft_reset_cfg.bit_mask << eqos->soft_reset_cfg.bit_offset, 0 << eqos->soft_reset_cfg.bit_offset);
			mdelay(100);
		}
		#if 0
		//gpio to reset ephy
		writel(0x1, 0x10200204 );
		writel(0x20,(0x0b020000 + 0x4));
		writel(0x20,(0x0b020000 + 0x8));
		writel(0x00,(0x0b020000));
		mdelay(50);
		writel(0x20,(0x0b020000));
		mdelay(100);
		#endif
	} else if(eqos->phy_type == INTERNAL_PHY)  {
		#if 0	
		//other gamc is also internal phy,should force & warning	
		if(qos_internal_phy_sel(eqos->apahb_base,eqos->ephy_soc_sel.reg_offset,eqos->ephy_soc_sel.bit_mask,eqos->ephy_soc_sel.bit_offset)){
			qos_update_bits(eqos->apahb_base, eqos->ephy_soc_sel.reg_offset,
				eqos->ephy_soc_sel.bit_mask << eqos->ephy_soc_sel.bit_offset, EXTERNAL_PHY << eqos->ephy_soc_sel.bit_offset);
			printf("warning, two gmac both internal phy,forcing other gmac to external phy mode....\n");
		}
		#endif
			/* EPHY Soft reset */
			qos_update_bits(eqos->apahb_base, eqos->soft_reset_cfg.reg_offset,
				eqos->soft_reset_cfg.bit_mask << eqos->soft_reset_cfg.bit_offset, 1 << eqos->soft_reset_cfg.bit_offset);
			mdelay(50);
			qos_update_bits(eqos->apahb_base, eqos->soft_reset_cfg.reg_offset,
				eqos->soft_reset_cfg.bit_mask << eqos->soft_reset_cfg.bit_offset, 0 << eqos->soft_reset_cfg.bit_offset);
			mdelay(100);

	} else {
		error("error phy type select!\n");
		return -ENXIO;

	}	

	return 0;
}

#else
static int qos_lowlevel_init(struct qos_priv *eqos)
{
	u32 phy_interface = eqos->phy_interface;
	/* enable */
	writel(readl(GMAC_CFG_BASE) | CKG_AXI_GMAC0_EN | CKG_GMAC0_CSR_EN, GMAC_CFG_BASE);
	writel(readl(REG_CEN_PERI_CLK_CTRL) | AXI_GMAC0_CLK_EN, REG_CEN_PERI_CLK_CTRL);
	writel(readl(GMAC_CTRL1_BASE) | GMAC0_RMII_EN | GMAC0_RX_EN | GMAC0_TX_EN, GMAC_CTRL1_BASE);

	if (phy_interface == PHY_INTERFACE_MODE_RMII) {
		/* set gmac rmii interface */
#if  defined(CONFIG_TARGET_FY01)
		writel(readl(REG_AP_AHB_BASE) & (~(7 << 16)),
			REG_AP_AHB_BASE);
		writel(readl(REG_AP_AHB_BASE) | (3 << 16),
			REG_AP_AHB_BASE);
		writel(0x4, REG_AP_GMAC);
#elif defined(CONFIG_TARGET_LT00)
		writel((readl(GMAC_CFG_BASE) & (~ GENMASK(3,2))) | (1 << 2) , GMAC_CFG_BASE);
		writel((readl(GMAC_CTRL0_BASE) & (~ GENMASK(15,13))) | (4 << 13), GMAC_CTRL0_BASE);
		writel(readl(CKG_EPHY_REF_EN) | CKG_EPHY0_REF_EN | CKG_APB_PHYCFG_EN, CKG_EPHY_REF_EN);

#endif

	} else if (phy_interface == PHY_INTERFACE_MODE_MII) {
		/* default mii */
#if defined(CONFIG_TARGET_FY10P)
		/*in fy10p, when the first net port is mii, the second net port is configured to rgmii*/
		writel(readl(REG_AP_AHB_BASE) | BIT(4) , REG_AP_AHB_BASE); //CKG_AXI_GMAC1_EN
		writel(readl(REG_AP_APB_BASE) | BIT(8) , REG_AP_APB_BASE);//CKG_GMAC1_CSR_EN
		writel(readl(REG_GLB_APB_BASE+0x74)| BIT(0) , (REG_GLB_APB_BASE+0x74));//CKG_EPHY_REF_EN

		writel(readl(REG_AP_AHB_BASE) & (~(0x1 << 12) ),
			REG_AP_AHB_BASE);
		writel(readl(REG_AP_AHB_BASE) & (~(0x3 << 15) ),
			REG_AP_AHB_BASE);
		writel((readl(REG_AP_AHB_BASE) & (~(1 << 12))) | (2 << 15),
			REG_AP_AHB_BASE);
		writel((0x1 << 3), REG_AP_GMAC);

#elif defined(CONFIG_TARGET_LT00)
		writel((readl(GMAC_CFG_BASE) & (~ GENMASK(3,2))) | (1 << 2) , GMAC_CFG_BASE);
		writel(readl(GMAC_CTRL0_BASE) & (~ GENMASK(15,13)), GMAC_CTRL0_BASE);
		writel(readl(CKG_EPHY_REF_EN) | CKG_EPHY0_REF_EN | CKG_APB_PHYCFG_EN, CKG_EPHY_REF_EN);

#endif
	} else if (phy_interface == PHY_INTERFACE_MODE_RGMII) {
#if  defined(CONFIG_TARGET_FY12)
		/* set gmac rgmii pin function interface */
		writel(readl(REG_AP_PERI_APB_CLK_CTRL1) & (~(3 << 13)),
			REG_AP_PERI_APB_CLK_CTRL1);
		writel(readl(CKG_EPHY_REF_EN) | (1 << 0),
			CKG_EPHY_REF_EN);

		/* set gmac rgmii interface */
		writel((readl(REG_AP_PERI_SEL) & (~(7 << 0))) | (1 << 0),
			REG_AP_PERI_SEL);

		/* external phy needs hardware reset by driver */
		writel(readl(REG_AP_AHB_PERI_SOFT_RST) | EPHY0_SOFT_RST,
			REG_AP_AHB_PERI_SOFT_RST);
		mdelay(50);
		writel(readl(REG_AP_AHB_PERI_SOFT_RST) & (~(EPHY0_SOFT_RST)),
			REG_AP_AHB_PERI_SOFT_RST);
		mdelay(100);
#else
#if defined(CONFIG_TARGET_FY10D)
		/* set gmac rgmii pin function interface */
		writel(readl(CKG_EPHY_REF_EN) | (1 << 0),
			CKG_EPHY_REF_EN);

		/* set gmac rgmii interface */
		writel((readl(REG_AP_AHB_BASE) & (~(1 << 11))) | (2 << 13),
			REG_AP_AHB_BASE);
		writel(0x1, REG_AP_GMAC);		
#endif
#if defined(CONFIG_TARGET_FY10P)
		writel(readl(REG_AP_AHB_BASE) | BIT(3) | BIT(4) , REG_AP_AHB_BASE); //CKG_AXI_GMAC0/1_EN
		writel(readl(REG_AP_APB_BASE)| BIT(17)| BIT(8) , REG_AP_APB_BASE);//CKG_GMAC0/1_CSR_EN
		writel(readl(REG_GLB_APB_BASE+0x74)| BIT(0) , (REG_GLB_APB_BASE+0x74));//CKG_EPHY_REF_EN

		writel(readl(REG_AP_AHB_BASE) & (~(0x3 << 11) ),
			REG_AP_AHB_BASE);
		writel(readl(REG_AP_AHB_BASE) & (~(0xf << 13) ),
			REG_AP_AHB_BASE);

		/* set gmac rgmii interface ckg_gmac0_rmii_pad_sel--11--0*/
		/* set gmac rgmii interface ephy_external_sel--13--01*/
		writel((readl(REG_AP_AHB_BASE) & (~(1 << 11))) | (1 << 13),
			REG_AP_AHB_BASE);
		writel((readl(REG_AP_AHB_BASE) & (~(1 << 12))) | (2 << 15),
			REG_AP_AHB_BASE);
		//gmac0/1_phy_intf_sel_i
		writel((0x1 | (0x1 << 3)), REG_AP_GMAC);

		/* external phy needs hardware reset by driver */
		writel((readl(REG_AP_AHB_PERI_SOFT_RST) | BIT(18) | BIT(19)), REG_AP_AHB_PERI_SOFT_RST);
		mdelay(50);
		writel((readl(REG_AP_AHB_PERI_SOFT_RST) & (~BIT(18)) & (~BIT(19))), REG_AP_AHB_PERI_SOFT_RST);
		mdelay(100);

#else
		 /* external phy needs hardware reset by driver */
		error("phy mode not support!\n");
		return -1;

#endif

#endif
	} else {
		error("%s(invalid interface=%d):\n", __func__, phy_interface);
		return -1;
	}
	if(eqos->phy_type == EXTERNAL_PHY) {
		//external rmii mode
		writel((readl(GMAC_CTRL0_BASE) & (~ GENMASK(1,0))) | (1 << 0), GMAC_CTRL0_BASE);
		writel(readl(GMAC_CTRL0_BASE) | ANA_CTRL_EPHY_RESET,
			GMAC_CTRL0_BASE);
		mdelay(50);
		writel(readl(GMAC_CTRL0_BASE) & (~(ANA_CTRL_EPHY_RESET)),
			GMAC_CTRL0_BASE);
		mdelay(100);

	}else if (eqos->phy_type == INTERNAL_PHY) {
		writel(readl(GMAC_CTRL0_BASE) & (~ GENMASK(1,0)), GMAC_CTRL0_BASE);
		writel(readl(CKG_EPHY_REF_EN) | ANA_CTRL_EPHY_RESET,
			CKG_EPHY_REF_EN);
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		mdelay(50);
		#else
		udelay(100);
		#endif
		writel(readl(CKG_EPHY_REF_EN) & (~(ANA_CTRL_EPHY_RESET)),
			CKG_EPHY_REF_EN);
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		mdelay(100);
		#else
		udelay(100);
		#endif
	}else {
		error("unknown phy type!\n");
		return -1;
	}

	return 0;
}

#endif
/*
 * TX and RX descriptors are 16 bytes. This causes problems with the cache
 * maintenance on CPUs where the cache-line size exceeds the size of these
 * descriptors. What will happen is that when the driver receives a packet
 * it will be immediately requeued for the hardware to reuse. The CPU will
 * therefore need to flush the cache-line containing the descriptor, which
 * will cause all other descriptors in the same cache-line to be flushed
 * along with it. If one of those descriptors had been written to by the
 * device those changes (and the associated packet) will be lost.
 *
 * To work around this, we make use of non-cached memory if available. If
 * descriptors are mapped uncached there's no need to manually flush them
 * or invalidate them.
 *
 * Note that this only applies to descriptors. The packet data buffers do
 * not have the same constraints since they are 1536 bytes large, so they
 * are unlikely to share cache-lines.
 */
static void *qos_alloc_descs(unsigned int num)
{
#ifdef CONFIG_SYS_NONCACHED_MEMORY
	return (void *)noncached_alloc(EQOS_DESCRIPTORS_SIZE,
				      EQOS_DESCRIPTOR_ALIGN);
#else
	return memalign(EQOS_DESCRIPTOR_ALIGN, EQOS_DESCRIPTORS_SIZE);
#endif
}

static void qos_free_descs(void *descs)
{
#ifdef CONFIG_SYS_NONCACHED_MEMORY
	/* FIXME: noncached_alloc() has no opposite */
#else
	free(descs);
#endif
}

static void qos_inval_desc(void *desc)
{
#if	0
	unsigned long start = (unsigned long)desc & ~(ARCH_DMA_MINALIGN - 1);
	unsigned long end = ALIGN(start + EQOS_DESCRIPTOR_SIZE,
				  ARCH_DMA_MINALIGN);

	invalidate_dcache_range(start, end);
#endif
}

static void qos_flush_desc(void *desc)
{
#if	0
	flush_cache((unsigned long)desc, EQOS_DESCRIPTOR_SIZE);
#endif
}

static void qos_inval_buffer(void *buf, size_t size)
{
#ifdef MMU_DCACHE_EN
	unsigned long start = (unsigned long)buf & ~(ARCH_DMA_MINALIGN - 1);
	unsigned long end = ALIGN(start + size, ARCH_DMA_MINALIGN);

	invalidate_dcache_range(start, end);
#endif
}

static void qos_flush_buffer(void *buf, size_t size)
{
#ifdef MMU_DCACHE_EN
	//flush_cache((unsigned long)buf, size);
	unsigned long start = (unsigned long)buf & ~(ARCH_DMA_MINALIGN - 1);
	unsigned long end = ALIGN(start + size, ARCH_DMA_MINALIGN);
	flush_cache((unsigned long)start, (end - start));
#endif
}

static int qos_mdio_wait_idle(struct qos_priv *eqos)
{
	return wait_for_bit(__func__, &eqos->mac_regs->mdio_address,
			    EQOS_MAC_MDIO_ADDRESS_GB, false, 1000000, true);
}

static int qos_mdio_read(struct mii_dev *bus, int mdio_addr, int mdio_devad,
			  int mdio_reg)
{
	struct qos_priv *eqos = bus->priv;
	u32 val;
	int ret;

	debug("%s(dev=%p, addr=%x, reg=%d):", __func__, eqos->dev, mdio_addr,
	      mdio_reg);

	ret = qos_mdio_wait_idle(eqos);
	if (ret) {
		error("MDIO not idle at entry");
		return ret;
	}

	val = readl(&eqos->mac_regs->mdio_address);
	val &= EQOS_MAC_MDIO_ADDRESS_SKAP |
		EQOS_MAC_MDIO_ADDRESS_C45E;
	val |= (mdio_addr << EQOS_MAC_MDIO_ADDRESS_PA_SHIFT) |
		(mdio_reg << EQOS_MAC_MDIO_ADDRESS_RDA_SHIFT) |
		(EQOS_MAC_MDIO_ADDRESS_CR_250_300 <<
		 EQOS_MAC_MDIO_ADDRESS_CR_SHIFT) |
		(EQOS_MAC_MDIO_ADDRESS_GOC_READ <<
		 EQOS_MAC_MDIO_ADDRESS_GOC_SHIFT) |
		EQOS_MAC_MDIO_ADDRESS_GB;
	writel(val, &eqos->mac_regs->mdio_address);

	ret = qos_mdio_wait_idle(eqos);
	if (ret) {
		error("MDIO read didn't complete");
		return ret;
	}

	val = readl(&eqos->mac_regs->mdio_data);
	val &= EQOS_MAC_MDIO_DATA_GD_MASK;

	debug("%s: val=%x\n", __func__, val);

	return val;
}

static int qos_mdio_write(struct mii_dev *bus, int mdio_addr, int mdio_devad,
			   int mdio_reg, u16 mdio_val)
{
	struct qos_priv *eqos = bus->priv;
	u32 val;
	int ret;

	debug("%s(dev=%p, addr=%x, reg=%d, val=%x):\n", __func__, eqos->dev,
	      mdio_addr, mdio_reg, mdio_val);
	ret = qos_mdio_wait_idle(eqos);
	if (ret) {
		error("MDIO not idle at entry");
		return ret;
	}

	writel(mdio_val, &eqos->mac_regs->mdio_data);
	writel(mdio_val, &eqos->mac_regs->mdio_data);

	val = readl(&eqos->mac_regs->mdio_address);
	val &= EQOS_MAC_MDIO_ADDRESS_SKAP |
		EQOS_MAC_MDIO_ADDRESS_C45E;
	val |= (mdio_addr << EQOS_MAC_MDIO_ADDRESS_PA_SHIFT) |
		(mdio_reg << EQOS_MAC_MDIO_ADDRESS_RDA_SHIFT) |
		(EQOS_MAC_MDIO_ADDRESS_CR_250_300 <<
		 EQOS_MAC_MDIO_ADDRESS_CR_SHIFT) |
		(EQOS_MAC_MDIO_ADDRESS_GOC_WRITE <<
		 EQOS_MAC_MDIO_ADDRESS_GOC_SHIFT) |
		EQOS_MAC_MDIO_ADDRESS_GB;
	writel(val, &eqos->mac_regs->mdio_address);

	ret = qos_mdio_wait_idle(eqos);
	if (ret) {
		error("MDIO read didn't complete");
		return ret;
	}

	return 0;
}

static int qos_set_full_duplex(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);
	mdelay(100);

	setbits_le32(&eqos->mac_regs->configuration, EQOS_MAC_CONFIGURATION_DM);

	return 0;
}

static int qos_set_half_duplex(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	clrbits_le32(&eqos->mac_regs->configuration, EQOS_MAC_CONFIGURATION_DM);

	/* WAR: Flush TX queue when switching to half-duplex */
	setbits_le32(&eqos->mtl_regs->txq0_operation_mode,
		     EQOS_MTL_TXQ0_OPERATION_MODE_FTQ);

	return 0;
}

static int qos_set_gmii_speed(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	clrbits_le32(&eqos->mac_regs->configuration,
		     EQOS_MAC_CONFIGURATION_PS | EQOS_MAC_CONFIGURATION_FES);

	return 0;
}

static int qos_set_mii_speed_100(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	setbits_le32(&eqos->mac_regs->configuration,
		     EQOS_MAC_CONFIGURATION_PS | EQOS_MAC_CONFIGURATION_FES);

	return 0;
}

static int qos_set_mii_speed_10(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	clrsetbits_le32(&eqos->mac_regs->configuration,
			EQOS_MAC_CONFIGURATION_FES, EQOS_MAC_CONFIGURATION_PS);

	return 0;
}

static int qos_adjust_link(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);
	int ret;

	debug("%s(dev=%p):\n", __func__, dev);

	if (eqos->phy->duplex)
		ret = qos_set_full_duplex(dev);
	else
		ret = qos_set_half_duplex(dev);
	if (ret < 0) {
		error("qos_set_*_duplex() failed: %d", ret);
		return ret;
	}

	switch (eqos->phy->speed) {
	case SPEED_1000:
		ret = qos_set_gmii_speed(dev);
		break;
	case SPEED_100:
		ret = qos_set_mii_speed_100(dev);
		break;
	case SPEED_10:
		ret = qos_set_mii_speed_10(dev);
		break;
	default:
		error("invalid speed %d", eqos->phy->speed);
		return -EINVAL;
	}
	if (ret < 0) {
		error("qos_set_*mii_speed*() failed: %d", ret);
		return ret;
	}

	return 0;
}

static int qos_write_hwaddr(struct udevice *dev)
{
	struct eth_pdata *plat = dev_get_platdata(dev);
	struct qos_priv *eqos = dev_get_priv(dev);
	uint32_t val;

	/*
	 * This function may be called before start() or after stop(). At that
	 * time, on at least some configurations of the EQoS HW, all clocks to
	 * the EQoS HW block will be stopped, and a reset signal applied. If
	 * any register access is attempted in this state, bus timeouts or CPU
	 * hangs may occur. This check prevents that.
	 *
	 * A simple solution to this problem would be to not implement
	 * write_hwaddr(), since start() always writes the MAC address into HW
	 * anyway. However, it is desirable to implement write_hwaddr() to
	 * support the case of SW that runs subsequent to U-Boot which expects
	 * the MAC address to already be programmed into the EQoS registers,
	 * which must happen irrespective of whether the U-Boot user (or
	 * scripts) actually made use of the EQoS device, and hence
	 * irrespective of whether start() was ever called.
	 *
	 * Note that this requirement by subsequent SW is not valid for
	 * Tegra186, and is likely not valid for any non-PCI instantiation of
	 * the EQoS HW block. This function is implemented solely as
	 * future-proofing with the expectation the driver will eventually be
	 * ported to some system where the expectation above is true.
	 */
	if (!eqos->config->reg_access_always_ok && !eqos->reg_access_ok)
		return 0;

	/* Update the MAC address */
	val = (plat->enetaddr[5] << 8) |
		(plat->enetaddr[4]);
	writel(val, &eqos->mac_regs->address0_high);
	val = (plat->enetaddr[3] << 24) |
		(plat->enetaddr[2] << 16) |
		(plat->enetaddr[1] << 8) |
		(plat->enetaddr[0]);
	writel(val, &eqos->mac_regs->address0_low);

	return 0;
}

static int qos_start(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);
	int ret, i;
	u32 val, tx_fifo_sz, rx_fifo_sz, tqs, rqs, pbl;
	ulong last_rx_desc;
	char *s;

	s = env_get("gmac_loopback");

	debug("%s(dev=%p):\n", __func__, dev);
	eqos->tx_desc_idx = 0;
	eqos->rx_desc_idx = 0;

	udelay(10);

	eqos->reg_access_ok = true;

	setbits_le32(&eqos->dma_regs->mode,
		     EQOS_DMA_MODE_SWR);

	ret = wait_for_bit(__func__, &eqos->dma_regs->mode,
			   EQOS_DMA_MODE_SWR, false, 10, false);
	if (ret) {
		error("EQOS_DMA_MODE_SWR stuck");
		goto err;
	}

	if((PHY_INTERFACE_MODE_MII == eqos->phy_interface) ||
		(PHY_INTERFACE_MODE_RMII == eqos->phy_interface)) {
		debug("skip ephy reset\n");
	}

	if (eqos->ephy_test_pattern == 1) {
		eqos->phy = phy_connect(eqos->mii, eqos->phy_addr, dev, eqos->phy_interface);
		if (!eqos->phy) {
			error("phy_connect() failed");
			goto err;
		}


		eqos->phy->autoneg = eqos->autoneg;
		eqos->phy->duplex = eqos->duplex;
		eqos->phy->speed = eqos->speed;
		printf("ephy autoneg:%x--duplex:%x--speed:%x\n",eqos->phy->autoneg,eqos->phy->duplex,eqos->phy->speed);

		ret = phy_config(eqos->phy);
		if (ret < 0) {
			error("phy_config() failed: %d", ret);
			//goto err_shutdown_phy;
		}
		ret = phy_startup(eqos->phy);
		if (ret < 0) {
			error("phy_startup() failed: %d", ret);
			//goto err_shutdown_phy;
		}

		if (!eqos->phy->link) {
			error("No link");
			//goto err_shutdown_phy;
		}
	
	} else {

		eqos->phy = phy_connect(eqos->mii, eqos->phy_addr, dev, eqos->phy_interface);
		if (!eqos->phy) {
			error("phy_connect() failed");
			goto err;
		}
		ret = phy_config(eqos->phy);
		if (ret < 0) {
			error("phy_config() failed: %d", ret);
			goto err_shutdown_phy;
		}

		if (s == NULL) {
			ret = phy_startup(eqos->phy);
			if (ret < 0) {
				error("phy_startup() failed: %d", ret);
				goto err_shutdown_phy;
			}

			if (!eqos->phy->link) {
				error("No link");
				goto err_shutdown_phy;
			}
		}

	}

	if (s == NULL) {
		ret = qos_adjust_link(dev);
		if (ret < 0) {
			error("qos_adjust_link() failed: %d", ret);
			goto err_shutdown_phy;
		}
	} else {
	
		qos_set_full_duplex(dev);
		qos_set_gmii_speed(dev);
	}

	/* Configure MTL */

	/* Enable Store and Forward mode for TX */
	/* Program Tx operating mode */
	setbits_le32(&eqos->mtl_regs->txq0_operation_mode,
		     EQOS_MTL_TXQ0_OPERATION_MODE_TSF |
		     (EQOS_MTL_TXQ0_OPERATION_MODE_TXQEN_ENABLED <<
		      EQOS_MTL_TXQ0_OPERATION_MODE_TXQEN_SHIFT));

	/* Transmit Queue weight */
	writel(0x10, &eqos->mtl_regs->txq0_quantum_weight);

	/* Enable Store and Forward mode for RX, since no jumbo frame */
	setbits_le32(&eqos->mtl_regs->rxq0_operation_mode,
		     EQOS_MTL_RXQ0_OPERATION_MODE_RSF);

	/* Transmit/Receive queue fifo size; use all RAM for 1 queue */
	val = readl(&eqos->mac_regs->hw_feature1);
	tx_fifo_sz = (val >> EQOS_MAC_HW_FEATURE1_TXFIFOSIZE_SHIFT) &
		EQOS_MAC_HW_FEATURE1_TXFIFOSIZE_MASK;
	rx_fifo_sz = (val >> EQOS_MAC_HW_FEATURE1_RXFIFOSIZE_SHIFT) &
		EQOS_MAC_HW_FEATURE1_RXFIFOSIZE_MASK;

	/*
	 * r/tx_fifo_sz is encoded as log2(n / 128). Undo that by shifting.
	 * r/tqs is encoded as (n / 256) - 1.
	 */
	tqs = (128 << tx_fifo_sz) / 256 - 1;
	rqs = (128 << rx_fifo_sz) / 256 - 1;

	clrsetbits_le32(&eqos->mtl_regs->txq0_operation_mode,
			EQOS_MTL_TXQ0_OPERATION_MODE_TQS_MASK <<
			EQOS_MTL_TXQ0_OPERATION_MODE_TQS_SHIFT,
			tqs << EQOS_MTL_TXQ0_OPERATION_MODE_TQS_SHIFT);
	clrsetbits_le32(&eqos->mtl_regs->rxq0_operation_mode,
			EQOS_MTL_RXQ0_OPERATION_MODE_RQS_MASK <<
			EQOS_MTL_RXQ0_OPERATION_MODE_RQS_SHIFT,
			rqs << EQOS_MTL_RXQ0_OPERATION_MODE_RQS_SHIFT);

	/* Flow control used only if each channel gets 4KB or more FIFO */
	if (rqs >= ((4096 / 256) - 1)) {
		u32 rfd, rfa;

		setbits_le32(&eqos->mtl_regs->rxq0_operation_mode,
			     EQOS_MTL_RXQ0_OPERATION_MODE_EHFC);

		/*
		 * Set Threshold for Activating Flow Contol space for min 2
		 * frames ie, (1500 * 1) = 1500 bytes.
		 *
		 * Set Threshold for Deactivating Flow Contol for space of
		 * min 1 frame (frame size 1500bytes) in receive fifo
		 */
		if (rqs == ((4096 / 256) - 1)) {
			/*
			 * This violates the above formula because of FIFO size
			 * limit therefore overflow may occur inspite of this.
			 */
			rfd = 0x3;	/* Full-3K */
			rfa = 0x1;	/* Full-1.5K */
		} else if (rqs == ((8192 / 256) - 1)) {
			rfd = 0x6;	/* Full-4K */
			rfa = 0xa;	/* Full-6K */
		} else if (rqs == ((16384 / 256) - 1)) {
			rfd = 0x6;	/* Full-4K */
			rfa = 0x12;	/* Full-10K */
		} else {
			rfd = 0x6;	/* Full-4K */
			rfa = 0x1E;	/* Full-16K */
		}

		clrsetbits_le32(&eqos->mtl_regs->rxq0_operation_mode,
				(EQOS_MTL_RXQ0_OPERATION_MODE_RFD_MASK <<
				 EQOS_MTL_RXQ0_OPERATION_MODE_RFD_SHIFT) |
				(EQOS_MTL_RXQ0_OPERATION_MODE_RFA_MASK <<
				 EQOS_MTL_RXQ0_OPERATION_MODE_RFA_SHIFT),
				(rfd <<
				 EQOS_MTL_RXQ0_OPERATION_MODE_RFD_SHIFT) |
				(rfa <<
				 EQOS_MTL_RXQ0_OPERATION_MODE_RFA_SHIFT));
	}

	/* Configure MAC */

	clrsetbits_le32(&eqos->mac_regs->rxq_ctrl0,
			EQOS_MAC_RXQ_CTRL0_RXQ0EN_MASK <<
			EQOS_MAC_RXQ_CTRL0_RXQ0EN_SHIFT,
			EQOS_MAC_RXQ_CTRL0_RXQ0EN_ENABLED_DCB <<
			EQOS_MAC_RXQ_CTRL0_RXQ0EN_SHIFT);

	/* Set TX flow control parameters */
	/* Set Pause Time */
	setbits_le32(&eqos->mac_regs->q0_tx_flow_ctrl,
		     0xffff << EQOS_MAC_Q0_TX_FLOW_CTRL_PT_SHIFT);
	/* Assign priority for TX flow control */
	clrbits_le32(&eqos->mac_regs->txq_prty_map0,
		     EQOS_MAC_TXQ_PRTY_MAP0_PSTQ0_MASK <<
		     EQOS_MAC_TXQ_PRTY_MAP0_PSTQ0_SHIFT);
	/* Assign priority for RX flow control */
	clrbits_le32(&eqos->mac_regs->rxq_ctrl2,
		     EQOS_MAC_RXQ_CTRL2_PSRQ0_MASK <<
		     EQOS_MAC_RXQ_CTRL2_PSRQ0_SHIFT);
	/* Enable flow control */
	setbits_le32(&eqos->mac_regs->q0_tx_flow_ctrl,
		     EQOS_MAC_Q0_TX_FLOW_CTRL_TFE);
	setbits_le32(&eqos->mac_regs->rx_flow_ctrl,
		     EQOS_MAC_RX_FLOW_CTRL_RFE);

	clrsetbits_le32(&eqos->mac_regs->configuration,
			EQOS_MAC_CONFIGURATION_GPSLCE |
			EQOS_MAC_CONFIGURATION_WD |
			EQOS_MAC_CONFIGURATION_JD |
			EQOS_MAC_CONFIGURATION_JE,0);

	qos_write_hwaddr(dev);

	/* Configure DMA */

	/* Enable OSP mode */
	setbits_le32(&eqos->dma_regs->ch0_tx_control,
		     EQOS_DMA_CH0_TX_CONTROL_OSP);

	/* RX buffer size. Must be a multiple of bus width */
	clrsetbits_le32(&eqos->dma_regs->ch0_rx_control,
			EQOS_DMA_CH0_RX_CONTROL_RBSZ_MASK <<
			EQOS_DMA_CH0_RX_CONTROL_RBSZ_SHIFT,
			EQOS_MAX_PACKET_SIZE <<
			EQOS_DMA_CH0_RX_CONTROL_RBSZ_SHIFT);

	setbits_le32(&eqos->dma_regs->ch0_control,
		     EQOS_DMA_CH0_CONTROL_PBLX8);

	/*
	 * Burst length must be < 1/2 FIFO size.
	 * FIFO size in tqs is encoded as (n / 256) - 1.
	 * Each burst is n * 8 (PBLX8) * 16 (AXI width) == 128 bytes.
	 * Half of n * 256 is n * 128, so pbl == tqs, modulo the -1.
	 */
	pbl = tqs + 1;
	if (pbl > 32)
		pbl = 32;
	clrsetbits_le32(&eqos->dma_regs->ch0_tx_control,
			EQOS_DMA_CH0_TX_CONTROL_TXPBL_MASK <<
			EQOS_DMA_CH0_TX_CONTROL_TXPBL_SHIFT,
			pbl << EQOS_DMA_CH0_TX_CONTROL_TXPBL_SHIFT);

	clrsetbits_le32(&eqos->dma_regs->ch0_rx_control,
			EQOS_DMA_CH0_RX_CONTROL_RXPBL_MASK <<
			EQOS_DMA_CH0_RX_CONTROL_RXPBL_SHIFT,
			8 << EQOS_DMA_CH0_RX_CONTROL_RXPBL_SHIFT);

	/* DMA performance configuration */
	val = (2 << EQOS_DMA_SYSBUS_MODE_RD_OSR_LMT_SHIFT) |
		EQOS_DMA_SYSBUS_MODE_EAME | EQOS_DMA_SYSBUS_MODE_BLEN16 |
		EQOS_DMA_SYSBUS_MODE_BLEN8 | EQOS_DMA_SYSBUS_MODE_BLEN4;
	writel(val, &eqos->dma_regs->sysbus_mode);

	/* Set up descriptors */

	memset(eqos->descs, 0, EQOS_DESCRIPTORS_SIZE);
	for (i = 0; i < EQOS_DESCRIPTORS_RX; i++) {
		struct qos_desc *rx_desc = &(eqos->rx_descs[i]);
		rx_desc->des0 = (u32)(ulong)(eqos->rx_dma_buf +
					     (i * EQOS_MAX_PACKET_SIZE));
		mb();
		rx_desc->des3 |= EQOS_DESC3_OWN | EQOS_DESC3_BUF1V;
	}
	flush_cache((unsigned long)eqos->descs, EQOS_DESCRIPTORS_SIZE);

	writel(0, &eqos->dma_regs->ch0_txdesc_list_haddress);
	writel((ulong)eqos->tx_descs, &eqos->dma_regs->ch0_txdesc_list_address);
	writel(EQOS_DESCRIPTORS_TX - 1,
	       &eqos->dma_regs->ch0_txdesc_ring_length);

	writel(0, &eqos->dma_regs->ch0_rxdesc_list_haddress);
	writel((ulong)eqos->rx_descs, &eqos->dma_regs->ch0_rxdesc_list_address);
	writel(EQOS_DESCRIPTORS_RX - 1,
	       &eqos->dma_regs->ch0_rxdesc_ring_length);

	/* Enable everything */

	setbits_le32(&eqos->mac_regs->configuration,
		     EQOS_MAC_CONFIGURATION_TE | EQOS_MAC_CONFIGURATION_RE);

	setbits_le32(&eqos->dma_regs->ch0_tx_control,
		     EQOS_DMA_CH0_TX_CONTROL_ST);
	setbits_le32(&eqos->dma_regs->ch0_rx_control,
		     EQOS_DMA_CH0_RX_CONTROL_SR);

	/* TX tail pointer not written until we need to TX a packet */
	/*
	 * Point RX tail pointer at last descriptor. Ideally, we'd point at the
	 * first descriptor, implying all descriptors were available. However,
	 * that's not distinguishable from none of the descriptors being
	 * available.
	 */
	last_rx_desc = (ulong)&(eqos->rx_descs[(EQOS_DESCRIPTORS_RX - 1)]);
	writel(last_rx_desc, &eqos->dma_regs->ch0_rxdesc_tail_pointer);

	eqos->started = true;



	debug("%s: OK\n", __func__);
	return 0;

err_shutdown_phy:
	phy_shutdown(eqos->phy);
	eqos->phy = NULL;
err:
	error("FAILED: %d", ret);
	return ret;
}

void qos_stop(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);
	int i;

	debug("%s(dev=%p):\n", __func__, dev);

	if (!eqos->started)
		return;
	eqos->started = false;
	eqos->reg_access_ok = false;

	/* Disable TX DMA */
	clrbits_le32(&eqos->dma_regs->ch0_tx_control,
		     EQOS_DMA_CH0_TX_CONTROL_ST);

	/* Wait for TX all packets to drain out of MTL */
	for (i = 0; i < 1000000; i++) {
		u32 val = readl(&eqos->mtl_regs->txq0_debug);
		u32 trcsts = (val >> EQOS_MTL_TXQ0_DEBUG_TRCSTS_SHIFT) &
			EQOS_MTL_TXQ0_DEBUG_TRCSTS_MASK;
		u32 txqsts = val & EQOS_MTL_TXQ0_DEBUG_TXQSTS;
		if ((trcsts != 1) && (!txqsts))
			break;
	}

	/* Turn off MAC TX and RX */
	clrbits_le32(&eqos->mac_regs->configuration,
		     EQOS_MAC_CONFIGURATION_TE | EQOS_MAC_CONFIGURATION_RE);

	/* Wait for all RX packets to drain out of MTL */
	for (i = 0; i < 1000000; i++) {
		u32 val = readl(&eqos->mtl_regs->rxq0_debug);
		u32 prxq = (val >> EQOS_MTL_RXQ0_DEBUG_PRXQ_SHIFT) &
			EQOS_MTL_RXQ0_DEBUG_PRXQ_MASK;
		u32 rxqsts = (val >> EQOS_MTL_RXQ0_DEBUG_RXQSTS_SHIFT) &
			EQOS_MTL_RXQ0_DEBUG_RXQSTS_MASK;
		if ((!prxq) && (!rxqsts))
			break;
	}

	/* Turn off RX DMA */
	clrbits_le32(&eqos->dma_regs->ch0_rx_control,
		     EQOS_DMA_CH0_RX_CONTROL_SR);

	if (eqos->phy) {
		phy_shutdown(eqos->phy);
		eqos->phy = NULL;
	}

	debug("%s: OK\n", __func__);
}

int qos_send(struct udevice *dev, void *packet, int length)
{
	struct qos_priv *eqos = dev_get_priv(dev);
	struct qos_desc *tx_desc;
	int i;

	debug("%s(dev=%p, packet=%p, length=%d):\n", __func__, dev, packet,
	      length);

	memcpy(eqos->tx_dma_buf, packet, length);
	qos_flush_buffer(eqos->tx_dma_buf, length);

	tx_desc = &(eqos->tx_descs[eqos->tx_desc_idx]);
	eqos->tx_desc_idx++;
	eqos->tx_desc_idx %= EQOS_DESCRIPTORS_TX;

	tx_desc->des0 = (ulong)eqos->tx_dma_buf;
	tx_desc->des1 = 0;
	tx_desc->des2 = length;
	/*
	 * Make sure that if HW sees the _OWN write below, it will see all the
	 * writes to the rest of the descriptor too.
	 */
	mb();
	tx_desc->des3 = EQOS_DESC3_OWN | EQOS_DESC3_FD | EQOS_DESC3_LD | length;
	qos_flush_desc(tx_desc);

	writel((ulong)(tx_desc + 1), &eqos->dma_regs->ch0_txdesc_tail_pointer);

	for (i = 0; i < 1000000; i++) {
		qos_inval_desc(tx_desc);
		if (!(readl(&tx_desc->des3) & EQOS_DESC3_OWN))
			return 0;
		udelay(1);
	}

	debug("%s: TX timeout\n", __func__);

	return -ETIMEDOUT;
}

int qos_recv(struct udevice *dev, int flags, uchar **packetp)
{
	struct qos_priv *eqos = dev_get_priv(dev);
	struct qos_desc *rx_desc;
	int length;

	//debug("%s(dev=%p, flags=%x):\n", __func__, dev, flags);

	rx_desc = &(eqos->rx_descs[eqos->rx_desc_idx]);
	if (rx_desc->des3 & EQOS_DESC3_OWN) {
		//debug("%s: RX packet not available\n", __func__);
		return -EAGAIN;
	}

	*packetp = eqos->rx_dma_buf +
		(eqos->rx_desc_idx * EQOS_MAX_PACKET_SIZE);
	length = rx_desc->des3 & 0x7fff;
	debug("%s: *packetp=%p, length=%d\n", __func__, *packetp, length);

	mb();
	qos_inval_buffer(*packetp, length);

	return length;
}

int qos_free_pkt(struct udevice *dev, uchar *packet, int length)
{
	struct qos_priv *eqos = dev_get_priv(dev);
	uchar *packet_expected;
	struct qos_desc *rx_desc;

	debug("%s(packet=%p, length=%d)\n", __func__, packet, length);

	packet_expected = eqos->rx_dma_buf +
		(eqos->rx_desc_idx * EQOS_MAX_PACKET_SIZE);
	if (packet != packet_expected) {
		debug("%s: Unexpected packet (expected %p)\n", __func__,
		      packet_expected);
		return -EINVAL;
	}

	rx_desc = &(eqos->rx_descs[eqos->rx_desc_idx]);
	rx_desc->des0 = (u32)(ulong)packet;
	rx_desc->des1 = 0;
	rx_desc->des2 = 0;
	/*
	 * Make sure that if HW sees the _OWN write below, it will see all the
	 * writes to the rest of the descriptor too.
	 */
	mb();
	rx_desc->des3 |= EQOS_DESC3_OWN | EQOS_DESC3_BUF1V;
	qos_flush_desc(rx_desc);

	writel((ulong)rx_desc, &eqos->dma_regs->ch0_rxdesc_tail_pointer);

	eqos->rx_desc_idx++;
	eqos->rx_desc_idx %= EQOS_DESCRIPTORS_RX;

	return 0;
}

static int qos_probe_resources_core(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);
	int ret;

	debug("%s(dev=%p):\n", __func__, dev);

	eqos->descs = qos_alloc_descs(EQOS_DESCRIPTORS_TX +
				       EQOS_DESCRIPTORS_RX);
	if (!eqos->descs) {
		debug("%s: qos_alloc_descs() failed\n", __func__);
		ret = -ENOMEM;
		goto err;
	}
	eqos->tx_descs = (struct qos_desc *)eqos->descs;
	eqos->rx_descs = (eqos->tx_descs + EQOS_DESCRIPTORS_TX);
	debug("%s: tx_descs=%p, rx_descs=%p\n", __func__, eqos->tx_descs,
	      eqos->rx_descs);

	eqos->tx_dma_buf = memalign(EQOS_BUFFER_ALIGN, EQOS_MAX_PACKET_SIZE);
	if (!eqos->tx_dma_buf) {
		debug("%s: memalign(tx_dma_buf) failed\n", __func__);
		ret = -ENOMEM;
		goto err_free_descs;
	}
	debug("%s: tx_dma_buf=%p\n", __func__, eqos->tx_dma_buf);

	eqos->rx_dma_buf = memalign(EQOS_BUFFER_ALIGN, EQOS_RX_BUFFER_SIZE);
	if (!eqos->rx_dma_buf) {
		debug("%s: memalign(rx_dma_buf) failed\n", __func__);
		ret = -ENOMEM;
		goto err_free_tx_dma_buf;
	}
	debug("%s: rx_dma_buf=%p\n", __func__, eqos->rx_dma_buf);

	eqos->rx_pkt = malloc(EQOS_MAX_PACKET_SIZE);
	if (!eqos->rx_pkt) {
		debug("%s: malloc(rx_pkt) failed\n", __func__);
		ret = -ENOMEM;
		goto err_free_rx_dma_buf;
	}
	debug("%s: rx_pkt=%p\n", __func__, eqos->rx_pkt);

	debug("%s: OK\n", __func__);
	mdelay(100);
	return 0;

err_free_rx_dma_buf:
	free(eqos->rx_dma_buf);
err_free_tx_dma_buf:
	free(eqos->tx_dma_buf);
err_free_descs:
	qos_free_descs(eqos->descs);
err:

	printf("%s: returns %d\n", __func__, ret);
	return ret;
}

static int qos_remove_resources_core(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	free(eqos->rx_pkt);
	free(eqos->rx_dma_buf);
	free(eqos->tx_dma_buf);
	qos_free_descs(eqos->descs);

	debug("%s: OK\n", __func__);
	return 0;
}

int change_loopback(struct udevice *dev, int switchon)
{
	int i;
	u16 reg;
	struct qos_priv *eqos;
	if (dev != NULL) {
		eqos = dev_get_priv(dev);
		if ((eqos == NULL) || (eqos->phy == NULL)) {
			printf("%s %d\n",__func__, __LINE__);	
			return -3;
		}
		//phy loopback
		if (eqos->phy != NULL) {
			phy_write(eqos->phy, MDIO_DEVAD_NONE, 0x1f, 0x0);
			//reg = (BIT(15) | BIT(14) | phy_read(eqos->phy, MDIO_DEVAD_NONE, 0x0));
			//phy_write(eqos->phy, MDIO_DEVAD_NONE, 0x0, reg);
			if (switchon) {
				reg = (BIT(14) | phy_read(eqos->phy, MDIO_DEVAD_NONE, 0x0));
				printf("open %s phy loop\n",dev->name);
			} else {
				reg = (~ BIT(14) & phy_read(eqos->phy, MDIO_DEVAD_NONE, 0x0));
				printf("close %s phy loop\n",dev->name);
			}

			phy_write(eqos->phy, MDIO_DEVAD_NONE, 0x0, reg);

			if (switchon) {

				printf("wait loopback up ...\n");
				for (i = 0; i < 3000; i++) {
					udelay(1000);
					if (phy_read(eqos->phy, MDIO_DEVAD_NONE, 0x1) & BIT(2))
						break;
				}
				if (i >= 3000) {
					printf("wait phy loopback up failed\n");
					return -3;
				}
			}

			printf("phy reg 0x0:%x\n",phy_read(eqos->phy, MDIO_DEVAD_NONE, 0x0));
			printf("phy reg 0x1:%x\n",phy_read(eqos->phy, MDIO_DEVAD_NONE, 0x1));
			printf("phy reg 0x1:%x\n",phy_read(eqos->phy, MDIO_DEVAD_NONE, 0x1));
			printf("phy reg 0x1:%x\n",phy_read(eqos->phy, MDIO_DEVAD_NONE, 0x1));
		}
	}

	//mac loopback
	//writel(readl(0x30900000) | (1 << 12), 0x30900000);
	//
	//promisc mode
	//writel(readl(0x30900008) | BIT(0), 0x30900000);

	return 0;
}

extern int gmac_loopback_autoscan(struct udevice *dev, int *tx_delay, int *rx_delay);
extern int env_save(void);

extern int auto_find_phy(void);
static int qos_probe(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);
	int ret;
	const char *phy_mode;
	char *s1;
	char *s2;
	char temp[50];
	debug("%s(dev=%p):\n", __func__, dev);
	eqos->dev = dev;
	eqos->config = (void *)dev_get_driver_data(dev);

	eqos->regs = devfdt_get_addr(dev);
	if (eqos->regs == FDT_ADDR_T_NONE) {
		error("devfdt_get_addr() failed");
		return -ENODEV;
	}
	eqos->phy_interface = -1;
	phy_mode = fdt_getprop(gd->fdt_blob, dev_of_offset(dev), "phy-mode",
			       NULL);
	if (phy_mode) {
		eqos->phy_interface = phy_get_interface_by_name(phy_mode);
		if (eqos->phy_interface == -1) {
			debug("%s: Invalid PHY interface '%s'\n", __func__, phy_mode);
			return -EINVAL;
		}
	} else {
		debug("%s: Get Invalid PHY interface from dts\n", __func__);
		return -EINVAL;
	}

	eqos->ctrl_id = fdtdec_get_uint(gd->fdt_blob, dev_of_offset(dev), "ctrl_id", -2);
	if (eqos->ctrl_id < -1) {
		debug("%s: Get Invalid ctrl_id from dts\n", __func__);
		//return -EINVAL;
	}
	eqos->phy_type = fdtdec_get_uint(gd->fdt_blob, dev_of_offset(dev), "phy-type", -1);
	if (eqos->phy_type < 0) {
		debug("%s: Get Invalid PHY type from dts\n", __func__);
		return -EINVAL;
	}

#if defined(CONFIG_GMAC_MOLCHIP_FDT)

	eqos->gmac_cap.has_txrx_delay = fdtdec_get_bool(gd->fdt_blob, dev_of_offset(dev), "has_txrx_delay");
	eqos->gmac_cap.dpll_version = fdtdec_get_uint(gd->fdt_blob, dev_of_offset(dev), "dpll_version", -2);
	if (eqos->ctrl_id < -1) {
		eqos->gmac_cap.dpll_version = 1;
	}
	
	if (eqos->gmac_cap.has_txrx_delay) {
		eqos->dpll_addr_map.txdpll_base = devfdt_get_addr_index(dev, 1);
		if (eqos->dpll_addr_map.txdpll_base == FDT_ADDR_T_NONE) {
			error("devfdt_get_addr() txdpll_base failed");
			//return -ENODEV;
		}
		eqos->dpll_addr_map.rxdpll_base = devfdt_get_addr_index(dev, 2);
		if (eqos->dpll_addr_map.rxdpll_base == FDT_ADDR_T_NONE) {
			error("devfdt_get_addr() rxdpll_base failed");
			//return -ENODEV;
		}
		if(eqos->gmac_cap.dpll_version == 1) {
			eqos->dpll_addr_map.eco0_base = devfdt_get_addr_index(dev, 3);
			if (eqos->dpll_addr_map.eco0_base == FDT_ADDR_T_NONE) {
				error("devfdt_get_addr() eco0_base failed");
				//return -ENODEV;
			}

			eqos->dpll_addr_map.eco1_base = devfdt_get_addr_index(dev, 4);
			if (eqos->dpll_addr_map.eco1_base == FDT_ADDR_T_NONE) {
				error("devfdt_get_addr() eco1_base failed");
				//return -ENODEV;
			}
			
			eqos->dpll_addr_map.dll_addr_is_fixup = fdtdec_get_bool(gd->fdt_blob, dev_of_offset(dev), "dll_addr_is_fixup");

		} else {
			eqos->dpll_addr_map.negrxdpll_base = devfdt_get_addr_index(dev, 3);
			if (eqos->dpll_addr_map.negrxdpll_base == FDT_ADDR_T_NONE) {
				error("devfdt_get_addr() negrxdpll_base failed");
				//return -ENODEV;
			}
		}
		eqos->tx_delay = fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev), "tx_delay", -2);
		if (eqos->tx_delay < -1) {
			debug("%s: Get Invalid tx_delay from dts\n", __func__);
			//return -EINVAL;
		}

		eqos->rx_delay = fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev), "rx_delay", -2);
		if (eqos->tx_delay < -1) {
			debug("%s: Get Invalid rx_delay from dts\n", __func__);
			//return -EINVAL;
		}
		eqos->negrx_delay = fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev), "negrx_delay", -2);
		if (eqos->tx_delay < -1) {
			debug("%s: Get Invalid negrx_delay from dts\n", __func__);
			//return -EINVAL;
		}

	}

	eqos->apahb_base = fdtdec_get_uint(gd->fdt_blob, dev_of_offset(dev), "molchip,syscon-ap-ahb", -1);
	eqos->apapb_base = fdtdec_get_uint(gd->fdt_blob, dev_of_offset(dev), "molchip,syscon-ap-apb", -1);
	eqos->glapb_base = fdtdec_get_uint(gd->fdt_blob, dev_of_offset(dev), "molchip,syscon-gl-apb", -1);

	eqos->phy_type = fdtdec_get_uint(gd->fdt_blob, dev_of_offset(dev), "phy-type", -1);
	eqos->platform = fdtdec_get_uint(gd->fdt_blob, dev_of_offset(dev), "platform", -1);
	//phys_addr_t temp2 = ((sizeof(phys_addr_t) > 4) ? 32:0);
	u32 temp2 = ((sizeof(phys_addr_t) > 4) ? 32:0);
	printf(" temp2:%d %d\n",temp2,sizeof(phys_addr_t));
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,gmac-axi-en",(u32*)&eqos->gmac_axi_en,6);
	eqos->gmac_axi_en.reg_base.base = eqos->gmac_axi_en.reg_base.s.base_low | ((phys_addr_t)(eqos->gmac_axi_en.reg_base.s.base_high) << temp2);
	printf(" regbase:%lx--offset:%x--bitof%x\n",eqos->gmac_axi_en.reg_base.base,eqos->gmac_axi_en.reg_offset,eqos->gmac_axi_en.bit_offset);
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,gmac-csr-en",(u32*)&eqos->gmac_csr_en,6);
	eqos->gmac_csr_en.reg_base.base = eqos->gmac_csr_en.reg_base.s.base_low | ((phys_addr_t)(eqos->gmac_csr_en.reg_base.s.base_high) << temp2);
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,ephy-type-sel",(u32*)&eqos->phy_type_sel,6);
	eqos->phy_type_sel.reg_base.base = eqos->phy_type_sel.reg_base.s.base_low | ((phys_addr_t)(eqos->phy_type_sel.reg_base.s.base_high) << temp2);
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,rmii-pad-sel",(u32*)&eqos->rmii_pad_sel,6);
	eqos->rmii_pad_sel.reg_base.base = eqos->rmii_pad_sel.reg_base.s.base_low | ((phys_addr_t)(eqos->rmii_pad_sel.reg_base.s.base_high) << temp2);
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,phy-intf-sel",(u32*)&eqos->phy_intf_sel,6);
	eqos->phy_intf_sel.reg_base.base = eqos->phy_intf_sel.reg_base.s.base_low | ((phys_addr_t)(eqos->phy_intf_sel.reg_base.s.base_high) << temp2);
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,ephy-ref-en",(u32*)&eqos->ref_en_cfg,6);
	eqos->ref_en_cfg.reg_base.base = eqos->ref_en_cfg.reg_base.s.base_low | ((phys_addr_t)(eqos->ref_en_cfg.reg_base.s.base_high) << temp2);
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,ephy-soc-sel",(u32*)&eqos->ephy_soc_sel,5);
	eqos->ephy_soc_sel.reg_base.base = eqos->ephy_soc_sel.reg_base.s.base_low | ((phys_addr_t)(eqos->ephy_soc_sel.reg_base.s.base_high) << temp2);
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,ephy-soft-reset",(u32*)&eqos->soft_reset_cfg,5);
	eqos->soft_reset_cfg.reg_base.base = eqos->soft_reset_cfg.reg_base.s.base_low | ((phys_addr_t)(eqos->soft_reset_cfg.reg_base.s.base_high) << temp2);
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,tx-en",(u32*)&eqos->tx_en,6);
	eqos->tx_en.reg_base.base = eqos->tx_en.reg_base.s.base_low | ((phys_addr_t)(eqos->tx_en.reg_base.s.base_high) << temp2);
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,rx-en",(u32*)&eqos->rx_en,6);
	eqos->rx_en.reg_base.base = eqos->rx_en.reg_base.s.base_low | ((phys_addr_t)(eqos->rx_en.reg_base.s.base_high) << temp2);
	fdtdec_get_int_array(gd->fdt_blob, dev_of_offset(dev),"snps,rmii-en",(u32*)&eqos->rmii_en,6);
	eqos->rmii_en.reg_base.base = eqos->rmii_en.reg_base.s.base_low | ((phys_addr_t)(eqos->rmii_en.reg_base.s.base_high) << temp2);

	ret = qos_lowlevel_init(eqos);
	if (ret < 0) {
		error("%s: Fail!\n", __func__);
		return -EINVAL;
	}
#else
	ret = qos_lowlevel_init(eqos);
	if (ret < 0) {
		error("%s: Fail!\n", __func__);
		return -EINVAL;
	}
#endif
	eqos->phy_addr = fdtdec_get_uint(gd->fdt_blob, dev_of_offset(dev), "phy-addr", -1);
	if (eqos->phy_addr < 0) {
		debug("%s: Get Invalid PHY address from dts\n", __func__);
		return -EINVAL;
	}

	eqos->mac_regs = (void *)(eqos->regs + EQOS_MAC_REGS_BASE);
	eqos->mtl_regs = (void *)(eqos->regs + EQOS_MTL_REGS_BASE);
	eqos->dma_regs = (void *)(eqos->regs + EQOS_DMA_REGS_BASE);

	ret = qos_probe_resources_core(dev);
	if (ret < 0) {
		error("qos_probe_resources_core() failed: %d", ret);
		return ret;
	}

	eqos->mii = mdio_alloc();
	if (!eqos->mii) {
		error("mdio_alloc() failed");
		goto err_remove_resources_core;
	}
	eqos->mii->read = qos_mdio_read;
	eqos->mii->write = qos_mdio_write;
	eqos->mii->priv = eqos;
	strcpy(eqos->mii->name, dev->name);

	ret = mdio_register(eqos->mii);
	if (ret < 0) {
		error("mdio_register() failed: %d", ret);
		goto err_free_mdio;
	}

	record_eths[eqos->ctrl_id].index = eqos->ctrl_id;
	record_eths[eqos->ctrl_id].dev = dev;

	if (eqos->phy_type == INTERNAL_PHY)
		auto_find_phy();
	debug("%s: OK\n", __func__);

	if (eqos->gmac_cap.has_txrx_delay) {
		if ( (eqos->tx_delay < -1) || (eqos->rx_delay < -1) ) {
			ret = scan_dpll_table(dev);
			if (ret < 0) {
				printf("cannot find scan dpll table entry:%d\n",ret);
				sprintf(temp, "%s_txdelay", dev->name);
				s1 = env_get(temp);
				if (s1 != NULL) {
					eqos->tx_delay = simple_strtoul(s1, NULL, 16);
				}
				sprintf(temp, "%s_rxdelay", dev->name);
				s2 = env_get(temp);
				if (s2 != NULL) {
					eqos->rx_delay = simple_strtoul(s2, NULL, 16);
				}

				if (ret == -1) {
					goto err_auto_scan;
				}

				if ((s1 == NULL) || (s2 == NULL)) {

					char str[17];
					printf("auto scan in loopback mode\n");
					ret = gmac_loopback_autoscan(dev, &(eqos->tx_delay), &(eqos->rx_delay));
					if (ret < 0)
						goto err_auto_scan;
					sprintf(str, "0x%x", eqos->tx_delay);
					sprintf(temp, "%s_txdelay", dev->name);
					env_set(temp,str);
					sprintf(str, "0x%x", eqos->rx_delay);
					sprintf(temp, "%s_rxdelay", dev->name);
					env_set(temp,str);
					env_save();

				}
				printf("%s,tx_delay:0x%x,rx_delay:0x%x\n", dev->name, eqos->tx_delay, eqos->rx_delay);
			}
		}
		if(eqos->gmac_cap.dpll_version == 1)
			config_dpll(dev);
		else if(eqos->gmac_cap.dpll_version == 2)
			config_dpll_v2(dev);
		else
			printf("gmac error dpll version");

	}


	return 0;
	
err_auto_scan:
	printf("%s autoloop mode scan error!!! no gmac tx/rx delay config!!!\n",dev->name);
	return 0;
	
err_free_mdio:
	mdio_free(eqos->mii);
err_remove_resources_core:
	qos_remove_resources_core(dev);

	printf("%s: returns %d\n", __func__, ret);
	return ret;
}

static int qos_remove(struct udevice *dev)
{
	struct qos_priv *eqos = dev_get_priv(dev);

	debug("%s(dev=%p):\n", __func__, dev);

	mdio_unregister(eqos->mii);
	mdio_free(eqos->mii);
	qos_probe_resources_core(dev);

	debug("%s: OK\n", __func__);
	return 0;
}

static const struct eth_ops qos_ops = {
	.start = qos_start,
	.stop = qos_stop,
	.send = qos_send,
	.recv = qos_recv,
	.free_pkt = qos_free_pkt,
	.write_hwaddr = qos_write_hwaddr,
};

static const struct udevice_id qos_ids[] = {
	{
		.compatible = "molchip,dwc-qos-ethernet-4.10",
	},
	{ }
};

U_BOOT_DRIVER(eth_molchip_eqos) = {
	.name = "eth_molchip_eqos",
	.id = UCLASS_ETH,
	.of_match = qos_ids,
	.probe = qos_probe,
	.remove = qos_remove,
	.ops = &qos_ops,
	.priv_auto_alloc_size = sizeof(struct qos_priv),
	.platdata_auto_alloc_size = sizeof(struct eth_pdata),
	.flags = DM_FLAG_PRE_RELOC,
};
