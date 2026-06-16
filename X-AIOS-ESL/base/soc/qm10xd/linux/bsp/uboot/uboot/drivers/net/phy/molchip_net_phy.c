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


struct molchip_phy_reg {
	u32 reg;
	u16	val;
};

struct molchip_phy_reg config_reg[MOLCHIP_PHY_CONFIG_NUM] = {
	{0x1f, 0x0},//select page 0
	{0x14, 0x2000},	//enable Auto-MDIX
	{0x1f, 0x2},	//select page 2
	{0x11, 0x8059},
	{0x12, 0x8975},
	{0x13, 0xba60},
	{0x1f, 0x4},	//select page 4
	{0x12, 0x5a40},
	{0x1f, 0x1},	//select page 1
	{0x10, 0x7560},
	//{0x10, 0xf958},//saving mode
	{0x11, 0xA528},
	{0x13, 0xA448},
	{0x14, 0x3780},
	{0x15, 0xB600},
	{0x16, 0xC800},
	{0x1f, 0x2},	//select page 2
	{0x16, 0x6F00},	//0
	{0x16, 0x6F8C},	//1
	{0x16, 0x7018},	//2
	{0x16, 0x70A2},	//3
	{0x16, 0x712A},	//4
	{0x16, 0x71AA},	//5
	{0x16, 0x722A},	//6
	{0x16, 0x72A2},	//7
	{0x16, 0x7318},	//8
	{0x16, 0x738C},	//9
	{0x16, 0x7400},	//10
	{0x16, 0x748C},	//11
	{0x16, 0x7518},	//12
	{0x16, 0x75A2},	//13
	{0x16, 0x762A},	//14
	{0x16, 0x76A2},	//15
	{0x16, 0x7722},	//16
	{0x16, 0x77A2},	//17
	{0x16, 0x7822},	//18
	{0x16, 0x78A2},	//19
	{0x16, 0x7922},	//20
	{0x16, 0x79A2},	//21
	{0x16, 0x7A22},	//22
	{0x16, 0x7AA2},	//23
	{0x16, 0x7B22},	//24
	{0x16, 0x7B94},	//25
	{0x16, 0x7C08},	//26
	{0x16, 0x7C84},	//27
	{0x16, 0x7D00},	//28
	{0x16, 0x7D80},	//29
	{0x16, 0x5D80},	//30
};

static int mochip7c01_phy_config(struct phy_device *phydev)
{
	int reg, cnt;

	phy_write(phydev, MDIO_DEVAD_NONE, MOLCHIP_MII_PHYPAGE, 0);
	reg = phy_read(phydev, MDIO_DEVAD_NONE, MOLCHIP_MII_BMCR1);
	if (phydev->interface == PHY_INTERFACE_MODE_RMII)
		reg |= MOLCHIP_PHY_CTL_REG_RMII | MOLCHIP_PHY_CTL_REG_FIFO32;
	else if (phydev->interface == PHY_INTERFACE_MODE_MII)
		reg &= (~MOLCHIP_PHY_CTL_REG_RMII);
	else {
		debug("Unknown PHY mode %d \n", phydev->interface);
		return -1;
	}
	reg |= MOLCHIP_PHY_CTL_REG_LED_MODE1;
	reg &= ~(MOLCHIP_PHY_CTL_REG_LED_MODE);

	phy_write(phydev, MDIO_DEVAD_NONE, MOLCHIP_MII_BMCR1, reg);

	for (cnt = 0; cnt < MOLCHIP_PHY_CONFIG_NUM; cnt++) {
		phy_write(phydev, MDIO_DEVAD_NONE, config_reg[cnt].reg, config_reg[cnt].val);
#if (MOLCHIP_PHY_DEBUG == 1)
		reg = phy_read(phydev, MDIO_DEVAD_NONE, config_reg[cnt].reg);
		if (reg != config_reg[cnt].val)
			debug("phy wr reg %x %x, %x\n",
			config_reg[cnt].reg, config_reg[cnt].val, reg);
#endif
	}
	phy_write(phydev, MDIO_DEVAD_NONE, MOLCHIP_MII_PHYPAGE, 0);

	return 0;
}

static int mochip7c01_phy_reset(struct phy_device *phydev)
{
	int reg;
	int timeout = 500;
	int devad = MDIO_DEVAD_NONE;

	if (phydev->flags & PHY_FLAG_BROKEN_RESET)
		return 0;

	if (phy_write(phydev, MDIO_DEVAD_NONE, MOLCHIP_MII_PHYPAGE, 0) < 0) {
		debug("PHY mochip7c01 reset page failed\n");
		return -1;
	}

	if (phy_write(phydev, devad, MII_BMCR, BMCR_RESET | BMCR_ANENABLE) < 0) {
		debug("PHY mochip7c01 reset failed\n");
		return -1;
	}

	reg = phy_read(phydev, devad, MII_BMCR);
	while ((reg & BMCR_RESET) && timeout--) {
		reg = phy_read(phydev, devad, MII_BMCR);

		if (reg < 0) {
			debug("PHY mochip7c01 status read failed\n");
			return -1;
		}
		udelay(1000);
	}

	if (reg & BMCR_RESET) {
		puts("PHY mochip7c01 reset timed out\n");
		return -1;
	}

	mdelay(100);

	return 0;
}

static int mochip7c01_config(struct phy_device *phydev)
{
	if (mochip7c01_phy_config(phydev))
		return -1;

	if (mochip7c01_phy_reset(phydev))
		return -1;

	return genphy_config(phydev);
}

int mochip7c01_parse_link(struct phy_device *phydev)
{
	int mii_reg = phy_read(phydev, MDIO_DEVAD_NONE, MII_BMSR);
	u32 ctl_reg = 0;
	u32 i=0,reg;

	/* We're using autonegotiation */
	if (phydev->autoneg == AUTONEG_ENABLE) {
		u32 lpa = 0;
		int gblpa = 0;
		u32 estatus = 0;

		/* Check for gigabit capability */
		if (phydev->supported & (SUPPORTED_1000baseT_Full |
					SUPPORTED_1000baseT_Half)) {
			/* We want a list of states supported by
			 * both PHYs in the link
			 */
			gblpa = phy_read(phydev, MDIO_DEVAD_NONE, MII_STAT1000);
			if (gblpa < 0) {
				debug("Could not read MII_STAT1000. Ignoring gigabit capability\n");
				gblpa = 0;
			}
			gblpa &= phy_read(phydev,
					MDIO_DEVAD_NONE, MII_CTRL1000) << 2;
		}

		/* Set the baseline so we only have to set them
		 * if they're different
		 */
		phydev->speed = SPEED_10;
		phydev->duplex = DUPLEX_HALF;

		/* Check the gigabit fields */
		if (gblpa & (PHY_1000BTSR_1000FD | PHY_1000BTSR_1000HD)) {
			phydev->speed = SPEED_1000;

			if (gblpa & PHY_1000BTSR_1000FD)
				phydev->duplex = DUPLEX_FULL;

			/* We're done! */
			return 0;
		}

		lpa = phy_read(phydev, MDIO_DEVAD_NONE, MII_ADVERTISE);
		lpa &= phy_read(phydev, MDIO_DEVAD_NONE, MII_LPA);

		if (lpa & (LPA_100FULL | LPA_100HALF)) {
			phydev->speed = SPEED_100;

			i=0;
			while(1) {
				phy_write(phydev, MDIO_DEVAD_NONE,MOLCHIP_MII_PHYPAGE, 4);
				ctl_reg = phy_read(phydev, MDIO_DEVAD_NONE, MOLCHIP_PHY_PCS_STATUS);
				ctl_reg = phy_read(phydev, MDIO_DEVAD_NONE, MOLCHIP_PHY_PCS_STATUS);

				if(!(ctl_reg & MOLCHIP_PHY_CTL_SEED)) {
					if(mochip7c01_phy_reset(phydev)) {
						printf("molchip ephy reset failed!\n");
					}

					/* Reset Ephy CLK */
					reg = ANA_EPHY_BASE_REG + 0x90;
					writel((readl((unsigned long)reg) & (~(1 << 11))), (unsigned long)reg);
					mdelay(100);
					writel(readl((unsigned long)reg) | (1 << 11), (unsigned long)reg);
					mdelay(20);
					i++;

					mdelay(1000);
				} else {
					break;
				}

				if(i > 10) {
					printf("Seed TIMEOUT !\n");
					phydev->link = 0;
					phy_write(phydev, MDIO_DEVAD_NONE, MOLCHIP_MII_PHYPAGE, 0);
					return -ETIMEDOUT;
				}

			}

			phy_write(phydev, MDIO_DEVAD_NONE, MOLCHIP_MII_PHYPAGE, 0);

			if (lpa & LPA_100FULL)
				phydev->duplex = DUPLEX_FULL;
		} else if (lpa & LPA_10FULL)
			phydev->duplex = DUPLEX_FULL;

		/*
		 * Extended status may indicate that the PHY supports
		 * 1000BASE-T/X even though the 1000BASE-T registers
		 * are missing. In this case we can't tell whether the
		 * peer also supports it, so we only check extended
		 * status if the 1000BASE-T registers are actually
		 * missing.
		 */
		if ((mii_reg & BMSR_ESTATEN) && !(mii_reg & BMSR_ERCAP))
			estatus = phy_read(phydev, MDIO_DEVAD_NONE,
					   MII_ESTATUS);

		if (estatus & (ESTATUS_1000_XFULL | ESTATUS_1000_XHALF |
				ESTATUS_1000_TFULL | ESTATUS_1000_THALF)) {
			phydev->speed = SPEED_1000;
			if (estatus & (ESTATUS_1000_XFULL | ESTATUS_1000_TFULL))
				phydev->duplex = DUPLEX_FULL;
		}

	} else {
		u32 bmcr = phy_read(phydev, MDIO_DEVAD_NONE, MII_BMCR);

		phydev->speed = SPEED_10;
		phydev->duplex = DUPLEX_HALF;

		if (bmcr & BMCR_FULLDPLX)
			phydev->duplex = DUPLEX_FULL;

		if (bmcr & BMCR_SPEED1000)
			phydev->speed = SPEED_1000;
		else if (bmcr & BMCR_SPEED100)
			phydev->speed = SPEED_100;
	}

	return 0;
}


int mochip7c01_startup(struct phy_device *phydev)
{
	int ret;

	ret = genphy_update_link(phydev);
	if (ret)
		return ret;

	return mochip7c01_parse_link(phydev);
}


static struct phy_driver MOL7c01_driver = {
	.name = "Molchip Ethernet PHY",
	.uid = 0x00177c01,
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

