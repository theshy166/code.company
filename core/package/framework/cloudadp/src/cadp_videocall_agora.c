#ifdef CONFIG_XOS_FWK_CADP_AGORA

#include "cadp_videocall.h"
#include "app_config.h"
#include "xos_res_conf.h"
#include "rtsa_api.h"

#define kAppId          "8559624fc7e44c058792bda0f9bdd9b6"

#define MAX_STRINGS 18
#define MAX_LENGTH 100


static int agora_config(int* argc ,char*** argv,qua_videocall_config_t* cf){

    *argc = MAX_STRINGS + (cf->ischeck ? 1 : 0);  // 设置数组大小
    *argv = malloc(*argc * sizeof(char*));  // 分配指针数组内存

    for (int i = 0; i < *argc; i++) {
        (*argv)[i] = malloc(MAX_LENGTH * sizeof(char));
        if ((*argv)[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            // 释放已分配的内存
            for (int j = 0; j < i; j++) {
                free((*argv)[j]);
            }
            free((*argv));
            return -1;
        }
    }
    LV_LOG_USER("agora_config channel_name = %s",cf->channel_name);
    // 初始化字符串
    strcpy((*argv)[0], "app.exe");
    strcpy((*argv)[1], "-i");
    strcpy((*argv)[2], cf->appid[0] == '\0' ? kAppId : cf->appid);
    strcpy((*argv)[3], "-c");
    strcpy((*argv)[4], cf->channel_name[0] == '\0' ? DEFAULT_CHANNEL_NAME : cf->channel_name);
    strcpy((*argv)[5], "-x");
    strcpy((*argv)[6], cf->resolution);
    strcpy((*argv)[7], "-e");
    strcpy((*argv)[8], cf->video_exist);

    int16_t index = 9;
    // if (cf->ischeck) {
    //     strcpy((*argv)[index++], "-R");
    // }
    // else {
    //     struct stat mmcblk0p1;
    //     char *parent_dir = "/nfs";
    //     if (0 == stat("/dev/mmcblk0p1", &mmcblk0p1)) {
    //         parent_dir = "/sdcard";
    //     }
    //     char video_path[64] = {0};
    //     lv_snprintf(video_path, 64, XOS_RES_VIDEOCALL_H264, parent_dir);
    //     char audio_path[64] = {0};
    //     lv_snprintf(audio_path, 64, XOS_RES_VIDEOCALL_16KPCM, parent_dir);
      
    //     strcpy((*argv)[index++], "-s");
    //     strcpy((*argv)[index++], video_path);
    //     strcpy((*argv)[index++], "-S");
    //     strcpy((*argv)[index++], audio_path);
    // }
    strcpy((*argv)[index++], "-M");
    strcpy((*argv)[index++], cf->audio_volume);

    strcpy((*argv)[index++], "-L");
    strcpy((*argv)[index++], "-1");
    strcpy((*argv)[index++], "-a");
    snprintf((*argv)[index++], MAX_LENGTH, "%d", AUDIO_DATA_TYPE_PCMA); //audio data type
    LV_LOG_USER("agora_config argc = %d", *argc);
    for (int i = 0; i < index; i++) {
        LV_LOG_USER("argv[%d] = %s", i, (*argv)[i]);
    }
    return 0;
}

int qm_video_call_init(app_callback_t callback, qua_videocall_t* videocall)
{
    videocall->call = rtsa_api_go;
    videocall->end = rtsa_api_stop;
    videocall->callback = callback;
    videocall->config = agora_config;
}
#else

#endif //CONFIG_XOS_FWK_CADP_AGORA