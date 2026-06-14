#include <common.h>
#include <command.h>
#include <spi.h>
#include <spi_flash.h>

/* vo */
#define LOGO_VO_NONE				(1)
#define LOGO_VO_1024x768			(2)
#define LOGO_VO_1920x1080			(3)
#define LOGO_VO_PAL					(4)
#define LOGO_VO_NTSC				(5)
#define LOGO_VO_ILI9881				(6)
#define LOGO_VO_EK79007				(7)

#define LOGO_VO_HDMI				(LOGO_VO_1920x1080)
#define LOGO_VO_CVBS				(LOGO_VO_NONE)
#define LOGO_VO_MIPI				(LOGO_VO_NONE)

/* format */
#define LOGO_YUV					(1)
#define LOGO_RGB_1555				(2)
#define LOGO_RGB_8888				(3)
#define LOGO_FORMAT					(LOGO_YUV)


typedef struct {
	uint32_t Clock;
	uint32_t Hactvie;
	uint32_t Vactive;
	uint32_t FrameMode;
	uint32_t FrameRate;
	uint32_t PixelRate;
	uint32_t Htotal;
	uint32_t Vtotal;
	uint32_t Hblank;
	uint32_t Vblank;
	uint32_t Hfront;
	uint32_t Hsync;
	uint32_t Hback;
	uint32_t Hpol;
	uint32_t Vfront;
	uint32_t Vsync;
	uint32_t Vback;
	uint32_t Vpol;
	uint32_t Alen;
	uint32_t Aline;
	uint32_t Hlen;
	uint32_t StartBlen;
	uint32_t EndBlen;
	uint32_t TVLine;
	uint32_t TBLineOfs;
	uint32_t TEndBLine;
	uint32_t BVLine;
	uint32_t BBLineOfs;
	uint32_t BEndBLine;
	uint32_t HSyncLength;
	uint32_t VSyncLength0;
} vou_output_t;



/*
	vou sync info configration
*/
static vou_output_t vou_out = {
#if (LOGO_VO_HDMI == LOGO_VO_1024x768)
	.Clock = 0x2384,// 65MHz
	.Hactvie = 1024,
	.Vactive = 768,
	.FrameRate = 60,
	.Hfront = 24,
	.Hback = 160,
	.Hsync = 136,
	.Hpol = 0,
	.Vfront = 3,
	.Vback = 29,
	.Vsync = 6,
	.Vpol = 0,
#elif (LOGO_VO_HDMI == LOGO_VO_1920x1080)
	.Clock = 0x03A9,// 148.5MHz
	.Hactvie = 1920,
	.Vactive = 1080,
	.FrameRate = 60,
	.Hfront = 88,
	.Hback = 44,
	.Hsync = 148,
	.Hpol = 1,
	.Vfront = 4,
	.Vback = 36,
	.Vsync = 5,
	.Vpol = 1,
#elif (LOGO_VO_MIPI == LOGO_VO_ILI9881)
	.Clock = 0,
	.Hactvie = 800,
	.Vactive = 1280,
	.FrameRate = 60,
	.Hfront = 60,
	.Hback = 30,
	.Hsync = 30,
	.Hpol = 1,
	.Vfront = 16,
	.Vback = 14,
	.Vsync = 2,
	.Vpol = 1,
#elif (LOGO_VO_MIPI == LOGO_VO_EK79007)
	.Clock = 0,
	.Hactvie = 1024,
	.Vactive = 600,
	.FrameRate = 60,
	.Hfront = 160,
	.Hback = 120,
	.Hsync = 40,
	.Hpol = 1,
	.Vfront = 12,
	.Vback = 18,
	.Vsync = 5,
	.Vpol = 1,
#endif
};



static unsigned char * logo_i_addr = NULL;
static unsigned char * logo_o_addr = NULL;
static int logo_w = 0;
static int logo_h = 0;

static uint32_t readl(uint32_t addr)
{
	uint32_t value = *(volatile uint32_t*)addr;
	return value;
}

static void writel(uint32_t value, uint32_t addr)
{
	*(volatile uint32_t*)addr = value;
}

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

static void writelr(uint32_t value, uint32_t addr, uint32_t start, uint32_t bit)
{
	uint32_t region = (0xFFFFFFFFU << ( start + bit )) | (0xFFFFFFFFULL >> ( 32 - start ));
	uint32_t tmp = readl(addr);
	tmp &= region;
	tmp |= (~region) & (value << start);
	writel(tmp, addr);
}

#if (LOGO_VO_MIPI == LOGO_VO_NONE)
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
#else
void writelr_delay(uint32_t value, uint32_t addr, uint32_t start, uint32_t bit)
{
	uint32_t region = (0xFFFFFFFFU << ( start + bit )) | (0xFFFFFFFFULL >> ( 32 - start ));
	uint32_t tmp = readl(addr);
	tmp &= region;
	tmp |= (~region) & (value << start);
	writel(tmp, addr);
	mdelay(1);
}
#endif


void fast_osd_init(void)
{
	int Hactvie = vou_out.Hactvie;
	int Vactive = vou_out.Vactive;
	int FrameRate = vou_out.FrameRate;
	int Hfront = vou_out.Hfront;
	int Hsync = vou_out.Hsync;
	int Hback = vou_out.Hback;
	int Vfront = vou_out.Vfront;
	int Vsync = vou_out.Vsync;
	int Vback = vou_out.Vback;

	vou_out.Hblank = Hfront + Hsync + Hback;
	vou_out.Vblank = Vfront + Vsync + Vback;
	vou_out.Htotal = Hactvie + vou_out.Hblank;
	vou_out.Vtotal = Vactive + vou_out.Vblank;
	vou_out.PixelRate = FrameRate * vou_out.Htotal * vou_out.Vtotal;
	vou_out.Alen = Hactvie;
	vou_out.Aline = Vactive;
	vou_out.Hlen = vou_out.Htotal - 1;
	vou_out.StartBlen = Hsync + Hback - 1;
	vou_out.EndBlen = Hfront;
	vou_out.TVLine = vou_out.Vtotal - 1;
	vou_out.TBLineOfs = Vsync + Vback - 1;
	vou_out.TEndBLine = Vfront;
	vou_out.BVLine = 0;
	vou_out.BBLineOfs = 0;
	vou_out.BEndBLine = 0;
	vou_out.HSyncLength = Hsync -1;
	vou_out.VSyncLength0 = Vsync - 1;

#if (LOGO_VO_HDMI != LOGO_VO_NONE)
	writels(0x00003f1f, 0x34000000);
	writelc(0x00000400, 0x34000078);
	writels(0x00000003, 0x28100230);
	writel(0x00000300, 0x34100010);
	writels(0x0000007f, 0x34000014);
	writelc(0x0000007f, 0x34000014);
	writel(0x00cc0000, 0x34100010);
	writel(0x0000000f, 0x34100004);
	writelc(0x00004000, 0x3400007c);
	writel(0x00000000, 0x28000068);
	writel(0x00000000, 0x34100040);

	writel(0x00000008, 0x34100020);
	writel(0x0000000a, 0x34100024);
	writel(0x00000300, 0x34100028);
	writel(0x00000600, 0x3410002c);
	writel(0x00000007, 0x34100030);
	writel(0x0000000a, 0x34100034);
	writel(0x00000080, 0x34100038);
	writel(0x00000100, 0x3410003c);
#endif
}

#if (LOGO_VO_MIPI != LOGO_VO_NONE)
static uint32_t mipi_dphy_hal_get_pixel_clk_div(void)
{
/*
pll---->div--->vou
	 \___1/8--_>mipi_ctrl
*/
	uint32_t val = 0;
	uint32_t i = 0;
	uint32_t clkDiv = 0;
	uint32_t clk_div_tab[16][2] =
	{ /*val , div*/
		{0,  2},
		{1,  3},
		{2,  4},
		{3,  5},
		{4,  6},
		{5,  7},
		{6,  8},
		{7,  9},
		{8,  10},
		{9,  11},
		{10, 12},
		{11, 13},
		{12, 14},
		{13, 15},
		{14, 16},
		{15, 1}
	};

	val = readl(0x3450005c);
	val &= 0x0f;

	for (i = 0; i < 16; i++)
	{
		if (val == clk_div_tab[i][0]) {
			clkDiv = clk_div_tab[i][1];
			break;
		}
	}

	return clkDiv;
}

#endif


/*
 * int x		 : logo display position x
 * int y		 : logo display position x
 * int w		 : logo display width
 * int h		 : logo display height
 * uint32_t addr : logo source address
 * int pw		 : logo source picture width
 * int pp		 : logo source picture pitch
 * int ph		 : logo source picture height
 *
 * for example:
 * 1. full screen osd:
 * fast_osd(0, 0, 1024, 600, 0x40000000, 1024, 1024, 600);
 * 2. full screen osd with smaller picture(yuv only):
 * fast_osd(0, 0, 256, 150, 0x40000000, 1024, 1024, 600);
 * 3. topleft 1/16 display area osd with same-sized picture
 * fast_osd(0, 0, 256, 150, 0x40000000, 256, 150, 256);
 */

void fast_osd_hdmi(int type, int x, int y, int w, int h, uint32_t addr, int pw, int pp, int ph)
{
#if (LOGO_VO_HDMI == LOGO_VO_1024x768 || LOGO_VO_HDMI == LOGO_VO_1920x1080)
	writebs(0x0b, 0x34204002);
	writebc(0x0000000b, 0x34204002);

	writelr(0x00040000|vou_out.Clock, 0x2800006c, 0, 20);//writelr(0x00042384, 0x2800006c, 0, 20);
	writelr(vou_out.Clock, 0x2800006c, 0, 20);//writelr(0x00002384, 0x2800006c, 0, 20);

	writeb(0x10, 0x34201000);
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
	writeb(0xff, 0x34201014);
	writeb(0x15, 0x34201015);
	writeb(0x00, 0x34201016);
	writeb(0x01, 0x34204005);
	writeb(0x00, 0x34204005);
	writel(0x000c000c, 0x28500078);
	writel(0x000c000c, 0x2850007c);
	writel(0x03d403d4, 0x2850004c);
	writel(0xa39003d4, 0x28500050);

	writel(0x00000000, 0x34101600);
	writel(0x0001fe00, 0x34101604);
	writel(0x00000001, 0x34101608);
	writel(0x00000000, 0x3410160c);
	writel(0x000007ff, 0x34101610);
	writel(0x0001fff4, 0x34101614);
	writel(0x0003f80e, 0x34101618);
	writel(0x00000001, 0x3410161c);
	writel(0x00000fff, 0x34101620);
	writel(0x0001f3e9, 0x34101624);
	writel(0x0003ee1f, 0x34101628);
	writel(0x00000002, 0x3410162c);
	writel(0x000013fe, 0x34101630);
	writel(0x0001e3e1, 0x34101634);
	writel(0x0003e233, 0x34101638);
	writel(0x0003fe04, 0x3410163c);
	writel(0x000017fe, 0x34101640);
	writel(0x0001c9dc, 0x34101644);
	writel(0x0003d847, 0x34101648);
	writel(0x0003fe05, 0x3410164c);
	writel(0x000019fe, 0x34101650);
	writel(0x0001add8, 0x34101654);
	writel(0x0003cc5c, 0x34101658);
	writel(0x0003fe07, 0x3410165c);
	writel(0x000019fd, 0x34101660);
	writel(0x00018dd7, 0x34101664);
	writel(0x0003c272, 0x34101668);
	writel(0x0003fc09, 0x3410166c);
	writel(0x000019fe, 0x34101670);
	writel(0x000165d7, 0x34101674);
	writel(0x0003ba88, 0x34101678);
	writel(0x0003fc0a, 0x3410167c);
	writel(0x000017fe, 0x34101680);
	writel(0x00013dd9, 0x34101684);
	writel(0x0003b29e, 0x34101688);
	writel(0x0003fc0b, 0x3410168c);
	writel(0x000015fe, 0x34101690);
	writel(0x000111dd, 0x34101694);
	writel(0x0003aeb2, 0x34101698);
	writel(0x0003fc0c, 0x3410169c);
	writel(0x000013fe, 0x341016a0);
	writel(0x0000e7e1, 0x341016a4);
	writel(0x0003aec5, 0x341016a8);
	writel(0x0003fa0c, 0x341016ac);
	writel(0x00000fff, 0x341016b0);
	writel(0x0000b9e6, 0x341016b4);
	writel(0x0003b0d6, 0x341016b8);
	writel(0x0003fc0c, 0x341016bc);
	writel(0x00000bff, 0x341016c0);
	writel(0x00008dec, 0x341016c4);
	writel(0x0003b8e5, 0x341016c8);
	writel(0x0003fc0b, 0x341016cc);
	writel(0x000009ff, 0x341016d0);
	writel(0x000067f1, 0x341016d4);
	writel(0x0003c2f1, 0x341016d8);
	writel(0x0003fc09, 0x341016dc);
	writel(0x00000400, 0x341016e0);
	writel(0x00003ff7, 0x341016e4);
	writel(0x0003d2f9, 0x341016e8);
	writel(0x0003fe07, 0x341016ec);
	writel(0x00000200, 0x341016f0);
	writel(0x00001ffc, 0x341016f4);
	writel(0x0003e8fe, 0x341016f8);
	writel(0x0003fe03, 0x341016fc);
	writel(0x0001fe00, 0x34101700);
	writel(0x00000001, 0x34101704);
	writel(0x0001fdf9, 0x34101708);
	writel(0x00000009, 0x3410170c);
	writel(0x0001eff4, 0x34101710);
	writel(0x0003fe16, 0x34101714);
	writel(0x0001d9f1, 0x34101718);
	writel(0x0003fc25, 0x3410171c);
	writel(0x0001bbf0, 0x34101720);
	writel(0x0003fa36, 0x34101724);
	writel(0x000197f0, 0x34101728);
	writel(0x0003f64a, 0x3410172c);
	writel(0x00016ff1, 0x34101730);
	writel(0x0003f25f, 0x34101734);
	writel(0x000143f3, 0x34101738);
	writel(0x0003ee75, 0x3410173c);
	writel(0x000117f5, 0x34101740);
	writel(0x0003ea8b, 0x34101744);
	writel(0x0000ebf7, 0x34101748);
	writel(0x0003e6a1, 0x3410174c);
	writel(0x0000bff9, 0x34101750);
	writel(0x0003e2b7, 0x34101754);
	writel(0x000095fb, 0x34101758);
	writel(0x0003e0cb, 0x3410175c);
	writel(0x00006dfd, 0x34101760);
	writel(0x0003e0dd, 0x34101764);
	writel(0x00004bfe, 0x34101768);
	writel(0x0003e2ec, 0x3410176c);
	writel(0x00002dff, 0x34101770);
	writel(0x0003e8f7, 0x34101774);
	writel(0x00001200, 0x34101778);
	writel(0x0003f2fe, 0x3410177c);
	writel(0x0001fe00, 0x34101800);
	writel(0x00000001, 0x34101804);
	writel(0x0001fdf9, 0x34101808);
	writel(0x00000009, 0x3410180c);
	writel(0x0001eff4, 0x34101810);
	writel(0x0003fe16, 0x34101814);
	writel(0x0001d9f1, 0x34101818);
	writel(0x0003fc25, 0x3410181c);
	writel(0x0001bbf0, 0x34101820);
	writel(0x0003fa36, 0x34101824);
	writel(0x000197f0, 0x34101828);
	writel(0x0003f64a, 0x3410182c);
	writel(0x00016ff1, 0x34101830);
	writel(0x0003f25f, 0x34101834);
	writel(0x000143f3, 0x34101838);
	writel(0x0003ee75, 0x3410183c);
	writel(0x000117f5, 0x34101840);
	writel(0x0003ea8b, 0x34101844);
	writel(0x0000ebf7, 0x34101848);
	writel(0x0003e6a1, 0x3410184c);
	writel(0x0000bff9, 0x34101850);
	writel(0x0003e2b7, 0x34101854);
	writel(0x000095fb, 0x34101858);
	writel(0x0003e0cb, 0x3410185c);
	writel(0x00006dfd, 0x34101860);
	writel(0x0003e0dd, 0x34101864);
	writel(0x00004bfe, 0x34101868);
	writel(0x0003e2ec, 0x3410186c);
	writel(0x00002dff, 0x34101870);
	writel(0x0003e8f7, 0x34101874);
	writel(0x00001200, 0x34101878);
	writel(0x0003f2fe, 0x3410187c);
	writel(0x00003404, 0x34101a00);
	writel(0x00009a3b, 0x34101a04);
	writel(0x0000343c, 0x34101a08);
	writel(0x00000004, 0x34101a0c);
	writel(0x00003004, 0x34101a10);
	writel(0x00009a39, 0x34101a14);
	writel(0x0000383d, 0x34101a18);
	writel(0x00000005, 0x34101a1c);
	writel(0x00002c03, 0x34101a20);
	writel(0x00009838, 0x34101a24);
	writel(0x00003c3f, 0x34101a28);
	writel(0x00000006, 0x34101a2c);
	writel(0x00002803, 0x34101a30);
	writel(0x00009a35, 0x34101a34);
	writel(0x00003e41, 0x34101a38);
	writel(0x00000007, 0x34101a3c);
	writel(0x00002402, 0x34101a40);
	writel(0x00009634, 0x34101a44);
	writel(0x00004642, 0x34101a48);
	writel(0x00000008, 0x34101a4c);
	writel(0x00002202, 0x34101a50);
	writel(0x00009431, 0x34101a54);
	writel(0x00004a44, 0x34101a58);
	writel(0x00000009, 0x34101a5c);
	writel(0x00001e01, 0x34101a60);
	writel(0x0000962f, 0x34101a64);
	writel(0x00004e45, 0x34101a68);
	writel(0x0000000a, 0x34101a6c);
	writel(0x00001c01, 0x34101a70);
	writel(0x0000922d, 0x34101a74);
	writel(0x00005247, 0x34101a78);
	writel(0x0000000b, 0x34101a7c);
	writel(0x00001801, 0x34101a80);
	writel(0x0000902b, 0x34101a84);
	writel(0x00005648, 0x34101a88);
	writel(0x0000020c, 0x34101a8c);
	writel(0x00001600, 0x34101a90);
	writel(0x00008e29, 0x34101a94);
	writel(0x00005a49, 0x34101a98);
	writel(0x0000020e, 0x34101a9c);
	writel(0x00001400, 0x34101aa0);
	writel(0x00008c27, 0x34101aa4);
	writel(0x00005e4a, 0x34101aa8);
	writel(0x0000020f, 0x34101aac);
	writel(0x00001200, 0x34101ab0);
	writel(0x00008825, 0x34101ab4);
	writel(0x0000624a, 0x34101ab8);
	writel(0x00000411, 0x34101abc);
	writel(0x00001000, 0x34101ac0);
	writel(0x00008423, 0x34101ac4);
	writel(0x0000684b, 0x34101ac8);
	writel(0x00000412, 0x34101acc);
	writel(0x00000e00, 0x34101ad0);
	writel(0x0000841f, 0x34101ad4);
	writel(0x00006a4c, 0x34101ad8);
	writel(0x00000614, 0x34101adc);
	writel(0x00000c00, 0x34101ae0);
	writel(0x00007e1e, 0x34101ae4);
	writel(0x0000704c, 0x34101ae8);
	writel(0x00000616, 0x34101aec);
	writel(0x00000a00, 0x34101af0);
	writel(0x00007c1c, 0x34101af4);
	writel(0x0000724c, 0x34101af8);
	writel(0x00000818, 0x34101afc);
	writel(0x0000fe3d, 0x34101b00);
	writel(0x00000c3e, 0x34101b04);
	writel(0x0000fe37, 0x34101b08);
	writel(0x00000c44, 0x34101b0c);
	writel(0x0000fc31, 0x34101b10);
	writel(0x00000e4a, 0x34101b14);
	writel(0x0000f82c, 0x34101b18);
	writel(0x00001050, 0x34101b1c);
	writel(0x0000f426, 0x34101b20);
	writel(0x00001456, 0x34101b24);
	writel(0x0000ee22, 0x34101b28);
	writel(0x0000185b, 0x34101b2c);
	writel(0x0000e81d, 0x34101b30);
	writel(0x00001c61, 0x34101b34);
	writel(0x0000e019, 0x34101b38);
	writel(0x00002266, 0x34101b3c);
	writel(0x0000d615, 0x34101b40);
	writel(0x00002a6b, 0x34101b44);
	writel(0x0000cc11, 0x34101b48);
	writel(0x00003270, 0x34101b4c);
	writel(0x0000c20e, 0x34101b50);
	writel(0x00003a74, 0x34101b54);
	writel(0x0000b80c, 0x34101b58);
	writel(0x00004476, 0x34101b5c);
	writel(0x0000ac0a, 0x34101b60);
	writel(0x00004c7a, 0x34101b64);
	writel(0x0000a008, 0x34101b68);
	writel(0x0000587c, 0x34101b6c);
	writel(0x00009407, 0x34101b70);
	writel(0x0000627e, 0x34101b74);
	writel(0x00008806, 0x34101b78);
	writel(0x00006e7f, 0x34101b7c);
	writel(0x0000fe3d, 0x34101c00);
	writel(0x00000c3e, 0x34101c04);
	writel(0x0000fe37, 0x34101c08);
	writel(0x00000c44, 0x34101c0c);
	writel(0x0000fc31, 0x34101c10);
	writel(0x00000e4a, 0x34101c14);
	writel(0x0000f82c, 0x34101c18);
	writel(0x00001050, 0x34101c1c);
	writel(0x0000f426, 0x34101c20);
	writel(0x00001456, 0x34101c24);
	writel(0x0000ee22, 0x34101c28);
	writel(0x0000185b, 0x34101c2c);
	writel(0x0000e81d, 0x34101c30);
	writel(0x00001c61, 0x34101c34);
	writel(0x0000e019, 0x34101c38);
	writel(0x00002266, 0x34101c3c);
	writel(0x0000d615, 0x34101c40);
	writel(0x00002a6b, 0x34101c44);
	writel(0x0000cc11, 0x34101c48);
	writel(0x00003270, 0x34101c4c);
	writel(0x0000c20e, 0x34101c50);
	writel(0x00003a74, 0x34101c54);
	writel(0x0000b80c, 0x34101c58);
	writel(0x00004476, 0x34101c5c);
	writel(0x0000ac0a, 0x34101c60);
	writel(0x00004c7a, 0x34101c64);
	writel(0x0000a008, 0x34101c68);
	writel(0x0000587c, 0x34101c6c);
	writel(0x00009407, 0x34101c70);
	writel(0x0000627e, 0x34101c74);
	writel(0x00008806, 0x34101c78);
	writel(0x00006e7f, 0x34101c7c);

	writel(0x00000087, 0x34101f14);
	writel(0x00000005, 0x34101f18);
	writel(0x00000000, 0x34101f1c);
	writel(0x00089000, 0x34101f00);
	writel(0x200c48ee, 0x34101f04);
	writel(0x00000000, 0x34101f08);
	writel(0x02000080, 0x34101f0c);
	writel(0x00000001, 0x34101f4c);

	writel((vou_out.Vactive << 16) + vou_out.Hactvie, 0x34101024);//writel(0x03000400, 0x34101024);
	writel((vou_out.StartBlen << 16) + vou_out.Hlen, 0x34101028);//writel(0x0127053f, 0x34101028);
	writel((vou_out.TBLineOfs << 16) + vou_out.TVLine, 0x3410102C);//writel(0x00220325, 0x3410102C);
	writel((vou_out.BBLineOfs << 16) + vou_out.BVLine, 0x34101030);//writel(0x00000000, 0x34101030);

	writel(0xff00ff00, 0x34101034);

	writel(0x356ebd01, 0x34101060);
	writel(0x00069901, 0x34101064);
	writel(0x14e00101, 0x34101068);
	writel(0x33fc4861, 0x3410106c);

	if (LOGO_YUV == type) {
		writel(0x00000040, 0x34101200);
		writel((ph<<16) + pw, 0x34101208);
		writel(pp, 0x3410120c);
		writel(addr, 0x34101210);
		writel(addr + pp*ph, 0x34101214);
		writel((y<<16) + x, 0x34101218);
		writel(0x00000000, 0x34101230);
		writel(0x00000000, 0x34101234);
		writel((h<<16) + w, 0x3410121c);
		writel(0x80800001, 0x34101004);
		writel(0x00000009, 0x34101000);
	} else {
		// w & h are not used
		if (LOGO_RGB_8888 == type) {
			writel(0x00000002, 0x34101120); // mc abgr8888
			//writel(0x0000001A, 0x34101120); //hik argb8888
		} else if (LOGO_RGB_1555 == type) {
			writel(0x00000010, 0x34101120);  //mc argb1555
		}

		writel(0x00008000, 0x34101124);
		writel(0x00000000, 0x34101128);
		writel((ph<<16) + pw, 0x3410112c);
		writel(pp, 0x34101130);
		writel(addr, 0x34101134);
		writel((y<<16) + x, 0x34101138);
		writel(0x00000011, 0x34101000);
	}

	writel(0x00000006, 0x341010b0);
	writel(0x00000000, 0x34101014);
	writel(0x008080ff, 0x34101020);
	writel(0x00ffffff, 0x3410103c);
	writel(0x00000300, 0x3410100c);
	writel(0x00000001, 0x3410100c);
	writel(0xFFFFFFFF, 0x34101008);
#endif
}

void fast_osd_ili9881(int type, int x, int y, int w, int h, uint32_t addr, int pw, int pp, int ph)
{
#if (LOGO_VO_MIPI == LOGO_VO_ILI9881)
	uint32_t clkdiv = 0;

	/*pinmux*/
	writel(0x00000002, 0x2820009c);
	writel(0x00000002, 0x282000a0);
	writel(0x00000002, 0x282000a4);
	writel(0x00000002, 0x282000a8);
	writel(0x00000002, 0x282000ac);
	writel(0x00000002, 0x282000b0);
	writel(0x00000002, 0x282000b4);
	writel(0x00000002, 0x282000b8);
	writel(0x00000002, 0x282000bc);
	writel(0x00000002, 0x282000c0);
	/*vou clock apb*/
	writels(0x00003f1f, 0x34000000);
	writelc(0x00000400, 0x34000078);
	writels(0x00000003, 0x28100030);
	writels(0x0000007f, 0x34000014);
	writelc(0x0000007f, 0x34000014);
	writel(0x0000000f, 0x34100004);
	writels(0x00000300, 0x34000078);

	writelr(0x00000000, 0x343000a0, 2, 1);
	writelr(0x00000001, 0x343000b4, 0, 1);
	writelr(0x00000001, 0x343000a0, 3, 1);
	writelc(0x00002000, 0x34500064);
	writelr(0x00000001, 0x343000b4, 1, 1);
	writelr(0x00000000, 0x343000b8, 16, 1);
	writelr(0x00000001, 0x343000a0, 0, 1);
	writelr(0x00000001, 0x343000a0, 1, 1);
	writelr(0x00000000, 0x343000b4, 0, 1);
	writels(0x00002000, 0x34500064);
	writelr(0x00000000, 0x34500064, 1, 7);
	writelr(0x00000007, 0x34500064, 9, 4);
	writelr(0x00000001, 0x34500064, 13, 1);
	writelr(0x00080000, 0x34500060, 0, 32);
	writelr(0x00000002, 0x34500068, 0, 4);
	writelr(0x00000002, 0x34500068, 4, 4);
	writelr(0x00000007, 0x34500068, 8, 3);
	writelr(0x00000000, 0x34500068, 11, 3);
	writelr(0x00000001, 0x3450007c, 0, 8);
	writelr(0x00000007, 0x3450007c, 8, 8);
	writelr(0x0000001f, 0x34500074, 0, 5);
	writelr(0x00000003, 0x34500074, 5, 2);
	writelr(0x00000000, 0x34500074, 7, 3);
	writelr(0x0a0296b5, 0x3450005c, 0, 32);
	writelr(0x000000b6, 0x34500058, 0, 32);
	writelr(0x000001f4, 0x34500000, 0, 15);
	writelr(0x00002710, 0x34500004, 0, 31);

	writelr(0x00030e02, 0x34500008, 0, 32);//08

	writelr(0x00120409, 0x3450000c, 0, 32);
	writelr(0x00000100, 0x34500078, 0, 32);
	writelr(0x000001f4, 0x34500010, 0, 32);
	writelr(0x00002710, 0x34500014, 0, 32);

	writelr(0x030e0609, 0x34500018, 0, 32);//18

	writelr(0x00000100, 0x3450001c, 0, 32);

	writelr(0x001c0a23, 0x34500020, 0, 32);//20

	writelr(0x000001f4, 0x34500024, 0, 32);
	writelr(0x00002710, 0x34500028, 0, 32);

	writelr(0x030e0609, 0x3450002c, 0, 32);//2c

	writelr(0x00000100, 0x34500030, 0, 32);
	writelr(0x000001f4, 0x34500034, 0, 32);
	writelr(0x00002710, 0x34500038, 0, 32);

	writelr(0x030e0609, 0x3450003c, 0, 32);//3c

	writelr(0x00000100, 0x34500040, 0, 32);
	writelr(0x000001f4, 0x34500044, 0, 32);
	writelr(0x00002710, 0x34500048, 0, 32);

	writelr(0x030e0609, 0x3450004c, 0, 32);//4c

	writelr(0x00000100, 0x34500050, 0, 32);
	writelr(0x00000005, 0x34500054, 0, 8);

	writelr(0x0000007f, 0x34500074, 0, 7);
	writelr(0x00000000, 0x34500074, 7, 3);

	writelr(0x00000001, 0x3450007c, 0, 8);
	writelr(0x0000000a, 0x3450007c, 8, 8);

	writelr(0x00000000, 0x3450006c, 0, 3);
	writelr(0x00000001, 0x3450006c, 3, 1);
	writelr(0x00000000, 0x3450006c, 4, 2);
	writelr(0x00000000, 0x3450006c, 6, 1);

	udelay(1000);

	writelr(0x00000000, 0x34500064, 1, 7);
	writelr(0x00000007, 0x34500064, 9, 4);//
	writelr(0x00000001, 0x34500064, 13, 1);

	writelr(0x00000000, 0x3430000c, 0, 2);
	writelr(0x00000005, 0x34300010, 0, 4);
	writelr(0x00000001, 0x34300014, 2, 1);
	writelr(0x00000001, 0x34300014, 1, 1);
	writelr(0x0000000c, 0x34300008, 0, 8);
	writelr(0x00000001, 0x34300008, 8, 8);
	writel(0xffffff01, 0x34300068);
	writelr(0x00000001, 0x3430001c, 0, 2);
	writelr(0x00000004, 0x34300020, 0, 4);
	writelr(0x00000001, 0x34300020, 16, 2);
	writelr(0x00000001, 0x34300024, 0, 1);
	writelr(0x00000064, 0x34300028, 16, 16);
	writelr(0x00000008, 0x34300028, 0, 16);
	writelr(0x00000001, 0x3430002c, 2, 1);
	writelr(0x00000001, 0x3430002c, 3, 1);
	writelr(0x00000001, 0x3430002c, 4, 1);
	writelr(0x00000000, 0x34300030, 0, 2);
	writelr(0x00000001, 0x34300034, 0, 1);
	writelr(0x00000002, 0x34300038, 0, 2);
	writelr(0x00000000, 0x34300038, 8, 1);
	writelr(0x00000001, 0x34300038, 9, 1);
	writelr(0x00000000, 0x34300038, 10, 1);
	writelr(0x00000000, 0x34300038, 11, 1);
	writelr(0x00000000, 0x34300038, 12, 1);
	writelr(0x00000000, 0x34300038, 13, 1);
	writelr(vou_out.Hactvie , 0x3430003c, 0, 14);//writelr(0x00000320, 0x3430003c, 0, 14);
	writelr(0x00000001, 0x34300040, 0, 13);
	writelr(0x0000000a, 0x34300044, 0, 13);

	clkdiv = mipi_dphy_hal_get_pixel_clk_div();

	writelr((vou_out.Hsync * clkdiv / 8), 0x34300048, 0, 12);//writelr(0x00000009, 0x34300048, 0, 12);
	writelr((vou_out.Hback * clkdiv / 8), 0x3430004c, 0, 12);//writelr(0x00000014, 0x3430004c, 0, 12);
	writelr(((vou_out.Hsync + vou_out.Hback + vou_out.Hfront + vou_out.Hactvie) * clkdiv / 8), 0x34300050, 0, 15);//writelr(0x000003a5, 0x34300050, 0, 15);
	writelr(vou_out.Vsync, 0x34300054, 0, 10);//writelr(0x00000004, 0x34300054, 0, 10);
	writelr(vou_out.Vback, 0x34300058, 0, 10);//writelr(0x00000018, 0x34300058, 0, 10);
	writelr(vou_out.Vfront, 0x3430005c, 0, 10);//writelr(0x00000014, 0x3430005c, 0, 10);
	writelr(vou_out.Vactive, 0x34300060, 0, 14);//writelr(0x00000500, 0x34300060, 0, 14);
	writelr(0x00000000, 0x34300018, 16, 8);
	writelr(0x00000000, 0x34300018, 0, 8);
	writelr(0x0000006e, 0x3430009c, 0, 15);
	writelr(0x00000025, 0x3430009c, 16, 8);
	writelr(0x00000000, 0x34300064, 0, 16);
	writelr(0x00000001, 0x34300094, 0, 1);
	writelr(0x00000000, 0x34300078, 0, 16);
	writelr(0x00000000, 0x34300078, 16, 16);
	writelr(0x00000000, 0x3430007c, 0, 16);
	writelr(0x00000000, 0x34300080, 0, 16);
	writelr(0x00000000, 0x34300084, 0, 16);
	writelr(0x00000000, 0x34300088, 0, 16);
	writelr(0x00000000, 0x3430008c, 0, 16);
	writelr(0x00000087, 0x34300098, 0, 10);
	writelr(0x00000025, 0x34300098, 16, 10);
	writelr(0x00000000, 0x343000f4, 0, 15);
	writelr(0x00000028, 0x343000a4, 8, 8);
	writelr(0x00000003, 0x343000a4, 0, 2);
	writelr(0x00000000, 0x343000a8, 0, 4);
	writelr(0x00000000, 0x343000ac, 0, 4);
	writelr(0x00000000, 0x34300100, 0, 1);
	writelr(0x00000000, 0x34300100, 8, 1);
	writelr(0x00000000, 0x34300100, 16, 1);
	writel(0xffffffff, 0x343000c4);
	writel(0xffffffff, 0x343000c8);
	writelr(0x00000000, 0x34300004, 0, 1);
	writelr(0x00000001, 0x34300004, 0, 1);

	writels(0x00004000, 0x3400007c); //vou->mipi dsi
	writel(0x000002ac, 0x340000a4);

	//----------------select size 1280/800/1/60------------------------
	writel(0x00000008, 0x34100020);
	writel(0x0000000a, 0x34100024);
	writel(0x00000300, 0x34100028);
	writel(0x00000600, 0x3410002c);
	writel(0x00000007, 0x34100030);
	writel(0x0000000a, 0x34100034);
	writel(0x00000080, 0x34100038);
	writel(0x00000100, 0x3410003c);
	writel(0x00000001, 0x34100040);

	//vou:[0]kick off start device
	writel(0x00000007, 0x34101f14);
	writel(0x00000003, 0x34101f18);
	writel(0x00000000, 0x34101f1c);

	printf("start reset GPIO7-1\n");

	writelr(0x00000001, 0x1b070008, 1, 1); // DIR:  set GPIO7-1 output
	writelr(0x00000001, 0x1b070004, 1, 1); // DMSK: enable GPIO7-1

	writelr(0x00000001, 0x1b070000, 1, 1); // DATA: GPIO7-1 output 1;
	mdelay(20);
	writelr(0x00000001, 0x1b070000, 1, 0); // DATA: GPIO7-1 output 0;
	mdelay(60);
	writelr(0x00000001, 0x1b070000, 1, 1); // DATA: GPIO7-1 output 1;
	mdelay(120);

/*---1280*800 register config-------------------------start*/
	printf("start write panel register\n");
	writel(0x038198ff, 0x34300070);  //long
	mdelay(1);
//	writelr_delay(0x00000439, 0x3430006c, 0, 24); //bist
	writelr_delay(0x00000429, 0x3430006c, 0, 24); //normal

	writelr_delay(0x00000115, 0x3430006c, 0, 24);
	writelr_delay(0x00000215, 0x3430006c, 0, 24);
	writelr_delay(0x00530315, 0x3430006c, 0, 24);
	writelr_delay(0x00000415, 0x3430006c, 0, 24);
	writelr_delay(0x00000515, 0x3430006c, 0, 24);
	writelr_delay(0x00080615, 0x3430006c, 0, 24);
	writelr_delay(0x00000715, 0x3430006c, 0, 24);
	writelr_delay(0x00000815, 0x3430006c, 0, 24);
	writelr_delay(0x00000915, 0x3430006c, 0, 24);
	writelr_delay(0x00000a15, 0x3430006c, 0, 24);
	writelr_delay(0x00000b15, 0x3430006c, 0, 24);
	writelr_delay(0x00000c15, 0x3430006c, 0, 24);
	writelr_delay(0x00000d15, 0x3430006c, 0, 24);
	writelr_delay(0x00000e15, 0x3430006c, 0, 24);
	writelr_delay(0x00260f15, 0x3430006c, 0, 24);
	writelr_delay(0x00261015, 0x3430006c, 0, 24);
	writelr_delay(0x00001115, 0x3430006c, 0, 24);
	writelr_delay(0x00001215, 0x3430006c, 0, 24);
	writelr_delay(0x00001315, 0x3430006c, 0, 24);
	writelr_delay(0x00001415, 0x3430006c, 0, 24);
	writelr_delay(0x00001515, 0x3430006c, 0, 24);
	writelr_delay(0x00001615, 0x3430006c, 0, 24);
	writelr_delay(0x00001715, 0x3430006c, 0, 24);
	writelr_delay(0x00001815, 0x3430006c, 0, 24);
	writelr_delay(0x00001915, 0x3430006c, 0, 24);
	writelr_delay(0x00001a15, 0x3430006c, 0, 24);
	writelr_delay(0x00001b15, 0x3430006c, 0, 24);
	writelr_delay(0x00001c15, 0x3430006c, 0, 24);
	writelr_delay(0x00001d15, 0x3430006c, 0, 24);
	writelr_delay(0x00401e15, 0x3430006c, 0, 24);
	writelr_delay(0x00c01f15, 0x3430006c, 0, 24);
	writelr_delay(0x00062015, 0x3430006c, 0, 24);
	writelr_delay(0x00012115, 0x3430006c, 0, 24);
	writelr_delay(0x00072215, 0x3430006c, 0, 24);
	writelr_delay(0x00002315, 0x3430006c, 0, 24);
	writelr_delay(0x008a2415, 0x3430006c, 0, 24);
	writelr_delay(0x008a2515, 0x3430006c, 0, 24);
	writelr_delay(0x00002615, 0x3430006c, 0, 24);
	writelr_delay(0x00002715, 0x3430006c, 0, 24);
	writelr_delay(0x00332815, 0x3430006c, 0, 24);
	writelr_delay(0x00332915, 0x3430006c, 0, 24);
	writelr_delay(0x00002a15, 0x3430006c, 0, 24);
	writelr_delay(0x00002b15, 0x3430006c, 0, 24);
	writelr_delay(0x00082c15, 0x3430006c, 0, 24);
	writelr_delay(0x00082d15, 0x3430006c, 0, 24);
	writelr_delay(0x000b2e15, 0x3430006c, 0, 24);
	writelr_delay(0x000b2f15, 0x3430006c, 0, 24);
	writelr_delay(0x00003015, 0x3430006c, 0, 24);
	writelr_delay(0x00003115, 0x3430006c, 0, 24);
	writelr_delay(0x00423215, 0x3430006c, 0, 24);
	writelr_delay(0x00003315, 0x3430006c, 0, 24);
	writelr_delay(0x00003415, 0x3430006c, 0, 24);
	writelr_delay(0x000a3515, 0x3430006c, 0, 24);
	writelr_delay(0x00003615, 0x3430006c, 0, 24);
	writelr_delay(0x00083715, 0x3430006c, 0, 24);
	writelr_delay(0x003c3815, 0x3430006c, 0, 24);
	writelr_delay(0x00003915, 0x3430006c, 0, 24);
	writelr_delay(0x00003a15, 0x3430006c, 0, 24);
	writelr_delay(0x00003b15, 0x3430006c, 0, 24);
	writelr_delay(0x00003c15, 0x3430006c, 0, 24);
	writelr_delay(0x00003d15, 0x3430006c, 0, 24);
	writelr_delay(0x00003e15, 0x3430006c, 0, 24);
	writelr_delay(0x00003f15, 0x3430006c, 0, 24);
	writelr_delay(0x00004015, 0x3430006c, 0, 24);
	writelr_delay(0x00004115, 0x3430006c, 0, 24);
	writelr_delay(0x00004215, 0x3430006c, 0, 24);
	writelr_delay(0x00084315, 0x3430006c, 0, 24);
	writelr_delay(0x00004415, 0x3430006c, 0, 24);
	writelr_delay(0x00015015, 0x3430006c, 0, 24);
	writelr_delay(0x00235115, 0x3430006c, 0, 24);
	writelr_delay(0x00455215, 0x3430006c, 0, 24);
	writelr_delay(0x00675315, 0x3430006c, 0, 24);
	writelr_delay(0x00895415, 0x3430006c, 0, 24);
	writelr_delay(0x00ab5515, 0x3430006c, 0, 24);
	writelr_delay(0x00015615, 0x3430006c, 0, 24);
	writelr_delay(0x00235715, 0x3430006c, 0, 24);
	writelr_delay(0x00455815, 0x3430006c, 0, 24);
	writelr_delay(0x00675915, 0x3430006c, 0, 24);
	writelr_delay(0x00895a15, 0x3430006c, 0, 24);
	writelr_delay(0x00ab5b15, 0x3430006c, 0, 24);
	writelr_delay(0x00cd5c15, 0x3430006c, 0, 24);
	writelr_delay(0x00ef5d15, 0x3430006c, 0, 24);
	writelr_delay(0x00005e15, 0x3430006c, 0, 24);
	writelr_delay(0x00015f15, 0x3430006c, 0, 24);
	writelr_delay(0x00016015, 0x3430006c, 0, 24);
	writelr_delay(0x00066115, 0x3430006c, 0, 24);
	writelr_delay(0x00066215, 0x3430006c, 0, 24);
	writelr_delay(0x00066315, 0x3430006c, 0, 24);
	writelr_delay(0x00066415, 0x3430006c, 0, 24);
	writelr_delay(0x00006515, 0x3430006c, 0, 24);
	writelr_delay(0x00006615, 0x3430006c, 0, 24);
	writelr_delay(0x00176715, 0x3430006c, 0, 24);
	writelr_delay(0x00026815, 0x3430006c, 0, 24);
	writelr_delay(0x00166915, 0x3430006c, 0, 24);
	writelr_delay(0x00166a15, 0x3430006c, 0, 24);
	writelr_delay(0x00026b15, 0x3430006c, 0, 24);
	writelr_delay(0x000d6c15, 0x3430006c, 0, 24);
	writelr_delay(0x000d6d15, 0x3430006c, 0, 24);
	writelr_delay(0x000c6e15, 0x3430006c, 0, 24);
	writelr_delay(0x000c6f15, 0x3430006c, 0, 24);
	writelr_delay(0x000f7015, 0x3430006c, 0, 24);
	writelr_delay(0x000f7115, 0x3430006c, 0, 24);
	writelr_delay(0x000e7215, 0x3430006c, 0, 24);
	writelr_delay(0x000e7315, 0x3430006c, 0, 24);
	writelr_delay(0x00027415, 0x3430006c, 0, 24);
	writelr_delay(0x00017515, 0x3430006c, 0, 24);
	writelr_delay(0x00017615, 0x3430006c, 0, 24);
	writelr_delay(0x00067715, 0x3430006c, 0, 24);
	writelr_delay(0x00067815, 0x3430006c, 0, 24);
	writelr_delay(0x00067915, 0x3430006c, 0, 24);
	writelr_delay(0x00067a15, 0x3430006c, 0, 24);
	writelr_delay(0x00007b15, 0x3430006c, 0, 24);
	writelr_delay(0x00007c15, 0x3430006c, 0, 24);
	writelr_delay(0x00177d15, 0x3430006c, 0, 24);
	writelr_delay(0x00027e15, 0x3430006c, 0, 24);
	writelr_delay(0x00167f15, 0x3430006c, 0, 24);
	writelr_delay(0x00168015, 0x3430006c, 0, 24);
	writelr_delay(0x00028115, 0x3430006c, 0, 24);
	writelr_delay(0x000d8215, 0x3430006c, 0, 24);
	writelr_delay(0x000d8315, 0x3430006c, 0, 24);
	writelr_delay(0x000c8415, 0x3430006c, 0, 24);
	writelr_delay(0x000c8515, 0x3430006c, 0, 24);
	writelr_delay(0x000f8615, 0x3430006c, 0, 24);
	writelr_delay(0x000f8715, 0x3430006c, 0, 24);
	writelr_delay(0x000e8815, 0x3430006c, 0, 24);
	writelr_delay(0x000e8915, 0x3430006c, 0, 24);
	writelr_delay(0x00028a15, 0x3430006c, 0, 24);

	writel(0x048198ff, 0x34300070);  //long
	mdelay(1);
	writelr_delay(0x00000429, 0x3430006c, 0, 24);

	writelr_delay(0x002b6e15, 0x3430006c, 0, 24);
	writelr_delay(0x00356f15, 0x3430006c, 0, 24);
	writelr_delay(0x00a43a15, 0x3430006c, 0, 24);
	writelr_delay(0x00173515, 0x3430006c, 0, 24);
	writelr_delay(0x001a8d15, 0x3430006c, 0, 24);
	writelr_delay(0x00ba8715, 0x3430006c, 0, 24);
	writelr_delay(0x00d1b215, 0x3430006c, 0, 24);
	writelr_delay(0x000b8815, 0x3430006c, 0, 24);
	writelr_delay(0x00013815, 0x3430006c, 0, 24);
	writelr_delay(0x00003915, 0x3430006c, 0, 24);
	writelr_delay(0x0007b515, 0x3430006c, 0, 24);
	writelr_delay(0x00753115, 0x3430006c, 0, 24);
	//writelr_delay(0x00012f15, 0x3430006c, 0, 24);//bist mode

	writel(0x018198ff, 0x34300070);  //long
	mdelay(1);
	writelr_delay(0x00000429, 0x3430006c, 0, 24);

	writelr_delay(0x000a2215, 0x3430006c, 0, 24);
	writelr_delay(0x00003115, 0x3430006c, 0, 24);
	writelr_delay(0x00405315, 0x3430006c, 0, 24);
	writelr_delay(0x00405515, 0x3430006c, 0, 24);
	writelr_delay(0x00955015, 0x3430006c, 0, 24);
	writelr_delay(0x00905115, 0x3430006c, 0, 24);
	writelr_delay(0x00226015, 0x3430006c, 0, 24);
	writelr_delay(0x00206215, 0x3430006c, 0, 24);

	writelr_delay(0x0000a015, 0x3430006c, 0, 24);
	writelr_delay(0x0022a115, 0x3430006c, 0, 24);
	writelr_delay(0x0032a215, 0x3430006c, 0, 24);
	writelr_delay(0x0015a315, 0x3430006c, 0, 24);
	writelr_delay(0x001aa415, 0x3430006c, 0, 24);
	writelr_delay(0x002ca515, 0x3430006c, 0, 24);
	writelr_delay(0x0020a615, 0x3430006c, 0, 24);
	writelr_delay(0x0021a715, 0x3430006c, 0, 24);
	writelr_delay(0x009aa815, 0x3430006c, 0, 24);
	writelr_delay(0x001ea915, 0x3430006c, 0, 24);
	writelr_delay(0x002aaa15, 0x3430006c, 0, 24);
	writelr_delay(0x0080ab15, 0x3430006c, 0, 24);
	writelr_delay(0x001aac15, 0x3430006c, 0, 24);
	writelr_delay(0x0018ad15, 0x3430006c, 0, 24);
	writelr_delay(0x004cae15, 0x3430006c, 0, 24);
	writelr_delay(0x0021af15, 0x3430006c, 0, 24);
	writelr_delay(0x0029b015, 0x3430006c, 0, 24);
	writelr_delay(0x004db115, 0x3430006c, 0, 24);
	writelr_delay(0x005ab215, 0x3430006c, 0, 24);
	writelr_delay(0x0023b315, 0x3430006c, 0, 24);

	writelr_delay(0x0000c015, 0x3430006c, 0, 24);
	writelr_delay(0x0022c115, 0x3430006c, 0, 24);
	writelr_delay(0x0032c215, 0x3430006c, 0, 24);
	writelr_delay(0x0016c315, 0x3430006c, 0, 24);
	writelr_delay(0x0019c415, 0x3430006c, 0, 24);
	writelr_delay(0x002cc515, 0x3430006c, 0, 24);
	writelr_delay(0x0020c615, 0x3430006c, 0, 24);
	writelr_delay(0x0021c715, 0x3430006c, 0, 24);
	writelr_delay(0x009ac815, 0x3430006c, 0, 24);
	writelr_delay(0x001dc915, 0x3430006c, 0, 24);
	writelr_delay(0x002aca15, 0x3430006c, 0, 24);
	writelr_delay(0x0081cb15, 0x3430006c, 0, 24);
	writelr_delay(0x001acc15, 0x3430006c, 0, 24);
	writelr_delay(0x0018cd15, 0x3430006c, 0, 24);
	writelr_delay(0x004cce15, 0x3430006c, 0, 24);
	writelr_delay(0x0022cf15, 0x3430006c, 0, 24);
	writelr_delay(0x0028d015, 0x3430006c, 0, 24);
	writelr_delay(0x004dd115, 0x3430006c, 0, 24);
	writelr_delay(0x005ad215, 0x3430006c, 0, 24);
	writelr_delay(0x0023d315, 0x3430006c, 0, 24);

	writel(0x008198ff, 0x34300070);  //long
	mdelay(1);
	writelr_delay(0x00000429, 0x3430006c, 0, 24);
	writelr_delay(0x00003515, 0x3430006c, 0, 24);
	mdelay(20);
	writelr(0x00001115, 0x3430006c, 0, 24);
	mdelay(600);
	writelr(0x00002915, 0x3430006c, 0, 24);
	mdelay(40);

/*------1280*800 register config-------------------end*/
	writel(0x00000000, 0x34300068);
	writel(0x00000014, 0x3430002c);
	writel(0x00000000, 0x34300034);
/*-------vou cfg dsi high speed end------------------*/
	writel(0x00000000, 0x34101600);
	writel(0x0001fe00, 0x34101604);
	writel(0x00000001, 0x34101608);
	writel(0x00000000, 0x3410160c);
	writel(0x000007ff, 0x34101610);
	writel(0x0001fff4, 0x34101614);
	writel(0x0003f80e, 0x34101618);
	writel(0x00000001, 0x3410161c);
	writel(0x00000fff, 0x34101620);
	writel(0x0001f3e9, 0x34101624);
	writel(0x0003ee1f, 0x34101628);
	writel(0x00000002, 0x3410162c);
	writel(0x000013fe, 0x34101630);
	writel(0x0001e3e1, 0x34101634);
	writel(0x0003e233, 0x34101638);
	writel(0x0003fe04, 0x3410163c);
	writel(0x000017fe, 0x34101640);
	writel(0x0001c9dc, 0x34101644);
	writel(0x0003d847, 0x34101648);
	writel(0x0003fe05, 0x3410164c);
	writel(0x000019fe, 0x34101650);
	writel(0x0001add8, 0x34101654);
	writel(0x0003cc5c, 0x34101658);
	writel(0x0003fe07, 0x3410165c);
	writel(0x000019fd, 0x34101660);
	writel(0x00018dd7, 0x34101664);
	writel(0x0003c272, 0x34101668);
	writel(0x0003fc09, 0x3410166c);
	writel(0x000019fe, 0x34101670);
	writel(0x000165d7, 0x34101674);
	writel(0x0003ba88, 0x34101678);
	writel(0x0003fc0a, 0x3410167c);
	writel(0x000017fe, 0x34101680);
	writel(0x00013dd9, 0x34101684);
	writel(0x0003b29e, 0x34101688);
	writel(0x0003fc0b, 0x3410168c);
	writel(0x000015fe, 0x34101690);
	writel(0x000111dd, 0x34101694);
	writel(0x0003aeb2, 0x34101698);
	writel(0x0003fc0c, 0x3410169c);
	writel(0x000013fe, 0x341016a0);
	writel(0x0000e7e1, 0x341016a4);
	writel(0x0003aec5, 0x341016a8);
	writel(0x0003fa0c, 0x341016ac);
	writel(0x00000fff, 0x341016b0);
	writel(0x0000b9e6, 0x341016b4);
	writel(0x0003b0d6, 0x341016b8);
	writel(0x0003fc0c, 0x341016bc);
	writel(0x00000bff, 0x341016c0);
	writel(0x00008dec, 0x341016c4);
	writel(0x0003b8e5, 0x341016c8);
	writel(0x0003fc0b, 0x341016cc);
	writel(0x000009ff, 0x341016d0);
	writel(0x000067f1, 0x341016d4);
	writel(0x0003c2f1, 0x341016d8);
	writel(0x0003fc09, 0x341016dc);
	writel(0x00000400, 0x341016e0);
	writel(0x00003ff7, 0x341016e4);
	writel(0x0003d2f9, 0x341016e8);
	writel(0x0003fe07, 0x341016ec);
	writel(0x00000200, 0x341016f0);
	writel(0x00001ffc, 0x341016f4);
	writel(0x0003e8fe, 0x341016f8);
	writel(0x0003fe03, 0x341016fc);
	writel(0x0001fe00, 0x34101700);
	writel(0x00000001, 0x34101704);
	writel(0x0001fdf9, 0x34101708);
	writel(0x00000009, 0x3410170c);
	writel(0x0001eff4, 0x34101710);
	writel(0x0003fe16, 0x34101714);
	writel(0x0001d9f1, 0x34101718);
	writel(0x0003fc25, 0x3410171c);
	writel(0x0001bbf0, 0x34101720);
	writel(0x0003fa36, 0x34101724);
	writel(0x000197f0, 0x34101728);
	writel(0x0003f64a, 0x3410172c);
	writel(0x00016ff1, 0x34101730);
	writel(0x0003f25f, 0x34101734);
	writel(0x000143f3, 0x34101738);
	writel(0x0003ee75, 0x3410173c);
	writel(0x000117f5, 0x34101740);
	writel(0x0003ea8b, 0x34101744);
	writel(0x0000ebf7, 0x34101748);
	writel(0x0003e6a1, 0x3410174c);
	writel(0x0000bff9, 0x34101750);
	writel(0x0003e2b7, 0x34101754);
	writel(0x000095fb, 0x34101758);
	writel(0x0003e0cb, 0x3410175c);
	writel(0x00006dfd, 0x34101760);
	writel(0x0003e0dd, 0x34101764);
	writel(0x00004bfe, 0x34101768);
	writel(0x0003e2ec, 0x3410176c);
	writel(0x00002dff, 0x34101770);
	writel(0x0003e8f7, 0x34101774);
	writel(0x00001200, 0x34101778);
	writel(0x0003f2fe, 0x3410177c);
	writel(0x0001fe00, 0x34101800);
	writel(0x00000001, 0x34101804);
	writel(0x0001fdf9, 0x34101808);
	writel(0x00000009, 0x3410180c);
	writel(0x0001eff4, 0x34101810);
	writel(0x0003fe16, 0x34101814);
	writel(0x0001d9f1, 0x34101818);
	writel(0x0003fc25, 0x3410181c);
	writel(0x0001bbf0, 0x34101820);
	writel(0x0003fa36, 0x34101824);
	writel(0x000197f0, 0x34101828);
	writel(0x0003f64a, 0x3410182c);
	writel(0x00016ff1, 0x34101830);
	writel(0x0003f25f, 0x34101834);
	writel(0x000143f3, 0x34101838);
	writel(0x0003ee75, 0x3410183c);
	writel(0x000117f5, 0x34101840);
	writel(0x0003ea8b, 0x34101844);
	writel(0x0000ebf7, 0x34101848);
	writel(0x0003e6a1, 0x3410184c);
	writel(0x0000bff9, 0x34101850);
	writel(0x0003e2b7, 0x34101854);
	writel(0x000095fb, 0x34101858);
	writel(0x0003e0cb, 0x3410185c);
	writel(0x00006dfd, 0x34101860);
	writel(0x0003e0dd, 0x34101864);
	writel(0x00004bfe, 0x34101868);
	writel(0x0003e2ec, 0x3410186c);
	writel(0x00002dff, 0x34101870);
	writel(0x0003e8f7, 0x34101874);
	writel(0x00001200, 0x34101878);
	writel(0x0003f2fe, 0x3410187c);
	writel(0x00003404, 0x34101a00);
	writel(0x00009a3b, 0x34101a04);
	writel(0x0000343c, 0x34101a08);
	writel(0x00000004, 0x34101a0c);
	writel(0x00003004, 0x34101a10);
	writel(0x00009a39, 0x34101a14);
	writel(0x0000383d, 0x34101a18);
	writel(0x00000005, 0x34101a1c);
	writel(0x00002c03, 0x34101a20);
	writel(0x00009838, 0x34101a24);
	writel(0x00003c3f, 0x34101a28);
	writel(0x00000006, 0x34101a2c);
	writel(0x00002803, 0x34101a30);
	writel(0x00009a35, 0x34101a34);
	writel(0x00003e41, 0x34101a38);
	writel(0x00000007, 0x34101a3c);
	writel(0x00002402, 0x34101a40);
	writel(0x00009634, 0x34101a44);
	writel(0x00004642, 0x34101a48);
	writel(0x00000008, 0x34101a4c);
	writel(0x00002202, 0x34101a50);
	writel(0x00009431, 0x34101a54);
	writel(0x00004a44, 0x34101a58);
	writel(0x00000009, 0x34101a5c);
	writel(0x00001e01, 0x34101a60);
	writel(0x0000962f, 0x34101a64);
	writel(0x00004e45, 0x34101a68);
	writel(0x0000000a, 0x34101a6c);
	writel(0x00001c01, 0x34101a70);
	writel(0x0000922d, 0x34101a74);
	writel(0x00005247, 0x34101a78);
	writel(0x0000000b, 0x34101a7c);
	writel(0x00001801, 0x34101a80);
	writel(0x0000902b, 0x34101a84);
	writel(0x00005648, 0x34101a88);
	writel(0x0000020c, 0x34101a8c);
	writel(0x00001600, 0x34101a90);
	writel(0x00008e29, 0x34101a94);
	writel(0x00005a49, 0x34101a98);
	writel(0x0000020e, 0x34101a9c);
	writel(0x00001400, 0x34101aa0);
	writel(0x00008c27, 0x34101aa4);
	writel(0x00005e4a, 0x34101aa8);
	writel(0x0000020f, 0x34101aac);
	writel(0x00001200, 0x34101ab0);
	writel(0x00008825, 0x34101ab4);
	writel(0x0000624a, 0x34101ab8);
	writel(0x00000411, 0x34101abc);
	writel(0x00001000, 0x34101ac0);
	writel(0x00008423, 0x34101ac4);
	writel(0x0000684b, 0x34101ac8);
	writel(0x00000412, 0x34101acc);
	writel(0x00000e00, 0x34101ad0);
	writel(0x0000841f, 0x34101ad4);
	writel(0x00006a4c, 0x34101ad8);
	writel(0x00000614, 0x34101adc);
	writel(0x00000c00, 0x34101ae0);
	writel(0x00007e1e, 0x34101ae4);
	writel(0x0000704c, 0x34101ae8);
	writel(0x00000616, 0x34101aec);
	writel(0x00000a00, 0x34101af0);
	writel(0x00007c1c, 0x34101af4);
	writel(0x0000724c, 0x34101af8);
	writel(0x00000818, 0x34101afc);
	writel(0x0000fe3d, 0x34101b00);
	writel(0x00000c3e, 0x34101b04);
	writel(0x0000fe37, 0x34101b08);
	writel(0x00000c44, 0x34101b0c);
	writel(0x0000fc31, 0x34101b10);
	writel(0x00000e4a, 0x34101b14);
	writel(0x0000f82c, 0x34101b18);
	writel(0x00001050, 0x34101b1c);
	writel(0x0000f426, 0x34101b20);
	writel(0x00001456, 0x34101b24);
	writel(0x0000ee22, 0x34101b28);
	writel(0x0000185b, 0x34101b2c);
	writel(0x0000e81d, 0x34101b30);
	writel(0x00001c61, 0x34101b34);
	writel(0x0000e019, 0x34101b38);
	writel(0x00002266, 0x34101b3c);
	writel(0x0000d615, 0x34101b40);
	writel(0x00002a6b, 0x34101b44);
	writel(0x0000cc11, 0x34101b48);
	writel(0x00003270, 0x34101b4c);
	writel(0x0000c20e, 0x34101b50);
	writel(0x00003a74, 0x34101b54);
	writel(0x0000b80c, 0x34101b58);
	writel(0x00004476, 0x34101b5c);
	writel(0x0000ac0a, 0x34101b60);
	writel(0x00004c7a, 0x34101b64);
	writel(0x0000a008, 0x34101b68);
	writel(0x0000587c, 0x34101b6c);
	writel(0x00009407, 0x34101b70);
	writel(0x0000627e, 0x34101b74);
	writel(0x00008806, 0x34101b78);
	writel(0x00006e7f, 0x34101b7c);
	writel(0x0000fe3d, 0x34101c00);
	writel(0x00000c3e, 0x34101c04);
	writel(0x0000fe37, 0x34101c08);
	writel(0x00000c44, 0x34101c0c);
	writel(0x0000fc31, 0x34101c10);
	writel(0x00000e4a, 0x34101c14);
	writel(0x0000f82c, 0x34101c18);
	writel(0x00001050, 0x34101c1c);
	writel(0x0000f426, 0x34101c20);
	writel(0x00001456, 0x34101c24);
	writel(0x0000ee22, 0x34101c28);
	writel(0x0000185b, 0x34101c2c);
	writel(0x0000e81d, 0x34101c30);
	writel(0x00001c61, 0x34101c34);
	writel(0x0000e019, 0x34101c38);
	writel(0x00002266, 0x34101c3c);
	writel(0x0000d615, 0x34101c40);
	writel(0x00002a6b, 0x34101c44);
	writel(0x0000cc11, 0x34101c48);
	writel(0x00003270, 0x34101c4c);
	writel(0x0000c20e, 0x34101c50);
	writel(0x00003a74, 0x34101c54);
	writel(0x0000b80c, 0x34101c58);
	writel(0x00004476, 0x34101c5c);
	writel(0x0000ac0a, 0x34101c60);
	writel(0x00004c7a, 0x34101c64);
	writel(0x0000a008, 0x34101c68);
	writel(0x0000587c, 0x34101c6c);
	writel(0x00009407, 0x34101c70);
	writel(0x0000627e, 0x34101c74);
	writel(0x00008806, 0x34101c78);
	writel(0x00006e7f, 0x34101c7c);
	writel((vou_out.Vactive << 16) + vou_out.Hactvie, 0x34101024);//writel(0x05000320, 0x34101024);
	writel((vou_out.StartBlen << 16) + vou_out.Hlen, 0x34101028);//writel(0x0019033d, 0x34101028);
	writel((vou_out.TBLineOfs << 16) + vou_out.TVLine, 0x3410102C);//writel(0x001b052f, 0x3410102C);
	writel((vou_out.BBLineOfs << 16) + vou_out.BVLine, 0x34101030);//writel(0x00000000, 0x34101030);
	writel(0xff00ff00, 0x34101034);

	writel(0x349ea100, 0x34101060);
	writel(0x00071900, 0x34101064);
	writel(0x16700100, 0x34101068);
	writel(0x34dc7487, 0x3410106c);
	writel(0x00000001, 0x34101000);
	//writel(0x00000003, 0x34101000);  //vou test mode

	writel(0x00000000, 0x34101014);

	if(type == LOGO_YUV) {
		//printf("LOGO_YUV !!!\n");
		writel(0x00000040, 0x34101200);
		writel((ph<<16) + pw, 0x34101208);
		writel(pp, 0x3410120c);
		writel(addr, 0x34101210);
		writel(addr + pp*ph, 0x34101214);
		writel((y<<16) + x, 0x34101218);
		writel(0x00000000, 0x34101230);
		writel(0x00000000, 0x34101234);
		writel((h<<16) + w, 0x3410121c);
		writel(0x80800001, 0x34101004);
		writel(0x00000009, 0x34101000);
	} else {
		// w & h are not used
		if (type == LOGO_RGB_8888) {
			//printf("LOGO_RGB_8888 !!!\n");
			//writel(0x00000002, 0x34101120); // mc abgr8888
			//writel(0x0000001A, 0x34101120); //hik argb8888
			writel(0x0000001A, 0x34101120);
		} else if (type == LOGO_RGB_1555) {
			//printf("LOGO_RGB_1555 !!!\n");
			writel(0x00000010, 0x34101120);  //mc argb1555
		}
		writel(0x00008000, 0x34101124);
		writel(0x00000000, 0x34101128);
		writel((ph<<16) + pw, 0x3410112c);
		writel(pp, 0x34101130);
		writel(addr, 0x34101134);
		writel((y<<16) + x, 0x34101138);
		writel(0x00000011, 0x34101000);
	}

	//set_work_ctrl()
	writel(0x00000b00, 0x3410100c);
	//set_work_ctrl()
	writel(0x00000801, 0x3410100c);
#endif
}

void fast_osd_1024x600(int type, int x, int y, int w, int h, uint32_t addr, int pw, int pp, int ph)
{
#if (LOGO_VO_MIPI == LOGO_VO_EK79007)
	uint32_t clkdiv = 0;
	//
	writelr(0x00000001, 0x1B000008, 3, 1); // DIR:  set GPIO0-3 output
	writelr(0x00000001, 0x1B000004, 3, 1); // DMSK: enable GPIO0-3
	writelr(0x00000001, 0x1B000000, 3, 1); // DATA: GPIO0-3 output 1;

	writel(0x00000002, 0x2820009c);
	writel(0x00000002, 0x282000a0);
	writel(0x00000002, 0x282000a4);
	writel(0x00000002, 0x282000a8);
	writel(0x00000002, 0x282000ac);
	writel(0x00000002, 0x282000b0);
	writel(0x00000002, 0x282000b4);
	writel(0x00000002, 0x282000b8);
	writel(0x00000002, 0x282000bc);
	writel(0x00000002, 0x282000c0);
	writels(0x00003f1f, 0x34000000);
	writelc(0x00000400, 0x34000078);
	writels(0x00000003, 0x28100030);
	writels(0x0000007f, 0x34000014);
	writelc(0x0000007f, 0x34000014);
	writel(0x0000000f, 0x34100004);
	writels(0x00000300, 0x34000078);
	writelr(0x00000000, 0x343000a0, 2, 1);
	writelr(0x00000001, 0x343000b4, 0, 1);
	writelr(0x00000001, 0x343000a0, 3, 1);
	writelc(0x00002000, 0x34500064);
	writelr(0x00000001, 0x343000b4, 1, 1);
	writelr(0x00000000, 0x343000b8, 16, 1);
	writelr(0x00000001, 0x343000a0, 0, 1);
	writelr(0x00000001, 0x343000a0, 1, 1);
	writelr(0x00000000, 0x343000b4, 0, 1);
	writels(0x00002000, 0x34500064);
	writelr(0x00000000, 0x34500064, 1, 7);
	writelr(0x00000007, 0x34500064, 9, 4);
	writelr(0x00000001, 0x34500064, 13, 1);
	writelr(0x00080000, 0x34500060, 0, 32);
	writelr(0x00000002, 0x34500068, 0, 4);
	writelr(0x00000002, 0x34500068, 4, 4);
	writelr(0x00000007, 0x34500068, 8, 3);
	writelr(0x00000007, 0x34500068, 11, 3);
	writelr(0x00000001, 0x3450007c, 0, 8);
	writelr(0x00000007, 0x3450007c, 8, 8);
	writelr(0x0000001f, 0x34500074, 0, 5);
	writelr(0x00000002, 0x34500074, 5, 2);
	writelr(0x00000000, 0x34500074, 7, 3);
	writelr(0x0a02687a, 0x3450005c, 0, 32);
	writelr(0x000000b6, 0x34500058, 0, 32);
	writelr(0x000001f4, 0x34500000, 0, 15);
	writelr(0x00002710, 0x34500004, 0, 31);
	writelr(0x00041902, 0x34500008, 0, 32);
	writelr(0x00120609, 0x3450000c, 0, 32);
	writelr(0x00000100, 0x34500078, 0, 32);
	writelr(0x000001f4, 0x34500010, 0, 32);
	writelr(0x00002710, 0x34500014, 0, 32);
	writelr(0x041e0809, 0x34500018, 0, 32);
	writelr(0x00000100, 0x3450001c, 0, 32);
	writelr(0x0030183c, 0x34500020, 0, 32);
	writelr(0x000001f4, 0x34500024, 0, 32);
	writelr(0x00002710, 0x34500028, 0, 32);
	writelr(0x041e0809, 0x3450002c, 0, 32);
	writelr(0x00000100, 0x34500030, 0, 32);
	writelr(0x000001f4, 0x34500034, 0, 32);
	writelr(0x00002710, 0x34500038, 0, 32);
	writelr(0x041e0809, 0x3450003c, 0, 32);
	writelr(0x00000100, 0x34500040, 0, 32);
	writelr(0x000001f4, 0x34500044, 0, 32);
	writelr(0x00002710, 0x34500048, 0, 32);
	writelr(0x041e0809, 0x3450004c, 0, 32);
	writelr(0x00000100, 0x34500050, 0, 32);
	writelr(0x0000000c, 0x34500054, 0, 8);
	writelr(0x0000001f, 0x34500074, 0, 5);
	writelr(0x00000000, 0x34500074, 5, 2);
	writelr(0x00000000, 0x34500074, 7, 3);
	writelr(0x00000001, 0x3450007c, 0, 8);
	writelr(0x0000000a, 0x3450007c, 8, 8);
	writelr(0x00000000, 0x3450006c, 0, 3);
	writelr(0x00000001, 0x3450006c, 3, 1);
	writelr(0x00000000, 0x3450006c, 4, 2);
	writelr(0x00000000, 0x3450006c, 6, 1);
	udelay(1000);
	writelr(0x00000000, 0x34500064, 1, 7);
	writelr(0x00000007, 0x34500064, 9, 4);
	writelr(0x00000001, 0x34500064, 13, 1);
	writelr(0x00000000, 0x3430000c, 0, 2);
	writelr(0x00000005, 0x34300010, 0, 4);
	writelr(0x00000001, 0x34300014, 2, 1);
	writelr(0x00000001, 0x34300014, 1, 1);
	writelr(0x0000000c, 0x34300008, 0, 8);
	writelr(0x00000001, 0x34300008, 8, 8);
	writel(0xffffff01, 0x34300068);
	writelr(0x00000001, 0x3430001c, 0, 2);
	writelr(0x00000004, 0x34300020, 0, 4);
	writelr(0x00000001, 0x34300020, 16, 2);
	writelr(0x00000001, 0x34300024, 0, 1);
	writelr(0x00000064, 0x34300028, 16, 16);
	writelr(0x00000008, 0x34300028, 0, 16);
	writelr(0x00000001, 0x3430002c, 2, 1);
	writelr(0x00000001, 0x3430002c, 3, 1);
	writelr(0x00000001, 0x3430002c, 4, 1);
	writelr(0x00000000, 0x34300030, 0, 2);
	writelr(0x00000001, 0x34300034, 0, 1);
	writelr(0x00000002, 0x34300038, 0, 2);
	writelr(0x00000000, 0x34300038, 8, 1);
	writelr(0x00000001, 0x34300038, 9, 1);
	writelr(0x00000000, 0x34300038, 10, 1);
	writelr(0x00000000, 0x34300038, 11, 1);
	writelr(0x00000000, 0x34300038, 12, 1);
	writelr(0x00000000, 0x34300038, 13, 1);
	writelr(vou_out.Hactvie , 0x3430003c, 0, 14);//writelr(0x00000400, 0x3430003c, 0, 14);
	writelr(0x00000001, 0x34300040, 0, 13);
	writelr(0x0000000a, 0x34300044, 0, 13);

	clkdiv = mipi_dphy_hal_get_pixel_clk_div();

	writelr((vou_out.Hsync * clkdiv / 8), 0x34300048, 0, 12);//writelr(0x0000002d, 0x34300048, 0, 12);
	writelr((vou_out.Hback * clkdiv / 8), 0x3430004c, 0, 12);//writelr(0x0000009d, 0x3430004c, 0, 12);
	writelr(((vou_out.Hsync + vou_out.Hback + vou_out.Hfront + vou_out.Hactvie) * clkdiv / 8), 0x34300050, 0, 15);//writelr(0x000005e8, 0x34300050, 0, 15);
	writelr(vou_out.Vsync, 0x34300054, 0, 10);//writelr(0x00000002, 0x34300054, 0, 10);
	writelr(vou_out.Vback, 0x34300058, 0, 10);//writelr(0x00000017, 0x34300058, 0, 10);
	writelr(vou_out.Vfront, 0x3430005c, 0, 10);//writelr(0x0000000c, 0x3430005c, 0, 10);
	writelr(vou_out.Vactive, 0x34300060, 0, 14);//writelr(0x00000258, 0x34300060, 0, 14);
	writelr(0x00000000, 0x34300018, 16, 8);
	writelr(0x00000000, 0x34300018, 0, 8);
	writelr(0x0000006e, 0x3430009c, 0, 15);
	writelr(0x00000025, 0x3430009c, 16, 8);
	writelr(0x00000000, 0x34300064, 0, 16);
	writelr(0x00000001, 0x34300094, 0, 1);
	writelr(0x00000000, 0x34300078, 0, 16);
	writelr(0x00000000, 0x34300078, 16, 16);
	writelr(0x00000000, 0x3430007c, 0, 16);
	writelr(0x00000000, 0x34300080, 0, 16);
	writelr(0x00000000, 0x34300084, 0, 16);
	writelr(0x00000000, 0x34300088, 0, 16);
	writelr(0x00000000, 0x3430008c, 0, 16);
	writelr(0x00000087, 0x34300098, 0, 10);
	writelr(0x00000025, 0x34300098, 16, 10);
	writelr(0x00000000, 0x343000f4, 0, 15);
	writelr(0x00000028, 0x343000a4, 8, 8);
	writelr(0x00000003, 0x343000a4, 0, 2);
	writelr(0x00000000, 0x343000a8, 0, 4);
	writelr(0x00000000, 0x343000ac, 0, 4);
	writelr(0x00000000, 0x34300100, 0, 1);
	writelr(0x00000000, 0x34300100, 8, 1);
	writelr(0x00000000, 0x34300100, 16, 1);
	writel(0xffffffff, 0x343000c4);
	writel(0xffffffff, 0x343000c8);
	writelr(0x00000000, 0x34300004, 0, 1);
	writelr(0x00000001, 0x34300004, 0, 1);
	writels(0x00004000, 0x3400007c);
	//----------------select size 1024/600/1/60------------------------
	writel(0x00000008, 0x34100020);
	writel(0x0000000a, 0x34100024);
	writel(0x00000300, 0x34100028);
	writel(0x00000600, 0x3410002c);
	writel(0x00000007, 0x34100030);
	writel(0x0000000a, 0x34100034);
	writel(0x00000080, 0x34100038);
	writel(0x00000100, 0x3410003c);
	writel(0x00000001, 0x34100040);
	writel(0x00000028, 0x34101f14);
	writel(0x00000002, 0x34101f18);
	writel(0x00000000, 0x34101f1c);
	writelr(0x00478015, 0x3430006c, 0, 24);
	writelr(0x00408115, 0x3430006c, 0, 24);
	writelr(0x00048215, 0x3430006c, 0, 24);
	writelr(0x00778315, 0x3430006c, 0, 24);
	writelr(0x000f8415, 0x3430006c, 0, 24);
	writelr(0x00708515, 0x3430006c, 0, 24);
	writelr(0x00708615, 0x3430006c, 0, 24);
	writelr(0x00001115, 0x3430006c, 0, 24);
	writelr(0x00002915, 0x3430006c, 0, 24);
	writel(0x00000000, 0x34300068);
	writel(0x00000014, 0x3430002c);
	writel(0x00000000, 0x34300034);
	writel(0x00000000, 0x34101600);
	writel(0x0001fe00, 0x34101604);
	writel(0x00000001, 0x34101608);
	writel(0x00000000, 0x3410160c);
	writel(0x000007ff, 0x34101610);
	writel(0x0001fff4, 0x34101614);
	writel(0x0003f80e, 0x34101618);
	writel(0x00000001, 0x3410161c);
	writel(0x00000fff, 0x34101620);
	writel(0x0001f3e9, 0x34101624);
	writel(0x0003ee1f, 0x34101628);
	writel(0x00000002, 0x3410162c);
	writel(0x000013fe, 0x34101630);
	writel(0x0001e3e1, 0x34101634);
	writel(0x0003e233, 0x34101638);
	writel(0x0003fe04, 0x3410163c);
	writel(0x000017fe, 0x34101640);
	writel(0x0001c9dc, 0x34101644);
	writel(0x0003d847, 0x34101648);
	writel(0x0003fe05, 0x3410164c);
	writel(0x000019fe, 0x34101650);
	writel(0x0001add8, 0x34101654);
	writel(0x0003cc5c, 0x34101658);
	writel(0x0003fe07, 0x3410165c);
	writel(0x000019fd, 0x34101660);
	writel(0x00018dd7, 0x34101664);
	writel(0x0003c272, 0x34101668);
	writel(0x0003fc09, 0x3410166c);
	writel(0x000019fe, 0x34101670);
	writel(0x000165d7, 0x34101674);
	writel(0x0003ba88, 0x34101678);
	writel(0x0003fc0a, 0x3410167c);
	writel(0x000017fe, 0x34101680);
	writel(0x00013dd9, 0x34101684);
	writel(0x0003b29e, 0x34101688);
	writel(0x0003fc0b, 0x3410168c);
	writel(0x000015fe, 0x34101690);
	writel(0x000111dd, 0x34101694);
	writel(0x0003aeb2, 0x34101698);
	writel(0x0003fc0c, 0x3410169c);
	writel(0x000013fe, 0x341016a0);
	writel(0x0000e7e1, 0x341016a4);
	writel(0x0003aec5, 0x341016a8);
	writel(0x0003fa0c, 0x341016ac);
	writel(0x00000fff, 0x341016b0);
	writel(0x0000b9e6, 0x341016b4);
	writel(0x0003b0d6, 0x341016b8);
	writel(0x0003fc0c, 0x341016bc);
	writel(0x00000bff, 0x341016c0);
	writel(0x00008dec, 0x341016c4);
	writel(0x0003b8e5, 0x341016c8);
	writel(0x0003fc0b, 0x341016cc);
	writel(0x000009ff, 0x341016d0);
	writel(0x000067f1, 0x341016d4);
	writel(0x0003c2f1, 0x341016d8);
	writel(0x0003fc09, 0x341016dc);
	writel(0x00000400, 0x341016e0);
	writel(0x00003ff7, 0x341016e4);
	writel(0x0003d2f9, 0x341016e8);
	writel(0x0003fe07, 0x341016ec);
	writel(0x00000200, 0x341016f0);
	writel(0x00001ffc, 0x341016f4);
	writel(0x0003e8fe, 0x341016f8);
	writel(0x0003fe03, 0x341016fc);
	writel(0x0001fe00, 0x34101700);
	writel(0x00000001, 0x34101704);
	writel(0x0001fdf9, 0x34101708);
	writel(0x00000009, 0x3410170c);
	writel(0x0001eff4, 0x34101710);
	writel(0x0003fe16, 0x34101714);
	writel(0x0001d9f1, 0x34101718);
	writel(0x0003fc25, 0x3410171c);
	writel(0x0001bbf0, 0x34101720);
	writel(0x0003fa36, 0x34101724);
	writel(0x000197f0, 0x34101728);
	writel(0x0003f64a, 0x3410172c);
	writel(0x00016ff1, 0x34101730);
	writel(0x0003f25f, 0x34101734);
	writel(0x000143f3, 0x34101738);
	writel(0x0003ee75, 0x3410173c);
	writel(0x000117f5, 0x34101740);
	writel(0x0003ea8b, 0x34101744);
	writel(0x0000ebf7, 0x34101748);
	writel(0x0003e6a1, 0x3410174c);
	writel(0x0000bff9, 0x34101750);
	writel(0x0003e2b7, 0x34101754);
	writel(0x000095fb, 0x34101758);
	writel(0x0003e0cb, 0x3410175c);
	writel(0x00006dfd, 0x34101760);
	writel(0x0003e0dd, 0x34101764);
	writel(0x00004bfe, 0x34101768);
	writel(0x0003e2ec, 0x3410176c);
	writel(0x00002dff, 0x34101770);
	writel(0x0003e8f7, 0x34101774);
	writel(0x00001200, 0x34101778);
	writel(0x0003f2fe, 0x3410177c);
	writel(0x0001fe00, 0x34101800);
	writel(0x00000001, 0x34101804);
	writel(0x0001fdf9, 0x34101808);
	writel(0x00000009, 0x3410180c);
	writel(0x0001eff4, 0x34101810);
	writel(0x0003fe16, 0x34101814);
	writel(0x0001d9f1, 0x34101818);
	writel(0x0003fc25, 0x3410181c);
	writel(0x0001bbf0, 0x34101820);
	writel(0x0003fa36, 0x34101824);
	writel(0x000197f0, 0x34101828);
	writel(0x0003f64a, 0x3410182c);
	writel(0x00016ff1, 0x34101830);
	writel(0x0003f25f, 0x34101834);
	writel(0x000143f3, 0x34101838);
	writel(0x0003ee75, 0x3410183c);
	writel(0x000117f5, 0x34101840);
	writel(0x0003ea8b, 0x34101844);
	writel(0x0000ebf7, 0x34101848);
	writel(0x0003e6a1, 0x3410184c);
	writel(0x0000bff9, 0x34101850);
	writel(0x0003e2b7, 0x34101854);
	writel(0x000095fb, 0x34101858);
	writel(0x0003e0cb, 0x3410185c);
	writel(0x00006dfd, 0x34101860);
	writel(0x0003e0dd, 0x34101864);
	writel(0x00004bfe, 0x34101868);
	writel(0x0003e2ec, 0x3410186c);
	writel(0x00002dff, 0x34101870);
	writel(0x0003e8f7, 0x34101874);
	writel(0x00001200, 0x34101878);
	writel(0x0003f2fe, 0x3410187c);
	writel(0x00003404, 0x34101a00);
	writel(0x00009a3b, 0x34101a04);
	writel(0x0000343c, 0x34101a08);
	writel(0x00000004, 0x34101a0c);
	writel(0x00003004, 0x34101a10);
	writel(0x00009a39, 0x34101a14);
	writel(0x0000383d, 0x34101a18);
	writel(0x00000005, 0x34101a1c);
	writel(0x00002c03, 0x34101a20);
	writel(0x00009838, 0x34101a24);
	writel(0x00003c3f, 0x34101a28);
	writel(0x00000006, 0x34101a2c);
	writel(0x00002803, 0x34101a30);
	writel(0x00009a35, 0x34101a34);
	writel(0x00003e41, 0x34101a38);
	writel(0x00000007, 0x34101a3c);
	writel(0x00002402, 0x34101a40);
	writel(0x00009634, 0x34101a44);
	writel(0x00004642, 0x34101a48);
	writel(0x00000008, 0x34101a4c);
	writel(0x00002202, 0x34101a50);
	writel(0x00009431, 0x34101a54);
	writel(0x00004a44, 0x34101a58);
	writel(0x00000009, 0x34101a5c);
	writel(0x00001e01, 0x34101a60);
	writel(0x0000962f, 0x34101a64);
	writel(0x00004e45, 0x34101a68);
	writel(0x0000000a, 0x34101a6c);
	writel(0x00001c01, 0x34101a70);
	writel(0x0000922d, 0x34101a74);
	writel(0x00005247, 0x34101a78);
	writel(0x0000000b, 0x34101a7c);
	writel(0x00001801, 0x34101a80);
	writel(0x0000902b, 0x34101a84);
	writel(0x00005648, 0x34101a88);
	writel(0x0000020c, 0x34101a8c);
	writel(0x00001600, 0x34101a90);
	writel(0x00008e29, 0x34101a94);
	writel(0x00005a49, 0x34101a98);
	writel(0x0000020e, 0x34101a9c);
	writel(0x00001400, 0x34101aa0);
	writel(0x00008c27, 0x34101aa4);
	writel(0x00005e4a, 0x34101aa8);
	writel(0x0000020f, 0x34101aac);
	writel(0x00001200, 0x34101ab0);
	writel(0x00008825, 0x34101ab4);
	writel(0x0000624a, 0x34101ab8);
	writel(0x00000411, 0x34101abc);
	writel(0x00001000, 0x34101ac0);
	writel(0x00008423, 0x34101ac4);
	writel(0x0000684b, 0x34101ac8);
	writel(0x00000412, 0x34101acc);
	writel(0x00000e00, 0x34101ad0);
	writel(0x0000841f, 0x34101ad4);
	writel(0x00006a4c, 0x34101ad8);
	writel(0x00000614, 0x34101adc);
	writel(0x00000c00, 0x34101ae0);
	writel(0x00007e1e, 0x34101ae4);
	writel(0x0000704c, 0x34101ae8);
	writel(0x00000616, 0x34101aec);
	writel(0x00000a00, 0x34101af0);
	writel(0x00007c1c, 0x34101af4);
	writel(0x0000724c, 0x34101af8);
	writel(0x00000818, 0x34101afc);
	writel(0x0000fe3d, 0x34101b00);
	writel(0x00000c3e, 0x34101b04);
	writel(0x0000fe37, 0x34101b08);
	writel(0x00000c44, 0x34101b0c);
	writel(0x0000fc31, 0x34101b10);
	writel(0x00000e4a, 0x34101b14);
	writel(0x0000f82c, 0x34101b18);
	writel(0x00001050, 0x34101b1c);
	writel(0x0000f426, 0x34101b20);
	writel(0x00001456, 0x34101b24);
	writel(0x0000ee22, 0x34101b28);
	writel(0x0000185b, 0x34101b2c);
	writel(0x0000e81d, 0x34101b30);
	writel(0x00001c61, 0x34101b34);
	writel(0x0000e019, 0x34101b38);
	writel(0x00002266, 0x34101b3c);
	writel(0x0000d615, 0x34101b40);
	writel(0x00002a6b, 0x34101b44);
	writel(0x0000cc11, 0x34101b48);
	writel(0x00003270, 0x34101b4c);
	writel(0x0000c20e, 0x34101b50);
	writel(0x00003a74, 0x34101b54);
	writel(0x0000b80c, 0x34101b58);
	writel(0x00004476, 0x34101b5c);
	writel(0x0000ac0a, 0x34101b60);
	writel(0x00004c7a, 0x34101b64);
	writel(0x0000a008, 0x34101b68);
	writel(0x0000587c, 0x34101b6c);
	writel(0x00009407, 0x34101b70);
	writel(0x0000627e, 0x34101b74);
	writel(0x00008806, 0x34101b78);
	writel(0x00006e7f, 0x34101b7c);
	writel(0x0000fe3d, 0x34101c00);
	writel(0x00000c3e, 0x34101c04);
	writel(0x0000fe37, 0x34101c08);
	writel(0x00000c44, 0x34101c0c);
	writel(0x0000fc31, 0x34101c10);
	writel(0x00000e4a, 0x34101c14);
	writel(0x0000f82c, 0x34101c18);
	writel(0x00001050, 0x34101c1c);
	writel(0x0000f426, 0x34101c20);
	writel(0x00001456, 0x34101c24);
	writel(0x0000ee22, 0x34101c28);
	writel(0x0000185b, 0x34101c2c);
	writel(0x0000e81d, 0x34101c30);
	writel(0x00001c61, 0x34101c34);
	writel(0x0000e019, 0x34101c38);
	writel(0x00002266, 0x34101c3c);
	writel(0x0000d615, 0x34101c40);
	writel(0x00002a6b, 0x34101c44);
	writel(0x0000cc11, 0x34101c48);
	writel(0x00003270, 0x34101c4c);
	writel(0x0000c20e, 0x34101c50);
	writel(0x00003a74, 0x34101c54);
	writel(0x0000b80c, 0x34101c58);
	writel(0x00004476, 0x34101c5c);
	writel(0x0000ac0a, 0x34101c60);
	writel(0x00004c7a, 0x34101c64);
	writel(0x0000a008, 0x34101c68);
	writel(0x0000587c, 0x34101c6c);
	writel(0x00009407, 0x34101c70);
	writel(0x0000627e, 0x34101c74);
	writel(0x00008806, 0x34101c78);
	writel(0x00006e7f, 0x34101c7c);
	writel((vou_out.Vactive << 16) + vou_out.Hactvie, 0x34101024);//writel(0x02580400, 0x34101024);
	writel((vou_out.StartBlen << 16) + vou_out.Hlen, 0x34101028);//writel(0x009F053F, 0x34101028);
	writel((vou_out.TBLineOfs << 16) + vou_out.TVLine, 0x3410102C);//writel(0x0016027A, 0x3410102C);
	writel((vou_out.BBLineOfs << 16) + vou_out.BVLine, 0x34101030);//writel(0x00000000, 0x34101030);
	writel(0xff00ff00, 0x34101034);
	writel(0x356ebd01, 0x34101060);
	writel(0x00069901, 0x34101064);
	writel(0x14e00101, 0x34101068);
	writel(0x33fc4861, 0x3410106c);
	writel(0x00000001, 0x34101000);

	writel(0x00000000, 0x34101014);

	if (LOGO_YUV == type) {
		writel(0x00000040, 0x34101200);
		writel((ph<<16) + pw, 0x34101208);
		writel(pp, 0x3410120c);
		writel(addr, 0x34101210);
		writel(addr + pp*ph, 0x34101214);
		writel((y<<16) + x, 0x34101218);
		writel(0x00000000, 0x34101230);
		writel(0x00000000, 0x34101234);
		writel((h<<16) + w, 0x3410121c);
		writel(0x80800001, 0x34101004);
		writel(0x00000009, 0x34101000);
	} else {
		// w & h are not used
		writel(0x0000000a, 0x34101120);
		writel(0x00008000, 0x34101124);
		writel(0x00000000, 0x34101128);
		writel((ph<<16) + pw, 0x3410112c);
		writel(pp, 0x34101130);
		writel(addr, 0x34101134);
		writel((y<<16) + x, 0x34101138);
		writel(0x00000011, 0x34101000);
	}
	//set_work_ctrl()
	writel(0x00000b00, 0x3410100c);
	//set_work_ctrl()
	writel(0x00000801, 0x3410100c);
#endif
}

// width / height / address must be 16 aligned
void fast_jpeg(uint32_t jpg, uint32_t jpg_len, uint32_t y, uint32_t uv)
{
	int cnt = 100;
	int status = 0;

	printf("begin to soft reset jpeg\r\n");
	// jpgd soft reset
	writels(0x00000040, 0x24000030);
	mdelay(100);
	writelc(0x00000040, 0x24000030);

	writels(0x00000006, 0x24000028);
	// jpgd enable clk
	writels(0x00000009, 0x2400002c);

	writel(0x00000001, 0x26100004);
	writel(jpg_len, 0x26100050);
	writel(0x00000001, 0x26100008);
	writel(jpg, 	  0x2610003C); /* JPG  */
	writel(y,		  0x26100034); /* Y    */
	writel(uv , 	  0x26100040); /* UV   */
	writel(0x00000000, 0x26100058);
	writels(0x0000000A, 0x26100014);
	writel(0x80000001, 0x26100010);

	// wait decode done
	while (cnt-- > 0) {
		udelay(1000);
		status = readl(0x26100004);
		if ((status & 0x01) != 0) {
			printf("JPEG: decode done.\n");

			writels(0x00000001, 0x26100004);
			break;
		}
	}

	if (cnt <= 0) {
		printf("JPEG: decode timeout.\n");
	}
}


void logo_display(void)
{
	fast_osd_init();

#if (LOGO_VO_HDMI == LOGO_VO_1024x768)
	fast_osd_hdmi(LOGO_YUV, (1024 - logo_w)/2, (768 - logo_h)/2, logo_w, logo_h, (uint32_t)logo_o_addr, logo_w, logo_w, logo_h);
#elif (LOGO_VO_HDMI == LOGO_VO_1920x1080)
	fast_osd_hdmi(LOGO_YUV, (1920 - logo_w)/2, (1080 - logo_h)/2, logo_w, logo_h, (uint32_t)logo_o_addr, logo_w, logo_w, logo_h);
#elif (LOGO_VO_MIPI == LOGO_VO_ILI9881)
	fast_osd_ili9881(LOGO_YUV, 0, 0, 800, 1280, (uint32_t)logo_o_addr, logo_w, logo_w, logo_h);
#elif (LOGO_VO_MIPI == LOGO_VO_EK79007)
	fast_osd_1024x600(LOGO_YUV, 0, 0, 1024, 600, (uint32_t)logo_o_addr, logo_w, logo_w, logo_h);
#endif
}

static int do_yuv(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	if (argc >= 3) {
		logo_o_addr = (unsigned char *)simple_strtoul(argv[1], NULL, 16);
		logo_w = simple_strtoul(argv[2], NULL, 10);
		logo_h = simple_strtoul(argv[3], NULL, 10);

		if (0 == logo_o_addr \
			|| 0 == logo_w \
			|| 0 == logo_h) {
			printf("YUV: no valid input!\n");
			return 0;
		}

		logo_display();
	}

	return 0;
}

static int do_jpeg(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	uint32_t jpg_len = 0x100000;		/* max jpeg file size 1MB */
	if (argc >= 3) {
		logo_i_addr = (unsigned char *)simple_strtoul(argv[1], NULL, 16);
		logo_o_addr = (unsigned char *)simple_strtoul(argv[2], NULL, 16);
		logo_w = simple_strtoul(argv[3], NULL, 10);
		logo_h = simple_strtoul(argv[4], NULL, 10);

		if (0 == logo_i_addr \
			|| 0 == logo_o_addr \
			|| 0 == logo_w \
			|| 0 == logo_h) {
			printf("JPEG: no valid input!\n");
			return 0;
		}

		fast_jpeg((uint32_t)logo_i_addr, jpg_len, (uint32_t)logo_o_addr, (uint32_t)(logo_o_addr + logo_w*logo_h));
	}

	return 0;
}


U_BOOT_CMD(
	yuv, 4, 4, do_yuv,
	"show yuv logo.\n",
	"\t- yuv addr w h                 show yuv logo\n"
);


U_BOOT_CMD(
	jpeg, 5, 5, do_jpeg,
	"decode jpeg.\n",
	"\t- jpeg jpg_addr yuv_addr w h   jpeg decoder\n"
);

