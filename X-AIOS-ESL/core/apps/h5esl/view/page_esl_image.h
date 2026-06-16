#ifndef __PAGE_ESL_IMAGE_H__
#define __PAGE_ESL_IMAGE_H__
#include "../utils/hashmap.h"
#include "../model/esl_show_model.h"
#include "../model/esl_product_info.h"
#include "../model/esl_device_setting.h"
#include "lvgl/lvgl.h"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "pipeline/video_decoder/qua_vdec_pipeline.h"
#include "player/qua_mm_player_common.h"
#endif

typedef struct {
    lv_img_dsc_t img_dsc;   // 指向 lv_img_dsc_t 的指针
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    qua_vdec_pipeline_frame_info_t *s_frame;
    #endif
} image_ll_node_t;

typedef struct {
    int index;
    char * file_path;
} thread_data_t;

typedef struct{
    int index;
}compare_data_t;

void handle_image_extension(lv_obj_t *bg, esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,int display_id,char **other_3_arr,int show_count);
void handle_video_extension(lv_obj_t *bg, esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,int display_id,char **other_3_arr,int show_count);

void los_videoplayer_quit(int display_id);

void _set_image_src(lv_obj_t *img, const char *src,int disp_width,int disp_height,int display_id);

void lv_ll_init();

void lv_ll_free();

void handle_image_extension_download(esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,int display_id,char **other_3_arr,int show_count);

void *compare_local_resouce(void *arg);

int logo_image_is_exist(const char *filePath);

int download_logo_image(const char *url, const char *filePath);

void set_rotation_1(int rotation);
void set_rotation_2(int rotation);

#endif // __PAGE_ESL_IMAGE_H__