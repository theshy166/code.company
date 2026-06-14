/**
 * @file page_esl_image.h
 * @brief ESL (Electronic Shelf Label) image and media handling functions
 */
#ifndef __PAGE_ESL_IMAGE_H__
#define __PAGE_ESL_IMAGE_H__

#include "esl_base_model.h"
#include "lvgl/lvgl.h"
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "pipeline/video_decoder/qua_vdec_pipeline.h"
#include "player/qua_mm_player_common.h"
#endif

/**
 * @brief Image linked list node structure
 */
typedef struct {
    lv_img_dsc_t img_dsc;   /**< LVGL image descriptor */
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    qua_vdec_pipeline_frame_info_t *s_frame;  /**< Video frame information (not for simulator) */
    #endif
} image_ll_node_t;

/**
 * @brief Thread data structure for passing parameters to threads
 */
typedef struct {
    int index;          /**< Display index */
    char * file_path;   /**< File path */
} thread_data_t;

/**
 * @brief Comparison data structure for resource comparison
 */
typedef struct{
    int index;          /**< Display index */
} compare_data_t;

/**
 * @brief Quit/stop the video player for a specific display
 * @param display_id Display identifier
 */
void los_videoplayer_quit(int display_id);

/**
 * @brief Set image source for an LVGL image object
 * @param img Pointer to the LVGL image object
 * @param src Image source path or data
 * @param disp_width Display width in pixels
 * @param disp_height Display height in pixels
 * @param display_id Display identifier
 */
void _set_image_src(lv_obj_t *img, const char *src, int disp_width, int disp_height, int display_id);

/**
 * @brief Thread function to compare local resources
 * @param arg Pointer to thread arguments (thread_data_t)
 * @return NULL
 */
void *compare_local_resouce(void *arg);

/**
 * @brief Check if a logo image exists at the specified path
 * @param filePath Local file path to check
 * @return 1 if the file exists, 0 otherwise
 */
int logo_image_is_exist(const char *filePath);

/**
 * @brief Download a logo image from a URL to a local file
 * @param url URL of the logo image to download
 * @param filePath Local file path to save the downloaded image
 * @return 0 on success, non-zero error code on failure
 */
int download_logo_image(const char *url, const char *filePath);

//-----------------------------------------------------------------------------------------------------

/**
 * @brief Handle image extension for ESL display
 * @param bg Pointer to the background LVGL object
 * @param item Pointer to the ESL component item
 * @param display_id Display identifier
 */
void handle_image_extension2(lv_obj_t *bg, esl_base_model_child_t *item, int display_id);

/**
 * @brief Handle video extension for ESL display
 * @param bg Pointer to the background LVGL object
 * @param model Pointer to the ESL base model
 * @param item Pointer to the ESL component item
 * @param display_id Display identifier
 */
void handle_video_extension2(lv_obj_t *bg, esl_base_model_t *model, esl_base_model_child_t *item, int display_id);

/**
 * @brief Handle QR code extension for ESL display
 * @param bg Pointer to the background LVGL object
 * @param item Pointer to the ESL component item
 * @param display_id Display identifier
 */
void handle_qrcode_extension2(lv_obj_t *bg, esl_base_model_child_t *item, int display_id);

/**
 * @brief Handle barcode extension for ESL display
 * @param bg Pointer to the background LVGL object
 * @param item Pointer to the ESL component item
 * @param display_id Display identifier
 */
void handle_barcode_extension2(lv_obj_t *bg, esl_base_model_child_t *item, int display_id);

/**
 * @brief Handle image extension download
 * @param img_url URL of the image to download
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param display_id Display identifier
 */
void handle_image_extension_download2(char * img_url, int width, int height, int display_id);

/**
 * @brief Handle video extension download
 * @param video_url URL of the video to download
 * @param display_id Display identifier
 */
void handle_video_extension_download2(char * video_url, int display_id);

/**
 * @brief Set rotation angle for the first display
 * @param rotation Rotation angle in degrees
 */
void set_rotation_1(int rotation);

/**
 * @brief Set rotation angle for the second display
 * @param rotation Rotation angle in degrees
 */
void set_rotation_2(int rotation);

/**
 * @brief Set download status for a specific display
 * @param is_downloading Download status (1 for downloading, 0 for not downloading)
 * @param index Display index
 */
void set_is_downloading(int is_downloading, int index);

/**
 * @brief Get download status for a specific display
 * @param index Display index
 * @return 1 if downloading, 0 otherwise
 */
int get_is_downloading(int index);

/**
 * @brief Get the current ESL type
 * @return ESL type identifier
 */
int get_esl_type();

/**
 * @brief Set the ESL type
 * @param esl_type ESL type identifier to set
 */
void set_esl_type(int esl_type);

#endif // __PAGE_ESL_IMAGE_H__