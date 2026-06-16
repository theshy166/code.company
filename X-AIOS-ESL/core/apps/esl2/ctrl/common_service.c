#ifdef CONFIG_XOS_APP_ESL2
#include "common_service.h"
#include "utils.h"
#include "../conf/conf.h"

void common_service_init(void){
    //创建 图片，视频等资源目录
    char* download_img_path = transform_path(k_path_img_prefix);
    create_directory(download_img_path);
    free(download_img_path);
    
    //创建json目录
    char* download_json_path = transform_path(k_path_json_prefix);
    create_directory(download_json_path);
    free(download_json_path);
    
    //创建开机logo目录
    char* download_logo_path = transform_path(k_path_logo_prefix);
    create_directory(download_logo_path);
    free(download_logo_path);
}
#endif

