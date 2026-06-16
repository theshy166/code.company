#ifdef CONFIG_XOS_USE_APP_NATIVE_ESL
#include "page_esl_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include "losplayer.h"
#include "../conf/conf.h"
#include "../utils/pic_utils.h"
#include "../utils/utils.h"
#include "../ctrl/esl_service.h"
#include "../utils/esl_http_utils.h"
#include "page_esl.h"
#include "cJSON.h"
#include "lvgl/lvgl.h"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "pipeline/video_decoder/qua_vdec_pipeline.h"
#include "player/qua_mm_player_common.h"
#include "player/qua_mm_player_common.h"
#include "utils/qua_video_parser.h"
#include "zxingWrapper.h"
#endif

#define MAX_FILES 20
#define MAX_FILENAME_LENGTH 64

static bool s_player_1_first_delay = true;
static bool s_player_2_first_delay = true;

static void * s_player_1 = NULL;
static void * s_player_2 = NULL;

static int rotation_1 = 0;
static int rotation_2 = 0;

static char download_files_1[MAX_FILES][MAX_FILENAME_LENGTH];
static int download_files_count_1 = 0;

static char download_files_2[MAX_FILES][MAX_FILENAME_LENGTH];
static int download_files_count_2 = 0;

static int s_player_1_count = 0;
static int s_player_2_count = 0;
static int s_player_1_index = 1;
static int s_player_2_index = 1;
static int s_player_1_count_base = 0;
static int s_player_2_count_base = 0;
static char ** s_player_1_video_list = NULL;
static char ** s_player_2_video_list = NULL;

pthread_mutex_t s_player_mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t s_player_mutex_2 = PTHREAD_MUTEX_INITIALIZER;

extern void qme_set_render_status(int index, bool status);

static int _file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fclose(file);
        return file_size > 0 ? 1 : 0;
    } else {
        return 0;
    }
}

static void _add_file_to_download_list(const char *filename,int display_id) {
    #if 1
        if(display_id == 0){
            if (download_files_count_1 < MAX_FILES) {
                strcpy(download_files_1[download_files_count_1], filename);
                download_files_count_1++;
                LV_LOG_USER("add file to download list 1: %s,index : %d",filename,download_files_count_1);
            }
        }else{
            if (download_files_count_2 < MAX_FILES) {
                strcpy(download_files_2[download_files_count_2], filename);
                download_files_count_2++;
                LV_LOG_USER("add file to download list 2: %s,index : %d",filename,download_files_count_1);
            }
        }
    #endif
}

static int _download_image_if_not_exist(const char *url, const char *filePath,int index,int type) {
    if(_file_exists(filePath)){
        LV_LOG_USER("file is exits! %s",filePath);
        return 1;
    }

    // 调用 lv_async_call
    const char* last_slash = strrchr(filePath, '/');
    if (last_slash == NULL) {
        last_slash = filePath; // 如果没有找到 '/', 返回整个路径
    } else {
        last_slash = last_slash + 1; // 跳过 '/' 返回文件名
    }

    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    if(index == 1){
        extern void fbdev_set_statusbar(int x, int y, int width, int height);
        fbdev_set_statusbar( 0, 0, LV_USE_HOR_SIZE, 30);
    }else if(index == 2){
        extern void fbdev2_set_statusbar(int x, int y, int width, int height);
        fbdev2_set_statusbar( 0, 0, LV_USE_HOR_SIZE, 30);
    }
    #endif

    printf("download_image_thread >>: %s\n", filePath);
    if (index == 1) {
        show_download_log_1(last_slash);
    } else {
        show_download_log_2(last_slash);
    }
    printf("download_image_thread slash: %s\n", last_slash);

    int result = 0;
    int loop_count = 0;
    while(result == 0 && loop_count < 3){
        LV_LOG_USER("loop process download_image_segment start  loop_count: %d========================\n", loop_count);
        result = download_image_segment(url, filePath,512 * 1024,type,index);
        loop_count ++;
        LV_LOG_USER("loop process download_image_segment end  loop_count: %d,result = %d ========================\n", loop_count,result);
    }

    return result;
}

int logo_image_is_exist(const char *filePath){
    return _file_exists(filePath);
}

int download_logo_image(const char *url, const char *filePath){
    int r = download_image(url, filePath);
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    extern void qm_rewrite_jpg_to_420_if_needed(const char *file_path);
    qm_rewrite_jpg_to_420_if_needed(filePath);
    #endif
    return r;
}

static void _generate_filename(const char *src, int display_id, char *output_file_path,int is_add) {
    // 找到最后一个 '/' 的位置
    const char *last_slash = strrchr(src, '/'); // last_slash: /uploads/1794/20230216/0ad93b49aa41c46661425fbb1a2f9aa4.jpg
    if (last_slash == NULL) {
        // 如果没有找到 '/', 则整个 src 作为文件名
        last_slash = src; // last_slash: http://xunmaotemp.oss-cn-hangzhou.aliyuncs.com/uploads/1794/20230216/0ad93b49aa41c46661425fbb1a2f9aa4.jpg
    } else {
        // 跳过 '/'
        last_slash++; // last_slash: 0ad93b49aa41c46661425fbb1a2f9aa4.jpg
    }

    // 找到最后一个 '.' 的位置
    const char *last_dot = strrchr(last_slash, '.'); // last_dot: .jpg
    if (last_dot == NULL) {
        // 如果没有找到 '.', 则假设没有扩展名
        last_dot = last_slash + strlen(last_slash); // last_dot: (end of string)
    }

    char output_filename[256];

    // 复制文件名部分
    size_t name_length = last_dot - last_slash; // name_length: 32
    strncpy(output_filename, last_slash, name_length); // output_filename: 0ad93b49aa41c46661425fbb1a2f9aa4
    output_filename[name_length] = '\0'; // output_filename: 0ad93b49aa41c46661425fbb1a2f9aa4
    strcpy(output_file_path, output_filename);//output_file_path: 0ad93b49aa41c46661425fbb1a2f9aa4

    // 添加 display_id 和扩展名
    sprintf(output_file_path + name_length, "_%d%s", display_id, last_dot); // output_filename: 0ad93b49aa41c46661425fbb1a2f9aa4_1.jpg

    // 添加前缀
    char temp_filename[256];
    strcpy(temp_filename, output_file_path); // temp_filename: 0ad93b49aa41c46661425fbb1a2f9aa4_1.jpg
    sprintf(output_file_path, "%s%s", k_path_img_prefix, temp_filename); // output_filename: /data/res/img/0ad93b49aa41c46661425fbb1a2f9aa4_1.jpg
    if(is_add == 1){
        char * file_name = temp_filename;
        _add_file_to_download_list(file_name,display_id);
    }
    
}

static void _set_image_properties(lv_obj_t *img, esl_show_model_child_t *child) {
    lv_obj_set_size(img, child->w, child->h);
    lv_obj_set_pos(img, child->x, child->y);
    if (child->desc.borderRadius > 0) {
        lv_obj_set_style_radius(img, LV_RADIUS_CIRCLE, 0);
    }
    if (child->angle != 0){
        lv_image_set_rotation(img, child->angle *10);
    }
}

void _set_image_src(lv_obj_t *img, const char *src,int disp_width,int disp_height,int display_id) {
    char full_path[256];
    // char file_bits_name[256] = {0};
    // char full_bits_path[256] = {0};
    unsigned int width = 0;
    unsigned int height = 0;
    char file_path[254];

    if (strncmp(src, "http://", 7) == 0 || strncmp(src, "https://", 8) == 0) {
        _generate_filename(src, display_id, file_path,1);
        int local_index = display_id + 1;
        FILE *file = fopen(file_path, "rb");
        if (!file) {
            perror("Failed to open downloaded image");
            return;
        }
        unsigned char header[8];
        fread(header, 1, 8, file);
        fclose(file);
        int actual_format = 0; // 1 for PNG, 2 for JPG
        if (is_png(header)) {
            actual_format = 1;
        } else if (is_jpg(header)) {
            actual_format = 2;
        }

        // Determine the expected format based on file extension
        const char *ext = strrchr(file_path, '.');
        int expected_format = 0; // 1 for PNG, 2 for JPG
        if (ext && (strcmp(ext, ".png") == 0 || strcmp(ext, ".PNG") == 0)) {
            expected_format = 1;
        } else if (ext && (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".JPG") == 0 || strcmp(ext, ".JPEG") == 0)) {
            expected_format = 2;
        }
        
        if (expected_format == 1 && actual_format == 2) {
            char jpg_path[254];
            strncpy(jpg_path, file_path, ext - file_path);
            jpg_path[ext - file_path] = '\0';
            strcat(jpg_path, ".jpg");
            strncpy(jpg_path, file_path, ext - file_path);
            snprintf(full_path, sizeof(full_path), "H:%s", jpg_path);
        }else if(expected_format == 2 && actual_format == 1){
            char png_path[254];
            strncpy(png_path, file_path, ext - file_path);
            png_path[ext - file_path] = '\0';
            strcat(png_path, ".png");
            strncpy(png_path, file_path, ext - file_path);
            snprintf(full_path, sizeof(full_path), "H:%s", png_path);
        }else{
            snprintf(full_path, sizeof(full_path), "H:%s", file_path);
        }
    }else{
        snprintf(full_path, sizeof(full_path), "%s", src);
    }

    printf("---[app]full_path:%s\n",full_path);
    lv_img_dsc_t *img_dsc = NULL;

    memset(file_path, 0, 256);
    strcpy(file_path, full_path + 2);

    extern void qme_img_fetch_by_path(const char *, int , int *, int *, lv_img_dsc_t **);
    qme_img_fetch_by_path(file_path, display_id, &width, &height, &img_dsc);
    
    printf("---[app]img_dsc %p\n", img_dsc);
    if (img_dsc == NULL) {
        unsigned char* argb_buffer = NULL;
        unsigned int mmz_phy_buf = 0;
        bool use_mmz = false;
        bool has_encape = false;
        lv_color_format_t cf;
        if (strstr(full_path, ".png") || strstr(full_path, ".PNG")) {
            //
            extern int qm_pngdecoder(char *, int , int , bool , unsigned int *, unsigned char **, int *, int *);
            extern int qm_png2param_create(const char *, uint8_t **, int *, int *, lv_color_format_t *cf, bool , unsigned int *, bool);
            if (disp_width == 331 && disp_height == 331)
            {
                if (strstr(file_path, "216c0779ea7118825ca42478ff3a018b")) {
                    LV_LOG_USER("---[app]qm_png2param_create ENTRY!!----display_id:%d----\n", display_id);
                    if (display_id == 0) {
                        qm_png2param_create(k_path_template_midautumn_0, &argb_buffer, &width, &height, &cf, use_mmz, &mmz_phy_buf, false);
                    }
                    else {
                        qm_png2param_create(k_path_template_midautumn_1, &argb_buffer, &width, &height, &cf, use_mmz, &mmz_phy_buf, false);
                    }
                }
                else {
                    LV_LOG_USER("---[app]qm_pngdecoder [disp_width: %d, disp_height: %d], [width: %d, height: %d]\n", disp_width, disp_height, width, height);
                    int result = qm_png2param_create(file_path, &argb_buffer, &width, &height, &cf, use_mmz, &mmz_phy_buf, false);
                    if(result != 0){
                        return;
                    }
                }
            }
            else {
                int result = qm_png2param_create(file_path, &argb_buffer, &width, &height, &cf, use_mmz, &mmz_phy_buf, false);
                if(result != 0){
                    return;
                }
                printf("---[app]qm_png2param_create [width: %d, height: %d]\n", disp_width, disp_height);
            }
            if (argb_buffer == NULL) {
                LV_LOG_ERROR("----------[esl_image]decode png failed");
                return;
            }
        }else {
            LV_LOG_USER("[esl_image]open file from jpeg: %s", full_path);
            
            #if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
            // 解码 JPEG 图片
            // extern unsigned char* decode_jpeg_with_turbo(const char* , int* , int* );
            extern unsigned char* decode_jpeg_with_turbo(const char* , int *, int *, bool , unsigned int *);
            argb_buffer = decode_jpeg_with_turbo(full_path, &width, &height, use_mmz, &mmz_phy_buf);
            if (!argb_buffer) {
                LV_LOG_ERROR("[esl_image]decode jpeg failed");
                lv_img_set_src(img, full_path);
                return;
            }
            printf("decode jpg success: width=%d,height=%d\n", width, height);
            #else
            has_encape = true;
            extern bool qme_img_get_jpeg(const char * image_path, int disp_width, int disp_height, int display_id, lv_img_dsc_t **out_img_dsc);
            qme_img_get_jpeg(full_path, disp_width, disp_height, display_id, &img_dsc);
            #endif
        }

        if (!has_encape) {
            extern void qme_img_encape_bits_to_imgdsc(char *, unsigned char *, unsigned int, int, int , int , lv_img_dsc_t **);
            // float x_ratio = (float)width / disp_width;
            // float y_ratio = (float)height / disp_height;
            // if (argb_buffer != NULL && (x_ratio > 1.1 || y_ratio > 1.1)) {
            //     LV_LOG_USER("----set scale factor:width=%d,height=%d,disp_width=%d,disp_height=%d\n", width, height, disp_width, disp_height);
            //     extern unsigned char* qm_argb_scale_image(unsigned char* , int , int , int , int , bool , unsigned int *);
            //     unsigned char* new_argb_buffer = qm_argb_scale_image(argb_buffer, width, height, disp_width, disp_height, use_mmz, &mmz_phy_buf);
            //     if (new_argb_buffer != NULL) {
            //         if (use_mmz) {
            //             qme_memory_free(mmz_phy_buf, argb_buffer);
            //         }
            //         else {
            //             free(argb_buffer);
            //         }
            //         argb_buffer = new_argb_buffer;
            //     }

                // qme_img_encape_bits_to_imgdsc(file_path, argb_buffer, mmz_phy_buf, disp_width, disp_height, display_id, &img_dsc);
            // }
            // else
            {
                qme_img_encape_bits_to_imgdsc(file_path, argb_buffer, mmz_phy_buf, width, height, display_id, &img_dsc);
            }
    /* if (strstr(full_path, ".png") || strstr(full_path, ".PNG")) {
        memset(file_path, 0, 256);
        strcpy(file_path, full_path + 2);
        GetPicWidthHeight(file_path, &width, &height);
        // Calculate and apply zoom factor
        if (width > 0 && height > 0) {
            int zoom_factor_x = (disp_width * 256) / width; // 256 is the default zoom level (100%)
            int zoom_factor_y = (disp_height * 256) / height; // 256 is the default zoom level (100%)
            lv_image_set_scale_x(img, zoom_factor_x);
            lv_image_set_scale_y(img, zoom_factor_y); */
        }
    }
    
    if (img_dsc != NULL) {
        // if (width > 0 && height > 0) {
        //     LV_LOG_USER("----set scale factor\n");
        //     int zoom_factor_x = (disp_width * 256) / width; // 256 is the default zoom level (100%)
        //     int zoom_factor_y = (disp_height * 256) / height; // 256 is the default zoom level (100%)
        //     lv_image_set_scale_x(img, zoom_factor_x);
        //     lv_image_set_scale_y(img, zoom_factor_y);
        // }
        printf("show image: img_dsc %p, %d\n", img_dsc, img_dsc->data_size);
        lv_image_set_scale(img, 256);
        lv_img_set_src(img, img_dsc);
    }
    else {
        LV_LOG_ERROR("----------[esl_image]decode image failed. rollback to path:%s", full_path);
        // LV_ASSERT_MSG(false, "----------[esl_image]decode image failed. rollback to path");
        lv_img_set_src(img, full_path);
    }
}

void los_videoplayer_quit(int display_id) {
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        if(display_id == 0){
            pthread_mutex_lock(&s_player_mutex_1);
            LV_LOG_USER("exit player: remove player.1 :%p",s_player_1);
            if (s_player_1 != NULL) {
                LV_LOG_USER("exit player: remove player.1A");
                los_player_stop(s_player_1);
                los_player_destroy2(s_player_1, display_id);
                s_player_1 = NULL;
                LV_LOG_USER("exit player: remove player.1B");
            }
            if(s_player_1_video_list != NULL && s_player_1_count > 0){
                for(int i = 0; i < s_player_1_count; i++){
                    LV_LOG_USER("==================================free s_player_1_video_list[%d]:%p",i,s_player_1_video_list[i]);
                    if(s_player_1_video_list[i] != NULL){
                        free(s_player_1_video_list[i]);
                        s_player_1_video_list[i] = NULL;
                    }
                }
                free(s_player_1_video_list);
                s_player_1_video_list = NULL;
                s_player_1_count = 0;
                s_player_1_index = 1;
            }
            pthread_mutex_unlock(&s_player_mutex_1);
        }else{
            pthread_mutex_lock(&s_player_mutex_2);
            LV_LOG_USER("exit player: remove player.2 :%p",s_player_2);
            if (s_player_2 != NULL) {
                LV_LOG_USER("exit player: remove player.2A");
                los_player_stop(s_player_2);
                los_player_destroy2(s_player_2,display_id);
                s_player_2 = NULL;
                LV_LOG_USER("exit player: remove player.2B");
            }
            if(s_player_2_video_list != NULL && s_player_2_count > 0){
                for(int i = 0; i < s_player_2_count; i++){
                    if(s_player_2_video_list[i] != NULL){
                        free(s_player_2_video_list[i]);
                        s_player_2_video_list[i] = NULL;
                    }
                }
                free(s_player_2_video_list);
                s_player_2_video_list = NULL;
                s_player_2_count = 0;
                s_player_2_index = 1;
            }
            pthread_mutex_unlock(&s_player_mutex_2);
        }
    #endif
}

//先开个异步解决不能在progress_cb中调用los_player_stop
static void* _player_replay_thread(void* arg) {
    void *current_player = (void *)arg;
    if (current_player == NULL) {
        LV_LOG_WARN("progress_cb: current_player is NULL");
        pthread_detach(pthread_self()); // 新增：分离线程
        return NULL;
    }
    if (s_player_1 != NULL &&current_player == s_player_1) {
        pthread_mutex_lock(&s_player_mutex_1);
        if (s_player_1 != NULL) {
            LV_LOG_WARN("replay 1");
            #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
                if(s_player_1_count < 1){
                    pthread_mutex_unlock(&s_player_mutex_1);
                    return NULL;
                }
                if(s_player_1_count == 1){
                    los_player_stop(s_player_1);
                    los_player_seek(s_player_1, 0);
                    los_player_play(s_player_1);
                }else {
                    int count = 0;
                    los_player_stop(s_player_1);
                    while(count < s_player_1_count){
                        int index = s_player_1_index%s_player_1_count;
                        char *path = s_player_1_video_list[index];
                        int ret = xos_player_reset(s_player_1);
                        if(ret != 0){
                            LV_LOG_ERROR("replay 1: xos_player_reset failed");
                            s_player_1_index ++;
                            count++;
                            usleep(50 * 1000);
                            continue;
                        }
                        ret = xos_player_set_data_source(s_player_1, path);
                        if(ret != 0){
                            LV_LOG_ERROR("replay 1: xos_player_set_data_source failed");
                            s_player_1_index ++;
                            count++;
                            usleep(50 * 1000);
                            continue;
                        }
                        ret = xos_player_prepare(s_player_1);
                        if(ret != 0){
                            LV_LOG_ERROR("replay 1: xos_player_prepare failed");
                            s_player_1_index ++;
                            count++;
                            usleep(50 * 1000);
                            continue;
                        }
                        s_player_1_index = index + 1;
                        break;
                    }
                    if(count >= s_player_1_count){
                        //所有视频都播放失败
                        los_player_stop(s_player_1);
                        los_player_destroy2(s_player_1, 0);
                        s_player_1 = NULL;
                        //s_player_1 = los_player_create2(parent, s_play_path, area, true,false,display_id);
                    }else{
                        los_player_play(s_player_1);
                    }
                }
            #endif
        }
        pthread_mutex_unlock(&s_player_mutex_1);
    }
    else if (s_player_2 != NULL && current_player == s_player_2) {
        pthread_mutex_lock(&s_player_mutex_2);
        if (s_player_2 != NULL) {
            LV_LOG_WARN("replay 2");
            #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
                if(s_player_2_count < 1){
                    pthread_mutex_unlock(&s_player_mutex_2);
                    return NULL;
                }
                if(s_player_2_count == 1){
                    LV_LOG_USER("replay 2: s_player_2_count == 1");
                    los_player_stop(s_player_2);
                    los_player_seek(s_player_2, 0);
                    los_player_play(s_player_2);
                }else {
                    LV_LOG_USER("replay 2: s_player_2_count == %d",s_player_2_count);
                    LV_LOG_USER("replay 2: s_player_2_index == %d",s_player_2_index);
                    int count = 0;
                    los_player_stop(s_player_2);
                    while(count < s_player_2_count){
                        int index = s_player_2_index % s_player_2_count;
                        LV_LOG_USER("replay 2: index == %d",index);
                        char *path = s_player_2_video_list[index];
                        LV_LOG_USER("replay 2: path == %s",path);
                        int ret = xos_player_reset(s_player_2);
                        if(ret != 0){
                            LV_LOG_ERROR("replay 2: xos_player_reset failed");
                            s_player_2_index ++;
                            count++;
                            usleep(50 * 1000);
                            continue;
                        }
                        ret = xos_player_set_data_source(s_player_2, path);
                        if(ret != 0){
                            LV_LOG_ERROR("replay 2: xos_player_set_data_source failed");
                            s_player_2_index ++;
                            count++;
                            usleep(50 * 1000);
                            continue;
                        }
                        ret = xos_player_prepare(s_player_2);
                        if(ret != 0){
                            LV_LOG_ERROR("replay 2: xos_player_prepare failed");
                            s_player_2_index ++;
                            count++;
                            usleep(50 * 1000);
                            continue;
                        }
                        s_player_2_index = index + 1;
                        break;
                    }
                    if(count >= s_player_2_count){
                        //所有视频都播放失败
                        los_player_stop(s_player_2);
                        los_player_destroy2(s_player_2, 1);
                        s_player_2 = NULL;
                        //s_player_2 = los_player_create2(parent, s_play_path, area, true,false,display_id);
                    }else{
                        los_player_play(s_player_2);
                    }
                }
            #endif
        }
        pthread_mutex_unlock(&s_player_mutex_2);
    }

    pthread_detach(pthread_self()); // 新增：分离线程
    return NULL;
}

static void progress_cb(void * user,int purpose, int64_t ct)
{
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    if (purpose == PLAYER_EVT_PLAYBACK_COMPLETE) {
        pthread_t thread_id;

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // 设置分离属性

        pthread_create(&thread_id, &attr, _player_replay_thread, user);
        pthread_attr_destroy(&attr);
    }
#endif
}

static void _set_player_src(lv_obj_t *parent, const char *full_path,float x,float y,float w,float h,int display_id) {
    /* char full_path[256];

    char new_url[254];  // 假设 URL 长度不超过 256，可根据实际情况调整数组大小
    // 查找 '?' 的位置
    char* pos = strchr(url, '?');
    if (pos == NULL) {  // 如果没有 '?'，原 URL 即为新 URL
        strcpy(new_url, url);
    } else {
        // 复制 '?' 之前的部分到 new_url
        strncpy(new_url, url, pos - url); 
        new_url[pos - url] = '\0';  // 添加字符串结束符
    }

    if (strncmp(new_url, "http://", 7) == 0 || strncmp(new_url, "https://", 8) == 0) {
        char file_path[254];
        _generate_filename(new_url, display_id, file_path,0);
        int local_index = display_id + 1;
        snprintf(full_path, sizeof(full_path), "H:%s", file_path);
    } else {
        snprintf(full_path, sizeof(full_path), "H:%s", new_url);
    } */
    char s_play_path[256] = {0};
    memset(s_play_path, 0, 256);
    #if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR
        strcpy(s_play_path, full_path + 2);
    #else
        strcpy(s_play_path, full_path);
    #endif
    lv_area_t area = {x, y, w, h};
    
    #if 0
        if(display_id == 0){
            if (s_player_1_first_delay) {
                s_player_1_first_delay = false;
                xos_player_video_delay(parent, s_play_path, area, true,false,display_id, &s_player_1);
            }
            else {
                s_player_1 = los_player_create2(parent, s_play_path, area, true,false,display_id);
            }
        }else{
            if (s_player_2_first_delay) {
                s_player_2_first_delay = false;
                xos_player_video_delay(parent, s_play_path, area, true,false,display_id, &s_player_2);
            }
            else {
                s_player_2 = los_player_create2(parent, s_play_path, area, true,false,display_id);
            }
        }
    #endif
    //display_id 从0开始
    if(display_id == 0){
        s_player_1 = los_player_create2(parent, s_play_path, area, true,false,display_id,rotation_1);
    }else{
        s_player_2 = los_player_create2(parent, s_play_path, area, true,false,display_id,rotation_2);
    }

    los_set_progress_callback(progress_cb);
}

static void _handle_image_extension(lv_obj_t *bg, esl_show_model_child_t *child, esl_product_info_t **product_info_list, struct hashmap *map,int count,int display_id,char **other_3_arr,int show_count) {
    struct esl_product_item_key_t *user;
    int index = 0;

    if (!is_string_empty(child->extension.bind)) {
        user = hashmap_get(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind});
        if(user){
            if(show_count - 1 > user->count){
                index = user->count + 1;
            }else{
                index = user->count;
            }
        }else{
            index = 0;
        }
        hashmap_set(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind, .count = index});
        char *item_no = other_3_arr[index];
        esl_product_info_t *product_info = find_product_info_by_item_no(item_no, product_info_list, count);
        lv_obj_t *img = lv_img_create(bg);

        if (strcmp(child->extension.bind, "$item_image") == 0) {
            _set_image_src(img, product_info->item_url,child->w, child->h,display_id);
        } else if (strcmp(child->extension.bind, "$ad_image") == 0) {
            _set_image_src(img, child->desc.src,child->w, child->h,display_id);
        } else if (strcmp(child->extension.bind, "$swiper") == 0) {
            int swiper_list_count = child->extension.swiper_list_count;
            char **swiper_list = child->extension.swiper_list;
            if (swiper_list_count > 0 && swiper_list) {
                for (int i = 0; i < swiper_list_count; i++) {
                    lv_obj_t *img1 = lv_img_create(bg);
                    if (!is_string_empty(swiper_list[i])) {
                        _set_image_src(img1, swiper_list[i],child->w, child->h,display_id);
                    }
                    _set_image_properties(img1, child);
                }
                return;
            }
        }else{
            _set_image_src(img, child->desc.src, child->w, child->h,display_id);
        }
        _set_image_properties(img, child);
    } else {
        int swiper_list_count = child->extension.swiper_list_count;
        char **swiper_list = child->extension.swiper_list;
        if (swiper_list_count > 0 && swiper_list) {
            for (int i = 0; i < swiper_list_count; i++) {
                lv_obj_t *img = lv_img_create(bg);
                if (!is_string_empty(swiper_list[i])) {
                    _set_image_src(img, swiper_list[i],child->w, child->h,display_id);
                }
                _set_image_properties(img, child);
            }
            return;
        } else {
            lv_obj_t *img = lv_img_create(bg);
            if (!is_string_empty(child->desc.src)) {
                _set_image_src(img, child->desc.src,child->w, child->h,display_id);
            }
            _set_image_properties(img, child);
        }
    }
}

static void _handle_qr_code_extension(lv_obj_t *bg, esl_show_model_child_t *child, esl_product_info_t **product_info_list, struct hashmap *map,int count,int display_id,char **other_3_arr,int show_count,esl_device_setting_t * device_setting) {
    struct  esl_product_item_key_t *user;
    int index = 0;

    if (!is_string_empty(child->extension.bind)) {
        user = hashmap_get(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind});
        if(user){
            if(show_count - 1 > user->count){
                index = user->count + 1;
            }else{
                index = user->count;
            }
        }else{
            index = 0;
        }
        hashmap_set(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind, .count = index});
        char *item_no = other_3_arr[index];
        esl_product_info_t *product_info = find_product_info_by_item_no(item_no, product_info_list, count);
        #if LV_USE_QRCODE
            if (strcmp(child->extension.bind, "$device_info") == 0) {
                int bg_index = display_id + 1;
                char *device_sn = get_device_sn(bg_index);
                cJSON *json = cJSON_CreateObject();
                cJSON_AddStringToObject(json, "SN", device_sn);
                char *data = cJSON_Print(json);

                float size = fminf(child->w, child->h) - 20;
                lv_obj_t * qr = lv_qrcode_create(bg);
                lv_qrcode_set_size(qr, size);
                lv_qrcode_update(qr, data, strlen(data));
                lv_obj_set_size(qr, child->w, child->h);
                lv_obj_set_pos(qr, child->x, child->y);
                if (child->desc.borderRadius > 0) {
                    lv_obj_set_style_radius(qr, LV_RADIUS_CIRCLE, 0);
                }
                free(data);
                cJSON_Delete(json);
            }
        #endif
    }
}

static void _handle_bar_code_extension(lv_obj_t *bg, esl_show_model_child_t *child, esl_product_info_t **product_info_list, struct hashmap *map,int count,int display_id,char **other_3_arr,int show_count) {
    struct  esl_product_item_key_t *user;
    int index = 0;

    if (!is_string_empty(child->extension.bind)) {
        user = hashmap_get(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind});
        if(user){
            if(show_count - 1 > user->count){
                index = user->count + 1;
            }else{
                index = user->count;
            }
        }else{
            index = 0;
        }
        hashmap_set(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind, .count = index});
        char *item_no = other_3_arr[index];
        #if 0
            #if LV_USE_BARCODE
                if (strcmp(child->extension.bind, "$bar_item_no") == 0) {
                    LV_LOG_USER("barcode child->w:%f,child->h:%f,child->x:%f,child->y:%f",child->w, child->h,child->x, child->y);
                    lv_obj_t * barcode = lv_barcode_create(bg);
                    lv_obj_set_size(barcode, child->w, child->h);
                    lv_obj_set_pos(barcode,child->x, child->y);
                    lv_barcode_update(barcode, item_no);
                }
            #endif
        #else
            if (strcmp(child->extension.bind, "$bar_item_no") == 0) {
                #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
                LV_LOG_USER("barcode child->w:%f,child->h:%f,child->x:%f,child->y:%f",child->w, child->h,child->x, child->y);
                //根据k_path_img_prefix item_no display_id  组成根据k_path_img_prefix+item_no+"_"+display_id+".png"
                char full_path[254];
                // 使用 sprintf 函数进行字符串拼接
                sprintf(full_path, "%s%s_%d.png", k_path_img_prefix, item_no, display_id);
                // 判断文件是否存在
                if (!_file_exists(full_path)) {
                    // 如果文件不存在，直接画一个
                    int flag = genarate_128code(item_no,10,full_path);
                    LV_LOG_USER("====================================genarate_128code flag:%d,item_no=%s",flag,item_no);
                    LV_LOG_USER("====================================genarate_128code flag:%d,full_path=%s",flag,full_path);
                    if(flag != 0){
                        genarate_128code(item_no,10,full_path);
                    }
                }
                _add_file_to_download_list(full_path,display_id);
                int width = 0;
                int height = 0;
                int disp_width = child->w;
                int disp_height = child->h;
                lv_obj_t *img = lv_img_create(bg);
                GetPicWidthHeight(full_path, &width, &height);
                // Calculate and apply zoom factor
                if (width > 0 && height > 0) {
                    int zoom_factor_x = (disp_width * 256) / width; // 256 is the default zoom level (100%)
                    int zoom_factor_y = (disp_height * 256) / height; // 256 is the default zoom level (100%)
                    lv_image_set_scale_x(img, zoom_factor_x);
                    lv_image_set_scale_y(img, zoom_factor_y);
                }
                char file_path[256];
                sprintf(file_path, "H:%s", full_path);
                lv_img_set_src(img, file_path);
                _set_image_properties(img, child);
                #endif
            }
        #endif
    }
}

static void _handle_video_extension(lv_obj_t *bg, esl_show_model_child_t *child, esl_product_info_t **product_info_list, struct hashmap *map,int count, int display_id,char **other_3_arr,int show_count) {
    struct esl_product_item_key_t *user;
    int index = 0;

    if (!is_string_empty(child->extension.bind)) {
        user = hashmap_get(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind});
        if(user){
            if(show_count - 1 > user->count){
                index = user->count + 1;
            }else{
                index = user->count;
            }
        }else{
            index = 0;
        }
        hashmap_set(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind, .count = index});
        char *item_no = other_3_arr[index];
        esl_product_info_t *product_info = find_product_info_by_item_no(item_no, product_info_list, count);

        if (strcmp(child->extension.bind, "$item_video") == 0) {
            if(display_id == 0){
                s_player_1_count = 0;
                s_player_1_index = 1;
                s_player_1_video_list = (char **)calloc(show_count, sizeof(char *));
                if (s_player_1_video_list == NULL) {
                    perror("Failed to allocate memory for s_player_1_video_list");
                    return;
                }
                for (int i = 0; i < show_count; i++){
                    char *item_no_temp = other_3_arr[i];
                    esl_product_info_t *product_info_temp = find_product_info_by_item_no(item_no_temp, product_info_list, count);
                    if(product_info_temp){
                        char * src = product_info_temp->item_video;
                        char new_url[256];  // 假设 URL 长度不超过 256，可根据实际情况调整数组大小
                        // 查找 '?' 的位置
                        char *pos = strchr(src, '?');
                        if (pos == NULL) {  // 如果没有 '?'，原 URL 即为新 URL
                            strcpy(new_url, src);
                        } else {
                            // 复制 '?' 之前的部分到 new_url
                            strncpy(new_url, src, pos - src);
                            new_url[pos - src] = '\0';  // 添加字符串结束符
                        }
                        char file_path[256];
                        _generate_filename(new_url, display_id, file_path,1);
                        if(_file_exists(file_path)){
                            s_player_1_video_list[s_player_1_count] = strdup(file_path);
                            s_player_1_count++;
                        }
                    }
                }
                if(s_player_1_video_list != NULL && s_player_1_count > 0){
                    _set_player_src(bg,s_player_1_video_list[0],child->x, child->y,child->w, child->h,display_id);
                }
            }else if(display_id == 1){
                s_player_2_count = 0;
                s_player_2_index = 1;
                s_player_2_video_list = (char **)calloc(show_count, sizeof(char *));
                if (s_player_2_video_list == NULL) {
                    perror("Failed to allocate memory for s_player_2_video_list");
                    return;
                }
                for (int i = 0; i < show_count; i++){
                    char *item_no_temp = other_3_arr[i];
                    esl_product_info_t *product_info_temp = find_product_info_by_item_no(item_no_temp, product_info_list, count);
                    if(product_info_temp){
                        char * src = product_info_temp->item_video;
                        char new_url[256];  // 假设 URL 长度不超过 256，可根据实际情况调整数组大小
                        // 查找 '?' 的位置
                        char *pos = strchr(src, '?');
                        if (pos == NULL) {  // 如果没有 '?'，原 URL 即为新 URL
                            strcpy(new_url, src);
                        } else {
                            // 复制 '?' 之前的部分到 new_url
                            strncpy(new_url, src, pos - src);
                            new_url[pos - src] = '\0';  // 添加字符串结束符
                        }
                        char file_path[256];
                        _generate_filename(new_url, display_id, file_path,1);
                        if(_file_exists(file_path)){
                            s_player_2_video_list[s_player_2_count] = strdup(file_path);
                            s_player_2_count++;
                            LV_LOG_USER("==================================s_player_2_video_list[%d]:%s",i,s_player_2_video_list[i]);
                            LV_LOG_USER("==================================s_player_2_count:%d",s_player_2_count);
                        }
                    }
                }
                if(s_player_2_video_list != NULL && s_player_2_count > 0){
                    _set_player_src(bg,s_player_2_video_list[0],child->x, child->y,child->w, child->h,display_id);
                }
            }
        }
    } else {
        if(display_id == 0){
            s_player_1_count = 0;
            s_player_1_index = 1;
            s_player_1_video_list = (char **)calloc(show_count, sizeof(char *));
            if (s_player_1_video_list == NULL) {
                perror("Failed to allocate memory for s_player_1_video_list");
                return;
            }
            for (int i = 0; i < show_count; i++){
                char * src = child->extension.value;
                char new_url[256];  // 假设 URL 长度不超过 256，可根据实际情况调整数组大小
                // 查找 '?' 的位置
                char *pos = strchr(src, '?');
                if (pos == NULL) {  // 如果没有 '?'，原 URL 即为新 URL
                    strcpy(new_url, src);
                } else {
                    // 复制 '?' 之前的部分到 new_url
                    strncpy(new_url, src, pos - src);
                    new_url[pos - src] = '\0';  // 添加字符串结束符
                }
                char file_path[256];
                _generate_filename(new_url, display_id, file_path,1);
                if(_file_exists(file_path)){
                    s_player_1_video_list[s_player_1_count] = strdup(file_path);
                    s_player_1_count++;
                }
            }
            if(s_player_1_video_list != NULL && s_player_1_count > 0){
                _set_player_src(bg,s_player_1_video_list[0],child->x, child->y,child->w, child->h,display_id);
            }
        }else if(display_id == 1){
            s_player_2_count = 0;
            s_player_2_index = 1;
            s_player_2_video_list = (char **)calloc(show_count, sizeof(char *));
            if (s_player_2_video_list == NULL) {
                perror("Failed to allocate memory for s_player_2_video_list");
                return;
            }
            for (int i = 0; i < show_count; i++){
                char * src = child->extension.value;
                char new_url[256];  // 假设 URL 长度不超过 256，可根据实际情况调整数组大小
                // 查找 '?' 的位置
                char *pos = strchr(src, '?');
                if (pos == NULL) {  // 如果没有 '?'，原 URL 即为新 URL
                    strcpy(new_url, src);
                } else {
                    // 复制 '?' 之前的部分到 new_url
                    strncpy(new_url, src, pos - src);
                    new_url[pos - src] = '\0';  // 添加字符串结束符
                }
                char file_path[256];
                _generate_filename(new_url, display_id, file_path,1);
                if(_file_exists(file_path)){
                    s_player_2_video_list[s_player_2_count] = strdup(file_path);
                    s_player_2_count++;
                }
            }
            if(s_player_2_video_list != NULL && s_player_2_count > 0){
                _set_player_src(bg,s_player_2_video_list[0],child->x, child->y,child->w, child->h,display_id);
                LV_LOG_USER("==================================s_player_2_video_list[0]:%s",s_player_2_video_list[0]);
                LV_LOG_USER("==================================s_player_2_count:%d",s_player_2_count);
            }
        }
    }
}

void handle_image_extension(lv_obj_t *bg, esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,int display_id,char **other_3_arr,int show_count) {
    if (strcmp(child->extension.ex_type, "image") == 0) {
        _handle_image_extension(bg, child, product_info_list, map, count,display_id,other_3_arr, show_count);
    } else if (strcmp(child->extension.ex_type, "video") == 0) {
        _handle_video_extension(bg, child, product_info_list, map, count, display_id, other_3_arr, show_count);
    }else if (strcmp(child->extension.ex_type, "qr_code") == 0) {
        _handle_qr_code_extension(bg, child, product_info_list, map, count,display_id,other_3_arr, show_count,device_setting);
    } else if (strcmp(child->extension.ex_type, "bar_code") == 0) {
        _handle_bar_code_extension(bg, child, product_info_list, map, count,display_id,other_3_arr, show_count);
    }
}

void handle_video_extension(lv_obj_t *bg, esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,int display_id,char **other_3_arr,int show_count) {
    if (strcmp(child->extension.ex_type, "video") == 0) {
        _handle_video_extension(bg, child, product_info_list, map, count, display_id, other_3_arr, show_count);
    }
}

//type: 1:image, 2:video
static int _download_image_or_video(char * src,int display_id,int type, int disp_width, int disp_height){
    // 判断 src 是否是 URL
    if (strncmp(src, "http://", 7) != 0 && strncmp(src, "https://", 8) != 0){
        return -1;
    }

    char new_url[256];  // 假设 URL 长度不超过 256，可根据实际情况调整数组大小
    if (type == 1) {
        strcpy(new_url, src);
    } else if (type == 2) {
        // 查找 '?' 的位置
        char *pos = strchr(src, '?');
        if (pos == NULL) {  // 如果没有 '?'，原 URL 即为新 URL
            strcpy(new_url, src);
        } else {
            // 复制 '?' 之前的部分到 new_url
            strncpy(new_url, src, pos - src);
            new_url[pos - src] = '\0';  // 添加字符串结束符
        }
    } else {
        return -1;  // 如果 type 不是 1 或 2，直接返回
    }

    char file_path[256];
    _generate_filename(new_url, display_id, file_path,0);
    int local_index = display_id + 1;
    int result = _download_image_if_not_exist(new_url, file_path,local_index,type);
    if(result == 0){
        return 0;
    }

    //scale file if needed
    //ignore video
    if (type != 1) {
        printf("[scale judge]ignore video\n");
        return 1;
    }

    char full_path[256];
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        perror("Failed to open downloaded image");
        return 0;
    }
    unsigned char header[8];
    fread(header, 1, 8, file);
    fclose(file);
    int actual_format = 0; // 1 for PNG, 2 for JPG
    if (is_png(header)) {
        actual_format = 1;
    } else if (is_jpg(header)) {
        actual_format = 2;
    }

    // Determine the expected format based on file extension
    const char *ext = strrchr(file_path, '.');
    int expected_format = 0; // 1 for PNG, 2 for JPG
    if (ext && (strcmp(ext, ".png") == 0 || strcmp(ext, ".PNG") == 0)) {
        expected_format = 1;
    } else if (ext && (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".JPG") == 0 || strcmp(ext, ".JPEG") == 0)) {
        expected_format = 2;
    }
    
    if (expected_format == 1 && actual_format == 2) {
        char jpg_path[254];
        strncpy(jpg_path, file_path, ext - file_path);
        jpg_path[ext - file_path] = '\0';
        strcat(jpg_path, ".jpg");
        strncpy(jpg_path, file_path, ext - file_path);
        snprintf(full_path, sizeof(full_path), "H:%s", jpg_path);
    }else if(expected_format == 2 && actual_format == 1){
        char png_path[254];
        strncpy(png_path, file_path, ext - file_path);
        png_path[ext - file_path] = '\0';
        strcat(png_path, ".png");
        strncpy(png_path, file_path, ext - file_path);
        snprintf(full_path, sizeof(full_path), "%s", png_path);
    }else{
        snprintf(full_path, sizeof(full_path), "%s", file_path);
    }
    //
    if (strstr(full_path, ".jpg")) {
        //logo
        #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        extern void qm_rewrite_jpg_to_420_if_needed(const char *file_path);
        qm_rewrite_jpg_to_420_if_needed(full_path);
        #endif
        return 1;
    }
    //
    if (disp_width == 331 && disp_height == 331)
    {
        if (strstr(full_path, "216c0779ea7118825ca42478ff3a018b")) {
            printf("ignore big png.\n");
            return 1;
        }
    }

    int width = 0;
    int height = 0;
    extern int qm_get_png_size(const char* path, unsigned int* punWidth, unsigned int* punHeight);
    int ret = qm_get_png_size(full_path, &width, &height);
    if (ret != 0) {
        LV_LOG_ERROR("qm_get_png_size failed\n");
        return 0;
    }

    printf("[After download] file_path=%s, type=%d, width=%d, height=%d, disp_width=%d, disp_height=%d\n", full_path, type, width, height, disp_width, disp_height);

    //scale
    if ((width != disp_width || height != disp_height)) {
        unsigned char* argb_buffer = NULL;
        unsigned int mmz_phy_buf = 0;
        lv_color_format_t cf = LV_COLOR_FORMAT_ARGB8888;
        bool use_mmz = false;

        extern int qm_png2param_create_v2(const char *h_image_path, uint8_t **pOutDecoded, int scale_width, int scale_height, bool need_reencode);
        int result = qm_png2param_create_v2(full_path, &argb_buffer, disp_width, disp_height, true);
        if(result != 0){
            return 0;
        }
        if (argb_buffer != NULL) {
            qm_pngencoder(argb_buffer, disp_width, disp_height, full_path);
            free(argb_buffer);
        } else {
            printf("------------------------qm_argb_scale_image failed------------------\n");
        }
    }
    else {
        printf("[after download] qm_argb_scale_image skip scale!\n");
    }
    return 1;
}

static void _handle_image_extension_download(esl_show_model_child_t *child, esl_product_info_t **product_info_list, struct hashmap *map,int count,int display_id,char **other_3_arr,int show_count){
    struct esl_product_item_key_t *user;
    int index = 0;

    if (!is_string_empty(child->extension.bind)) {
        user = hashmap_get(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind});
        if(user){
            if(show_count - 1 > user->count){
                index = user->count + 1;
            }else{
                index = user->count;
            }
        }else{
            index = 0;
        }
        hashmap_set(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind, .count = index});
        char *item_no = other_3_arr[index];
        esl_product_info_t *product_info = find_product_info_by_item_no(item_no, product_info_list, count);

        if (strcmp(child->extension.bind, "$item_image") == 0) {
            _download_image_or_video(product_info->item_url,display_id,1,child->w,child->h);
        } else if (strcmp(child->extension.bind, "$ad_image") == 0) {
            _download_image_or_video(child->desc.src,display_id,1,child->w,child->h);
        } else if (strcmp(child->extension.bind, "$swiper") == 0) {
            int swiper_list_count = child->extension.swiper_list_count;
            char **swiper_list = child->extension.swiper_list;
            if (swiper_list_count > 0 && swiper_list) {
                for (int i = 0; i < swiper_list_count; i++) {
                    if (!is_string_empty(swiper_list[i])) {
                        _download_image_or_video(swiper_list[i],display_id,1,child->w,child->h);
                    }
                }
                return;
            }
        }else{
            _download_image_or_video(child->desc.src,display_id,1,child->w,child->h);
        }
    } else {
        int swiper_list_count = child->extension.swiper_list_count;
        char **swiper_list = child->extension.swiper_list;
        if (swiper_list_count > 0 && swiper_list) {
            for (int i = 0; i < swiper_list_count; i++) {
                if (!is_string_empty(swiper_list[i])) {
                    _download_image_or_video(swiper_list[i],display_id,1,child->w,child->h);
                }
            }
            return;
        } else {
            if (!is_string_empty(child->desc.src)) {
                _download_image_or_video(child->desc.src,display_id,1,child->w,child->h);
            }
        }
    }
}

static void _handle_qr_code_extension_download(esl_show_model_child_t *child,int display_id) {

    if (is_string_empty(child->extension.bind)) {
        /* if (!is_string_empty(child->desc.src)) {
            _download_image_or_video(child->desc.src,display_id,1,child->w,child->h);
        } */
    }
}

static void _handle_bar_code_extension_download(esl_show_model_child_t *child,int display_id) {
    /* if (!is_string_empty(child->desc.src)) {
        _download_image_or_video(child->desc.src,display_id,1,child->w,child->h);
    } */
}

static void _handle_video_extension_download(esl_show_model_child_t *child, esl_product_info_t **product_info_list, struct hashmap *map,int count, int display_id,char **other_3_arr,int show_count) {
    struct esl_product_item_key_t *user;
    int index = 0;

    if (!is_string_empty(child->extension.bind)) {
        /* user = hashmap_get(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind});
        if(user){
            if(show_count - 1 > user->count){
                index = user->count + 1;
            }else{
                index = user->count;
            }
        }else{
            index = 0;
        }
        hashmap_set(map, &(struct esl_product_item_key_t){.product_item_key = child->extension.bind, .count = index});
        char *item_no = other_3_arr[index];
        esl_product_info_t *product_info = find_product_info_by_item_no(item_no, product_info_list, count); */

        if (strcmp(child->extension.bind, "$item_video") == 0) {
            for(int i = 0;i < count; i++){
                char *item_no = other_3_arr[i];
                esl_product_info_t *product_info = find_product_info_by_item_no(item_no, product_info_list, count);
                _download_image_or_video(product_info->item_video,display_id,2,child->w,child->h);
            }
        }
    } else {
        _download_image_or_video(child->extension.value,display_id,2,child->w,child->h);
    }
}

void handle_image_extension_download(esl_show_model_child_t *child,esl_device_setting_t * device_setting, esl_product_info_t **product_info_list, struct hashmap *map,int count,int display_id,char **other_3_arr,int show_count){
    if (strcmp(child->extension.ex_type, "image") == 0) {
        _handle_image_extension_download(child, product_info_list, map, count,display_id,other_3_arr, show_count);
    } else if (strcmp(child->extension.ex_type, "qr_code") == 0) {
        _handle_qr_code_extension_download(child,display_id);
    } else if (strcmp(child->extension.ex_type, "bar_code") == 0) {
        _handle_bar_code_extension_download(child,display_id);
    } else if (strcmp(child->extension.ex_type, "video") == 0) {
        _handle_video_extension_download(child, product_info_list, map, count, display_id, other_3_arr, show_count);
    }
}

static void _remove_file_extension(char *filename) {
    char *dot = strrchr(filename, '.');
    if (dot) {
        *dot = '\0';
    }
}

void *compare_local_resouce(void *arg){
    compare_data_t *args = (compare_data_t *)arg;
    int index = args->index;
    int display_id = index - 1;
    int flag = is_need_delete_resource();
    if(flag == 0){
        // 重置下载文件列表
        if (display_id == 0) {
            memset(download_files_1, 0, sizeof(download_files_1));
            download_files_count_1 = 0;
        } else {
            memset(download_files_2, 0, sizeof(download_files_2));
            download_files_count_2 = 0;
        }
        free(args);
        return NULL;
    }
    struct dirent *entry;
    DIR *dp = opendir(k_path_img_prefix);
    if (dp == NULL) {
        perror("opendir");
        // 重置下载文件列表
        if (display_id == 0) {
            memset(download_files_1, 0, sizeof(download_files_1));
            download_files_count_1 = 0;
        } else {
            memset(download_files_2, 0, sizeof(download_files_2));
            download_files_count_2 = 0;
        free(args);
        }
        return NULL;
    }
    char files_to_download[MAX_FILES][MAX_FILENAME_LENGTH];
    int files_to_download_count = 0;
    if(display_id == 0){
        if(download_files_1 == NULL || download_files_count_1 == 0){
            free(args);
            closedir(dp);
            return NULL;
        }
        memcpy(files_to_download, download_files_1, sizeof(download_files_1));
        files_to_download_count = download_files_count_1;
    }else{
        if(download_files_2 == NULL || download_files_count_2 == 0){
            free(args);
            closedir(dp);
            return NULL;
        }
        memcpy(files_to_download, download_files_2, sizeof(download_files_2));
        files_to_download_count = download_files_count_2;
    }

    char identifier[8];
    snprintf(identifier, sizeof(identifier), "_%d", display_id);

    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_REG) {
            char *file_name = entry->d_name;
            if(!strstr(file_name, identifier)){
                continue;
            }
            bool found = false;
            char file_name_no_ext[MAX_FILENAME_LENGTH];
            strncpy(file_name_no_ext, file_name, MAX_FILENAME_LENGTH);
            _remove_file_extension(file_name_no_ext);

            for (int i = 0; i < files_to_download_count; i++) {
                // 2. 判断本地文件名是否包含下载文件的标识符
                if (strstr(files_to_download[i], file_name_no_ext) && !ends_with(file_name_no_ext , ".download")) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                char file_path[256];
                snprintf(file_path, sizeof(file_path), "%s%s", k_path_img_prefix, file_name);
                LV_LOG_USER("=============compare_local_resouce not found file_path:%s",file_path);
                remove(file_path);
            }
        }
    }
    closedir(dp);
    // 重置下载文件列表
    if (display_id == 0) {
        memset(download_files_1, 0, sizeof(download_files_1));
        download_files_count_1 = 0;
    } else {
        memset(download_files_2, 0, sizeof(download_files_2));
        download_files_count_2 = 0;
    }

    free(arg);
    return NULL;
}

void set_rotation_1(int rotation){
    rotation_1 = rotation;
}
void set_rotation_2(int rotation){
    rotation_2 = rotation;
}
#endif
