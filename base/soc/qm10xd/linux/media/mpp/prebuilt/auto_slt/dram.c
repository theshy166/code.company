#include <common.h>
#include <asm/io.h>
#include <mach/uart_drv.h>
#if defined(CONFIG_SUPPORT_MEMTEST) && defined(CONFIG_SPL_BUILD)
#include "../memtest/mem_types.h"
#endif
#include "../board/efuse.h"
#include <mach/hardware.h>
#ifdef CONFIG_DDR_TRAINING
#include "../ddr_training/ddr_training.h"
#endif
#ifdef CONFIG_PSRAM_TRAINING
#include "../psram_training/psram_training.h"
#endif
#if defined(CONFIG_EYE_SCAN) && defined(CONFIG_SPL_BUILD)
#include "../eye_scan/eye_scan.h"
#endif

#define __DEFINE_TO_STR(s)   #s
#define DEFINE_TO_STR(s)     __DEFINE_TO_STR(s)
#define BUILD_VERSION DEFINE_TO_STR(VER_B)

#define RAM_BASE	0x40000000
#define DDR_SIZE	0x4000000
#define PSRAM_SIZE	0x1000000

struct reg_info_t {
	unsigned int reg_addr;
	unsigned int regv;
	unsigned int mask;
};

#if defined(CONFIG_SUPPORT_MEMTEST) && defined(CONFIG_SPL_BUILD)
struct mem_test memtests[] = {
	{"Random Value", test_random_value},
	{"Compare XOR", test_xor_comparison},
	{"Compare SUB", test_sub_comparison},
	{"Compare MUL", test_mul_comparison},
	{"Compare DIV", test_div_comparison},
	{"Compare OR", test_or_comparison},
	{"Compare AND", test_and_comparison},
	{"Sequential Increment", test_seqinc_comparison},
	{"Solid Bits", test_solidbits_comparison},
	{"Block Sequential", test_blockseq_comparison},
	{"Checkerboard", test_checkerboard_comparison},
	{"Bit Spread", test_bitspread_comparison},
	{"Bit Flip", test_bitspread_comparison},
	{"Walking Ones", test_walkbits1_comparison},
	{"Walking Zeroes", test_walkbits0_comparison},
	{0, 0}
};

struct mem_info meminfo = {
	.magic_start = 0x5a5a5a5a,
	.mem_base = RAM_BASE,
	.mem_len = DDR_SIZE,
	.magic_end = 0x5a5a5a5a
};
#endif

unsigned int ram_size = DDR_SIZE;
unsigned int _parameter_start;

#ifdef CONFIG_PSRAM
extern unsigned int _wb_start;
extern unsigned int _ap_start;
#else
extern unsigned int _esmt_start;
extern unsigned int _unis_start;
#endif

#if defined(CONFIG_SUPPORT_MEMTEST) && defined(CONFIG_SPL_BUILD)
void ram_test(void)
{
	meminfo.mem_len = ram_size;
	unsigned int halflen = meminfo.mem_len / 2;
	unsigned int count = halflen / sizeof(unsigned int);
	unsigned int *bufa = (unsigned int *)meminfo.mem_base;
	unsigned int *bufb = (unsigned int *)(meminfo.mem_base + halflen);
	unsigned int i;

	if (test_stuck_address((unsigned int *)meminfo.mem_base,
		meminfo.mem_len/sizeof(unsigned int))) {
		mc_printf("test_stuck_address[FAIL]\r\n");
		return;
	}

	for (i = 0; i <= 14; i++) {
		if (!memtests[i].name)
			break;

		if (memtests[i].fp(bufa, bufb, count))
			mc_printf("[%s][FAIL]\r\n", memtests[i].name);
#ifndef CONFIG_MOLCHIP_FASTBOOT
		else
			mc_printf("[%s][PASS]\r\n", memtests[i].name);
#endif
	}
}
#endif

unsigned int check_ram_access(unsigned int pat)
{
	unsigned int *pb = (unsigned int *)RAM_BASE;
	unsigned int cnt, sz = 0x1000;

	for (cnt = 0; cnt < sz; cnt++)
		pb[cnt] = pat;

	for (cnt = 0; cnt < sz; cnt++) {
		if (pb[cnt] != pat) {
			mc_printf("%s failed 0x%08x 0x%08x 0x%08x\r\n", __func__,
				&pb[cnt], pat, pb[cnt]);
			return 1;
		}
	}

	return 0;
}

#ifndef CONFIG_MOLCHIP_FASTBOOT
void ram_simple_test(void)
{
	int i;
	unsigned int second_base = RAM_BASE + (ram_size / 2);

	mc_printf("%s start\r\n", __func__);

	for (i = 0; i < (1024 * 1024); i++) {
		/* 0xdeaddead is a random value for test */
		*(volatile unsigned int *)(RAM_BASE + (i * 4)) = 0xdeaddead;
		*(volatile unsigned int *)(second_base + (i * 4)) = 0xdeaddead;
	}

	for (i = 0; i < (1024 * 1024); i++) {
		if ((*(volatile unsigned int *)(RAM_BASE + (i * 4))) !=
			(*(volatile int unsigned *)(second_base + (i * 4)))) {
			mc_printf("[0x%08x]:0x%08x != [0x%08x]:0x%08x\r\n",
			(RAM_BASE + (i * 4)),
			(*(volatile unsigned int *)(RAM_BASE + (i * 4))),
			(second_base + (i * 4)),
			(*(volatile unsigned int *)(second_base + (i * 4))));
			//break;
		}
	}

	if (i == (1024 * 1024)) {
		mc_printf("%s success\r\n", __func__);
	} else {
		mc_printf("%s fail\r\n", __func__);
		while (1)
			;
	}
	mc_printf("%s end\r\n", __func__);

}

void mem_addr_test(void)
{
	int i;

	mc_printf("%s start\r\n", __func__);

	for (i = 0; i < ((1024 * 1024) / 4); i++)
		*(volatile unsigned int *)(RAM_BASE + (i * 4)) =
			(RAM_BASE + (i * 4));

	for (i = 0; i < ((1024 * 1024) / 4); i++) {
		if ((RAM_BASE + (i * 4)) !=
			(*(volatile unsigned int *)(RAM_BASE + (i * 4)))) {
			mc_printf("[0x%08x]:0x%08x\r\n",
			(RAM_BASE + (i * 4)),
			(*(volatile unsigned int *)(RAM_BASE + (i * 4))));
			//break;
		}
	}

	if (i == (1024 * 1024 / 4)) {
		mc_printf("%s success\r\n", __func__);
	} else {
		mc_printf("%s fail\r\n", __func__);
		while (1)
			;
	}

	mc_printf("%s end\r\n", __func__);
}
#endif

void get_ram_type(void)
{
	u32 val;

	efuse_init();
	if (efuse_apb_read(REG_EFUSE_BASE, SW_BONDING_OPT, &val) == false)
		val = 0;

	val >>= RAM_TYPE_SHIFT;
	val &= RAM_TYPE_MASK;

	switch (val) {
	#ifdef CONFIG_PSRAM
	case 2:
		_parameter_start = _wb_start;
		ram_size = PSRAM_SIZE;
		break;
	case 3:
		_parameter_start = _ap_start;
		ram_size = PSRAM_SIZE;
		break;
	#else
	case 0:
	default:
		_parameter_start = _esmt_start;
		ram_size = DDR_SIZE;
		break;
	case 1:
		_parameter_start = _unis_start;
		ram_size = DDR_SIZE;
		break;
	#endif
	}

}

void ram_init(void)
{
	struct reg_info_t *reginfo = (struct reg_info_t *)(_parameter_start + 8);
	unsigned int len = *(volatile unsigned int *)(_parameter_start + 4);
	unsigned int i, reg_val;

#ifndef CONFIG_MOLCHIP_FASTBOOT
	mc_printf("%s start 0x%08x 0x%08x 0x%08x(%d)\r\n", __func__,
		(_parameter_start),
		*(volatile unsigned int *)(_parameter_start),
		*(volatile unsigned int *)(_parameter_start + 4),
		*(volatile unsigned int *)(_parameter_start + 4));
#endif

	for (i = 0; i < len; i++) {
		if ((reginfo[i].reg_addr & 0x3) == 0x0) {
			reg_val = *(volatile unsigned int *)reginfo[i].reg_addr;
			reg_val &= ~reginfo[i].mask;
			reg_val |= reginfo[i].regv;
			*(volatile unsigned int *)reginfo[i].reg_addr = reg_val;

#ifndef CONFIG_MOLCHIP_FASTBOOT
			mc_printf("%03d-0x%08x 0x%08x\r\n", i, reginfo[i].reg_addr,
				*(volatile unsigned int *)reginfo[i].reg_addr);
#endif
		} else if ((reginfo[i].reg_addr & 0x3) == 0x1) {
			while ((*(volatile unsigned int *)(reginfo[i].reg_addr
				& ~0x3) & reginfo[i].mask) !=
				reginfo[i].regv) {
#ifndef CONFIG_MOLCHIP_FASTBOOT
				mc_printf("polling %03d-0x%08x 0x%08x\r\n", i,
				reginfo[i].reg_addr,
				*(volatile unsigned int *)reginfo[i].reg_addr);
#endif
			}
		} else {
#ifndef CONFIG_MOLCHIP_FASTBOOT
			mc_printf("%03d-0x%08x\r\n", i, reginfo[i].reg_addr);
#endif
		}
	}
#ifndef CONFIG_MOLCHIP_FASTBOOT
	reg_val = *(volatile unsigned int *)PSRAM_CTRL;
	mc_printf("ddr_psram_sel:0x%08x is %s\r\n", reg_val,
		(reg_val & DDR_PSRAM_SEL) ? "PSRAM" : "DDR");
#endif
}

void start_watchdog(int sec)
{
	unsigned int val;
	val = readl(0xc0000B0);
	writel(val | (0x1 << 4) | (0x1 << 9),0xc0000B0);
	mc_printf("Open wdt:[0xc0000B0]:[0x%08x]\r\n",*(volatile unsigned int *)0xc0000B0);
	*(volatile unsigned int*)0x09600018 = 0x1acce551;
	*(volatile unsigned int*)0x09600000 = sec * 0x8000;
	*(volatile unsigned int*)0x09600004 = 0xe;
}

void serial_get_expect_string(const char *e_str)
{
	static u8 autoslt_str[16];
	int cnt = 0;
	unsigned int len = strlen(e_str);

	memset(autoslt_str, 0x0, 16);


	while(1) {

			if(cnt == 15) {
					cnt = 0;
					memset(autoslt_str, 0x0, 16);
			}

			autoslt_str[cnt] = uart_getc(0x08f00000);
			if(cnt == (len - 1)) {
					autoslt_str[cnt+1] = 0;
					if(0 == strcmp((const char*)autoslt_str, e_str)) {
							break;
					} else {
							cnt = 0;
							memset(autoslt_str, 0x0, 16);
							mc_printf("please input again\r\n");
							continue;
					}
			}

			cnt++;
	}
}

extern unsigned int reset_type;
void DRAM_Init(void)
{
#if defined(CONFIG_SPL_BUILD)
	if (reset_type == 3) {
			mc_printf("\033[31m<<ErrCode=02>>\033[0m \r\n");
			while(1);
	}
	if (reset_type == 5) {
			mc_printf("\033[31m<<ErrCode=02>>\033[0m \r\n");
			while(1);
	}

	serial_get_expect_string("<<START>>");

	mc_printf("<<INITIALOK@%s.IMG>>\r\n",BUILD_VERSION);

	serial_get_expect_string("<<NEXT>>");
#endif
	get_ram_type();
	ram_init();

#ifdef CONFIG_PSRAM
#ifdef CONFIG_PSRAM_TRAINING
	if ((_parameter_start == _wb_start) ||
		(_parameter_start == _ap_start))
		psram_training();
#endif

#else

#if defined(CONFIG_EYE_SCAN) && defined(CONFIG_SPL_BUILD)
	if ((_parameter_start == _esmt_start) ||
		(_parameter_start == _unis_start))
		DRAM_EyeScan_Debug();
#endif

#ifdef CONFIG_DDR_TRAINING
	if ((_parameter_start == _esmt_start) ||
		(_parameter_start == _unis_start))
		training_main(DRAM_ADDR_WID);
#endif

#endif

#ifndef CONFIG_MOLCHIP_FASTBOOT
	mem_addr_test();
	ram_simple_test();
#endif

	if (check_ram_access(0x5a5aa5a5)) {
		mc_printf("ram_init test fail,need training\r\n");
		while (1)
			;
	}

#if defined(CONFIG_SUPPORT_MEMTEST) && defined(CONFIG_SPL_BUILD)
	ram_test();
#endif
#if defined(CONFIG_SPL_BUILD)
	start_watchdog(180);
#endif
}
