#ifndef __PIC_UTILS_H__
#define __PIC_UTILS_H__

#include "lvgl/lvgl.h"

int GetPNGWidthHeight(const char* path, unsigned int* punWidth, unsigned int* punHeight);

int GetJPEGWidthHeight(const char* path, unsigned int *punWidth, unsigned int *punHeight);

void GetPicWidthHeight(const char* path, unsigned int *punWidth, unsigned int *punHeight);

uint32_t hex_to_uint32(const char *hex_str);

#endif//__PIC_UTILS_H__