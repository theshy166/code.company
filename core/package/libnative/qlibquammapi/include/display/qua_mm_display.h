/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_MM_DISPLAY_MODULE_H__
#define __QUA_MM_DISPLAY_MODULE_H__

#include "common/qua_mm_common.h"
#include "common/qua_mm_video_common_type.h"

#define QUA_MM_DISP_DEV_VO "display.vo"
#define QUA_MM_DISP_DEV_FB "display.fb"
#define QUA_MM_DISP_DEV_VO_SPI "display.vo.spi"
#define QUA_MM_DISP_DEV_FB_SPI "display.fb.spi"

#define QUA_VO_INTF_CVBS     (0x01L<<0)
#define QUA_VO_INTF_YPBPR    (0x01L<<1)
#define QUA_VO_INTF_VGA      (0x01L<<2)
#define QUA_VO_INTF_BT656    (0x01L<<3)
#define QUA_VO_INTF_BT1120   (0x01L<<4)
#define QUA_VO_INTF_HDMI     (0x01L<<5)
#define QUA_VO_INTF_LCD      (0x01L<<6)
#define QUA_VO_INTF_BT656_H  (0x01L<<7)
#define QUA_VO_INTF_BT656_L  (0x01L<<8)
#define QUA_VO_INTF_AHD      (0x01L<<9)

#define QUA_FB_ACTIVATE_NOW 0
#define QUA_FB_ACTIVATE_NXTOPEN 1
#define QUA_FB_ACTIVATE_TEST 2
#define QUA_FB_ACTIVATE_MASK 15
#define QUA_FB_ACTIVATE_VBL 16
#define QUA_FB_CHANGE_CMAP_VBL 32
#define QUA_FB_ACTIVATE_ALL 64
#define QUA_FB_ACTIVATE_FORCE 128
#define QUA_FB_ACTIVATE_INV_MODE 256

typedef QUA_S32 QUA_VO_INTF_TYPE;
typedef QUA_S32 QUA_VO_DEV;
typedef QUA_S32 QUA_VO_LAYER;
typedef QUA_S32 QUA_VO_CHN;

typedef enum qua_display_device_type {
    QUA_DISPLAY_DEVICE_TYPE_NORMAL = 0,
    QUA_DISPLAY_DEVICE_TYPE_SPI,
    QUA_DISPLAY_DEVICE_TYPE_NUM,
} qua_display_device_type_t;

typedef enum qua_vo_intf_sync {
    QUA_VO_OUTPUT_PAL = 0,
    QUA_VO_OUTPUT_NTSC,
    QUA_VO_OUTPUT_960H_PAL,
    QUA_VO_OUTPUT_960H_NTSC,
    QUA_VO_OUTPUT_1080P24,
    QUA_VO_OUTPUT_1080P25,
    QUA_VO_OUTPUT_1080P30,
    QUA_VO_OUTPUT_720P50,
    QUA_VO_OUTPUT_720P60,
    QUA_VO_OUTPUT_1080I50,
    QUA_VO_OUTPUT_1080I60,
    QUA_VO_OUTPUT_1080P50,
    QUA_VO_OUTPUT_1080P60,
    QUA_VO_OUTPUT_576P50,
    QUA_VO_OUTPUT_480P60,
    QUA_VO_OUTPUT_640x480_60,
    QUA_VO_OUTPUT_800x480_60,
    QUA_VO_OUTPUT_800x600_60,
    QUA_VO_OUTPUT_1024x768_60,
    QUA_VO_OUTPUT_1024x600_60,
    QUA_VO_OUTPUT_1280x1024_60,
    QUA_VO_OUTPUT_1366x768_60,
    QUA_VO_OUTPUT_1440x900_60,
    QUA_VO_OUTPUT_1280x800_60,
    QUA_VO_OUTPUT_1680x1050_60,
    QUA_VO_OUTPUT_1920x2160_30,
    QUA_VO_OUTPUT_1600x1200_60,
    QUA_VO_OUTPUT_1920x1200_60,
    QUA_VO_OUTPUT_2560x1440_30,
    QUA_VO_OUTPUT_2560x1440_60,
    QUA_VO_OUTPUT_2560x1600_60,
    QUA_VO_OUTPUT_3840x2160_25,
    QUA_VO_OUTPUT_3840x2160_30,
    QUA_VO_OUTPUT_3840x2160_50,
    QUA_VO_OUTPUT_3840x2160_60,
    QUA_VO_OUTPUT_4096x2160_30,
    QUA_VO_OUTPUT_7680x4320_30_FRL_8G,
    QUA_VO_OUTPUT_USER,
    QUA_VO_OUTPUT_800x1280_60,
    QUA_VO_OUTPUT_240x320_60,
    QUA_VO_OUTPUT_480x854_60,
    QUA_VO_OUTPUT_1200x576_50,
    QUA_VO_OUTPUT_3840x2160_15,
    QUA_VO_OUTPUT_1280x960_60,
    QUA_VO_OUTPUT_720x540_60,
    QUA_VO_OUTPUT_960x1280_60,
    QUA_VO_OUTPUT_720x575_50,
    QUA_VO_OUTPUT_720x1280_60,
    QUA_VO_OUTPUT_720P25,
    QUA_VO_OUTPUT_720P30,
    QUA_VO_OUTPUT_1080x1920_60,
    QUA_VO_OUTPUT_320x240_30,
    QUA_VO_OUTPUT_BUTT
}qua_vo_intf_sync_t;

typedef enum qua_hdmi_format {
    QUA_HDMI_FORMAT_RGB888 = 0,
    QUA_HDMI_FORMAT_YUV444,
    QUA_HDMI_FORMAT_EXT_YUV422_8BIT,
    QUA_HDMI_FORMAT_EXT_YUV422_16BIT,
    QUA_HDMI_FORMAT_EXT_YUV444_24BIT,
    QUA_HDMI_FORMAT_BT_1120,
    QUA_HDMI_FORMAT_NUM,
} qua_hdmi_format_t;

typedef enum qua_vo_csc_matrix {
    QUA_VO_CSC_MATRIX_IDENTITY = 0,
    QUA_VO_CSC_MATRIX_BT601_TO_BT709,
    QUA_VO_CSC_MATRIX_BT709_TO_BT601,
    QUA_VO_CSC_MATRIX_BT601_TO_RGB_PC,
    QUA_VO_CSC_MATRIX_BT709_TO_RGB_PC,
    QUA_VO_CSC_MATRIX_RGB_TO_BT601_PC,
    QUA_VO_CSC_MATRIX_RGB_TO_BT709_PC,
    QUA_VO_CSC_MATRIX_BUTT,
} qua_vo_csc_matrix_t;

typedef enum qua_fb_dev {
    QUA_FB_DEV0 = 0,
    QUA_FB_DEV1,
    QUA_FB_DEV2,
    QUA_FB_DEV3,
    QUA_FB_DEV4,
    QUA_FB_DEV5,
    QUA_FB_DEV6,
    QUA_FB_DEV7,
    QUA_FBDEV_BUTT
} qua_fb_dev_t;

typedef enum qua_fb_buffer_mode
{
    FB_BUF_MODE_DOUBLE = 0,
    FB_BUF_MODE_ONE,
    FB_BUF_MODE_NONE,
    FB_BUF_MODE_DOUBLE_IMMEDIATE,
    FB_BUF_MODE_BUTT
} qua_fb_buffer_mode_t;

typedef enum qua_fb_layer_mask
{
    FB_LAYER_MASK_BUFMODE = 0x1,
    FB_LAYER_MASK_ANTIFLICKER_MODE = 0x2,
    FB_LAYER_MASK_POS = 0x4,
    FB_LAYER_MASK_CANVASSIZE = 0x8,
    FB_LAYER_MASK_DISPSIZE = 0x10,
    FB_LAYER_MASK_SCREENSIZE = 0x20,
    FB_LAYER_MASK_BMUL = 0x40,
    FB_LAYER_MASK_BUTT
} qua_fb_layer_mask_t;

typedef enum qua_fb_antiflicker_level
{
    FB_ANTIFLICKER_NONE = 0,
    FB_ANTIFLICKER_LOW,
    FB_ANTIFLICKER_MIDDLE,
    FB_ANTIFLICKER_HIGH,
    FB_ANTIFLICKER_AUTO,
    FB_ANTIFLICKER_BUTT
} qua_fb_antiflicker_level_t;

typedef struct qua_vo_sync_info {
    QUA_BOOL  synm;
    QUA_BOOL  iop;
    QUA_U8    intfb;

    QUA_U16   vact;
    QUA_U16   vbb;
    QUA_U16   vfb;

    QUA_U16   hact;
    QUA_U16   hbb;
    QUA_U16   hfb;
    QUA_U16   hmid;

    QUA_U16   bvact;
    QUA_U16   bvbb;
    QUA_U16   bvfb;

    QUA_U16   hpw;
    QUA_U16   vpw;

    QUA_BOOL  idv;
    QUA_BOOL  ihs;
    QUA_BOOL  ivs;

    QUA_U32   clk;
} qua_vo_sync_info_t;

typedef struct qua_user_sync_info {
    QUA_U16   width;
    QUA_U16   height;
    QUA_U16   framerate;
    QUA_U8    progressive;
    QUA_U16   Hfront;
    QUA_U16   Hsync;
    QUA_U16   Hback;
    QUA_U16   Hpol;
    QUA_U16   Vfront;
    QUA_U16   Vsync;
    QUA_U16   Vback;
    QUA_U16   Vpol;
} qua_user_sync_info_t;

typedef struct qua_vo_pub_attr {
    QUA_U32                    bg_color;
    QUA_VO_INTF_TYPE           intf_type;
    qua_vo_intf_sync_t         intf_sync;
    qua_vo_sync_info_t         sync_info;
    qua_user_sync_info_t       user_info;
} qua_vo_pub_attr_t;

typedef struct qua_vo_video_layer_attr {
    qua_rect_t      disp_rect;
    qua_size_t      image_size;
    QUA_U32         disp_frmrt;
    qua_pixel_fmt_t pix_format;
    QUA_BOOL        double_frame;
    QUA_BOOL        cluster_mode;
} qua_vo_video_layer_attr_t;

typedef struct qua_vo_compress_attr {
    QUA_BOOL support_compress;
} qua_vo_compress_attr_t;

typedef struct qua_vo_hdmi_fmt {
    qua_hdmi_format_t hdmi_fmt;
} qua_vo_hdmi_fmt_t;

typedef struct qua_vo_csc {
    qua_vo_csc_matrix_t csc_matrix;
    QUA_U32 luma;
    QUA_U32 contrast;
    QUA_U32 hue;
    QUA_U32 saturation;
} qua_vo_csc_t;

typedef struct qua_vo_chn_attr {
    QUA_U32      priority;
    qua_rect_t   rect;
    QUA_BOOL     deflicker;
} qua_vo_chn_attr_t;

typedef struct qua_aspect_radio_info {
    qua_aspect_ratio_t mode;
    QUA_U32                 bg_color;
    qua_rect_t              video_rect;
} qua_aspect_radio_info_t;

typedef struct qua_vo_chn_param_t {
    qua_aspect_radio_info_t aspect_ratio;
} qua_vo_chn_param_t;

typedef struct qua_fb_colorkey {
    QUA_BOOL     key_enable;
    QUA_U32      key_value;
} qua_fb_colorkey_t;

typedef struct qua_fb_alpha {
    QUA_BOOL    alpha_enable;
    QUA_BOOL    alpha_channel;
    QUA_U8      alpha0;
    QUA_U8      alpha1;
    QUA_U8      global_alpha;
    QUA_U8      reserved;
} qua_fb_alpha_t;

typedef struct qua_fb_fix_screeninfo {
	char id[16];
	QUA_ULONG smem_start;
	QUA_U32 smem_len;
	QUA_U32 type;
	QUA_U32 type_aux;
	QUA_U32 visual;
	QUA_U16 xpanstep;
	QUA_U16 ypanstep;
	QUA_U16 ywrapstep;
	QUA_U32 line_length;
	QUA_ULONG mmio_start;
	QUA_U32 mmio_len;
	QUA_U32 accel;
	QUA_U16 capabilities;
	QUA_U16 reserved[2];
} qua_fb_fix_screeninfo_t;

typedef struct qua_fb_bitfield {
    QUA_U32    offset;
    QUA_U32    length;
    QUA_U32    msb_right;
} qua_fb_bitfield_t;

typedef struct qua_fb_var_screeninfo {
	QUA_U32 xres;
	QUA_U32 yres;
	QUA_U32 xres_virtual;
	QUA_U32 yres_virtual;
	QUA_U32 xoffset;
	QUA_U32 yoffset;

	QUA_U32 bits_per_pixel;
	QUA_U32 grayscale;
	qua_fb_bitfield_t red;
	qua_fb_bitfield_t green;
	qua_fb_bitfield_t blue;
	qua_fb_bitfield_t transp;

	QUA_U32 nonstd;

	QUA_U32 activate;

	QUA_U32 height;
	QUA_U32 width;

	QUA_U32 accel_flags;

	QUA_U32 pixclock;
	QUA_U32 left_margin;
	QUA_U32 right_margin;
	QUA_U32 upper_margin;
	QUA_U32 lower_margin;
	QUA_U32 hsync_len;
	QUA_U32 vsync_len;
	QUA_U32 sync;
	QUA_U32 vmode;
	QUA_U32 rotate;
	QUA_U32 colorspace;
	QUA_U32 reserved[4];
} qua_fb_var_screeninfo_t;

typedef struct qua_fb_layer_info {
    qua_fb_buffer_mode_t buf_mode;
    qua_fb_antiflicker_level_t antiflicker_level;
    QUA_S32 origion_x;
    QUA_S32 origion_y;
    QUA_S32 canvas_width;
    QUA_S32 canvas_height;
    QUA_U32 display_width;
    QUA_U32 display_height;
    QUA_U32 screen_width;
    QUA_U32 screen_height;
    QUA_BOOL pre_mul;
    QUA_U32 mask;
    QUA_U32 buf_len;
    QUA_U32 buf_phys;
} qua_fb_layer_info_t;

typedef struct qua_mm_display_module_t {
    struct qua_mm_module_t parent;
} qua_mm_display_module_t;

typedef struct qua_mm_vo_device_t {
    struct qua_mm_device_t parent;

    QUA_VO_DEV dev_num;

    QUA_S32 (*set_public_attr)(QUA_VO_DEV dev_num, const qua_vo_pub_attr_t *attr);
    QUA_S32 (*get_public_attr)(QUA_VO_DEV dev_num, qua_vo_pub_attr_t *attr);
    QUA_S32 (*enable)(QUA_VO_DEV dev_num);
    QUA_S32 (*disable)(QUA_VO_DEV dev_num);
    QUA_S32 (*set_video_layer_attr)(QUA_VO_LAYER layer, const qua_vo_video_layer_attr_t *layer_attr,
        const qua_vo_compress_attr_t *compress_attr);
    QUA_S32 (*get_video_layer_attr)(QUA_VO_LAYER layer, qua_vo_video_layer_attr_t *layer_attr,
        qua_vo_compress_attr_t *compress_attr);
    QUA_S32 (*enable_video_layer)(QUA_VO_LAYER layer);
    QUA_S32 (*disable_video_layer)(QUA_VO_LAYER layer);
    QUA_S32 (*bind_video_layer)(QUA_VO_LAYER layer, QUA_VO_DEV dev_num);
    QUA_S32 (*unbind_video_layer)(QUA_VO_LAYER layer, QUA_VO_DEV dev_num);
    QUA_S32 (*set_video_layer_priority)(QUA_VO_LAYER layer, QUA_U32 priority);
    QUA_S32 (*set_video_disp_buflen)(QUA_VO_LAYER layer, QUA_U32 buflen);
    QUA_S32 (*set_hdmi_fmt)(QUA_VO_DEV dev_num, const qua_vo_hdmi_fmt_t *hdmi_fmt);
    QUA_S32 (*set_csc_matrix)(QUA_VO_LAYER layer, const qua_vo_csc_t *csc);
    QUA_S32 (*get_csc_matrix)(QUA_VO_LAYER layer, qua_vo_csc_t *csc);
    QUA_S32 (*get_display_size)(QUA_VO_DEV dev_num, qua_user_sync_info_t *user_info);
} qua_mm_vo_device_t;

typedef struct qua_mm_vo_channel_t {
    struct qua_mm_channel_t parent;

    QUA_VO_CHN chn_id;

    QUA_S32 (*set_chn_attr)(QUA_VO_LAYER layer, QUA_VO_CHN chn, const qua_vo_chn_attr_t *chn_attr);
    QUA_S32 (*get_chn_attr)(QUA_VO_LAYER layer, QUA_VO_CHN chn, qua_vo_chn_attr_t *chn_attr);
    QUA_S32 (*enable_chn)(QUA_VO_LAYER layer, QUA_VO_CHN chn);
    QUA_S32 (*disable_chn)(QUA_VO_LAYER layer, QUA_VO_CHN chn);
    QUA_S32 (*set_chn_param)(QUA_VO_LAYER layer, QUA_VO_CHN chn, const qua_vo_chn_param_t *param);
    QUA_S32 (*get_chn_param)(QUA_VO_LAYER layer, QUA_VO_CHN chn, qua_vo_chn_param_t *param);
    QUA_S32 (*set_chn_disp_pos)(QUA_VO_LAYER layer, QUA_VO_CHN chn, const qua_point_t *pos);
    QUA_S32 (*get_chn_disp_pos)(QUA_VO_LAYER layer, QUA_VO_CHN chn, qua_point_t *pos);
    QUA_S32 (*set_chn_frame_rate)(QUA_VO_LAYER layer, QUA_VO_CHN chn, QUA_S32 frame_rate);
    QUA_S32 (*get_chn_frame_rate)(QUA_VO_LAYER layer, QUA_VO_CHN chn, QUA_S32 *frame_rate);
    QUA_S32 (*get_chn_frame)(QUA_VO_LAYER layer, QUA_VO_CHN chn, qua_video_frame_info_t *frame, QUA_S32 m_sec);
    QUA_S32 (*release_chn_frame)(QUA_VO_LAYER layer, QUA_VO_CHN chn, const qua_video_frame_info_t *frame);
    QUA_S32 (*show_chn)(QUA_VO_LAYER layer, QUA_VO_CHN chn);
    QUA_S32 (*hide_chn)(QUA_VO_LAYER layer, QUA_VO_CHN chn);
    QUA_S32 (*get_chn_pts)(QUA_VO_LAYER layer, QUA_VO_CHN chn, QUA_U64 *chn_pts);
    QUA_S32 (*send_frame)(QUA_VO_LAYER layer, QUA_VO_CHN chn, const qua_video_frame_info_t *frame, QUA_S32 m_sec);
    QUA_S32 (*clear_chn_buffer)(QUA_VO_LAYER layer, QUA_VO_CHN chn, QUA_BOOL clear_all);
    QUA_S32 (*fill_chn)(QUA_VO_LAYER layer, QUA_VO_CHN chn, QUA_U32 rgb);
} qua_mm_vo_channel_t;

typedef struct qua_mm_fb_device {
    struct qua_mm_device_t parent;

    qua_fb_dev_t dev_num;
    QUA_S32 fd;

    QUA_S32 (*show)(struct qua_mm_fb_device *dev, QUA_BOOL is_show);
    QUA_S32 (*compress)(struct qua_mm_fb_device *dev, QUA_BOOL is_compress);
    QUA_S32 (*get_origin)(struct qua_mm_fb_device *dev, qua_point_t *pos);
    QUA_S32 (*put_origin)(struct qua_mm_fb_device *dev, const qua_point_t *pos);
    QUA_S32 (*get_size)(struct qua_mm_fb_device *dev, qua_size_t *screen_size);
    QUA_S32 (*put_size)(struct qua_mm_fb_device *dev, const qua_size_t *screen_size);
    QUA_S32 (*get_colorkey)(struct qua_mm_fb_device *dev, qua_fb_colorkey_t *colorkey);
    QUA_S32 (*put_colorkey)(struct qua_mm_fb_device *dev, const qua_fb_colorkey_t *colorkey);
    QUA_S32 (*get_alpha)(struct qua_mm_fb_device *dev, qua_fb_alpha_t *alpha);
    QUA_S32 (*put_alpha)(struct qua_mm_fb_device *dev, const qua_fb_alpha_t *alpha);
    QUA_S32 (*get_var_info)(struct qua_mm_fb_device *dev, qua_fb_var_screeninfo_t *var_info);
    QUA_S32 (*put_var_info)(struct qua_mm_fb_device *dev, const qua_fb_var_screeninfo_t *var_info);
    QUA_S32 (*get_fix_info)(struct qua_mm_fb_device *dev, qua_fb_fix_screeninfo_t *fix_info);
    QUA_VOID_PTR (*map)(struct qua_mm_fb_device *dev, QUA_SIZE_T size, QUA_OFF_T offset);
    QUA_VOID (*unmap)(struct qua_mm_fb_device *dev, QUA_VOID_PTR vaddr, QUA_SIZE_T size);
    QUA_S32 (*render)(struct qua_mm_fb_device *dev, const qua_fb_var_screeninfo_t *var_info);
    QUA_S32 (*bind)(struct qua_mm_fb_device *dev, QUA_VO_DEV dev_num, QUA_VO_LAYER layer);
    QUA_S32 (*unbind)(struct qua_mm_fb_device *dev, QUA_VO_DEV dev_num, QUA_VO_LAYER layer);
    QUA_S32 (*get_layer_info)(struct qua_mm_fb_device *dev, qua_fb_layer_info_t *layer_info);
    QUA_S32 (*put_layer_info)(struct qua_mm_fb_device *dev, const qua_fb_layer_info_t *layer_info);
} qua_mm_fb_device_t;

#endif
