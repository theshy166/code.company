#ifndef LOSPLAYER_H
#define LOSPLAYER_H

#include "lvgl/lvgl.h"
#include "xos_app_conf.h"

typedef enum {
    VOLUME_LEVEL_MUTE = 0,
    VOLUME_LEVEL_LOW,
    VOLUME_LEVEL_MEDIUM,
    VOLUME_LEVEL_HIGH,
    VOLUME_LEVEL_MAX
} xos_volume_level;

typedef void (*progress_callback)(void * user, int purpose, int64_t ct);


//return a player.
void *los_player_create(lv_obj_t *parent, const char * path, lv_area_t area, bool auto_restart);
void *los_player_create2(lv_obj_t *parent, const char * path, lv_area_t area, bool auto_restart,bool auto_center,int display_idx,int rotation);
bool xos_player_video_delay(lv_obj_t *parent, const char * videoPath, lv_area_t area, bool auto_restart,bool auto_center,int display_idx,void **player,int rotation);
void *xos_player_playvoice(const char * videoPath, bool auto_restart);
int64_t los_get_duration(void *player);
int xos_player_destroyvoice(void);
//Succeed seek time
void los_player_seek(void *player, int64_t miliseconds);

//
void los_set_progress_callback(progress_callback progress_cb);

void los_player_play(void *player);
void los_player_pause(void *player);
void los_player_resume(void *player);
void los_player_stop(void *player);
void los_player_destroy(void *player);
void los_player_destroy2(void *player,int display_idx);

//async
void xos_player_stop_async(void *player);
void xos_player_destroy_async(void *player);
void xos_player_destroy_async2(void *player,int display_idx);

void xos_player_set_volume_level(void *player, xos_volume_level level);
void xos_player_set_volume(void *player, int volume);

int xos_player_reset(void *player);
int xos_player_set_data_source(void *player, const char *path);
int xos_player_prepare(void *player);

#endif // LOSPLAYER_H
