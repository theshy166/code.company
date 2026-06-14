#ifndef MC_MMC_CARD_H
#define MC_MMC_CARD_H

struct mmc_card_info {
	unsigned int manfid;
	unsigned int hs200_dll;
	unsigned int hs400_dll;
	unsigned int drv_strength;
};

static struct mmc_card_info card_lists[] =
{
//default
	{.manfid = 0x0,
	 .hs200_dll = 0x383858,
	 .hs400_dll = 0x101010,
	.drv_strength = 1,

	},
//samsung
	{.manfid = 0x15,
	 .hs200_dll = 0x383858,
	 .hs400_dll = 0x101010,
	 .drv_strength = 1,
	},
//longsys
	{.manfid = 0xD6,
	.hs200_dll = 0x383858,
	.hs400_dll = 0x101010,
	.drv_strength = 1,

	},
//toshiba
	{.manfid = 0x11,
	.hs200_dll = 0x383858,
	.hs400_dll = 0x101010,
	.drv_strength = 1,

	},
//hik
	{.manfid = 0x2c,
	.hs200_dll = 0x383858,
	.hs400_dll = 0x202010,
	.drv_strength = 1,

	},
};

#endif
