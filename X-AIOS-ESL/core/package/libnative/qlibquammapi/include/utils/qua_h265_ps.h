/*
 *  Copyright (c) 2023-2025 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef _QUA_H265_PS_H_
#define _QUA_H265_PS_H_

#include <stdint.h>
#include <stdio.h>
#include "common/qua_type.h"
#include "qua_bitread.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LOG2_CTB_SIZE 6
#define MAX_SUB_LAYERS 7
#define MAX_SHORT_TERM_RPS_COUNT 64
#define MAX_VPS_COUNT 16
#define MAX_SPS_COUNT 16
#define MAX_DPB_SIZE 17 // A.4.1
#define LOG2_MAX_CU_SIZE    6
#define LOG2_MIN_CU_SIZE    3
#define LOG2_MAX_TU_SIZE    5
#define LOG2_MIN_TU_SIZE    2

#define QUA_ARRAY_ELEMS(a)      (sizeof(a) / sizeof((a)[0]))

typedef struct qua_short_term_rps {
    uint32_t num_negative_pics;
    int32_t num_delta_pocs;
    int32_t rps_idx_num_delta_pocs;
    int32_t delta_poc[32];
    uint8_t used[32];
} qua_short_term_rps_t;

typedef struct qua_hevc_window {
    int32_t left_offset;
    int32_t right_offset;
    int32_t top_offset;
    int32_t bottom_offset;
} qua_hevc_window_t;

/*
 * Chromaticity coordinates of the source primaries.
 */
typedef enum {
    QUA_FRAME_PRI_RESERVED0     = 0,
    QUA_FRAME_PRI_BT709         = 1,    ///< also ITU-R BT1361 / IEC 61966-2-4 / SMPTE RP177 Annex B
    QUA_FRAME_PRI_UNSPECIFIED   = 2,
    QUA_FRAME_PRI_RESERVED      = 3,
    QUA_FRAME_PRI_BT470M        = 4,    ///< also FCC Title 47 Code of Federal Regulations 73.682 (a)(20)

    QUA_FRAME_PRI_BT470BG       = 5,    ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM
    QUA_FRAME_PRI_SMPTE170M     = 6,    ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC/SMPTE ST 170 (2004)
    QUA_FRAME_PRI_SMPTE240M     = 7,    ///< functionally identical to above/SMPTE ST 240
    QUA_FRAME_PRI_FILM          = 8,    ///< colour filters using Illuminant C
    QUA_FRAME_PRI_BT2020        = 9,    ///< ITU-R BT2020 / ITU-R BT.2100-2
    QUA_FRAME_PRI_SMPTEST428_1  = 10,   ///< SMPTE ST 428-1 (CIE 1931 XYZ)
    QUA_FRAME_PRI_SMPTE431      = 11,   ///< SMPTE ST 431-2 (2011) / DCI P3
    QUA_FRAME_PRI_SMPTE432      = 12,   ///< SMPTE ST 432-1 (2010) / P3 D65 / Display P3
    QUA_FRAME_PRI_JEDEC_P22     = 22,   ///< JEDEC P22 phosphors
    QUA_FRAME_PRI_NB,                   ///< Not part of ABI
} qua_frame_color_primaries_t;

/*
 * Color Transfer Characteristic.
 */
typedef enum {
    QUA_FRAME_TRC_RESERVED0    = 0,
    QUA_FRAME_TRC_BT709        = 1,     ///< also ITU-R BT1361
    QUA_FRAME_TRC_UNSPECIFIED  = 2,
    QUA_FRAME_TRC_RESERVED     = 3,
    QUA_FRAME_TRC_GAMMA22      = 4,     ///< also ITU-R BT470M / ITU-R BT1700 625 PAL & SECAM
    QUA_FRAME_TRC_GAMMA28      = 5,     ///< also ITU-R BT470BG
    QUA_FRAME_TRC_SMPTE170M    = 6,     ///< also ITU-R BT601-6 525 or 625 / ITU-R BT1358 525 or 625 / ITU-R BT1700 NTSC
    QUA_FRAME_TRC_SMPTE240M    = 7,
    QUA_FRAME_TRC_LINEAR       = 8,     ///< "Linear transfer characteristics"
    QUA_FRAME_TRC_LOG          = 9,     ///< "Logarithmic transfer characteristic (100:1 range)"
    QUA_FRAME_TRC_LOG_SQRT     = 10,    ///< "Logarithmic transfer characteristic (100 * Sqrt(10) : 1 range)"
    QUA_FRAME_TRC_IEC61966_2_4 = 11,    ///< IEC 61966-2-4
    QUA_FRAME_TRC_BT1361_ECG   = 12,    ///< ITU-R BT1361 Extended Colour Gamut
    QUA_FRAME_TRC_IEC61966_2_1 = 13,    ///< IEC 61966-2-1 (sRGB or sYCC)
    QUA_FRAME_TRC_BT2020_10    = 14,    ///< ITU-R BT2020 for 10 bit system
    QUA_FRAME_TRC_BT2020_12    = 15,    ///< ITU-R BT2020 for 12 bit system
    QUA_FRAME_TRC_SMPTEST2084  = 16,    ///< SMPTE ST 2084 for 10-, 12-, 14- and 16-bit systems
    QUA_FRAME_TRC_SMPTEST428_1 = 17,    ///< SMPTE ST 428-1
    QUA_FRAME_TRC_ARIB_STD_B67 = 18,    ///< ARIB STD-B67, known as "Hybrid log-gamma"
    QUA_FRAME_TRC_NB,                   ///< Not part of ABI
} qua_frame_color_transfer_characteristic_t;

/*
 * YUV colorspace type.
 */
typedef enum {
    QUA_FRAME_SPC_RGB         = 0,      ///< order of coefficients is actually GBR, also IEC 61966-2-1 (sRGB)
    QUA_FRAME_SPC_BT709       = 1,      ///< also ITU-R BT1361 / IEC 61966-2-4 xvYCC709 / SMPTE RP177 Annex B
    QUA_FRAME_SPC_UNSPECIFIED = 2,
    QUA_FRAME_SPC_RESERVED    = 3,
    QUA_FRAME_SPC_FCC         = 4,      ///< FCC Title 47 Code of Federal Regulations 73.682 (a)(20)
    QUA_FRAME_SPC_BT470BG     = 5,      ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM / IEC 61966-2-4 xvYCC601
    QUA_FRAME_SPC_SMPTE170M   = 6,      ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC / functionally identical to above
    QUA_FRAME_SPC_SMPTE240M   = 7,
    QUA_FRAME_SPC_YCOCG       = 8,      ///< Used by Dirac / VC-2 and H.264 FRext, see ITU-T SG16
    QUA_FRAME_SPC_BT2020_NCL  = 9,      ///< ITU-R BT2020 non-constant luminance system
    QUA_FRAME_SPC_BT2020_CL   = 10,     ///< ITU-R BT2020 constant luminance system
    QUA_FRAME_SPC_SMPTE2085   = 11,     ///< SMPTE 2085, Y'D'zD'x
    QUA_FRAME_SPC_CHROMA_DERIVED_NCL = 12,  ///< Chromaticity-derived non-constant luminance system
    QUA_FRAME_SPC_CHROMA_DERIVED_CL = 13,   ///< Chromaticity-derived constant luminance system
    QUA_FRAME_SPC_ICTCP       = 14,     ///< ITU-R BT.2100-0, ICtCp
    QUA_FRAME_SPC_NB,                   ///< Not part of ABI
} qua_frame_Color_space_t;

typedef struct qua_rational_t {
    int32_t num; ///< numerator
    int32_t den; ///< denominator
} qua_rational_t;

typedef struct qua_vui_t {
    qua_rational_t sar;

    int32_t overscan_info_present_flag;
    int32_t overscan_appropriate_flag;

    int32_t video_signal_type_present_flag;
    int32_t video_format;
    int32_t video_full_range_flag;
    int32_t colour_description_present_flag;
    uint8_t colour_primaries;
    uint8_t transfer_characteristic;
    uint8_t matrix_coeffs;

    int32_t chroma_loc_info_present_flag;
    int32_t chroma_sample_loc_type_top_field;
    int32_t chroma_sample_loc_type_bottom_field;
    int32_t neutra_chroma_indication_flag;

    int32_t field_seq_flag;
    int32_t frame_field_info_present_flag;

    int32_t default_display_window_flag;
    qua_hevc_window_t def_disp_win;

    int32_t vui_timing_info_present_flag;
    uint32_t vui_num_units_in_tick;
    uint32_t vui_time_scale;
    int32_t vui_poc_proportional_to_timing_flag;
    int32_t vui_num_ticks_poc_diff_one_minus1;
    int32_t vui_hrd_parameters_present_flag;

    int32_t bitstream_restriction_flag;
    int32_t tiles_fixed_structure_flag;
    int32_t motion_vectors_over_pic_boundaries_flag;
    int32_t restricted_ref_pic_lists_flag;
    int32_t min_spatial_segmentation_idc;
    int32_t max_bytes_per_pic_denom;
    int32_t max_bits_per_min_cu_denom;
    int32_t log2_max_mv_length_horizontal;
    int32_t log2_max_mv_length_vertical;
} qua_vui_t;

typedef struct qua_ptl_common_t {
    uint8_t profile_space;
    uint8_t tier_flag;
    uint8_t profile_idc;
    uint8_t profile_compatibility_flag[32];
    uint8_t level_idc;
    uint8_t progressive_source_flag;
    uint8_t interlaced_source_flag;
    uint8_t non_packed_constraint_flag;
    uint8_t frame_only_constraint_flag;
} qua_ptl_common_t;

typedef struct qua_ptl_t {
    qua_ptl_common_t general_ptl;
    qua_ptl_common_t sub_layer_ptl[MAX_SUB_LAYERS];

    uint8_t  sub_layer_profile_present_flag[MAX_SUB_LAYERS];
    uint8_t  sub_layer_level_present_flag[MAX_SUB_LAYERS];

    int32_t sub_layer_profile_space[MAX_SUB_LAYERS];
    uint8_t  sub_layer_tier_flag[MAX_SUB_LAYERS];
    int32_t sub_layer_profile_idc[MAX_SUB_LAYERS];
    uint8_t  sub_layer_profile_compatibility_flags[MAX_SUB_LAYERS][32];
    int32_t sub_layer_level_idc[MAX_SUB_LAYERS];
} qua_ptl_t;

typedef struct qua_scaling_list_t {
    /* This is a little wasteful, since sizeID 0 only needs 8 coeffs,
     * and size ID 3 only has 2 arrays, not 6. */
    uint8_t sl[4][6][64];
    uint8_t sl_dc[2][6];
} qua_scaling_list_t;

typedef struct qua_hevc_sps_t {
    uint32_t vps_id;
    uint32_t sps_id;
    int32_t chroma_format_idc;
    uint8_t separate_colour_plane_flag;

    ///< output (i.e. cropped) values
    int32_t output_width, output_height;
    qua_hevc_window_t output_window;

    qua_hevc_window_t pic_conf_win;

    int32_t bit_depth;
    int32_t bit_depth_chroma;
    int32_t pixel_shift;
    int32_t pix_fmt;

    uint32_t log2_max_poc_lsb;
    int32_t pcm_enabled_flag;

    int32_t max_sub_layers;
    struct {
        int max_dec_pic_buffering;
        int num_reorder_pics;
        int max_latency_increase;
    } temporal_layer[MAX_SUB_LAYERS];

    qua_vui_t vui;
    qua_ptl_t ptl;

    uint8_t scaling_list_enable_flag;
    qua_scaling_list_t scaling_list;

    uint32_t nb_st_rps;
    qua_short_term_rps_t st_rps[MAX_SHORT_TERM_RPS_COUNT];

    uint8_t amp_enabled_flag;
    uint8_t sao_enabled;

    uint8_t long_term_ref_pics_present_flag;
    uint16_t lt_ref_pic_poc_lsb_sps[32];
    uint8_t used_by_curr_pic_lt_sps_flag[32];
    uint8_t num_long_term_ref_pics_sps;

    struct {
        uint8_t  bit_depth;
        uint8_t  bit_depth_chroma;
        uint32_t log2_min_pcm_cb_size;
        uint32_t log2_max_pcm_cb_size;
        uint8_t  loop_filter_disable_flag;
    } pcm;
    uint8_t sps_temporal_mvp_enabled_flag;
    uint8_t sps_strong_intra_smoothing_enable_flag;

    uint32_t log2_min_cb_size;
    uint32_t log2_diff_max_min_coding_block_size;
    uint32_t log2_min_tb_size;
    uint32_t log2_max_trafo_size;
    int32_t log2_ctb_size;
    uint32_t log2_min_pu_size;

    int32_t max_transform_hierarchy_depth_inter;
    int32_t max_transform_hierarchy_depth_intra;

    ///< coded frame dimension in various units
    int32_t width;
    int32_t height;
    int32_t ctb_width;
    int32_t ctb_height;
    int32_t ctb_size;
    int32_t min_cb_width;
    int32_t min_cb_height;
    int32_t min_tb_width;
    int32_t min_tb_height;
    int32_t min_pu_width;
    int32_t min_pu_height;

    int32_t hshift[3];
    int32_t vshift[3];

    int32_t qp_bd_offset;
} qua_hevc_sps_t;

typedef struct qua_hevc_vps_t {
    uint8_t  vps_temporal_id_nesting_flag;
    int32_t vps_max_layers;
    int32_t vps_max_sub_layers; ///< vps_max_temporal_layers_minus1 + 1

    qua_ptl_t ptl;
    int32_t vps_sub_layer_ordering_info_present_flag;
    uint32_t vps_max_dec_pic_buffering[MAX_SUB_LAYERS];
    uint32_t vps_num_reorder_pics[MAX_SUB_LAYERS];
    uint32_t vps_max_latency_increase[MAX_SUB_LAYERS];
    int32_t vps_max_layer_id;
    int32_t vps_num_layer_sets; ///< vps_num_layer_sets_minus1 + 1
    uint8_t  vps_timing_info_present_flag;
    uint32_t vps_num_units_in_tick;
    uint32_t vps_time_scale;
    uint8_t vps_poc_proportional_to_timing_flag;
    int32_t vps_num_ticks_poc_diff_one; ///< vps_num_ticks_poc_diff_one_minus1 + 1
    int32_t vps_num_hrd_parameters;

    int32_t vps_extension_flag;

    int32_t max_num_ref_frames;
} qua_hevc_vps_t;

int32_t qua_hevc_decode_nal_vps(bitread_ctx_t *bitCtx, qua_hevc_vps_t *hevc_vps);
int32_t qua_hevc_decode_nal_sps(bitread_ctx_t *bitCtx, qua_hevc_sps_t *hevc_sps);

#ifdef  __cplusplus
}
#endif

#endif // _QUA_H265_PS_H_
