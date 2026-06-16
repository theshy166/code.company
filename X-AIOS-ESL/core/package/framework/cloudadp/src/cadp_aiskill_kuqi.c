#if defined (CONFIG_XOS_FWK_CADP_KUQI)  && !defined(BUILD_SIMULATOR)
#include "coapi_pf_ext.h"
#include "coapi_common.h"
#include "coapi_devsta.h"
#include "coapi_funchub.h"
#include "coapi_register.h"
#include "coapi_tts.h"
#include "coapi_logger.h"

#include "coapi_audio.h"
#include "coapi_cmd.h"

#include "coapi_bindcode.h"
#include "coapi_devbind.h"
#include "coapi_devbind_wxid.h"
#include "coapi_logic.h"
#include "coapi_timer.h"

#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <string.h>  
#include <unistd.h>  
#include "cadp_kuqi_ai.h"
#include "lvgl/lvgl.h"
#include "cos_skill_picture_to_poetry_test.h"
#include "cos_skill_picture_to_poetry.h"
#include "cadp_aiskill.h"
#include "cos_app.h"
#if 0
/* 开发公用 */
#define APP_KEY    "01722480001640"
#define SECRET_KEY "068caa9c90fd73fbfcd505e65c8c42f1"
#define DEVICE_ID    "quamingtest"
#define RECORD_PATH  "/data/coapi"
/* 默认是COAPI_SERVER_GRAY即灰度服务器, 切记生产时切换到生产服务器,COAPI_SERVER_SVR */
#define COAPI_SERVER_USE COAPI_SERVER_GRAY
//#define COAPI_SERVER_USE COAPI_SERVER_SVR
#endif
#define COAPI_MORE_LOG_OPEN
coapi_struct g_coapiStruct = {0};

static void sendSignal(void){
    pthread_cond_signal(&g_coapiStruct.cond);
}

static void kuqi_coapi_deinit(void);

/**
 * @breif 与云端的连接回调
 * 连接上云端后所有coapi的功能才能使用
 * 如需判断是否已经连接上云端使用coapi_logic.h的coapi_connect_status()
 * 函数主动查询,比回调实时性更好
 */
void cos_connect_cb(unsigned char status)
{
    int last_status;
    AiMessage aiMessage = {0};
    last_status = g_coapiStruct.coapi_link_status;
    g_coapiStruct.coapi_link_status = status;
    aiMessage.val = g_coapiStruct.coapi_link_status;
    g_coapiStruct.connectCallback(&aiMessage);

    switch (status) {
    case 0:
        xdebug("未连接 ");
        break;
    case 1:
        xdebug("连接中 ");
        break;
    case 2:
        xdebug("已连接 ");
        break;
    case 3:
        xdebug("链接关闭 ");
        break;
    case 4:
        xdebug("网络差，信号弱");
        break;
    case 5:
        xdebug("重新连接");
        break;
    case 6:
        xdebug("网络不好,重发数据");
        /* do some notices */
        g_coapiStruct.coapi_link_status = last_status; /* 恢复到上一次的状态 */
        break;
    case 7:
    default:
        xdebug("未知状态 ");
        break;
    }
}

int cos_wifi_cb(void)
{
    /* 以下为示例代码，用户需要按要求实现 */
    int wifi_status = 1;
    return wifi_status;
}

int cos_link_status(void)
{
    return g_coapiStruct.coapi_link_status;
}

void cos_dev_register_cb(unsigned char result, char *devid, char *uuid, char *wxid,int seqid)
{
    g_coapiStruct.reg = result;
    xinfo("result: %d, seqid: %d", result, seqid);
}

void cos_register_status_reset(void)
{
    g_coapiStruct.reg = -1;
}

int cos_register_status(void)
{
    return g_coapiStruct.reg;
}

/*
 * @brief 用于判断机器是否开机完成，用于启动流程初始化的接口调用判断选择
 * @return 0: 未启动完成
 *         1: 启动完成
 */
int cos_boot_completed(void)
{
    g_coapiStruct.boot_completed = 1;
    return g_coapiStruct.boot_completed;
}

/*
 * 通过定义用户printf可以在需要的时候方便用户将log输出进行重定向
 */
static int my_printf_complete(const char* fmt,
                              const char* time,
                              const char* file,
                              const char* func,
                              unsigned long line,
                              const char* log)
{
    /* 注意此处不能使用coapi logger ,防止死递 */
    return printf(fmt, time, file, func, line, log);
}

static int my_printf_brief(const char* fmt, const char* log)
{
    /* 注意此处不能使用coapi logger ,防止死递 */
    return printf(fmt, log);
}


threads_stack_size_t thd_stack_set[8];
/**
 * 传入设备唯一id,如: MAC/SN/IMEI/..., 可选择云端分配的方式传入NULL, 参考coapi_register.h
 * ssid: wifi名称用于公众号显示
 * nodevid 必须，0： 设备端注册时有devid，无需云端分配id; 1: 设备端没有唯一ID,云端分配ID
 */
void cos_init(char *device_id, char *ssid, int nodevid)
{
    int res = 0, ret = 0;

    /* 根据产品实际情况修改 */
    dev_comm dev = {
        .devtype = "机器人",
        .ver = "00.01", /* 需要ota时需配置此版本 */
        .cpu = "x86",
        .ops = "linux",
        .name = "X小酷",
        .vendor = "XVendor",
    };

    co_set_base_task_prio(5);
    thd_stack_set[0].prio = THD_LLT_PRIO;
    thd_stack_set[0].size = THD_LLT_STACK_SIZE;
    thd_stack_set[0].id = THD_LLT_ID;
    thd_stack_set[1].prio = THD_RECV_PRIO;
    thd_stack_set[1].size = THD_RECV_STACK_SIZE;
    thd_stack_set[1].id = THD_RECV_ID;
    thd_stack_set[2].prio = THD_NETCORE_PRIO;
    thd_stack_set[2].size = THD_NETCORE_STACK_SIZE;
    thd_stack_set[2].id = THD_NETCORE_ID;
    thd_stack_set[3].prio = THD_STM_PRIO;
    thd_stack_set[3].size = THD_STM_STACK_SIZE;
    thd_stack_set[3].id = THD_STM_ID;
    thd_stack_set[4].prio = THD_IMG_PRIO;
    thd_stack_set[4].size = THD_IMG_STACK_SIZE;
    thd_stack_set[4].id = THD_IMG_ID;
    thd_stack_set[5].prio = THD_AW_PRIO;
    thd_stack_set[5].size = THD_AW_STACK_SIZE;
    thd_stack_set[5].id = THD_AW_ID;
    thd_stack_set[6].prio = THD_ASYNC_PRIO;
    thd_stack_set[6].size = THD_ASYNC_STACK_SIZE;
    thd_stack_set[6].id = THD_ASYNC_ID;
    thd_stack_set[7].prio = THD_OTA_PRIO;
    thd_stack_set[7].size = THD_OTA_STACK_SIZE;
    thd_stack_set[7].id = THD_OTA_ID;
    res = co_threads_stack_size_set(thd_stack_set,(unsigned char)8);
    co_strcpy((char *)dev.devid, (const char *)device_id);
    coapi_dev_init(&dev);
    coapi_logic_init(cos_connect_cb, cos_wifi_cb, g_coapiStruct.server_type);
    #ifdef COAPI_MORE_LOG_OPEN
    coapi_logger_debug(1024 * 10);
    #endif
    printf_itf_t my_printf_itf = {
          .printf_complete = my_printf_complete,
          .printf_brief = my_printf_brief
    };
    coapi_logger_printf_set(&my_printf_itf);
    cos_comm_init();
    ret = coapi_register_start(cos_dev_register_cb, g_coapiStruct.appkey, g_coapiStruct.secretkey, ssid, NULL, nodevid);

    xdebug("register ret = %d ",ret);

    return;
}


static void* coapi_thread_entry(void *context_ptr)
{
    int ret =0;
	cos_init(g_coapiStruct.deviceid,NULL,0);

	while (cos_register_status() != AUTH_STATE_AUTH_OK
            && cos_register_status() != AUTH_STATE_HAS_AUTH) {
        co_sleep(1);
    }
    cos_skill_cloud_status_init();
    cos_dev_status_could_update_can_be();

reinit:
    ret = cos_skill_picture_to_poetry_init();
    if(ret != 0) {
        xerror("cos_skill_picture_to_poetry_init err");
        //co_msleep(1000);
        goto reinit;
    }


	while (!g_coapiStruct.toStop) { // 常驻循环  
	
        pthread_mutex_lock(&g_coapiStruct.mutex);  
        while (!g_coapiStruct.image_ready) {  
            pthread_cond_wait(&g_coapiStruct.cond,&g_coapiStruct.mutex);  
        }  
        g_coapiStruct.image_ready = 0;  
		//cos_skill_picture_to_poetry_test( &image_path );
		cos_skill_picture_to_poetry_test( g_coapiStruct.image_path );
        pthread_mutex_unlock(&g_coapiStruct.mutex);  
        sleep(1);
    }  
    cos_skill_picture_to_poetry_deinit();
	kuqi_coapi_deinit();
    return NULL;
}

void kuqi_coapi_register_respose(respose callback){
	g_coapiStruct.respose = callback;
}


void kuqi_coapi_register_connect_callback(connectCallback callback){	
	g_coapiStruct.connectCallback = callback;
}

void kuqi_upload_coapi_iamge(AiMessage* message){
    memset(g_coapiStruct.image_path, 0x0, sizeof(g_coapiStruct.image_path));
    strcpy (g_coapiStruct.image_path, message->data1);
}

void kuqi_coapi_do_right_now(AiMessage* message){
	g_coapiStruct.image_ready = 1;
	sendSignal();
}

void* kuqi_coapi_invoke(void *arg)
{
	coapi_thread_entry( arg );
}

void kuqi_coapi_init( AiDevInfo* devinfo ){

    g_coapiStruct.server_type = devinfo->server_type;
    strcpy(g_coapiStruct.secretkey, devinfo->securet_key);
	strcpy(g_coapiStruct.appkey, devinfo->public_key);	
	strcpy(g_coapiStruct.deviceid, devinfo->clientid);
    strcpy(g_coapiStruct.recordpath, "/data/coapi");
    g_coapiStruct.image_ready = 0;

	pthread_t tid2;
    pthread_create(&tid2, NULL, kuqi_coapi_invoke, NULL);
}

void kuqi_coapi_deinit(void)
{
    g_coapiStruct.toStop = 1;
    sendSignal();
    cos_comm_deinit();
    coapi_logic_deinit();
    coapi_dev_deinit();
}


AiModule g_CoapiMoudule = {
    .init = kuqi_coapi_init,
    .deinit = kuqi_coapi_deinit,
    .register_respose = kuqi_coapi_register_respose,
    .register_conn_respose = kuqi_coapi_register_connect_callback,
    .send_cmd = kuqi_coapi_do_right_now,
    .input_content = kuqi_upload_coapi_iamge
};


#endif /*CONFIG_XOS_FWK_CADP_KUQI*/


