#include <common.h>

#define LOGO_USE_YUV                (0)

#define LOGO_YUV					(1)
#define LOGO_RGB_1555				(2)
#define LOGO_RGB_8888				(3)

#define LOGO_RGB_FORMAT					(LOGO_RGB_8888)

#define LOGO_W_720_H_480
#if defined LOGO_W_1024_H_768
#define LOGO_W						(1024)
#define LOGO_H						(768)
#elif defined LOGO_W_1920_H_1080
#define LOGO_W						(1920)
#define LOGO_H						(1080)
#elif defined LOGO_W_1280_H_720
#define LOGO_W						(1280)
#define LOGO_H						(720)
#elif defined LOGO_W_720_H_480
#define LOGO_W						(720)
#define LOGO_H						(480)
#endif

#define LOGO_YUV_ADDR_MEM			(0x80000000+512*1024*1024-LOGO_W*LOGO_H*4)
#define LOGO_RGB_ADDR_MEM			(0x80000000+512*1024*1024-LOGO_W*LOGO_H*4)


static uint32_t readl(uint32_t addr)
{
	uint32_t value = *(volatile uint32_t*)addr;
	return value;
}

static void writel(uint32_t value, uint32_t addr)
{
	*(volatile uint32_t*)addr = value;
}

/*
static void writels(uint32_t value, uint32_t addr)
{
	uint32_t tmp = readl(addr);
	tmp |= value;
	writel(tmp, addr);
}


static void writelc(uint32_t value, uint32_t addr)
{
	uint32_t tmp = readl(addr);
	tmp &= (~value);
	writel(tmp, addr);
}
*/

static void writelr(uint32_t value, uint32_t addr, uint32_t start, uint32_t bit)
{
	uint32_t region = (0xFFFFFFFFU << ( start + bit )) | (0xFFFFFFFFULL >> ( 32 - start ));
	uint32_t tmp = readl(addr);
	tmp &= region;
	tmp |= (~region) & (value << start);
	writel(tmp, addr);
}

static uint8_t readb(uint32_t addr)
{
	uint8_t value = *(volatile uint8_t*)addr;
	return value;
}

static void writeb(uint8_t value, uint32_t addr)
{
	*(volatile uint8_t*)addr = value;
}

static void writebs(uint8_t value, uint32_t addr)
{
	uint8_t tmp = readb(addr);
	tmp |= value;
	writeb(tmp, addr);
}

static void writebc(uint8_t value, uint32_t addr)
{
	uint8_t tmp = readb(addr);
	tmp &= (~value);
	writeb(tmp, addr);
}

void dsd_pal_fast_osd(int type, int x, int y, int w, int h, uint32_t addr, int pw, int pp, int ph)
{
    //----------------select size 720/576/0/50------------------------
    //[1]_vou_cmd_set_dev_output_info[720x576/0/50]27000kHz -> 27000kHz
    //[1]_vou_cmd_toggle_dev=1
    writel(0x0, 0x34101004);

	writel(0x00000006, 0x34101F00);
	writel(0x00000002, 0x34101F04);
	writel(0x00000000, 0x34101F08);
	writel(0x0, 0x34101100);
	writel(0x008080FF, 0x3410110C);
	writel(0x024002D0, 0x34101110);
	writel(0x0090035F, 0x34101114);
	writel(0x00150137, 0x34101118);
	writel(0x00160138, 0x3410111C);
	writel(0xF010EB10, 0x34101108);
	writel(0x8080EB, 0x34101130);
	writel(0x9210D2, 0x34101134);
	writel(0x10A6AA, 0x34101138);
	writel(0x223691, 0x3410113C);
	writel(0xDECA6A, 0x34101140);
	writel(0xF05A51, 0x34101144);
	writel(0x6EF029, 0x34101148);
	writel(0x808010, 0x3410114C);
	writel(0x356EBD01, 0x34101150);
	writel(0x00069901, 0x34101154);
	writel(0x14E00101, 0x34101158);
	writel(0x33FC4861, 0x3410115C);
	writel(0x0, 0x34101200);
	writel(0x8F000000,0x34101204);
	writel(0x8F065400, 0x34101208);
	writel(0x0, 0x3410120C);
	writel(0xF00140,0x34101210);
	writel(0x1E00280, 0x34101214);
	writel(0x2, 0x34101300);
	writel(0xFF005001, 0x34101304);
	writel(0x8C0100, 0x3410130C);
	writel(0x100, 0x34101310);
	writel(0x89000000, 0x34101314);
	writel(0xF00000, 0x34101318);

	if (LOGO_YUV == type) {
		writel(0x00008001, 0x34101190);
		writel(0x80000040, 0x34101500);
		writel((y<<16) + x, 0x34101504);
		writel((ph<<16) + pw, 0x34101508);
		writel(pp, 0x3410150C);
		writel(addr, 0x34101510);
		writel(addr + pp*ph, 0x34101514);
		writel((y<<16) + x, 0x34101518);
		writel(0x024002D0, 0x3410151c);
		writel(0x00020000, 0x34101520);
		writel(0x00002000, 0x34101524);
		writel(0x00000000, 0x34101528);
		writel(0x00100000, 0x3410152C);

	} else {
		writel(0x00008000, 0x34101190);
		if (LOGO_RGB_8888 == type) {
			writel(0x80000002, 0x34101400);
		} else if (LOGO_RGB_1555 == type) {
			writel(0x80000000, 0x34101400);
		}
		writel(0xFF005001, 0x34101404);
		writel(0x024002D0, 0x3410140C);
		writel(0x000002D0, 0x34101410);
		writel(LOGO_RGB_ADDR_MEM, 0x34101414);
		writel(0x0, 0x34101418);
	}

	writel(0x0, 0x34101020);
	writel(0x0, 0x34101024);
	writel(0x0, 0x34101028);
	writel(0x1, 0x3410102C);
	writel(0x1, 0x34101034);
	writel(0x1, 0x34101030);

}

void dsd_ntsc_fast_osd(int type, int x, int y, int w, int h, uint32_t addr, int pw, int pp, int ph)
{
	int i, j;
    int bpp = 0;
    if (LOGO_YUV == type) {
        bpp = 1;
    } else if (LOGO_RGB_8888 == type) {
        bpp = 4;
    } else if (LOGO_RGB_1555 == type) {
        bpp = 2;
    }
    for ( i = 1; i < ph; i += 2 ) {
        for ( j = 0; j < pw * bpp; j++ ) {
            *(((volatile uint8_t*)addr) + ( i - 2 ) * pp * bpp + j) = *(((volatile uint8_t*)addr) + i * pp * bpp + j);
        }
    }
    writel(0x0, 0x34101004);

	writel(0x4, 0x34101F00);
	writel(0x2, 0x34101F04);
	writel(0x0, 0x34101F08);
	writel(0x0, 0x34101100);
	writel(0x008080FF, 0x3410110C);
	writel(0x1E002D0, 0x34101110);
	writel(0x8A0359, 0x34101114);
	writel(0x130105, 0x34101118);
	writel(0x130106, 0x3410111C);
	writel(0xFF00FF00, 0x34101108);
	writel(0x8080EB, 0x34101130);
	writel(0x9210D2, 0x34101134);
	writel(0x10A6AA, 0x34101138);
	writel(0x223691, 0x3410113C);
	writel(0xDECA6A, 0x34101140);
	writel(0xF05A51, 0x34101144);
	writel(0x6EF029, 0x34101148);
	writel(0x808010, 0x3410114C);
	writel(0x349EA100, 0x34101150);
	writel(0x00071900, 0x34101154);
	writel(0x16700100, 0x34101158);
	writel(0x34DC7487, 0x3410115C);
	writel(0x0, 0x34101200);
	writel(0x8F000000,0x34101204);
	writel(0x8F065400, 0x34101208);
	writel(0x0, 0x3410120C);
	writel(0xF00140,0x34101210);
	writel(0x1E00280, 0x34101214);
	writel(0x2, 0x34101300);
	writel(0xFF005001, 0x34101304);
	writel(0x8C0100, 0x3410130C);
	writel(0x100, 0x34101310);
	writel(0x89000000, 0x34101314);
	writel(0xF00000, 0x34101318);

	if (LOGO_YUV == type) {
		writel(0x00008001, 0x34101190);
		writel(0x80000040, 0x34101500);
		writel((y<<16) + x, 0x34101504);
		writel((ph<<16) + pw, 0x34101508);
		writel(pp, 0x3410150C);
		writel(addr - pp * bpp, 0x34101510);
		writel(addr + pp*ph, 0x34101514);
		writel((y<<16) + x, 0x34101518);
		writel(0x024002D0, 0x3410151c);
		writel(0x00020000, 0x34101520);
		writel(0x00002000, 0x34101524);
		writel(0x00000000, 0x34101528);
		writel(0x00100000, 0x3410152C);
	} else {
		writel(0x00008000, 0x34101190);
		if (LOGO_RGB_8888 == type) {
			writel(0x80000032, 0x34101400);
		} else if (LOGO_RGB_1555 == type) {
			writel(0x80000000, 0x34101400);
		}
		writel(0xFF005001, 0x34101404);
		writel((ph<<16) + pw, 0x3410140C);
		writel(pp, 0x34101410);
		writel(addr - pp*bpp, 0x34101414);
		writel((y<<16) + x, 0x34101418);
	}
	writel(0x0, 0x34101020);
	writel(0x0, 0x34101024);
	writel(0x0, 0x34101028);
	writel(0x1, 0x3410102C);
	writel(0x1, 0x34101034);
	writel(0x1, 0x34101030);
}

/*
 * int x         : logo display position x
 * int y         : logo display position x
 * int w         : logo display width
 * int h         : logo display height
 * uint32_t addr : logo source address
 * int pw        : logo source picture width
 * int pp        : logo source picture pitch
 * int ph        : logo source picture height
 *
 * for example:
 * 1. full screen osd:
 * fast_osd(0, 0, 1024, 600, 0x40000000, 1024, 1024, 600);
 * 2. full screen osd with smaller picture(yuv only):
 * fast_osd(0, 0, 256, 150, 0x40000000, 1024, 1024, 600);
 * 3. topleft 1/16 display area osd with same-sized picture
 * fast_osd(0, 0, 256, 150, 0x40000000, 256, 150, 256);
 */

void fast_osd(int type, int x, int y, int w, int h, uint32_t addr, int pw, int pp, int ph)
{
	//[0]_vou_cmd_set_dev_output_info[1024x768/1/60]65000kHz -> 65000kHz
	//							|	 [1280x720/1/60]74250kHz -> 74250kHz
	//                          |    [1920x1080/1/60]148500kHz -> 148500kHz
	//[0]_vou_cmd_toggle_dev=1
	//[0]kick off start device!

	writel(0x00000000, 0x34104004);
	writel(0x00000000, 0x34108004);
	writel(0xFFC00, 0x34104E00);
	writel(0xFFC00, 0x34104E04);
	writel(0x00000000, 0x34104E08);
	writel(0x0, 0x34104E0C);
	writel(0x5F, 0x34104E14);
	writel(0x1, 0x34104E18);
	writel(0x1, 0x34104E4C);
	writel(0x10000, 0x341041A8);
	writel(0x8000, 0x34104180);
	writel(0x8000, 0x34104190);
	writel(0x1, 0x34104100);
	writel(0x008080FF, 0x3410410C);
	writel(0x00FFFFFF, 0x34104128);

#if defined LOGO_W_1024_H_768
	writel(0x03000400, 0x34104110);
	writel(0x0127053F, 0x34104114);
	writel(0x00220325, 0x34104118);
#elif defined LOGO_W_1920_H_1080
	writel(0x04380780, 0x34104110);
	writel(0x00BF0897, 0x34104114);
	writel(0x00280464, 0x34104118);
#elif defined LOGO_W_1280_H_720
	writel(0x02D00500, 0x34104110);
	writel(0x01030671, 0x34104114);
	writel(0x001802ED, 0x34104118);
#endif

	writel(0x0, 0x3410411C);
	writel(0x0, 0x34104120);
	writel(0xFF00FF00, 0x34104108);
	writel(0x1F001F, 0x34104124);
	writel(0x8080EB, 0x34104130);
	writel(0x9210D2, 0x34104134);
	writel(0x10A6AA, 0x34104138);
	writel(0x223691, 0x3410413C);
	writel(0xDECA6A, 0x34104140);
	writel(0xF05A51, 0x34104144);
	writel(0x6EF029, 0x34104148);
	writel(0x808010, 0x3410414C);
	writel(0x349EA100, 0x34104150);
	writel(0x71900, 0x34104154);
	writel(0x16700100, 0x34104158);
	writel(0x34DC7487, 0x3410415C);
	writel(0x3, 0x34104160);
	writel(0x5F6CFF, 0x34104164);
	writel(0x58FF4C, 0x34104168);
	writel(0x0, 0x341041A0);

	if (LOGO_YUV == type) {
		writel(0x00008001, 0x34104180);
		writel(0x80000000, 0x34105000);
		writel((y<<16) + x, 0x34105004);
		writel((ph<<16) + pw, 0x34105008);
		writel(pp, 0x3410500c);
		writel(addr, 0x34105010);
		writel(addr + pp*ph, 0x34105014);
		writel((y<<16) + x, 0x34105018);
		writel((h<<16) + w, 0x3410501c);
		writel(0x00020000, 0x34105020);
		writel(0x00002000, 0x34105024);
		writel(0x00000000, 0x34105028);
		writel(0x00100000, 0x3410502c);
	} else {
		// w & h are not used

	    if (LOGO_RGB_8888 == type) {
			writel(0x80000032, 0x34104400); //argb8888
	    } else if (LOGO_RGB_1555 == type) {
			writel(0x80000000, 0x34104400);
	    }
		writel(0xFF008001, 0x34104404);
		writel((y<<16) + x, 0x34104408);
		writel((ph<<16) + pw, 0x3410440C);
		writel(pp, 0x34104410);
		writel(LOGO_RGB_ADDR_MEM, 0x34104414); //rgb address
		writel(0x0, 0x34104418);
	}
	writebs(0x0000000b, 0x34204002);
	writebc(0x0000000b, 0x34204002);

#if defined LOGO_W_1024_H_768
	writelr(0x00042384, 0x18000048, 0, 20);
	udelay(10);
	writelr(0x00002384, 0x18000048, 0, 20);
	writelr(0x00042384, 0x18000050, 0, 20);
	udelay(10);
	writelr(0x00002384, 0x18000050, 0, 20);
	writeb(0x78, 0x34201000);
	writeb(0x80, 0x34201001);
	writeb(0x07, 0x34201002);
	writeb(0x18, 0x34201003);
	writeb(0x01, 0x34201004);
	writeb(0x38, 0x34201005);
	writeb(0x04, 0x34201006);
	writeb(0x2d, 0x34201007);
	writeb(0x58, 0x34201008);
	writeb(0x00, 0x34201009);
	writeb(0x2c, 0x3420100a);
	writeb(0x00, 0x3420100b);
	writeb(0x04, 0x3420100c);
	writeb(0x05, 0x3420100d);
	writeb(0x0c, 0x34201011);
	writeb(0x20, 0x34201012);
	writeb(0x51, 0x34201013);
	writeb(0x00, 0x34201014);
	writeb(0x00, 0x34201015);
	writeb(0x3f, 0x34201016);
#elif defined LOGO_W_1920_H_1080
	writelr(0x000403a9, 0x18000048, 0, 20);
	udelay(10);
	writelr(0x000003a9, 0x18000048, 0, 20);
	writelr(0x000403a9, 0x18000050, 0, 20);
	udelay(10);
	writelr(0x000003a9, 0x18000050, 0, 20);
	writeb(0x18, 0x34201000);
	writeb(0x00, 0x34201001);
	writeb(0x04, 0x34201002);
	writeb(0x40, 0x34201003);
	writeb(0x01, 0x34201004);
	writeb(0x00, 0x34201005);
	writeb(0x03, 0x34201006);
	writeb(0x26, 0x34201007);
	writeb(0x18, 0x34201008);
	writeb(0x00, 0x34201009);
	writeb(0x88, 0x3420100a);
	writeb(0x00, 0x3420100b);
	writeb(0x03, 0x3420100c);
	writeb(0x06, 0x3420100d);
	writeb(0x0c, 0x34201011);
	writeb(0x20, 0x34201012);
	writeb(0x21, 0x34201013);
	writeb(0xFF, 0x34201014);
	writeb(0x15, 0x34201015);
	writeb(0x00, 0x34201016);
#elif defined LOGO_W_1280_H_720
	writelr(0x000423A9, 0x18000048, 0, 20);
	udelay(10);
	writelr(0x000023A9, 0x18000048, 0, 20);
	writelr(0x000423A9, 0x18000050, 0, 20);
	udelay(10);
	writelr(0x000023A9, 0x18000050, 0, 20);
	writeb(0x78, 0x34201000);
	writeb(0x00, 0x34201001);
	writeb(0x05, 0x34201002);
	writeb(0x72, 0x34201003);
	writeb(0x01, 0x34201004);
	writeb(0xD0, 0x34201005);
	writeb(0x02, 0x34201006);
	writeb(0x1E, 0x34201007);
	writeb(0x6E, 0x34201008);
	writeb(0x00, 0x34201009);
	writeb(0x28, 0x3420100a);
	writeb(0x00, 0x3420100b);
	writeb(0x05, 0x3420100c);
	writeb(0x05, 0x3420100d);
	writeb(0x0c, 0x34201011);
	writeb(0x20, 0x34201012);
	writeb(0x21, 0x34201013);
	writeb(0xFF, 0x34201014);
	writeb(0x15, 0x34201015);
	writeb(0x00, 0x34201016);
#endif

	writeb(0x00000001, 0x34204005);
	writeb(0x00000000, 0x34204005);
	writel(0x000c000c, 0x28500078);
	writel(0x000c000c, 0x2850007c);
	writel(0x0, 0x34104020);
	writel(0x0, 0x34104024);
	writel(0x0, 0x34104028);
	writel(0x1, 0x3410402C);
	writel(0x1, 0x34104034);
	writel(0x1, 0x34104030);

}

void logo_color(void)
{
	static int idx = 0;
	int i, size;
#if (LOGO_RGB_FORMAT == LOGO_RGB_1555)
	unsigned short colors[3] = {0xF800, 0x0360, 0x003E};
	unsigned short* ptr = (unsigned short *)LOGO_RGB_ADDR_MEM;
#elif (LOGO_RGB_FORMAT == LOGO_RGB_8888)
	unsigned int colors[3] = {0xFF0000FF, 0xFF00FF00, 0xFFFE0000};
	unsigned int* ptr = (unsigned int *)LOGO_RGB_ADDR_MEM;
#endif
	char* name[3] = {"Red", "Green", "Blue"};

	size = LOGO_W * LOGO_H;

	printf("\t%s\n", name[idx]);

	for (i = 0; i < size; i++) {
		ptr[i] = colors[idx];
	}

	idx++;
	idx = idx % 3;
}

void vou_global(void)
{
	writel(0x00000003, 0x28100200);
	writel(0x03800380, 0x2850004c);
	writel(0xa3900380, 0x28500050);
	writel(0x037FFFFF, 0x34000000);
	writel(0x00000040, 0x34000014);
	writel(0x00000000, 0x34000014);

	writel(0x1,0x34100010);
	writel(0x1, 0x34100014);
	writel(0x0, 0x34100018);
	writel(0x0, 0x34100040);
	writel(0x7, 0x34100060);
	writel(0xF, 0x34100064);
	writel(0x200, 0x34100068);
	writel(0x400, 0x3410006C);
	writel(0x7, 0x34100070);
	writel(0xF, 0x34100074);
	writel(0x80, 0x34100078);
	writel(0x100, 0x3410007C);
	writelr(0x0, 0x1800004c, 6, 1);
	writelr(0x0, 0x1800004c, 10, 1);
	writel(0x1, 0x34100020);
	writelr(0x1, 0x1800004c, 6, 1);
	writelr(0x1, 0x1800004c, 10, 1);
	writel(0x1, 0x34100024);
	writel(0x1, 0x34104010);
	writel(0x1, 0x34104014);
	writel(0x1, 0x341000F8);
	writel(0x4, 0x341000FC);
	writelr(0x0, 0x1800004c, 6, 1);
	writelr(0x0, 0x1800004c, 10, 1);
	writelr(0x00000001, 0x34000078, 0, 1);
	writelr(0x00000001, 0x34000078, 2, 1);
	writelr(0x1, 0x1800004c, 6, 1);
	writelr(0x1, 0x1800004c, 10, 1);

}

void logo_display(void)
{
    vou_global();
#if (LOGO_USE_YUV == 0)
	fast_osd(LOGO_RGB_FORMAT, 0, 0, LOGO_W, LOGO_H, LOGO_RGB_ADDR_MEM, LOGO_W, LOGO_W, LOGO_H);
#else
	fast_osd(LOGO_YUV, 0, 0, LOGO_W, LOGO_H, LOGO_YUV_ADDR_MEM, LOGO_W, LOGO_W, LOGO_H);
#endif
}

void boot_logo(void)
{
	//printf("start boot_logo!\n");
	logo_color();
	logo_display();
	//printf("end boot_logo!\n");
}
