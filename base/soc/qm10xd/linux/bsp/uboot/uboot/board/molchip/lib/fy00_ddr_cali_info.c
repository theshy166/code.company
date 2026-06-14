#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <command.h>
#define DDR_PHY_BASE 0x2C200000
#define CEN_GLB_APB_REG 0x28000000

int do_print_ddr_cali_info(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{	
	unsigned int dpll_rate = 0;
	unsigned int dpll_div = 0;
	dpll_rate = ((*(volatile unsigned int*)(CEN_GLB_APB_REG+0x00D8)&0x00380000) >> 19);
	dpll_div = ((*(volatile unsigned int*)(CEN_GLB_APB_REG+0x00D8)&0x0007FFF0) >> 4);

	#if 0
		printf("RX-window represents the window of the rx data bus\n");
		printf("TX-window represents the window of the tx data bus\n");
		printf("CA-window represents the window of the command and the address bus\n");
	#endif

	if((dpll_rate == 0)&&(dpll_div == 0x26E0)){
		printf("Frequency\t\t: %dM\n",1866);
	}else if((dpll_rate == 1)&&(dpll_div == 0x26E0)){
		printf("Frequency\t\t: %dM\n",933);
	}else if((dpll_rate == 2)&&(dpll_div == 0x26D5)){
		printf("Frequency\t\t: %dM\n",466);
	}else if((dpll_rate == 2)&&(dpll_div == 0x2880)){
		printf("Frequency\t\t: %dM\n",486);
	}else if((dpll_rate == 2)&&(dpll_div == 0x2C60)){
		printf("Frequency\t\t: %dM\n",533);
	}else {
		printf("cannot get Frequency\t\t ");

	}

	#if 0
        printf("Version\t\t\t: %x\n",(*(volatile unsigned int*)(DDR_PHY_BASE + 0x0004) & 0x000000FF));
        printf("Calibration result\t: 0x%04x (all zero is right)\n",(*(volatile unsigned int*)(DDR_PHY_BASE + 0x0508) & 0xFFFF0000) >> 16);
        printf("RX-window-0\t\t: %d (expect: 3 or  4@1866M, About  8@1066M)\n",(*(volatile unsigned int*)(DDR_PHY_BASE+0x0010)&0x0000F000) >> 12);
        printf("RX-window-1\t\t: %d (expect: 3 or  4@1866M, About  8@1066M)\n",(*(volatile unsigned int*)(DDR_PHY_BASE+0x0010)&0x00000F00) >> 8);
        printf("RX-window-2\t\t: %d (expect: 3 or  4@1866M, About  8@1066M)\n",(*(volatile unsigned int*)(DDR_PHY_BASE+0x0010)&0x000000F0) >> 4);
        printf("RX-window-3\t\t: %d (expect: 3 or  4@1866M, About  8@1066M)\n",(*(volatile unsigned int*)(DDR_PHY_BASE+0x0010)&0x0000000F) >> 0);
        printf("TX-window-0\t\t: %d (expect: 4 or  5@1866M, About  8@1066M)\n",(*(volatile unsigned int*)(DDR_PHY_BASE+0x0014)&0x0000F000) >> 12);
        printf("TX-window-1\t\t: %d (expect: 4 or  5@1866M, About  8@1066M)\n",(*(volatile unsigned int*)(DDR_PHY_BASE+0x0014)&0x00000F00) >> 8);
        printf("TX-window-2\t\t: %d (expect: 4 or  5@1866M, About  8@1066M)\n",(*(volatile unsigned int*)(DDR_PHY_BASE+0x0014)&0x000000F0) >> 4);
        printf("TX-window-3\t\t: %d (expect: 4 or  5@1866M, About  8@1066M)\n",(*(volatile unsigned int*)(DDR_PHY_BASE+0x0014)&0x0000000F) >> 0);
        printf("CA-window\t\t: %d (expect: About 9@1866M, About 10@1066M)\n",  (*(volatile unsigned int*)(DDR_PHY_BASE+0x000C)&0x000000FF) >> 0);
	#endif
		return 0;
}

U_BOOT_CMD(
        ddr_info, 2, 0, do_print_ddr_cali_info,
        "ddr training info molchip soc",
        "ddr training info molchip soc"
);
