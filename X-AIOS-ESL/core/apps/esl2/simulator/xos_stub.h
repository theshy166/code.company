#ifndef XOS_STUB_H
#define XOS_STUB_H

#include "lvgl/lvgl.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================
   qua_font.h (字体)
   ======================== */
/**
 * @brief 加载字体并通过输出参数返回字体指针
 * @param font_out  [OUT] 接收创建的 lv_font_t* 指针
 * @param size      字号
 * @param path      字体文件路径（可为 NULL）
 * @param style     样式选择器（0=正常）
 */
void qua_xos_ft_init_with_size_path(lv_font_t **font_out, short size, const char *path, short style);
const char *sim_get_device_serial(void);

/* ========================
   qm_wifi_cfg.h (WiFi)
   ======================== */
int qm_wifi_connect_status(void);
void qm_connection_wifi_restart(void);

/* ========================
   qm_gpio.h (GPIO)
   ======================== */
int qm_gpio_export_direction(int pin, int dir);
int qm_gpio_set_value(int pin, int val);

/* ========================
   qm_backlight.h (背光)
   ======================== */
void qm_backlight_setbri(int brightness);

/* ========================
   xos 系统函数
   ======================== */
int xos_lcm_get_screen_num(void);
int xos_logo_update(const char *path);
/* xos_esl_ui_init / xos_esl_ui_init_2 由 page_esl.c 提供，不在此声明 */
const char *xos_get_sn(void);
lv_display_t *lv_display_get_1st_scr(void);
lv_display_t *lv_display_get_2nd_scr(void);

/* ========================
   模拟器字体初始化（必须在 UI 创建前调用）
   ======================== */
void sim_init_fonts(void);

/* ========================
   serial_number.h
   ======================== */
const char *serial_number_get(void);

/* ========================
   qm_lv_obj.h
   ======================== */
lv_obj_t *qm_container_create(lv_obj_t *parent);
lv_obj_t *qm_label_create(lv_obj_t *parent);
void qm_label_set_text(lv_obj_t *label, const char *text);
void qm_obj_set_pos(lv_obj_t *obj, int x, int y);
void qm_obj_set_size(lv_obj_t *obj, int w, int h);
void qm_obj_set_style_text_color(lv_obj_t *obj, lv_color_t color);
void qm_obj_set_style_bg_color(lv_obj_t *obj, lv_color_t color);
lv_obj_t *qm_image_create(lv_obj_t *parent);

/* ========================
   qua_param.h
   ======================== */
int param_get_int(const char *key, int def);
const char *param_get_string(const char *key, const char *def);
int param_set_int(const char *key, int val);
int param_set_string(const char *key, const char *val);
int param_save(void);

/* ========================
   图片/多媒体
   ======================== */
int is_jpg(const char *path);
int is_png(const char *path);
unsigned char *decode_jpeg_with_turbo(const char *path, int *w, int *h, bool use_mmz, unsigned int *mmz_phy_buf);
int download_image(const char *url, const char *save_path);

/* ========================
   LOS 视频播放器
   ======================== */
void *los_player_create2(lv_obj_t *parent, const char *path, lv_area_t area,
                         bool auto_restart, bool auto_center, int display_idx, int rotation);
void los_set_progress_callback(void (*progress_cb)(void *user, int purpose, int64_t ct));
void los_player_stop(void *player);
void los_player_destroy2(void *player, int display_idx);
void los_videoplayer_quit(int display_id);

/* ========================
   PNG 编码/处理
   ======================== */
int qm_png2param_create(const char *path, uint8_t **out, int *w, int *h, lv_color_format_t *cf,
                        bool use_mmz, unsigned int *mmz_phy_buf, bool need_reencode);
int qm_png2param_create_v2(const char *path, uint8_t **out, int scale_width, int scale_height, bool need_reencode);
void *qm_pngencoder(const unsigned char *data, int w, int h, int bpp, int *len);
int qm_get_png_size(const char *path, int *w, int *h);

/* ========================
   qme 图片解码 (硬件加速桩)
   ======================== */
void qme_img_fetch_by_path(const char *path, int display_id, int *width, int *height, lv_img_dsc_t **out_img_dsc);
bool qme_img_get_jpeg(const char *image_path, int disp_width, int disp_height, int display_id, lv_img_dsc_t **out_img_dsc);
void qme_img_encape_bits_to_imgdsc(char *file_path, unsigned char *buffer, unsigned int phy_buf,
                                    int w, int h, int display_id, lv_img_dsc_t **out_img_dsc);

/* ========================
   升级相关
   ======================== */
int qm_updater_change_text_label(const char *text);

/* ========================
   其他
   ======================== */
int draw_border_for_debug(lv_obj_t *obj);
unsigned int hex_to_uint32(const char *hex_str);

#ifdef __cplusplus
}
#endif

#endif /* XOS_STUB_H */
