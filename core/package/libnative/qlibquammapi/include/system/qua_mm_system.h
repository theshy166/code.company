/*******************************************************************************************
#  Copyright (c) 2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/
#ifndef __QUA_MM_SYSTEM_H__
#define __QUA_MM_SYSTEM_H__

#include "common/qua_mm_common.h"

#define QUA_VB_MAX_POOL_NAME_LEN 32
#define QUA_VB_MAX_POOLS 256
#define QUA_VB_MAX_COMMON_POOLS 16
#define QUA_VB_INVALID_POOLID (-1U)
#define QUA_VB_INVALID_HANDLE (-1U)

typedef struct qua_vb_config {
    QUA_U32 max_pool_cnt;
    struct {
        QUA_U32 block_size;
        QUA_U32 block_cnt;
        QUA_CHAR pool_name[QUA_VB_MAX_POOL_NAME_LEN];
    } common_pools[QUA_VB_MAX_COMMON_POOLS];
} qua_vb_config_t;

typedef struct qua_version_info
{
    QUA_CHAR *quamm_version;
    QUA_U32 mpp_build_date;
    QUA_U32 mpp_sw_version;
    QUA_U32 mpp_hw_version;
} qua_version_info_t;

typedef enum qua_mpp_mod {
    QUA_MPP_MOD_CMPI    = 0,
    QUA_MPP_MOD_VB      = 1,
    QUA_MPP_MOD_SYS     = 2,
    QUA_MPP_MOD_RGN     = 3,
    QUA_MPP_MOD_CHNL    = 4,
    QUA_MPP_MOD_VDEC    = 5,
    QUA_MPP_MOD_GROUP   = 6,
    QUA_MPP_MOD_VPSS    = 7,
    QUA_MPP_MOD_VENC    = 8,
    QUA_MPP_MOD_VDA     = 9,
    QUA_MPP_MOD_H264E   = 10,
    QUA_MPP_MOD_JPEGED  = 11,
    QUA_MPP_MOD_MPEG4E  = 12,
    QUA_MPP_MOD_H264D   = 13,
    QUA_MPP_MOD_JPEGD   = 14,
    QUA_MPP_MOD_VOU     = 15,
    QUA_MPP_MOD_VIU     = 16,
    QUA_MPP_MOD_DSU     = 17,
    QUA_MPP_MOD_VALG    = 18,
    QUA_MPP_MOD_RC      = 19,
    QUA_MPP_MOD_AIO     = 20,
    QUA_MPP_MOD_AI      = 21,
    QUA_MPP_MOD_AO      = 22,
    QUA_MPP_MOD_AENC    = 23,
    QUA_MPP_MOD_ADEC    = 24,
    QUA_MPP_MOD_AVENC   = 25,
    QUA_MPP_MOD_PCIV    = 26,
    QUA_MPP_MOD_PCIVFMW = 27,
    QUA_MPP_MOD_ISP     = 28,
    QUA_MPP_MOD_IVE     = 29,
    QUA_MPP_MOD_DCCM    = 31,
    QUA_MPP_MOD_DCCS    = 32,
    QUA_MPP_MOD_PROC    = 33,
    QUA_MPP_MOD_LOG     = 34,
    QUA_MPP_MOD_MST_LOG = 35,
    QUA_MPP_MOD_VD      = 36,
    QUA_MPP_MOD_VCMP    = 38,
    QUA_MPP_MOD_FB      = 39,
    QUA_MPP_MOD_HDMI    = 40,
    QUA_MPP_MOD_VOIE    = 41,
    QUA_MPP_MOD_TDE     = 42,
    QUA_MPP_MOD_USR     = 43,
    QUA_MPP_MOD_VEDU    = 44,
    QUA_MPP_MOD_VGS     = 45,
    QUA_MPP_MOD_H265E   = 46,
    QUA_MPP_MOD_FD      = 47,
    QUA_MPP_MOD_ODT     = 48,
    QUA_MPP_MOD_VQA     = 49,
    QUA_MPP_MOD_LPR     = 50,
    QUA_MPP_MOD_FISHEYE = 51,
    QUA_MPP_MOD_VPPU    = 52,
} qua_mpp_mod_t;

typedef enum qua_mpp_obj
{
    QUA_OBJ_NONE    = 0,
    QUA_OBJ_VICAP   = 1,
    QUA_OBJ_ISP     = 2,
    QUA_OBJ_VPU_VI  = 3,
    QUA_OBJ_VPU_VO  = 4,
    QUA_OBJ_VPU_BGM = 5,
    QUA_OBJ_ENC     = 6,
    QUA_OBJ_JPEG    = 7,
    QUA_OBJ_BGM     = 8,
    QUA_OBJ_NN      = 9,
    QUA_OBJ_VOU     = 10,
    QUA_OBJ_VPPU    = 11,
    QUA_OBJ_VDEC    = 12,
    QUA_OBJ_JPEGD   = 13,
    QUA_OBJ_VDSP    = 14,
} qua_mpp_obj_t;

typedef struct qua_mpp_chn_t {
    union {
        qua_mpp_mod_t mod;
        qua_mpp_obj_t obj;
    };
    QUA_S32 dev;
    QUA_S32 chn;
} qua_mpp_chn_t;

typedef QUA_U32 QUA_VB_POOL;
typedef QUA_U32 QUA_VB_BLK;

typedef enum qua_vb_uid
{
    QUA_VB_UID_VIU          = 0,
    QUA_VB_UID_VOU          = 1,
    QUA_VB_UID_VGS          = 2,
    QUA_VB_UID_VENC         = 3,
    QUA_VB_UID_VDEC         = 4,
    QUA_VB_UID_VDA          = 5,
    QUA_VB_UID_H264E        = 6,
    QUA_VB_UID_JPEGED       = 7,
    QUA_VB_UID_MPEG4E       = 8,
    QUA_VB_UID_H264D        = 9,
    QUA_VB_UID_JPEGD        = 10,
    QUA_VB_UID_MPEG4D       = 11,
    QUA_VB_UID_VPSS         = 12,
    QUA_VB_UID_GRP          = 13,
    QUA_VB_UID_MPI          = 14,
    QUA_VB_UID_PCIV         = 15,
    QUA_VB_UID_AI           = 16,
    QUA_VB_UID_AENC         = 17,
    QUA_VB_UID_RC           = 18,
    QUA_VB_UID_VFMW         = 19,
    QUA_VB_UID_USER         = 20,
    QUA_VB_UID_H265E        = 21,
    QUA_VB_UID_BGM          = 22,
    QUA_VB_UID_BGMX         = 23,
    QUA_VB_UID_BGMSW        = 24,
    QUA_VB_UID_VPPU         = 25,
    QUA_VB_UID_BUTT
} qua_vb_uid_t;

typedef struct qua_mm_system_ops_t {
    struct qua_mm_system_t system;

    QUA_S32 (*sys_init)(qua_vb_config_t *vb_config);
    QUA_S32 (*sys_bind)(qua_mpp_chn_t *src_chn, qua_mpp_chn_t *dst_chn);
    QUA_S32 (*sys_unbind)(qua_mpp_chn_t *src_chn, qua_mpp_chn_t *dst_chn);
    QUA_S32 (*sys_unbind_by_src)(qua_mpp_chn_t *src_chn);
    QUA_S32 (*sys_unbind_by_dst)(qua_mpp_chn_t *dst_chn);
    QUA_S32 (*sys_get_bind_by_dst)(qua_mpp_chn_t *dst_chn, qua_mpp_chn_t *src_chn);
    QUA_U32 (*sys_get_chipid)();
    QUA_S32 (*sys_set_reg)(QUA_ULONG phy_addr, QUA_U32 value);
    QUA_S32 (*sys_get_reg)(QUA_ULONG phy_addr, QUA_U32 *value);
    QUA_VOID_PTR (*sys_mmap)(QUA_ULONG phy_addr, QUA_U32 size);
    QUA_VOID_PTR (*sys_mmap_cache)(QUA_ULONG phy_addr, QUA_U32 size);
    QUA_S32 (*sys_munmap)(QUA_VOID_PTR virt_addr, QUA_U32 size);
    QUA_S32 (*sys_mflush_cache)(QUA_ULONG phy_addr, QUA_VOID_PTR virt_addr, QUA_U32 size);
    QUA_S32 (*sys_mmz_alloc)(const QUA_CHAR *mmb_name, const QUA_CHAR *mmz_zone, QUA_U32 size,
        QUA_ULONG *phy_addr, QUA_VOID_PTR *virt_addr);
    QUA_S32 (*sys_mmz_alloc_cached)(const QUA_CHAR *mmb_name, const QUA_CHAR *mmz_zone, QUA_U32 size,
        QUA_ULONG *phy_addr, QUA_VOID_PTR *virt_addr);
    QUA_S32 (*sys_mmz_flush_cache)(QUA_ULONG phy_addr, QUA_VOID_PTR virt_addr, QUA_U32 size);
    QUA_S32 (*sys_mmz_invalidate_cache)(QUA_ULONG phy_addr, QUA_VOID_PTR virt_addr, QUA_U32 size);
    QUA_S32 (*sys_mmz_free)(QUA_ULONG phy_addr, QUA_VOID_PTR virt_addr);
    QUA_VB_POOL (*vb_create_pool)(QUA_U32 blk_size, QUA_U32 blk_cnt,const QUA_CHAR *mmz_zone);
    QUA_S32 (*vb_destroy_pool)(QUA_VB_POOL pool);
    QUA_VB_BLK (*vb_get_block)(QUA_VB_POOL pool, QUA_U32 blk_size,const QUA_CHAR *mmz_zone);
    QUA_S32 (*vb_release_block)(QUA_VB_BLK block);
    QUA_U64 (*vb_handle_to_phyaddr)(QUA_VB_BLK block);
    QUA_VB_POOL (*vb_handle_to_poolid)(QUA_VB_BLK block);
    QUA_S32 (*vb_inquire_user_cnt)(QUA_VB_BLK block);
    QUA_S32 (*vb_mmap_pool)(QUA_VB_POOL pool);
    QUA_S32 (*vb_munmap_pool)(QUA_VB_POOL pool);
    QUA_S32 (*vb_get_block_virtaddr)(QUA_VB_POOL pool, QUA_U64 phy_addr, QUA_VOID_PTR *virt_addr);
    QUA_VB_POOL (*vb_create_virt_pool)(QUA_U32 blk_cnt, const QUA_CHAR *mmz_zone);
    QUA_VB_BLK (*vb_add_block)(QUA_VB_POOL pool, QUA_U64 phy_addr, QUA_U32 size);
    QUA_S32 (*clk_enable)(QUA_CHAR *clk_name);
    QUA_S32 (*clk_disable)(QUA_CHAR *clk_name);
    QUA_S32 (*clk_set_rate)(QUA_CHAR *clk_name, QUA_ULONG freq);
    QUA_S32 (*clk_get_rate)(QUA_CHAR *clk_name, QUA_ULONG *freq);
} qua_mm_system_ops_t;

#endif
