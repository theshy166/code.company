/*
 *  Copyright (c) 2023~2025 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_AV_COMMON_H__
#define __QUA_AV_COMMON_H__

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QUA_AV_PKT_FLAG_KEY         0x0001
#define QUA_AV_PKT_FLAG_CORRUPT     0x0002
#define QUA_AV_PKT_FLAG_DISCARD     0x0004
#define QUA_AV_PKT_FLAG_TRUSTED     0x0008
#define QUA_AV_PKT_FLAG_DISPOSABLE  0x0010

#define QUA_AV_FRAME_FLAG_CORRUPT            (1 << 0)
#define QUA_AV_FRAME_FLAG_KEY                (1 << 1)
#define QUA_AV_FRAME_FLAG_DISCARD            (1 << 2)
#define QUA_AV_FRAME_FLAG_INTERLACED         (1 << 3)
#define QUA_AV_FRAME_FLAG_TOP_FIELD_FIRST    (1 << 4)

#define QUA_EXTRADATA_MAXSIZE    (1024*5)

typedef enum seek_mode {
    SEEK_PREVIOUS_SYNC = 0,
    SEEK_NEXT_SYNC = 1,
    SEEK_CLOSEST_SYNC = 2,
    SEEK_CLOSEST = 3,
    SEEK_FRAME_INDEX = 4,
    NONBLOCKING = 5,
} seek_mode_t;

typedef enum qua_aac_transtype {
    QUA_TT_UNKNOWN = -1,
    QUA_TT_MP4_RAW = 0,
    QUA_TT_MP4_ADIF = 1,
    QUA_TT_MP4_ADTS = 2,
    QUA_TT_MP4_LATM_MCP1 = 6,
    QUA_TT_MP4_LATM_MCP0 = 7,
    QUA_TT_MP4_LOAS = 10,
    QUA_TT_DRM = 12,
} qua_aac_transtype_t;

typedef enum qua_sample_format {
    QUA_SAMPLE_FMT_NONE = -1,
    QUA_SAMPLE_FMT_U8,          ///< unsigned 8 bits
    QUA_SAMPLE_FMT_S16,         ///< signed 16 bits
    QUA_SAMPLE_FMT_S32,         ///< signed 32 bits
    QUA_SAMPLE_FMT_FLT,         ///< float
    QUA_SAMPLE_FMT_DBL,         ///< double

    // below unsupport with plannar
    QUA_SAMPLE_FMT_U8P,         ///< unsigned 8 bits, planar
    QUA_SAMPLE_FMT_S16P,        ///< signed 16 bits, planar
    QUA_SAMPLE_FMT_S32P,        ///< signed 32 bits, planar
    QUA_SAMPLE_FMT_FLTP,        ///< float, planar
    QUA_SAMPLE_FMT_DBLP,        ///< double, planar
    QUA_SAMPLE_FMT_S64,         ///< signed 64 bits
    QUA_SAMPLE_FMT_S64P,        ///< signed 64 bits, planar

    QUA_SAMPLE_FMT_NB           ///< Number of sample formats. DO NOT USE if linking dynamically
} qua_sample_format_t;

typedef enum qua_mediastream_type {
    QUA_STREAM_NULL          = -1,
    QUA_STREAM_VIDEO         = 0,
    QUA_STREAM_AUDIO         = 1,
    QUA_STREAM_IMAGE_JPEG    = 2,
    QUA_STREAM_MOTION_JPEG   = 3,
    QUA_STREAM_IMAGE_PNG     = 4,
} qua_mediastream_type_t;

typedef enum qua_output_format {
    QUA_OUTPUT_FORMAT_DEFAULT     = 0,
    QUA_OUTPUT_FORMAT_THREE_GPP   = 1,
    QUA_OUTPUT_FORMAT_MPEG_4      = 2,
    QUA_OUTPUT_FORMAT_AAC_ADTS    = 3,
    QUA_OUTPUT_FORMAT_MPEG2TS     = 4,
    QUA_OUTPUT_FORMAT_MP3         = 5,
    QUA_OUTPUT_FORMAT_MAX,
} qua_output_format_t;

typedef enum qua_png_output_format {
    QUA_PNG_OUTPUT_FMT_RGBA8 = 1,
    QUA_PNG_OUTPUT_FMT_RGBA16 = 2,
    QUA_PNG_OUTPUT_FMT_RGB8 = 4,

    QUA_PNG_OUTPUT_FMT_PNG = 256,
} qua_png_output_format_t;

typedef enum qua_png_color_type {
    QUA_PNG_COLOR_TYPE_GRAYSCALE = 0,
    QUA_PNG_COLOR_TYPE_TRUECOLOR = 2,
    QUA_PNG_COLOR_TYPE_INDEXED = 3,
    QUA_PNG_COLOR_TYPE_GRAYSCALE_ALPHA = 4,
    QUA_PNG_COLOR_TYPE_TRUECOLOR_ALPHA = 6
} qua_png_color_type_t;

typedef enum qua_png_param_key {
    QUA_PNG_PARAM_RESOLUTION = 0x1,
    QUA_PNG_PARAM_IMAGE_SIZE = 0x2,
    QUA_PNG_PARAM_COLOR_TYPE = 0x3,
    QUA_PNG_PARAM_BIT_DEPTH  = 0x4,
    QUA_PNG_PARAM_OUTPUT_FMT = 0x5,
    QUA_PNG_PARAM_BLOCK_NUM  = 0x6,
} qua_png_param_key_t;

typedef struct qua_packet {
    uint8_t* data;
    size_t offset;
    int size;
    int capacity;
    uint64_t pts;
    uint64_t dts;
    uint64_t duration;
    uint64_t baseTime;
    int flags;
    size_t streamIndex;
    int end_of_stream;
} qua_packet_t;

typedef struct qua_audio_config {
    int in_nb_channels;
    int in_sample_rate;
    int in_sample_fmt;
    int out_nb_channels;
    int out_sample_rate;
    int out_sample_fmt;
    int nb_samples;
    int bit_rate;
    int bits_per_coded_sample;
    int block_align;
    int profile;
    int level;
    uint8_t extra_data[QUA_EXTRADATA_MAXSIZE];
    int extra_data_size;
    qua_aac_transtype_t trans_type;
} qua_audio_config_t;

typedef struct qua_png_config {
    uint32_t width;
    uint32_t height;
    uint8_t color_type;
    uint8_t bit_depth;
    FILE *png;
} qua_png_config_t;

typedef struct qua_video_config {
    int width;
    int height;
    int fps;
    int bitrate;
    int codec_id;
    int pix_fmt;
    int profile;
    int level;
    uint8_t extra_data[QUA_EXTRADATA_MAXSIZE];
    int extra_data_size;
} qua_video_config_t;

typedef struct qua_audio_frame_ext {
    uint8_t *data;
    int offset;
    int size;
    int capacity;
    int nb_samples;
    int sample_fmt;
    int sample_rate;
    int nb_channels;
    int flags;
    uint64_t pts;
} qua_audio_frame_ext_t;

typedef struct qua_video_frame_ext {
    int width;
    int height;
    uint8_t *data[3];
    int linesize[3];
    uint64_t pts;
    uint64_t dts;
    int pix_fmt;
    int eos;
} qua_video_frame_ext_t;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __QUA_AV_COMMON_H__