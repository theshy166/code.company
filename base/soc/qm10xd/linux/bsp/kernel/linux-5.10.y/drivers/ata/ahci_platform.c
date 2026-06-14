// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * AHCI SATA platform driver
 *
 * Copyright 2004-2005  Red Hat, Inc.
 *   Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2010  MontaVista Software, LLC.
 *   Anton Vorontsov <avorontsov@ru.mvista.com>
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pm.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/libata.h>
#include <linux/ahci_platform.h>
#include <linux/acpi.h>
#include <linux/pci_ids.h>
#include <linux/regmap.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/proc_fs.h>
#include "ahci.h"

#define DRV_NAME "ahci"

static const struct ata_port_info ahci_port_info = {
	.flags		= AHCI_FLAG_COMMON,
	.pio_mask	= ATA_PIO4,
	.udma_mask	= ATA_UDMA6,
	.port_ops	= &ahci_platform_ops,
};

static const struct ata_port_info ahci_port_info_nolpm = {
	.flags		= AHCI_FLAG_COMMON | ATA_FLAG_NO_LPM,
	.pio_mask	= ATA_PIO4,
	.udma_mask	= ATA_UDMA6,
	.port_ops	= &ahci_platform_ops,
};

static struct scsi_host_template ahci_platform_sht = {
	AHCI_SHT(DRV_NAME),
};

static void ahci_phy_config(struct ahci_host_priv * hpriv){
	void __iomem * phy_mmio = hpriv->comb_phy_cfg;
	u8 tmp;

	tmp = readb(phy_mmio + 0xe);
	tmp |= 0x1;
	writeb(tmp, phy_mmio + 0xe);

	tmp = readb(phy_mmio + 0x18);
	tmp |= (0x1 << 7);
	tmp |= 0x3;
	writeb(tmp, phy_mmio + 0x18);

/**	rx_afe_rtrim[3:0] (0,4,8,12,15)**/
/** rx_afe_ctrim[2:0] (0,2,4,6,7) **/
}

static void ahci_vdu_platform_init(struct ahci_host_priv *hpriv)
{
	struct regmap *mmio = hpriv->vdu_ahb_mmio;
	int i;

	regmap_update_bits(hpriv->cen_apb_mmio, 0x100, 0xffffffff, 0xfedc0123);
	regmap_update_bits(hpriv->cen_apb_mmio, 0xE4, 0x1 << 1, 0x1 << 1);
	regmap_update_bits(hpriv->vdu_ahb_mmio, 0x44, 0x7 << 5, 0x7 << 5);

	for(i=0; i<hpriv->soc_vdu_reg_num; i++)
		regmap_update_bits(mmio, hpriv->soc_reg_offset[i], 
			hpriv->soc_bit_mask[i] << hpriv->soc_bit_offset[i], hpriv->soc_bit_val[i] << hpriv->soc_bit_offset[i]);
	ahci_phy_config(hpriv);
}

static int ahci_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct ahci_host_priv *hpriv;
	const struct ata_port_info *port;
	int rc;
	int pre_num,num;

	hpriv = ahci_platform_get_resources(pdev,
					    AHCI_PLATFORM_GET_RESETS);
	if (IS_ERR(hpriv))
		return PTR_ERR(hpriv);

	pre_num = num = device_property_read_u32_array(dev, "molchip,syscon-reg-offset", NULL, 0);
	if ((num <= 0) || (num > SOC_REG_CFG_MAX) || (pre_num != num)) {
		dev_err(dev, "Fail to get molchip,syscon-reg-offset num\n");
		return -ENODEV;
	}

	rc = device_property_read_u32_array(dev, "molchip,syscon-reg-offset", hpriv->soc_reg_offset, num);
	if (rc < 0) {
		dev_err(dev, "Fail to get syscon-reg-offset\n");
		return rc;
	}

	num = device_property_read_u32_array(dev, "molchip,syscon-bit-offset", NULL, 0);
	if ((num <= 0) || (num > SOC_REG_CFG_MAX) || (pre_num != num)) {
		dev_err(dev, "Fail to get molchip,syscon-bit-offset num\n");
		return -ENODEV;
	}

	rc = device_property_read_u32_array(dev, "molchip,syscon-bit-offset", hpriv->soc_bit_offset, num);
	if (rc < 0) {
		dev_err(dev, "Fail to get syscon-bit-offset\n");
		return rc;
	}

	num = device_property_read_u32_array(dev, "molchip,syscon-bit-mask", NULL, 0);
	if ((num <= 0) || (num > SOC_REG_CFG_MAX) || (pre_num != num)) {
		dev_err(dev, "Fail to get molchip,syscon-bit-mask num\n");
		return -ENODEV;
	}

	rc = device_property_read_u32_array(dev, "molchip,syscon-bit-mask", hpriv->soc_bit_mask, num);
	if (rc < 0) {
		dev_err(dev, "Fail to get syscon-bit-mask\n");
		return rc;
	}

	num = device_property_read_u32_array(dev, "molchip,syscon-bit-val", NULL, 0);
	if ((num <= 0) || (num > SOC_REG_CFG_MAX) || (pre_num != num)) {
		dev_err(dev, "Fail to get molchip,syscon-bit-val num\n");
		return -ENODEV;
	}

	rc = device_property_read_u32_array(dev, "molchip,syscon-bit-val", hpriv->soc_bit_val, num);
	if (rc < 0) {
		dev_err(dev, "Fail to get syscon-bit-val\n");
		return rc;
	}

	hpriv->soc_vdu_reg_num = num;

	num = device_property_read_u32_array(dev, "molchip,sata-phy-rst-sel", NULL, 0);
	if ((num <= 0) || (num > 3)) {
		dev_err(dev, "Fail to get molchip,sata-phy-rst-sel num\n");
		return -ENODEV;
	}

	rc = device_property_read_u32_array(dev, "molchip,sata-phy-rst-sel", hpriv->phy_rst_sel, num);
	if (rc < 0) {
		dev_err(dev, "Fail to get syscon-bit-val\n");
		return rc;
	}

	num = device_property_read_u32_array(dev, "molchip,sata-phy-rst", NULL, 0);
	if ((num <= 0) || (num > 3)) {
		dev_err(dev, "Fail to get molchip,sata-phy-rst num\n");
		return -ENODEV;
	}

	rc = device_property_read_u32_array(dev, "molchip,sata-phy-rst", hpriv->phy_rst, num);
	if (rc < 0) {
		dev_err(dev, "Fail to get molchip,sata-phy-rst\n");
		return rc;
	}

	num = device_property_read_u32_array(dev, "molchip,sata-ctrl-rst", NULL, 0);
	if ((num <= 0) || (num > 3)) {
		dev_err(dev, "Fail to get molchip,sata-ctrl-rst num\n");
		return -ENODEV;
	}

	rc = device_property_read_u32_array(dev, "molchip,sata-ctrl-rst", hpriv->ctrl_rst, num);
	if (rc < 0) {
		dev_err(dev, "Fail to get molchip,sata-ctrl-rst\n");
		return rc;
	}

	of_property_read_u32(dev->of_node,
			     "platform", &hpriv->platform);
	/*init vdu*/
	ahci_vdu_platform_init(hpriv);

	rc = ahci_platform_enable_resources(hpriv);
	if (rc)
		return rc;

	of_property_read_u32(dev->of_node,
			     "ports-implemented", &hpriv->force_port_map);

	if (of_device_is_compatible(dev->of_node, "hisilicon,hisi-ahci"))
		hpriv->flags |= AHCI_HFLAG_NO_FBS | AHCI_HFLAG_NO_NCQ;

	port = acpi_device_get_match_data(dev);
	if (!port)
		port = &ahci_port_info;

	hpriv->flags |= AHCI_HFLAG_YES_FBS;
	rc = ahci_platform_init_host(pdev, hpriv, port,
				     &ahci_platform_sht);
	if (rc)
		goto disable_resources;

	return 0;
disable_resources:
	ahci_platform_disable_resources(hpriv);
	return rc;
}

static SIMPLE_DEV_PM_OPS(ahci_pm_ops, ahci_platform_suspend,
			 ahci_platform_resume);

static const struct of_device_id ahci_of_match[] = {
	{ .compatible = "generic-ahci", },
	/* Keep the following compatibles for device tree compatibility */
	{ .compatible = "snps,spear-ahci", },
	{ .compatible = "ibm,476gtr-ahci", },
	{ .compatible = "snps,dwc-ahci", },
	{ .compatible = "hisilicon,hisi-ahci", },
	{ .compatible = "cavium,octeon-7130-ahci", },
	{},
};
MODULE_DEVICE_TABLE(of, ahci_of_match);

static const struct acpi_device_id ahci_acpi_match[] = {
	{ "APMC0D33", (unsigned long)&ahci_port_info_nolpm },
	{ ACPI_DEVICE_CLASS(PCI_CLASS_STORAGE_SATA_AHCI, 0xffffff) },
	{},
};
MODULE_DEVICE_TABLE(acpi, ahci_acpi_match);

static struct platform_driver ahci_driver = {
	.probe = ahci_probe,
	.remove = ata_platform_remove_one,
	.shutdown = ahci_platform_shutdown,
	.driver = {
		.name = DRV_NAME,
		.of_match_table = ahci_of_match,
		.acpi_match_table = ahci_acpi_match,
		.pm = &ahci_pm_ops,
	},
};
module_platform_driver(ahci_driver);

MODULE_DESCRIPTION("AHCI SATA platform driver");
MODULE_AUTHOR("Anton Vorontsov <avorontsov@ru.mvista.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:ahci");
