/*
 * Copyright (C) 2020 Jason Qin <jason.qin@molchip.com>.
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

#ifndef _PHY_MOLCHIP_USB_H_
#define _PHY_MOLCHIP_USB_H_

enum molchip_phy_id {
	MOL_PHY_ID_0,
	MOL_PHY_ID_1,
	MOL_PHY_ID_NUM
};

struct molchip_usb_phy {
	struct device *dev;
	struct phy *phy;
	void __iomem *baseaddr;
	struct regmap *usb2_apb_cfg;
	enum phy_mode mode[MOL_PHY_ID_NUM];
	u32 c_phy_id;
	spinlock_t lock;
};

#define MC_PHY_ID(mode)	(((mode) >> 8) & 0xff)
#define MC_PHY_MOD(mode)	((mode) & 0xff)

#endif
