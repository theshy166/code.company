/*******************************************************************************************
#  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_MM_TDE_H__
#define __QUA_MM_TDE_H__

#include "common/qua_mm_common.h"

#define QUA_TDE_INVALID_HANDLE -1

typedef QUA_S32 QUA_TDE_HANDLE;

typedef enum qua_tde2_color_fmt
{
    QUA_TDE2_COLOR_FMT_RGB444 = 0,                 /**< RGB444 format */
    QUA_TDE2_COLOR_FMT_BGR444,                     /**< BGR444 format */
    QUA_TDE2_COLOR_FMT_RGB555,                     /**< RGB555 format */
    QUA_TDE2_COLOR_FMT_BGR555,                     /**< BGR555 format */
    QUA_TDE2_COLOR_FMT_RGB565,                     /**< RGB565 format */
    QUA_TDE2_COLOR_FMT_BGR565,                     /**< BGR565 format */
    QUA_TDE2_COLOR_FMT_RGB888,                     /**< RGB888 format */
    QUA_TDE2_COLOR_FMT_BGR888,                     /**< BGR888 format */
    QUA_TDE2_COLOR_FMT_ARGB4444,                   /**< ARGB4444 format */
    QUA_TDE2_COLOR_FMT_ABGR4444,                   /**< ABGR4444 format */
    QUA_TDE2_COLOR_FMT_RGBA4444,                   /**< RGBA4444 format */
    QUA_TDE2_COLOR_FMT_BGRA4444,                   /**< BGRA4444 format */
    QUA_TDE2_COLOR_FMT_ARGB1555,                   /**< ARGB1555 format */
    QUA_TDE2_COLOR_FMT_ABGR1555,                   /**< ABGR1555 format */
    QUA_TDE2_COLOR_FMT_RGBA1555,                   /**< RGBA1555 format */
    QUA_TDE2_COLOR_FMT_BGRA1555,                   /**< BGRA1555 format */
    QUA_TDE2_COLOR_FMT_ARGB8565,                   /**< ARGB8565 format */
    QUA_TDE2_COLOR_FMT_ABGR8565,                   /**< ABGR8565 format */
    QUA_TDE2_COLOR_FMT_RGBA8565,                   /**< RGBA8565 format */
    QUA_TDE2_COLOR_FMT_BGRA8565,                   /**< BGRA8565 format */
    QUA_TDE2_COLOR_FMT_ARGB8888,                   /**< ARGB8888 format */
    QUA_TDE2_COLOR_FMT_ABGR8888,                   /**< ABGR8888 format */
    QUA_TDE2_COLOR_FMT_RGBA8888,                   /**< RGBA8888 format */
    QUA_TDE2_COLOR_FMT_BGRA8888,                   /**< BGRA8888 format */
    QUA_TDE2_COLOR_FMT_RABG8888,                   /**<RABG8888 format*/
    QUA_TDE2_COLOR_FMT_CLUT1,                      /**CLUT1 */
    QUA_TDE2_COLOR_FMT_CLUT2,                      /**CLUT2 */
    QUA_TDE2_COLOR_FMT_CLUT4,                      /**CLUT4 */
    QUA_TDE2_COLOR_FMT_CLUT8,                      /**CLUT8 */
    QUA_TDE2_COLOR_FMT_ACLUT44,                    /**CLUT44 */
    QUA_TDE2_COLOR_FMT_ACLUT88,                    /**CLUT88 */
    QUA_TDE2_COLOR_FMT_A1,                         /**<alpha format??1bit */
    QUA_TDE2_COLOR_FMT_A8,                         /**<alpha format??8bit */
    QUA_TDE2_COLOR_FMT_YCbCr888,                   /**<YUV packet format??no alpha*/
    QUA_TDE2_COLOR_FMT_AYCbCr8888,                 /**<YUV packet format??with alpha*/
    QUA_TDE2_COLOR_FMT_YCbCr422,                   /**<YUV packet422 format */
    QUA_TDE2_COLOR_FMT_byte,                       /**<byte*/
    QUA_TDE2_COLOR_FMT_halfword,                   /**<halfword*/
    QUA_TDE2_COLOR_FMT_JPG_YCbCr400MBP,            /**<Semi-planar YUV400 format in the JPEG encoding format */
    QUA_TDE2_COLOR_FMT_JPG_YCbCr422MBHP,           /**<Semi-planar YUV422 format (half of the horizontal sampling)*/
    QUA_TDE2_COLOR_FMT_JPG_YCbCr422MBVP,           /**<Semi-planar YUV422 format (half of the vertical sampling) */
    QUA_TDE2_COLOR_FMT_MP1_YCbCr420MBP,            /**<Semi-planar YUV420 format */
    QUA_TDE2_COLOR_FMT_MP2_YCbCr420MBP,            /**<Semi-planar YUV420 format */
    QUA_TDE2_COLOR_FMT_MP2_YCbCr420MBI,            /**<Semi-planar YUV400 format */
    QUA_TDE2_COLOR_FMT_JPG_YCbCr420MBP,            /**<Semi-planar YUV400 format in the JPEG encoding format */
    QUA_TDE2_COLOR_FMT_JPG_YCbCr444MBP,            /**<Semi-planar YUV444 format */
    QUA_TDE2_COLOR_FMT_YUV420,
    QUA_TDE2_COLOR_FMT_YUV422,
    QUA_TDE2_COLOR_FMT_BUTT
} qua_tde2_color_fmt_t;

typedef struct qua_tde2_rect
{
    QUA_S32              x_pos;
    QUA_S32              y_pos;
    QUA_U32              width;
    QUA_U32              height;
} qua_tde2_rect_t;

/* Clip mode */
typedef enum qua_tde2_clip_mode
{
    QUA_TDE2_CLIPMODE_NONE = 0,
    QUA_TDE2_CLIPMODE_INSIDE,
    QUA_TDE2_CLIPMODE_OUTSIDE,
    QUA_TDE2_CLIPMODE_BUTT
} qua_tde2_clip_mode_t;

/* Type of the alpha output source */
typedef enum qua_tde2_outalpha_from
{
    QUA_TDE2_OUTALPHA_FROM_NORM = 0,               /* from the result of the alpha
                                                  blending or anti-flicker operation */
    QUA_TDE2_OUTALPHA_FROM_BACKGROUND,             /* from background */
    QUA_TDE2_OUTALPHA_FROM_FOREGROUND,             /* from foreground */
    QUA_TDE2_OUTALPHA_FROM_GLOBALALPHA,            /* from the global alpha */
    QUA_TDE2_OUTALPHA_FROM_BUTT
} qua_tde2_outalpha_from_t;

/* the anti-flicker level */
typedef enum qua_tde_deflicker_level
{
    QUA_TDE_DEFLICKER_AUTO = 0,                    /**< *<The anti-flicker coefficient is selected by the TDE*/
    QUA_TDE_DEFLICKER_LOW,                         /**< *Low-level anti-flicker*/
    QUA_TDE_DEFLICKER_MIDDLE,                      /**< *Medium-level anti-flicker*/
    QUA_TDE_DEFLICKER_HIGH,                        /**< *Intermediate-level anti-flicker*/
    QUA_TDE_DEFLICKER_BUTT
} qua_tde_deflicker_level_t;

/* Defines the alpha blending command */
/* pixel = (source * fs + destination * fd), sa = source alpha, da = destination alpha */
typedef enum qua_tde2_blendcmd
{
    QUA_TDE2_BLENDCMD_NONE = 0x0,                  /**< fs: sa      fd: 1.0-sa */
    QUA_TDE2_BLENDCMD_CLEAR,                       /**< fs: 0.0     fd: 0.0 */
    QUA_TDE2_BLENDCMD_SRC,                         /**< fs: 1.0     fd: 0.0 */
    QUA_TDE2_BLENDCMD_SRCOVER,                     /**< fs: 1.0     fd: 1.0-sa */
    QUA_TDE2_BLENDCMD_DSTOVER,                     /**< fs: 1.0-da  fd: 1.0 */
    QUA_TDE2_BLENDCMD_SRCIN,                       /**< fs: da      fd: 0.0 */
    QUA_TDE2_BLENDCMD_DSTIN,                       /**< fs: 0.0     fd: sa */
    QUA_TDE2_BLENDCMD_SRCOUT,                      /**< fs: 1.0-da  fd: 0.0 */
    QUA_TDE2_BLENDCMD_DSTOUT,                      /**< fs: 0.0     fd: 1.0-sa */
    QUA_TDE2_BLENDCMD_SRCATOP,                     /**< fs: da      fd: 1.0-sa */
    QUA_TDE2_BLENDCMD_DSTATOP,                     /**< fs: 1.0-da  fd: sa */
    QUA_TDE2_BLENDCMD_ADD,                         /**< fs: 1.0     fd: 1.0 */
    QUA_TDE2_BLENDCMD_XOR,                         /**< fs: 1.0-da  fd: 1.0-sa */
    QUA_TDE2_BLENDCMD_DST,                         /**< fs: 0.0     fd: 1.0 */
    QUA_TDE2_BLENDCMD_CONFIG,                      /**< User-defined configuration */
    QUA_TDE2_BLENDCMD_BUTT
} qua_tde2_blendcmd_t;

/* blend mode */
typedef enum qua_tde2_blend_mode
{
    QUA_TDE2_BLEND_ZERO = 0x0,
    QUA_TDE2_BLEND_ONE,
    QUA_TDE2_BLEND_SRC2COLOR,
    QUA_TDE2_BLEND_INVSRC2COLOR,
    QUA_TDE2_BLEND_SRC2ALPHA,
    QUA_TDE2_BLEND_INVSRC2ALPHA,
    QUA_TDE2_BLEND_SRC1COLOR,
    QUA_TDE2_BLEND_INVSRC1COLOR,
    QUA_TDE2_BLEND_SRC1ALPHA,
    QUA_TDE2_BLEND_INVSRC1ALPHA,
    QUA_TDE2_BLEND_SRC2ALPHASAT,
    QUA_TDE2_BLEND_BUTT
} qua_tde2_blend_mode_t;

/* the options for alpha blending */
typedef struct qua_tde2_blend_opt
{
    QUA_BOOL             global_alpha_enable;    /**< Global alpha enable */
    QUA_BOOL             pixel_alpha_enable;     /**< Pixel alpha enable */
    QUA_BOOL             src1_alpha_premulti;    /**< Src1 alpha premultiply enable */
    QUA_BOOL             src2_alpha_premulti;    /**< Src2 alpha premultiply enable */
    qua_tde2_blendcmd_t     blend_cmd;             /**< Alpha blending command */
    qua_tde2_blend_mode_t   src1_blend_mode;       /**< Src1 blending mode select. It is valid when blend_cmd is set to QUA_TDE2_BLENDCMD_CONFIG */
    qua_tde2_blend_mode_t   src2_blend_mode;       /**< Src2 blending mode select. It is valid when blend_cmd is set to QUA_TDE2_BLENDCMD_CONFIG */
} qua_tde2_blend_opt_t;

/* the attributes of the picture fill colors */
typedef struct qua_tde2_fillcolor
{
    qua_tde2_color_fmt_t    color_fmt;
    QUA_U32                 fill_color;
} qua_tde2_fillcolor_t;

/* the mirror attributes of a picture */
typedef enum qua_tde2_mirror
{
    QUA_TDE2_MIRROR_NONE = 0,
    QUA_TDE2_MIRROR_HORIZONTAL,
    QUA_TDE2_MIRROR_VERTICAL,
    QUA_TDE2_MIRROR_BOTH,
    QUA_TDE2_MIRROR_BUTT
} qua_tde2_mirror_t;

/* the operation modes for raster picture */
typedef enum qua_tde2_alucmd
{
    QUA_TDE2_ALUCMD_NONE = 0x0,                    /**< none */
    QUA_TDE2_ALUCMD_BLEND = 0x1,                   /**< Alpha blending type */
    QUA_TDE2_ALUCMD_ROP = 0x2,                     /**< rop */
    QUA_TDE2_ALUCMD_COLORIZE = 0x4,                /**< Colorize */
    QUA_TDE2_ALUCMD_BUTT = 0x8
} qua_tde2_alucmd_t;

/* the ROP type supported by the TDE */
typedef enum qua_tde2_rop_code
{
    QUA_TDE2_ROP_BLACK = 0,                        /**<Blackness*/
    QUA_TDE2_ROP_NOTMERGEPEN,                      /**<~(S2 | S1)*/
    QUA_TDE2_ROP_MASKNOTPEN,                       /**<~S2&S1*/
    QUA_TDE2_ROP_NOTCOPYPEN,                       /**< ~S2*/
    QUA_TDE2_ROP_MASKPENNOT,                       /**< S2&~S1 */
    QUA_TDE2_ROP_NOT,                              /**< ~S1 */
    QUA_TDE2_ROP_XORPEN,                           /**< S2^S1 */
    QUA_TDE2_ROP_NOTMASKPEN,                       /**< ~(S2 & S1) */
    QUA_TDE2_ROP_MASKPEN,                          /**< S2&S1 */
    QUA_TDE2_ROP_NOTXORPEN,                        /**< ~(S2^S1) */
    QUA_TDE2_ROP_NOP,                              /**< S1 */
    QUA_TDE2_ROP_MERGENOTPEN,                      /**< ~S2|S1 */
    QUA_TDE2_ROP_COPYPEN,                          /**< S2 */
    QUA_TDE2_ROP_MERGEPENNOT,                      /**< S2|~S1 */
    QUA_TDE2_ROP_MERGEPEN,                         /**< S2|S1 */
    QUA_TDE2_ROP_WHITE,                            /**< Whiteness */
    QUA_TDE2_ROP_BUTT
} qua_tde2_rop_code_t;

/* colorkey mode */
typedef enum qua_tde2_colorkey_mode
{
    QUA_TDE2_COLORKEY_MODE_NONE = 0,               /**< no color key operation*/
    QUA_TDE2_COLORKEY_MODE_FOREGROUND,             /**< Performs the colorkey operation in the foreground mode*/
    QUA_TDE2_COLORKEY_MODE_BACKGROUND,             /**< Performs the colorkey operation in the background mode*/
    QUA_TDE2_COLORKEY_MODE_BUTT
} qua_tde2_colorkey_mode_t;

/* the colorkey attributes of each color component */
typedef struct qua_tde2_colorkey_comp
{
    QUA_U8               comp_min;             /*Minimum colorkey of a component.*/
    QUA_U8               comp_max;             /*Maximum colorkey of a component.*/
    QUA_U8               comp_out;             /*The colorkey of a component is within or out of the range.*/
    QUA_U8               comp_ignore;          /*Whether to ignore a component.*/
    QUA_U8               comp_mask;            /**<Component mask*/
    QUA_U8               reserved;
    QUA_U8               reserved1;
    QUA_U8               reserved2;
} qua_tde2_colorkey_comp_t;

/* the attributes of the colorkey */
typedef union qua_tde2_colorkey
{
    struct
    {
        qua_tde2_colorkey_comp_t alpha;
        qua_tde2_colorkey_comp_t red;
        qua_tde2_colorkey_comp_t green;
        qua_tde2_colorkey_comp_t blue;
    } ck_argb;
    struct
    {
        qua_tde2_colorkey_comp_t alpha;
        qua_tde2_colorkey_comp_t y;
        qua_tde2_colorkey_comp_t cb;
        qua_tde2_colorkey_comp_t cr;
    } ck_ycbcr;
    struct
    {
        qua_tde2_colorkey_comp_t alpha;
        qua_tde2_colorkey_comp_t clut;
    } ck_clut;
} qua_tde2_colorkey_t;

/* the anti-flicker configuration of a channel */
typedef enum qua_tde2_deflicker_mode
{
    QUA_TDE2_DEFLICKER_MODE_NONE = 0,              /**< No anti-flicker */
    QUA_TDE2_DEFLICKER_MODE_RGB,                   /**< Anti-flicker on RGB component */
    QUA_TDE2_DEFLICKER_MODE_BOTH,                  /**< Anti-flicker on alpha component */
    QUA_TDE2_DEFLICKER_MODE_BUTT
} qua_tde2_deflicker_mode_t;

/* filtering mode */
typedef enum qua_tde2_filter_mode
{
    QUA_TDE2_FILTER_MODE_COLOR = 0,                /**< Filters the color */
    QUA_TDE2_FILTER_MODE_ALPHA,                    /**< Filters the alpha */
    QUA_TDE2_FILTER_MODE_BOTH,                     /* both color and alpha will be filter */
    QUA_TDE2_FILTER_MODE_NONE,                     /**<No filter *//**<CNcomment:$)A2;=xPPBK2( */
    QUA_TDE2_FILTER_MODE_BUTT
} qua_tde2_filter_mode_t;

/* CSC parameter options */
typedef struct qua_tde2_csc_opt
{
    QUA_BOOL             icsc_user_enable;       /**User-defined ICSC parameter enable*/
    QUA_BOOL             icsc_param_reload;      /**User-defined ICSC parameter reload enable*/
    QUA_BOOL             ocsc_user_enable;       /**User-defined OCSC parameter enable*/
    QUA_BOOL             ocsc_param_reload;      /**User-defined OCSC parameter reload enable*/
    QUA_U32              icsc_param_addr;      /**ICSC parameter address. The address must be 128-bit aligned.*/
    QUA_U32              ocsc_param_addr;      /**OCSC parameter address. The address must be 128-bit aligned.*/
} qua_tde2_csc_opt_t;

/* raster picture information */
typedef struct qua_tde2_surface
{
    QUA_ULONG            phy_addr;             /* the physical address of picture data */
    qua_tde2_color_fmt_t color_fmt;            /* color format */
    QUA_U32              height;               /* picture height */
    QUA_U32              width;                /* picture width */
    QUA_U32              stride;               /* picture stride */
    QUA_U8*              clut_phy_addr;        /* the physical address of Clut data */
    QUA_BOOL             ycbcr_clut;           /* if the Clut is in YCbCr space */
    QUA_BOOL             alpha_max255;         /* if the alpha max value is 255 */
    QUA_BOOL             alpha_ext_1555;       /* for ARGB1555 to extend alpha */
    QUA_U8               alpha0;
    QUA_U8               alpha1;
    QUA_ULONG            cbcr_phy_addr;        /**< CbCr address */
    QUA_U32              cbcr_stride;          /**< CbCr stride */
    QUA_BOOL             fbc_compress;         /* enable compress */
} qua_tde2_surface_t;

/* the attributes of a TDE operation*/
typedef struct qua_tde2_opt
{
    qua_tde2_alucmd_t          alu_cmd;              /* Logical operation type */
    qua_tde2_rop_code_t        rop_code_color;       /* ROP type of the color space */
    qua_tde2_rop_code_t        rop_code_alpha;       /* ROP type of the alpha */
    qua_tde2_colorkey_mode_t   color_key_mode;       /* Colorkey mode */
    qua_tde2_colorkey_t        color_key_value;      /* Colorkey value */
    qua_tde2_clip_mode_t       clip_mode;            /* Intra-area clip or entra-area clip */
    qua_tde2_rect_t            clip_rect;            /* Definition of the clipped area */
    QUA_BOOL                   deflicker;            /* Whether to deflicker */
    qua_tde2_deflicker_mode_t  deflicker_mode;       /* anti-flicker mode */
    QUA_BOOL                   resize;               /* Whether to scale */
    qua_tde2_filter_mode_t     filter_mode;          /* Filtering mode for scaling or anti-flicker */
    qua_tde2_mirror_t          mirror;               /* Mirror type */
    QUA_BOOL                   clut_reload;          /* Whether to reload the CLUT */
    QUA_U8                     global_alpha;         /* Global alpha value */
    qua_tde2_outalpha_from_t   out_alpha_from;       /* Alpha output source */
    QUA_U32                    colorize;             /**< Colorize value */
    qua_tde2_blend_opt_t       blend_opt;
    qua_tde2_csc_opt_t         csc_opt;
} qua_tde2_opt_t;

typedef struct qua_mm_tde_device_t {
    struct qua_mm_device_t parent;

    QUA_TDE_HANDLE (*qua_tde2_begin_job)();
    QUA_S32 (*qua_tde2_end_job)(QUA_TDE_HANDLE handle, QUA_BOOL sync, QUA_BOOL block, QUA_U32 timeout);
    QUA_S32 (*qua_tde2_cancel_job)(QUA_TDE_HANDLE handle);
    QUA_S32 (*qua_tde2_wait_for_done)(QUA_TDE_HANDLE handle);
    QUA_S32 (*qua_tde2_wait_all_done)();
    QUA_S32 (*qua_tde2_reset)();
    QUA_S32 (*qua_tde2_set_deflicker_level)(qua_tde_deflicker_level_t deflicker_level);
    QUA_S32 (*qua_tde2_get_deflicker_level)(qua_tde_deflicker_level_t *deflicker_level);
    QUA_S32 (*qua_tde2_set_alpha_threshold_value)(QUA_U8 threshold_value);
    QUA_S32 (*qua_tde2_get_alpha_threshold_value)(QUA_U8 *threshold_value);
    QUA_S32 (*qua_tde2_set_alpha_threshold_state)(QUA_BOOL en_alpha_threshold);
    QUA_S32 (*qua_tde2_get_alpha_threshold_state)(QUA_BOOL *en_alpha_threshold);
    QUA_S32 (*qua_tde2_quick_copy)(QUA_TDE_HANDLE handle,
                                    qua_tde2_surface_t *src, qua_tde2_rect_t *src_rect,
                                    qua_tde2_surface_t *dst, qua_tde2_rect_t *dst_rect);
    QUA_S32 (*qua_tde2_quick_fill)(QUA_TDE_HANDLE handle,
                                    qua_tde2_surface_t *dst, qua_tde2_rect_t *dst_rect,
                                    QUA_U32 fill_data);
    QUA_S32 (*qua_tde2_quick_resize)(QUA_TDE_HANDLE handle,
                                    qua_tde2_surface_t *src, qua_tde2_rect_t *src_rect,
                                    qua_tde2_surface_t *dst, qua_tde2_rect_t *dst_rect);
    QUA_S32 (*qua_tde2_quick_deflicker)(QUA_TDE_HANDLE handle,
                                    qua_tde2_surface_t *src, qua_tde2_rect_t *src_rect,
                                    qua_tde2_surface_t *dst, qua_tde2_rect_t *dst_rect);
    QUA_S32 (*qua_tde2_bitblit)(QUA_TDE_HANDLE handle,
                                    qua_tde2_surface_t *back_ground, qua_tde2_rect_t *back_ground_rect,
                                    qua_tde2_surface_t *fore_ground, qua_tde2_rect_t *fore_ground_rect,
                                    qua_tde2_surface_t *dst, qua_tde2_rect_t *dst_rect, qua_tde2_opt_t *opt);
    QUA_S32 (*qua_tde2_solid_draw)(QUA_TDE_HANDLE handle,
                                    qua_tde2_surface_t *fore_ground, qua_tde2_rect_t *fore_ground_rect,
                                    qua_tde2_surface_t *dst, qua_tde2_rect_t *dst_rect,
                                    qua_tde2_fillcolor_t *fill_color, qua_tde2_opt_t *opt);
    QUA_S32 (*qua_tde2_compress)(QUA_TDE_HANDLE handle, qua_tde2_surface_t *src, qua_tde2_surface_t *dst);
    QUA_S32 (*qua_tde2_decompress)(QUA_TDE_HANDLE handle, qua_tde2_surface_t *src, qua_tde2_surface_t *dst);
} qua_mm_tde_device_t;

#endif
