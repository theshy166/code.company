/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_AUDIO_CTL_H__
#define __QUA_AUDIO_CTL_H__

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
  Include header files
 *****************************************************************************/
#include <stddef.h>
#include <sys/ioctl.h>
#include "fy_comm_i2s.h"
#include "common/qua_type.h"
/******************************************************************************
  Macros, Variables definition
 *****************************************************************************/
#define MAX_I2S_NUM 5
#define MAX_CTL_ENUM_NUM 8
typedef enum {
    QUA_AUDIO_CTL_TYPE_BOOL,
    QUA_AUDIO_CTL_TYPE_INT,
    QUA_AUDIO_CTL_TYPE_ENUM,
    QUA_AUDIO_CTL_TYPE_BYTE,
    QUA_AUDIO_CTL_TYPE_IEC958,
    QUA_AUDIO_CTL_TYPE_INT64,
    QUA_AUDIO_CTL_TYPE_UNKNOWN,
    QUA_AUDIO_CTL_TYPE_MAX,
}qua_audio_ctl_type;

enum {
    CTL_I2S_0_RX_AUDIO_MIXER_MULTIMEDIA1,
    CTL_I2S_3_RX_AUDIO_MIXER_MULTIMEDIA2,
    CTL_I2S_4_RX_AUDIO_MIXER_MULTIMEDIA3,
    CTL_I2S_0_RX_AUDIO_MIXER_MULTIMEDIA4,
    CTL_I2S_3_RX_AUDIO_MIXER_MULTIMEDIA5,
    CTL_I2S_4_RX_AUDIO_MIXER_MULTIMEDIA6,

    CTL_I2S_3_I2S_4_RX_I2S_0_RX_AUDIO_MIXER_MULTIMEDIA7,
    CTL_I2S_3_I2S_4_RX_I2S_0_RX_AUDIO_MIXER_MULTIMEDIA8,
    CTL_I2S_0_RX_AW87519_PA_AUDIO_MIXER_MULTIMEDIA9,
    CTL_I2S_0_RX_AW87519_PA_AUDIO_MIXER_MULTIMEDIA10,
    CTL_I2S_0_RX_AUDIO_MIXER_HEADSET_MULTIMEDIA11,
    CTL_I2S_0_RX_AUDIO_MIXER_HEADSET_MULTIMEDIA12,
    CTL_MULTIMEDIA1_MIXER_I2S_0_TX,
};

typedef struct {
    QUA_CONST_CHAR *name;
    QUA_S32 num_values;
    qua_audio_ctl_type type;
    union {
        union {
            QUA_S32 value[128];
        } integer;
        union {
            QUA_S64 value[64];
        } integer64;
        union {
            QUA_U32 item[128];
        } enumerated;
        union {
            QUA_U8 data[512];
        } bytes;
    } value;

    QUA_S32 i2s_id;

    struct {
        QUA_U32 items;	/* R: number of items */
        QUA_U8 name[MAX_CTL_ENUM_NUM][64];		/* R: value name */
    } enumerated;
}qua_audio_ctl;

typedef enum {
    I2S_DISABLE,
    I2S_ENABLE
}qua_i2s_op;

typedef enum {
    I2S_NONE,
    I2S_AO = 0x1,
    I2S_AI = 0x2
}qua_i2s_direction;

typedef struct {
    qua_i2s_op i2s_op;  //0 disable 1 enable
    qua_i2s_direction i2s_direction;
}qua_i2s_param;

static const qua_audio_ctl qua_audio_ctl_table[] = {
    [CTL_I2S_0_RX_AUDIO_MIXER_MULTIMEDIA1] = {.name = "I2S_0_RX Audio Mixer MultiMedia1", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}, .i2s_id = 0},
    [CTL_I2S_3_RX_AUDIO_MIXER_MULTIMEDIA2] = {.name = "I2S_3_RX Audio Mixer MultiMedia2", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}, .i2s_id = 3},
    [CTL_I2S_4_RX_AUDIO_MIXER_MULTIMEDIA3] = {.name = "I2S_4_RX Audio Mixer MultiMedia3", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}, .i2s_id = 4},
    [CTL_I2S_0_RX_AUDIO_MIXER_MULTIMEDIA4] = {.name = "I2S_0_RX Audio Mixer MultiMedia4", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}, .i2s_id = 0},
    [CTL_I2S_3_RX_AUDIO_MIXER_MULTIMEDIA5] = {.name = "I2S_3_RX Audio Mixer MultiMedia5", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}, .i2s_id = 3},
    [CTL_I2S_4_RX_AUDIO_MIXER_MULTIMEDIA6] = {.name = "I2S_4_RX Audio Mixer MultiMedia6", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}, .i2s_id = 4},
    [CTL_I2S_3_I2S_4_RX_I2S_0_RX_AUDIO_MIXER_MULTIMEDIA7] = {.name = "I2S_3_RX I2S_4_RX I2S_0_RX Audio Mixer MultiMedia7", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}},
    [CTL_I2S_3_I2S_4_RX_I2S_0_RX_AUDIO_MIXER_MULTIMEDIA8] = {.name = "I2S_3_RX I2S_4_RX I2S_0_RX Audio Mixer MultiMedia8", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}},
    [CTL_I2S_0_RX_AW87519_PA_AUDIO_MIXER_MULTIMEDIA9] = {.name = "I2S_0_RX Audio Mixer AW87519 PA MultiMedia9", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}},
    [CTL_I2S_0_RX_AW87519_PA_AUDIO_MIXER_MULTIMEDIA10] = {.name = "I2S_0_RX Audio Mixer AW87519 PA MultiMedia10", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}},
    [CTL_I2S_0_RX_AUDIO_MIXER_HEADSET_MULTIMEDIA11] = {.name = "I2S_0_RX Audio Mixer Headset MultiMedia11", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}},
    [CTL_I2S_0_RX_AUDIO_MIXER_HEADSET_MULTIMEDIA12] = {.name = "I2S_0_RX Audio Mixer Headset MultiMedia12", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}},
    [CTL_MULTIMEDIA1_MIXER_I2S_0_TX] = {.name = "MultiMedia1 Mixer I2S_0_TX", .num_values = 1, .type = QUA_AUDIO_CTL_TYPE_BOOL, .value = {.integer = {0}}, .i2s_id = 0},
};

#define QUA_AUDIO_IOCTL_MAGIC 'Q'
#define QUA_AUDIO_IOCTL_DEV_OPEN _IOW(QUA_AUDIO_IOCTL_MAGIC, 0, unsigned int)
#define QUA_AUDIO_IOCTL_DEV_CLOSE _IOW(QUA_AUDIO_IOCTL_MAGIC, 1, unsigned int)

/******************************************************************************
  function decalare
 *****************************************************************************/
const qua_audio_ctl* qua_audio_get_ctl(QUA_S32 ctl_index);
QUA_U32 qua_audio_ctl_get_num_values(qua_audio_ctl *ctl);
QUA_U32 qua_audio_get_num_ctls(QUA_VOID);
QUA_CONST_CHAR *qua_audio_ctl_get_name(const qua_audio_ctl *ctl);
qua_audio_ctl_type qua_audio_ctl_get_type(const qua_audio_ctl *ctl);
const qua_audio_ctl* qua_audio_get_ctl_by_name(QUA_CONST_CHAR *name);
QUA_VOID qua_audio_ctl_update(QUA_VOID);
QUA_S32 qua_audio_ctl_set_value(qua_audio_ctl *ctl, QUA_VOID* value_ptr);
QUA_S32 qua_audio_ctl_get_value(qua_audio_ctl *ctl, QUA_U32 id);
QUA_S32 qua_audio_ctl_get_array(qua_audio_ctl *ctl, QUA_VOID *array, QUA_SIZE_T count);
QUA_U32 qua_audio_ctl_get_num_enums(qua_audio_ctl *ctl);
QUA_CONST_CHAR *qua_audio_ctl_get_enum_string(qua_audio_ctl *ctl, QUA_U32 enum_id);
QUA_S32 qua_audio_get_i2smask(QUA_VOID);
QUA_BOOL qua_audio_play_caputure_running(QUA_S32 i2s_id);

#ifdef __cplusplus
}
#endif
#endif
