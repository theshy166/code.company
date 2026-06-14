#ifndef QUACAST_VIDEO_H
#define QUACAST_VIDEO_H
#include <common/qua_type.h>

int quacast_video_init(qua_cast_init_params_t params, char *data, int len);
int quacast_video_play(char *data, int len, QUA_U64 pts);
int quacast_video_deinit(void);
int quacast_video_decoder_reset(int width, int height, int framerate);

#endif