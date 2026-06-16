/*
 *  Copyright (c) 2023-2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef _QUA_BITREAD_H_
#define _QUA_BITREAD_H_

#include <stdint.h>
#include <string.h>

#define __BITREAD_ERR   __bitread_error
#define READ_ONEBIT(bitctx, out)\
    do {\
        int32_t _out; \
        bitctx->ret = qua_read_bits(bitctx, 1, &_out); \
        if (!bitctx->ret) { *out = _out; }\
        else { goto __BITREAD_ERR; }\
    } while (0)

#define READ_BITS(bitctx, num_bits, out)\
    do {\
        int32_t _out; \
        bitctx->ret = qua_read_bits(bitctx, num_bits, &_out); \
        if (!bitctx->ret) { *out = _out; }\
        else { goto __BITREAD_ERR; }\
    } while (0)

#define READ_BITS_LONG(bitctx, num_bits, out)\
    do {\
        uint32_t _out; \
        bitctx->ret = qua_read_longbits(bitctx, num_bits, &_out); \
        if (!bitctx->ret) { *out = _out; }\
        else { goto __BITREAD_ERR; }\
    } while (0)

#define SHOW_BITS(bitctx, num_bits, out)\
    do {\
        int32_t _out; \
        bitctx->ret = qua_show_bits(bitctx, num_bits, &_out); \
        if (!bitctx->ret) { *out = _out; }\
        else { goto __BITREAD_ERR; }\
    } while (0)

#define SHOW_BITS_LONG(bitctx, num_bits, out)\
    do {\
        uint32_t _out; \
        bitctx->ret = qua_show_longbits(bitctx, num_bits, &_out); \
        if (!bitctx->ret) { *out = _out; }\
        else { goto __BITREAD_ERR; }\
    } while (0)

#define SKIP_BITS(bitctx, num_bits)\
    do {\
        bitctx->ret = qua_skip_longbits(bitctx, num_bits); \
        if (bitctx->ret) { goto __BITREAD_ERR; }\
    } while (0)

#define SKIP_BITS_LONG(bitctx, num_bits)\
    do {\
        bitctx->ret = qua_skip_longbits(bitctx, num_bits); \
        if (bitctx->ret) { goto __BITREAD_ERR; }\
    } while (0)

#define READ_UE(bitctx, out)\
    do {\
        uint32_t _out; \
        bitctx->ret = qua_read_ue(bitctx, &_out); \
        if (!bitctx->ret) { *out = _out; }\
        else { goto __BITREAD_ERR; }\
    } while (0)

#define READ_SE(bitctx, out)\
    do {\
        int32_t _out; \
        bitctx->ret = qua_read_se(bitctx, &_out); \
        if (!bitctx->ret) { *out = _out; }\
        else { goto __BITREAD_ERR; }\
    } while (0)

typedef enum  qua_pseudo_code_type_t {
    QUA_PSEUDO_CODE_NONE = 0,
    QUA_PSEUDO_CODE_H264_H265,
    QUA_PSEUDO_CODE_BUT
} qua_pseudo_code_type_t;

typedef struct bitread_ctx_t {
    // Pointer to the next unread (not in curr_byte_) byte in the stream.
    uint8_t *data_;
    // Bytes left in the stream (without the curr_byte_).
    uint32_t bytes_left_;
    // Contents of the current byte; first unread bit starting at position
    // 8 - num_remaining_bits_in_curr_byte_ from MSB.
    int64_t curr_byte_;
    // Number of bits remaining in curr_byte_
    int32_t num_remaining_bits_in_curr_byte_;
    // Used in emulation prevention three byte detection (see spec).
    // Initially set to 0xffff to accept all initial two-byte sequences.
    int64_t prev_two_bytes_;
    // Number of emulation presentation bytes (0x000003) we met.
    int64_t emulation_prevention_bytes_;
    // count PPS SPS SEI read bits
    int32_t used_bits;
    uint8_t  *buf;
    int32_t buf_len;
    // ctx
    int32_t   ret;
    qua_pseudo_code_type_t prevention_type;
    int32_t (*update_curbyte)(struct bitread_ctx_t *bitctx);
} bitread_ctx_t;

#ifdef __cplusplus
extern "C" {
#endif

int32_t qua_read_bits(bitread_ctx_t *bitctx, int32_t num_bits, int32_t *out);
int32_t qua_read_longbits(bitread_ctx_t *bitctx, int32_t num_bits, uint32_t *out);
int32_t qua_skip_bits(bitread_ctx_t *bitctx, int32_t num_bits);
int32_t qua_skip_longbits(bitread_ctx_t *bitctx, int32_t num_bits);
int32_t qua_show_bits(bitread_ctx_t *bitctx, int32_t num_bits, int32_t *out);
int32_t qua_show_longbits(bitread_ctx_t *bitctx, int32_t num_bits, uint32_t *out);
int32_t qua_read_ue(bitread_ctx_t *bitctx, uint32_t *val);
int32_t qua_read_se(bitread_ctx_t *bitctx, int32_t *val);
uint32_t qua_has_more_rbsp_data(bitread_ctx_t *bitctx);
void qua_set_bitread_ctx(bitread_ctx_t *bitctx, uint8_t *data, int32_t size);
void qua_set_bitread_pseudo_code_type(bitread_ctx_t *bitctx, qua_pseudo_code_type_t type);
uint8_t *qua_align_get_bits(bitread_ctx_t *bitctx);
int32_t qua_get_bits_left(bitread_ctx_t *bitctx);
int32_t qua_get_bits_count(bitread_ctx_t *bitctx);

#ifdef  __cplusplus
}
#endif

#endif // _QUA_BITREAD_H_
