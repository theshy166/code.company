#include "sample_common_isp.h"
#include "isp/isp_common.h"
#include "isp/isp_api.h"
#include "isp/isp_enum.h"
#include "_sensor_.h"
#include "isp_sensor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "pthread.h"
#include "time.h"
#include <signal.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/prctl.h>


#define SENSOR_WIDTH 2688
#define SENSOR_HEIGHT 1520

static struct dev_isp_info g_isp_info;

void LoadDayNightParam(int u32Id, unsigned int isDay)
{
    //int wdr_mode = isp_get_wdr_mode(u32Id);
    //char filename[64] = {'\0'};
    char *fileName = "./hex/ovos02k_mipi_attr.hex";//"./hex/ovos02k_mipi_wdr_attr.hex"
    ISP_PARAM_CONFIG param;

    //GetSensorHexName(isDay,wdr_mode, filename, u32Id);
    FILE *param_file;
    API_ISP_GetBinAddr(0, &param);
    param_file = fopen(fileName,"rb");
    if(param_file == NULL)
    {
        printf("open file failed! %s\n", fileName);
        return ;
    }
    printf("[irc] open file! %s\n", fileName);
    printf("API_ISP_GetBinAddr size = %d\n", param.u32BinSize);

    char *isp_param_buff = malloc(sizeof(unsigned int)*param.u32BinSize);
    fread(isp_param_buff,1,param.u32BinSize,param_file);
    fclose(param_file);
    API_ISP_LoadIspParam(0, isp_param_buff);
    free(isp_param_buff);
    isp_param_buff = NULL;
}

QUA_S32 sample_common_isp_start(QUA_U32 dev_id)
{
    QUA_S32 ret;
    ISP_MEM_INIT stMemInit = {0};
    Sensor_Init_t initConf = {0};
    ISP_VI_ATTR_S sensor_vi_attr = {0};

    printf("dev_id = %d\n",dev_id);
    int csi = 0;

    printf("start API_ISP_MemInit, isp_max_width = %d, isp_max_height = %d\n", SENSOR_WIDTH, SENSOR_HEIGHT);
    stMemInit.enOfflineWorkMode = ISP_OFFLINE_MODE_DISABLE;
    stMemInit.stPicConf.u32Width = SENSOR_WIDTH;
    stMemInit.stPicConf.u32Height = SENSOR_HEIGHT;
    stMemInit.enLut2dWorkMode = ISP_LUT2D_BYPASS;
    if(stMemInit.enLut2dWorkMode == ISP_LUT2D_OFFLINE){
        stMemInit.enIspOutMode = ISP_OUT_TO_DDR;
    }
    else{
        stMemInit.enIspOutMode = ISP_OUT_TO_VPU;
    }

    ret = API_ISP_MemInit(0, &stMemInit);
    if (ret){
        printf("Error(%d - %x): API_ISP_MemInit (grpid):(%d)!\n", ret, ret, dev_id);
        return ret;
    }
	printf("\r\n==========g_isp_info[grpid].sensor_name:%s\r\n", "04c10");
    struct isp_sensor_if *sensor = start_sensor("ovos04c10_mipi", dev_id);
    if (!sensor){
        printf("Error(%d - %x): start_sensor (grpid):(%d)!\n", ret, ret, dev_id);
        return -1;
    }

    ret = API_ISP_SensorRegCb(0, 0, sensor);
    if (ret){
        printf("Error(%d - %x): API_ISP_SensorRegCb (grpid):(%d)!\n", ret, ret, dev_id);
        return ret;
    }

    initConf.u8CsiDeviceId = csi;
	initConf.u8CciDeviceId=3;
#if defined DUAL_CAMERA_DEMO
    initConf.bGrpSync = FH_TRUE; //拼接需要同步
#else
    initConf.bGrpSync = FH_FALSE;
#endif
	printf("start API_ISP_SensorInit. i2c %d-%d\r\n", initConf.u8CsiDeviceId, initConf.u8CciDeviceId);
    ret = API_ISP_SensorInit(0, &initConf);
    if (ret) {
        printf("Error(%d - %x): API_ISP_SensorInit (grpid):(%d)!\n", ret, ret, dev_id);
        return ret;
    }

    // 下载配置到sensor
	printf("start API_ISP_SetSensorFmt: 0x%x\n", FORMAT_2688X1520P60);
#ifdef EMU
    ret = API_ISP_SetSensorFmt(0, FORMAT_2688X1520P60);
    if (ret){
        printf("Error(%d - %x): API_ISP_SetSensorFmt (grpid):(%d)!\n", ret, ret, dev_id);
        return ret;
    }
    printf("start API_ISP_SetSensorFmt done !\n");
    sensor_vi_attr.u16InputHeight = SENSOR_HEIGHT;
    sensor_vi_attr.u16InputWidth = SENSOR_WIDTH;
    sensor_vi_attr.u16PicHeight = SENSOR_HEIGHT;
    sensor_vi_attr.u16PicWidth = SENSOR_WIDTH;
    sensor_vi_attr.u16OffsetX = 0;
    sensor_vi_attr.u16OffsetY = 0;
    sensor_vi_attr.enBayerType = BAYER_GRBG;
    printf("start API_ISP_SetViAttr\r\n");
    ret = API_ISP_SetViAttr(0, &sensor_vi_attr);
    if (ret){
        printf("Error(%d - %x): API_ISP_SetViAttr (grpid):(%d)!\n", ret, ret, dev_id);
        return ret;
    }
#else
    ret = API_ISP_SetSensorFmt(0, FORMAT_2688X1520P60);
    if (ret){
        printf("Error(%d - %x): API_ISP_SetSensorFmt (grpid):(%d)!\n", ret, ret, dev_id);
        return ret;
    }
#endif


    // 启动sensor输出
     printf("start API_ISP_SensorKick\r\n");
    ret = API_ISP_SensorKick(0);
    if (ret){
        printf("Error(%d - %x): API_ISP_SensorKick (grpid):(%d)!\n", ret, ret, dev_id);
        return ret;
    }

    // 初始化ISP硬件寄存器
    printf("start API_ISP_Init\r\n");
    ret = API_ISP_Init(0);
    if (ret){
        printf("Error(%d - %x): API_ISP_Init (grpid):(%d)!\n", ret, ret, dev_id);
        return ret;
    }

    // 3 根据实际使用，配置vicap是离线模式还是在线模式
     printf("start API_ISP_GetViAttr\r\n");
    ret = API_ISP_GetViAttr(0, &sensor_vi_attr);
    if (ret){
        printf("Error(%d - %x): API_ISP_GetViAttr (grpid):(%d)!\n", ret, ret, dev_id);
        return ret;
    }


#ifdef CONFIG_VICAP_OFFLINE_MODE
    src.obj_id = FH_OBJ_VICAP;
    src.dev_id = 0;
    src.chn_id = grpid;
    dst.obj_id = FH_OBJ_ISP;
    dst.dev_id = grpid;
    dst.chn_id = 0;

    ret = FH_SYS_Bind(src, dst);
    if (ret){
        printf("Error(%d - %x): FH_SYS_BindVicap2Isp (grpid):(%d)!\n", ret, ret, grpid);
        return -1;
    }
    printf("VICAP[%d] bind to ISP[%d] success\n", grpid, grpid);
#endif
    LoadDayNightParam(dev_id, 1);


#if defined(FH_APP_USING_IRCUT_G0) || defined(FH_APP_USING_IRCUT_G1) || defined(FH_APP_USING_IRCUT_G2)
    sample_SmartIR_init(g_isp_info[grpid].sensor->name, grpid);
#endif

    memset(&g_isp_info, 0, sizeof(g_isp_info));
    return 0;
}

QUA_VOID *sample_isp_proc(QUA_VOID *arg)
{
    struct dev_isp_info *isp_info = (struct dev_isp_info *)arg;
    char name[20];

    sprintf(name, "demo_isp%d", isp_info->channel);
    prctl(PR_SET_NAME, name);
    isp_info->running = 1;

    while (!isp_info->bStop){
        API_ISP_Run(0);
#if defined(FH_APP_USING_IRCUT_G0) || defined(FH_APP_USING_IRCUT_G1) || defined(FH_APP_USING_IRCUT_G2)
        sample_SmartIR_Ctrl(isp_info->channel);
#endif

#ifdef FH_APP_OPEN_AF
        sample_af_run();
#endif
        usleep(10000);
    }

    isp_info->bStop = 0;
    isp_info->running = 0;
    return NULL;
}

QUA_S32 sample_common_isp_run(QUA_VOID)
{
    FH_UINT32 ret;
    pthread_t isp_thread;
    pthread_attr_t attr;
    struct sched_param param;
    FH_UINT32 grpid = 0;

    g_isp_info.bStop = 0;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 8 * 1024);
#ifdef __RTTHREAD_OS__
            param.sched_priority = 130;
#endif
    pthread_attr_setschedparam(&attr, &param);
    ret = pthread_create(&isp_thread, &attr, sample_isp_proc, &g_isp_info);
    if (ret != 0){
        printf("Error: create ISP%d thread failed!\n", grpid);
        return -1;
    }

    return ret;
}

QUA_S32 sample_common_isp_stop(QUA_VOID)
{
    if (g_isp_info.bStop == 0) {
        g_isp_info.bStop = 1;
        while (g_isp_info.running){
            usleep(20 * 1000);
        }

#if defined(FH_APP_USING_IRCUT_G0) || defined(FH_APP_USING_IRCUT_G1) || defined(FH_APP_USING_IRCUT_G2)
        sample_SmartIR_deinit(grpid);
#endif
        API_ISP_Exit(0);
    }

    return 0;
}


