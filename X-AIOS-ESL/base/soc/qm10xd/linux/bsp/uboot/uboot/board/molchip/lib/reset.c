#include <common.h>
#include <command.h>

enum reset_e {
    POR_RST = 0,
    EXT_RST = 1,
    WDG_RST = 3,
    SW_RESET = 5,
    NO__RST = 7,
};

#ifdef CONFIG_TARGET_FY12
#define REG_POR_FLAG0     (0x28000034)
#define REG_POR_FLAG1     (0x28000038)
#define REG_POR_FLAG2     (0x2800003c)
#define REG_POR_FLAG3     (0x28000040)
#define REG_POR_FLAG4     (0x28000044)
#define REG_POR_FLAG5     (0x28000048)
#define REG_POR_FLAG6     (0x2800004C)
#define REG_RST_CTRL      (0x280000A8)
#define WR_POR_CTRL       (0x280000AC)
#define WR_POR_DATA       (0x5A5A5A5A)

#define POR_ENABLE        0x1

#else
#define REG_POR_FLAG0     (0x28000024)
#define REG_POR_FLAG1     (0x28000028)
#define REG_POR_FLAG2     (0x2800002c)
#define REG_POR_FLAG3     (0x28000030)
#define REG_POR_FLAG4     (0x280000f8)
#define REG_POR_FLAG5     (0x280000fc)
#define REG_POR_FLAG6     (0x28000100)
#define REG_RST_CTRL      (0x2800010c)

#define REG_RST_WR_PR     (0x28000120)
#define DIS_PROTECT_KEY   (0x5a5a5a5a)
#endif
#define BIT_SW_GLB_RST_EN     (0x1 << 0)

/*
 * por/ext/wdg/sw  por/wdg/sw      sw             por/ext/wdg       wdg           por/wdg               por
 * POR_FLAG6       POR_FLAG5    POR_FLAG4       POR_FLAG3    POR_FLAG2          POR_FLAG1           POR_FLAG0
 *     0              0            1                0            1                  0                  0          POR_RST
 *     0              1            1                0            1                  1                  1          EXT_RST
 *     0              0            1                0            0                  0                  1          WDG_RST
 *     0              0            0                1            1                  1                  1          SW_RESET
 *     1              1            1                1            1                  1                  1          NO__RST
 */

unsigned int MC_GetResetType(void)
{
	unsigned int por_flag[7];
	por_flag[0] = *(volatile unsigned int*)REG_POR_FLAG0 != 0 ? 1 : 0;
	por_flag[1] = *(volatile unsigned int*)REG_POR_FLAG1 != 0 ? 1 : 0;
	por_flag[2] = *(volatile unsigned int*)REG_POR_FLAG2 != 0 ? 1 : 0;
	por_flag[3] = *(volatile unsigned int*)REG_POR_FLAG3 != 0 ? 1 : 0;
	por_flag[4] = *(volatile unsigned int*)REG_POR_FLAG4 != 0 ? 1 : 0;
	por_flag[5] = *(volatile unsigned int*)REG_POR_FLAG5 != 0 ? 1 : 0;
	por_flag[6] = *(volatile unsigned int*)REG_POR_FLAG6 != 0 ? 1 : 0;

#ifdef CONFIG_TARGET_FY12
	/* disable reg write protect */
	*(volatile unsigned int*)(WR_POR_CTRL) = WR_POR_DATA;
#endif
	
	*(volatile unsigned int*)REG_POR_FLAG0 = 1;
	*(volatile unsigned int*)REG_POR_FLAG1 = 1;
	*(volatile unsigned int*)REG_POR_FLAG2 = 1;
	*(volatile unsigned int*)REG_POR_FLAG3 = 1;
	*(volatile unsigned int*)REG_POR_FLAG4 = 1;
	*(volatile unsigned int*)REG_POR_FLAG5 = 1;
	*(volatile unsigned int*)REG_POR_FLAG6 = 1;

#if POR_ENABLE
		if ((por_flag[0] == 0) && (por_flag[1] == 0) && (por_flag[5] == 0))
			return POR_RST;
		else if ((por_flag[1] == 0) && (por_flag[2] == 0) && (por_flag[3] == 0) && (por_flag[5] == 0) && (por_flag[6] == 0))
			return WDG_RST;
		else if ((por_flag[3] == 0) && (por_flag[4] == 0) && (por_flag[5] == 0) && (por_flag[6] == 0))
			return SW_RESET;
		else
			return NO__RST;
#else
	if ((por_flag[0] == 0) && (por_flag[1] == 0) && (por_flag[2] == 1)&& (por_flag[3] == 0) && (por_flag[4] == 1) && (por_flag[5] == 0) && (por_flag[6] == 0))
		return POR_RST;
	else if ((por_flag[0] == 1) && (por_flag[1] == 1) && (por_flag[2] == 1)&& (por_flag[3] == 0) && (por_flag[4] == 1) && (por_flag[5] == 1) && (por_flag[6] == 0))
		return EXT_RST;
	else if ((por_flag[0] == 1) && (por_flag[1] == 0) && (por_flag[2] == 0)&& (por_flag[3] == 0) && (por_flag[4] == 1) && (por_flag[5] == 0) && (por_flag[6] == 0))
		return WDG_RST;
	else if ((por_flag[0] == 1) && (por_flag[1] == 1) && (por_flag[2] == 1)&& (por_flag[3] == 1) && (por_flag[4] == 0) && (por_flag[5] == 0) && (por_flag[6] == 0))
		return SW_RESET;
	else
		return NO__RST;
#endif
}

void MC_ExtResetEnable(unsigned int is_en)
{
	*(volatile unsigned int*)REG_RST_CTRL = ((is_en != 0) ? 1 : 0);
}

int do_reboot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	/* disable reg write protect */
#ifdef CONFIG_TARGET_FY12
	*(volatile unsigned int*)(WR_POR_CTRL) = WR_POR_DATA;
#else
	*(volatile unsigned int*)(REG_RST_WR_PR) = DIS_PROTECT_KEY;
#endif

	/* do reset */
	*(volatile unsigned int*)(REG_RST_CTRL) = BIT_SW_GLB_RST_EN;

        return 0;
}

U_BOOT_CMD(
        reboot, 2, 0, do_reboot,
        "reboot molchip soc",
        "reboot molchip soc"
);

