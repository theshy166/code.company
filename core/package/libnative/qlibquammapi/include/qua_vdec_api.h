/*
 *  Copyright (c) 2023 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef QUA_VDEC_API_H
#define QUA_VDEC_API_H

#include <stdint.h>
#include "fy_common.h"
#include "fy_comm_vdec.h"

#include "common/qua_type.h"

#ifdef __cplusplus
extern "C" {
#endif

QUA_S32 qua_vdec_init();
QUA_S32 qua_vdec_exit();
QUA_S32 qua_vdec_initAttr(VDEC_CHN_ATTR_S *pstAttr, PAYLOAD_TYPE_E enType, SIZE_S *pstSize, QUA_U32 bufSize);
QUA_S32 qua_vdec_createChn(QUA_S32 s32ChnId, VDEC_CHN_ATTR_S *pstAttr, QUA_S32 u32BlkCnt);
QUA_S32 qua_vdec_destroyChn(QUA_S32 s32ChnId);
QUA_S32 qua_vdec_start(QUA_S32 s32ChnId);
QUA_S32 qua_vdec_stop(QUA_S32 s32ChnId);
QUA_S32 qua_vdec_getFd(QUA_S32 s32ChnId);
QUA_S32 qua_vdec_resetChn(QUA_S32 s32ChnId);
QUA_S32 qua_vdec_sendStream(QUA_S32 s32ChnId, const VDEC_STREAM_S *pstStream, QUA_S32 s32MilliSec);
QUA_S32 qua_vdec_getFrame(QUA_S32 s32ChnId, VIDEO_FRAME_INFO_S *pstFrameInfo, QUA_S32 s32MilliSec);
QUA_S32 qua_vdec_releaseFrame(QUA_S32 s32ChnId, VIDEO_FRAME_INFO_S *pstFrameInfo);
QUA_S32 qua_vdec_getChnAttr(QUA_S32 s32ChnId, VDEC_CHN_ATTR_S *pstAttr);
QUA_S32 qua_vdec_query(QUA_S32 s32ChnId, VDEC_CHN_STAT_S *pstStat);
QUA_S32 qua_vdec_getChnParam(QUA_S32 s32ChnId, VDEC_CHN_PARAM_S* pstParam);
QUA_S32 qua_vdec_setChnParam(QUA_S32 s32ChnId, VDEC_CHN_PARAM_S* pstParam);
QUA_S32 qua_vdec_getChnLuma(QUA_S32 s32ChnId, VDEC_CHN_LUM_S *pstLuma);

#ifdef __cplusplus
}
#endif

#endif // QUA_VDEC_API_H
