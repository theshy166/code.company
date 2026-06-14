/*
 *  Copyright (c) 2023-2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef _QUA_H264_SPS_H
#define _QUA_H264_SPS_H

#include <stdint.h>
#include <stdio.h>
#include "qua_bitread.h"
#include "qua_h264_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QUA_MAX_BR       240000 /* for level 5.1 */
#define QUA_MAX_CPB      240000 /* for level 5.1 */

#define QUA_MAX_IMUM_VALUE_OF_CPB_CNT               32
#define QUA_MAX_NUM_REF_FRAMEs_IN_POC_CYCLE         256

typedef enum qua_h264_chroma_fmt_t {
    QUA_H264_CHROMA_400                 = 0,    //!< Monochrome
    QUA_H264_CHROMA_420                 = 1,    //!< 4:2:0
    QUA_H264_CHROMA_422                 = 2,    //!< 4:2:2
    QUA_H264_CHROMA_444                 = 3     //!< 4:4:4
} qua_h264_chroma_fmt_t;

typedef struct qua_h264_hrd_t {
    uint32_t    cpb_cnt_minus1;                                  // ue(v)
    uint32_t    bit_rate_scale;                                  // u(4)
    uint32_t    cpb_size_scale;                                  // u(4)
    uint32_t    bit_rate_value_minus1[QUA_MAX_IMUM_VALUE_OF_CPB_CNT];    // ue(v)
    uint32_t    cpb_size_value_minus1[QUA_MAX_IMUM_VALUE_OF_CPB_CNT];    // ue(v)
    uint32_t    cbr_flag[QUA_MAX_IMUM_VALUE_OF_CPB_CNT];                 // u(1)
    uint32_t    initial_cpb_removal_delay_length_minus1;         // u(5)
    uint32_t    cpb_removal_delay_length_minus1;                 // u(5)
    uint32_t    dpb_output_delay_length_minus1;                  // u(5)
    uint32_t    time_offset_length;                              // u(5)
} qua_h264_hrd_t;

typedef struct qua_h264_vui_t {
    int32_t    aspect_ratio_info_present_flag;                   // u(1)
    uint32_t   aspect_ratio_idc;                                 // u(8)
    uint16_t   sar_width;                                        // u(16)
    uint16_t   sar_height;                                       // u(16)
    int32_t    overscan_info_present_flag;                       // u(1)
    int32_t    overscan_appropriate_flag;                        // u(1)
    int32_t    video_signal_type_present_flag;                   // u(1)
    uint32_t   video_format;                                     // u(3)
    int32_t    video_full_range_flag;                            // u(1)
    int32_t    colour_description_present_flag;                  // u(1)
    uint32_t   colour_primaries;                                 // u(8)
    uint32_t   transfer_characteristics;                         // u(8)
    uint32_t   matrix_coefficients;                              // u(8)
    int32_t    chroma_location_info_present_flag;                // u(1)
    uint32_t   chroma_sample_loc_type_top_field;                // ue(v)
    uint32_t   chroma_sample_loc_type_bottom_field;             // ue(v)
    int32_t    timing_info_present_flag;                         // u(1)
    uint32_t   num_units_in_tick;                                // u(32)
    uint32_t   time_scale;                                       // u(32)
    int32_t    fixed_frame_rate_flag;                            // u(1)
    int32_t    nal_hrd_parameters_present_flag;                  // u(1)
    struct qua_h264_hrd_t    nal_hrd_parameters;                        // hrd_paramters_t
    int32_t    vcl_hrd_parameters_present_flag;                  // u(1)
    struct qua_h264_hrd_t    vcl_hrd_parameters;                        // hrd_paramters_t
    // if ((nal_hrd_parameters_present_flag || (vcl_hrd_parameters_present_flag))
    int32_t    low_delay_hrd_flag;                               // u(1)
    int32_t    pic_struct_present_flag;                          // u(1)
    int32_t    bitstream_restriction_flag;                       // u(1)
    int32_t    motion_vectors_over_pic_boundaries_flag;          // u(1)
    uint32_t   max_bytes_per_pic_denom;                          // ue(v)
    uint32_t   max_bits_per_mb_denom;                            // ue(v)
    uint32_t   log2_max_mv_length_vertical;                      // ue(v)
    uint32_t   log2_max_mv_length_horizontal;                    // ue(v)
    uint32_t   num_reorder_frames;                               // ue(v)
    uint32_t   max_dec_frame_buffering;                          // ue(v)
} qua_h264_vui_t;

typedef struct qua_h264_sps_t {
    int32_t    valid;                  // indicates the parameter set is valid

    int32_t    profile_idc;                                       // u(8)
    int32_t    constrained_set0_flag;                             // u(1)
    int32_t    constrained_set1_flag;                             // u(1)
    int32_t    constrained_set2_flag;                             // u(1)
    int32_t    constrained_set3_flag;                             // u(1)
    int32_t    constrained_set4_flag;                             // u(1)
    int32_t    constrained_set5_flag;                             // u(2)

    int32_t    level_idc;                                         // u(8)
    uint32_t    seq_parameter_set_id;                              // ue(v)
    int32_t    chroma_format_idc;                                 // ue(v)

    int32_t    seq_scaling_matrix_present_flag;                   // u(1)
    int32_t    seq_scaling_list_present_flag[12];                 // u(1)
    int32_t    scaling_list_4x4[6][QUA_H264_Scaling_List_4x4_Length];       // se(v)
    int32_t    scaling_list_8x8[6][QUA_H264_Scaling_List_8x8_Length];       // se(v)
    int32_t    use_default_scaling_matrix_4x4_flag[6];
    int32_t    use_default_scaling_matrix_8x8_flag[6];

    int32_t    bit_depth_luma_minus8;                             // ue(v)
    int32_t    bit_depth_chroma_minus8;                           // ue(v)
    int32_t    log2_max_frame_num_minus4;                         // ue(v)
    int32_t    pic_order_cnt_type;
    // if( pic_order_cnt_type == 0 )
    int32_t    log2_max_pic_order_cnt_lsb_minus4;                 // ue(v)
    // else if( pic_order_cnt_type == 1 )
    int32_t    delta_pic_order_always_zero_flag;                  // u(1)
    int32_t    offset_for_non_ref_pic;                            // se(v)
    int32_t    offset_for_top_to_bottom_field;                    // se(v)
    int32_t    num_ref_frames_in_pic_order_cnt_cycle;             // ue(v)
    // for( i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )
    int32_t    offset_for_ref_frame[QUA_MAX_NUM_REF_FRAMEs_IN_POC_CYCLE];   // se(v)
    int32_t    max_num_ref_frames;                                     // ue(v)
    int32_t    gaps_in_frame_num_value_allowed_flag;             // u(1)
    int32_t    pic_width_in_mbs_minus1;                          // ue(v)
    int32_t    pic_height_in_map_units_minus1;                   // ue(v)
    int32_t    frame_mbs_only_flag;                              // u(1)
    // if( !frame_mbs_only_flag )
    int32_t    mb_adaptive_frame_field_flag;                     // u(1)
    int32_t    direct_8x8_inference_flag;                        // u(1)
    int32_t    frame_cropping_flag;                              // u(1)
    int32_t    frame_crop_left_offset;                // ue(v)
    int32_t    frame_crop_right_offset;               // ue(v)
    int32_t    frame_crop_top_offset;                 // ue(v)
    int32_t    frame_crop_bottom_offset;              // ue(v)
    int32_t    vui_parameters_present_flag;           // u(1)
    struct qua_h264_vui_t vui_seq_parameters;             // vui_seq_parameters_t
    int32_t    separate_colour_plane_flag;            // u(1)
    int32_t    max_dec_frame_buffering;
    int32_t    qpprime_y_zero_transform_bypass_flag;
    //----
    int32_t    expected_delta_per_pic_order_cnt_cycle;
} qua_h264_sps_t;

int32_t qua_parser_sps(bitread_ctx_t *p_bitctx, qua_h264_sps_t *cur_sps);

#ifdef  __cplusplus
}
#endif

#endif // _QUA_H264_SPS_H
