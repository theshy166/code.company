#include "mc_vou_osal.h"
#include "mc_lcm_drv.h"

extern vou_sync_info_t sync_info_t;

void lcm_bus_pin_cfg(void)
{
	writel(0x00000000, 0x102000bc);
	writel(0x00000000, 0x102000c0);
	writel(0x00000000, 0x102000c4);
	writel(0x00000000, 0x102000c8);
	writel(0x00000000, 0x102000cc);
	writel(0x00000000, 0x102000d0);
	writel(0x00000000, 0x102000d4);
	writel(0x00000000, 0x102000d8);
	writel(0x00000000, 0x102000dc);
	writel(0x00000000, 0x102000e0);
	writel(0x00000000, 0x102000e4);
	writel(0x00000000, 0x102000e8);
}

void lcm_clk_on(void)
{
	writel(0xc3b0ea00, 0x24000308);
	writel(0x10000000, 0x24000404);
	writel(0x10000000, 0x24000404);
	writel(0x00000000, 0x24000404);
	writel(0x00000000, 0x24000404);
	writel(0xc3b4ea00, 0x24000308);
	writel(0x02000000, 0x24000404);
	writel(0x02000000, 0x24000404);
	writel(0x00000000, 0x24000404);
	writel(0x00000000, 0x24000404);
	writel(0xc3bcea00, 0x24000308);
	writel(0x01000000, 0x24000404);
	writel(0x01000000, 0x24000404);
	writel(0x00000000, 0x24000404);
	writel(0x00000000, 0x24000404);
}

void dsi_host_power_on(void)
{
	writelr(0x00000000, 0x25c000a0, 2, 1);
	writelr(0x00000001, 0x25c000b4, 0, 1);
	writelr(0x00000001, 0x25c000a0, 3, 1);
	writelr(0x00000001, 0x25c000b4, 1, 1);
	writelr(0x00000000, 0x25c000b8, 16, 1);
	writelr(0x00000001, 0x25c000a0, 0, 1);
	writelr(0x00000001, 0x25c000a0, 1, 1);
	writelr(0x00000000, 0x25c000b4, 0, 1);
}

void _dsi_power_on(void)
{
	lcm_clk_on();
	dsi_host_power_on();
}

void _dsi_phy_v0_cfg(void)
{
	uint32_t pll_lock[2] = {0};

	writelr(0x00000001, 0x25d00070, 6, 1);
	writelr(0x00000000, 0x25d00070, 17, 1);
	writelr(0x00000000, 0x25d00070, 16, 1);
	writelr(0x00000000, 0x25d00070, 17, 1);
	writelr(0x00000000, 0x25d00070, 16, 1);
	writelr(0x00000000, 0x25d00070, 15, 1);
	writelr(0x00000000, 0x25d00070, 13, 1);
	writelr(0x00000000, 0x25d00070, 14, 1);
	writelr(0x00000000, 0x25d00070, 12, 1);
	writelr(0x00000000, 0x25d00070, 11, 1);

	writelr(0x00000008, 0x25d00088, 2, 4);
	writelr(0x00000001, 0x25d00088, 8, 1);
	writelr(0x00000001, 0x25d00088, 7, 1);

	writelr(0x00000001, 0x25d00074, 25, 1);
	writelr(0x00000001, 0x25d00074, 25, 1);
	writelr(0x00000001, 0x25d00374, 25, 1);
	writelr(0x00000001, 0x25d00374, 25, 1);

	writelr(0x00000006, 0x25d0008c, 14, 3);
	writelr(0x00000006, 0x25d00090, 14, 3);
	writelr(0x00000003, 0x25d0008c, 11, 3);
	writelr(0x00000003, 0x25d00090, 11, 3);
	writelr(0x00000003, 0x25d0008c, 8, 3);
	writelr(0x00000003, 0x25d00090, 8, 3);

	writelr(0x00000001, 0x25d00360, 0, 1); //link mode

	//pll cfg
	writel(0x32000000, 0x25d00074);
	writel(0x32000000, 0x25d00374);
	writel(0x10000000, 0x25d00078);
	writel(0x10000000, 0x25d00378);
	writel(0x00000000, 0x25d00080);
	writel(0x00000000, 0x25d00380);
	writel(0x11000000, 0x25d00078);
	writel(0x11000000, 0x25d00378);
	writel(0xac083000, 0x25d0007c);
	writel(0xac083000, 0x25d0037c);
	writel(0xac08302a, 0x25d0007c);
	writel(0xac08302a, 0x25d0037c);

	//dhd div
	writel(0x00001001, 0x26000004);

	writelr(0x00000001, 0x25d00098, 10, 1);
	writelr(0x00000001, 0x25d00398, 10, 1);
	writelr(0x00000001, 0x25d00098, 11, 1);
	writelr(0x00000001, 0x25d00398, 11, 1);
	writelr(0x00000001, 0x25d00098, 12, 1);
	writelr(0x00000001, 0x25d00398, 12, 1);

	//clk lane
	writel(0x000001f4, 0x25d00000);
	writel(0x00000000, 0x25d00300);
	writel(0x00000000, 0x25d00004);
	writel(0x00000000, 0x25d00304);
	writel(0x00010000, 0x25d00008);
	writel(0x00000000, 0x25d00308);
	writel(0x00012200, 0x25d00008);
	writel(0x00000000, 0x25d00308);
	writel(0x00012205, 0x25d00008);
	writel(0x00000000, 0x25d00308);
	writel(0x000b0000, 0x25d0000c);
	writel(0x00000000, 0x25d0030c);
	writel(0x000b0400, 0x25d0000c);
	writel(0x00000000, 0x25d0030c);
	writel(0x000b0406, 0x25d0000c);
	writel(0x00000000, 0x25d0030c);
	writel(0x00000100, 0x25d0005c);
	writel(0x00000000, 0x25d0035c);

	//data lane
	writel(0x0000006c, 0x25d00010);
	writel(0x0000006c, 0x25d00024);
	writel(0x0000006c, 0x25d00034);
	writel(0x0000006c, 0x25d00044);
	writel(0x00000000, 0x25d00014);
	writel(0x00000000, 0x25d00028);
	writel(0x00000000, 0x25d00038);
	writel(0x00000000, 0x25d00048);
	writel(0x01000000, 0x25d00018);
	writel(0x01000000, 0x25d0002c);
	writel(0x01000000, 0x25d0003c);
	writel(0x01000000, 0x25d0004c);
	writel(0x010f0000, 0x25d00018);
	writel(0x010f0000, 0x25d0002c);
	writel(0x010f0000, 0x25d0003c);
	writel(0x010f0000, 0x25d0004c);
	writel(0x010f0500, 0x25d00018);
	writel(0x010f0500, 0x25d0002c);
	writel(0x010f0500, 0x25d0003c);
	writel(0x010f0500, 0x25d0004c);
	writel(0x010f0506, 0x25d00018);
	writel(0x010f0506, 0x25d0002c);
	writel(0x010f0506, 0x25d0003c);
	writel(0x010f0506, 0x25d0004c);
	writel(0x0000d50c, 0x25d0001c);
	writel(0x0000d50c, 0x25d00030);
	writel(0x0000d50c, 0x25d00040);
	writel(0x0000d50c, 0x25d00050);
	writel(0x001b0000, 0x25d00020);
	writel(0x001b0000, 0x25d00340);
	writel(0x001b0b00, 0x25d00020);
	writel(0x001b0b00, 0x25d00340);
	writel(0x001b0b22, 0x25d00020);
	writel(0x001b0b22, 0x25d00340);
	writel(0x00000006, 0x25d00054);

	writelr(0x00000001, 0x25d00058, 24, 1);
	writelr(0x00000003, 0x25d00058, 5, 2);
	writelr(0x00000001, 0x25d00058, 25, 1);
	writelr(0x00000000, 0x25d00058, 26, 1);
	writelr(0x00000000, 0x25d00058, 0, 5);
	writelr(0x0000001f, 0x25d00058, 0, 5);
	writelr(0x0000001f, 0x25d00358, 0, 5);

	writel(0x00000800, 0x25d00060);
	writel(0x00000802, 0x25d00060);

	writelr(0x00000001, 0x25d00070, 14, 1);
	writelr(0x00000001, 0x25d00070, 14, 1);
	writelr(0x00000001, 0x25d00070, 12, 1);
	writelr(0x00000001, 0x25d00070, 12, 1);
	udelay(1);
	writelr(0x00000001, 0x25d00070, 15, 1);
	writelr(0x00000001, 0x25d00070, 15, 1);
	writelr(0x00000001, 0x25d00070, 13, 1);
	writelr(0x00000001, 0x25d00070, 13, 1);
	udelay(100);
	writelr(0x00000000, 0x25d00074, 25, 1);
	writelr(0x00000000, 0x25d00074, 25, 1);
	writelr(0x00000000, 0x25d00374, 25, 1);
	writelr(0x00000000, 0x25d00374, 25, 1);

	do {
		pll_lock[0] = readl(0x25d00080);
		pll_lock[1] = readl(0x25d00380);

		if ((1 == (pll_lock[0] & 0x1)) && (1 == (pll_lock[1] & 0x1))) {
			#ifndef CONFIG_MOLCHIP_FASTBOOT
			printf("f pll lock success !\n");
			#endif
			break;
		}
	} while (1);

	writel(0x10000000, 0x25d00064);
	writel(0x10000000, 0x25d00064);
	writel(0x00000000, 0x25d00064);
	writel(0x00000000, 0x25d00064);
	writel(0x80000000, 0x25d00064);
	writel(0x80000000, 0x25d00064);
	writel(0x00000000, 0x25d00064);
	writel(0x00000000, 0x25d00064);

	writel(0x0300007f, 0x25d00058);
	writel(0x00000000, 0x25d00064);
	writel(0x00a00000, 0x25d00068);
	writel(0x00a05000, 0x25d00068);
	writel(0x00a05000, 0x25d00068);
	writel(0x00a05000, 0x25d00068);
	writel(0x00000000, 0x25d0006c);
	writel(0x00000000, 0x25d000f0);
	writel(0x00000000, 0x25d000f8);
	writel(0x00000000, 0x25d000f8);
	writel(0x0000f040, 0x25d00070);
	writel(0x0000f040, 0x25d00070);
	writel(0x0000f0c0, 0x25d00070);
	writel(0x0000f0c0, 0x25d00070);
	writel(0x0000f0ce, 0x25d00070);
	writel(0x00000000, 0x25d00084);
	writel(0x00000000, 0x25d00084);
	writel(0x00000000, 0x25d00084);
	writel(0x4e2001a0, 0x25d00088);
	writel(0x4e2001a0, 0x25d00088);
	writel(0x4e2001a0, 0x25d00088);
	writel(0x00019800, 0x25d0008c);
	writel(0x00019800, 0x25d0008c);
	writel(0x00019820, 0x25d0008c);
	writel(0x00019822, 0x25d0008c);
	writel(0x00001000, 0x25d000a0);
	writel(0x00000000, 0x25d000e4);
	writel(0x0300007f, 0x25d00058);
	writel(0x0300007f, 0x25d00058);
	writel(0x00001c00, 0x25d00098);
	writel(0x00001c00, 0x25d00098);
	writel(0x00001c00, 0x25d00398);
	writel(0x00000000, 0x25d0009c);
	writel(0x00001c00, 0x25d00098);
	writel(0x00001c00, 0x25d00398);
	writel(0x00000000, 0x25d0009c);
	writel(0x00001c00, 0x25d00098);
	writel(0x00001c00, 0x25d00398);
	writel(0x00000000, 0x25d0009c);
	writel(0x0000f0ce, 0x25d00070);
	writel(0x00071000, 0x25d000a0);
	writel(0x00000000, 0x25d000a4);
	writel(0x000003e8, 0x25d000a4);
	writel(0x03e803e8, 0x25d000a8);
	writel(0x00000000, 0x25d000ac);
	writel(0x00000000, 0x25d003ac);
	writel(0x0f0f0f00, 0x25d000b0);
	writel(0x00000000, 0x25d000b4);
	writel(0x00000101, 0x25d000b0);
	writel(0x01000000, 0x25d000b4);
	writel(0x00000000, 0x25d003ac);
	writel(0x6300037f, 0x25d00058);
	writel(0x00000000, 0x25d00064);
	writel(0x70000000, 0x25d00074);
	writel(0x70000000, 0x25d00374);
	writel(0x70000000, 0x25d00074);
	writel(0x70000000, 0x25d00374);
	writel(0x70000000, 0x25d00074);
	writel(0x70000000, 0x25d00374);
	writel(0x70000000, 0x25d00074);
	writel(0x70000000, 0x25d00374);
	writel(0x11000000, 0x25d00078);
	writel(0x11000000, 0x25d00378);
	writel(0x11000000, 0x25d00078);
	writel(0x11000000, 0x25d00378);
	writel(0x11000000, 0x25d00078);
	writel(0x11000000, 0x25d00378);
	writel(0x11000800, 0x25d00078);
	writel(0x11000800, 0x25d00378);
	writel(0x11000a00, 0x25d00078);
	writel(0x11000a00, 0x25d00378);
	writel(0x11000b00, 0x25d00078);
	writel(0x11000b00, 0x25d00378);
	writel(0x11000b40, 0x25d00078);
	writel(0x11000b40, 0x25d00378);
	writel(0x11000b40, 0x25d00078);
	writel(0x11000b40, 0x25d00378);
	writel(0x11000b44, 0x25d00078);
	writel(0x11000b44, 0x25d00378);
	writel(0x11000b46, 0x25d00078);
	writel(0x11000b46, 0x25d00378);
	writel(0x00000801, 0x25d00080);
	writel(0x00000801, 0x25d00380);
	writel(0x00000801, 0x25d00080);
	writel(0x00000801, 0x25d00380);
	writel(0x00000000, 0x25d000e0);
	writel(0x00000000, 0x25d003e0);
	writel(0x00000000, 0x25d000e0);
	writel(0x00000000, 0x25d003e0);
	writel(0x00000000, 0x25d000e4);
	writel(0x00000000, 0x25d003e4);
	writel(0x000186a0, 0x25d000e4);
	writel(0x000186a0, 0x25d003e4);
	writel(0x00000000, 0x25d000e8);
	writel(0x00000000, 0x25d003e8);
	writel(0x00019822, 0x25d0008c);
	writel(0x01980000, 0x25d00090);
	writel(0x00001c00, 0x25d00098);
	writel(0x00001c00, 0x25d00398);
	writel(0x00001c00, 0x25d00098);
	writel(0x00001c00, 0x25d00398);
	writel(0x00000000, 0x25d000f4);
	writel(0x00000000, 0x25d003f4);
	writel(0x00000000, 0x25d000f4);
	writel(0x00000000, 0x25d003f4);
	writel(0x00000000, 0x25d000f4);
	writel(0x00000000, 0x25d003f4);
	writel(0x00000000, 0x25d000f4);
	writel(0x00000000, 0x25d003f4);
	writel(0x00000000, 0x25d000f4);
	writel(0x00000000, 0x25d003f4);
	writel(0x000aa000, 0x25d000f4);
	writel(0x000aa000, 0x25d003f4);
	writel(0x000ab800, 0x25d000f4);
	writel(0x000ab800, 0x25d003f4);
	writel(0x000ab800, 0x25d000f4);
	writel(0x000ab800, 0x25d003f4);
	writel(0x000ab800, 0x25d000f4);
	writel(0x000ab800, 0x25d003f4);
	writel(0x000ab80f, 0x25d000f4);
	writel(0x000ab80f, 0x25d003f4);

	do {
		pll_lock[0] = readl(0x25d00080);
		pll_lock[1] = readl(0x25d00380);

		if ((1 == (pll_lock[0] & 0x1)) && (1 == (pll_lock[1] & 0x1))) {
			#ifndef CONFIG_MOLCHIP_FASTBOOT
			printf("s pll lock success !\n");
			#endif
			break;
		}
	} while (1);
}

void dsi_phy_v0_init(void)
{
	_dsi_phy_v0_cfg();
}

void _dsi_host_v0_cfg(void)
{
	uint32_t clkdiv = 0;

	writelr(0x00000000, 0x25c0000c, 0, 2);
	writelr(0x00000005, 0x25c00010, 0, 4);
	writelr(0x00000001, 0x25c00014, 2, 1);
	writelr(0x00000001, 0x25c00014, 1, 1);
	writelr(0x0000000c, 0x25c00008, 0, 8);
	writelr(0x00000001, 0x25c00008, 8, 8);
	writel(0x10f7f01, 0x25c00068);
	writelr(0x00000001, 0x25c0001c, 0, 2);
	writelr(0x00000004, 0x25c00020, 0, 4);
	writelr(0x00000001, 0x25c00020, 16, 2);
	writelr(0x00000001, 0x25c00024, 0, 1);
//	writelr(0x00000064, 0x25c00028, 16, 16);
	writelr(0x00000008, 0x25c00028, 0, 16);
	writelr(0x00000001, 0x25c0002c, 2, 1);
	writelr(0x00000001, 0x25c0002c, 3, 1);
	writelr(0x00000001, 0x25c0002c, 4, 1);
	writelr(0x00000000, 0x25c00030, 0, 2);
	writelr(0x00000001, 0x25c00034, 0, 1);
	writelr(0x00000002, 0x25c00038, 0, 2);
	writelr(0x00000000, 0x25c00038, 8, 1);
	writelr(0x00000001, 0x25c00038, 9, 1);
	writelr(0x00000000, 0x25c00038, 10, 1);
	writelr(0x00000000, 0x25c00038, 11, 1);
	writelr(0x00000000, 0x25c00038, 12, 1);
	writelr(0x00000000, 0x25c00038, 13, 1);
	writelr(sync_info_t.Hactvie , 0x25c0003c, 0, 14);//writelr(0x00000400, 0x25c0003c, 0, 14);
	writelr(0x00000001, 0x25c00040, 0, 13);
	writelr(0x0000000a, 0x25c00044, 0, 13);

	clkdiv = 6;

	writelr((sync_info_t.Hsync * clkdiv / 8), 0x25c00048, 0, 12);//writelr(0x0000002d, 0x25c00048, 0, 12);
	writelr((sync_info_t.Hback * clkdiv / 8), 0x25c0004c, 0, 12);//writelr(0x0000009d, 0x25c0004c, 0, 12);
	writelr(((sync_info_t.Hsync + sync_info_t.Hback + sync_info_t.Hfront + sync_info_t.Hactvie) * clkdiv / 8), 0x25c00050, 0, 15);//writelr(0x000005e8, 0x25c00050, 0, 15);
	writelr(sync_info_t.Vsync, 0x25c00054, 0, 10);//writelr(0x00000002, 0x25c00054, 0, 10);
	writelr(sync_info_t.Vback, 0x25c00058, 0, 10);//writelr(0x00000017, 0x25c00058, 0, 10);
	writelr(sync_info_t.Vfront, 0x25c0005c, 0, 10);//writelr(0x0000000c, 0x25c0005c, 0, 10);
	writelr(sync_info_t.Vactive, 0x25c00060, 0, 14);//writelr(0x00000258, 0x25c00060, 0, 14);
	writelr(0x00000000, 0x25c00018, 16, 8);
	writelr(0x00000000, 0x25c00018, 0, 8);
	writelr(0x0000006e, 0x25c0009c, 0, 15);
	writelr(0x00000025, 0x25c0009c, 16, 8);
	writelr(0x00000000, 0x25c00064, 0, 16);
	writelr(0x00000001, 0x25c00094, 0, 1);
	writelr(0x00000000, 0x25c00078, 0, 16);
	writelr(0x00000000, 0x25c00078, 16, 16);
	writelr(0x00000000, 0x25c0007c, 0, 16);
	writelr(0x00000000, 0x25c00080, 0, 16);
	writelr(0x00000000, 0x25c00084, 0, 16);
	writel(0x00000000, 0x25c00084);
	writelr(0x00000000, 0x25c00088, 0, 16);
//	writelr(0x00000000, 0x25c0008c, 0, 16);
	writel(0x00000008, 0x25c0008c);
	writelr(0x00000087, 0x25c00098, 0, 10);
	writelr(0x00000025, 0x25c00098, 16, 10);
//	writelr(0x00000000, 0x25c000f4, 0, 15);
	writelr(0x00000028, 0x25c000a4, 8, 8);
	writelr(0x00000003, 0x25c000a4, 0, 2);
	writelr(0x00000000, 0x25c000a8, 0, 4);
	writelr(0x00000000, 0x25c000ac, 0, 4);
	writelr(0x00000000, 0x25c00100, 0, 1);
	writelr(0x00000000, 0x25c00100, 8, 1);
	writelr(0x00000000, 0x25c00100, 16, 1);
	writel(0xffffffff, 0x25c000c4);
	writel(0xffffffff, 0x25c000c8);
	writelr(0x00000000, 0x25c00004, 0, 1);
	writelr(0x00000001, 0x25c00004, 0, 1);
}

void dsi_host_v0_init(void)
{
	_dsi_host_v0_cfg();
}

void _mipi_bus_hs_cfg(void)
{
	writel(0x00000000, 0x25c00068);
	writel(0x00000014, 0x25c0002c);
	writel(0x00000000, 0x25c00034);
}

void _lcm_mipi_ek79007_init(void)
{
	writelr(0x00478015, 0x25c0006c, 0, 24);
	writelr(0x00408115, 0x25c0006c, 0, 24);
	writelr(0x00048215, 0x25c0006c, 0, 24);
	writelr(0x00778315, 0x25c0006c, 0, 24);
	writelr(0x000f8415, 0x25c0006c, 0, 24);
	writelr(0x00708515, 0x25c0006c, 0, 24);
	writelr(0x00708615, 0x25c0006c, 0, 24);
	mdelay(20);
	writelr(0x00001115, 0x25c0006c, 0, 24);
	mdelay(20);
	writelr(0x00002915, 0x25c0006c, 0, 24);
	mdelay(20);
}

void mol_lcm_init(vou_dev_idx_e devId)
{
	if (devId == VOU_DEV_IDX_DHD) {
		//push table
		_lcm_mipi_ek79007_init();
		_mipi_bus_hs_cfg();
	}
}

void lcm_func_init(vou_dev_idx_e devId)
{
	mol_lcm_init(devId);
}

void dsi_init(void)
{
	lcm_bus_pin_cfg();
	_dsi_power_on();
	dsi_phy_v0_init();
	dsi_host_v0_init();
}

void rgb_lcm_bus_pin_cfg(void)
{
	writel(0x00000000,0x10200064);
	writel(0x00000000,0x10200068);
	writel(0x00000000,0x1020006c);
	writel(0x00000000,0x10200070);
	writel(0x00000000,0x10200074);
	writel(0x00000000,0x10200078);
	writel(0x00000000,0x1020007c);
	writel(0x00000000,0x10200080);
	writel(0x00000000,0x10200084);
	writel(0x00000000,0x10200088);
	writel(0x00000000,0x1020008c);
	writel(0x00000000,0x10200090);
	writel(0x00000000,0x10200094);
	writel(0x00000000,0x10200098);
	writel(0x00000000,0x1020009c);
	writel(0x00000000,0x102000a0);
	writel(0x00000000,0x102000a4);
	writel(0x00000000,0x102000a8);
	writel(0x00000000,0x102000ac);
	writel(0x00000000,0x102000b0);
	writel(0x00000000,0x102000b4);
	writel(0x00000000,0x102000b8);
}

void _dsi_phy_v1_cfg(void)
{
	uint32_t pll_lock;

	writelr(0x00000001, 0x25d00070, 6, 1);
	writelr(0x00000007, 0x25d00070, 1, 4);
	writelr(0x00000000, 0x25d00070, 11, 1);
	writelr(0x00000001, 0x25d00074, 30, 1);
	writelr(0x00000001, 0x25d00374, 30, 1);

	writelr(0x00000001, 0x25d00374, 25, 1);
	writelr(0x00000001, 0x25d00374, 25, 1);//delay
	writelr(0x00000000,0x25d00058, 22, 2);
	writelr(0x00000000,0x25d00058, 20, 2);
	writelr(0x00000000,0x25d00058, 18, 2);
	writelr(0x00000000,0x25d00058, 16, 2);
	writelr(0x00000000,0x25d00058, 14, 2);
	writelr(0x00000000,0x25d00058, 12, 2);

	writelr(0x00000001,0x25d001ec, 8, 2);
	writelr(0x00000001,0x25d001ec, 21, 2);
	writelr(0x00000001,0x25d00098, 12, 1);
	writelr(0x00000000,0x25d00398, 12, 1);
	writelr(0x00000001,0x25d00098, 11, 1);
	writelr(0x00000001,0x25d00398, 11, 1);
	writelr(0x00000001,0x25d00098, 10, 1);
	writelr(0x00000001,0x25d00398, 10, 1);

	writelr(0x00000000,0x25d00070, 17, 1);
	writelr(0x00000000,0x25d00070, 16, 1);
	writelr(0x00000000,0x25d00070, 17, 1);//delay
	writelr(0x00000000,0x25d00070, 16, 1);//delay
	writelr(0x00000000,0x25d00070, 17, 1);
	writelr(0x00000000,0x25d00070, 16, 1);
	writelr(0x00000000,0x25d00070, 17, 1);//delay
	writelr(0x00000000,0x25d00070, 16, 1);//delay
	writelr(0x00000000,0x25d00070, 13, 1);
	writelr(0x00000000,0x25d00070, 13, 1);//delay
	writelr(0x00000000,0x25d00070, 12, 1);
	writelr(0x00000000,0x25d00070, 12, 1);//delay
	writelr(0x0000000c,0x25d00074, 26, 4);
	writelr(0x00000000,0x25d00374, 26, 4);

#if 1
	writelr(0x00000001,0x25d00078, 28, 1);
	writelr(0x00000001,0x25d00378, 28, 1);
	writelr(0x00000000,0x25d00078, 27, 1);
	writelr(0x00000000,0x25d00378, 27, 1);

	writelr(0x00000000,0x25d00080, 14, 11);
	writelr(0x00000000,0x25d00380, 14, 11);
	writelr(0x00000002,0x25d00078, 23, 3);
	writelr(0x00000005,0x25d00378, 23, 3);

	writelr(0x00560418,0x25d0007c, 9, 23);
	writelr(0x002d0e56,0x25d0037c, 9, 23);
	writelr(0x0000002a,0x25d0007c, 0, 8);
	writelr(0x0000002c,0x25d0037c, 0, 8);
#endif

	//base 0x26000000
	writelr(0x00000001,0x26000004, 1, 1);
	writelr(0x00000001,0x26000004, 16, 4);

	writelr(0x00000001,0x25d00070, 12, 1);
	writelr(0x00000001,0x25d00070, 12, 1);//delay
	udelay(1);
	writelr(0x00000001,0x25d00070, 13, 1);
	writelr(0x00000001,0x25d00070, 13, 1);//delay
	udelay(100);
	writelr(0x00000000,0x25d00374, 25, 1);
	writelr(0x00000000,0x25d00374, 25, 1);//delay

	do {
		pll_lock = readl(0x25d00380);

		if (1 == (pll_lock & 0x1)) {
			#ifndef CONFIG_MOLCHIP_FASTBOOT
			printf("s pll lock success !\n");
			#endif
			break;
		}
	} while (1);
}

void dsi_phy_v1_init(void)
{
	_dsi_phy_v1_cfg();
}

void _lcd_src_sel(void)
{
	//base 0x24000e00
	writelr(0x00000001,0x24000e0c, 6, 1);
}

void _dhd_src_sel(void)
{
	//base 0x24000e00
	writelr(0x00000001,0x24000e0c, 5, 1);
}

void rgb_init(void)
{
	rgb_lcm_bus_pin_cfg();
	dsi_phy_v1_init();
	_lcd_src_sel();
	_dhd_src_sel();
}

