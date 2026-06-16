
#ifndef __QM_PNGDECODER_H__
#define __QM_PNGDECODER_H__

#include "lvgl/lvgl.h"

//
int qm_png2param_create(const char *image_path, uint8_t **pOutDecoded, int *pWidth, int *pHeight, lv_color_format_t *pCf, bool use_mmz, unsigned int *mmz_phy_buf, bool need_reencode);
int qm_png2img_create(const char *png_path, lv_img_dsc_t *img);
void qm_png2img_free(char *pBuf);

#endif
