#ifndef QUA_CAPTURE_AUDIO_H
#define QUA_CAPTURE_AUDIO_H

#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0

#include "common/qua_mm_common.h"
#include "common/qua_sys_platform.h"
#include "common/qua_mm_video_common_type.h"
#include "audio/qua_mm_audio.h"
#include "avext/qua_media_encoder.h"
#include "avext/qua_mm_avext.h"
#include "system/qua_mm_system.h"

#include "qua_camera.h"

typedef int (*qua_capture_stream_cb)(int channel, qua_steram_type_t stream_type, qua_camera_stream_info_t *stream_info);

QUA_S32 qua_capture_audio_start(int id, qua_capture_stream_cb stream_callback);
QUA_S32 qua_capture_audio_stop(int id);
QUA_U32 set_audioCapture_volume(QUA_U32 volume);

#endif 

#endif // QUA_CAPTURE_AUDIO_H
