/*
 * MOLCHIP USB PHY Support
 *
 * (C) Copyright 2021
 * MOLCHIP, <www.molchip.com>
 *
 * Author: Junbao Zhang <junbao.zhang@molchip.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <errno.h>
#include <asm/io.h>
#include <usb.h>
#include <asm/arch/hardware.h>

struct molchip_usb3_phy {
	bool init;
	void *cen_glb;
	void *vou_sys;
	void *hsif_sys_ahb;
	void *veu_sys_ahb;
};

struct molchip_usb3_phy mphy = {
	.init = false,
	.cen_glb = (void *)0x2c100000,
	.vou_sys = (void *)0x2c440000,
};

static void molchip_usb3_phy_cfg(struct molchip_usb3_phy *usb3)
{
	u32 val;

	val = readl(usb3->cen_glb + 0x4);
	val |= (1 << 0);
	val |= (1 << 1);
	val |= (1 << 7);
	val |= (1 << 11);
	writel(val, usb3->cen_glb + 0x4);  

	val = readl(usb3->vou_sys + 0x50);
	val |= (1 << 3); 
	val &= ~(0x7f << 6);
	val |= (0x7f << 6);
	val &= ~(0x3f << 13);
	val |= (0x1c << 13);
	val &= ~(0x3f << 19);
	val |= (0x1c << 19);  
	writel(val, usb3->vou_sys + 0x50);	

	val = readl(usb3->vou_sys + 0x4c);
	val &= ~(0x1f << 11);
	val |= (0x9 << 11);
	val &= ~(0x7 << 16);
	val |= (0x5 << 16);
	writel(val, usb3->vou_sys + 0x4c);	

	val = readl(usb3->vou_sys + 0x54);
	val &= ~(0x7 << 12);
	val |= (0x6 << 12);
	val &= ~(0x1 << 16);
	val |= (0x1 << 30);
	writel(val, usb3->vou_sys + 0x54);	

	val = readl(usb3->vou_sys + 0x48);
	val &= ~(0x3 << 24);
	val |= (0x3 << 24);
	val &= ~(0x3 << 22);
	val |= (0x3 << 22);
	val &= ~(0x3 << 17);
	val |= (0x3 << 17);
	val &= ~(0xf << 13);
	val |= (0x8 << 13);

	writel(val, usb3->vou_sys + 0x48);	

	mdelay(100);

}

void molchip_usb3_enable_phy(void)
{
	if(mphy.init == false){
		molchip_usb3_phy_cfg(&mphy);
		mphy.init = true;
	}
}

