/*
 * Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
 *
 * All Rights Reserved.
 * Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_MM_PARAM_H__
#define __QUA_MM_PARAM_H__

#include "common/qua_type.h"
#include "common/qua_mm_video_common_type.h"

typedef enum quamm_param_key {
    // media common params
    QUAMM_PARAM_MEDIA_Resolution = 0x100,    // see qua_size_t or quamm_size_t
    QUAMM_PARAM_MEDIA_ContainerFormat,       // see qua_container_fmt_t
    QUAMM_PARAM_MEDIA_FilePath,              // a string
    QUAMM_PARAM_MEDIA_FileDate,              // a string
    QUAMM_PARAM_MEDIA_CycleMode,             // QUA_S8, 1-on, 0-off
    QUAMM_PARAM_MEDIA_CurrentTime,           // QUA_U64, msec
    QUAMM_PARAM_MEDIA_Duration,              // QUA_U16, msec
    QUAMM_PARAM_MEDIA_Orientation,           // see qua_orientation_t
    QUAMM_PARAM_MEDIA_ChannelId,             // QUA_S8
    QUAMM_PARAM_MEDIA_SeekTimeMs,            // QUA_U16
    QUAMM_PARAM_MEDIA_BufDepth,              // QUA_S8
    QUAMM_PARAM_MEDIA_Rect,                  // see qua_rect_t
    QUAMM_PARAM_MEDIA_FrameRate,             // QUA_S8
    QUAMM_PARAM_MEDIA_Bitrate,               // QUA_U16
    QUAMM_PARAM_MEDIA_ROISetting,            // see qua_media_roi_setting_t
    QUAMM_PARAM_MEDIA_SpeedControl,          // QUA_FLOAT
    QUAMM_PARAM_MEDIA_CallingPid,            // QUA_S32
    QUAMM_PARAM_MEDIA_CallingUid,            // QUA_S32
    // video params
    QUAMM_PARAM_VIDEO_Source = 0x200,        // see qua_video_source_t
    QUAMM_PARAM_VIDEO_PixelFormat,           // see qua_pixel_fmt_t
    QUAMM_PARAM_VIDEO_CodecFormat,           // see qua_video_codec_fmt_t
    QUAMM_PARAM_VIDEO_Profile,               // see qua_video_profile_t
    QUAMM_PARAM_VIDEO_Level,                 // QUA_S8
    QUAMM_PARAM_VIDEO_MakeIFrame,            // no payload
    QUAMM_PARAM_VIDEO_IFrameInterval,        // QUA_U8, the number of frame between two adjacent I frames
    QUAMM_PARAM_VIDEO_GOPLength,             // QUA_U8, the number of frame within a GOP interval
    QUAMM_PARAM_VIDEO_RateControlMode,       // see qua_video_rc_mode_t
    QUAMM_PARAM_VIDEO_QuantizationSetting,   // see qua_video_quanti_setting_t
    QUAMM_PARAM_VIDEO_RefFrames,             // QUA_U8
    QUAMM_PARAM_VIDEO_EnableBFrames,         // QUA_S8, 1-enable, 0-disable
    QUAMM_PARAM_VIDEO_Quality,               // see qua_video_quality_t
    QUAMM_PARAM_VIDEO_SmartCoding,           // QUA_S8, 1-enable, 0-disable

    // audio params
    QUAMM_PARAM_AUDIO_Source = 0x300,        // see qua_audio_source_t
    QUAMM_PARAM_AUDIO_PcmFormat,             // see qua_pcm_fmt_t
    QUAMM_PARAM_AUDIO_CodecFormat,           // see qua_audio_codec_fmt_t
    QUAMM_PARAM_AUDIO_Profile,               // see qua_audio_profile_t
    QUAMM_PARAM_AUDIO_SampleRate,            // QUA_S16
    QUAMM_PARAM_AUDIO_BitDepth,              // see qua_audio_bit_depth_t
    QUAMM_PARAM_AUDIO_Channels,              // see qua_audio_channel_t
    QUAMM_PARAM_AUDIO_Volume,                // QUA_S8
    QUAMM_PARAM_AUDIO_VolumeType,            // see qua_volume_type_t
    QUAMM_PARAM_AUDIO_IOType,                // see qua_audio_iotype_t
    QUAMM_PARAM_AUDIO_Mute,                  // QUA_S8, 1-on, 0-off
    QUAMM_PARAM_AUDIO_Enable,                // QUA_S8, 1-on, 0-off
    QUAMM_PARAM_AUDIO_MakeCSD,               // no payload
    QUAMM_PARAM_AUDIO_FrameSize,             // QUA_U8
    QUAMM_PARAM_AUDIO_FrameCnt,              // QUA_U8

    // camera params
    QUAMM_PARAM_CAMERA_BrightnessLevel = 0x400,  // QUA_U8
    QUAMM_PARAM_CAMERA_Zoom,                     // QUA_FLOAT
    QUAMM_PARAM_CAMERA_ContrastLevel,            // QUA_U8
    QUAMM_PARAM_CAMERA_SharpnessLevel,           // QUA_U8
    QUAMM_PARAM_CAMERA_ExposureDuration,         // QUA_U16, msec
    QUAMM_PARAM_CAMERA_AutoFocus,                // QUA_S8, 1-on, 0-off
    QUAMM_PARAM_CAMERA_FocusMode,                // see qua_camera_focus_mode_t
    QUAMM_PARAM_CAMERA_HDRSetting,               // QUA_S8, 1-on, 0-off
    QUAMM_PARAM_CAMERA_Id,                       // QUA_S8

    // image params
    QUAMM_PARAM_IMAGE_CodecFormat = 0x500,       // see qua_image_codec_fmt_t
    QUAMM_PARAM_IMAGE_JpegQuality,               // see qua_jpeg_quality_t
    QUAMM_PARAM_IMAGE_EnableThumbnail,           // QUA_S8, 1-on, 0-off
    QUAMM_PARAM_IMAGE_EnableCallback,            // QUA_S8, 1-on, 0-off
    QUAMM_PARAM_IMAGE_RequestCapture,            // no payload

    // display params
    QUAMM_PARAM_DISPLAY_CanvasSize = 0x600,      // qua_size_t

    // MPP specific params
    QUAMM_PARAM_MPP_VpuViMode = 0x700,           // see qua_vpu_vi_mode_t
} quamm_param_key_t;

typedef enum quamm_direction {
    QUAMM_DIRECTION_IN,
    QUAMM_DIRECTION_OUT,
    QUAMM_DIRECTION_IN_OUT
} quamm_direction_t;

typedef struct quamm_param {
    quamm_param_key_t key;
    QUA_U16 len;
    QUA_VOID *data;
} quamm_param_t;

typedef struct quamm_size {
    qua_size_t size;
    quamm_direction_t direction;
} quamm_size_t;

#endif

