/*******************************************************************************************
#  Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_SPNG_UTILS_H__
#define __QUA_SPNG_UTILS_H__

#define QUA_SPNG_BLOCK_MAX_WIDTH 400
#define QUA_SPNG_BLOCK_MAX_HEIGHT 640

int qua_spng_chunk_transform_process(const char *src_path, const char *dst_path, int scale_w, int scale_h);

#endif // __QUA_SPNG_UTILS_H__