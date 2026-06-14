#ifdef CONFIG_XOS_FWK_CADP_KVS
#include "cadp_videocall.h"
#include "../aivendor/aws-kvs/include/kvs_api.h"

#define MAX_STRINGS 2
#define MAX_LENGTH 100


static int agora_config(int* argc ,char*** argv,qua_videocall_config_t* cf){
    *argc = MAX_STRINGS;  // 设置数组大小
    *argv = malloc(*argc * sizeof(char *));  // 分配指针数组内存

    for (int i = 0; i < MAX_STRINGS; i++) {
        (*argv)[i] = malloc(MAX_LENGTH * sizeof(char));
        if ((*argv)[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            // 释放已分配的内存
            for (int j = 0; j < i; j++) {
                free((*argv)[j]);
            }
            free((*argv));
            return 1;
        }
    }
    
    strcpy((*argv)[0], "");
    strcpy((*argv)[1], (cf->channel_name[0] != '\0'? cf->channel_name : "dghjsm-rtc-test"));
    return 0;
}

int qm_video_call_init(app_callback_t callback, qua_videocall_t* videocall)
{
    videocall->call = kvs_api_start;
    videocall->end = kvs_api_stop;
    videocall->callback = callback;
    videocall->config = agora_config;
    return 0;
}
#endif //CONFIG_XOS_FWK_CADP_KVS