// SPDX-License-Identifier: GPL-2.0
/*
 * PCIe host controller driver for Molchip SoCs
 *
 * Copyright (C) 2023 Molchip Semiconductor.
 *
 * Author: jerry.cao <jerry.cao@molchip.com>
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/of_pci.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/resource.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/gpio.h>
#include "pcie-designware.h"
#include "pcie-molchip.h"

struct mc_pcie_rc_drvdata {
	const struct dw_pcie_host_ops *ops;
	const struct dw_pcie_ops *dw_pcie_ops;
};

struct mc_pcie_rc {
	struct dw_pcie *pci;
	const struct mc_pcie_rc_drvdata *drvdata;
	struct mc_common *com;
	unsigned int reset_gpio;
	int msi_irq;
	char msi_irqname[24];
};

#define to_mc_pcie_rc(x)	dev_get_drvdata((x)->dev)

void disable_rc_bar(struct dw_pcie *pci)
{
	dw_pcie_writel_dbi(pci, SHADOW_OFFSET + PCI_BASE_ADDRESS_0, 0);
	dw_pcie_writel_dbi(pci, SHADOW_OFFSET + PCI_BASE_ADDRESS_1, 0);
}

static int mc_pcie_host_init(struct pcie_port *pp)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct mc_pcie_rc *pcie = to_mc_pcie_rc(pci);
	struct mc_common *com = pcie->com;

	mdelay(200);

	gpio_set_value(pcie->reset_gpio, 1);

	disable_ltssm(com);

	set_type(com, ROOT_PORT_OF_PCIE_RC);

#ifdef CONFIG_PCIE_MOLCHIP_RC_DE_EMPHASIS
	if ((com->port_num == 0))
		adjust_x2_phy_de_emphasis(com, 1);
#endif


	dw_pcie_setup_rc(pp);

	dw_pcie_dbi_ro_wr_en(pci);
	disable_rc_bar(pci);
	dw_pcie_dbi_ro_wr_dis(pci);

	enable_ltssm(com);

	mdelay(100);

	if (!dw_pcie_wait_for_link(pci)) {
		get_speed(com, pci);
		get_ltssm_state(com, pci);
		return 0;
	}

	return -ETIMEDOUT;
}

static int mc_pcie_msi_host_init(struct pcie_port *pp)
{
	struct dw_pcie *pci = to_dw_pcie_from_pp(pp);
	struct device *dev = pci->dev;
	struct device_node *np = dev->of_node;
	struct device_node *msi_node;

	/*
	 * The MSI domain is set by the generic of_msi_configure().  This
	 * .msi_host_init() function keeps us from doing the default MSI
	 * domain setup in dw_pcie_host_init() and also enforces the
	 * requirement that "msi-parent" exists.
	 */
	msi_node = of_parse_phandle(np, "msi-parent", 0);
	if (!msi_node) {
		dev_err(dev, "failed to find msi-parent\n");
		return -EINVAL;
	}

	of_node_put(msi_node);
	return 0;
}

void molchip_msi_init(struct dw_pcie *pci)
{
	dw_pcie_writel_dbi(pci, PCIE_MSI_ADDR_LO, 0x01050040);
	dw_pcie_writel_dbi(pci, PCIE_MSI_ADDR_HI, 0x00000000);
	dw_pcie_writel_dbi(pci, PCIE_MSI_INTR0_ENABLE, 0xffffffff);
	dw_pcie_writel_dbi(pci, PCIE_MSI_INTR0_MASK, 0x00000000);
}

int rc_pcie_link_up(struct dw_pcie *pci)
{
	struct mc_pcie_rc *pcie = to_mc_pcie_rc(pci);
	struct mc_common *com = pcie->com;

	return pcie_link_up(com, pci);
}

static irqreturn_t mc_pcie_msi_handler(int irq, void *arg)
{
	struct mc_pcie_rc *pcie = arg;
	struct dw_pcie *pci = pcie->pci;
	unsigned long val;
	int i, pos;
	irqreturn_t ret = IRQ_NONE;

	for (i = 0; i < MAX_MSI_CTRLS; i++) {
		val = dw_pcie_readl_dbi(pci, PCIE_MSI_INTR0_STATUS + i * 12);
		if (val) {
			dev_info(pci->dev, "molchip_msi_irq val=%ld\r\n", val);
			ret = IRQ_HANDLED;
			pos = 0;
			while ((pos = find_next_bit(&val, 32, pos)) != 32) {
				dw_pcie_writel_dbi(pci,
					PCIE_MSI_INTR0_STATUS + i * 12,
								1 << pos);
				pos++;
			}
		}
	}

	return ret;
}

int register_msi_int(struct mc_pcie_rc *pcie, struct platform_device *pdev)
{
	struct dw_pcie *pci = pcie->pci;
	struct device *dev = pci->dev;
	struct mc_common *com = pcie->com;
	int ret;

	pcie->msi_irq = platform_get_irq_byname(pdev, "msi");
	if (pcie->msi_irq <= 0) {
		dev_err(dev, "failed to get msi irq\n");
		return -ENODEV;
	}

	snprintf(pcie->msi_irqname, sizeof(pcie->msi_irqname),
				"pcie-%d-msi", com->port_num);

	ret = devm_request_irq(dev, pcie->msi_irq,
				mc_pcie_msi_handler,
				IRQF_SHARED | IRQF_NO_THREAD,
				pcie->msi_irqname, pcie);
	if (ret) {
		dev_err(dev, "failed to request msi irq\n");
		return ret;
	}

	return 0;
}

irqreturn_t mc_rc_error_handler(int irq, void *arg)
{
	struct mc_pcie_rc *pcie = arg;
	struct mc_common *com = pcie->com;

	pcie_error_handler(com, pcie->pci);

	return IRQ_HANDLED;
}

irqreturn_t mc_rc_dma_handler(int irq, void *arg)
{
	struct mc_pcie_rc *pcie = arg;
	struct mc_common *com = pcie->com;

	pcie_dma_handler(com, pcie->pci);

	return IRQ_HANDLED;
}

irqreturn_t mc_rc_linkdown_handler(int irq, void *arg)
{
	struct mc_pcie_rc *pcie = arg;
	struct mc_common *com = pcie->com;

	pcie_linkdown_handler(com, pcie->pci);

	return IRQ_HANDLED;
}

int rc_register_dma_int(struct mc_pcie_rc *pcie, struct platform_device *pdev)
{
	struct dw_pcie *pci = pcie->pci;
	struct device *dev = pci->dev;
	struct mc_common *com = pcie->com;
	int ret;

	com->dma_irq = platform_get_irq_byname(pdev, "dma");
	if (com->dma_irq <= 0) {
		dev_err(dev, "failed to get dma irq\n");
		return -ENODEV;
	}

	pcie_dma_init(com, pci);

	snprintf(com->dma_irqname, sizeof(com->dma_irqname),
				"pcie-%d-dma", com->port_num);

	ret = devm_request_irq(dev, com->dma_irq,
				mc_rc_dma_handler,
				IRQF_SHARED | IRQF_NO_THREAD,
				com->dma_irqname, pcie);
	if (ret) {
		dev_err(dev, "failed to request dma irq\n");
		return ret;
	}

	return 0;
}

int rc_register_error_int(struct mc_pcie_rc *pcie, struct platform_device *pdev)
{
	struct dw_pcie *pci = pcie->pci;
	struct device *dev = pci->dev;
	struct mc_common *com = pcie->com;
	int ret;

	com->error_irq = platform_get_irq_byname(pdev, "error");
	if (com->error_irq <= 0) {
		dev_err(dev, "failed to get error irq\n");
		return -ENODEV;
	}

	pcie_error_init(com);

	snprintf(com->err_irqname, sizeof(com->err_irqname),
				"pcie-%d-error", com->port_num);

	ret = devm_request_irq(dev, com->error_irq,
				mc_rc_error_handler,
				IRQF_SHARED | IRQF_NO_THREAD,
				com->err_irqname, pcie);
	if (ret) {
		dev_err(dev, "failed to request error irq\n");
		return ret;
	}

	return 0;
}

int rc_register_linkdown_int(struct mc_pcie_rc *pcie, struct platform_device *pdev)
{
	struct dw_pcie *pci = pcie->pci;
	struct device *dev = pci->dev;
	struct mc_common *com = pcie->com;
	int ret;

	com->linkdown_irq = platform_get_irq_byname(pdev, "linkdown");
	if (com->linkdown_irq <= 0) {
		dev_err(dev, "failed to get linkdown irq\n");
		return -ENODEV;
	}

	snprintf(com->linkdown_irqname, sizeof(com->linkdown_irqname),
				"pcie-%d-linkdown", com->port_num);

	ret = devm_request_irq(dev, com->linkdown_irq,
				mc_rc_linkdown_handler,
				IRQF_SHARED | IRQF_NO_THREAD,
				com->linkdown_irqname, pcie);
	if (ret) {
		dev_err(dev, "failed to request linkdown irq\n");
		return ret;
	}

	return 0;
}

static int __init mc_add_pcie_port(struct mc_pcie_rc *pcie,
				struct platform_device *pdev)
{
	struct dw_pcie *pci = pcie->pci;
	struct pcie_port *pp = &pci->pp;
	struct device *dev = pci->dev;
	struct mc_common *com = pcie->com;
	int ret;
	u8 parm = 0;

	pp->ops = pcie->drvdata->ops;

	pcie_ref_clk_enable(com);
	if (com->port_num == 1)
		port1_rst_and_clk_enable(com, pdev, ROOT_PORT_OF_PCIE_RC);
	else
		port0_rst_and_clk_enable(com, pdev, ROOT_PORT_OF_PCIE_RC);

	ret = pcie_ref_clk_ready_check(com);
	if (ret) {
		dev_err(dev, "failed to get pcie ref clk\n");
		return ret;
	}

	ret = dw_pcie_host_init(pp);
	if (ret) {
		dev_err(dev, "failed to initialize host\n");
		return ret;
	}

	if (com->port_num == 1) {
		parm = mc_readb_phy_cfg(com, 0x19);
		parm &= ~BIT(5);
		mc_writeb_phy_cfg(com, 0x19, parm);
	}

	ret = rc_register_dma_int(pcie, pdev);
	if (ret) {
		dev_err(dev, "failed to reg dam int\n");
		return ret;
	}

	ret = rc_register_error_int(pcie, pdev);
	if (ret) {
		dev_err(dev, "failed to reg error int\n");
		return ret;
	}

	molchip_msi_init(pci);

	ret = register_msi_int(pcie, pdev);
	if (ret) {
		dev_err(dev, "failed to reg msi int\n");
		return ret;
	}

	ret = rc_register_linkdown_int(pcie, pdev);
	if (ret) {
		dev_err(dev, "failed to reg linkdown int\n");
		return ret;
	}

	return 0;
}

#ifdef CONFIG_PCIE_MOLCHIP_RC_COM_TEST
ssize_t rc_compliance_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	struct mc_pcie_rc *pcie = dev_get_drvdata(dev);
	struct dw_pcie *pci = pcie->pci;
	struct mc_common *com = pcie->com;
	int speed = 0;
	int preset = 0;
	int ret;

	ret = sscanf(buf, "%d %d", &speed, &preset);
	if (ret < 1)
		return -EINVAL;
	else if (ret < 2)
		preset = 0;

	dev_info(dev, "compliance GEN%d preset=0x%x\r\n", speed, preset);

	pcie_compliance_test(com, pci, speed, preset);

	return count;
}
#endif

#ifdef CONFIG_PCIE_MOLCHIP_RC_LB_TEST
ssize_t rc_loopback_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	struct mc_pcie_rc *pcie = dev_get_drvdata(dev);
	struct dw_pcie *pci = pcie->pci;
	struct mc_common *com = pcie->com;
	int speed = 0;
	int ret;

	ret = sscanf(buf, "%du", &speed);
	if (ret < 1)
		return -EINVAL;

	dev_info(dev, "loopback speed=%d\r\n", speed);

	pcie_loopback_test(com, pci, speed);

	return count;
}
#endif

#ifdef CONFIG_PCIE_MOLCHIP_RC_DMA_TEST
ssize_t rc_dma_test_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	struct mc_pcie_rc *pcie = dev_get_drvdata(dev);
	struct dw_pcie *pci = pcie->pci;
	struct mc_common *com = pcie->com;
	struct dma_parm parm;
	int num = 0;
	int ret;

	ret = sscanf(buf, "%du", &num);
	if (ret < 1)
		return -EINVAL;

	dev_info(pci->dev, "dma test case=%d\r\n", num);

	if (com->port_num == 0) {
		parm.rd_src = 0x40000000;/*pcie region address*/
		parm.rd_dst = 0x1E0000000;/*cpu region addresss*/
		parm.wr_src = 0x1E0000000;/*cpu region addresss*/
		parm.wr_dst = 0x40000000;/*pcie region address*/
		parm.rd_sim_src = 0x40000000;/*pcie region address*/;
		parm.rd_sim_dst = 0x1E0000000;/*cpu region addresss*/;
		parm.wr_sim_src = 0x1F0000000;/*cpu region addresss*/;
		parm.wr_sim_dst = 0x50000000;/*pcie region address*/;
	} else {
		parm.rd_src = 0x80000000;/*pcie region address*/
		parm.rd_dst = 0x1E0000000;/*cpu region addresss*/
		parm.wr_src = 0x1E0000000;/*cpu region addresss*/
		parm.wr_dst = 0x80000000;/*pcie region address*/
		parm.rd_sim_src = 0x80000000;/*pcie region address*/;
		parm.rd_sim_dst = 0x1E0000000;/*cpu region addresss*/;
		parm.wr_sim_src = 0x1F0000000;/*cpu region addresss*/;
		parm.wr_sim_dst = 0x90000000;/*pcie region address*/;
	}

	dma_test(pci, num, &parm);

	return count;
}
#endif

ssize_t rc_show_status(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct mc_pcie_rc *pcie = dev_get_drvdata(dev);
	struct mc_common *com = pcie->com;
	u32 ret = 0;

	get_speed(com, pcie->pci);

	ret = get_ltssm_state(com, pcie->pci);

	return sprintf(buf, "ltssm:0x%02x\n", ret);
}

ssize_t rc_show_eye_diagram(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct mc_pcie_rc *pcie = dev_get_drvdata(dev);
	struct dw_pcie *pci = pcie->pci;
	struct mc_common *com = pcie->com;
	int speed;

	speed = get_speed(com, pci);

	if (com->port_num == 0) {
		if (pci->num_lanes == 1) {
			get_lane_eye_diagram(com, pci, speed, LANE0_EN_SCOPE_XEYE,
			LANE0_SCOPE_EN, LANE0_XEYE_WIDTH, LANE0_YEYE_HEIGHT);
		} else if (pci->num_lanes == 2) {
			get_lane_eye_diagram(com, pci, speed, LANE0_EN_SCOPE_XEYE,
			LANE0_SCOPE_EN, LANE0_XEYE_WIDTH, LANE0_YEYE_HEIGHT);

			get_lane_eye_diagram(com, pci, speed, LANE1_EN_SCOPE_XEYE,
			LANE1_SCOPE_EN, LANE1_XEYE_WIDTH, LANE1_YEYE_HEIGHT);
		}

		return sprintf(buf, "rx eyes scope\r\n");
	} else {
		return sprintf(buf, "port1 rx eyes scope not support yet!\r\n");
	}
}

static DEVICE_ATTR(status, (S_IRUSR | S_IRGRP), rc_show_status, NULL);

#ifdef CONFIG_PCIE_MOLCHIP_RC_COM_TEST
static DEVICE_ATTR(compliance, (S_IWUSR | S_IWGRP), NULL, rc_compliance_store);
#endif

#ifdef CONFIG_PCIE_MOLCHIP_RC_LB_TEST
static DEVICE_ATTR(loopback, (S_IWUSR | S_IWGRP), NULL, rc_loopback_store);
#endif

#ifdef CONFIG_PCIE_MOLCHIP_RC_DMA_TEST
static DEVICE_ATTR(dma, (S_IWUSR | S_IWGRP), NULL, rc_dma_test_store);
#endif

static DEVICE_ATTR(eye, (S_IRUSR | S_IRGRP), rc_show_eye_diagram, NULL);

static struct attribute *pcie_rc_attributes[] = {
	&dev_attr_status.attr,
#ifdef CONFIG_PCIE_MOLCHIP_RC_COM_TEST
	&dev_attr_compliance.attr,
#endif
#ifdef CONFIG_PCIE_MOLCHIP_RC_LB_TEST
	&dev_attr_loopback.attr,
#endif
#ifdef CONFIG_PCIE_MOLCHIP_RC_DMA_TEST
	&dev_attr_dma.attr,
#endif
	&dev_attr_eye.attr,
	NULL,
};

static const struct attribute_group pcie_rc_attr_group = {
	.attrs = pcie_rc_attributes,
};

static int __init mc_pcie_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct dw_pcie *pci;
	struct mc_common *com;
	struct mc_pcie_rc *pcie;
	struct resource *dbi_base;
	struct resource *ahb_core_base;
	struct resource *ahb_misc_base;
	struct resource *phy_cfg_base;
	u32 eco_reg1_val = 0;
	int ret;

	pcie = devm_kzalloc(dev, sizeof(*pcie), GFP_KERNEL);
	if (!pcie)
		return -ENOMEM;

	pci = devm_kzalloc(dev, sizeof(*pci), GFP_KERNEL);
	if (!pci)
		return -ENOMEM;

	com = devm_kzalloc(dev, sizeof(*com), GFP_KERNEL);
	if (!com)
		return -ENOMEM;

	pcie->drvdata = of_device_get_match_data(dev);

	pci->dev = dev;
	pci->ops = pcie->drvdata->dw_pcie_ops;

	pcie->pci = pci;
	pcie->com = com;

	dbi_base = platform_get_resource_byname(pdev, IORESOURCE_MEM, "dbi");
	pci->dbi_base = devm_pci_remap_cfg_resource(dev, dbi_base);
	if (IS_ERR(pci->dbi_base))
		return PTR_ERR(pci->dbi_base);


	ahb_core_base = platform_get_resource_byname(pdev, IORESOURCE_MEM,
							"ahb_core");
	com->ahb_core_base = devm_ioremap_resource(dev,
					ahb_core_base);
	if (IS_ERR(com->ahb_core_base))
		return PTR_ERR(com->ahb_core_base);

	ret = of_property_read_u32(dev->of_node, "port_num",
					&com->port_num);
	if (ret)
		com->port_num = 0;

	ahb_misc_base = platform_get_resource_byname(pdev,
					IORESOURCE_MEM, "ahb_misc");
	com->ahb_misc_base = devm_ioremap_resource(dev,
							ahb_misc_base);
	if (IS_ERR(com->ahb_misc_base))
		return PTR_ERR(com->ahb_misc_base);

	phy_cfg_base = platform_get_resource_byname(pdev,
					IORESOURCE_MEM, "phy_cfg");
	com->phy_cfg_base = devm_ioremap_resource(dev,
							phy_cfg_base);
	if (IS_ERR(com->phy_cfg_base))
		return PTR_ERR(com->phy_cfg_base);

	ret = of_property_read_u32(dev->of_node, "reset_gpio",
				&pcie->reset_gpio);
	if (ret)
		dev_err(dev, "reset-gpio not found in dts\r\n");

	com->hsif_regmap = syscon_regmap_lookup_by_phandle(dev->of_node,
				"molchip,syscon-hsif-ahb");
	if (IS_ERR(com->hsif_regmap))
		return PTR_ERR(com->hsif_regmap);

	com->glb_regmap = syscon_regmap_lookup_by_phandle(dev->of_node,
				"molchip,syscon-cen-glb");
	if (IS_ERR(com->glb_regmap))
		return PTR_ERR(com->glb_regmap);

	eco_reg1_val = get_eco_reg1(com);
	if ((BIT(com->port_num) & eco_reg1_val)  == 0) {
		dev_err(dev, "port not avaliable\r\n");
		return -1;
	}
	ret = gpio_request(pcie->reset_gpio, "pcie-reset");
	if (ret < 0) {
		dev_err(dev, "reset_gpio request fail\r\n");
		return -1;
	}
	gpio_direction_output(pcie->reset_gpio, 0);
	gpio_set_value(pcie->reset_gpio, 0);

	if (check_once == 0) {
		if (get_strap_status(com) & RESET_CHECK) {
			/*High, internal clk*/
			internal_ref_clk = 1;
		} else {
			/*default Low, extral clk*/
			internal_ref_clk = 0;
		}
		check_once++;
	}

	platform_set_drvdata(pdev, pcie);

	ret = mc_add_pcie_port(pcie, pdev);
	if (ret < 0)
		return ret;

	if (sysfs_create_group(&(pdev->dev.kobj), &pcie_rc_attr_group)) {
		dev_err(dev, "create sysfs failed.\n");
		return -ENOMEM;
	}

	return 0;
}

static const struct dw_pcie_host_ops mc_pcie_host_ops = {
	.host_init = mc_pcie_host_init,
	.msi_host_init = mc_pcie_msi_host_init,
};

static const struct dw_pcie_ops dw_mc_pcie_ops = {
	.link_up = rc_pcie_link_up,
};


static const struct mc_pcie_rc_drvdata mc_drvdata = {
	.ops = &mc_pcie_host_ops,
	.dw_pcie_ops = &dw_mc_pcie_ops,
};

static const struct of_device_id mc_pcie_of_match[] = {
	{ .compatible = "molchip,pcie", .data = &mc_drvdata },
	{ },
};

static struct platform_driver mc_pcie_driver = {
	.driver = {
		.name = "molchip-pcie",
		.of_match_table = mc_pcie_of_match,
	},
};
builtin_platform_driver_probe(mc_pcie_driver, mc_pcie_probe);
