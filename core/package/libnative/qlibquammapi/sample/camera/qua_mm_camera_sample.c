#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "camera/qua_mm_camera.h"
#include "system/qua_mm_system.h"
#include "sample_common_isp.h"

#define SENSOR_WIDTH 2688
#define SENSOR_HEIGHT 1520
static QUA_BOOL g_stop = 0;
static  qua_mm_vi_device_t *g_vi_dev = NULL;

static void qua_mm_int_sys(const char *platform) {
    QUA_BOOL primary = QUA_FALSE;
    qua_mm_system_t *system;
    qua_mm_system_ops_t *ops;
    QUA_U32 blk_size;
    qua_vb_config_t vb_cfg;
#ifndef __ANDROID__
    primary = QUA_TRUE;
#endif
    qua_mm_init(primary, platform, &system);
    ops = (qua_mm_system_ops_t *)system;

    memset(&vb_cfg, 0, sizeof(qua_vb_config_t));
    blk_size = SENSOR_WIDTH * SENSOR_HEIGHT * 2;
    vb_cfg.max_pool_cnt = QUA_VB_MAX_POOLS;
    vb_cfg.common_pools[0].block_size = blk_size;
    vb_cfg.common_pools[0].block_cnt = 8;

    if (primary) {
        ops->sys_init(&vb_cfg);
    }

}

static int vi_dev_init(void) {
    qua_mm_device_t *mm_device = NULL;
    char platform[QUA_MAX_PLATFORM_STR_LEN];

    sprintf(platform, "%s_%s", QUA_SYS_CHIP_QM108H, qua_sys_os());

    const qua_mm_module_t *cam_module = NULL;
    qua_mm_system_t *system = NULL;
    QUA_S32 ret = qua_mm_init(QUA_FALSE, platform, &system);
    if (ret != QUA_SUCCESS || system == NULL) {
        ALOGD("vi_dev_init module init fail");
        return QUA_EINVAL;
    }

    ret = qua_mm_load_module(QUA_MM_MODULE_CAMERA, &cam_module);
    if (ret != QUA_SUCCESS || cam_module == NULL) {
        ALOGD("vi_dev_init get module fail");
        return QUA_EINVAL;
    }

    ret = cam_module->open_device(cam_module, QUA_MM_CAMERA_DEV_VI, 0, &mm_device);
    if (ret != QUA_SUCCESS || mm_device == NULL) {
        ALOGD("vi_dev_init open device fail");
        return QUA_EINVAL;
    }

    g_vi_dev = (qua_mm_vi_device_t *)mm_device;
    qua_mm_int_sys(platform);
    return QUA_SUCCESS;
}

static int vi_dev_deinit(void) {
    g_vi_dev->parent.close(&g_vi_dev->parent);
    g_vi_dev = NULL;
    return QUA_SUCCESS;
}

static void vicap_deinit(){
     g_vi_dev->exit_dev(0);
}

static void vicap_init(){
	int vi_dev = 0;
    qua_vi_attr_t vi_attr;
    qua_vi_crop_cfg_t crop_cfg;
    qua_vi_dev_attr_t dev_attr;
    qua_vi_peak_load_info_t peakload = {0};

    printf("isp_init_width = %d, isp_init_height = %d\n", SENSOR_WIDTH,  SENSOR_HEIGHT);
    memset(&peakload, 0, sizeof(peakload));
    peakload.size_fps_info[0].width = SENSOR_WIDTH;
    peakload.size_fps_info[0].height = SENSOR_HEIGHT;
    peakload.size_fps_info[0].fps = 30;
    peakload.size_fps_info[0].pclk = 600000000;
    peakload.size_fps_info[1].width = 0;
    peakload.size_fps_info[1].height = 0;
    peakload.size_fps_info[1].fps = 0;
    peakload.size_fps_info[2].width = 0;
    peakload.size_fps_info[2].height = 0;
    peakload.size_fps_info[2].fps = 0;
    peakload.pclk_set = 0;
    g_vi_dev->set_peak_load(&peakload);

    memset(&vi_attr, 0, sizeof(vi_attr));
    vi_attr.using_vb = QUA_FALSE;
    vi_attr.data_type_in = QUA_VI_DATA_IN_RAW12;
    vi_attr.work_mode = QUA_VI_WORK_MODE_ONLINE;//VICAP_WORK_MODE_ONLINE;//VICAP_WORK_MODE_OFFLINE;
    vi_attr.size.width = SENSOR_WIDTH;
    vi_attr.size.height = SENSOR_HEIGHT;
    g_vi_dev->init_dev(vi_dev, &vi_attr);

    memset(&crop_cfg, 0, sizeof(crop_cfg));
    crop_cfg.cut_enable = QUA_FALSE;
    g_vi_dev->set_pipe_crop(vi_dev, QUA_VI_LFRAME, &crop_cfg);

    memset(&dev_attr, 0, sizeof(dev_attr));
    dev_attr.bayer_type = QUA_VI_BAYER_RGGB;
    dev_attr.work_mode = QUA_VI_WORK_MODE_ONLINE;//VICAP_WORK_MODE_ONLINE;//VICAP_WORK_MODE_OFFLINE;
    dev_attr.wdr_mode = 0;
    dev_attr.in_size.width = SENSOR_WIDTH;
    dev_attr.in_size.height = SENSOR_HEIGHT;
    dev_attr.offline_cfg.priority = 0;
    dev_attr.crop_size.cut_enable = QUA_FALSE;
    g_vi_dev->set_attr(vi_dev, &dev_attr);
    printf("vicap_init ok\n");

    return;
}

static void signal_handler(int sig)
{
    (void)sig;
    g_stop = 1;
}


void set_reg()
{

    //reg config
    QUA_U32 u32RegValue;

    FH_SYS_SetReg(0x10000188, 0x09); //6M
    FH_SYS_SetReg(0x2d000094, 0x01); //配置极性


    //i2c0(pinmux select as i2c0)
	FH_SYS_SetReg(0x10200014, 0x00);
	FH_SYS_SetReg(0x10200018, 0x00);

    //sensor0 clk&reset (pinmux select as SENSOR_CLK0, SENSOR_RSTN0
    FH_SYS_SetReg(0x1020000C, 0x00);
    FH_SYS_SetReg(0x10200010, 0x00);

    //i2c1(pinmux select as i2c1)
    FH_SYS_SetReg(0x10200024, 0x00);
	FH_SYS_SetReg(0x10200028, 0x00);

    //sensor 1 clk&reset (pinmux select as SENSOR_CLK1, SENSOR_RSTN1
	FH_SYS_SetReg(0x1020001C, 0x00);
	FH_SYS_SetReg(0x10200020, 0x00);

#if 1
	//i2c3(pinmux select as i2c3)
	FH_SYS_SetReg(0x102001b8, 0x02);
	FH_SYS_SetReg(0x102001bc, 0x02);

    //sensor 2 clk&reset (pinmux select as SENSOR_CLK2, SENSOR_RSTN2
	FH_SYS_SetReg(0x102001C4, 0x07);
	FH_SYS_SetReg(0x102001C8, 0x07);
#endif

	//vpu clock
	fh_clk_enable("vpu");
	//fh_clk_set_rate("vpu", 400000000);

	//isp clock
	fh_clk_enable("isp");
	//fh_clk_set_rate("isp", 333600000);

	//sensor0 clock
	fh_clk_enable("sensor0");
	fh_clk_set_rate("sensor0", 24000000);

	//sensor1 clock
	fh_clk_enable("sensor1");
	fh_clk_set_rate("sensor1", 24000000);

	//sensor2 clock
	fh_clk_enable("sensor2");
	fh_clk_set_rate("sensor2", 24000000);

    //sensor reset
    // 0x2-sensor0, 0x20-sensor1,0x200-sensor2
	FH_SYS_SetReg(0x2c100094, 0x222);

#ifndef ISP460
	fh_clk_enable("isp460");
    //for new ISP
    FH_SYS_GetReg(0x1010033C, &u32RegValue);
    FH_SYS_SetReg(0x1010033C, u32RegValue & (~0x4));

    FH_SYS_GetReg(0x10100014, &u32RegValue);
    FH_SYS_SetReg(0x10100014, u32RegValue | 0x4);

    FH_SYS_GetReg(0x1C0001BC, &u32RegValue);
    FH_SYS_SetReg(0x1C0001BC, (u32RegValue  & (~0x4)) | 0x8);
#endif

}

int main(int argc, char *argv[])
{
    printf("qua mm camera sample start\n");
    set_reg();
    vi_dev_init();
    vicap_init();
    sample_common_isp_start(0);
    sample_common_isp_run();
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGABRT, signal_handler);
    while (!g_stop){
        sleep(1);
    }
    sample_common_isp_stop();
    vicap_deinit();
    vi_dev_deinit();
    printf("qua mm camera sample end\n");
    return 0;
}
