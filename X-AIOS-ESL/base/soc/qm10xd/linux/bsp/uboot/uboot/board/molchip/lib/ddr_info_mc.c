#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <command.h>
#define CKG_DDR_REG		0x24000300
#define CKG_DMC_2X_SEL_OFFSET	14
#define	CKG_DMC_2X_SEL_MASK	0x7

#define MSTR_REG		0x24100000
#define DDR_TYPE_OFFSET		0
#define DDR_TYPE_MASK		0x3F

int do_print_ddr_cali_info(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ckg_data, type_data;
	char info_regs[256] = "DRAM INFO: ";

	ckg_data = *(volatile unsigned int*)CKG_DDR_REG;
	type_data = *(volatile unsigned int*)MSTR_REG;

	switch ((type_data >> DDR_TYPE_OFFSET) & DDR_TYPE_MASK) {
	case BIT(0):
		strcat(info_regs,"DDR3-");
		break;
	case BIT(1):
		strcat(info_regs,"LPDDR-");
		break;
	case BIT(2):
		strcat(info_regs,"LPDDR2-");
		break;
	case BIT(3):
		strcat(info_regs,"LPDDR3-");
		break;
	case BIT(4):
		strcat(info_regs,"DDR4-");
		break;
	case BIT(5):
		strcat(info_regs,"LPDDR4-");
		break;
	case 0:
	default:
		strcat(info_regs,"DDR2-");
		break;
	}

	switch ((ckg_data >> CKG_DMC_2X_SEL_OFFSET) & CKG_DMC_2X_SEL_MASK) {
	case 0:
		strcat(info_regs,"48Mbps-");
		break;
	case 1:
		strcat(info_regs,"600Mbps-");
		break;
	case 2:
		strcat(info_regs,"800Mbps-");
		break;
	case 3:
		strcat(info_regs,"960Mbps-");
		break;
	case 4:
	default:
		strcat(info_regs,"1200Mbps-");
		break;
	}

	strcat(info_regs,"512Mbits(64MBytes)");

	printf("%s\r\n",info_regs);
	return 0;
}

U_BOOT_CMD(
        ddr_info, 2, 0, do_print_ddr_cali_info,
        "ddr training info molchip soc",
        "ddr training info molchip soc"
);
