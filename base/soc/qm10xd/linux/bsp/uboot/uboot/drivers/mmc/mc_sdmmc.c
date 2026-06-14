/*
 * Copyright (c) 2013 Google, Inc
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <dt-structs.h>
#include <dwmmc.h>
#include <errno.h>
#include <mapmem.h>
#include <pwrseq.h>
#include <syscon.h>
#include <linux/err.h>
#include <mach/hardware.h>

#define	CKG_MUX_SDIO0			(BIT(6))

DECLARE_GLOBAL_DATA_PTR;

struct dw_dwmci_host {
	struct mmc_config cfg;
	struct mmc mmc;
};

#if 0
static uint mc_dwmmc_get_mmc_clk(struct dwmci_host *host, uint freq)
{
	struct udevice *dev = host->priv;
	struct dwmci_host *priv = dev_get_priv(dev);
	int ret;

	ret = clk_set_rate(&priv->clock, freq);
	if (ret < 0) {
		debug("%s: err=%d\n", __func__, ret);
		return ret;
	}

	return freq;
}
#endif

static int mc_dwmmc_probe(struct udevice *dev)
{
	struct dw_dwmci_host *plat = dev_get_platdata(dev);
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct dwmci_host *host = dev_get_priv(dev);

	host->name = dev->name;
	host->ioaddr = (void *)devfdt_get_addr_index(dev, 0);

	host->ctrl_id = fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev),
					 "id", 4);

	#ifndef CONFIG_MOLCHIP_FASTBOOT
	printf("sd id = %d \n", (int)host->ctrl_id);
	#endif
	mc_dwmmc_clk_cfg(host->ctrl_id);

	//****** host->quirks = ;
	host->buswidth = fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev),
					 "bus-width", 4);
	#ifndef CONFIG_MOLCHIP_FASTBOOT
	printf("sd buswidth = %d \n", (int)host->buswidth);
	#endif
	host->get_mmc_clk = 0;
	host->bus_hz = 50000000;
	host->dev_index = 0;

	host->fifoth_val = 0;
	host->fifo_mode = 0;

	dwmci_setup_cfg(&plat->cfg, host, 100000000, 400000);
	host->mmc = &plat->mmc;
	host->mmc->priv = host;
	host->mmc->dev = dev;
	upriv->mmc = host->mmc;

	return dwmci_probe(dev);
}

static int mc_dwmmc_bind(struct udevice *dev)
{
	struct dw_dwmci_host *plat = dev_get_platdata(dev);

	return dwmci_bind(dev, &plat->mmc, &plat->cfg);
}

static const struct udevice_id mc_dwmmc_ids[] = {
	{ .compatible = "mc,dw-sdmmc" },
	{ }
};

U_BOOT_DRIVER(mc_sdmmc_dev) = {
	.name		= "mc_sdmmc",
	.id		= UCLASS_MMC,
	.of_match	= mc_dwmmc_ids,
	.ops		= &dm_dwmci_ops,
	.bind		= mc_dwmmc_bind,
	.probe		= mc_dwmmc_probe,
	.priv_auto_alloc_size = sizeof(struct dwmci_host),
	.platdata_auto_alloc_size = sizeof(struct dw_dwmci_host),
};


