/*
 *  Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef _QUA_H2645_PARSE_H_
#define _QUA_H2645_PARSE_H_

#include <stdint.h>
#include <stdio.h>
#include "utils/qua_bitread.h"
#include "utils/qua_h264_pps.h"
#include "utils/qua_h264_sps.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_REORDER_TIMES   33
#define MAX_MARKING_TIMES   35

typedef enum {
    QUA_VIDEO_CODING_H264,
    QUA_VIDEO_CODING_H265,
} qua_video_type_t;

typedef enum {
    QUA_H264_NAL_UNSPECIFIED,
    QUA_H264_NAL_SLICE,
    QUA_H264_NAL_DPA,
    QUA_H264_NAL_DPB,
    QUA_H264_NAL_DPC,
    QUA_H264_NAL_IDR_SLICE,
    QUA_H264_NAL_SEI,
    QUA_H264_NAL_SPS,
    QUA_H264_NAL_PPS,
    QUA_H264_NAL_AUD,
    QUA_H264_NAL_END_SEQUENCE,
    QUA_H264_NAL_END_STREAM,
    QUA_H264_NAL_FILLER_DATA,
    QUA_H264_NAL_SPS_EXT,
    QUA_H264_NAL_PREFIX,
    QUA_H264_NAL_SUB_SPS,
    QUA_H264_NAL_DPS,
    QUA_H264_NAL_RESERVED17,
    QUA_H264_NAL_RESERVED18,
    QUA_H264_NAL_AUXILIARY_SLICE,
    QUA_H264_NAL_EXTEN_SLICE,
    QUA_H264_NAL_DEPTH_EXTEN_SLICE,
    QUA_H264_NAL_RESERVED22,
    QUA_H264_NAL_RESERVED23,
    QUA_H264_NAL_UNSPECIFIED24,
    QUA_H264_NAL_UNSPECIFIED25,
    QUA_H264_NAL_UNSPECIFIED26,
    QUA_H264_NAL_UNSPECIFIED27,
    QUA_H264_NAL_UNSPECIFIED28,
    QUA_H264_NAL_UNSPECIFIED29,
    QUA_H264_NAL_UNSPECIFIED30,
    QUA_H264_NAL_UNSPECIFIED31
} qua_h264_nal_type_t;

typedef enum {
    QUA_HEVC_NAL_TRAIL_N,
    QUA_HEVC_NAL_TRAIL_R,
    QUA_HEVC_NAL_TSA_N,
    QUA_HEVC_NAL_TSA_R,
    QUA_HEVC_NAL_STSA_N,
    QUA_HEVC_NAL_STSA_R,
    QUA_HEVC_NAL_RADL_N,
    QUA_HEVC_NAL_RADL_R,
    QUA_HEVC_NAL_RASL_N,
    QUA_HEVC_NAL_RASL_R,
    QUA_HEVC_NAL_VCL_N10,
    QUA_HEVC_NAL_VCL_R11,
    QUA_HEVC_NAL_VCL_N12,
    QUA_HEVC_NAL_VCL_R13,
    QUA_HEVC_NAL_VCL_N14,
    QUA_HEVC_NAL_VCL_R15,
    QUA_HEVC_NAL_BLA_W_LP,
    QUA_HEVC_NAL_BLA_W_RADL,
    QUA_HEVC_NAL_BLA_N_LP,
    QUA_HEVC_NAL_IDR_W_RADL,
    QUA_HEVC_NAL_IDR_N_LP,
    QUA_HEVC_NAL_CRA_NUT,
    QUA_HEVC_NAL_RSV_IRAP_VCL22,
    QUA_HEVC_NAL_RSV_IRAP_VCL23,
    QUA_HEVC_NAL_RSV_VCL24,
    QUA_HEVC_NAL_RSV_VCL25,
    QUA_HEVC_NAL_RSV_VCL26,
    QUA_HEVC_NAL_RSV_VCL27,
    QUA_HEVC_NAL_RSV_VCL28,
    QUA_HEVC_NAL_RSV_VCL29,
    QUA_HEVC_NAL_RSV_VCL30,
    QUA_HEVC_NAL_RSV_VCL31,
    QUA_HEVC_NAL_VPS,
    QUA_HEVC_NAL_SPS,
    QUA_HEVC_NAL_PPS,
    QUA_HEVC_NAL_AUD,
    QUA_HEVC_NAL_EOS_NUT,
    QUA_HEVC_NAL_EOB_NUT,
    QUA_HEVC_NAL_FD_NUT,
    QUA_HEVC_NAL_SEI_PREFIX,
    QUA_HEVC_NAL_SEI_SUFFIX,
    QUA_HEVC_NAL_RSV_NVCL41,
    QUA_HEVC_NAL_RSV_NVCL42,
    QUA_HEVC_NAL_RSV_NVCL43,
    QUA_HEVC_NAL_RSV_NVCL44,
    QUA_HEVC_NAL_RSV_NVCL45,
    QUA_HEVC_NAL_RSV_NVCL46,
    QUA_HEVC_NAL_RSV_NVCL47,
    QUA_HEVC_NAL_UNSPEC48,
    QUA_HEVC_NAL_UNSPEC49,
    QUA_HEVC_NAL_UNSPEC50,
    QUA_HEVC_NAL_UNSPEC51,
    QUA_HEVC_NAL_UNSPEC52,
    QUA_HEVC_NAL_UNSPEC53,
    QUA_HEVC_NAL_UNSPEC54,
    QUA_HEVC_NAL_UNSPEC55,
    QUA_HEVC_NAL_UNSPEC56,
    QUA_HEVC_NAL_UNSPEC57,
    QUA_HEVC_NAL_UNSPEC58,
    QUA_HEVC_NAL_UNSPEC59,
    QUA_HEVC_NAL_UNSPEC60,
    QUA_HEVC_NAL_UNSPEC61,
    QUA_HEVC_NAL_UNSPEC62,
    QUA_HEVC_NAL_UNSPEC63
} qua_h265_nal_type_t;

typedef enum {
    LIST_0,
    LIST_1
} qua_ref_list_t;

typedef struct {
    uint32_t first_mb_in_slice;
    int32_t slice_type;
    uint32_t pic_parameter_set_id;
    uint32_t colour_plane_id;
    uint32_t frame_num;
    int32_t field_pic_flag;
    int32_t bottom_field_flag;
    int32_t structure;
    int32_t mb_aff_frame_flag;
    int32_t pic_order_cnt_lsb;
    int32_t delta_pic_order_cnt_bottom;
    int32_t delta_pic_order_cnt[2];
    uint32_t poc_used_bitlen;
    int32_t redundant_pic_cnt;
    int32_t direct_spatial_mv_pred_flag;
    int32_t num_ref_idx_active[2];
    int32_t num_ref_idx_override_flag;
    int32_t ref_pic_list_reordering_flag[2];
    int32_t reordering_of_pic_nums_idc[2][MAX_REORDER_TIMES];
    int32_t abs_diff_pic_num_minus1[2][MAX_REORDER_TIMES];
    int32_t long_term_pic_idx[2][MAX_REORDER_TIMES];
    uint32_t drpm_used_bitlen;
    int32_t no_output_of_prior_pics_flag;
    int32_t long_term_refrence_flag;
    int32_t adaptive_ref_pic_buffering_flag;

    qua_h264_sps_t active_sps;
    qua_h264_pps_t active_pps;
    uint32_t RefPicListX[2][MAX_REORDER_TIMES];
    uint32_t refIdxLX[2];
} qua_h264_slice_t;

typedef struct {
    union {
        qua_h264_slice_t h264_slice;
    };
} qua_slice_t;

typedef struct {
    uint8_t forbidden_zero_bit;
    uint8_t nal_ref_idc;
    uint8_t nal_unit_type;
} qua_h264_nalu_t;

typedef struct {
    uint8_t forbidden_zero_bit;
    uint8_t nal_unit_type;
    uint8_t nuh_layer_id;
    uint8_t nuh_temporal_id_plus1;
} qua_h265_nalu_t;

typedef struct {
    bitread_ctx_t bit_ctx;
    union {
        qua_h264_nalu_t h264_nalu;
        qua_h265_nalu_t h265_nalu;
    };
} qua_nalu_t;

int32_t qua_h2645_extract_nalu(uint8_t *data, size_t size, size_t *nalSize);
int32_t qua_h2645_split_codec_nal(uint8_t *data, size_t size, size_t *nalSize);
int32_t qua_parse_nal_header(qua_video_type_t type, uint8_t *nal);
int32_t qua_parse_one_nalu(qua_video_type_t type, uint8_t *nal_data, uint32_t nalu_size, qua_nalu_t *nalu);
int32_t qua_parse_slice_header(qua_video_type_t type, qua_nalu_t *nalu, qua_slice_t *slice);
void check_refer_picture_lists(qua_slice_t *slice, int32_t *slice_num_pic_refs);

const char *qua_hevc_nal_unit_name(int nal_type);
const char *qua_h264_nal_unit_name(int nal_type);

#ifdef __cplusplus
}
#endif

#endif
