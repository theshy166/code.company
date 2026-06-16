/*
 *  Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_H264_PPS_H__
#define __QUA_H264_PPS_H__

#include <stdint.h>
#include <stdio.h>
#include "qua_bitread.h"
#include "qua_h264_common.h"
#include "qua_h264_sps.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QUA_MAX_NUM_SLICE_GROUP_MINUS1      8

typedef struct qua_h264_pps_t {
    int32_t     valid;                  // indicates the parameter set is valid
    uint32_t    pic_parameter_set_id;                             // ue(v)
    uint32_t    seq_parameter_set_id;                             // ue(v)
    int32_t     entropy_coding_mode_flag;                            // u(1)
    int32_t     transform_8x8_mode_flag;                             // u(1)

    int32_t     pic_scaling_matrix_present_flag;                     // u(1)
    int32_t     pic_scaling_list_present_flag[12];                   // u(1)
    int32_t     ScalingList4x4[6][QUA_H264_Scaling_List_4x4_Length];                               // se(v)
    int32_t     ScalingList8x8[6][QUA_H264_Scaling_List_8x8_Length];                               // se(v)
    int32_t     UseDefaultScalingMatrix4x4Flag[6];
    int32_t     UseDefaultScalingMatrix8x8Flag[6];

    // if( pic_order_cnt_type < 2 )  in the sequence parameter set
    int32_t     bottom_field_pic_order_in_frame_present_flag;                           // u(1)
    int32_t     num_slice_groups_minus1;                          // ue(v)
    int32_t     slice_group_map_type;                             // ue(v)
    // if( slice_group_map_type = = 0 )
    int32_t     run_length_minus1[QUA_MAX_NUM_SLICE_GROUP_MINUS1];    // ue(v)
    // else if( slice_group_map_type = = 2 )
    int32_t     top_left[QUA_MAX_NUM_SLICE_GROUP_MINUS1];             // ue(v)
    int32_t     bottom_right[QUA_MAX_NUM_SLICE_GROUP_MINUS1];         // ue(v)
    // else if( slice_group_map_type = = 3 || 4 || 5
    int32_t     slice_group_change_direction_flag;                // u(1)
    int32_t     slice_group_change_rate_minus1;                   // ue(v)
    // else if( slice_group_map_type = = 6 )
    int32_t     pic_size_in_map_units_minus1;                     // ue(v)
    uint8_t     *slice_group_id;                                  // complete MBAmap u(v)

    int32_t     num_ref_idx_l0_default_active_minus1;             // ue(v)
    int32_t     num_ref_idx_l1_default_active_minus1;             // ue(v)
    int32_t     weighted_pred_flag;                               // u(1)
    int32_t     weighted_bipred_idc;                              // u(2)
    int32_t     pic_init_qp_minus26;                              // se(v)
    int32_t     pic_init_qs_minus26;                              // se(v)
    int32_t     chroma_qp_index_offset;                           // se(v)

    int32_t     cb_qp_index_offset;                               // se(v)
    int32_t     cr_qp_index_offset;                               // se(v)
    int32_t     second_chroma_qp_index_offset;                    // se(v)

    int32_t     deblocking_filter_control_present_flag;           // u(1)
    int32_t     constrained_intra_pred_flag;                      // u(1)
    int32_t     redundant_pic_cnt_present_flag;                   // u(1)
    int32_t     vui_pic_parameters_flag;                          // u(1)
    int32_t     scaleing_list_enable_flag;
    uint32_t    scaleing_list_address;
} qua_h264_pps_t;


int32_t qua_parser_pps(bitread_ctx_t *p_bitctx, qua_h264_sps_t *cur_sps, qua_h264_pps_t *cur_pps);

#ifdef  __cplusplus
}
#endif

#endif // __QUA_H264_PPS_H__

