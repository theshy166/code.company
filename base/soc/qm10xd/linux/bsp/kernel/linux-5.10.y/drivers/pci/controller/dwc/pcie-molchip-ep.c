// SPDX-License-Identifier: GPL-2.0
/*
 * PCIe controller EP driver for Molchip SoCs
 *
 * Copyright (C) 2023 Molchip Semiconductor.
 *
 * Author: jerry cao <jerry.cao@molchip.com>
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of_pci.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/resource.h>
#include <linux/mfd/syscon.h>
#include <linux/delay.h>
#include "pcie-designware.h"
#include "pcie-molchip.h"

#define to_mc_pcie_ep(x)	dev_get_drvdata((x)->dev)

struct mc_pcie_ep_drvdata {
	u32				func_offset;
	const struct dw_pcie_ep_ops	*ops;
	const struct dw_pcie_ops	*dw_pcie_ops;
};

struct mc_pcie_ep {
	struct dw_pcie			*pci;
	struct pci_epc_features		*mc_epc;
	const struct mc_pcie_ep_drvdata *drvdata;
	struct mc_common *com;
};

struct pci_epf_header molchip_ep_header = {
	.vendorid = 0x4659,
	.deviceid = 0x4d43,
	.revid = 0x1,
	.progif_code = 0x1,
	.subclass_code = 0x80,
	.baseclass_code = 0x4,
	.cache_line_size = 0x0,
	.subsys_vendor_id = 0x3133,
	.subsys_id = 0x4131,
	.interrupt_pin = 0x01
};

int ep_pcie_link_up(struct dw_pcie *pci)
{
	struct mc_pcie_ep *pcie = to_mc_pcie_ep(pci);
	struct mc_common *com = pcie->com;

	return pcie_link_up(com, pci);
}

int ep_pcie_start_link(struct dw_pcie *pci)
{
	struct mc_pcie_ep *pcie = to_mc_pcie_ep(pci);
	struct mc_common *com = pcie->com;

	enable_ltssm(com);

	return 0;
}

void ep_pcie_stop_link(struct dw_pcie *pci)
{
	struct mc_pcie_ep *pcie = to_mc_pcie_ep(pci);
	struct mc_common *com = pcie->com;

	disable_ltssm(com);
}

static const struct dw_pcie_ops dw_mc_pcie_ep_ops = {
	.link_up = ep_pcie_link_up,
	.start_link = ep_pcie_start_link,
	.stop_link = ep_pcie_stop_link,
};

static const struct pci_epc_features*
mc_pcie_ep_get_features(struct dw_pcie_ep *ep)
{
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);
	struct mc_pcie_ep *pcie = to_mc_pcie_ep(pci);

	return pcie->mc_epc;
}

static void mc_pcie_ep_init(struct dw_pcie_ep *ep)
{
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);
	struct mc_pcie_ep *pcie = to_mc_pcie_ep(pci);
	struct dw_pcie_ep_func *ep_func;
	enum pci_barno bar;

	ep_func = dw_pcie_ep_get_func_from_ep(ep, 0);
	if (!ep_func)
		return;

	for (bar = 0; bar < PCI_STD_NUM_BARS; bar++)
		dw_pcie_ep_reset_bar(pci, bar);

	pcie->mc_epc->msi_capable = ep_func->msi_cap ? true : false;
	pcie->mc_epc->msix_capable = ep_func->msix_cap ? true : false;
}

int mc_pcie_ep_raise_legacy_irq(struct dw_pcie_ep *ep, u8 func_no)
{
	u32 val = 0;
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);
	struct mc_pcie_ep *pcie = to_mc_pcie_ep(pci);
	struct mc_common *com = pcie->com;

	val = mc_readl_ahb_core(com, SII_INT0);
	val &= (~BIT(0));
	val |= BIT(0);
	mc_writel_ahb_core(com, SII_INT0, val);

	return 0;
}


static int mc_pcie_ep_raise_irq(struct dw_pcie_ep *ep, u8 func_no,
				enum pci_epc_irq_type type, u16 interrupt_num)
{
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);

	switch (type) {
	case PCI_EPC_IRQ_LEGACY:
		return mc_pcie_ep_raise_legacy_irq(ep, func_no);
	case PCI_EPC_IRQ_MSI:
		return dw_pcie_ep_raise_msi_irq(ep, func_no, interrupt_num);
	case PCI_EPC_IRQ_MSIX:
		return dw_pcie_ep_raise_msix_irq_doorbell(ep, func_no,
							  interrupt_num);
	default:
		dev_err(pci->dev, "UNKNOWN IRQ type\n");
		return -EINVAL;
	}
}

static unsigned int mc_pcie_ep_func_conf_select(struct dw_pcie_ep *ep,
						u8 func_no)
{
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);
	struct mc_pcie_ep *pcie = to_mc_pcie_ep(pci);

	WARN_ON(func_no && !pcie->drvdata->func_offset);
	return pcie->drvdata->func_offset * func_no;
}

static const struct dw_pcie_ep_ops mc_pcie_ep_ops = {
	.ep_init = mc_pcie_ep_init,
	.raise_irq = mc_pcie_ep_raise_irq,
	.get_features = mc_pcie_ep_get_features,
	.func_conf_select = mc_pcie_ep_func_conf_select,
};


irqreturn_t mc_ep_error_handler(int irq, void *arg)
{
	struct mc_pcie_ep *pcie = arg;
	struct mc_common *com = pcie->com;

	pcie_error_handler(com, pcie->pci);

	return IRQ_HANDLED;
}

irqreturn_t mc_ep_dma_handler(int irq, void *arg)
{
	struct mc_pcie_ep *pcie = arg;
	struct mc_common *com = pcie->com;

	pcie_dma_handler(com, pcie->pci);

	return IRQ_HANDLED;
}

irqreturn_t mc_ep_linkdown_handler(int irq, void *arg)
{
	struct mc_pcie_ep *pcie = arg;
	struct mc_common *com = pcie->com;

	pcie_linkdown_handler(com, pcie->pci);

	return IRQ_HANDLED;
}

int ep_register_dma_int(struct mc_pcie_ep *pcie, struct platform_device *pdev)
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
				mc_ep_dma_handler,
				IRQF_SHARED | IRQF_NO_THREAD,
				com->dma_irqname, pcie);
	if (ret) {
		dev_err(dev, "failed to request dma irq\n");
		return ret;
	}

	return 0;
}

int ep_register_error_int(struct mc_pcie_ep *pcie, struct platform_device *pdev)
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
				mc_ep_error_handler,
				IRQF_SHARED | IRQF_NO_THREAD,
				com->err_irqname, pcie);
	if (ret) {
		dev_err(dev, "failed to request error irq\n");
		return ret;
	}

	return 0;
}

int ep_register_linkdown_int(struct mc_pcie_ep *pcie, struct platform_device *pdev)
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
				mc_ep_linkdown_handler,
				IRQF_SHARED | IRQF_NO_THREAD,
				com->linkdown_irqname, pcie);
	if (ret) {
		dev_err(dev, "failed to request linkdown irq\n");
		return ret;
	}

	return 0;
}

int mc_pcie_ep_set_bar(struct dw_pcie *pci, u8 func_no,
			int index, int bar, u64 cpu_addr,
			enum dw_pcie_as_type as_type, int flags, u32 size)
{
	u32 reg, val, offset;

	reg = PCI_BASE_ADDRESS_0 + (4 * bar);
	dw_pcie_prog_inbound_atu(pci, func_no, index, bar, cpu_addr, as_type);
	dw_pcie_dbi_ro_wr_en(pci);

	dw_pcie_writel_dbi(pci, reg + SHADOW_OFFSET, 0x1);
	dw_pcie_writel_dbi(pci, reg, flags);

	if ((bar % 2) == 0) {
		offset = dw_pcie_find_ext_capability(pci, PCI_EXT_CAP_ID_REBAR);

		switch (size) {
		case 1:
			val = 0x10;
			break;
		case 2:
			val = 0x20;
			break;
		case 4:
			val = 0x40;
			break;
		case 8:
			val = 0x80;
			break;
		case 16:
			val = 0x100;
			break;
		case 32:
			val = 0x200;
			break;
		case 64:
			val = 0x400;
			break;
		case 128:
			val = 0x800;
			break;
		case 256:
			val = 0x1000;
			break;
		case 512:
			val = 0x2000;
			break;
		case 1024:
			val = 0x4000;
			break;
		case 2048:
			val = 0x8000;
			break;
		default:
			val = 0x10;
			break;
		}
		dw_pcie_writel_dbi(pci, offset + 4 + 8 * (bar / 2), val);
	}
	dw_pcie_dbi_ro_wr_dis(pci);

	return 0;
}

static int __init mc_add_pcie_ep(struct mc_pcie_ep *pcie,
				 struct platform_device *pdev)
{
	struct dw_pcie *pci = pcie->pci;
	struct device *dev = pci->dev;
	struct dw_pcie_ep *ep;
	struct resource *res;
	struct mc_common *com = pcie->com;
	int ret;
	u8 parm = 0;
	u32 val;

	ep = &pci->ep;
	ep->ops = pcie->drvdata->ops;

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "addr_space");
	if (!res)
		return -EINVAL;

	ep->phys_base = res->start;
	ep->addr_size = resource_size(res);

	pcie_ref_clk_enable(com);

	if (com->port_num == 1)
		port1_rst_and_clk_enable(com, pdev, PCIE_EP);
	else
		port0_rst_and_clk_enable(com, pdev, PCIE_EP);

	ret = pcie_ref_clk_ready_check(com);
	if (ret) {
		dev_err(dev, "failed to get pcie ref clk\n");
		return ret;
	}

	disable_ltssm(com);

	set_type(com, PCIE_EP);


	ret = dw_pcie_ep_init(ep);
	if (ret) {
		dev_err(dev, "failed to initialize endpoint\n");
		return ret;
	}

	pci->ep.epc->ops->write_header(pci->ep.epc, 0, &molchip_ep_header);
	set_MPS(pci, MAX_256_BYTE_SIZE);
	set_trgt_map(pci);

	if (com->port_num == 0) {
		mc_pcie_ep_set_bar(pci, 0, 0, 0, 0x1C0000000, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 256);
		mc_pcie_ep_set_bar(pci, 0, 1, 1, 0x1F0000000, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 0);
		mc_pcie_ep_set_bar(pci, 0, 2, 2, 0x1D0000000, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 256);
		mc_pcie_ep_set_bar(pci, 0, 3, 3, 0x1F0010000, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 0);
		mc_pcie_ep_set_bar(pci, 0, 4, 4, 0x1E0000000, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 256);
		mc_pcie_ep_set_bar(pci, 0, 5, 5, 0x1F0020000, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 0);
	}

	if (com->port_num == 1) {
		mc_pcie_ep_set_bar(pci, 0, 0, 0, 0x200000000, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 256);
		mc_pcie_ep_set_bar(pci, 0, 1, 1, 0x230000000, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 0);
		mc_pcie_ep_set_bar(pci, 0, 2, 2, 0x210000000, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 256);
		mc_pcie_ep_set_bar(pci, 0, 3, 3, 0x230000100, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 0);
		mc_pcie_ep_set_bar(pci, 0, 4, 4, 0x220000000, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 256);
		mc_pcie_ep_set_bar(pci, 0, 5, 5, 0x230000200, DW_PCIE_AS_MEM,
				PCI_BASE_ADDRESS_MEM_TYPE_32, 0);
	}

	/* Setup command register */
	val = dw_pcie_readl_dbi(pci, PCI_COMMAND);
	val &= 0xffff0000;
	val |= PCI_COMMAND_IO | PCI_COMMAND_MEMORY |
		PCI_COMMAND_MASTER | PCI_COMMAND_SERR;
	dw_pcie_writel_dbi(pci, PCI_COMMAND, val);

	enable_ltssm(com);

	mdelay(100);

	if (!dw_pcie_wait_for_link(pci)) {
		get_speed(com, pci);
		get_ltssm_state(com, pci);
	}

	if (com->port_num == 1) {
		parm = mc_readb_phy_cfg(com, 0x19);
		parm &= ~BIT(5);
		mc_writeb_phy_cfg(com, 0x19, parm);
	}

	ret = ep_register_dma_int(pcie, pdev);
	if (ret) {
		dev_err(dev, "failed to reg dam int\n");
		return ret;
	}

	ret = ep_register_error_int(pcie, pdev);
	if (ret) {
		dev_err(dev, "failed to reg error int\n");
		return ret;
	}

	ret = ep_register_linkdown_int(pcie, pdev);
	if (ret) {
		dev_err(dev, "failed to reg linkdown int\n");
		return ret;
	}

	return 0;
}

#ifdef CONFIG_PCIE_MOLCHIP_EP_COM_TEST
ssize_t ep_compliance_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	struct mc_pcie_ep *pcie = dev_get_drvdata(dev);
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

#ifdef CONFIG_PCIE_MOLCHIP_EP_LB_TEST
ssize_t ep_loopback_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	struct mc_pcie_ep *pcie = dev_get_drvdata(dev);
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

#ifdef CONFIG_PCIE_MOLCHIP_EP_DMA_TEST
ssize_t ep_dma_test_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	struct mc_pcie_ep *pcie = dev_get_drvdata(dev);
	struct dw_pcie *pci = pcie->pci;
	struct dma_parm parm;
	int num = 0;
	int ret;

	ret = sscanf(buf, "%du", &num);
	if (ret < 1)
		return -EINVAL;

	dev_info(pci->dev, "dma test case=%d\r\n", num);

	parm.rd_src = 0x1E0000000;/*pcie region address*/
	parm.rd_dst = 0x1E0000000;/*cpu region addresss*/
	parm.wr_src = 0x1E0000000;/*cpu region addresss*/
	parm.wr_dst = 0x1E0000000;/*pcie region address*/
	parm.rd_sim_src = 0x1E0000000;/*pcie region address*/;
	parm.rd_sim_dst = 0x1E0000000;/*cpu region addresss*/;
	parm.wr_sim_src = 0x1F0000000;/*cpu region addresss*/;
	parm.wr_sim_dst = 0x1F0000000;/*pcie region address*/;

	dma_test(pci, num, &parm);

	return count;
}
#endif

ssize_t ep_show_status(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct mc_pcie_ep *pcie = dev_get_drvdata(dev);
	struct mc_common *com = pcie->com;
	u32 ret = 0;

	get_speed(com, pcie->pci);

	ret = get_ltssm_state(com, pcie->pci);

	return sprintf(buf, "ltssm:0x%02x\n", ret);
}

ssize_t ep_show_eye_diagram(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct mc_pcie_ep *pcie = dev_get_drvdata(dev);
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

static DEVICE_ATTR(status, (S_IRUSR | S_IRGRP), ep_show_status, NULL);

#ifdef CONFIG_PCIE_MOLCHIP_EP_COM_TEST
static DEVICE_ATTR(compliance, (S_IWUSR|S_IWGRP), NULL, ep_compliance_store);
#endif

#ifdef CONFIG_PCIE_MOLCHIP_EP_LB_TEST
static DEVICE_ATTR(loopback, (S_IWUSR|S_IWGRP), NULL, ep_loopback_store);
#endif

#ifdef CONFIG_PCIE_MOLCHIP_EP_DMA_TEST
static DEVICE_ATTR(dma, (S_IWUSR|S_IWGRP), NULL, ep_dma_test_store);
#endif

static DEVICE_ATTR(eye, (S_IRUSR | S_IRGRP), ep_show_eye_diagram, NULL);

static struct attribute *pcie_ep_attributes[] = {
	&dev_attr_status.attr,
#ifdef CONFIG_PCIE_MOLCHIP_EP_COM_TEST
	&dev_attr_compliance.attr,
#endif
#ifdef CONFIG_PCIE_MOLCHIP_EP_LB_TEST
	&dev_attr_loopback.attr,
#endif
#ifdef CONFIG_PCIE_MOLCHIP_EP_DMA_TEST
	&dev_attr_dma.attr,
#endif
	&dev_attr_eye.attr,
	NULL,
};

static const struct attribute_group pcie_ep_attr_group = {
	.attrs = pcie_ep_attributes,
};

static int __init mc_pcie_ep_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct dw_pcie *pci;
	struct mc_pcie_ep *pcie;
	struct mc_common *com;
	struct pci_epc_features *mc_epc;
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

	mc_epc = devm_kzalloc(dev, sizeof(*mc_epc), GFP_KERNEL);
	if (!mc_epc)
		return -ENOMEM;

	pcie->drvdata = of_device_get_match_data(dev);

	pci->dev = dev;
	pci->ops = pcie->drvdata->dw_pcie_ops;

	mc_epc->msi_capable = true;
	mc_epc->msix_capable = true;

	pcie->pci = pci;
	pcie->mc_epc = mc_epc;
	pcie->com = com;


	dbi_base = platform_get_resource_byname(pdev, IORESOURCE_MEM, "dbi");
	pci->dbi_base = devm_pci_remap_cfg_resource(dev, dbi_base);
	if (IS_ERR(pci->dbi_base))
		return PTR_ERR(pci->dbi_base);

	pci->dbi_base2 = pci->dbi_base + SHADOW_OFFSET;

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

	ret = mc_add_pcie_ep(pcie, pdev);
	if (ret < 0)
		return ret;

	if (sysfs_create_group(&(pdev->dev.kobj), &pcie_ep_attr_group)) {
		dev_err(dev, "create sysfs failed.\n");
		return -ENOMEM;
	}

	return 0;
}

static const struct mc_pcie_ep_drvdata mc_ep_drvdata = {
	.ops = &mc_pcie_ep_ops,
	.dw_pcie_ops = &dw_mc_pcie_ep_ops,
};

static const struct of_device_id mc_pcie_ep_of_match[] = {
	{ .compatible = "molchip,pcie_ep", .data = &mc_ep_drvdata },
	{ },
};

static struct platform_driver mc_pcie_ep_driver = {
	.driver = {
		.name = "molchip-pcie_ep",
		.of_match_table = mc_pcie_ep_of_match,
	},
};
builtin_platform_driver_probe(mc_pcie_ep_driver, mc_pcie_ep_probe);
