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

#define IRAM_BASE    (0x27000000)
#define DDR_CFG_BASE (0x24100000)
#define RESET_BASE   (0x10000000)
#define RESET_OFFSET_REG (0x0100)
#define RESET_OFFSET_BIT (1<<0)
#define FLAG_POLL 0
#define FLAG_SET  1

struct config_reg {
	unsigned int addr;
	unsigned int val;
	unsigned int mask;
	unsigned int flag;  //0:set, 1:poll
};

struct config_reg cfg_reg[] =
{
	{DDR_CFG_BASE + 0x0490, 0, 1<<0, FLAG_SET},
	{DDR_CFG_BASE + 0x0540, 0, 1<<0, FLAG_SET},
	{DDR_CFG_BASE + 0x05f0, 0, 1<<0, FLAG_SET},
	{DDR_CFG_BASE + 0x03fc, 0, 0xffffffff, FLAG_POLL},
	{DDR_CFG_BASE + 0x0030, 1<<5, 1<<5, FLAG_SET},
	{DDR_CFG_BASE + 0x0004, 2<<4, 3<<4, FLAG_POLL},
	{RESET_BASE + RESET_OFFSET_REG, RESET_OFFSET_BIT, RESET_OFFSET_BIT, FLAG_SET},
	{0,0,0,0} //end flag
};

unsigned int cfg_reg_len = sizeof(cfg_reg)/sizeof(cfg_reg[0]);

unsigned int iram_mach_code[0x100] = {
0xE2800C01,
0xE5901000,
0xE3510000,
0x0A000006,
0xE590100C,
0xE2011001,
0xE3510001,
0x0A000003,
0xEA00000C,
0xE2800010,
0xEAFFFFF5,
0xEAFFFFFE,
0xE5901000,
0xE5902004,
0xE5903008,
0xE5914000,
0xE1E05003,
0xE0044005,
0xE0022003,
0xE1844002,
0xE5814000,
0xEAFFFFF2,
0xE5901000,
0xE5902004,
0xE5903008,
0xE5914000,
0xE1A05003,
0xE0044005,
0xE1540002,
0x1AFFFFFA,
0xEAFFFFE9,
0xE52DB004,
0xE28DB000,
0xE3A03000,
0xE1A00003,
0xE24BD000,
0xE49DB004,
0xE12FFF1E,

0 //end flag
};

void init_iram_reboot(void)
{
	unsigned int i=0;

	/* copy code */
	while(iram_mach_code[i] != 0) {
		*(volatile unsigned int *)(IRAM_BASE + i*4) = iram_mach_code[i];
		i++;
	}

	for(i=0; i<cfg_reg_len; i++)
	{
		*(volatile unsigned int *)(IRAM_BASE + 0x100 + i*sizeof(struct config_reg) + 0x0) = cfg_reg[i].addr;
		*(volatile unsigned int *)(IRAM_BASE + 0x100 + i*sizeof(struct config_reg) + 0x4) = cfg_reg[i].val;
		*(volatile unsigned int *)(IRAM_BASE + 0x100 + i*sizeof(struct config_reg) + 0x8) = cfg_reg[i].mask;
		*(volatile unsigned int *)(IRAM_BASE + 0x100 + i*sizeof(struct config_reg) + 0xc) = cfg_reg[i].flag;
	}

}

/*
 * por/ext/wdg/sw  por/wdg/sw      sw             por/ext/wdg       wdg           por/wdg               por
 * POR_FLAG6       POR_FLAG5    POR_FLAG4       POR_FLAG3    POR_FLAG2          POR_FLAG1           POR_FLAG0
 *     0              0            0                0            0                  0                  0          POR_RST
 *     0              0            1                1            1                  0                  0          EXT_RST
 *     1              0            1                1            0                  0                  1          WDG_RST
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

	*(volatile unsigned int*)REG_POR_FLAG0 = 1;
	*(volatile unsigned int*)REG_POR_FLAG1 = 1;
	*(volatile unsigned int*)REG_POR_FLAG2 = 1;
	*(volatile unsigned int*)REG_POR_FLAG3 = 1;
	*(volatile unsigned int*)REG_POR_FLAG4 = 1;
	*(volatile unsigned int*)REG_POR_FLAG5 = 1;
	*(volatile unsigned int*)REG_POR_FLAG6 = 1;


	if ((por_flag[0] == 0) && (por_flag[1] == 0) && (por_flag[3] == 0) && (por_flag[4] == 0) && (por_flag[5] == 0) && (por_flag[6] == 0))
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

int print_reset_type(void)
{
        int reset_type = MC_GetResetType();
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

        return 0;
}


void MC_ExtResetEnable(unsigned int is_en)
{
	*(volatile unsigned int*)REG_RST_CTRL = ((is_en != 0) ? 1 : 0);
}

#ifdef CONFIG_MOLCHIP_FASTBOOT
static void disable_mmu_and_jump_to_iram(unsigned long iram_base_pa)
{

        asm volatile("mov r0,%0"::"r"(iram_base_pa));
        asm volatile("mov r1,#0x0");
        asm volatile("dmajti_lp_start: cmp r1,#0x1");
        asm volatile("mrceq p15, 0, r1, c1, c0, 0");
        asm volatile("biceq r1,r1,#0x5");
        asm volatile("mcreq p15, 0, r1, c1, c0, 0");
        asm volatile("blxeq r0");
        asm volatile("mov r1,#0x1");
        asm volatile("b dmajti_lp_start");
}
#endif

int do_reboot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	/* do reset */
	init_iram_reboot();
#ifdef CONFIG_MOLCHIP_FASTBOOT
	disable_mmu_and_jump_to_iram(IRAM_BASE);
#else
	asm volatile("mov r0,%0"::"r"(IRAM_BASE));
	asm volatile("blx r0");
#endif
	while(1);
	*(volatile unsigned int*)(0x10000100) = 1;

        return 0;
}

U_BOOT_CMD(
        reboot, 2, 0, do_reboot,
        "reboot molchip soc",
        "reboot molchip soc"
);

