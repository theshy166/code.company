
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <time.h>

#include "uvc.h"
#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "system/qua_mm_system.h"
#include "display/qua_mm_display.h"
#include "video/qua_mm_video.h"
#include "filter/qua_mm_filter.h"


#define NVR_CHN_NUM  1
#define NVR_CHN_WIDTH 640
#define NVR_CHN_HEIGHT 480

static qua_mm_system_t *g_system = NULL;
static qua_mm_vo_device_t *g_vo_device = NULL;
static qua_mm_vo_channel_t *g_vo_chn = NULL;
static qua_mm_vdec_device_t *g_vdec_device = NULL;
static qua_mm_vdec_channel_t *g_vdec_chn  = NULL;
static qua_mm_vppu_device_t *g_vppu_device = NULL;
static qua_mm_vgs_channel_t *g_vgs_chn = NULL;

static int g_vdec_chn_id = -1;
static qua_rect_t g_rect;
static int g_stop = 0;
static pthread_t g_uvctid[NVR_CHN_NUM];
typedef struct{
    int chn;
    int record_need;
} UVC_VDEC_PARAM_S;

static QUA_S32 quamm_sys_device_init() 
{
    QUA_S32 ret = QUA_SUCCESS;
    QUA_BOOL primary = QUA_FALSE;

    ret = qua_mm_init(primary, qua_mm_get_platform(), &g_system);
    if (ret || NULL == g_system){
        printf("qua_mm_init fail\n");
        return QUA_FAILURE;
    }
    
    return QUA_SUCCESS;
}

int voDeinit()
{
    g_vo_chn->disable_chn(0, 0);
    g_vo_chn->parent.release((qua_mm_channel_t *)g_vo_chn);
    g_vo_device->disable_video_layer(0);
    g_vo_device->parent.close(&g_vo_device->parent);
    g_vo_device = NULL;
    return 0;
}

int voInit()
{
    qua_mm_device_t *mm_device = NULL;
    const qua_mm_module_t *display_module = NULL;
    QUA_CONST_CHAR *vo_id, *fb_id;
    QUA_S32 vo_num, fb_num;

    qua_init_display_parser();
    QUA_S32 ret = qua_mm_load_module(QUA_MM_MODULE_DISPLAY, &display_module);
    if (ret != QUA_SUCCESS || display_module == NULL) {
        printf("load display module failed\n");
        return QUA_FAILURE;
    }

    ret = qua_parse_display_id("id:display0", &vo_id, &vo_num, &fb_id, &fb_num);
    if (ret != QUA_SUCCESS) {
        ALOGE("%s qua_parse_display_id error", __func__);
        return QUA_FAILURE;
    }

    ret = display_module->open_device(display_module, vo_id, vo_num, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("open vo dev failed\n");
        return QUA_FAILURE;
    }

    g_vo_device = (qua_mm_vo_device_t *)mm_device;
    g_vo_device->set_video_disp_buflen(0, 2);
    qua_vo_video_layer_attr_t layer_attr;
    qua_vo_compress_attr_t compress_attr;
    ret = g_vo_device->get_video_layer_attr(0, &layer_attr, &compress_attr);
    if (ret)
        return ret;
    
    layer_attr.disp_rect = g_rect;
    layer_attr.image_size.width = g_rect.width;
    layer_attr.image_size.height = g_rect.height;
    layer_attr.pix_format = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
    layer_attr.disp_frmrt = 60;
    ret = g_vo_device->set_video_layer_attr(0, &layer_attr, &compress_attr);
    if (ret)
        return ret;
    
    ret = g_vo_device->enable_video_layer(0);
    if (ret)
        return ret;

    QUA_S32 chn_id = 0;
    qua_mm_channel_t *mm_channel;
    qua_mm_vo_channel_t *vo_chn;
    ret = g_vo_device->parent.create_channel((qua_mm_device_t*)g_vo_device, &chn_id, NULL, &mm_channel);
    if (ret != QUA_SUCCESS) {
        printf("Error: create channel return %d\n", ret);
        return -1;
    }

    vo_chn = (qua_mm_vo_channel_t *)mm_channel;
    qua_vo_chn_attr_t vo_chn_attr;
    ret = vo_chn->get_chn_attr(0, chn_id, &vo_chn_attr);
    if (ret != QUA_SUCCESS) {
        printf("Error: get  chn attr return %d\n", ret);
        return -1;
    }
    
    vo_chn_attr.priority = 0;
    vo_chn_attr.rect = g_rect;
    vo_chn_attr.deflicker = QUA_FALSE;
    ret = vo_chn->set_chn_attr(0, chn_id, &vo_chn_attr);
    if (ret != QUA_SUCCESS) {
        printf("Error: set  chn attr return %d\n", ret);
        return -1;
    }

    ret = vo_chn->set_chn_frame_rate(0, chn_id, 60);
    if (ret != QUA_SUCCESS) {
        printf("Error: set chn frame rate return %d\n", ret);
        return -1;
    }

    ret = vo_chn->enable_chn(0, chn_id);
    if (ret != QUA_SUCCESS) {
        printf("Error: enable chn return %d\n", ret);
        return -1;
    }

    g_vo_chn = vo_chn;
    return 0;
}

static int vdecInit()
{
    const qua_mm_module_t *video_module = NULL;
    int ret = qua_mm_load_module(QUA_MM_MODULE_VIDEO, &video_module);
    if (ret != QUA_SUCCESS || video_module == NULL)
        return -1;
    qua_mm_device_t *mm_device = NULL;
    ret = video_module->open_device(video_module, QUA_MM_VIDEO_DEV_DEC, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL)
        return -1;
    
    qua_mm_vdec_device_t *vdec_device = (qua_mm_vdec_device_t *)mm_device;
    
    qua_vdec_chn_attr_t chn_attr;
    memset(&chn_attr, 0, sizeof(chn_attr));
    chn_attr.coding_type = QUA_VIDEO_CodingMJpeg;
    chn_attr.in_stream_buf_size = 2 * 640 * 480;
    chn_attr.priority = 5;
    chn_attr.pic_width = 640;
    chn_attr.pic_height = 480;
	chn_attr.vdec_jpeg_attr.mode = QUA_VIDEO_MODE_FRAME;
	chn_attr.vdec_jpeg_attr.jpeg_fmt = QUA_JPG_COLOR_FMT_YCBCR420;
   // chn_attr.vdec_video_attr.ref_frame_num = 2;

    qua_mm_channel_t *mm_channel;
    qua_mm_vdec_chn_attr_t vdec_chn_attr;
    vdec_chn_attr.chn_attr = chn_attr;
    vdec_chn_attr.vb_cnt = 4;
	g_vdec_chn_id = -1;
    vdec_device->parent.create_channel(mm_device, &g_vdec_chn_id, (QUA_VOID_PTR)&vdec_chn_attr, &mm_channel);
    qua_mm_vdec_channel_t *vdec_chn = (qua_mm_vdec_channel_t*)mm_channel;

    vdec_chn->start_chn(g_vdec_chn_id);
    g_vdec_device = vdec_device;
    g_vdec_chn = vdec_chn;
    return 0;
}

static int vdecDeinit()
{
    qua_mm_channel_t *mm_channel = (qua_mm_channel_t *)g_vdec_chn;
    g_vdec_chn->stop_chn(g_vdec_chn_id);
    mm_channel->release(mm_channel);
    g_vdec_device->parent.close((qua_mm_device_t *)g_vdec_device);
    return 0;
}

static int vgsInit()
{
    const qua_mm_module_t* filter_module = NULL;
    int ret = qua_mm_load_module(QUA_MM_MODULE_FILTER, &filter_module);
    if (ret != QUA_SUCCESS || filter_module == NULL) {
        printf("load filter module failed\n");
        return -1;
    }

    qua_mm_device_t *mm_device = NULL;
    ret = filter_module->open_device(filter_module, QUA_MM_FILTER_DEV_VPPU, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        printf("Error: open_device QUA_MM_FILTER_DEV_VPPU return %d\n", ret);
        return -1;
    }

    g_vppu_device = (qua_mm_vppu_device_t *)mm_device;
    ret = g_vppu_device->init();
    if (ret != QUA_SUCCESS) {
        printf("Error: init vppu device return %#X\n", ret);
        return -1;
    }
    
    qua_mm_channel_t *mm_channel;
    qua_filter_chn_attr_t filter_chn_attr;
    memset(&filter_chn_attr, 0, sizeof(filter_chn_attr));
    filter_chn_attr.filter_chn = QUA_MM_FILTER_VGS_CHN;
    QUA_S32 chn_id = 0;
    ret = g_vppu_device->parent.create_channel((qua_mm_device_t *)g_vppu_device, &chn_id, (QUA_VOID_PTR)&filter_chn_attr, &mm_channel);
    if (ret != QUA_SUCCESS) {
        printf("Error: create vgs channel return %d\n", ret);
        return -1;
    }
    
    g_vgs_chn = (qua_mm_vgs_channel_t *)mm_channel;
    qua_vgs_chn_mode_t vgs_chn_mode;
    
    vgs_chn_mode.chn_mode = QUA_CHN_MODE_AUTO;
    vgs_chn_mode.width = g_rect.width;
    vgs_chn_mode.height = g_rect.height;
    vgs_chn_mode.pixel_format = QUA_PIXEL_FMT_YUV_SEMIPLANAR_420;
    ret = g_vgs_chn->set_chn_mode(chn_id , 0, &vgs_chn_mode);
    if (ret != QUA_SUCCESS) {
        printf("Error: set vgs chn mode %d\n", ret);
        return -1;
    }

    qua_mm_system_ops_t *sys_ops = (qua_mm_system_ops_t *)g_system;
    qua_mpp_chn_t src_chn;
    qua_mpp_chn_t dst_chn;
    src_chn.obj = QUA_OBJ_VPPU;
    src_chn.dev = 0;
    src_chn.chn = 0;
    dst_chn.mod = QUA_OBJ_VOU;
    dst_chn.dev = 0;
    dst_chn.chn = 0;
    sys_ops->sys_bind(&src_chn, &dst_chn);
    
    src_chn.mod = QUA_OBJ_VDEC;
    src_chn.dev = 0;
    src_chn.chn = g_vdec_chn_id;
    dst_chn.mod = QUA_OBJ_VPPU;
    dst_chn.dev = 0;
    dst_chn.chn = 0;
    sys_ops->sys_bind(&src_chn, &dst_chn);
    return 0;
}

static int vgsDeinit()
{

    qua_mm_system_ops_t *sys_ops = (qua_mm_system_ops_t *)g_system;
    qua_mpp_chn_t src_chn;
    qua_mpp_chn_t dst_chn;
    src_chn.obj = QUA_OBJ_VPPU;
    src_chn.dev = 0;
    src_chn.chn = 0;
    dst_chn.mod = QUA_OBJ_VOU;
    dst_chn.dev = 0;
    dst_chn.chn = 0;
    sys_ops->sys_unbind(&src_chn, &dst_chn);
    
    src_chn.mod = QUA_OBJ_VDEC;
    src_chn.dev = 0;
    src_chn.chn = g_vdec_chn_id;
    dst_chn.mod = QUA_OBJ_VPPU;
    dst_chn.dev = 0;
    dst_chn.chn = 0;
    sys_ops->sys_unbind(&src_chn, &dst_chn);
    g_vgs_chn->parent.release((qua_mm_channel_t *)g_vgs_chn);
    g_vppu_device->exit();
    g_vppu_device->parent.close((qua_mm_device_t *)g_vppu_device);
    return 0;
}

static void *uvcVdecThread(void *arg)
{   
    int ret = 0;
    int open = 0;

    int nReadFailCnt = 0;
    int nReadLen = -1;
    unsigned char *pBuff = NULL;
    UVC_VDEC_PARAM_S stParam;
    qua_vdec_stream_t packet;
    memcpy(&stParam, (UVC_VDEC_PARAM_S*)arg, sizeof(UVC_VDEC_PARAM_S));
    int chn = 0;

    printf("uvcVdecThread start");
    prctl(PR_SET_NAME, "uvcVdecThread", 0, 0, 0);
    while (!g_stop)
	{
		if (!open)
		{
			ret = uvcOpen(chn);
			if (0 == ret)
			{
			    open = 1;
				printf("uvc camera open sucess\n");
			}
			else
			{
			    open = 0;
                sleep(1);
                continue;
			}
		}
		
		nReadLen = uvcRead(chn, &pBuff);
        // printf("read len=%d\n", nReadLen);
		if (nReadLen > 0)
		{
			nReadFailCnt = 0;
            memset(&packet, 0, sizeof(qua_vdec_stream_t));
            packet.pts = 0;
            packet.in_data = (QUA_U64)(pBuff);
            packet.in_size = nReadLen;
            packet.end_of_frame = QUA_TRUE;
            packet.end_of_stream = QUA_FALSE;
            g_vdec_chn->send_stream(g_vdec_chn_id, &packet, 500);
			if (stParam.record_need)
			{
				//FileRecordWrite(chn, pBuff, nReadLen);
			}
			uvcRelease(chn);
		}
		else 
		{
		    printf("uvcRead fail\n");
		    usleep(100 *100);
			//nReadFailCnt++;
		}
		
		if (nReadFailCnt > 5)
		{
			printf(" uvc fail times[%d] need reopen", nReadFailCnt);
			open = 0;
			uvcClose(chn);
			nReadFailCnt = 0;
		}
	}

	uvcClose(chn);
	printf("uvcVdecThread end");
    return NULL;
}

static int uvcInit()
{
    for (int i = 0; i < NVR_CHN_NUM; i++)
    {
        UVC_VDEC_PARAM_S stParam;
        memset(&stParam, 0, sizeof(stParam));
        stParam.chn = i;
        stParam.record_need = 0;
       // FileRecordOpen(i, 32 << 20, NVR_CHN_WIDTH, NVR_CHN_HEIGHT, PT_H264);
        pthread_create(&g_uvctid[i], NULL, uvcVdecThread, (void*)&stParam);
    }

    return 0;
}

static int uvcDeinit()
{
    for (int i = 0; i < NVR_CHN_NUM; i++)
    {
        pthread_join(g_uvctid[i], NULL);
        //FileRecordClose(i);
    }

    return 0;
}

int uvc_preview_start(qua_rect_t *rect)
{
	if (!rect)
		return -1;
	
	memcpy(&g_rect, rect, sizeof(qua_rect_t));
	printf("uvc_preview_start [%d-%d-%d-%d]\n", g_rect.x, g_rect.y, g_rect.width, g_rect.height);
    if (g_system)
        return 0;

    quamm_sys_device_init();
    g_stop = 0;
    if (voInit())
        return -1;
    
    if (vdecInit())
        return -1;
        
    if (vgsInit())
        return -1;

    uvcInit();
    printf("qua uvc_preview_start sucess\n");
	return 0;
}

int uvc_preview_stop()
{
    if (!g_system)
        return 0;
    
    g_stop = 1;
    uvcDeinit();
    vgsDeinit();
    vdecDeinit();
    voDeinit();
    g_system = NULL;
    printf("qua uvc_preview_stop\n");
	return 0;
}

