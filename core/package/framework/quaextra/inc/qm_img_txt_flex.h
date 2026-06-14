#ifndef __QM_IMG_TXT_FLEX_H__
#define __QM_IMG_TXT_FLEX_H__

#include "lvgl/lvgl.h"

/**
 * @brief 创建一个图片和文字的布局，图片和文字在同一行
 * 
 * @param p 父对象
 * @param w 宽度
 * @param h 高度
 * @param img_normal 正常状态下的图片
 * @param img_selected 选中状态下的图片
 * @param img_txt_space 图片和文字之间的间距
 * @param padding_top 上边距
 * @param padding_bottom 下边距
 * @param font_path 字体路径
 * @param label_name 文字内容
 * @param font_size 字体大小
 * @param is_selected 是否选中
 * @return lv_obj_t* 创建的对象
 */
lv_obj_t *qm_img_txt_flex_init(lv_obj_t *p, int w, int h, 
    const void *img_normal, const void *img_selected, 
    int img_txt_space, int padding_top, int padding_bottom,
    const char *font_path,
    const char *label_name, int font_size, bool is_selected);

void qm_img_txt_flex_update(lv_obj_t *cont, bool is_selected);

#endif
