/*
 *  Copyright (c) 2025 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_H264_SCALIST_H__
#define __QUA_H264_SCALIST_H__

int32_t parse_scaling_list(bitread_ctx_t *p_bitctx, int32_t size, int32_t *scaling_list, int32_t *use_default);

#endif // __QUA_H264_SCALIST_H__
