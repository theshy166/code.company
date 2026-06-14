#ifndef QUACAST_AUDIO_H
#define QUACAST_AUDIO_H
#include <common/qua_type.h>
int quacast_audio_init(int sample, int channels);
//int quacast_audio_cfg(int sample, int channels);
int quacast_audio_play(char *data, int len);
int quacast_audio_deinit(void);
#endif