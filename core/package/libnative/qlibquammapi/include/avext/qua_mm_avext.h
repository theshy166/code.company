/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_MM_AV_EXT_H__
#define __QUA_MM_AV_EXT_H__

#include "common/qua_mm_common.h"
#include "avext/common/qua_mm_metaformat.h"

#define QUA_MM_EXT_EXTRACTOR    "ext.extractor"
#define QUA_MM_EXT_MUXER        "ext.muxer"
#define QUA_MM_EXT_DECODER      "ext.decoder"
#define QUA_MM_EXT_ENCODER      "ext.encoder"

typedef struct qua_mm_ext_module_t {
    struct qua_mm_module_t parent;
    qua_metaformat_t* (*create_meta_format)(QUA_CHAR* name);
    QUA_S32 (*delete_meta_format)(qua_metaformat_t* meta_format);
} qua_mm_ext_module_t;

typedef struct qua_ext_muxer_chn_attr {
    qua_output_format_t format;
    QUA_CHAR file[1024];
} qua_ext_muxer_chn_attr_t;

typedef struct qua_ext_codec_chn_attr {
    QUA_CONST_CHAR* mimetype;
    QUA_VOID* mime_extra;
    QUA_VOID* param;
} qua_ext_codec_chn_attr_t;

typedef struct qua_ext_chn_attr {
    QUA_VOID_PTR *attr;
} qua_ext_chn_attr_t;

typedef struct qua_mm_extractor_device_t {
    struct qua_mm_device_t parent;

} qua_mm_extractor_device_t;

typedef struct qua_mm_extractor_channel {
    struct qua_mm_channel_t parent;
    QUA_VOID* extractor;

    QUA_SIZE_T (*track_count)(QUA_VOID* ex);
    QUA_S32 (*get_audio_track_index)(QUA_VOID* ex);
    QUA_S32 (*get_video_track_index)(QUA_VOID* ex);
    QUA_S32 (*meta_data)(QUA_VOID* ex, qua_metaformat_t** meta);
    QUA_S32 (*track_meta_data)(QUA_VOID* ex, QUA_SIZE_T track_index, qua_metaformat_t** track_meta);
    QUA_SSIZE_T (*read_sample_data)(QUA_VOID* ex, QUA_SIZE_T track_index, qua_packet_t* packet);
    QUA_S32 (*seek)(QUA_VOID* ex, QUA_U64 time_us, seek_mode_t mode);
    QUA_BOOL (*advance)(QUA_VOID* ex);
} qua_mm_extractor_channel_t;

typedef struct qua_mm_muxer_device {
    struct qua_mm_device_t parent;

} qua_mm_muxer_device_t;

typedef struct qua_mm_muxer_channel {
    struct qua_mm_channel_t parent;
    QUA_VOID* muxer;

    QUA_S32 (*track_meta_data)(QUA_VOID* mux, qua_mediastream_type_t stream_type, qua_metaformat_t* track_meta);
    QUA_S32 (*start)(QUA_VOID* mux);
    QUA_S32 (*write_packet)(QUA_VOID* mux, qua_mediastream_type_t stream_type, qua_packet_t* packet);
    QUA_S32 (*set_param)(QUA_VOID* mux, QUA_S32 key, QUA_VOID_PTR param);
    QUA_S32 (*get_param)(QUA_VOID* mux, QUA_S32 key, QUA_VOID_PTR param);
    QUA_S32 (*stop)(QUA_VOID* mux);
} qua_mm_muxer_channel_t;

typedef struct qua_mm_decoder_device_t {
    struct qua_mm_device_t parent;

} qua_mm_decoder_device_t;

typedef struct qua_mm_decoder_channel {
    struct qua_mm_channel_t parent;
    QUA_VOID* decoder;

    QUA_S32 (*start)(QUA_VOID* dec);
    QUA_S32 (*send_packet)(QUA_VOID* dec, qua_packet_t* packet);
    QUA_S32 (*receive_frame)(QUA_VOID* dec, QUA_VOID* frame);
    QUA_S32 (*query)(QUA_VOID* dec);
    QUA_S32 (*flush)(QUA_VOID* dec);
    QUA_S32 (*stop)(QUA_VOID* dec);
    QUA_S32 (*set_param)(QUA_VOID* dec, const QUA_CHAR *key, QUA_VOID_PTR param);
    QUA_S32 (*get_param)(QUA_VOID* dec, const QUA_CHAR *key, QUA_VOID_PTR param);
} qua_mm_decoder_channel_t;

typedef struct qua_mm_encoder_device {
    struct qua_mm_device_t parent;

} qua_mm_encoder_device_t;

typedef struct qua_mm_encoder_channel {
    struct qua_mm_channel_t parent;
    QUA_VOID* encoder;

    QUA_S32 (*start)(QUA_VOID* enc);
    QUA_S32 (*send_frame)(QUA_VOID* enc, QUA_VOID* frame);
    QUA_S32 (*receive_packet)(QUA_VOID* enc, qua_packet_t* packet);
    QUA_S32 (*query)(QUA_VOID* enc);
    QUA_S32 (*flush)(QUA_VOID* enc);
    QUA_S32 (*stop)(QUA_VOID* enc);
} qua_mm_encoder_channel_t;

#endif // __QUA_MM_AV_EXT_H__
