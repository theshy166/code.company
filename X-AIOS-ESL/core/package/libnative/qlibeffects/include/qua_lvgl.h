#ifndef _LV_BLEND_QUA_H
#define _LV_BLEND_QUA_H


/*********************
 *      INCLUDES
 *********************/

// #ifndef LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_ARGB8888_WITH_MASK
// #define LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_ARGB8888_WITH_MASK(dsc)  
//     _lv_argb8888_blend_normal_to_argb8888_with_mask_qua(dsc)
// #endif

// #ifndef LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_ARGB8888
// #define LV_DRAW_SW_ARGB8888_BLEND_NORMAL_TO_ARGB8888(dsc) qm_effect_blend_normal_to_argb8888(dsc)
// #endif

//get lv_image_dsc_t
// bool lv_qua_img_mask_height(int height);
int xos_qm_record_image(unsigned char * data, unsigned int data_size, void **new_ptr, char is_bg_img);
int xos_qm_image_free(const void *data);
unsigned long lv_qua_query_phy(void *vir);
int lv_qua_free(unsigned long phy, void* virtual);
//

#endif //_LV_BLEND_QUA_H
