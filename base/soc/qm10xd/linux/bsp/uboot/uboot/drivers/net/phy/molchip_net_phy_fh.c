/*
 * (C) Copyright 2019
 * Molchip <www.molchip.com>
 * Written-by: Jason Qin <Jason.qin@molchip.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <config.h>
#include <common.h>
#include <linux/bitops.h>
#include <phy.h>
#include <miiphy.h>
#include <asm/mach-types.h>
#include <asm/io.h>
#include <dm.h>
#include <dm/uclass-internal.h>
#include <misc.h>
#include <linux/bitops.h>

#define reg_read(addr) (*((volatile unsigned int *)(addr)))
#define reg_write(addr,value) (*(volatile unsigned int *)(addr)=(value))

#define SET_REG_M(addr,value,mask) reg_write(addr,(reg_read(addr)&(~(mask)))|((value)&(mask)))
#define SET_REG(addr,value) reg_write(addr,value)

#define MOLCHIP_PHY_FEATURES	(SUPPORTED_Autoneg | \
								SUPPORTED_TP | \
								SUPPORTED_MII | \
								PHY_10BT_FEATURES | \
								PHY_100BT_FEATURES)

#define MOLCHIP_MII_PHYPAGE	0x1f
#define MOLCHIP_MII_BMCR1	0x13
#define MOLCHIP_PHY_PCS_STATUS	0x14
#define MOLCHIP_PHY_CONFIG_NUM	47
#define MOLCHIP_PHY_CTL_REG_LED_MODE	BIT(2)
#define MOLCHIP_PHY_CTL_REG_RMII	BIT(5)
#define MOLCHIP_PHY_CTL_REG_FIFO32	BIT(9)
#define MOLCHIP_PHY_CTL_REG_LED_MODE1	BIT(11)
#define MOLCHIP_PHY_CTL_SEED	BIT(15)

#define MOLCHIP_PHY_DEBUG	0
#define ANA_EPHY_BASE_REG	0x28500000
#define EPHY_REG_BASE 0x0B500000
#define PMU_SWRSTN_NSR	0x15c40000
#define EPHY_TRAIN_SET	13

unsigned short train_data;

static int mochip7c01_phy_config(struct phy_device *phydev)
{

	debug("skip internal phy config...\n");
#if 0
	SET_REG(EPHY_REG_BASE + 0x1370, 0x1006);
	SET_REG(EPHY_REG_BASE + 0x0b84, 0x0600);
	SET_REG(EPHY_REG_BASE + 0x1270, 0x1900);
	SET_REG(EPHY_REG_BASE + 0x0b98, 0xd150);
	SET_REG(EPHY_REG_BASE + 0x1274, 0x0000);
	SET_REG(EPHY_REG_BASE + 0x0bf8, 0x0130);
	SET_REG(EPHY_REG_BASE + 0x1374, 0x0009);
	SET_REG(EPHY_REG_BASE + 0x0b6c, 0x0704);
	SET_REG(EPHY_REG_BASE + 0x1340, 0x0000);
	SET_REG(EPHY_REG_BASE + 0x0ba8, 0x0000);
	SET_REG(EPHY_REG_BASE + 0x0b7c, 0x10c0);
	SET_REG(EPHY_REG_BASE + 0x1308, 0x0000);
	SET_REG(EPHY_REG_BASE + 0x1378, 0x1820);
	SET_REG(EPHY_REG_BASE + 0x0000, 0x20000000);
	SET_REG(EPHY_REG_BASE + 0x0004, 0x00000018);
	SET_REG(EPHY_REG_BASE + 0x0014, 0x06000000);
	SET_REG(EPHY_REG_BASE + 0x0018, 0x00c00040);
	SET_REG(EPHY_REG_BASE + 0x001c, 0xa1800000);
	SET_REG(EPHY_REG_BASE + 0x0020, 0x0002340c);
	SET_REG(EPHY_REG_BASE + 0x0024, 0x10000000);
	SET_REG(EPHY_REG_BASE + 0x0028, 0x00402800);
	SET_REG(EPHY_REG_BASE + 0x000c, 0xba0edfff);
	SET_REG(EPHY_REG_BASE + 0x0010, 0x7fcbf9de);
	SET_REG(EPHY_REG_BASE + 0x1224, 0x0000);
	SET_REG(EPHY_REG_BASE + 0x12f0, 0x0000);
	SET_REG(EPHY_REG_BASE + 0x0bc0, 0x07de);
	SET_REG(EPHY_REG_BASE + 0x0bc8, 0x09de);
	SET_REG(EPHY_REG_BASE + 0x070c, 0x0401);
	SET_REG(EPHY_REG_BASE + 0x06b4, 0xfd00);
	SET_REG(EPHY_REG_BASE + 0x0a84, 0x1d20);
	SET_REG(EPHY_REG_BASE + 0x06d4, 0x3130);
	SET_REG(EPHY_REG_BASE + 0x0a6c, 0x0818);
	SET_REG(EPHY_REG_BASE + 0x0aec, 0x1000);
	SET_REG(EPHY_REG_BASE + 0x0658, 0x1c00);
	SET_REG(EPHY_REG_BASE + 0x07d4, 0x6900);
	SET_REG(EPHY_REG_BASE + 0x06b8, 0x0004);
	SET_REG(EPHY_REG_BASE + 0x06e4, 0x04a0);
	SET_REG(EPHY_REG_BASE + 0x0bbc, 0x1759);
	SET_REG(EPHY_REG_BASE + 0x0788, 0x000f);
	SET_REG(EPHY_REG_BASE + 0x0680, 0x0552);
	SET_REG(EPHY_REG_BASE + 0x06c8, 0xc000);
	SET_REG(EPHY_REG_BASE + 0x06e0, 0x8508);
	SET_REG(EPHY_REG_BASE + 0x1360, 0xff37);
	SET_REG(EPHY_REG_BASE + 0x076c, 0x0430);
	SET_REG(EPHY_REG_BASE + 0x0794, 0x9000);
	SET_REG(EPHY_REG_BASE +  0x2c, 0x03);

	SET_REG_M(EPHY_REG_BASE + 0x07f8, 3 << 8, 3 << 8);
#endif

	printf("skip internal phy config...done");

	return 0;
}

int auto_find_phy(void)
{
	int ret;
	struct udevice *dev;
	unsigned int cp_phy_train = 0;
	#ifndef CONFIG_MOLCHIP_FASTBOOT
	printf("auto find phy...\n");
	#endif
	/* retrieve the device */
	ret = uclass_get_device_by_driver(UCLASS_MISC,
						DM_GET_DRIVER(mc_efuse), &dev);

	if (ret) {
		debug("%s: could not find efuse device\n", __func__);
	}else {
		ret = misc_read(dev, EPHY_TRAIN_SET, &cp_phy_train, 1);
		if (ret) {
			train_data = 0;
			debug("%s: ephy train get error\n", __func__);
		}

		train_data = (cp_phy_train >> 16);
		#ifndef CONFIG_MOLCHIP_FASTBOOT
		printf("ephy train data: 0x%x\n", train_data);
		#endif
	}

	SET_REG_M(EPHY_REG_BASE + 0x1370, 0x0, 1<<3);
	SET_REG(EPHY_REG_BASE + 0x1274, 0x3c3);
	SET_REG(EPHY_REG_BASE + 0x0b6C, 0xf04);
	SET_REG(EPHY_REG_BASE + 0x1340, 0x0000);
	SET_REG(EPHY_REG_BASE + 0x0bA8, 0x20);
	SET_REG(EPHY_REG_BASE + 0x0018, 0x00c02800);

	//vgb set
	if(BIT(15) & train_data) {
		SET_REG_M(EPHY_REG_BASE + 0x0018, (((train_data & GENMASK(7,6)) >> 6) << 22), 3 << 22);
	}

	SET_REG(EPHY_REG_BASE + 0x001c, 0x00810000);
	SET_REG(EPHY_REG_BASE + 0x0020, 0x0000340c);
	SET_REG(EPHY_REG_BASE + 0x0024, 0x30131500);
	//10M trim
	if(BIT(13) & train_data) {
		SET_REG_M(EPHY_REG_BASE + 0x0024, (((train_data & GENMASK(12,8)) >> 8) << 16), 0x1f << 16);
	}
	//100M trim
	if(BIT(5) & train_data) {
		SET_REG_M(EPHY_REG_BASE + 0x0024, (((train_data & GENMASK(4,0)) >> 0) << 8), 0x1f << 8);
	}

	SET_REG(EPHY_REG_BASE + 0x0028, 0x01402800);
	SET_REG(EPHY_REG_BASE + 0x001c, 0xa1800000);
	SET_REG(EPHY_REG_BASE + 0x0028, 0x00402800);
	SET_REG(EPHY_REG_BASE + 0x0014, 0x06000000);
	SET_REG(EPHY_REG_BASE + 0x0020, 0x2340c);
	SET_REG(EPHY_REG_BASE + 0x000c, 0xba0edfff);
	SET_REG(EPHY_REG_BASE + 0x0010, 0x7fcbf9de);
	SET_REG(EPHY_REG_BASE + 0x0b98, 0xd150);
	SET_REG(EPHY_REG_BASE + 0x1224, 0);
	SET_REG(EPHY_REG_BASE + 0x12f0, 0);
	SET_REG(EPHY_REG_BASE + 0x0bc0, 0x7de);
	SET_REG(EPHY_REG_BASE + 0x0bc8, 0x09de);
	SET_REG(EPHY_REG_BASE + 0x070c, 0x0401);
	SET_REG(EPHY_REG_BASE + 0x0a84, 0x1d20);
	SET_REG(EPHY_REG_BASE + 0x06d4, 0x3130);
	SET_REG(EPHY_REG_BASE + 0x0a6c, 0x0818);
	SET_REG(EPHY_REG_BASE + 0x0aec, 0x1000);
	SET_REG(EPHY_REG_BASE + 0x0658, 0x1c00);
	SET_REG(EPHY_REG_BASE + 0x07d4, 0x6900);
	SET_REG(EPHY_REG_BASE + 0x06b8, 0x4);
	SET_REG(EPHY_REG_BASE + 0x06E4, 0x04a0);
	SET_REG(EPHY_REG_BASE + 0x0BBC, 0x1759);
	SET_REG(EPHY_REG_BASE + 0x0bf8, 0x0100);
	SET_REG(EPHY_REG_BASE + 0x0794, 0x9000);
	SET_REG(EPHY_REG_BASE + 0x0788, 0xf);
	SET_REG(EPHY_REG_BASE + 0x1378, 0x2338);
	SET_REG_M(EPHY_REG_BASE + 0x0, 0x20<<24, 0xff<<24);
	SET_REG_M(EPHY_REG_BASE + 0x4, 0x18, 0xff);
	SET_REG(EPHY_REG_BASE + 0x0680, 0x0552);
	SET_REG(EPHY_REG_BASE + 0x1274, 0);
	SET_REG(EPHY_REG_BASE + 0x0b6C, 0x704);
	SET_REG(EPHY_REG_BASE + 0x1340, 0);
	SET_REG(EPHY_REG_BASE + 0x0ba8, 0);
	SET_REG(EPHY_REG_BASE + 0x06c8, 0xc000);
	SET_REG(EPHY_REG_BASE + 0x06e0, 0x8508);
	SET_REG(EPHY_REG_BASE + 0x1328, 0x0065);
	SET_REG(EPHY_REG_BASE + 0x076c, 0x0430);
	SET_REG(EPHY_REG_BASE + 0x06b4, 0xfd00);
	SET_REG(EPHY_REG_BASE + 0x1360, 0xff37);
	SET_REG_M(EPHY_REG_BASE + 0x0778, 0x24 << 8, 0xff << 8);
	SET_REG_M(EPHY_REG_BASE + 0x077c, 0x24, 0xff);
	SET_REG(EPHY_REG_BASE + 0xb10, 0x5000);
	SET_REG(EPHY_REG_BASE + 0xb14, 0x8000);
	SET_REG(EPHY_REG_BASE + 0xb1c, 0x000e);
	SET_REG(EPHY_REG_BASE + 0xb20, 0x000e);
	SET_REG(EPHY_REG_BASE + 0xb1c, 0x000f);
	SET_REG(EPHY_REG_BASE + 0xb20, 0x0035);
	SET_REG(EPHY_REG_BASE + 0xb1c, 0x0000);
	SET_REG(EPHY_REG_BASE + 0xb10, 0x0000);
	SET_REG(EPHY_REG_BASE + 0xb14, 0x0000);

	SET_REG(EPHY_REG_BASE + 0x7ec, 0x8a6);
	SET_REG(EPHY_REG_BASE + 0x2c, 0x10);
	SET_REG(EPHY_REG_BASE + 0x07f8, 0x325);
	#ifndef CONFIG_MOLCHIP_FASTBOOT
	printf("phy config...done\n");
	#endif

	return 0;

}

int mochip_ephy_test(struct phy_device *phydev)
{

	printf("ephy test open ...\n");
	//10M full
	SET_REG(EPHY_REG_BASE + 0x600, 0x100);
	// force link up
	SET_REG(EPHY_REG_BASE + 0x680, 0x492);
	return 0;
}

static int mochip7c01_phy_reset(struct phy_device *phydev)
{
	debug("no mochip7c01_phy_reset\n");

	return 0;
}

static int mochip7c01_config(struct phy_device *phydev)
{
	debug("mochip7c01_config\n");
	if (mochip7c01_phy_config(phydev))
		return -1;

	return genphy_config(phydev);
}

int mochip7c01_startup(struct phy_device *phydev)
{
	int ret;
	debug("mochip7c01_startup\n");
	ret = genphy_update_link(phydev);
	if (ret)
		return ret;
	//phydev->speed = SPEED_100;
	//phydev->duplex = DUPLEX_FULL;
	return genphy_parse_link(phydev);
}


static struct phy_driver MOL7c01_driver = {
	.name = "Molchip FH Ethernet PHY",
	//.uid = 0x00177c01,
	.uid = 0x46480000,
	.mask = 0x001fffff,
	.features = MOLCHIP_PHY_FEATURES,
	.config = &mochip7c01_config,
	.startup = &mochip7c01_startup,
	.shutdown = &genphy_shutdown,
	.softreset = &mochip7c01_phy_reset,
};

int phy_molchip_init(void)
{
	phy_register(&MOL7c01_driver);

	return 0;
}

