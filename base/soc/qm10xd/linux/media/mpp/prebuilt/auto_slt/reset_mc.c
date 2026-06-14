#include <common.h>
#include <command.h>
#include <mach/uart_drv.h>

enum reset_e {
    POR_RST = 0,
    EXT_RST = 1,
    WDG_RST = 3,
    SW_RESET = 5,
    NO__RST = 7,
};
#define REG_POR_FLAG0     (0x10000124)
#define REG_POR_FLAG1     (0x10000128)
#define REG_POR_FLAG2     (0x1000012c)
#define REG_POR_FLAG3     (0x10000130)
#define REG_POR_FLAG4     (0x10000134)
#define REG_POR_FLAG5     (0x10000138)
#define REG_POR_FLAG6     (0x1000013c)
#define REG_RST_CTRL      (0x10000100)

#define BIT_SW_GLB_RST_EN     (0x1 << 0)

/*
 * por/ext/wdg/sw  por/wdg/sw      sw             por/ext/wdg       wdg           por/wdg               por
 * POR_FLAG6       POR_FLAG5    POR_FLAG4       POR_FLAG3    POR_FLAG2          POR_FLAG1           POR_FLAG0
 *     0              0            1                0            1                  0                  0          POR_RST
 *     0              0            1                1            1                  0                  0          EXT_RST
 *     1              0            1                1            0                  0                  1          WDG_RST
 *     0              0            0                1            1                  1                  1          SW_RESET
 *     1              1            1                1            1                  1                  1          NO__RST
 */


static void Set_Por_Flag(void)
{
	*(volatile unsigned int*)REG_POR_FLAG0 = 1;
	*(volatile unsigned int*)REG_POR_FLAG1 = 1;
	*(volatile unsigned int*)REG_POR_FLAG2 = 1;
	*(volatile unsigned int*)REG_POR_FLAG3 = 1;
	*(volatile unsigned int*)REG_POR_FLAG4 = 1;
	*(volatile unsigned int*)REG_POR_FLAG5 = 1;
	*(volatile unsigned int*)REG_POR_FLAG6 = 1;
}

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


	if ((por_flag[0] == 0) && (por_flag[1] == 0) && (por_flag[3] == 0) && (por_flag[5] == 0) && (por_flag[6] == 0))
		return POR_RST;
	else if ((por_flag[0] == 0) && (por_flag[1] == 0) && (por_flag[2] == 1)&& (por_flag[3] == 1) && (por_flag[4] == 1) && (por_flag[5] == 0) && (por_flag[6] == 0))
		return EXT_RST;
	else if ((por_flag[0] == 1) && (por_flag[1] == 0) && (por_flag[2] == 0)&& (por_flag[3] == 1) && (por_flag[4] == 1) && (por_flag[5] == 0) && (por_flag[6] == 1))
		return WDG_RST;
	else if ((por_flag[0] == 1) && (por_flag[1] == 1) && (por_flag[2] == 1)&& (por_flag[3] == 1) && (por_flag[4] == 0) && (por_flag[5] == 0) && (por_flag[6] == 0))
		return SW_RESET;
	else
		return NO__RST;

}

unsigned int reset_type;
int print_reset_type(void)
{
#ifndef CONFIG_MOLCHIP_FASTBOOT
        reset_type = MC_GetResetType();
        if (reset_type == POR_RST )
                mc_printf("RESET TYPE : Power on reset \r\n");
        else if (reset_type == EXT_RST )
                mc_printf("RESET TYPE : external reset \r\n");
        else if (reset_type == WDG_RST )
                mc_printf("RESET TYPE : Wdg reset \r\n");
        else if (reset_type == SW_RESET )
                mc_printf("RESET TYPE :Soft reset \r\n");
        else
                mc_printf("RESET TYPE :Other  reset \r\n");
#endif

		Set_Por_Flag();
        return 0;
}
