/*
 * drivers/mmc/host/sdhci-mc.c - MC SDHCI Platform driver
 *
 * Copyright (c) 2021-2022, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/delay.h>
#include <linux/mmc/mmc.h>
#include <linux/slab.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <linux/reboot.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>

#include "sdhci-pltfm.h"
#include "mc_mmc_card.h"

#include <linux/mini_clock/clock.h>

#define EMMC_PHY_VERSION_ID		(0x0)
#define EMMC_PHY_INT_EN			(0x4)
#define EMMC_PHY_INT_SRC		(0x08)
#define EMMC_PHY_CTRL			(0x0c)
#define EMMC_PHY_TEST_O			(0x10)
#define EMMC_PHY_TEST_I			(0x14)
#define EMMC_PHY_IE_CTRL		(0x18)
#define EMMC_PHY_DL_CTRL		(0x1c)
#define EMMC_PHY_INT_MASK		(0x20)
#define EMMC_PHY_INT_CTRL2		(0x24)
/**
* INT_SRC
*/
#define 	EMMC_PHY_MODE		BIT(0)
#define 	LOOPBACK_MODE		BIT(1)
#define 	PRBS_GEN_EN			BIT(2)
#define 	PRBS_CHK_EN			BIT(3)
#define 	PRBS_MODE			BIT(4)
#define 	PRBS_CHK_IN_SEL_OFS	6
#define 	PRBS_CHK_IN_SEL_MSK	(0xf << PRBS_CHK_IN_SEL_OFS)
#define 	TX_CLK_INV_EN		BIT(9)
#define 	TX_CLK_GATE			BIT(10)
#define 	RX_CLK_SEL_DAT		BIT(11)
#define 	RX_CLK_SEL_CMD		BIT(12)
#define 	RX_CLK_INV_EN_DAT	BIT(13)
#define 	RX_CLK_INV_EN_CMD	BIT(14)
#define 	TX_DL_BYPASS		BIT(15)
#define 	RX_DL_BYPASS_DAT	BIT(16)
#define 	RX_DL_BYPASS_CMD	BIT(17)
#define 	PRBS_GEN_OUT_EN_OFS	18
#define 	PRBS_GEN_OUT_EN_MSK	(0x1ff << PRBS_GEN_OUT_EN_OFS)
#define 	FIFO_CLR			BIT(27)
#define 	SAMPLE_EN			BIT(28)
#define 	OUTPUT_BIT_MODE_OFS	29
#define 	OUTPUT_BIT_MODE_MSK	(0x3 << OUTPUT_BIT_MODE_OFS)
#define 	DAT_INPUT_EN		BIT(31)

#define		BIT_CKG_EMMC_2X_EN			BIT(24)
#define		BIT_CKG_EMMC_EN			BIT(23)
#define 	BIT_EMMC_HOST_SOFT_RST	BIT(29)
#define 	BIT_EMMC_PHY_SOFT_RST	BIT(28)

#define 	BITS_CKG_EMMC_2X_SEL(x)			(((x) & 0x3) << 2)
#define 	BITS_CKG_EMMC_2X_SEL_MSK			(0x3 << 2)

#define 	BITS_CKG_EMMC_DIV(x)			(((x) & 0x7F) << 11)
#define 	BITS_CKG_EMMC_DIV_MSK			(0x7F << 11)

#define 	BITS_CKG_EMMC_2X_DIV(x)			(((x) & 0x7F) << 4)
#define 	BITS_CKG_EMMC_2X_DIV_MSK			(0x7F << 4)

#define 	BITS_CKG_EMMC_BASE_SEL(x)			(((x) & 0x3) << 18)
#define 	BITS_CKG_EMMC_BASE_SEL_MSK			(0x3 << 18)

#define 	BITS_CKG_EMMC_TM_DIV(x)				(((x) & 0x1F) << 9)
#define 	BITS_CKG_EMMC_TM_DIV_MSK			(0x1F << 9)

#define 	BITS_CKG_EMMC_TM_SEL(x)				(((x) & 0x3) << 7)
#define 	BITS_CKG_EMMC_TM_SEL_MSK			(0x3 << 7)


/**
*DL2_CTRL
**/
#define BITS_EMMC_TX_SAMPLE_CLK_SEL BIT(0)
#define BITS_EMMC_TX_SAMPLE_CLK_1X_GATE BIT(1)
#define BITS_EMMC_TX_SAMPLE_CLK_2X_GATE BIT(2)
#define BITS_EMMC_CLK_STOP_EN BIT(3)

/**
* IE_CTRL
*/
#define 	IE_CTRL_CMD   		BIT(0)
#define 	IE_CTRL_DAT_OFS		1
#define 	IE_CTRL_DAT_MSK		0xff
#define 	CARD_RESET_N		BIT(9)
/**
* DL_CTRL
*/
#define 	DELAY_LINE_CTRL_TX_OFS	0
#define 	DELAY_LINE_CTRL_TX_MSK	0xff
#define 	DELAY_LINE_CTRL_RX_DAT_OFS	8
#define 	DELAY_LINE_CTRL_RX_DAT_MSK	0xff
#define 	DELAY_LINE_CTRL_RX_CMD_OFS	16
#define 	DELAY_LINE_CTRL_RX_CMD_MSK	0xff
#define		DELAY_LINE_TUNING_SEL		BIT(24)

#define REG_EMMC_HSIF_DLL_HS400_VAL		(0x1c0000f8)
#define EMMC_FREQUENCY					(50000000)
#define REG_EMMC_PIN_DRV				(0x1f000004)
#define EMMC_CLK_SEL 					(0x194)

struct sdhci_mc_host {
	struct platform_device *pdev;
	void __iomem *phy_base;	/* emmc phy mapped address enable*/

	int phy_irq;		/* phy irq */
	int wakeup_irq;		/* wakeup irq */
	struct regmap		*map;
	struct mmc_host *mmc;
	int drv_ability;
	struct mmc_card_info * card_info;

};

void sdhci_mc_phy_reset(struct sdhci_mc_host *mc_host)
{
	unsigned int cfg;
	regmap_read(mc_host->map, EMMC_CLK_SEL, &cfg);
	cfg |=(BIT_EMMC_HOST_SOFT_RST | BIT_EMMC_PHY_SOFT_RST);
	regmap_write(mc_host->map, EMMC_CLK_SEL, cfg);

	mdelay(1);

	cfg &= ~(BIT_EMMC_HOST_SOFT_RST | BIT_EMMC_PHY_SOFT_RST);
	regmap_write(mc_host->map, EMMC_CLK_SEL, cfg);
}

void mc_init_phy(struct sdhci_mc_host *mc_host)
{
	u32 ctrl;
	sdhci_mc_phy_reset(mc_host);
	clk_mini_set_rate(CKG_MUX_EMMC_1X, 24000000);

	ctrl = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);

	ctrl &= ~(SAMPLE_EN|DAT_INPUT_EN);
	writel_relaxed(ctrl, mc_host->phy_base + EMMC_PHY_CTRL);

	ctrl = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);

	ctrl = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);
	ctrl &= ~(EMMC_PHY_MODE |  RX_CLK_SEL_DAT | RX_CLK_SEL_CMD);
	ctrl |= TX_CLK_GATE | RX_DL_BYPASS_DAT | RX_DL_BYPASS_CMD | TX_DL_BYPASS;
	ctrl |= TX_CLK_INV_EN | RX_CLK_INV_EN_DAT | RX_CLK_INV_EN_CMD;
	writel_relaxed(ctrl, mc_host->phy_base + EMMC_PHY_CTRL);

	ctrl = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);

	ctrl |= FIFO_CLR;
	writel_relaxed(ctrl, mc_host->phy_base + EMMC_PHY_CTRL);

	ctrl = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);

	ctrl &= ~FIFO_CLR;
	writel_relaxed(ctrl, mc_host->phy_base + EMMC_PHY_CTRL);

	ctrl = readl_relaxed(mc_host->phy_base + EMMC_PHY_IE_CTRL);
	ctrl &= ~CARD_RESET_N;
	writel_relaxed(ctrl, mc_host->phy_base + EMMC_PHY_IE_CTRL);

	ctrl |= CARD_RESET_N;
	writel_relaxed(ctrl, mc_host->phy_base + EMMC_PHY_IE_CTRL);

	ctrl = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);
	ctrl |= SAMPLE_EN | DAT_INPUT_EN | TX_CLK_INV_EN;
	ctrl &= ~OUTPUT_BIT_MODE_MSK;
	ctrl |= 3 << OUTPUT_BIT_MODE_OFS;
	writel_relaxed(ctrl, mc_host->phy_base + EMMC_PHY_CTRL);

	ctrl = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);


}

static void mc_sdhci_drv(u32 drv_value)
{
	u8 i;
	u32 value;
	void* addr_remap = ioremap(REG_EMMC_PIN_DRV, 48);
	void* addr = addr_remap;
	if (!addr_remap)
		return;
	for(i = 0; i < 12; i++){
		value = readl(addr);
		value &= ~(0xf);
		value |= drv_value;
		writel(value, addr);
		addr += 4;
	}
	iounmap(addr_remap);
}

static int mc_select_drive_strength(struct mmc_card *card,unsigned int max_dtr, int host_drv,
				 int card_drv, int *drv_type) {

	struct sdhci_pltfm_host *pltfm_host;
	struct sdhci_mc_host *mc_host;
	struct sdhci_host *host;

	host = mmc_priv(card->host);
	pltfm_host = sdhci_priv(host);
	mc_host = sdhci_pltfm_priv(pltfm_host);

	return mc_host->card_info->drv_strength;
}

static void get_card_info(struct sdhci_host *host)
{
	 int i = 0;
	 struct sdhci_pltfm_host *pltfm_host;
	 struct sdhci_mc_host *mc_host;

	 pltfm_host = sdhci_priv(host);
	 mc_host = sdhci_pltfm_priv(pltfm_host);

	if (!host->mmc->card) {
		return;
	}
	 mc_host->card_info = card_lists;

	 for(i = 0; i < ARRAY_SIZE(card_lists); i++) {
		 if (card_lists[i].manfid == host->mmc->card->cid.manfid) {
			 mc_host->card_info = &card_lists[i];
			 break;
		 }
	 }
}

static u32 get_hs400_phy_dll(void)
{
	void * addr = NULL;
	u32 phy_val = 0;
	addr = ioremap(REG_EMMC_HSIF_DLL_HS400_VAL, 4);
	if (!addr)
		return -1;
	phy_val = readl(addr);
	iounmap(addr);
	return phy_val;
}

void sdhci_mc_set_clk(struct sdhci_mc_host *mc_host,unsigned int clock, int mode)
{

	unsigned int cfg;
	if (clock > 200000000)
		clock = 200000000;

	regmap_read(mc_host->map, EMMC_CLK_SEL, &cfg);
	cfg &= ~(BITS_CKG_EMMC_BASE_SEL_MSK);
	if (clock > 12000000)
		cfg |= (BITS_CKG_EMMC_BASE_SEL(2));
	else
		cfg |= (BITS_CKG_EMMC_BASE_SEL(0));
	regmap_write(mc_host->map, EMMC_CLK_SEL, cfg);

	if (mode == MMC_TIMING_MMC_HS400) {
		if (!clk_mini_is_enabled(CKG_EMMC_2X_EN))
				clk_mini_enable(CKG_EMMC_2X_EN);
		clk_mini_set_rate(CKG_MUX_EMMC_1X, clock/2);
		clk_mini_set_rate(CKG_MUX_EMMC_2X, clock*2);
	} else {
		if (clk_mini_is_enabled(CKG_EMMC_2X_EN))
			clk_mini_disable(CKG_EMMC_2X_EN);
		clk_mini_set_rate(CKG_MUX_EMMC_1X, clock);
	}
}


void sdhci_mc_set_clock(struct sdhci_host *host, unsigned int clock)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	struct sdhci_mc_host *mc_host = sdhci_pltfm_priv(pltfm_host);
	u16 clk;
	u32 reg;

	unsigned long timeout;

	if (clock == 0)
		return;

	get_card_info(host);
	sdhci_mc_set_clk(mc_host, clock, host->timing);


	if(host->timing == MMC_TIMING_MMC_HS400){
		u32 val;
		reg = BITS_EMMC_CLK_STOP_EN;
		reg |= BITS_EMMC_TX_SAMPLE_CLK_2X_GATE;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_INT_CTRL2);
		reg |= BITS_EMMC_TX_SAMPLE_CLK_SEL;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_INT_CTRL2);
		reg &= ~BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_INT_CTRL2);//phy ddr mode

		reg = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);

		reg &= ~(SAMPLE_EN |RX_DL_BYPASS_DAT| TX_DL_BYPASS | RX_CLK_INV_EN_DAT |
			RX_DL_BYPASS_CMD | RX_CLK_INV_EN_CMD);
		reg |= TX_CLK_INV_EN;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

		reg &= ~DAT_INPUT_EN;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

		reg |= FIFO_CLR;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

		reg &= ~FIFO_CLR;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

		reg |= DAT_INPUT_EN;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

		val = get_hs400_phy_dll();
		if (-1 == val)
			writel_relaxed(mc_host->card_info->hs400_dll, mc_host->phy_base + EMMC_PHY_DL_CTRL);
		else
			writel_relaxed(val, mc_host->phy_base + EMMC_PHY_DL_CTRL);

		reg |= SAMPLE_EN;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);
		reg |= RX_CLK_SEL_DAT | RX_CLK_SEL_CMD;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

	}else{

		reg = BITS_EMMC_CLK_STOP_EN;
		reg &= ~(BITS_EMMC_TX_SAMPLE_CLK_2X_GATE);
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_INT_CTRL2);//phy sdr mode
		reg &= ~(BITS_EMMC_TX_SAMPLE_CLK_SEL);
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_INT_CTRL2);
		reg |= BITS_EMMC_TX_SAMPLE_CLK_1X_GATE;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_INT_CTRL2);

		reg = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);
		reg &= ~SAMPLE_EN;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

		reg = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);
		reg &= ~DAT_INPUT_EN;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

		reg = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);
		reg &= ~(RX_CLK_INV_EN_CMD |RX_CLK_INV_EN_DAT | RX_CLK_SEL_CMD | RX_CLK_SEL_DAT | TX_CLK_INV_EN);
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

		if(clock > EMMC_FREQUENCY){
			reg &= ~(RX_DL_BYPASS_DAT| TX_DL_BYPASS | RX_DL_BYPASS_CMD);
			writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);
			writel_relaxed(mc_host->card_info->hs200_dll, mc_host->phy_base + EMMC_PHY_DL_CTRL);
		}else{
			reg |= (RX_DL_BYPASS_DAT | TX_DL_BYPASS | RX_DL_BYPASS_CMD);
			writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);
		}

		reg = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);
		reg |= FIFO_CLR;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);
		reg = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);
		reg &= ~FIFO_CLR;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

		reg = readl_relaxed(mc_host->phy_base + EMMC_PHY_CTRL);
		reg |= SAMPLE_EN | DAT_INPUT_EN;
		writel_relaxed(reg, mc_host->phy_base + EMMC_PHY_CTRL);

	}

	sdhci_writew(host, 0, SDHCI_CLOCK_CONTROL);

	host->mmc->actual_clock = 0;
	clk = sdhci_calc_clk(host, clock, &host->mmc->actual_clock);

	clk |= SDHCI_CLOCK_INT_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);

	/* Wait max 20 ms */
	timeout = 20;
	while (!((clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL))
		& SDHCI_CLOCK_INT_STABLE)) {
		if (timeout == 0) {
			pr_err("%s: Internal clock never stabilised.\n",
			       mmc_hostname(host->mmc));
			return;
		}
		timeout--;
		spin_unlock_irq(&host->lock);
		usleep_range(900, 1100);
		spin_lock_irq(&host->lock);
	}

	clk |= SDHCI_CLOCK_CARD_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);

}

static irqreturn_t sdhci_mc_phy_irq(int irq, void *data)
{
	//struct sdhci_host *host = (struct sdhci_host *)data;

	return IRQ_HANDLED;
}

static const struct sdhci_ops sdhci_mc_ops = {
	.reset = sdhci_reset,
	.set_clock = sdhci_mc_set_clock,
	.set_bus_width = sdhci_set_bus_width,
	.set_uhs_signaling = sdhci_set_uhs_signaling,
	.get_max_clock		= sdhci_pltfm_clk_get_max_clock,
};

static const struct sdhci_pltfm_data sdhci_mc_pdata = {
	.quirks = SDHCI_QUIRK_BROKEN_CARD_DETECTION |
		  SDHCI_QUIRK_NO_CARD_NO_RESET |
		  SDHCI_QUIRK_SINGLE_POWER_WRITE |
		  SDHCI_QUIRK_BROKEN_ADMA_ZEROLEN_DESC,
	.quirks2 = SDHCI_QUIRK2_BROKEN_HS200,
	.ops = &sdhci_mc_ops,
};

static void conflict_disable(struct sdhci_mc_host *mc_host)
{

	u32 cfg;
	regmap_read(mc_host->map, 0x128, &cfg);
	cfg &=  ~(0x1 << 4);
	regmap_write(mc_host->map, 0x128, cfg);
}

static int check_work_over(struct sdhci_host *host)
{
	u32 intmask;
	u32 blkcnt;
	intmask = sdhci_readl(host, SDHCI_INT_STATUS);
	if (intmask & SDHCI_INT_ERROR) {
		printk("err:mmc trans error 0x%x\r\n", intmask);
		return 1;
	}
	blkcnt = sdhci_readl(host, SDHCI_32BIT_BLK_CNT);
	if (blkcnt == 0) {
		if ((intmask & SDHCI_INT_RESPONSE) && (intmask & SDHCI_INT_DATA_END)) {
			sdhci_writel(host, intmask, SDHCI_INT_STATUS);
		} else {
			// no data trans
			return 1;
		}
	}
	return 0;
}

static int sdhci_mc_probe(struct platform_device *pdev)
{
	struct sdhci_host *host;
	struct sdhci_pltfm_host *pltfm_host;
	struct sdhci_mc_host *mc_host;
	struct resource *phy_res;
	struct device *dev = &pdev->dev;
	int ret;
	u16 host_version;
	u32 phy_version;
	int tries = 0;

	host = sdhci_pltfm_init(pdev, &sdhci_mc_pdata, sizeof(*mc_host));
	if (IS_ERR(host))
		return PTR_ERR(host);

	for (tries = 0; tries < 100; tries++) {
		ret = check_work_over(host);
		if (ret)
			break;
		msleep(1);
	}

	pltfm_host = sdhci_priv(host);
	mc_host = sdhci_pltfm_priv(pltfm_host);
	mc_host->mmc = host->mmc;
	mc_host->pdev = pdev;

	dev->platform_data = mc_host;

	host->mmc_host_ops.select_drive_strength = mc_select_drive_strength;

	ret = mmc_of_parse(host->mmc);
	if (ret)
		goto pltfm_free;

	sdhci_get_of_property(pdev);

	ret = of_property_read_u32(dev->of_node, "drv_ability", &mc_host->drv_ability);
	if(ret < 0){
		mc_host->drv_ability = 0xa;
		dev_warn(dev, "Fail to get drv_ability, use default 6!!\n");
	}
	mc_sdhci_drv(mc_host->drv_ability);

	ret = clk_mini_enable(CKG_EMMC_EN);
	if (ret)
		goto clk_disable;
	ret = clk_mini_enable(CKG_EMMC_BASE_EN);
	if (ret)
		goto clk_disable;

	ret = clk_mini_enable(CKG_EMMC_CQETM_EN);
	if (ret)
		goto clk_disable;

	ret = clk_mini_enable(CKG_EMMC_TM_EN);
	if (ret)
		goto clk_disable;
	ret = clk_mini_enable(CKG_AXI_EMMC_EN);
	if (ret)
		goto clk_disable;

	ret = clk_mini_enable(CKG_APB_EMMC_PHYCFG_EN);
	if (ret)
		goto clk_disable;

	ret = clk_mini_enable(CKG_AHB_EMMC_EN);
	if (ret)
		goto clk_disable;

	phy_res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	mc_host->phy_base = devm_ioremap_resource(&pdev->dev, phy_res);

	if (IS_ERR(mc_host->phy_base)) {
		dev_err(&pdev->dev, "Failed to remap registers\n");
		ret = PTR_ERR(mc_host->phy_base);
		goto clk_disable;
	}

	host_version = readw_relaxed((host->ioaddr + SDHCI_HOST_VERSION));
	dev_dbg(&pdev->dev, "Host Version: 0x%x Vendor Version 0x%x\n",
		host_version, ((host_version & SDHCI_VENDOR_VER_MASK) >>
			       SDHCI_VENDOR_VER_SHIFT));

	phy_version = readl_relaxed(mc_host->phy_base + EMMC_PHY_VERSION_ID);
	dev_dbg(&pdev->dev, "MCI Version: 0x%08x\n",
		phy_version);

	/* Setup IRQ for handling power/voltage tasks with PMIC */
	mc_host->phy_irq = platform_get_irq_byname(pdev, "emmc_phy");
	if (mc_host->phy_irq < 0) {
		dev_err(&pdev->dev, "Get phy_irq failed (%d)\n",
			mc_host->phy_irq);
		ret = mc_host->phy_irq;
		goto clk_disable;
	}

	mc_host->wakeup_irq = platform_get_irq_byname(pdev, "emmc_wakeup");
	if (mc_host->wakeup_irq < 0) {
		dev_err(&pdev->dev, "Get wakeup_irq failed (%d)\n",
			mc_host->wakeup_irq);
		ret = mc_host->wakeup_irq;
		goto clk_disable;
	}

	ret = devm_request_threaded_irq(&pdev->dev, mc_host->phy_irq, NULL,
					sdhci_mc_phy_irq, IRQF_ONESHOT,
					dev_name(&pdev->dev), host);
	if (ret) {
		dev_err(&pdev->dev, "Request IRQ failed (%d)\n", ret);
		goto clk_disable;
	}


	mc_host->map = syscon_regmap_lookup_by_phandle(pdev->dev.of_node, "mc,hsif");
	if (IS_ERR_OR_NULL(mc_host->map)) {
		dev_err(&pdev->dev, "failed to get syscon\n");
		ret = -EFAULT;
		goto clk_disable;
	}

	conflict_disable(mc_host);
	mc_init_phy(mc_host);

	writel_relaxed(0xa, mc_host->phy_base + EMMC_PHY_INT_CTRL2);

	sdhci_enable_v4_mode(host);

	ret = sdhci_add_host(host);
	if (ret)
		goto clk_disable;

	return 0;

clk_disable:

	clk_mini_disable(CKG_EMMC_EN);
	clk_mini_disable(CKG_EMMC_BASE_EN);
	clk_mini_disable(CKG_EMMC_CQETM_EN);
	clk_mini_disable(CKG_EMMC_TM_EN);
	clk_mini_disable(CKG_AXI_EMMC_EN);
	clk_mini_disable(CKG_APB_EMMC_PHYCFG_EN);
	clk_mini_disable(CKG_EMMC_2X_EN);
	clk_mini_disable(CKG_AHB_EMMC_EN);

pltfm_free:
	sdhci_pltfm_free(pdev);
	return ret;
}

static int sdhci_mc_remove(struct platform_device *pdev)
{
	struct sdhci_host *host = platform_get_drvdata(pdev);

	int dead = (readl_relaxed(host->ioaddr + SDHCI_INT_STATUS) ==
		    0xffffffff);

	sdhci_remove_host(host, dead);

	clk_mini_disable(CKG_EMMC_EN);
	clk_mini_disable(CKG_EMMC_BASE_EN);
	clk_mini_disable(CKG_EMMC_CQETM_EN);
	clk_mini_disable(CKG_EMMC_TM_EN);
	clk_mini_disable(CKG_AXI_EMMC_EN);
	clk_mini_disable(CKG_APB_EMMC_PHYCFG_EN);
	clk_mini_disable(CKG_EMMC_2X_EN);
	clk_mini_disable(CKG_AHB_EMMC_EN);

	sdhci_pltfm_free(pdev);
	return 0;
}

static const struct of_device_id sdhci_mc_dt_match[] = {
	{ .compatible = "mc,sdhci" },
	{},
};

static int mc_mmc_Suspend(struct device *dev)
{
	struct sdhci_mc_host *mc_host = dev->platform_data;
	printk("emmc clk off\r\n");

	while(mc_host->mmc->ops->card_busy(mc_host->mmc));

	if (!mc_host)
		return -ENOMEM;

	clk_mini_disable(CKG_EMMC_EN);
	clk_mini_disable(CKG_EMMC_CQETM_EN);
	clk_mini_disable(CKG_EMMC_TM_EN);
	clk_mini_disable(CKG_AXI_EMMC_EN);
	clk_mini_disable(CKG_APB_EMMC_PHYCFG_EN);
	clk_mini_disable(CKG_EMMC_2X_EN);

	return 0;
}

static const struct dev_pm_ops mc_mmc_pm_ops = {
	.suspend = mc_mmc_Suspend,
};

MODULE_DEVICE_TABLE(of, sdhci_mc_dt_match);

static struct platform_driver sdhci_mc_driver = {
	.probe = sdhci_mc_probe,
	.remove = sdhci_mc_remove,
	.driver = {
		   .name = "sdhci_mc",
		   .pm = &mc_mmc_pm_ops,
		   .of_match_table = sdhci_mc_dt_match,
	},
};

module_platform_driver(sdhci_mc_driver);

MODULE_DESCRIPTION("Secure Digital Host Controller Interface driver");
MODULE_LICENSE("GPL v2");

