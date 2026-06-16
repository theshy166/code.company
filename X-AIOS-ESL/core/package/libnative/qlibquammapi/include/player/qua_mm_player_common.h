/*
 *  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
 *
 *  All Rights Reserved.
 *  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
 */

#ifndef __QUA_MM_PLAYER_COMMON_H__
#define __QUA_MM_PLAYER_COMMON_H__

#include "common/qua_type.h"
#include "display/qua_mm_display.h"

typedef QUA_S32 (*CallBack)(QUA_VOID* user, QUA_S32 evt, QUA_VOID* info);

enum qua_player_states {
    PLAYER_STATE_ERROR        = 0,
    PLAYER_IDLE               = 1,
    PLAYER_INITIALIZED        = 2,
    PLAYER_PREPARING          = 3,
    PLAYER_PREPARED           = 4,
    PLAYER_STARTED            = 5,
    PLAYER_PAUSED             = 6,
    PLAYER_STOPPED            = 7,
    PLAYER_PLAYBACK_COMPLETE  = 8,
    PLAYER_SEEKING            = 9,
};

enum qua_player_event {
    PLAYER_EVT_ERROR = 0,
    PLAYER_EVT_SEEK_COMPLETE,
    PLAYER_EVT_PLAYBACK_COMPLETE,
    PLAYER_EVT_CURRENT_POSITION,
    PLAYER_EVT_PREPARE_STATUS,
    PLAYER_EVT_START_STATUS,
    PLAYER_EVT_PAUSE_STATUS,
    PLAYER_EVT_RESUME_STATUS,
    PLAYER_EVT_SEEK_STATUS,
    PLAYER_EVT_STOP_STATUS,
    PLAYER_EVT_DESTROY_STATUS,
};

enum qua_parameter_keys {
    KEY_PARAMETER_VO_CHN_RECT,          // set before prepare
    KEY_PARAMETER_VO_DISPLAY_RECT,      // set before prepare
    KEY_PARAMETER_VO_IMG_SIZE,          // set before prepare
    KEY_PARAMETER_VO_ROTATE,            // set before prepare
    KEY_PARAMETER_FILTER_CHN_ATTR,      // set before prepare
    KEY_PARAMETER_SPEED_RATE,
    KEY_PARAMETER_SET_CURRENT_POSITION_CALLBACK,
    KEY_PARAMETER_VIDEO_SIZE,
    KEY_PARAMETER_VO_DISPLAY_ID,        // set before prepare value: <id:display0, ...>
    KEY_PARAMETER_VO_FRAMERATE,
    KEY_PARAMETER_VIDEO_FRAMERATE,      // get after setdata_source
};

#endif // __QUA_MM_PLAYER_COMMON_H__