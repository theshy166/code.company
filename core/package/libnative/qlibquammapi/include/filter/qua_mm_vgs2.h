/*******************************************************************************************
#  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_MM_VGS2_H__
#define __QUA_MM_VGS2_H__

#include "filter/qua_mm_filter.h"

typedef enum qua_vgs2_comp
{
    QUA_VGS2_COMP_Y   = (1 << 0),
    QUA_VGS2_COMP_U   = (1 << 1),
    QUA_VGS2_COMP_V   = (1 << 2)
} qua_vgs2_comp_t;

typedef enum qua_vgs2_pix_width
{
    QUA_VGS2_WIDTH_1BIT    = 0,
    QUA_VGS2_WIDTH_2BIT,
    QUA_VGS2_WIDTH_4BIT,
    QUA_VGS2_WIDTH_8BIT,
    QUA_VGS2_WIDTH_16BIT,
    QUA_VGS2_WIDTH_32BIT
} qua_vgs2_pix_width_t;

typedef struct qua_vgs2_handle
{
	QUA_U64 handle_idx;
	QUA_U32 module_idx;
	QUA_U32 prior;
} qua_vgs2_handle_t;

typedef struct qua_vgs2_image
{
    qua_mem_info_t       data_addr;
    qua_mem_info_t       data_addr_u;
    qua_mem_info_t       data_addr_v;
    qua_size_t           image_size;
    qua_point_t          start;
    QUA_U32              pool_id;
} qua_vgs2_image_t;

typedef struct qua_vgs2_image_div
{
    qua_mem_info_t       data_yaddr;
    qua_mem_info_t       data_caddr;
    qua_size_t           image_size;
    qua_point_t          start;
    QUA_U32              pool_id;
} qua_vgs2_image_div_t;

typedef struct qua_vgs2_cvt_ctrl
{
    QUA_U32                    prior;
    qua_vgs2_image_div_t       src_data;
    qua_vgs2_image_div_t       dst_data;
    qua_size_t                 op_size;
    qua_vpu_vo_mode_t          src_format;
    QUA_U32                    src_comp;
    qua_vpu_vo_mode_t          dst_format;
} qua_vgs2_cvt_ctrl_t;

typedef struct qua_vgs2_rot_ctrl
{
    QUA_U32                    prior;
    qua_vgs2_image_t           src_data;
    qua_vgs2_image_t           dst_data;
    qua_size_t                 op_size;
    qua_vpu_vo_mode_t          src_format;
    QUA_U32                    src_comp;
    qua_vgs2_pix_width_t       src_pixw;
    qua_vgs_rotation_t         rotate;
    QUA_U32                    rotmode;
} qua_vgs2_rot_ctrl_t;

typedef struct qua_vgs2_jnt_ctrl
{
    QUA_U32                    prior;
    qua_vgs2_image_t           src_data0;
    qua_vgs2_image_t           src_data1;
    qua_vgs2_image_t           dst_data;
    qua_size_t                 op_size0;
    qua_size_t                 op_size1;
    qua_size_t                 op_size;
    qua_vpu_vo_mode_t          src_format;
    QUA_U32                    src_comp;
} qua_vgs2_jnt_ctrl_t;

typedef struct qua_vgs2_scl_ctrl
{
    QUA_U32                    prior;
    qua_vgs2_image_t           pic_src;
    qua_size_t                 op_size_src;
    qua_vgs2_image_t           pic_dst;
    qua_size_t                 op_size_dst;
    qua_vpu_vo_mode_t          src_format;
    QUA_U32                    src_comp;
} qua_vgs2_scl_ctrl_t;

typedef struct qua_vgs2_osd_cvt
{
    QUA_U32                    prior;
    qua_vgs2_image_t           pic_src;
    qua_vgs2_image_t           pic_dst;
    qua_size_t                 op_size;
    qua_point_t                osd_start;
    qua_vgs2_pix_width_t       pixw;
    qua_vgs_rotation_t         rotate;
} qua_vgs2_osd_cvt_ctrl_t;

typedef struct qua_mm_vgs2_device_t {
    struct qua_mm_device_t parent;

    QUA_S32 (*qua_vgs2_convert)(qua_vgs2_handle_t *handle, qua_vgs2_cvt_ctrl_t *ctrl, QUA_BOOL instant);
    QUA_S32 (*qua_vgs2_rotate)(qua_vgs2_handle_t *handle, qua_vgs2_rot_ctrl_t *ctrl, QUA_BOOL instant);
    QUA_S32 (*qua_vgs2_joint)(qua_vgs2_handle_t *handle, qua_vgs2_jnt_ctrl_t *ctrl, QUA_BOOL instant);
    QUA_S32 (*qua_vgs2_scale)(qua_vgs2_handle_t *handle, qua_vgs2_scl_ctrl_t *ctrl, QUA_BOOL instant);
    QUA_S32 (*qua_vgs2_osd_convert)(qua_vgs2_handle_t *handle, qua_vgs2_osd_cvt_ctrl_t *ctrl, QUA_BOOL instant);
    QUA_S32 (*qua_vgs2_query)(qua_vgs2_handle_t handle, QUA_BOOL *is_finish, QUA_BOOL is_block);
} qua_mm_vgs2_device_t;

#endif
