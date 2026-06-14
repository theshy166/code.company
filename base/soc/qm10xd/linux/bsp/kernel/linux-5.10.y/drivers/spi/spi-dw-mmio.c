// SPDX-License-Identifier: GPL-2.0-only
/*
 * Memory-mapped interface driver for DW SPI Core
 *
 * Copyright (c) 2010, Octasic semiconductor.
 */

#include <linux/clk.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/spi/spi.h>
#include <linux/scatterlist.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/acpi.h>
#include <linux/property.h>
#include <linux/regmap.h>
#include <linux/reset.h>

#include "spi-dw.h"
#include <linux/mini_clock/clock.h>

#define DRIVER_NAME "dw_spi_mmio"

struct dw_spi_mmio {
	struct dw_spi  dws;
	struct clk     *clk;
	struct clk     *pclk;
	void           *priv;
	struct reset_control *rstc;
};

#define MSCC_CPU_SYSTEM_CTRL_GENERAL_CTRL	0x24
#define OCELOT_IF_SI_OWNER_OFFSET		4
#define JAGUAR2_IF_SI_OWNER_OFFSET		6
#define MSCC_IF_SI_OWNER_MASK			GENMASK(1, 0)
#define MSCC_IF_SI_OWNER_SISL			0
#define MSCC_IF_SI_OWNER_SIBM			1
#define MSCC_IF_SI_OWNER_SIMC			2

#define MSCC_SPI_MST_SW_MODE			0x14
#define MSCC_SPI_MST_SW_MODE_SW_PIN_CTRL_MODE	BIT(13)
#define MSCC_SPI_MST_SW_MODE_SW_SPI_CS(x)	(x << 5)

#define SPARX5_FORCE_ENA			0xa4
#define SPARX5_FORCE_VAL			0xa8

struct dw_spi_mscc {
	struct regmap       *syscon;
	void __iomem        *spi_mst; /* Not sparx5 */
};

/*
 * The Designware SPI controller (referred to as master in the documentation)
 * automatically deasserts chip select when the tx fifo is empty. The chip
 * selects then needs to be either driven as GPIOs or, for the first 4 using the
 * the SPI boot controller registers. the final chip select is an OR gate
 * between the Designware SPI controller and the SPI boot controller.
 */
static void dw_spi_mscc_set_cs(struct spi_device *spi, bool enable)
{
	struct dw_spi *dws = spi_master_get_devdata(spi->master);
	struct dw_spi_mmio *dwsmmio = container_of(dws, struct dw_spi_mmio, dws);
	struct dw_spi_mscc *dwsmscc = dwsmmio->priv;
	u32 cs = spi->chip_select;

	if (cs < 4) {
		u32 sw_mode = MSCC_SPI_MST_SW_MODE_SW_PIN_CTRL_MODE;

		if (!enable)
			sw_mode |= MSCC_SPI_MST_SW_MODE_SW_SPI_CS(BIT(cs));

		writel(sw_mode, dwsmscc->spi_mst + MSCC_SPI_MST_SW_MODE);
	}

	dw_spi_set_cs(spi, enable);
}

static int dw_spi_mscc_init(struct platform_device *pdev,
			    struct dw_spi_mmio *dwsmmio,
			    const char *cpu_syscon, u32 if_si_owner_offset)
{
	struct dw_spi_mscc *dwsmscc;

	dwsmscc = devm_kzalloc(&pdev->dev, sizeof(*dwsmscc), GFP_KERNEL);
	if (!dwsmscc)
		return -ENOMEM;

	dwsmscc->spi_mst = devm_platform_ioremap_resource(pdev, 1);
	if (IS_ERR(dwsmscc->spi_mst)) {
		dev_err(&pdev->dev, "SPI_MST region map failed\n");
		return PTR_ERR(dwsmscc->spi_mst);
	}

	dwsmscc->syscon = syscon_regmap_lookup_by_compatible(cpu_syscon);
	if (IS_ERR(dwsmscc->syscon))
		return PTR_ERR(dwsmscc->syscon);

	/* Deassert all CS */
	writel(0, dwsmscc->spi_mst + MSCC_SPI_MST_SW_MODE);

	/* Select the owner of the SI interface */
	regmap_update_bits(dwsmscc->syscon, MSCC_CPU_SYSTEM_CTRL_GENERAL_CTRL,
			   MSCC_IF_SI_OWNER_MASK << if_si_owner_offset,
			   MSCC_IF_SI_OWNER_SIMC << if_si_owner_offset);

	dwsmmio->dws.set_cs = dw_spi_mscc_set_cs;
	dwsmmio->priv = dwsmscc;

	return 0;
}

static int dw_spi_mscc_ocelot_init(struct platform_device *pdev,
				   struct dw_spi_mmio *dwsmmio)
{
	return dw_spi_mscc_init(pdev, dwsmmio, "mscc,ocelot-cpu-syscon",
				OCELOT_IF_SI_OWNER_OFFSET);
}

static int dw_spi_mscc_jaguar2_init(struct platform_device *pdev,
				    struct dw_spi_mmio *dwsmmio)
{
	return dw_spi_mscc_init(pdev, dwsmmio, "mscc,jaguar2-cpu-syscon",
				JAGUAR2_IF_SI_OWNER_OFFSET);
}

/*
 * The Designware SPI controller (referred to as master in the
 * documentation) automatically deasserts chip select when the tx fifo
 * is empty. The chip selects then needs to be driven by a CS override
 * register. enable is an active low signal.
 */
static void dw_spi_sparx5_set_cs(struct spi_device *spi, bool enable)
{
	struct dw_spi *dws = spi_master_get_devdata(spi->master);
	struct dw_spi_mmio *dwsmmio = container_of(dws, struct dw_spi_mmio, dws);
	struct dw_spi_mscc *dwsmscc = dwsmmio->priv;
	u8 cs = spi->chip_select;

	if (!enable) {
		/* CS override drive enable */
		regmap_write(dwsmscc->syscon, SPARX5_FORCE_ENA, 1);
		/* Now set CSx enabled */
		regmap_write(dwsmscc->syscon, SPARX5_FORCE_VAL, ~BIT(cs));
		/* Allow settle */
		usleep_range(1, 5);
	} else {
		/* CS value */
		regmap_write(dwsmscc->syscon, SPARX5_FORCE_VAL, ~0);
		/* Allow settle */
		usleep_range(1, 5);
		/* CS override drive disable */
		regmap_write(dwsmscc->syscon, SPARX5_FORCE_ENA, 0);
	}

	dw_spi_set_cs(spi, enable);
}

static int dw_spi_mscc_sparx5_init(struct platform_device *pdev,
				   struct dw_spi_mmio *dwsmmio)
{
	const char *syscon_name = "microchip,sparx5-cpu-syscon";
	struct device *dev = &pdev->dev;
	struct dw_spi_mscc *dwsmscc;

	if (!IS_ENABLED(CONFIG_SPI_MUX)) {
		dev_err(dev, "This driver needs CONFIG_SPI_MUX\n");
		return -EOPNOTSUPP;
	}

	dwsmscc = devm_kzalloc(dev, sizeof(*dwsmscc), GFP_KERNEL);
	if (!dwsmscc)
		return -ENOMEM;

	dwsmscc->syscon =
		syscon_regmap_lookup_by_compatible(syscon_name);
	if (IS_ERR(dwsmscc->syscon)) {
		dev_err(dev, "No syscon map %s\n", syscon_name);
		return PTR_ERR(dwsmscc->syscon);
	}

	dwsmmio->dws.set_cs = dw_spi_sparx5_set_cs;
	dwsmmio->priv = dwsmscc;

	return 0;
}

static int dw_spi_alpine_init(struct platform_device *pdev,
			      struct dw_spi_mmio *dwsmmio)
{
	dwsmmio->dws.caps = DW_SPI_CAP_CS_OVERRIDE;

	return 0;
}

static int dw_spi_dw_apb_init(struct platform_device *pdev,
			      struct dw_spi_mmio *dwsmmio)
{
	dw_spi_dma_setup_generic(&dwsmmio->dws);

	return 0;
}

static int dw_spi_dwc_ssi_init(struct platform_device *pdev,
			       struct dw_spi_mmio *dwsmmio)
{
	dwsmmio->dws.caps = DW_SPI_CAP_DWC_SSI;

	dw_spi_dma_setup_generic(&dwsmmio->dws);

	return 0;
}

static int dw_spi_keembay_init(struct platform_device *pdev,
			       struct dw_spi_mmio *dwsmmio)
{
	dwsmmio->dws.caps = DW_SPI_CAP_KEEMBAY_MST | DW_SPI_CAP_DWC_SSI;

	return 0;
}

static int molchip_spi_apclk_init(struct platform_device *pdev,
		struct dw_spi_mmio *spi)
{
/*	int ret;

	struct device   *dev = &pdev->dev;
	spi->clk = devm_clk_get(dev, "spi_clk");
	if (IS_ERR(spi->clk))
		return PTR_ERR(spi->clk);
	ret = clk_prepare_enable(spi->clk);
	if (ret)
		return ret;

	spi->pclk = devm_clk_get(dev, "apb_spi");
	if (IS_ERR(spi->pclk))
		return PTR_ERR(spi->pclk);
	ret = clk_prepare_enable(spi->pclk);
	if (ret)
		return ret;
*/
	return 0;
}

static int mol_spi_disable_clk(struct dw_spi *dws)
{
	if (dws->id == SPI_DEV0) {
		clk_mini_disable(CKG_SPI0_EN);
		clk_mini_disable(CKG_APB_SPI0_EN);
	} else if (dws->id == SPI_DEV1) {
		clk_mini_disable(CKG_SPI1_EN);
		clk_mini_disable(CKG_APB_SPI1_EN);
	} else if (dws->id == SPI_DEV2) {
		clk_mini_disable(CKG_SPI_SLV_EN);
		clk_mini_disable(CKG_APB_SPI_SLV_EN);
	} else {
		printk("Unknown SPI Device Id[%d] To Close Clk!\n",dws->id);
		return -ENODEV;
	}

	return 0;
}

static int mol_spi_clk_cfg(struct dw_spi *dws)
{
	if (dws->id == SPI_DEV0) {
		clk_mini_set_rate(CKG_MUX_SPI0,SPI_NORMAL_RATE);
		clk_mini_enable(CKG_SPI0_EN);
		clk_mini_enable(CKG_APB_SPI0_EN);
		dws->max_freq = clk_mini_get_rate(CKG_MUX_SPI0);
	} else if (dws->id == SPI_DEV1) {
		clk_mini_set_rate(CKG_MUX_SPI1,SPI_NORMAL_RATE);
		clk_mini_enable(CKG_SPI1_EN);
		clk_mini_enable(CKG_APB_SPI1_EN);
		dws->max_freq = clk_mini_get_rate(CKG_MUX_SPI1);
	} else if (dws->id == SPI_DEV2) {
		clk_mini_set_rate(CKG_MUX_SPI_SLV,SPI_NORMAL_RATE);
		clk_mini_enable(CKG_SPI_SLV_EN);
		clk_mini_enable(CKG_APB_SPI_SLV_EN);
		dws->max_freq = clk_mini_get_rate(CKG_MUX_SPI_SLV);
	} else {
		printk("Unknown SPI Device Id[%d] To Config Clk!\n",dws->id);
		return -ENODEV;
	}

	return 0;
}

static int dw_spi_mmio_probe(struct platform_device *pdev)
{
	int (*init_func)(struct platform_device *pdev,
			 struct dw_spi_mmio *dwsmmio);
	struct dw_spi_mmio *dwsmmio;
	struct resource *mem;
	struct dw_spi *dws;
	int ret;
	int num_cs;

	dwsmmio = devm_kzalloc(&pdev->dev, sizeof(struct dw_spi_mmio),
			GFP_KERNEL);
	if (!dwsmmio)
		return -ENOMEM;

	dws = &dwsmmio->dws;

	/* Get basic io resource and map it */
	dws->regs = devm_platform_get_and_ioremap_resource(pdev, 0, &mem);
	if (IS_ERR(dws->regs))
		return PTR_ERR(dws->regs);

	dws->paddr = mem->start;

	dws->irq = platform_get_irq(pdev, 0);
	if (dws->irq < 0)
		return dws->irq; /* -ENXIO */

	/* use new clock interfaces */
	device_property_read_u32(&pdev->dev, "id", &dws->id);
	mol_spi_clk_cfg(dws);

	/* find an optional reset controller */
	dwsmmio->rstc = devm_reset_control_get_optional_exclusive(&pdev->dev, "spi");
	if (IS_ERR(dwsmmio->rstc)) {
		ret = PTR_ERR(dwsmmio->rstc);
		goto out_clk;
	}
	reset_control_deassert(dwsmmio->rstc);

	dws->bus_num = pdev->id;

	device_property_read_u32(&pdev->dev, "reg-io-width", &dws->reg_io_width);

	num_cs = 4;

	device_property_read_u32(&pdev->dev, "num-cs", &num_cs);

	dws->num_cs = num_cs;
	dws->slave_mode = of_property_read_bool(pdev->dev.of_node,
			"spi-slave-mode");

	init_func = device_get_match_data(&pdev->dev);
	if (init_func) {
		ret = init_func(pdev, dwsmmio);
		if (ret)
			goto out;
	}

	pm_runtime_enable(&pdev->dev);

	ret = dw_spi_add_host(&pdev->dev, dws);
	if (ret)
		goto out;

	platform_set_drvdata(pdev, dwsmmio);
	return 0;

out:
	pm_runtime_disable(&pdev->dev);
	mol_spi_disable_clk(&dwsmmio->dws);
out_clk:
	reset_control_assert(dwsmmio->rstc);

	return ret;
}

static int dw_spi_mmio_remove(struct platform_device *pdev)
{
	struct dw_spi_mmio *dwsmmio = platform_get_drvdata(pdev);

	dw_spi_remove_host(&dwsmmio->dws);
	pm_runtime_disable(&pdev->dev);
	mol_spi_disable_clk(&dwsmmio->dws);
	reset_control_assert(dwsmmio->rstc);

	return 0;
}

static int dw_spi_plat_suspend(struct device *dev)
{
	struct dw_spi_mmio *dwsmmio = dev_get_drvdata(dev);
	struct dw_spi *dws = &dwsmmio->dws;

	spi_hw_disable(dws);
	mol_spi_disable_clk(dws);

	dws->suspended = true;
	return 0;
}

static int dw_spi_plat_resume(struct device *dev)
{
	struct dw_spi_mmio *dwsmmio = dev_get_drvdata(dev);
	struct dw_spi *dws = &dwsmmio->dws;

	if (dws->suspended) {
		mol_spi_clk_cfg(dws);
	}

	dws->suspended = false;

	return 0;
}

static SIMPLE_DEV_PM_OPS(mol_pm_ops, dw_spi_plat_suspend, dw_spi_plat_resume);

static const struct of_device_id dw_spi_mmio_of_match[] = {
	{ .compatible = "snps,dw-apb-ssi", .data = dw_spi_dw_apb_init},
	{ .compatible = "mscc,ocelot-spi", .data = dw_spi_mscc_ocelot_init},
	{ .compatible = "mscc,jaguar2-spi", .data = dw_spi_mscc_jaguar2_init},
	{ .compatible = "amazon,alpine-dw-apb-ssi", .data = dw_spi_alpine_init},
	{ .compatible = "renesas,rzn1-spi", .data = dw_spi_dw_apb_init},
	{ .compatible = "snps,dwc-ssi-1.01a", .data = dw_spi_dwc_ssi_init},
	{ .compatible = "intel,keembay-ssi", .data = dw_spi_keembay_init},
	{ .compatible = "microchip,sparx5-spi", dw_spi_mscc_sparx5_init},
	{ .compatible = "molchip,spi", .data = molchip_spi_apclk_init},
	{ .compatible = "molchip,spi-slave", .data = molchip_spi_apclk_init},
	{ /* end of table */}
};
MODULE_DEVICE_TABLE(of, dw_spi_mmio_of_match);

#ifdef CONFIG_ACPI
static const struct acpi_device_id dw_spi_mmio_acpi_match[] = {
	{"HISI0173", (kernel_ulong_t)dw_spi_dw_apb_init},
	{},
};
MODULE_DEVICE_TABLE(acpi, dw_spi_mmio_acpi_match);
#endif

static struct platform_driver dw_spi_mmio_driver = {
	.probe		= dw_spi_mmio_probe,
	.remove		= dw_spi_mmio_remove,
	.driver		= {
		.name	= DRIVER_NAME,
		.of_match_table = dw_spi_mmio_of_match,
		.acpi_match_table = ACPI_PTR(dw_spi_mmio_acpi_match),
		.pm	= &mol_pm_ops,
	},
};
module_platform_driver(dw_spi_mmio_driver);

MODULE_AUTHOR("Jean-Hugues Deschenes <jean-hugues.deschenes@octasic.com>");
MODULE_DESCRIPTION("Memory-mapped I/O interface driver for DW SPI Core");
MODULE_LICENSE("GPL v2");
