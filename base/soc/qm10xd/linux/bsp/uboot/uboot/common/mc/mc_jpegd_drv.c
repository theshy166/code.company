#include "mc_jpegd_drv.h"
#include "mc_vou_osal.h"

static void jpeg_clk_on(void)
{
	writelr(0x1, 0x0C0000AC, 19, 1);/* ckg_vdec_en */
	writelr(0x1, 0x0C0000A0, 4, 1);/* ckg_vdec_en */
}

static void jpeg_clk_off(void)
{
	writelr(0x0, 0x0B310010, 0, 1);/* set jpeg pause */
	writelr(0x3, 0x0B300008, 0, 2);/* set axim pause */
	writelr(0x2, 0x0B300028, 12, 4);/* set axim pause */
	do {
		if (((readl(0x0B300070) & 0x000000007) == 0) && ((readl(0x0B31000C) & 0xF0000000) == 0)) {
			#ifndef CONFIG_MOLCHIP_FASTBOOT
			printf("veu bus idle !\n");
			#endif
			break;
		}
	} while (1);
	writelr(0x0, 0x0C0000A0, 4, 1);/* ckg_vdec_en */
	writelr(0x0, 0x0C0000AC, 19, 1);/* ckg_vdec_en */
}

static void JPEG_Module_Reset(void)
{
	unsigned int bus_state;
	writelr(0x0, 0x0B310010, 0, 1);/* set jpeg pause */

	do {
		bus_state = readl(0x0B31000C);
		if ((bus_state & 0xF0000000) == 0) {
			#ifndef CONFIG_MOLCHIP_FASTBOOT
			printf("jpg bus idle !\n");
			#endif
			break;
		}
	} while (1);

	writelr(0x1, 0x0C0000A0+0x1000, 2, 1);/* reset */
	udelay(10);
	writelr(0x1, 0x0C0000A0+0x2000, 2, 1);/* clear */
}

static void JPEG_SetIntEnable(void)
{
	writel(0x3, JPEGD_BASE + JPEG_INT_EN);
}

static void JPEG_SetDecParam(unsigned int jpg_addr, unsigned int jpg_size, unsigned int luma_addr, unsigned int chroma_addr)
{
	writel(jpg_addr, JPEGD_BASE + JPEG_REG8);
	writel(jpg_size, JPEGD_BASE + JPEG_BUFF0_SIZE);
	writel(luma_addr, JPEGD_BASE + JPEG_STM_ADDR0);
	writel(chroma_addr, JPEGD_BASE + JPEG_REG9);
	writel(((1 <<3) | 2), JPEGD_BASE + JPEG_REG1);
	writel(0x0, JPEGD_BASE + JPEG_REG11);
}

static void JPEG_SetEncDecStart(void)
{
	writel(0x80000001, JPEGD_BASE + JPEG_REG0);
}

static unsigned int JPEG_GetIntStatus(void)
{
	return readl(JPEGD_BASE + JPEG_INT_STATUS);
}

static void JPEG_SetIntClear(void)
{
	writel(0x00000003, JPEGD_BASE + JPEG_INT_STATUS);
}

unsigned int JPEG_WaitIRQ(void)
{
	unsigned int jpg_int_state;

	do {
		jpg_int_state = JPEG_GetIntStatus();
		if (jpg_int_state & JPEGD_DONE_STATE) {
			JPEG_SetIntClear();
			jpeg_clk_off();
			#ifndef CONFIG_MOLCHIP_FASTBOOT
			printf("jpg dec done !\n");
			#endif
			break;
		}
	} while (1);

	return 0;
}

#define JPEG_DEC_TIMEOUT 1000 //msec
unsigned int JPEG_WaitIRQ_Timeout(void)
{
	unsigned int jpg_int_state;
	unsigned int start = get_timer(0);

	do {
		jpg_int_state = JPEG_GetIntStatus();
		if (jpg_int_state & JPEGD_DONE_STATE) {
			JPEG_SetIntClear();
			jpeg_clk_off();
			#ifndef CONFIG_MOLCHIP_FASTBOOT
			printf("jpg dec done !\n");
			#endif
			break;
		}
		if (get_timer(start) > JPEG_DEC_TIMEOUT) {
			printf("jpg dec failed !\n");
			JPEG_SetIntClear();
			jpeg_clk_off();
			return 1;
		}
	} while (1);

	return 0;
}

void hw_jpeg_dec(unsigned int jpg_addr, unsigned int jpg_size, unsigned int luma_addr, unsigned int chroma_addr)
{
	jpeg_clk_on();
	JPEG_Module_Reset();
	JPEG_SetIntEnable();
	JPEG_SetDecParam(jpg_addr, jpg_size, luma_addr, chroma_addr);
	JPEG_SetEncDecStart();
}

void sw_jpeg_dec(unsigned int jpg_addr, unsigned int jpg_size, unsigned int luma_addr, unsigned int chroma_addr)
{
/* todo */
}

