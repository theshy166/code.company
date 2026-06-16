/*
 * Copyright (c) 2019 Molchip Limited.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 1, as published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/ir.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/mfd/syscon.h>
#include <linux/of_device.h>
#include <linux/regmap.h>
#include <media/rc-core.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>

#define IR_ENABLE			0x00
#define IR_CONFIG			0x04
#define CNT_LEADS			0x08
#define CNT_LEADE			0x0c
#define CNT_SLEADE			0x10
#define CNT0_B				0x14
#define CNT1_B				0x18
#define IR_DEBUG			0x1c
#define IR_DATAH			0x20
#define IR_DATAL			0x24
#define IR_INTM				0x28
#define IR_INTS				0x2c
#define IR_INTC				0x30
#define IR_START			0x34
#define IR_LEVEL_WIDTH0			0X38
#define IR_LEVEL_WIDTH1			0X3c
#define IR_LEVEL_WIDTH2			0X40
#define IR_LEVEL_WIDTH3			0X44
#define IR_LEVEL_WIDTH_SP		0X48
#define IR_SPEND_DET_WIDTH		0X4C
#define IR_CNT_DL			0x50
#define IR_CNT_BR			0x54
#define IR_M0_FRAME_ERR_STATUS		0x58
#define IR_M0_LEADS_ERR_DET_TH		0x5C

/*IR_INT_MASK*/
#define BIT_INTM_M1_DATA_FULL		BIT(17)
#define BIT_INTM_M1_DATA_END		BIT(16)
#define BIT_INTM_M0_FRAMERR_L		BIT(5)
#define BIT_INTM_M0_REPEAT		BIT(4)
#define BIT_INTM_M0_RELEASE		BIT(3)
#define BIT_INTM_M0_OVERFLOW		BIT(2)
#define BIT_INTM_M0_FRAMERR		BIT(1)
#define BIT_INTM_M0_DATA_OK		BIT(0)

/*REG_M0_FRAME_ERR_STATUS*/
#define BIT_M0_FRAMERR_L_SLS		BIT(4)
#define BIT_M0_FRAMERR_L_DL		BIT(3)
#define BIT_M0_FRAMERR_L_BST		BIT(2)
#define BIT_M0_FRAMERR_SLE		BIT(1)
#define BIT_M0_FRAMERR_DH		BIT(0)

/*IR_INT_STATUS*/
#define BIT_INTMS_M1_DATA_FULL		BIT(25)
#define BIT_INTMS_M1_DATA_END		BIT(24)
#define BIT_INTMS_M0_FRAMERR_L		BIT(21)
#define BIT_INTMS_M0_REPEAT		BIT(20)
#define BIT_INTMS_M0_RELEASE		BIT(19)
#define BIT_INTMS_M0_OVERFLOW		BIT(18)
#define BIT_INTMS_M0_FRAMERR		BIT(17)
#define BIT_INTMS_M0_DATA_OK		BIT(16)
#define BIT_INTRS_M1_DATA_FULL		BIT(9)
#define BIT_INTRS_M1_DATA_END		BIT(8)
#define BIT_INTRS_M0_FRAMERR_L		BIT(5)
#define BIT_INTRS_M0_REPEAT		BIT(4)
#define BIT_INTRS_M0_RELEASE		BIT(3)
#define BIT_INTRS_M0_OVERFLOW		BIT(2)
#define BIT_INTRS_M0_FRAMERR		BIT(1)
#define BIT_INTRS_M0_DATA_OK		BIT(0)

/*IR_INT_CLR*/
#define BIT_INTC_M1_DATA_FULL		BIT(17)
#define BIT_INTC_M1_DATA_END		BIT(16)
#define BIT_INTC_M0_FRAMERR_L		BIT(5)
#define BIT_INTC_M0_REPEAT		BIT(4)
#define BIT_INTC_M0_RELESE		BIT(3)
#define BIT_INTC_M0_OVERFLOW		BIT(2)
#define BIT_INTC_M0_FRAMERR		BIT(1)
#define BIT_INTC_M0_DATA_OK		BIT(0)

/*ir reset*/
#define SOFT_RESET			0x8
#define IR_SOFT_RESET			BIT(5)

/*THRESHOLD  mode 0 valid*/
#define	IR_ALLOWENCE_THRESHOLD		(400)
#define	IR_ALLOWENCE_THRESHOLD_S	(225)
#define	IR_ALLOWENCE_THRESHOLD_P1	(20)
#define	IR_ALLOWENCE_THRESHOLD_P0	(8)

/*config reg */
#define BITS_IR_MAX_LEVEL_WIDTH_SP(x)	(((x) & 0xFFFF) << 0)
#define BITS_IR_BITS(x)			(((x) & 0x7F) << 2)
#define BITS_IR_FORMAT(x)		(((x) & 0x3) << 9)
#define BITS_IR_MAX_LEVEL_WIDTH_FP(x)	(((x) & 0xFFFF) << 16)
#define BITS_IR_MODE(x)			(((x) & 0x1) << 7)
#define BITS_IR_FREQ(x)			(((x) & 0xFF) << 0)

#define MCIR_PFX			"mc-ir: "
#define BR_DATA				0x001c0060
#define BIT_IR_MODE			(0x1 << 1)
#define IR_MC_NAME			"mc-ir"

struct mc_ir_priv {
	int			irq;
	void __iomem		*base;
	struct device		*dev;
	struct rc_dev		*rdev;
	struct regmap		*regmap;
	struct regmap		*regmap1;
	struct clk		*clock;
	struct clk		*ir_clock;
	struct clk		*ir_apb_clock;
	struct ir_pro_desc	*prodata;
	struct cdev		ir_cdev;

};

static void mc_ir_enable(struct mc_ir_priv *dev)
{
	regmap_update_bits(dev->regmap, SOFT_RESET, 1 << 5, IR_SOFT_RESET);
	mdelay(10);
	regmap_update_bits(dev->regmap, SOFT_RESET, 1 << 5, (unsigned int)~IR_SOFT_RESET);
	writel_relaxed(0x01, dev->base + IR_ENABLE);
	writel_relaxed(0x00, dev->base + IR_CONFIG);


	/*DISABLE ALL INT*/
	writel_relaxed(0xFFFFFFFF, dev->base + IR_INTM);
}

static int ir_get_protocol_allowance(u32 val, u32 th)
{
	u16 max = 0, min = 0;
	u16 percent = 0;

	if(val < th)
		percent = IR_ALLOWENCE_THRESHOLD_P1;
	else
		percent = IR_ALLOWENCE_THRESHOLD_P0;

	max = val * (100 + percent) / 100;
	min = val * (100 - percent) / 100;

	return (u32)((max & 0xffff) | ((min & 0xffff) << 16));
}

static void ir_protocol_config(struct mc_ir_priv *priv)
{
	u32 val = 0, intr = 0;
	u32 reg = 0;

	reg = ir_get_protocol_allowance(priv->prodata->lead_s, IR_ALLOWENCE_THRESHOLD);
	writel_relaxed(reg, priv->base + CNT_LEADS);
	reg = ir_get_protocol_allowance(priv->prodata->lead_e, IR_ALLOWENCE_THRESHOLD);
	writel_relaxed(reg, priv->base + CNT_LEADE);
	reg = ir_get_protocol_allowance(priv->prodata->slead_e, IR_ALLOWENCE_THRESHOLD_S);
	writel_relaxed(reg, priv->base + CNT_SLEADE);
//	reg = ir_get_protocol_allowance(priv->prodata->b_0, IR_ALLOWENCE_THRESHOLD);
	reg = BR_DATA;
	writel_relaxed(reg, priv->base + CNT0_B);
	reg = ir_get_protocol_allowance(priv->prodata->b_1, IR_ALLOWENCE_THRESHOLD);
	writel_relaxed(reg, priv->base + CNT1_B);
	//reg = ir_get_protocol_allowance(priv->prodata->burst, IR_ALLOWENCE_THRESHOLD_S);
	reg = BR_DATA;
	writel_relaxed(reg, priv->base + IR_CNT_BR);
	val = (readl_relaxed(priv->base + CNT1_B) & 0x3FF) * 2;
	writel_relaxed(val, priv->base + IR_SPEND_DET_WIDTH);
	if (priv->prodata->ir_format == 3) {
		val = (u32)((priv->prodata->b_1 * 15) / 10);
		writel_relaxed(val, priv->base + IR_M0_LEADS_ERR_DET_TH);
		reg = ir_get_protocol_allowance(priv->prodata->b_1, IR_ALLOWENCE_THRESHOLD_S);
		writel_relaxed(reg, priv->base + IR_CNT_DL);
	} else {
		writel_relaxed(priv->prodata->b_1 * 3, priv->base + IR_M0_LEADS_ERR_DET_TH);
	//	reg = ir_get_protocol_allowance(priv->prodata->b_0, IR_ALLOWENCE_THRESHOLD_S);
		reg = BR_DATA;
		writel_relaxed(reg, priv->base + IR_CNT_DL);
	}

	val = readl_relaxed(priv->base + IR_CONFIG);
	val |= BITS_IR_BITS(priv->prodata->ir_bits);
	val |= BITS_IR_FORMAT(priv->prodata->ir_format);
	val |= BITS_IR_MAX_LEVEL_WIDTH_FP(priv->prodata->release_time_fp);
	val |= BITS_IR_FREQ(priv->prodata->ir_freq);
	val |= BITS_IR_MODE(priv->prodata->ir_mode);
	writel_relaxed(val, priv->base + IR_CONFIG);
	val = BITS_IR_MAX_LEVEL_WIDTH_SP(priv->prodata->release_time_sp);
	writel_relaxed(val, priv->base + IR_LEVEL_WIDTH_SP);

	intr = BIT_INTM_M0_RELEASE | BIT_INTM_M0_OVERFLOW | BIT_INTM_M0_DATA_OK |
		BIT_INTM_M0_REPEAT | BIT_INTM_M0_FRAMERR | BIT_INTM_M0_FRAMERR_L;

	writel_relaxed(~intr, priv->base + IR_INTM);

	/* write arbitrary value to start  */
	mdelay(1);
	writel_relaxed(0x01, priv->base + IR_START);
}

static int mc_ir_config(struct mc_ir_priv *data)
{
	struct mc_ir_priv *priv = data;

	ir_protocol_config(priv);
	return 0;
}

static int mc_ir_open(struct rc_dev *rdev)
{
	struct mc_ir_priv *priv = rdev->priv;

	mc_ir_enable(priv);

	/* init mcir_dev */
#ifdef 	CONFIG_IR_MOLCHIP_NEC_S
	priv->prodata = &static_dev_param[0];
#endif

#ifdef	CONFIG_IR_MOLCHIP_NEC_F
	priv->prodata = &static_dev_param[1];
#endif

#ifdef	CONFIG_IR_MOLCHIP_TC9012
	priv->prodata = &static_dev_param[2];
#endif

#ifdef	CONFIG_IR_MOLCHIP_SONY
	priv->prodata = &static_dev_param[3];
#endif

	return mc_ir_config(priv);
}

static void mc_ir_close(struct rc_dev *rdev)
{
	struct mc_ir_priv *priv = rdev->priv;

	mc_ir_enable(priv);
}

static irqreturn_t mc_ir_rx_interrupt(int irq, void *data)
{
	u32 intr = 0, ferr = 0;
	u32 datal = 0, datah = 0;
	struct mc_ir_priv *priv = data;
	struct input_dev *input = priv->rdev->input_dev;

	intr = readl_relaxed(priv->base + IR_INTS);
	ferr = readl_relaxed(priv->base + IR_M0_FRAME_ERR_STATUS);
	datal = readl_relaxed(priv->base + IR_DATAL);
	datah = readl_relaxed(priv->base + IR_DATAH);

	if (intr & BIT_INTMS_M0_DATA_OK) {
		writel_relaxed(BIT_INTC_M0_DATA_OK, priv->base + IR_INTC);
		input_report_key(input,  KEY_INFO, 1);
		input_event(input, EV_MSC, MSC_SCAN, datal);
	} else if (intr & BIT_INTMS_M0_REPEAT) {
		writel_relaxed(BIT_INTC_M0_REPEAT, priv->base + IR_INTC);
		input_event(input, EV_MSC, MSC_SCAN, datal);

	} else if (intr & BIT_INTMS_M0_RELEASE) {
		writel_relaxed(BIT_INTC_M0_RELESE, priv->base + IR_INTC);
		input_event(input, EV_KEY, KEY_INFO, 0);

	} else if (intr & BIT_INTMS_M0_FRAMERR_L) {
		dev_err(priv->dev, "Frame_L Error: ");
		writel_relaxed(BIT_INTC_M0_FRAMERR_L, priv->base + IR_INTC);
		if (ferr & BIT_M0_FRAMERR_L_SLS) {
			dev_err(priv->dev, "Frame_L_SLS Error\n");
		} else if (ferr & BIT_M0_FRAMERR_L_DL) {
			dev_err(priv->dev, "Frame_L_DL Error\n");
		} else if (ferr & BIT_M0_FRAMERR_L_BST) {
			dev_err(priv->dev, "Frame_L_BST Error\n");
		} else if (ferr & BIT_M0_FRAMERR_SLE) {
			dev_err(priv->dev, "Frame_SLE Error\n");
		} else if (ferr & BIT_M0_FRAMERR_DH) {
			dev_err(priv->dev, "Frame_DH Error\n");
		} else {
			dev_err(priv->dev, "Not Find descrpit Error, Check it!\n");
		}
	} else if (intr & BIT_INTMS_M0_FRAMERR) {
		dev_err(priv->dev, "Frame Error\n");
		writel_relaxed(BIT_INTC_M0_FRAMERR, priv->base + IR_INTC);
	} else if (intr & BIT_INTMS_M0_OVERFLOW) {
		writel_relaxed(BIT_INTC_M0_OVERFLOW, priv->base + IR_INTC);
		dev_err(priv->dev, "Data Overflow\n");
	} else {
		dev_err(priv->dev, "Not Find interrupt\n");
	}

	input_sync(input);
	//rc_keydown(priv->rdev, protocol, scanmode, 0);
	return IRQ_HANDLED;
}

static int mc_ir_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct rc_dev *rdev = NULL;
	struct device *dev = &pdev->dev;
	struct resource *res = NULL;
	struct mc_ir_priv *priv = NULL;
	struct device_node *node = pdev->dev.of_node, *ir_node;
	const char *map_name = NULL;
	int r = 0;
	u32 ckg_ir_div_addr_offset = 0;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->regmap = syscon_regmap_lookup_by_phandle(node, "molchip,reset-syscon");
//	dev_info(dev, "regmap = 0x%x\n", priv->regmap);
	if (IS_ERR(priv->regmap)) {
		dev_err(dev, "no ir-soft-reg\n");
		return -EINVAL;
	}

	ir_node = of_parse_phandle(node, "molchip,ir-regmap", 0);
	if (ir_node) {
		priv->regmap1 = syscon_node_to_regmap(ir_node);
		if (IS_ERR(priv->regmap1))
			return PTR_ERR(priv->regmap1);
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	priv->base = devm_ioremap_resource(dev, res);
	if (IS_ERR(priv->base))
		return PTR_ERR(priv->base);

	priv->irq = platform_get_irq(pdev, 0);
	dev_info(dev, "ir irq num = 0x%x\n", priv->irq);
	if (priv->irq < 0) {
		dev_err(dev, "irq can not get\n");
		return priv->irq;
	}

	rdev = rc_allocate_device(RC_DRIVER_IR_RAW);
	if (!rdev)
		return -ENOMEM;

	/*enable ir clock*/
	priv->ir_clock = devm_clk_get(dev, "ir_clk");
//	dev_info(dev, "ir clk num = 0x%x\n", priv->ir_clock);
	if (IS_ERR(priv->ir_clock)) {
		dev_err(dev, "ir_clock not found\n");
		ret = PTR_ERR(priv->ir_clock);
		goto err;
	}
	r = clk_prepare_enable(priv->ir_clock);
	if (r) {
		dev_err(dev, "Unable to enable ir clock.\n");
		return r;
	}

	/*enable ir apb clock*/
	priv->ir_apb_clock = devm_clk_get(dev, "ir_apb_clk");
	if (IS_ERR(priv->ir_apb_clock)) {
		dev_err(dev, "apb_clock not found\n");
		ret = PTR_ERR(priv->ir_apb_clock);
		goto err;
	}
	r = clk_prepare_enable(priv->ir_apb_clock);
	if (r) {
		dev_err(dev, "Unable to enable ir apb clock.\n");
		return r;
	}

	/*get clock*/
	priv->clock = devm_clk_get(dev, "ir_mux_clk");
//	dev_info(dev, "ir clock num = 0x%x\n", priv->clock);
	if (IS_ERR(priv->clock)) {
		dev_err(dev, "ir_mux_clk not found.\n");
		return PTR_ERR(priv->clock);
	}

	ret = of_property_read_u32(node, "ckg-ir-div-addr-offset",
				&ckg_ir_div_addr_offset);
	if (ckg_ir_div_addr_offset) {
		regmap_update_bits(priv->regmap1, ckg_ir_div_addr_offset, 0xef << 8, 0xef << 8);
	} else {
		return -ENOMEM;
	}

	priv->prodata =  devm_kzalloc(dev,sizeof(struct ir_pro_desc),GFP_KERNEL);
	if(!priv->prodata){
		dev_err(dev, "kzalloc dma struct failed\n");
		return -ENOMEM;
	}

	rdev->driver_type = RC_DRIVER_SCANCODE;
	rdev->priv = priv;
	rdev->open = mc_ir_open;
	rdev->close = mc_ir_close;
	rdev->driver_name = IR_MC_NAME;
	map_name = of_get_property(node, "linux,rc-map-name", NULL);
	rdev->map_name = map_name ?: RC_MAP_EMPTY;
	rdev->device_name = IR_MC_NAME;
	rdev->input_phys = IR_MC_NAME "/input0";
	rdev->input_id.bustype = BUS_HOST;
	rdev->input_id.vendor = 0x0001;
	rdev->input_id.product = 0x0001;
	rdev->input_id.version = 0x0100;

	ret = rc_register_device(rdev);
	if (ret < 0)
		goto clkerr;

	if (devm_request_irq(dev, priv->irq, mc_ir_rx_interrupt,
				0, pdev->name, priv) < 0) {
		dev_err(dev, "IRQ %d register failed\n", priv->irq);
		ret = -EINVAL;
		goto regerr;
	}

	priv->rdev = rdev;
	priv->dev = dev;
	platform_set_drvdata(pdev, priv);

	if (ret) {
		dev_err(dev, "failed to mcir_init !\n");
		goto err;
	}

	return ret;

regerr:
	rc_unregister_device(rdev);
	rdev = NULL;
clkerr:
	clk_disable_unprepare(priv->clock);
err:
	rc_free_device(rdev);
	dev_err(dev, "Unable to register device (%d)\n", ret);
	return ret;
}

static int mc_ir_remove(struct platform_device *pdev)
{
	struct mc_ir_priv *priv = platform_get_drvdata(pdev);

	clk_disable_unprepare(priv->clock);
	rc_unregister_device(priv->rdev);
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int mc_ir_suspend(struct device *dev)
{
	struct mc_ir_priv *priv = dev_get_drvdata(dev);

	clk_disable_unprepare(priv->clock);
	mc_ir_enable(priv);

	return 0;
}

static int mc_ir_resume(struct device *dev)
{
	struct mc_ir_priv *priv = dev_get_drvdata(dev);
	int ret;

	mc_ir_enable(priv);
	ret = clk_prepare_enable(priv->clock);
	if (ret < 0)
		dev_err(dev, "ir resume cannot enable clock\n");

	writel_relaxed(0x01, priv->base + IR_ENABLE);
	writel_relaxed(0x00, priv->base + IR_INTM);
	writel_relaxed(0xff, priv->base + IR_INTC);
	writel_relaxed(0x01, priv->base + IR_START);

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(mc_ir_pm_ops, mc_ir_suspend, mc_ir_resume);

static const struct of_device_id mc_ir_table[] = {
	{ .compatible = "molchip,mcir", },
	{},
};
MODULE_DEVICE_TABLE(of, mc_ir_table);

static struct platform_driver mc_ir_driver = {
	.driver = {
		.name = "molchip-ir",
		.of_match_table = of_match_ptr(mc_ir_table),
		.pm     = &mc_ir_pm_ops,
	},
	.probe = mc_ir_probe,
	.remove = mc_ir_remove,
};

module_platform_driver(mc_ir_driver);

MODULE_DESCRIPTION("IR controller driver for molchip platforms");
MODULE_AUTHOR("jim.gao <jim.gao@molchip.com>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:mc-ir");
