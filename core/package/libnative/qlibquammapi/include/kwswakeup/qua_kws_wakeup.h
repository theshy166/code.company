/*******************************************************************************************
#  Copyright (c) 2023~2024 Quaming Intelligent Technology Co., Ltd.
#  All Rights Reserved.
#  Confidential and Proprietary - Quaming Intelligent Technology Co., Ltd.
#*******************************************************************************************/

#ifndef __QUA_KEWS_WAKEUP_H__
#define __QUA_KEWS_WAKEUP_H__

#include "common/qua_type.h"

enum {
    QUA_KWS_RESULT_XIAOMING = 0,
    QUA_KWS_RESULT_OPEN_PHOTO = 1,
    QUA_KWS_RESULT_START_PHOTO = 2,
    QUA_KWS_RESULT_OPEN_RECORDING = 3,
    QUA_KWS_RESULT_START_RECORDING = 4,
    QUA_KWS_RESULT_STOP_RECORDING = 5,
    QUA_KWS_RESULT_OPEN_CALENDAR = 6,
    QUA_KWS_RESULT_OPEN_WEATHER = 7,
    QUA_KWS_RESULT_MAKE_CALL = 8,
    QUA_KWS_RESULT_ANSWER_CALL = 9,
    QUA_KWS_RESULT_HUNGUP_CALL = 10,
    QUA_KWS_RESULT_PREVIOUS_PICTURE = 11,
    QUA_KWS_RESULT_NEXT_PICTURE = 12,
    QUA_KWS_RESULT_INCREASE_VOLUME = 13,
    QUA_KWS_RESULT_DECREASE_VOLUME = 14,
};

typedef QUA_S32 (*QUA_KWS_WAKEUP_CALLBACK)(QUA_S32 result, QUA_VOID* user_data);

extern QUA_VOID* qua_kws_wakeup_startup(QUA_VOID* config, QUA_KWS_WAKEUP_CALLBACK cb, QUA_VOID* user_data);
extern QUA_S32 qua_kws_wakeup_shutdown(QUA_VOID* handler);

#endif /* __QUA_KEWS_WAKEUP_H__ */