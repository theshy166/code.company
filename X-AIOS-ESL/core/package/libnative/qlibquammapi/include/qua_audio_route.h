/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_AUDIO_ROUTE_H__
#define __QUA_AUDIO_ROUTE_H__
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/******************************************************************************
  Include header files
 *****************************************************************************/
#include "qua_audio_ctl.h"

/******************************************************************************
  Macros, Variables definition
 *****************************************************************************/
typedef union {
    QUA_S32 *enumerated;
    QUA_S64 *integer;
    QUA_VOID *ptr;
    QUA_U8 *bytes;
} ctl_values;

typedef struct {
    QUA_U32 ctl_index;
    QUA_S32 index;
    QUA_S64 value;
} mixer_value;

typedef struct {
    QUA_U32 ctl_index;
    QUA_U32 num_values;
    QUA_U32 type;
    ctl_values value;
} mixer_setting;

typedef struct {
    QUA_CHAR *name;
    QUA_U32 size;
    QUA_U32 length;
    mixer_setting *setting;
} mixer_path;

typedef struct {
    qua_audio_ctl *ctl;
    QUA_U32 num_values;
    ctl_values old_value;
    ctl_values new_value;
    ctl_values reset_value;
    QUA_U32 active_count;
} mixer_state;

typedef struct {
    QUA_U32 num_qua_audio_ctls;
    mixer_state *mixer_state;

    QUA_U32 mixer_path_size;
    QUA_U32 num_mixer_paths;
    mixer_path *mixer_path;
} qua_audio_route;

typedef struct {
    qua_audio_route *ar;
    mixer_path *path;
    QUA_S32 level;
} config_parse_state;

/* Initialize and free the audio routes */
qua_audio_route *qua_audio_route_init(QUA_CONST_CHAR *xml_path);

/* Apply an audio route path by name */
QUA_VOID qua_audio_route_free(qua_audio_route *ar);

/* Apply an audio route path by name */
QUA_S32 qua_audio_route_apply_path(qua_audio_route *ar, QUA_CONST_CHAR *name);

/* Apply and update mixer with audio route path by name */
QUA_S32 qua_audio_route_apply_and_update_path(qua_audio_route *ar, QUA_CONST_CHAR *name);

/* Reset an audio route path by name */
QUA_S32 qua_audio_route_reset_path(qua_audio_route *ar, QUA_CONST_CHAR *name);

/* Reset and update mixer with audio route path by name */
QUA_S32 qua_audio_route_reset_and_update_path(qua_audio_route *ar, QUA_CONST_CHAR *name);

/* Reset the audio routes back to the initial state */
QUA_VOID qua_audio_route_reset(qua_audio_route *ar);

/* Update the mixer with any changed values */
QUA_S32 qua_audio_route_update_mixer(qua_audio_route *ar);

#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif   /* __QUA_AUDIO_ROUTE_H__ */
