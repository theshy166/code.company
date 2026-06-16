/**
 * @file fbmpi.c
 *
 */
#ifdef CONFIG_XOS_USE_DLMPI
/*********************
 *      INCLUDES
 *********************/
#include "fbdev.h"

#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <linux/fb.h>

#include <sys/sem.h>

#include "fh_defines.h"
#include "fh_math.h"

#include "fh_common.h"
#include "fh_system_mpipara.h"
#include "fh_system_mpi.h"
#include "vmm_api.h"
#include "fh_vb_mpi.h"
#include "fh_vo_mpi.h"
#include "fh_venc_mpi.h"
#include "fh_vdec_mpi.h"
#include "fh_vgs_mpi.h"
#include "fh_tde_mpi.h"
#include "fb_drv_ioc.h"

/*********************
 *      DEFINES
 *********************/

#ifndef DIV_ROUND_UP
#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#endif

#define DHD0_FB_TEST 1
#define DHD1_FB_TEST 0

#define DHD0_GFX_ARGB8888_FMT 1
#define DHD1_GFX_ARGB8888_FMT 0

#define WIDTH                  LV_USE_HOR_SIZE
#define HEIGHT                 LV_USE_VER_SIZE

#define SAMPLE_VO_DEV_DHD0 0
#define SAMPLE_VO_DEV_DHD1 1

#define GRAPHICS_LAYER_G0  0
#define GRAPHICS_LAYER_G1  4

#define SAMPLE_SYS_ALIGN_WIDTH  16

#define FYFB_RED_1555   0xFC00
#define FYFB_BLUE_1555   0x801F

#define XOS_LOGU(fmt, ...)
#define XOS_LOGI(fmt, ...)    LV_LOG_ERROR(fmt, ##__VA_ARGS__)
#define XOS_LOGE(fmt, ...)    LV_LOG_ERROR(fmt, ##__VA_ARGS__)

static struct fb_bitfield s_r16 = {10, 5, 0};
static struct fb_bitfield s_g16 = {5, 5, 0};
static struct fb_bitfield s_b16 = {0, 5, 0};
static struct fb_bitfield s_a16 = {15, 1, 0};

static struct fb_bitfield s_a32 = {24,8,0};
static struct fb_bitfield s_r32 = {16,8,0};
static struct fb_bitfield s_g32 = {8,8,0};
static struct fb_bitfield s_b32 = {0,8,0};

static unsigned char dhd0_fb_test_en = DHD0_FB_TEST;
static unsigned char dhd1_fb_test_en = DHD1_FB_TEST;

typedef struct fyGfxSize {
    FY_S32 w;
    FY_S32 h;
} fyGfxSize_t;

static fyGfxSize_t gfx[2];

extern char*fbdev_get_fbp(void);
extern void fbdev_get_info(struct fb_var_screeninfo* , struct fb_fix_screeninfo *);
extern void fbdev_init_xd(void);
extern void fbdev_exit_xd(void);
extern int  fbdev_pan_disp(void);

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      STRUCTURES
 **********************/

struct bsd_fb_var_info{
    uint32_t xoffset;
    uint32_t yoffset;
    uint32_t xres;
    uint32_t yres;
    int bits_per_pixel;
 };

struct bsd_fb_fix_info{
    long int line_length;
    long int smem_len;
};

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;

static char *fbp = 0;
static long int screensize = 0;
static int fbfd = 0;

static void dump_fix(struct fb_fix_screeninfo *info, char *msg);
static void dump_var(struct fb_var_screeninfo *info, char *msg);

#ifndef CEILING_2_POWER
#define CEILING_2_POWER(x,a)     ( ((x) + ((a) - 1) ) & ( ~((a) - 1) ) )
#endif

/**********************
 *      MACROS
 **********************/

#define ___CHECK_GOTO(cond, label, msg, ...)  \
if(cond) { \
  printf("[Func]:%s [Line]:%d: " msg "\n",  __FUNCTION__, __LINE__, ##__VA_ARGS__); \
  goto label; \
}

static int FB_COMM_SYS_Init(VB_CONF_S *pstVbConf)
{
#if 0
    int i=0, ret;
	VB_CONF_S stVbConf;

	FH_SYS_Exit();
	for(i=0;i<VB_MAX_USER;i++){
		FH_VB_ExitModCommPool(i);
	}
	for(i=0; i<VB_MAX_POOLS; i++){
		FH_VB_DestroyPool(i);
	}
	FH_VB_Exit();

	ret = FH_VB_SetConf(&stVbConf);
	___CHECK_GOTO((FH_SUCCESS != ret), EXIT, "FH_VB_SetConf failed!");

	ret = FH_VB_Init();
	___CHECK_GOTO((FH_SUCCESS != ret), EXIT, "FH_VB_Init failed!");

	ret = FH_SYS_Init();
	___CHECK_GOTO((FH_SUCCESS != ret), EXIT, "FH_SYS_Init failed!");

EXIT:
	return ret;
#endif
    /*
    MPP_SYS_CONF_S stSysConf = {0};
    int s32Ret = -1;
    int i = 0;

    do{
        s32Ret = FY_MPI_SYS_Exit();
        if (s32Ret) usleep(1000);
        if (++i > 1000) break;
    }while(s32Ret);

    for (i = 0; i < VB_UID_BUTT; i++)
        FY_MPI_VB_ExitModCommPool(i);
    for (i = 0; i < VB_MAX_POOLS; i++)
        FY_MPI_VB_DestroyPool(i);
    FY_MPI_VB_Exit();

    s32Ret = FY_MPI_VB_SetConf(pstVbConf);
    if (0 != s32Ret)
    {
        XOS_LOGE("FY_MPI_VB_SetConf Error s32Ret=0x%x!", s32Ret);
        return -1;
    }

    s32Ret = FY_MPI_VB_Init();
    if (0 != s32Ret)
    {
        XOS_LOGE("FY_MPI_VB_Init Error s32Ret=%d!", s32Ret);
        return -1;
    }

    stSysConf.u32AlignWidth = SAMPLE_SYS_ALIGN_WIDTH;
    s32Ret = FY_MPI_SYS_SetConf(&stSysConf);
    if (0 != s32Ret)
    {
        XOS_LOGE("FY_MPI_SYS_SetConf(u32AlignWidth:%d) Error ret=0x%x", stSysConf.u32AlignWidth, s32Ret);
        return -1;
    }

    s32Ret = FY_MPI_SYS_Init();
    if (0 != s32Ret)
    {
        XOS_LOGE("FY_MPI_SYS_Init Error ret=%d", s32Ret);
        return -1;
    }
*/
    return 0;
}

static void FB_COMM_SYS_Exit(void)
{
 /*
	 FY_S32	i,ret;
	ret	= FY_MPI_SYS_Exit();
	sleep(2);
	printf("FY_MPI_SYS_Exit() ret=%d\n",ret);

	for(i=0;i<VB_UID_BUTT;i++)
	{
		 ret = FY_MPI_VB_ExitModCommPool(i);
		 printf("FY_MPI_VB_ExitModCommPool(%d) ret=%d\n",i,ret);
	}
	for(i=0; i<VB_MAX_POOLS; i++)
	{
		 ret = FY_MPI_VB_DestroyPool(i);
		 if(ret==0)	printf("FY_MPI_VB_DestroyPool(%d) ret=%d\n",i,ret);
	}
	ret	= FY_MPI_VB_Exit();
	printf("FY_MPI_VB_Exit ret=%d\n",ret);
*/
}

static int SAMPLE_COMM_VO_StartDev(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr)
{
	int s32Ret = FH_SUCCESS;

	s32Ret = FH_VO_SetPubAttr(VoDev, pstPubAttr);
	if (s32Ret != FH_SUCCESS)
	{
		printf("failed with %#x!\n", s32Ret);
		return FH_FAILURE;
	}

	if(pstPubAttr->enIntfType & VO_INTF_HDMI){
		VO_HDMI_FMT_S stHdmiFmt;
		if(0){
	            stHdmiFmt.bHdmiFmt = HDMI_FORMAT_BT_1120;		//emb
	        }
	        else{
	            stHdmiFmt.bHdmiFmt = HDMI_FORMAT_EXT_YUV422_16BIT;		//ext
	        }
		s32Ret = FH_VO_SetHdmiFmt(VoDev, &stHdmiFmt);
	}

	s32Ret = FH_VO_Enable(VoDev);
	if (s32Ret != FH_SUCCESS)
	{
		printf("failed with %#x!\n", s32Ret);
		return FH_FAILURE;
	}

	return s32Ret;
}

static int FB_COMM_VO_Init(void)
{
    FY_S32 s32Ret = FY_SUCCESS;
	VO_DEV VoDev;
	VO_PUB_ATTR_S stVoPubAttr;
	USER_SYNC_INFO_S syncInfo;

    if (dhd0_fb_test_en) {
            printf("dhd0_fb_test_en 1\n");
        VoDev = SAMPLE_VO_DEV_DHD0;

        s32Ret = FH_VO_GetDispSize(VoDev, &syncInfo);
        if (FH_SUCCESS != s32Ret)
        {
            printf("FH_VO_GetDispSize failed!\n");
            goto __EXIT;
        }

        stVoPubAttr.enIntfSync = VO_OUTPUT_USER;
        stVoPubAttr.enIntfType = VO_INTF_LCD;
        stVoPubAttr.u32BgColor = 0xffffff;
        stVoPubAttr.stUserSync.width = syncInfo.width;
        stVoPubAttr.stUserSync.height = syncInfo.height;
        stVoPubAttr.stUserSync.framerate = syncInfo.framerate;

        gfx[0].w = syncInfo.width;
        gfx[0].h = syncInfo.height;

        printf("FB_COMM_VO_Init hd0 w=%d h=%d\n", gfx[0].w, gfx[0].h);
        s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
        if (FH_SUCCESS != s32Ret)
        {
            printf("Start SAMPLE_COMM_VO_StartDev failed!\n");
            goto __EXIT;
        }
            printf("dhd0_fb_test_en 2\n");
    }
    //while(1);   // Mark 

    if (dhd1_fb_test_en) {
            printf("dhd1_fb_test_en 1\n");
        VoDev = SAMPLE_VO_DEV_DHD1;

        s32Ret = FH_VO_GetDispSize(VoDev, &syncInfo);
        if (FH_SUCCESS != s32Ret)
        {
            printf("FH_VO_GetDispSize failed!\n");
            goto __EXIT;
        }

        stVoPubAttr.enIntfSync = VO_OUTPUT_USER;
        stVoPubAttr.enIntfType = VO_INTF_LCD;
        stVoPubAttr.u32BgColor = 0xFF000000;
        stVoPubAttr.stUserSync.width = syncInfo.width;
        stVoPubAttr.stUserSync.height = syncInfo.height;
        stVoPubAttr.stUserSync.framerate = syncInfo.framerate;

        gfx[1].w = syncInfo.width;
        gfx[1].h = syncInfo.height;

        printf("FB_COMM_VO_Init hd1 w=%d h=%d\n", gfx[1].w, gfx[1].h);

        s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
        if (FH_SUCCESS != s32Ret)
        {
            printf("Start SAMPLE_COMM_VO_StartDev failed!\n");
            goto __EXIT;
        }
            printf("dhd1_fb_test_en 2\n");
    }

__EXIT:
	printf("FB_COMM_VO_Init ret=%d\n",s32Ret);
	return 0;
}

static FY_VOID	FB_COMM_VO_Exit(void)
{
	FY_S32 s32Ret = FY_SUCCESS;

    if (dhd0_fb_test_en) {
        s32Ret = FH_VO_Disable(SAMPLE_VO_DEV_DHD0);
        if (FY_SUCCESS!=s32Ret) {
            printf("FY_MPI_VO_Disable, s32Ret=0x%X.\n", s32Ret);
        }
    }

    if (dhd1_fb_test_en) {
        s32Ret = FH_VO_Disable(SAMPLE_VO_DEV_DHD1);
        if (FY_SUCCESS!=s32Ret) {
            printf("FY_MPI_VO_Disable, s32Ret=0x%X.\n", s32Ret);
        }
    }

	printf("FB_COMM_VO_Exit ret=%d\n",s32Ret);
}

static void dump_fix(struct fb_fix_screeninfo *info, char *msg)
{
    printf("\nfix_screeninfo %s:\n\
char id[16]              = %s\n\
unsigned long smem_start = 0x%lx\n\
__u32 smem_len           = %u\n\
__u32 type               = %u\n\
__u32 type_aux           = %u\n\
__u32 visual             = %u\n\
__u16 xpanstep           = %hu\n\
__u16 ypanstep           = %hu\n\
__u16 ywrapstep          = %hu\n\
__u32 line_length        = %u\n\
unsigned long mmio_start = %lu\n\
__u32 mmio_len           = %u\n\
__u32 accel              = %u\n\
__u16 capabilities       = %hu\n",
             msg,
             info->id,
             info->smem_start,
             info->smem_len,
             info->type,
             info->type_aux,
             info->visual,
             info->xpanstep,
             info->ypanstep,
             info->ywrapstep,
             info->line_length,
             info->mmio_start,
             info->mmio_len,
             info->accel,
             info->capabilities);
}

static void dump_var(struct fb_var_screeninfo *info, char *msg)
{
    printf("\nvar_screeninfo %s:\n\
__u32 xres                = %u\n\
__u32 yres                = %u\n\
__u32 xres_virtual        = %u\n\
__u32 yres_virtual        = %u\n\
__u32 xoffset             = %u\n\
__u32 yoffset             = %u\n\
__u32 bits_per_pixel      = %u\n\
__u32 grayscale           = %u\n\
__u32 nonstd              = %u\n\
__u32 activate            = %u\n\
__u32 height              = %d\n\
__u32 width               = %d\n\
__u32 accel_flags         = %u\n\
__u32 pixclock            = %u\n\
__u32 left_margin         = %u\n\
__u32 right_margin        = %u\n\
__u32 upper_margin        = %u\n\
__u32 lower_margin        = %u\n\
__u32 hsync_len           = %u\n\
__u32 vsync_len           = %u\n\
__u32 sync                = %u\n\
__u32 vmode               = %u\n\
__u32 rotate              = %u\n\
__u32 colorspace          = %u\n",
             msg,
             info->xres,
             info->yres,
             info->xres_virtual,
             info->yres_virtual,
             info->xoffset,
             info->yoffset,
             info->bits_per_pixel,
             info->grayscale,
             info->nonstd,
             info->activate,
             info->height,
             info->width,
             info->accel_flags,
             info->pixclock,
             info->left_margin,
             info->right_margin,
             info->upper_margin,
             info->lower_margin,
             info->hsync_len,
             info->vsync_len,
             info->sync,
             info->vmode,
             info->rotate,
             info->colorspace);
}

char * fbdev_get_fbp(void)
{
    return fbp;
}

void fbdev_get_info(struct fb_var_screeninfo* pOutScreenInfo, struct fb_fix_screeninfo *pOutFixInfo)
{
    memcpy(pOutScreenInfo, &vinfo, sizeof(struct fb_var_screeninfo));
    memcpy(pOutFixInfo, &finfo, sizeof(struct fb_fix_screeninfo));
}

void fbdev_init_xd(void)
{
    printf("%s e\n", __func__);
    int s32Ret = 0;
    unsigned int u32PicWidth = WIDTH;
    unsigned int u32PicHeight = HEIGHT;
    bool bShow=true, bEnable=0;
    //	FY_U8 *pShowScreen;

    VB_CONF_S stVbConf;
    unsigned int u32BlkSize;
    FYFB_LAYER_INFO_S stLayerinfo;
    // int i, x, y;
    int _k=0;
    	FY_S32 width = 0;
	FY_S32 height = 0;

    // struct fb_fix_screeninfo fix;
    // struct fb_var_screeninfo var;

    // while(1);   // normal disp
    /******************************************
     step  1: init variable
    ******************************************/
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = CEILING_2_POWER(u32PicWidth, 16) * CEILING_2_POWER(u32PicHeight, 16) * 2;
    stVbConf.u32MaxPoolCnt = 128;

    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 6;

    /******************************************
     step 2: mpp system init.
    ******************************************/
    s32Ret = FB_COMM_SYS_Init(&stVbConf);
    if (0 != s32Ret)
    {
        // return;
        printf("system init Error failed with %d!\n", s32Ret);
        // goto SAMPLE_FYFB_NoneBufMode_0;
    }

    // while(1);   // normal disp
    // return for debug here;
    //	return;

    /******************************************
     step 3:  start vo hd0.
    *****************************************/
    FB_COMM_VO_Init();
    // while(1);   // black disp

    // Open the file for reading and writing
    // fbfd = open(FBDEV_PATH, O_RDWR);
    for(int i=0; i<1000; i++)
        _k++;
    sleep(1);
    printf("_k=%d\n", _k);

    if (dhd0_fb_test_en)
    {
        fbfd = open("/dev/fb0", O_RDWR, 0);
        width = gfx[0].w;
        height = gfx[0].h;
        printf("file=%s fd=%d width=%d height=%d\n", "/dev/fb0", fbfd, width, height);
    }

    if (dhd1_fb_test_en)
    {
        fbfd = open("/dev/fb4", O_RDWR, 0);
        width = gfx[1].w;
        height = gfx[1].h;
        printf("file=%s fd=%d width=%d height=%d\n", "/dev/fb4", fbfd, width, height);
    }

    if (fbfd < 0)
    {
        printf("Error: cannot open framebuffer device\n");
        goto __EXIT_VOU;
    }
    printf("The framebuffer device was opened successfully\n");

    // Make sure that the display is on.
    // if (ioctl(fbfd, FBIOBLANK, FB_BLANK_UNBLANK) != 0) {
    // while(1);   // black disp
    bShow = 0;
    if (ioctl(fbfd, FBIOPUT_SHOW_FYFB, &bShow) != 0)
    {
        printf("ioctl(FBIOBLANK) Error\n");
        close(fbfd);
        goto __EXIT_VOU;
    }

    /* 2. set the screen original position */
    FYFB_POINT_S stPoint = {0, 0};
    stPoint.s32XPos = 0;
    stPoint.s32YPos = 0;
    if (ioctl(fbfd, FBIOPUT_SCREEN_ORIGIN_FYFB, &stPoint) != 0)
    {
        printf("Error reading fixed information");
        close(fbfd);
        goto __EXIT_VOU;
    }
    // while(1);   // black disp

    /* 3 get the variable screen info  */
    // Get variable screen information
    // hard code here for verifying only
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0)
    {
        printf("Error reading variable information\n");
        close(fbfd);
        goto __EXIT_VOU;
    }

    //	usleep(1000);
    /* 4. modify the variable screen info
          the screen size: IMAGE_WIDTH*IMAGE_HEIGHT
          the virtual screen size: VIR_SCREEN_WIDTH*VIR_SCREEN_HEIGHT
          (which equals to VIR_SCREEN_WIDTH*(IMAGE_HEIGHT*2))
          the pixel format: ARGB1555
    */
    // my_delay_ms(1000);
    // usleep(1000);

    if (dhd0_fb_test_en) {
    #if DHD0_GFX_ARGB8888_FMT
        vinfo.xres_virtual = width; // src total canvas
        vinfo.yres_virtual = height;
        vinfo.xoffset = vinfo.yoffset = 0; // reset src offset
        vinfo.xres = width; // src size
        vinfo.yres = height;

        vinfo.transp= s_a32;
        vinfo.red = s_r32;
        vinfo.green = s_g32;
        vinfo.blue = s_b32;
        vinfo.bits_per_pixel = 32;
        vinfo.activate = FB_ACTIVATE_NOW;

        printf("vinfo.xres_virtual=%d vinfo.yres_virtual=%d vinfo.xoffset=%d vinfo.yoffset=%d", vinfo.xres_virtual, vinfo.yres_virtual, vinfo.xoffset, vinfo.yoffset);
        printf("vinfo.xres=%d vinfo.yres=%d", vinfo.xres, vinfo.yres);
    #else
        vinfo.xres_virtual = width; // src total canvas
        vinfo.yres_virtual = height;
        vinfo.xoffset = vinfo.yoffset = 0; // reset src offset
        vinfo.xres = width; // src size
        vinfo.yres = height;

        vinfo.transp= s_a16;
        vinfo.red = s_r16;
        vinfo.green = s_g16;
        vinfo.blue = s_b16;
        vinfo.bits_per_pixel = 16;
    #endif
    }

    if (dhd1_fb_test_en) {

    #if DHD0_GFX_ARGB8888_FMT
        vinfo.xres_virtual = width; // src total canvas
        vinfo.yres_virtual = height;
        vinfo.xoffset = vinfo.yoffset = 0; // reset src offset
        vinfo.xres = width; // src size
        vinfo.yres = height;

        vinfo.transp= s_a32;
        vinfo.red = s_r32;
        vinfo.green = s_g32;
        vinfo.blue = s_b32;
        vinfo.bits_per_pixel = 32;
        vinfo.activate = FB_ACTIVATE_NOW;
    #else
        vinfo.xres_virtual = width; // src total canvas
        vinfo.yres_virtual = height;
        vinfo.xoffset = vinfo.yoffset = 0; // reset src offset
        vinfo.xres = width; // src size
        vinfo.yres = height;

        vinfo.transp= s_a16;
        vinfo.red = s_r16;
        vinfo.green = s_g16;
        vinfo.blue = s_b16;
        vinfo.bits_per_pixel = 16;
    #endif
    }


    dump_var(&vinfo, "vinfo");

    printf("vinfo.yres=%d\n", vinfo.yres);
    /* 5. set the variable screeninfo */
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo) < 0)
    {
        printf("Put variable screen info failed!\n");
        close(fbfd);
        goto __EXIT_VOU;
    }
    printf("vinfo.yres=%d\n", vinfo.yres);
    // while(1);   // black disp
    /* 6. get the fix screen info */
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) < 0)
    {
        printf("Get fix screen info failed!\n");
        close(fbfd);
        goto __EXIT_VOU;
    }
    dump_fix(&finfo, "finfo");

    if (ioctl(fbfd, FBIOGET_LAYER_INFO, &stLayerinfo) < 0)
    {
        printf("FBIOGET_LAYER_INFO failed!\n");
        close(fbfd);
        goto __EXIT_VOU;
    }

   	stLayerinfo.u32Mask = 0;
    stLayerinfo.BufMode = FYFB_LAYER_BUF_NONE;
    stLayerinfo.u32Mask |= FYFB_LAYERMASK_BUFMODE;

    s32Ret = ioctl(fbfd, FBIOPUT_LAYER_INFO, &stLayerinfo);
    if (s32Ret < 0)
    {
        printf("FBIOPUT_LAYER_INFO failed!\n");
        close(fbfd);
        goto __EXIT_VOU;
    }

    s32Ret = ioctl(fbfd, FBIOGET_COMPRESSION_FYFB, &bEnable);
    if ( s32Ret ) {
		printf("get compression err = %d\n", s32Ret);
	} else {
		printf("get compression = %d\n", bEnable);
	}
    // while(1);   // black disp
    //  Figure out the size of the screen in bytes

    bEnable = 1;
    s32Ret = ioctl(fbfd, FBIOPUT_COMPRESSION_FYFB, &bEnable);
    if (s32Ret < 0)
    {
        printf("FBIOPUT_COMPRESSION_FYFB failed!\n");
        close(fbfd);
        goto __EXIT_VOU;
    }

    //unsigned int u32FixScreenStride = finfo.line_length; /*fix screen stride*/
    finfo.smem_len = finfo.line_length * vinfo.yres;
    screensize = finfo.smem_len; // finfo.line_length * vinfo.yres;
    printf("screensize=%d finfo.line_length=%d vinfo.yres=%d!\n", screensize, finfo.line_length, vinfo.yres);
#if 0
    //enable alpha ability
    FYFB_ALPHA_S alpha_s;
    alpha_s.bAlphaEnable = 1;   /**<  alpha enable flag */
    alpha_s.bAlphaChannel = 0;  /**<  alpha channel enable flag */
    alpha_s.u8Alpha0 = 0;         /**<  alpha0 value, used in ARGB1555 */
    alpha_s.u8Alpha1 = 0;         /**<  alpha1 value, used in ARGB1555 */
    alpha_s.u8GlobalAlpha = 128;    /**<  global alpha value */
    alpha_s.u8Reserved = 0;
    s32Ret = ioctl(fbfd, FBIOPUT_ALPHA_FYFB, &alpha_s);
    if (s32Ret < 0) {
        printf("FBIOPUT_ALPHA_FYFB failed!.s32Ret=0x%x\n", s32Ret);
        close(fbfd);
        return;
    }
#endif
    if (DHD0_GFX_ARGB8888_FMT) screensize = WIDTH * HEIGHT * 4;
    else screensize = WIDTH * HEIGHT * 2;
    // Map the device to memory
    // fbp = (char *)mmap(0L, fix.smem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);
    fbp = (char *)mmap((void*)0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((intptr_t)fbp == -1)
    {
        printf("Error: failed to map framebuffer device to memory\n");
        close(fbfd);
        // while(1);
        goto __EXIT_VOU;
    }
    // memset(fbp, 0, screensize);
    // while(1);   // black disp
    /* time to play*/
    bShow = 1;
    if (ioctl(fbfd, FBIOPUT_SHOW_FYFB, &bShow) < 0)
    {
        printf("Error: FBIOPUT_SHOW_FYFB failed\n");
        munmap(fbp, finfo.smem_len);
        close(fbfd);
        goto __EXIT_VOU;
    }

#if 0
    unsigned int *ptmp = (unsigned int*)fbp;
    int __i, __j;

    for(__i=0+WIDTH*10; __i<WIDTH+WIDTH*10; __i++)
        ptmp[__i] = 0x8000FF00;

    for(__i=WIDTH*100; __i<WIDTH+WIDTH*100; __i++)
        ptmp[__i] = 0x800000FF;

    for(__j=0; __j<300; __j++)
        for(__i=0; __i<WIDTH; __i++)
            ptmp[__i+__j*WIDTH] = (__j%10)?0x800000FF: 0x8000FF00;

    for(__i=0; __i<WIDTH; __i++)
        for(__j=0; __j<HEIGHT; __j++)
            ptmp[__i+__j*WIDTH] = (__j%10)?0x800000FF: 0x8000FF00;

	if (ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo) < 0)
	{
		printf("FBIOPAN_DISPLAY failed!\n");
		close(fbfd);
		goto __EXIT_VOU;
	}

    {
        int i, j;

        for ( i = 0; i < WIDTH; i++ ) {
            for ( j = 0; j < HEIGHT; j++ ) {
                *(((uint16_t*)fbp)+i*WIDTH+j) = 0x8000 + (j%0x100) + (((i/256)*(j/256)*8)<<8);
            }
        }

		if (ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo) < 0)
		{
			printf("FBIOPAN_DISPLAY failed!\n");
			close(fbfd);
			goto __EXIT_VOU;
		}
    }


    //memset(fbp, 0x80, finfo.smem_len);
    unsigned int *ptmp = (unsigned int*)fbp;
    int __i, __j;
    for(__i=0+1024*10; __i<1024+1024*10; __i++)
        ptmp[__i] = 0x8000FF00; //FYFB_RED_1555;  0x8000FF00;

    for(__i=1024*100; __i<1024+1024*100; __i++)
        ptmp[__i] = 0x800000FF; //FYFB_RED_1555;  0x800000FF;

    for(__j=0; __j<300; __j++)
        for(__i=0; __i<1024; __i++)
            ptmp[__i+__j*1024] = (__j%10)?0x800000FF: 0x8000FF00; //FYFB_RED_1555;  0x800000FF;


    /* time to play*/
    bShow = true;
    if (ioctl(fbfd, FBIOPUT_SHOW_FYFB, &bShow) < 0)
    {
        printf("FBIOPUT_SHOW_FYFB failed!");
        munmap(fbp, finfo.smem_len);
        close(fbfd);
        return;
    }
    for(__i=0+1024*200; __i<1024+1024*200; __i++)
        ptmp[__i] = 0x8000FF00; //FYFB_RED_1555;  0x8000FF00;

    for(__i=1024*300; __i<1024+1024*300; __i++)
        ptmp[__i] = 0x800000FF; //FYFB_RED_1555;  0x800000FF;

    for(__j=0; __j<300; __j++)
        for(__i=0; __i<1024; __i++)
            ptmp[__i+__j*1024] = (__j%10)?0x800000FF: 0x8000FF00; //FYFB_RED_1555;  0x800000FF;


    if (ioctl(fbfd, FBIOPAN_DISPLAY, &var) < 0)
    {
        printf("FBIOPAN_DISPLAY failed!");
        munmap(pShowScreen, fix.smem_len);
        close(fbfd);
        return;
    }

    //if (GRAPHICS_LAYER_HC0 != pstInfo->ctrlkey)
    {
        var.yoffset = 0;
        FY_U16 *ptemp = (FY_U16*)(pShowScreen);
        for (y = 0; y < HEIGHT; y++)
        {
            for (x = 0; x < WIDTH/2; x++)
            {
                *(ptemp + y * var.xres + x) = 0x0;
            }
            for (x = WIDTH/2; x < WIDTH; x++)
            {
                *(ptemp + y * var.xres + x) = FYFB_RED_1555;
            }
            usleep(100);
        }
        sleep(2);

    }

    sleep(20);
    close(fbfd);
    return FY_NULL;
#endif
    // Don't initialise the memory to retain what's currently displayed / avoid clearing the screen.
    // This is important for applications that only draw to a subsection of the full framebuffer.
    printf("The framebuffer device was mapped to memory successfully\n");
    printf("%s x\n", __func__);
    //while(1);
    return;


__EXIT_VOU:
    FB_COMM_VO_Exit();
__EXIT_SYS:
    FB_COMM_SYS_Exit();
    printf("%s x error.\n", __func__);
}

void fbdev_exit_xd(void)
{
    close(fbfd);
    FB_COMM_VO_Exit();
    FB_COMM_SYS_Exit();
}

int fbdev_pan_disp(void)
{
//    printf("%s e\n", __func__);
    if (ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo) < 0)
    {
        printf("FBIOPAN_DISPLAY failed!");
        return -1;
    }
 //   printf("%s x\n", __func__);
    return 0;
}

static void fbdev_get_sizes(uint32_t *width, uint32_t *height, uint32_t *dpi) {
    if (width)
        *width = vinfo.xres;

    if (height)
        *height = vinfo.yres;

    if (dpi && vinfo.height)
        *dpi = DIV_ROUND_UP(vinfo.xres * 254, vinfo.width * 10);
}

static void fbdev_set_offset(uint32_t xoffset, uint32_t yoffset) {
    vinfo.xoffset = xoffset;
    vinfo.yoffset = yoffset;
}

#endif /* CONFIG_XOS_RES_800x1280 */