/*
 * Copyright (C) 2015 Atmel Corporation
 *		      Wenyou.Yang <wenyou.yang@atmel.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <malloc.h>
#include <sdhci.h>
#include <asm/arch/clk.h>
#include <mach/hardware.h>


#define MC_SDHC_MIN_FREQ	400000

/**************************EMMC PHY INIT ********************************************/
#define EMMC_PHY_VERSION_ID                 0x00
#define EMMC_PHY_INT_EN                     0x04
#define EMMC_PHY_INT_SRC                    0x08
#define EMMC_PHY_CTRL                       0x0C
#define EMMC_PHY_TEST_O                     0x10
#define EMMC_PHY_TEST_I                     0x14
#define EMMC_PHY_IE_CTRL                    0x18
#define EMMC_PHY_DL_CTRL                    0x1C
#define EMMC_PHY_CTRL2                      0x24

#define BIT_EMMC_DAT_INPUT_EN                                     (0x1 << 31)
#define BIT_EMMC_OUTPUT_BIT_MODE(x)                               (((x) & 0x3) << 29)
#define BIT_EMMC_SAMPLE_EN                                        (0x1 << 28)
#define BIT_EMMC_FIFO_CLR                                         (0x1 << 27)
#define BIT_EMMC_PRBS_GEN_OUT_EN(x)                               (((x) & 0x1FF) << 18)
#define BIT_EMMC_RX_DL_BYPASS_CMD                                 (0x1 << 17)
#define BIT_EMMC_RX_DL_BYPASS_DAT                                 (0x1 << 16)
#define BIT_EMMC_TX_DL_BYPASS                                     (0x1 << 15)
#define BIT_EMMC_RX_CLK_INV_EN_CMD                                (0x1 << 14)
#define BIT_EMMC_RX_CLK_INV_EN_DAT                                (0x1 << 13)
#define BIT_EMMC_RX_CLK_SEL_CMD                                   (0x1 << 12)
#define BIT_EMMC_RX_CLK_SEL_DAT                                   (0x1 << 11)
#define BIT_EMMC_TX_CLK_GATE                                      (0x1 << 10)
#define BIT_EMMC_TX_CLK_INV_EN                                    (0x1 << 9)
#define BITS_EMMC_PRBS_CHK_IN_SEL(X)                              (((x) & 0xF) << 5)
#define BIT_EMMC_PRBS_MODE                                        (0x1 << 4)
#define BIT_EMMC_PRBS_CHK_EN                                      (0x1 << 3)
#define BIT_EMMC_PRBS_GEN_EN                                      (0x1 << 2)
#define BIT_EMMC_LOOPBACK_MODE                                    (0x1 << 1)
#define BIT_EMMC_PHY_MODE                                         (0x1 << 0)

#define BIT_EMMC_CART_RESET_N                                     (0x1 << 9)
#define BITS_EMMC_IE_CTRL_DAT(X)                                  (((x) & 0xFF) << 1)
#define BIT_EMMC_IE_CTRL_CMD                                      (0x1 << 0)

#define BIT_EMMC_TEST_CMD_OE                                      (0x1 << 16)
#define BITS_EMMC_TEST_DAT_OE(x)                                  (((x) & 0xFF) << 17)

#define BIT_EMMC_TEST_CMD_O                                      (0x1 << 0)
#define BITS_EMMC_TEST_DAT_O(x)                                   (((x) & 0xFF) << 1)

#define BITS_EMMC_CLK_STOP_EN                                     (0x1 << 3)
#define BITS_EMMC_TX_SAMPLE_CLK_SEL                               (0x1 << 0)
#define BITS_EMMC_TX_SAMPLE_CLK_1X_GATE			          (1 << 1)
#define BITS_EMMC_TX_SAMPLE_CLK_2X_GATE			          (1 << 2)

#define EMMC_FREQUENCY							(50000000)

#define REG_EMMC_PHY_HSXX_DLL	(0x1c0000f8)
#define MMC_DLL_HS400			0x202010
#define MMC_DLL_HS200			0x101010
struct mc_sdhci_phy {
	void *ioaddr;
};

struct mc_sdhci_plat {
	struct mmc_config cfg;
	struct mmc mmc;
};

static struct mc_sdhci_phy sdhci_phy;

static inline void sdhci_phy_writel(struct mc_sdhci_phy *host, u32 val, int reg)
{
	writel(val, host->ioaddr + reg);
}

static inline void sdhci_phy_writew(struct mc_sdhci_phy *host, u16 val, int reg)
{
	writew(val, host->ioaddr + reg);
}

static inline void sdhci_phy_writeb(struct mc_sdhci_phy *host, u8 val, int reg)
{
	writeb(val, host->ioaddr + reg);
}
static inline u32 sdhci_phy_readl(struct mc_sdhci_phy *host, int reg)
{
	return readl(host->ioaddr + reg);
}

static inline u16 sdhci_phy_readw(struct mc_sdhci_phy *host, int reg)
{
	return readw(host->ioaddr + reg);
}

static inline u8 sdhci_phy_readb(struct mc_sdhci_phy *host, int reg)
{
	return readb(host->ioaddr + reg);
}

void mmc_ctl_reset(void)
{
	u32 val;
	int i;
	void *addr = (void *)REG_EMMC_CLK_RST_CTRL;
	val = readl(addr);
	val |= BIT_EMMC_PHY_SOFT_RST | BIT_EMMC_HOST_SOFT_RST;
	writel(val, addr);
	for(i=65536;i;i--);
	val &= ~(BIT_EMMC_PHY_SOFT_RST | BIT_EMMC_HOST_SOFT_RST);
	writel(val, addr);
}

void mc_sdhci_set_clock(struct sdhci_host *host, u32 div)
{
	int val;

	if(div == 0){
		div++;
	}else{
		div <<= 1;
	}

	if(host->max_clk == 200000000)
		div <<= 1;
	else if(host->max_clk == 100000000)
		div <<= 2;

	val = readl(REG_EMMC_CLK_RST_CTRL);
	val &= ~(BIT_CKG_EMMC_EN | BIT_CKG_EMMC_2X_EN);
	writel(val, REG_EMMC_CLK_RST_CTRL);


	if(div > 60){
		val = readl(REG_EMMC_CLK_RST_CTRL);
		val &= ~(BITS_CKG_EMMC_2X_SEL_MSK);
		val |= BITS_CKG_EMMC_2X_SEL(0);
		writel(val, REG_EMMC_CLK_RST_CTRL);

		val = readl(REG_EMMC_CLK_RST_CTRL);
		val &= ~(BITS_CKG_EMMC_DIV_MSK| BITS_CKG_EMMC_2X_DIV_MSK);
		val |= BITS_CKG_EMMC_DIV(61) | BITS_CKG_EMMC_2X_DIV(30);
		writel(val, REG_EMMC_CLK_RST_CTRL);
	}
	else{
		val = readl(REG_EMMC_CLK_RST_CTRL);
		val &= ~(BITS_CKG_EMMC_2X_SEL_MSK);
		val &= ~(BITS_CKG_EMMC_BASE_MSK);
		val |= BITS_CKG_EMMC_2X_SEL(3);
		val |= BITS_CKG_EMMC_BASE_SEL(2);
		writel(val, REG_EMMC_CLK_RST_CTRL);

		val = readl(REG_EMMC_CLK_RST_CTRL);
		val &= ~(BITS_CKG_EMMC_DIV_MSK| BITS_CKG_EMMC_2X_DIV_MSK);
		val |= BITS_CKG_EMMC_DIV(div - 1) | BITS_CKG_EMMC_2X_DIV((div / 2) - 1);
		writel(val, REG_EMMC_CLK_RST_CTRL);
	}
	val = readl(REG_EMMC_CLK_RST_CTRL);
	val |= (BIT_CKG_EMMC_EN | BIT_CKG_EMMC_2X_EN);
	writel(val, REG_EMMC_CLK_RST_CTRL);

}


	void mc_sdhci_set_ios_post(struct sdhci_host *host)
	{
		struct mmc *mmc = host->mmc;
		struct mc_sdhci_phy *mc_phy;
		u32 value;
		u32 phy_val;
		mc_phy = &sdhci_phy;

		if(mmc->ddr_mode){
			value = BITS_EMMC_CLK_STOP_EN;
			value |= BITS_EMMC_TX_SAMPLE_CLK_2X_GATE;
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL2);
			value |= BITS_EMMC_TX_SAMPLE_CLK_SEL;
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL2);
			value &= ~BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
			sdhci_phy_writel(mc_phy, value,  EMMC_PHY_CTRL2);//phy ddr mode
	
			value = sdhci_phy_readl(mc_phy, EMMC_PHY_CTRL);
	
			value &= ~(BIT_EMMC_SAMPLE_EN |BIT_EMMC_RX_DL_BYPASS_DAT| BIT_EMMC_TX_DL_BYPASS |
				 BIT_EMMC_RX_CLK_INV_EN_DAT | BIT_EMMC_RX_DL_BYPASS_CMD | BIT_EMMC_RX_CLK_INV_EN_CMD);
			value |= BIT_EMMC_TX_CLK_INV_EN;
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL);
	
			value &= ~BIT_EMMC_DAT_INPUT_EN;
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL);
	
			value |= BIT_EMMC_FIFO_CLR;
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL);
	
			value &= ~BIT_EMMC_FIFO_CLR;
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL);
	
			value |= BIT_EMMC_DAT_INPUT_EN;
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL);

			phy_val = readl(REG_EMMC_PHY_HSXX_DLL);
			if (-1 == phy_val) 
				phy_val = MMC_DLL_HS400;
			sdhci_phy_writel(mc_phy, phy_val, EMMC_PHY_DL_CTRL);

			value |= BIT_EMMC_SAMPLE_EN;
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL);
	
			value |= BIT_EMMC_RX_CLK_SEL_DAT | BIT_EMMC_RX_CLK_SEL_CMD;
			sdhci_phy_writel(mc_phy, value,  EMMC_PHY_CTRL);
	
		}else {
			value = BITS_EMMC_CLK_STOP_EN;
			value &= ~(BITS_EMMC_TX_SAMPLE_CLK_2X_GATE);
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL2);//phy sdr mode
			value &= ~(BITS_EMMC_TX_SAMPLE_CLK_SEL);
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL2);
			value |= BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
			sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL2);
	
			value = sdhci_phy_readl(mc_phy, EMMC_PHY_CTRL);
	
			value &= ~BIT_EMMC_SAMPLE_EN;
			sdhci_phy_writel(mc_phy, value,  EMMC_PHY_CTRL);
	
			value &= ~BIT_EMMC_DAT_INPUT_EN;
			sdhci_phy_writel(mc_phy, value,  EMMC_PHY_CTRL);
	
			if (mmc->clock > EMMC_FREQUENCY) {
				value &= ~(BIT_EMMC_RX_DL_BYPASS_DAT| BIT_EMMC_TX_DL_BYPASS |
					BIT_EMMC_RX_DL_BYPASS_CMD | BIT_EMMC_TX_CLK_INV_EN);
				value |= BIT_EMMC_TX_CLK_INV_EN;
				sdhci_phy_writel(mc_phy, value,  EMMC_PHY_CTRL);

				sdhci_phy_writel(mc_phy, MMC_DLL_HS200, EMMC_PHY_DL_CTRL);
			} else {
				value |= BIT_EMMC_RX_DL_BYPASS_DAT	| BIT_EMMC_TX_DL_BYPASS;
				sdhci_phy_writel(mc_phy, value,  EMMC_PHY_CTRL);
			}
	
			value &= ~(BIT_EMMC_RX_CLK_INV_EN_DAT | BIT_EMMC_RX_CLK_INV_EN_CMD);
			sdhci_phy_writel(mc_phy, value,  EMMC_PHY_CTRL);
	
			value &= ~(BIT_EMMC_RX_CLK_SEL_DAT | BIT_EMMC_RX_CLK_SEL_CMD);
			sdhci_phy_writel(mc_phy, value,  EMMC_PHY_CTRL);
	
			value |= BIT_EMMC_DAT_INPUT_EN;
			sdhci_phy_writel(mc_phy, value,  EMMC_PHY_CTRL);
	
			value |= BIT_EMMC_SAMPLE_EN;
			sdhci_phy_writel(mc_phy, value,  EMMC_PHY_CTRL);
		}
	}

struct sdhci_ops ops = {
	.set_clock	= mc_sdhci_set_clock,
	.set_ios_post = mc_sdhci_set_ios_post,
};

static void mmc_clk_enable(void)
{
	u32 val;
	val = readl(REG_EMMC_CLK_RST_CTRL);
	val |= BIT_CKG_EMMC_2X_EN | BIT_CKG_EMMC_EN | BIT_CKG_EMMC_BASE_EN | BIT_CKG_EMMC_CQETM_EN;
	val |= BIT_CKG_EMMC_TM_EN | BIT_CKG_AXI_EMMC_EN | BIT_CKG_EMMC_PHYCFG_APB_EN;
	writel(val, REG_EMMC_CLK_RST_CTRL);
}

static int mc_sdhci_phy_init(struct udevice *dev)
{
	struct mc_sdhci_phy *mc_phy = &sdhci_phy;
	u32 value;

	mmc_ctl_reset();
	mmc_clk_enable();

	mc_phy->ioaddr = (void *)devfdt_get_addr_index(dev, 1);

	sdhci_phy_writel(mc_phy, 0, EMMC_PHY_INT_SRC);
	value = BITS_EMMC_CLK_STOP_EN | BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
	value &= ~(BITS_EMMC_TX_SAMPLE_CLK_SEL | BITS_EMMC_TX_SAMPLE_CLK_2X_GATE);
	sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL2);//phy sdr mode

	value = sdhci_phy_readl(mc_phy, EMMC_PHY_CTRL);
	value &= ~(BIT_EMMC_PHY_MODE); //function mode
	value &= ~(BIT_EMMC_TX_CLK_INV_EN); //not invert
	value |= BIT_EMMC_TX_CLK_GATE; //not gate
	value &= ~(BIT_EMMC_RX_CLK_SEL_DAT); //clk emmc
	value &= ~(BIT_EMMC_RX_CLK_SEL_CMD); //clk emmc
	value |= BIT_EMMC_RX_DL_BYPASS_DAT | BIT_EMMC_RX_DL_BYPASS_CMD | BIT_EMMC_TX_DL_BYPASS; //enable bypass
	value |= BIT_EMMC_FIFO_CLR;
	sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL);

	value &= ~(BIT_EMMC_FIFO_CLR);
	sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL);//clear receive fifo

	value = sdhci_phy_readl(mc_phy, EMMC_PHY_IE_CTRL);
	value &= ~(BIT_EMMC_CART_RESET_N);
	sdhci_phy_writel(mc_phy, value, EMMC_PHY_IE_CTRL); //reset emmc card

	value |= BIT_EMMC_CART_RESET_N;
	sdhci_phy_writel(mc_phy, value, EMMC_PHY_IE_CTRL); //reset emmc card

	value = sdhci_phy_readl(mc_phy, EMMC_PHY_CTRL);
	value |= BIT_EMMC_SAMPLE_EN |BIT_EMMC_DAT_INPUT_EN | BIT_EMMC_OUTPUT_BIT_MODE(3) | BIT_EMMC_TX_CLK_INV_EN ;
	sdhci_phy_writel(mc_phy, value, EMMC_PHY_CTRL);

	return 0;
}

static int mc_sdhci_bind(struct udevice *dev)
{
	struct mc_sdhci_plat *plat = dev_get_platdata(dev);

	return sdhci_bind(dev, &plat->mmc, &plat->cfg);
}


void sdhci_do_enable_v4_mode(struct sdhci_host *host)
{
	u16 ctrl2;

	ctrl2 = sdhci_readw(host, SDHCI_HOST_CONTROL2);
	if (ctrl2 & SDHCI_CTRL_V4_MODE)
		return;

	ctrl2 |= SDHCI_CTRL_V4_MODE;

	sdhci_writew(host, ctrl2, SDHCI_HOST_CONTROL2);
}

static void emmc_tm_clk_set(void)
{
#if 1
	u32 val;
	void *addr = (void *)REG_EMMC_CLK_RST_CTRL1;
	val = readl(addr);
	val &= ~(BITS_CKG_EMMC_TM_DIV_MSK);
	val |= BITS_CKG_EMMC_TM_DIV(23);
	writel(val, addr);

	val = readl(addr);
	val &= ~(BITS_CKG_EMMC_TM_SEL_MSK);
	val |= BITS_CKG_EMMC_TM_SEL(0);
	writel(val, addr);

#endif
}

static void emmc_driver_ds(void)
{
	u32 val;
	int i = 0;
	void * addr;
	u32 base = 0x1f000004;
	for(i=0; i<11; i++) {
		addr = (void *)(base + i * 4);
		val = readl(addr);
		val &= ~(0xf);
		val |= 0x8;
		writel(val, addr);
	}
}

static void conflict_disable(void)
{
	u32 val;
	val = readl(0x1c000128);
	val &= ~(0x1 << 4);
	writel(val, 0x1c000128);

}

static int mc_sdhci_probe(struct udevice *dev)
{
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct mc_sdhci_plat *plat = dev_get_platdata(dev);
	struct sdhci_host *host = dev_get_priv(dev);
	int ret;

	mc_sdhci_phy_init(dev);

	host->name = dev->name;
	host->ioaddr = (void *)devfdt_get_addr_index(dev, 0);

	host->quirks = SDHCI_QUIRK_WAIT_SEND_CMD |SDHCI_QUIRK_32BIT_DMA_ADDR;
	host->bus_width	= fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev),
					 "bus-width", 4);
	printf("bus_width = %d\n", (int)host->bus_width);

	host->max_clk = 200000000;
	host->host_caps = MMC_MODE_8BIT | MMC_MODE_EMMC_HS200 | MMC_MODE_EMMC_HS400;
	host->ops = &ops;
	ret = sdhci_setup_cfg(&plat->cfg, host, 0, MC_SDHC_MIN_FREQ);
	if (ret)
		return ret;

	host->mmc = &plat->mmc;
	host->mmc->dev = dev;
	host->mmc->priv = host;
	upriv->mmc = host->mmc;

	ret = sdhci_probe(dev);

	sdhci_do_enable_v4_mode(host);
	emmc_tm_clk_set();
	emmc_driver_ds();
	conflict_disable();

	return ret;

}

static const struct udevice_id mc_sdhci_ids[] = {
	{ .compatible = "mc,dw-sdhci" },
	{ }
};

U_BOOT_DRIVER(mc_sdhci_drv) = {
	.name		= "mc_sdhci",
	.id		= UCLASS_MMC,
	.of_match	= mc_sdhci_ids,
	.ops		= &sdhci_ops,
	.bind		= mc_sdhci_bind,
	.probe		= mc_sdhci_probe,
	.priv_auto_alloc_size = sizeof(struct sdhci_host),
	.platdata_auto_alloc_size = sizeof(struct mc_sdhci_plat),
};

