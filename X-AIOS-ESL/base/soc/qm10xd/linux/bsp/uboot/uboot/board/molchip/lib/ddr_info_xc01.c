#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <command.h>
#define DDR_INFO_REG 0x10000188

int do_print_ddr_cali_info(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int info_data;
	char info_regs[256] = "DRAM INFO: ";
	info_data = *(volatile unsigned int*)DDR_INFO_REG;
	//info_data = 0x25a1;


	if(((info_data>>9)&0x3) == 0)
		strcat(info_regs,"6X8-");
	else if(((info_data>>9)&0x3) == 1)
		strcat(info_regs,"11X11-");
	else if(((info_data>>9)&0x3) == 2)
		strcat(info_regs,"13X13-");
	else if(((info_data>>9)&0x3) == 3)
		strcat(info_regs,"reserved-");
	if((info_data&0x3) == 0)
		strcat(info_regs,"DDR2-");
	else if((info_data&0x3) == 1)
		strcat(info_regs,"DDR3-");
	else if((info_data&0x3) == 2)
		strcat(info_regs,"DDR4-");
	else if((info_data&0x3) == 3)
		strcat(info_regs,"DDR3L-");

	if(((info_data>>3)&0x7) == 0)
		strcat(info_regs,"1066Mbps-");
	else if(((info_data>>3)&0x7) == 1)
		strcat(info_regs,"1333Mbps-");
	else if(((info_data>>3)&0x7) == 2)
		strcat(info_regs,"1600Mbps-");
	else if(((info_data>>3)&0x7) == 3)
		strcat(info_regs,"1866Mbps-");
	else if(((info_data>>3)&0x7) == 4)
		strcat(info_regs,"2133Mbps-");
	else if(((info_data>>3)&0x7) == 5)
		strcat(info_regs,"2400Mbps-");
	else if(((info_data>>3)&0x7) == 6)
		strcat(info_regs,"2666Mbps-");
	else if(((info_data>>3)&0x7) == 7)
		strcat(info_regs,"3200Mbps-");

	if(((info_data>>6)&0x3) == 0)
		strcat(info_regs,"8bit-");
	else if(((info_data>>6)&0x3) == 1)
		strcat(info_regs,"16bit-");
	else if(((info_data>>6)&0x3) == 2)
		strcat(info_regs,"32bit-");
	else if(((info_data>>6)&0x3) == 3)
		strcat(info_regs,"reserved-");

	if(((info_data>>11)&0x7) == 0)
		strcat(info_regs,"256Mb-");
	else if(((info_data>>11)&0x7) == 1)
		strcat(info_regs,"512Mb-");
	else if(((info_data>>11)&0x7) == 2)
		strcat(info_regs,"1Gb-");
	else if(((info_data>>11)&0x7) == 3)
		strcat(info_regs,"2Gb-");
	else if(((info_data>>11)&0x7) == 4)
		strcat(info_regs,"4Gb-");
	else if(((info_data>>11)&0x7) == 5)
		strcat(info_regs,"8Gb-");
	else if(((info_data>>11)&0x7) == 6)
		strcat(info_regs,"16Gb-");
	else if(((info_data>>11)&0x7) == 7)
		strcat(info_regs,"reserved-");

	if(((info_data>>14)&0x3) == 0)
		strcat(info_regs,"0-");
	else if(((info_data>>14)&0x3) == 1)
		strcat(info_regs,"1-");
	else if(((info_data>>14)&0x3) == 2)
		strcat(info_regs,"2-");
	else if(((info_data>>14)&0x3) == 3)
		strcat(info_regs,"3-");

	printf("%s\r\n",info_regs);
	return 0;
}

U_BOOT_CMD(
        ddr_info, 2, 0, do_print_ddr_cali_info,
        "ddr training info molchip soc",
        "ddr training info molchip soc"
);
