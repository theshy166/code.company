/*
 * drivers/net/phy/realtek.c
 *
 * Driver for Realtek PHYs
 *
 * Author: Johnson Leung <r58129@freescale.com>
 *
 * Copyright (c) 2004 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#include <linux/phy.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/nvmem-consumer.h>
#include <linux/mdio.h>

MODULE_DESCRIPTION("Molchip Ethernet PHY driver");
MODULE_AUTHOR("Jason Qin");
MODULE_LICENSE("GPL");


#define EPHY_REG_BASE 0x0B500000
#define PMU_SWRSTN_NSR	0x15c40000
#define reg_read(addr) (*((volatile unsigned int *)(addr)))
#define reg_write(addr,value) (*(volatile unsigned int *)(addr)=(value))

#define SET_REG_M(addr,value,mask) reg_write(addr,(reg_read(addr)&(~(mask)))|((value)&(mask)))
#define SET_REG(addr,value) reg_write(addr,value)

#define FH_GMAC_PHY_IP101G	0x02430C54
#define FH_GMAC_PHY_RTL8201	0x001CC816
#define FH_GMAC_PHY_TI83848	0xFFFFFFFF
#define FH_GMAC_PHY_INTERNAL 0x441400
#define FH_GMAC_PHY_DUMMY	0xE3FFE3FF
#define FH_GMAC_PHY_INTERNAL_V2 0x46480000
#define FH_GMAC_PHY_RTL8211F 0x001cc916
#define FH_GMAC_PHY_MAE0621 0x7b744411
#define FH_GMAC_PHY_JL2101 0x937c4032


static void* va_ephy;

#define W_PHY	0x01
#define M_PHY	0x02
#define ACTION_MASK	0xff

#define MOLCHIP_PHY_CONFIG_NUM	47
#define MOLCHIP_PHY_PAGE_REG	0x1f
#define MOLCHIP_PHY_PCS_STATUS	0x14
#define MOLCHIP_PHY_CTL_REG	0x13
#define MOLCHIP_PHY_CTL_REG_LED_MODE	BIT(2)
#define MOLCHIP_PHY_CTL_REG_RMII	BIT(5)
#define MOLCHIP_PHY_CTL_REG_FIFO32	BIT(9)
#define MOLCHIP_PHY_CTL_REG_LED_MODE1	BIT(11)
#define MOLCHIP_PHY_RESET	BIT(15)
#define MOLCHIP_PHY_CTL_SEED	BIT(15)

#define SEED_WAIT_CNT 3
#define RESET_WAIT_CNT 12

#define MOLCHIP_PHY_DEBUG 0

static int molchip_ack_interrupt(struct phy_device *phydev)
{
	pr_debug("miku %s: %d\n",__FUNCTION__, __LINE__);
	/* TODO */
	return 0;
}

static int molchip_config_intr(struct phy_device *phydev)
{
	pr_debug("miku %s: %d\n",__FUNCTION__, __LINE__);
	/* TODO */
	return 0;
}

static char *nvmem_read(struct device *dev, const char *cname)
{
	struct nvmem_cell *cell;
	ssize_t data;
	char *ret;

	cell = nvmem_cell_get(dev, cname);
	if (IS_ERR(cell))
		return ERR_CAST(cell);

	ret = nvmem_cell_read(cell, &data);
	nvmem_cell_put(cell);

	return ret;
}

static int molchip_config_init(struct phy_device *phydev)
{
	struct mdio_device *mdiodev = &phydev->mdio;
	char *phy_train;
	unsigned short train_data;

	pr_debug("miku %s: %d\n",__FUNCTION__, __LINE__);

	if (!va_ephy)
		va_ephy = ioremap(EPHY_REG_BASE, SZ_16K);
	if (!va_ephy) {
		pr_err("ioremap internal phy err\n");
		BUG_ON(1);
	}

	phy_train = nvmem_read(&mdiodev->dev,"ephy_train");

	if (IS_ERR(phy_train)) {
		printk("Get ephy train data fail!\n");
		return PTR_ERR(phy_train);
	}

	train_data = *((unsigned short*)phy_train);

	printk("ephy train data 0x%x\n",train_data);

	SET_REG_M((u32)va_ephy + 0x1370, 0x0, 1<<3);
	SET_REG((u32)va_ephy + 0x1274, 0x3c3);
	SET_REG((u32)va_ephy + 0x0b6C, 0xf04);
	SET_REG((u32)va_ephy + 0x1340, 0x0000);
	SET_REG((u32)va_ephy + 0x0bA8, 0x20);
	SET_REG((u32)va_ephy + 0x0018, 0x00c02800);
	//vgb set
	if(BIT(15) & train_data) {
		SET_REG_M((u32)va_ephy + 0x0018, (((train_data & GENMASK(7,6)) >> 6) << 22), 3 << 22);
	}
	SET_REG((u32)va_ephy + 0x001c, 0x00810000);
	SET_REG((u32)va_ephy + 0x0020, 0x0000340c);
	SET_REG((u32)va_ephy + 0x0024, 0x30131500);
	//10M trim
	if(BIT(13) & train_data) {
		SET_REG_M((u32)va_ephy + 0x0024, (((train_data & GENMASK(12,8)) >> 8) << 16), 0x1f << 16);
	}
	//100M trim
	if(BIT(5) & train_data) {
		SET_REG_M((u32)va_ephy + 0x0024, (((train_data & GENMASK(4,0)) >> 0) << 8), 0x1f << 8);
	}
	SET_REG((u32)va_ephy + 0x0028, 0x01402800);
	SET_REG((u32)va_ephy + 0x001c, 0xa1800000);
	SET_REG((u32)va_ephy + 0x0028, 0x00402800);
	SET_REG((u32)va_ephy + 0x0014, 0x06000000);
	SET_REG((u32)va_ephy + 0x0020, 0x2340c);
	SET_REG((u32)va_ephy + 0x000c, 0xba0edfff);
	SET_REG((u32)va_ephy + 0x0010, 0x7fcbf9de);
	SET_REG((u32)va_ephy + 0x0b98, 0xd150);
	SET_REG((u32)va_ephy + 0x1224, 0);
	SET_REG((u32)va_ephy + 0x12f0, 0);
	SET_REG((u32)va_ephy + 0x0bc0, 0x7de);
	SET_REG((u32)va_ephy + 0x0bc8, 0x09de);
	SET_REG((u32)va_ephy + 0x070c, 0x0401);
	SET_REG((u32)va_ephy + 0x0a84, 0x1d20);
	SET_REG((u32)va_ephy + 0x06d4, 0x3130);
	SET_REG((u32)va_ephy + 0x0a6c, 0x0818);
	SET_REG((u32)va_ephy + 0x0aec, 0x1000);
	SET_REG((u32)va_ephy + 0x0658, 0x1c00);
	SET_REG((u32)va_ephy + 0x07d4, 0x6900);
	SET_REG((u32)va_ephy + 0x06b8, 0x4);
	SET_REG((u32)va_ephy + 0x06E4, 0x04a0);
	SET_REG((u32)va_ephy + 0x0BBC, 0x1759);
	SET_REG((u32)va_ephy + 0x0bf8, 0x0100);
	SET_REG((u32)va_ephy + 0x0794, 0x9000);
	SET_REG((u32)va_ephy + 0x0788, 0xf);
	SET_REG((u32)va_ephy + 0x1378, 0x2338);
	SET_REG_M((u32)va_ephy + 0x0, 0x20<<24, 0xff<<24);
	SET_REG_M((u32)va_ephy + 0x4, 0x18, 0xff);
	SET_REG((u32)va_ephy + 0x0680, 0x0552);
	SET_REG((u32)va_ephy + 0x1274, 0);
	SET_REG((u32)va_ephy + 0x0b6C, 0x704);
	SET_REG((u32)va_ephy + 0x1340, 0);
	SET_REG((u32)va_ephy + 0x0ba8, 0);
	SET_REG((u32)va_ephy + 0x06c8, 0xc000);
	SET_REG((u32)va_ephy + 0x06e0, 0x8508);
	SET_REG((u32)va_ephy + 0x1328, 0x0065);
	SET_REG((u32)va_ephy + 0x076c, 0x0430);
	SET_REG((u32)va_ephy + 0x06b4, 0xfd00);
	SET_REG((u32)va_ephy + 0x1360, 0xff37);
	SET_REG_M((u32)va_ephy + 0x0778, 0x24 << 8, 0xff << 8);
	SET_REG_M((u32)va_ephy + 0x077c, 0x24, 0xff);
	SET_REG((u32)va_ephy + 0xb10, 0x5000);
	SET_REG((u32)va_ephy + 0xb14, 0x8000);
	SET_REG((u32)va_ephy + 0xb1c, 0x000e);
	SET_REG((u32)va_ephy + 0xb20, 0x000e);
	SET_REG((u32)va_ephy + 0xb1c, 0x000f);
	SET_REG((u32)va_ephy + 0xb20, 0x0035);
	SET_REG((u32)va_ephy + 0xb1c, 0x0000);
	SET_REG((u32)va_ephy + 0xb10, 0x0000);
	SET_REG((u32)va_ephy + 0xb14, 0x0000);
	/* led */
	SET_REG((u32)va_ephy + 0x7ec, 0x8a6);
	SET_REG((u32)va_ephy + 0x2c, 0x10);
	SET_REG((u32)va_ephy + 0x07f8, 0x325);

	pr_debug("miku %s: %d done\n",__FUNCTION__, __LINE__);

	return 0;
}

#if 0
static int molchip_read_status(struct phy_device *phydev)
{
	printk(KERN_DEBUG "miku %s: %d\n",__FUNCTION__, __LINE__);
	return 0;
}
#endif

static int molchip_enphy_resume(struct phy_device *phydev)
{
	int ret;
	pr_info(KERN_DEBUG "miku %s\n",__FUNCTION__);

	ret = genphy_resume(phydev);
	/* Exit ephy low power state */
	SET_REG((u32)va_ephy + 0x28, 0x00402800);
	SET_REG((u32)va_ephy + 0x0c, 0xBA0EDFFF);
	SET_REG((u32)va_ephy + 0x10, 0x7FCBF9DE);
	SET_REG((u32)va_ephy + 0x00, 0x20000000);

	return ret;
}

static int molchip_enphy_suspend(struct phy_device *phydev)
{
	int ret;
	pr_info(KERN_DEBUG "miku %s\n",__FUNCTION__);

	ret = genphy_suspend(phydev);
	/* Enable ephy low power state */
	SET_REG((u32)va_ephy + 0x28, 0x287870);
	SET_REG((u32)va_ephy + 0x0c, 0xba1edfff);
	SET_REG((u32)va_ephy + 0x10, 0x7fcbf9fe);
	SET_REG((u32)va_ephy + 0x00, 0x00400080);

	return ret;
}

static int molchip_soft_reset(struct phy_device *phydev)
{
	pr_debug("miku %s: %d\n",__FUNCTION__, __LINE__);
	return 0;
}

static int fh_internal_phy_link_up_cb(struct phy_device *phydev)
{

	printk("miku [fh_internal_phy_link_up_cb] %s: %d\n",__FUNCTION__, __LINE__);

	if (!phydev)
		return -1;

	if (!va_ephy) {
		pr_err("va_ephy is null\n");
		BUG_ON(1);
	}

	SET_REG((u32)va_ephy + 0x0b10, 0x5000);

    	SET_REG((u32)va_ephy + 0x0b14, 0x8000);

    	SET_REG((u32)va_ephy + 0x0b1c, 0x000e);

    	SET_REG((u32)va_ephy + 0x0b20, 0x0004);

    	SET_REG((u32)va_ephy + 0x0aec, 0x1800);

    	SET_REG((u32)va_ephy + 0x0b10, 0x0000);

    	SET_REG((u32)va_ephy + 0x0b14, 0x0000);

	//if (phydev->speed == SPEED_10)
	//	SET_REG_M((u32)va_ephy + 0x7f8, 0x3<<8, 0x3<<8);
	return 0;
}

static int fh_internal_phy_link_down_cb(struct phy_device * phydev)
{
	printk("miku [fh_internal_phy_link_down_cb] %s: %d\n",__FUNCTION__, __LINE__);
	if (!va_ephy) {
		pr_err("va_ephy is null\n");
		BUG_ON(1);
	}


	SET_REG((u32)va_ephy + 0xb10, 0x5000);

	SET_REG((u32)va_ephy + 0xb14, 0x8000);

	SET_REG((u32)va_ephy + 0xb1c, 0x000e);

	SET_REG((u32)va_ephy + 0xb20, 0x000e);

	SET_REG((u32)va_ephy + 0xb1c, 0x000f);

	SET_REG((u32)va_ephy + 0xb20, 0x0035);

	SET_REG((u32)va_ephy + 0xb1c, 0x0000);

	SET_REG((u32)va_ephy + 0xb10, 0x0000);

	SET_REG((u32)va_ephy + 0xb14, 0x0000);

	return 0;
}

int molchip_config_aneg(struct phy_device *phydev)
{

	pr_debug("miku [molchip_config_aneg] %s: %d\n",__FUNCTION__, __LINE__);
	return genphy_config_aneg(phydev);
}

static struct phy_driver molchip_fh_drvs[] = {
	{
	 //.phy_id = 0x00177c01,
	 .phy_id = 0x46480000,
	 .name = "MOLCHIP 10/100Mbps Ethernet",
	 .phy_id_mask = 0x001fffff,
	 .features = PHY_BASIC_FEATURES,
	 //.flags = PHY_HAS_INTERRUPT,
	 //.phy_data = &fh_gmac0_data,
	 .brd_link_up_cb = fh_internal_phy_link_up_cb,
	 .brd_link_down_cb = fh_internal_phy_link_down_cb,
	 //.config_aneg = molchip_config_aneg,
	 //.read_status = molchip_read_status,
	 .ack_interrupt = molchip_ack_interrupt,
	 .config_intr = molchip_config_intr,
	 .config_init = molchip_config_init,
	 .soft_reset = molchip_soft_reset,
	 .suspend = molchip_enphy_suspend,
	 .resume = molchip_enphy_resume,
	 },
};

module_phy_driver(molchip_fh_drvs);

static struct mdio_device_id __maybe_unused molchip_fh_tbl[] = {
	{0x46480000, 0x001fffff},
	{}
};

MODULE_DEVICE_TABLE(mdio, molchip_fh_tbl);
