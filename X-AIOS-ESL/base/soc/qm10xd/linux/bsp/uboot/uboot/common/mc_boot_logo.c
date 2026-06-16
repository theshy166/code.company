#include <common.h>

#define LOGO_USE_YUV                (0)

#define MIPI_LCD

#define LOGO_YUV				(1)
#define LOGO_RGB_1555				(2)
#define LOGO_RGB_8888				(3)

#define LOGO_RGB_FORMAT					(LOGO_RGB_1555)

#define LOGO_W						(800)
#define LOGO_H						(1280)


#define LOGO_YUV_ADDR_MEM			(0x40000000+512*1024*1024-LOGO_W*LOGO_H*4)
#define LOGO_RGB_ADDR_MEM			(0x40000000+512*1024*1024-LOGO_W*LOGO_H*4)

static uint32_t readl(uint32_t addr)
{
	uint32_t value = *(volatile uint32_t*)addr;
	return value;
}

static void writel(uint32_t value, uint32_t addr)
{
	*(volatile uint32_t*)addr = value;
//	printf("[%x]=0x%x\n",addr,value);
}

/**/
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
#ifndef MIPI_LCD
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
#endif
#ifdef MIPI_LCD

typedef struct {
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
} vou_sync_info_t;

/*
mipi screen sync info configration
*/
vou_sync_info_t sync_info_t = {
	.Hactvie = 800,
	.Vactive = 1280,
	.FrameRate = 60,
	.Hfront = 4,
	.Hback = 18,
	.Hsync = 8,
	.Hpol = 1,
	.Vfront = 20,
	.Vback = 24,
	.Vsync = 4,
	.Vpol = 1,
};


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

static int sync_info_gen(void)
{
	int Hactvie = sync_info_t.Hactvie;
	int Vactive = sync_info_t.Vactive;
	int FrameRate = sync_info_t.FrameRate;
	int Hfront = sync_info_t.Hfront;
	int Hsync = sync_info_t.Hsync;
	int Hback = sync_info_t.Hback;
	int Vfront = sync_info_t.Vfront;
	int Vsync = sync_info_t.Vsync;
	int Vback = sync_info_t.Vback;

	sync_info_t.Hblank = Hfront + Hsync + Hback;
	sync_info_t.Vblank = Vfront + Vsync + Vback;
	sync_info_t.Htotal = Hactvie + sync_info_t.Hblank;
	sync_info_t.Vtotal = Vactive + sync_info_t.Vblank;
	sync_info_t.PixelRate = FrameRate * sync_info_t.Htotal * sync_info_t.Vtotal;
	sync_info_t.Alen = Hactvie;
	sync_info_t.Aline = Vactive;
	sync_info_t.Hlen = sync_info_t.Htotal - 1;
	sync_info_t.StartBlen = Hsync + Hback - 1;
	sync_info_t.EndBlen = Hfront;
	sync_info_t.TVLine = sync_info_t.Vtotal - 1;
	sync_info_t.TBLineOfs = Vsync + Vback - 1;
	sync_info_t.TEndBLine = Vfront;
	sync_info_t.BVLine = 0;
	sync_info_t.BBLineOfs = 0;
	sync_info_t.BEndBLine = 0;
	sync_info_t.HSyncLength = Hsync -1;
	sync_info_t.VSyncLength0 = Vsync - 1;
	return 0;
}

static void writelr_delay(uint32_t value, uint32_t addr, uint32_t start, uint32_t bit)
{
	uint32_t region = (0xFFFFFFFFU << ( start + bit )) | (0xFFFFFFFFULL >> ( 32 - start ));
	uint32_t tmp = readl(addr);
	tmp &= region;
	tmp |= (~region) & (value << start);
	writel(tmp, addr);
	mdelay(1);
}

#endif
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
 * fast_osd(0, 0, 1024, 768, 0x40000000, 1024, 1024, 768);
 * 2. full screen osd with smaller picture(yuv only):
 * fast_osd(0, 0, 256, 150, 0x40000000, 1024, 1024, 768);
 * 3. topleft 1/16 display area osd with same-sized picture
 * fast_osd(0, 0, 256, 150, 0x40000000, 256, 150, 256);
 */

void fast_osd(int type, int x, int y, int w, int h, uint32_t addr, int pw, int pp, int ph)
{
#ifdef MIPI_LCD
	uint32_t clkdiv = 0;
	sync_info_gen();
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
	writelr(0x00000007, 0x34500068, 11, 3);
	writelr(0x00000001, 0x3450007c, 0, 8);
	writelr(0x00000007, 0x3450007c, 8, 8);
	writelr(0x0000001f, 0x34500074, 0, 5);
	writelr(0x00000002, 0x34500074, 5, 2);
	writelr(0x00000000, 0x34500074, 7, 3);
	writelr(0x0a026877, 0x3450005c, 0, 32);
	writelr(0x000000b6, 0x34500058, 0, 32);
	writelr(0x000001f4, 0x34500000, 0, 15);
	writelr(0x00002710, 0x34500004, 0, 31);

	writelr(0x00001902, 0x34500008, 0, 32);//08

	writelr(0x00120609, 0x3450000c, 0, 32);
	writelr(0x00000100, 0x34500078, 0, 32);
	writelr(0x000001f4, 0x34500010, 0, 32);
	writelr(0x00002710, 0x34500014, 0, 32);

	writelr(0x001e0809, 0x34500018, 0, 32);//18

	writelr(0x00000100, 0x3450001c, 0, 32);

	writelr(0x001c0a23, 0x34500020, 0, 32);//20

	writelr(0x000001f4, 0x34500024, 0, 32);
	writelr(0x00002710, 0x34500028, 0, 32);

	writelr(0x001e0809, 0x3450002c, 0, 32);//2c

	writelr(0x00000100, 0x34500030, 0, 32);
	writelr(0x000001f4, 0x34500034, 0, 32);
	writelr(0x00002710, 0x34500038, 0, 32);

	writelr(0x001e0809, 0x3450003c, 0, 32);//3c

	writelr(0x00000100, 0x34500040, 0, 32);
	writelr(0x000001f4, 0x34500044, 0, 32);
	writelr(0x00002710, 0x34500048, 0, 32);

	writelr(0x001e0809, 0x3450004c, 0, 32);//4c

	writelr(0x00000100, 0x34500050, 0, 32);
	writelr(0x0000000c, 0x34500054, 0, 8);
	writelr(0x0000001f, 0x34500074, 0, 5);
	writelr(0x00000000, 0x34500074, 5, 2);
	writelr(0x00000000, 0x34500074, 7, 3);
	writelr(0x00000001, 0x3450007c, 0, 8);
	writelr(0x0000000a, 0x3450007c, 8, 8);

	writelr(0x00000000, 0x3450006c, 0, 3);
	writelr(0x00000000, 0x3450006c, 3, 1);
	writelr(0x00000000, 0x3450006c, 4, 2);
	writelr(0x00000000, 0x3450006c, 6, 1);

	udelay(1000);

	writelr(0x00000000, 0x34500064, 1, 7);
	writelr(0x0000000f, 0x34500064, 9, 4);//

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
	writelr(sync_info_t.Hactvie , 0x3430003c, 0, 14);//writelr(0x00000320, 0x3430003c, 0, 14);
	writelr(0x00000001, 0x34300040, 0, 13);
	writelr(0x0000000a, 0x34300044, 0, 13);

	clkdiv = mipi_dphy_hal_get_pixel_clk_div();

	writelr((sync_info_t.Hsync * clkdiv / 8), 0x34300048, 0, 12);//writelr(0x00000009, 0x34300048, 0, 12);
	writelr((sync_info_t.Hback * clkdiv / 8), 0x3430004c, 0, 12);//writelr(0x00000014, 0x3430004c, 0, 12);
	writelr(((sync_info_t.Hsync + sync_info_t.Hback + sync_info_t.Hfront + sync_info_t.Hactvie) * clkdiv / 8), 0x34300050, 0, 15);//writelr(0x000003a5, 0x34300050, 0, 15);
	writelr(sync_info_t.Vsync, 0x34300054, 0, 10);//writelr(0x00000004, 0x34300054, 0, 10);
	writelr(sync_info_t.Vback, 0x34300058, 0, 10);//writelr(0x00000018, 0x34300058, 0, 10);
	writelr(sync_info_t.Vfront, 0x3430005c, 0, 10);//writelr(0x00000014, 0x3430005c, 0, 10);
	writelr(sync_info_t.Vactive, 0x34300060, 0, 14);//writelr(0x00000500, 0x34300060, 0, 14);
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

#if 1 //gpio7-1 
	printf("start config reset gpio\n");
	//writel(0x00000001, 0x28200130); //function
	writel(0x00000002, 0x1b070004); //mask
	writel(0x00000002, 0x1b070008); //output
	writel(0x00000000, 0x1b070000);
	//mdelay(200);
	writel(0x00000002, 0x1b070000);
	mdelay(20);
	writel(0x00000000, 0x1b070000);
	mdelay(60);
	writel(0x00000002, 0x1b070000);
	mdelay(120);
#endif
#if 1
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
#endif	
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
	writel((sync_info_t.Vactive << 16) + sync_info_t.Hactvie, 0x34101024);//writel(0x05000320, 0x34101024);
	writel((sync_info_t.StartBlen << 16) + sync_info_t.Hlen, 0x34101028);//writel(0x0019033d, 0x34101028);
	writel((sync_info_t.TBLineOfs << 16) + sync_info_t.TVLine, 0x3410102C);//writel(0x001b052f, 0x3410102C);
	writel((sync_info_t.BBLineOfs << 16) + sync_info_t.BVLine, 0x34101030);//writel(0x00000000, 0x34101030);
	writel(0xff00ff00, 0x34101034);

	writel(0x349ea100, 0x34101060);
	writel(0x00071900, 0x34101064);
	writel(0x16700100, 0x34101068);
	writel(0x3cd47487, 0x3410106c);
	writel(0x00000001, 0x34101000);
	//writel(0x00000003, 0x34101000);  //vou test mode

	writel(0x00000000, 0x34101014);

	if(type == LOGO_YUV) {
		printf("LOGO_YUV !!!\n");
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
			printf("LOGO_RGB_8888 !!!\n");
			//writel(0x00000002, 0x34101120); // mc abgr8888
			//writel(0x0000001A, 0x34101120); //hik argb8888
			writel(0x0000001A, 0x34101120);
	    } else if (type == LOGO_RGB_1555) {
			printf("LOGO_RGB_1555 !!!\n");
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
#else
	//[0]_vou_cmd_set_dev_output_info[1024x768/1/60]65000kHz -> 65000kHz
	//[0]_vou_cmd_toggle_dev=1
	//[0]kick off start device!
	vou_global();
	writebs(0x0b, 0x34204002);
	writebc(0x0000000b, 0x34204002);
	writelr(0x00042384, 0x2800006c, 0, 20);
	writelr(0x00002384, 0x2800006c, 0, 20);
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
	writel(0x0ccc0000, 0x28500080);
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

	writel(0x03000400, 0x34101024);
	writel(0x0127053f, 0x34101028);
	writel(0x00220325, 0x3410102c);
	writel(0x00000000, 0x34101030);
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

void logo_color(void)
{
	static int idx = 2;
	int i, size;
#if (LOGO_RGB_FORMAT == LOGO_RGB_1555)
	unsigned short colors[3] = {0xF800, 0x07C0, 0x003E};
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

}

void logo_display(void)
{
    //vou_global();
#if (LOGO_USE_YUV == 0)
	printf("logo_display fast_osd\n");
	fast_osd(LOGO_RGB_FORMAT, 0, 0, LOGO_W, LOGO_H, LOGO_RGB_ADDR_MEM, LOGO_W, LOGO_W, LOGO_H);
#else
	fast_osd(LOGO_YUV, 0, 0, LOGO_W, LOGO_H, LOGO_YUV_ADDR_MEM, LOGO_W, LOGO_W, LOGO_H);
	//memcpy((void*)LOGO_YUV_ADDR_MEM, &t_yuv[0], sizeof(t_yuv));
	//fast_osd(LOGO_YUV, 0, 0, 256, 150, LOGO_YUV_ADDR_MEM, 256, 256, 150);
#endif
}

void boot_logo(void)
{
	printf("start boot_logo!\n");
	logo_color();
	logo_display();
	printf("end boot_logo!\n");
}
