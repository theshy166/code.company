/*******************************************************************************************
#  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_MM_VIDEO_COMMON_TYPE_H__
#define __QUA_MM_VIDEO_COMMON_TYPE_H__

#include "qua_type.h"

typedef enum {
    QUA_VIDEO_FIELD_TOP               = 0x1,
    QUA_VIDEO_FIELD_BOTTOM            = 0x2,
    QUA_VIDEO_FILED_INTERLACED        = 0x3,
    QUA_VIDEO_FIELD_FRAME             = 0x4,

    QUA_VIDEO_FIELD_BUTT
} qua_video_field_t;

typedef enum {
    QUA_PIXEL_FMT_RGB_1BPP = 0,

    QUA_PIXEL_FMT_RGB_2BPP,
    QUA_PIXEL_FMT_RGB_4BPP,
    QUA_PIXEL_FMT_RGB_8BPP,
    QUA_PIXEL_FMT_RGB_444,

    QUA_PIXEL_FMT_RGB_4444,
    QUA_PIXEL_FMT_RGB_555,
    QUA_PIXEL_FMT_RGB_565,
    QUA_PIXEL_FMT_RGB_1555,

    QUA_PIXEL_FMT_RGB_888,
    QUA_PIXEL_FMT_RGB_8888,

    QUA_PIXEL_FMT_RGB_PLANAR_888,
    QUA_PIXEL_FMT_RGB_BAYER_8BPP,
    QUA_PIXEL_FMT_RGB_BAYER_10BPP,
    QUA_PIXEL_FMT_RGB_BAYER_12BPP,
    QUA_PIXEL_FMT_RGB_BAYER_14BPP,

    QUA_PIXEL_FMT_RGB_BAYER,

    QUA_PIXEL_FMT_YUV_A422,
    QUA_PIXEL_FMT_YUV_A444,

    QUA_PIXEL_FMT_YUV_PLANAR_422,
    QUA_PIXEL_FMT_YUV_PLANAR_420,

    QUA_PIXEL_FMT_YUV_PLANAR_444,

    QUA_PIXEL_FMT_YUV_SEMIPLANAR_422,
    QUA_PIXEL_FMT_YUV_SEMIPLANAR_420,
    QUA_PIXEL_FMT_YUV_SEMIPLANAR_444,

    QUA_PIXEL_FMT_UYVY_PACKAGE_422,
    QUA_PIXEL_FMT_YUYV_PACKAGE_422,
    QUA_PIXEL_FMT_VYUY_PACKAGE_422,
    QUA_PIXEL_FMT_YCbCr_PLANAR,

    QUA_PIXEL_FMT_YUV_400,

    QUA_PIXEL_FMT_YUV_422_10BPP = 43,
    QUA_PIXEL_FMT_YUV_420_BLOCK,

    QUA_PIXEL_FMT_YUV_422_MIRROR,

    QUA_PIXEL_FMT_YUV_FIELD_422,
    QUA_PIXEL_FMT_YUV_FIELD_420,
    QUA_PIXEL_FMT_YUV_FIELD_1D_422,

    QUA_PIXEL_FMT_YVU_SEMIPLANAR_420,

    QUA_PIXEL_FMT_YVYU_PACKAGE_422,
    QUA_PIXEL_FMT_ABGR_8888,
    QUA_PIXEL_FMT_RGBA_8888,
    QUA_PIXEL_FMT_BGRA_8888,

    QUA_PIXEL_FMT_BUTT
} qua_pixel_fmt_t;

typedef enum qua_video_format_t {
    QUA_VIDEO_FORMAT_LINEAR,
    QUA_VIDEO_FORMAT_TILE,
    QUA_VIDEO_FORMAT_TILE64,
    QUA_VIDEO_FORMAT_BLOCK,
    QUA_VIDEO_FORMAT_BUTT
} qua_video_format_t;

typedef enum qua_compress_mode_t {
    QUA_COMPRESS_MODE_NONE   = 0x0,
    QUA_COMPRESS_MODE_SEG,
    QUA_COMPRESS_MODE_SEG128,
    QUA_COMPRESS_MODE_LINE,
    QUA_COMPRESS_MODE_FRAME,
    QUA_COMPRESS_MODE_128byte,
    QUA_COMPRESS_MODE_160byte,
    QUA_COMPRESS_MODE_192byte,
    QUA_COMPRESS_MODE_TILE_192,
    QUA_COMPRESS_MODE_SLICE,
    QUA_COMPRESS_MODE_CF10,
    QUA_COMPRESS_MODE_FBC,
    QUA_COMPRESS_MODE_TILE_224,
    QUA_COMPRESS_MODE_TILE_256,
    QUA_COMPRESS_MODE_MFBC,
    QUA_COMPRESS_MODE_BUTT
} qua_compress_mode_t;

typedef enum {
    QUA_FRAME_FLASH_OFF = 0,
    QUA_FRAME_FLASH_ON = 1,
    QUA_FRAME_FLASH_BUTT
} qua_frame_flash_type_t;

typedef struct qua_video_supplement_t {
    QUA_U32 jpeg_dcf_phy_addr;
    QUA_U64 jpeg_dcf_vir_addr;

    qua_frame_flash_type_t flash_type;
} qua_video_supplement_t;

typedef struct qua_video_frame_t {
    QUA_U32 width;
    QUA_U32 height;
    qua_video_field_t field;
    qua_pixel_fmt_t pixel_fmt;

    qua_video_format_t video_fmt;
    qua_compress_mode_t compress_mode;

    QUA_U64 phy_addr[3];
    QUA_U64 vir_addr[3];
    QUA_U32 stride[3];

    QUA_U64 header_phy_addr[3];
    QUA_U64 header_vir_addr[3];
    QUA_U32 header_stride[3];

    QUA_S16 offset_top;
    QUA_S16 offset_bottom;
    QUA_S16 offset_left;
    QUA_S16 offset_right;

    QUA_U64 pts;
    QUA_U32 time_ref;

    QUA_U32 priv_data;
    qua_video_supplement_t supplement;
    QUA_U32 src_expand_info;
} qua_video_frame_t;

typedef struct qua_video_frame_info_t {
    qua_video_frame_t video_frame;
    QUA_U32 pool_id;
} qua_video_frame_info_t;

typedef struct qua_rect_t {
    QUA_S32 x;
    QUA_S32 y;
    QUA_U32 width;
    QUA_U32 height;
} qua_rect_t;

typedef struct qua_size_t {
    QUA_U32 width;
    QUA_U32 height;
} qua_size_t;

typedef struct qua_point_t {
    QUA_S32 x;
    QUA_S32 y;
} qua_point_t;

typedef struct qua_border {
    QUA_U32 top_width;
    QUA_U32 bottom_width;
    QUA_U32 left_width;
    QUA_U32 right_width;
    QUA_U32 color;
} qua_border_t;

typedef struct qua_crop_info {
    QUA_BOOL enable;
    qua_rect_t rect;
} qua_crop_info_t;

typedef enum qua_aspect_ratio {
    QUA_ASPECT_RATIO_NONE   = 0,
    QUA_ASPECT_RATIO_AUTO   = 1,
    QUA_ASPECT_RATIO_MANUAL = 2,
    QUA_ASPECT_RATIO_BUTT
} qua_aspect_ratio_t;

typedef struct qua_aspect_ratio_info {
    qua_aspect_ratio_t mode;
    QUA_U32 bg_color;
    qua_rect_t video_rect;
} qua_aspect_ratio_info_t;

typedef enum qua_compress_ratio {
    QUA_COMPRESS_RATIO_NONE      = 100,
    QUA_COMPRESS_RATIO_75_PER    = 75,
    QUA_COMPRESS_RATIO_70_PER    = 70,
    QUA_COMPRESS_RATIO_67_PER    = 67,
    QUA_COMPRESS_RATIO_65_PER    = 65,
    QUA_COMPRESS_RATIO_63_PER    = 63,
    QUA_COMPRESS_RATIO_60_PER    = 60,
    QUA_COMPRESS_RATIO_59_PER    = 59,
    QUA_COMPRESS_RATIO_55_PER    = 55,
    QUA_COMPRESS_RATIO_50_PER    = 50,
    QUA_COMPRESS_RATIO_45_PER    = 45,
    QUA_COMPRESS_RATIO_40_PER    = 40,
    QUA_COMPRESS_RATIO_35_PER    = 35,
    QUA_COMPRESS_RATIO_30_PER    = 30
} qua_compress_ratio_t;

typedef enum qua_rotate {
    QUA_ROTATE_NONE    = 0,
    QUA_ROTATE_90      = 1,
    QUA_ROTATE_180     = 2,
    QUA_ROTATE_270     = 3,
    QUA_ROTATE_MIRROR  = 4,
    QUA_ROTATE_FLIP    = 5,
    QUA_ROTATE_BUTT
} qua_rotate_t;

#endif // __QUA_MM_VIDEO_COMMON_TYPE_H__
