#include <fcntl.h>
#include "losplayer.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#include "common/qua_sys_platform.h"
#include "system/qua_mm_system.h"
#include "player/qua_mm_player.h"
#include "player/qua_mm_player_common.h"
#endif
#endif //CONFIG_XOS_FWK_PLAYER

#if defined(CONFIG_CHIPSET_QM10XD) && (!defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0)
extern int g_screen_rotation;

#define GET_SCR_WIDTH()  (g_screen_rotation == 1 || g_screen_rotation == 3 ? LV_USE_VER_SIZE : LV_USE_HOR_SIZE)
#define GET_SCR_HEIGHT() (g_screen_rotation == 1 || g_screen_rotation == 3 ? LV_USE_HOR_SIZE : LV_USE_VER_SIZE)
#else
#define GET_SCR_WIDTH()  LV_USE_HOR_SIZE
#define GET_SCR_HEIGHT() LV_USE_VER_SIZE
#endif

//**********static variable************
static progress_callback s_progress_cb = NULL;
static pthread_mutex_t s_player_voice_mutex = PTHREAD_MUTEX_INITIALIZER;;
static void *s_voice_player = NULL;
static pthread_mutex_t s_voice_over_mutex = PTHREAD_MUTEX_INITIALIZER;;
static int s_voice_over = 1;
static int setting_vol;
extern const lv_image_dsc_t mediaplayer_play;
extern int _get_scr_width(void);
extern int _get_scr_height(void);

typedef struct _xos_player_volume_t_ {
    void * player;
    int volume;
} XOS_PLAYER_VOLUME_T;

typedef struct _xos_player_t_ {
    void **pOutPlayer;
    int display_idx;
    lv_area_t area;
    bool auto_center;
    bool auto_restart;
    char videoPath[256];
    int rotation;// 0:0  1:90  2：180  3:270
} XOS_PLAYER_T;

static XOS_PLAYER_VOLUME_T s_xos_player_volume = {0};

#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
static const QUA_CHAR* display_id[] = {
    "id:display0",
    "id:display1",
    "id:display2",
    "id:display3",
};
#endif
#endif
//*****

static void xos_player_debug_log(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    FILE *fp = fopen("/data/overlay_debug.log", "a");
    if (fp != NULL) {
        va_start(ap, fmt);
        vfprintf(fp, fmt, ap);
        va_end(ap);
        fclose(fp);
    }
#endif
}

////////////////////implements//////////////////////////
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
#define USEC_TO_SECONDS (1000 * 1000)
static int my_call_back(void* user, int evt, void* info)
{
    #if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
    (void)user;
    if (evt == PLAYER_EVT_PLAYBACK_COMPLETE) {
        // is_stop = QUA_TRUE;
        // printf("call_back evt = %d\n", evt);
        if (s_progress_cb != NULL) {
            s_progress_cb(user,evt, 0);
        }
    } else if (evt == PLAYER_EVT_CURRENT_POSITION) {
        // printf("pts = %lld\n", (long long)(*(QUA_U64*)info));
        if (s_progress_cb != NULL && info != NULL) {
            s_progress_cb(user,evt, (long long)(*(QUA_U64*)info)/USEC_TO_SECONDS);
        }
    }
    #endif
    return 0;
}

#else

static void my_ffmpeg_progress_callback(double current_time)
{
    if (s_progress_cb != NULL) {
        s_progress_cb(NULL,3, current_time);
    }
}

#endif

static int my_voice_callback(void* user, int evt, void* info)
{
    #if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    if (evt == PLAYER_EVT_PLAYBACK_COMPLETE) {
        xos_player_destroy_async(user);
        #if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
        #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        pthread_mutex_lock(&s_player_voice_mutex);
        s_voice_player = NULL;
        pthread_mutex_unlock(&s_player_voice_mutex);
        pthread_mutex_lock(&s_voice_over_mutex);
        s_voice_over = 1;
        pthread_mutex_unlock(&s_voice_over_mutex);
        #endif
        #endif
    }
    else if (evt == PLAYER_EVT_STOP_STATUS){
        pthread_mutex_lock(&s_voice_over_mutex);
        s_voice_over = 1;
        pthread_mutex_unlock(&s_voice_over_mutex);
    }
    #endif
    #endif
    return 0;
}

int xos_player_destroyvoice(void)
{
    #if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    pthread_mutex_lock(&s_player_voice_mutex);
    if (s_voice_player) {
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        qua_mm_player_stop(s_voice_player);
        qua_mm_player_destroy(s_voice_player);
#endif
    }
    s_voice_player = NULL;
    pthread_mutex_unlock(&s_player_voice_mutex);
    pthread_mutex_lock(&s_voice_over_mutex);
    s_voice_over = 1;
    pthread_mutex_unlock(&s_voice_over_mutex);
    #endif
    #endif
    return 0;
}

void *xos_player_playvoice(const char * videoPath, bool auto_restart)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    // LV_LOG_USER("[losplayer]path=%s. (%d,%d)\n", videoPath, area.x1, area.y1);
    pthread_mutex_lock(&s_voice_over_mutex);
    if (!s_voice_over) {
        LV_LOG_WARN("voice is playing now!\n");
        pthread_mutex_unlock(&s_voice_over_mutex);
        return NULL;
    }
    s_voice_over = 0;
    pthread_mutex_unlock(&s_voice_over_mutex);
    QUA_S32 ret = 0;
    pthread_mutex_lock(&s_player_voice_mutex);
    void *player = s_voice_player;

    if (player) {
        // restart
        qua_mm_player_seek(player, 0);
        if (ret != 0) {
            LV_LOG_ERROR("player: seek fail. ret=%d\n", ret);
            goto fail;
        }
        ret = qua_mm_player_start(player);
        if (ret != 0) {
            LV_LOG_ERROR("player: start fail. ret=%d\n", ret);
            goto fail;
        }
    } else {
        printf("start voice filename: %s\n", videoPath);
        player = qua_mm_player_create(videoPath);
        if (player == NULL) {
            LV_LOG_ERROR("player: %p\n", player);
            goto fail;
        }
        ret = qua_mm_player_set_data_source(player, videoPath);
        if (ret != 0) {
            printf("player: set_data_source fail\n");
            goto fail;
        }
        ret = qua_mm_player_prepare(player);
        if (ret != 0) {
            LV_LOG_ERROR("player: prepare fail. ret=%d\n", ret);
            goto fail;
        }
        ret = qua_mm_player_set_callback(player, my_voice_callback, player);
        if (ret != 0) {
            LV_LOG_ERROR("player: set_callback fail. ret=%d\n", ret);
            goto fail;
        }
        ret = qua_mm_player_start(player);
        if (ret != 0) {
            LV_LOG_ERROR("player: start fail. ret=%d\n", ret);
            goto fail;
        }
        s_voice_player = player;
    }
    xos_player_set_volume(player, setting_vol);

    pthread_mutex_unlock(&s_player_voice_mutex);
    return s_voice_player;
fail:
    qua_mm_player_destroy(player);
    s_voice_player = NULL;
    pthread_mutex_unlock(&s_player_voice_mutex);
    pthread_mutex_lock(&s_voice_over_mutex);
    s_voice_over = 1;
    pthread_mutex_unlock(&s_voice_over_mutex);
    return NULL;
#endif
#endif //CONFIG_XOS_FWK_PLAYER
    return NULL;
}
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if !defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 0
static void calculate_display_rect(int img_width, int img_height, lv_area_t *area, qua_rect_t *rect,bool auto_center) {
    int is_screen_landscape = GET_SCR_WIDTH() > GET_SCR_HEIGHT();
    int is_image_landscape = img_width > img_height;
    int is_adaptive = (is_image_landscape != is_screen_landscape);
    LV_LOG_USER("area [%d %d]  area rect size [%d %d]", area->x1, area->y1, area->x2, area->y2);
    LV_LOG_USER("img_width: %d, img_height:%d\n", img_width, img_height);
    rect->x = area->x1;
    rect->y = area->y1;
    rect->width = area->x2 - area->x1 + 1;
    rect->height = area->y2 - area->y1 + 1;
    //LV_LOG_USER("rect [%d %d] rect size [%d %d]", rect->x, rect->y, rect->width, rect->height);
    //LV_LOG_USER("screen [%d %d] image [%d %d]", _get_scr_width(), _get_scr_height(), img_width, img_height);
    //LV_LOG_USER("auto_center =%d,is_adaptive=%d", auto_center, is_adaptive);
    if(auto_center){
        if (true) {
            float screen_ratio = (float)GET_SCR_WIDTH() / GET_SCR_HEIGHT();
            float image_ratio = (float)img_width / img_height;
            //LV_LOG_USER("screen_ratio =%f,image_ratio=%d", screen_ratio, image_ratio);
            if (screen_ratio > image_ratio) {
                rect->width = GET_SCR_HEIGHT() * img_width / img_height;
                rect->height = GET_SCR_HEIGHT();
                rect->x = (GET_SCR_WIDTH() - rect->width) / 2;
                rect->y = 0;
                if (rect->x > 0 && rect->x % 2 > 0)
                    rect->x += 1;
            } else {
                rect->width = GET_SCR_WIDTH();
                rect->height = GET_SCR_WIDTH() * img_height / img_width;
                rect->x = 0;
                rect->y = (GET_SCR_HEIGHT() - rect->height) / 2;
                if (rect->y > 0 && rect->y % 2 > 0)
                    rect->y += 1;
                    //rect->height = (area->y2- area->y1)/ 4 *4;
                    //LV_LOG_USER("#######");
            }
        }
    }else{
        rect->x = area->x1 / 2 * 2 ;
        rect->y = area->y1 / 2 * 2 ;
        rect->width = area->x2 / 8 * 8;
        rect->height = area->y2 / 8 *8;
        //LV_LOG_USER("@@@@@@@@@");
    }
    rect->width = rect->width / 8 * 8;
    rect->height = rect->height / 8 *8;
    LV_LOG_USER("display rect [%d %d %d %d]", rect->x, rect->y, rect->width, rect->height);
}

#endif
#endif
void *los_player_create(lv_obj_t *parent, const char * videoPath, lv_area_t area, bool auto_restart){
    return los_player_create2(parent,videoPath,area,auto_restart,true,0,0);
}

static void *xos_player_create_delay_thread(void *arg)
{
    XOS_PLAYER_T *xos_player = (XOS_PLAYER_T *)arg;
    usleep(300*1000);
    *(xos_player->pOutPlayer) = los_player_create2(NULL,xos_player->videoPath,xos_player->area,xos_player->auto_restart,xos_player->auto_center,xos_player->display_idx,xos_player->rotation);
    free(xos_player);
    return NULL;
}

bool xos_player_video_delay(lv_obj_t *parent, const char * videoPath, lv_area_t area, bool auto_restart,bool auto_center,int display_idx,void **player,int rotation)
{
    pthread_t tid;
    XOS_PLAYER_T *xos_player = (XOS_PLAYER_T *)malloc(sizeof(XOS_PLAYER_T));
    xos_player->area = area;
    xos_player->auto_center = auto_center;
    xos_player->auto_restart = auto_restart;
    xos_player->display_idx = display_idx;
    xos_player->pOutPlayer = player;
    xos_player->rotation = rotation;
    strcpy(xos_player->videoPath, videoPath);
    pthread_create(&tid, NULL, xos_player_create_delay_thread, xos_player);
    pthread_detach(tid);
    return true;
}


void *los_player_create2(lv_obj_t *parent, const char * videoPath, lv_area_t area, bool auto_restart,bool auto_center,int display_idx,int rotation)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
     LV_LOG_USER("[losplayer]path=%s. (%d,%d) (%d, %d)\n", videoPath, area.x1, area.y1, area.x2, area.y2);
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    lv_obj_t *player = NULL;
    #if LV_USE_FFMPEG == 1
    player = lv_ffmpeg_player_create(parent);
    lv_res_t res = lv_ffmpeg_player_set_src(player, videoPath);
    if (res != LV_RES_OK) {
        return NULL;
    }
    lv_ffmpeg_player_set_auto_restart(player, true);
    lv_ffmpeg_player_set_cmd(player, LV_FFMPEG_PLAYER_CMD_START);

    lv_obj_center(player);
    #endif
    return player;
#else
    void *player = NULL;
    QUA_S32 ret = 0;
    QUA_S64 durationUs = 0;
    QUA_BOOL pos_cb = QUA_TRUE;

    char *new_videoPath = (char *)videoPath;
    const char *fs_videoPath = videoPath;
    if (strncmp(videoPath, "H:", 2) == 0) {
        new_videoPath = (char *)videoPath + 2;
        fs_videoPath = new_videoPath;
        printf("new src_filename: %s\n", new_videoPath);
    } else if (videoPath[0] != '/') {
        while(*new_videoPath != '/' && *new_videoPath!= '\0') {
            new_videoPath++;
        }
        fs_videoPath = new_videoPath;

        printf("new src_filename: %s\n", new_videoPath);
    }
    else {
        printf("start src_filename: %s\n", videoPath);
    }
    xos_player_debug_log("[XOSPLAYER] create2 raw=%s new=%s display=%d area=(%d,%d,%d,%d) rotation=%d access=%d\n",
                         videoPath, new_videoPath, display_idx,
                         area.x1, area.y1, area.x2, area.y2, rotation,
                         access(fs_videoPath, R_OK));
    player = qua_mm_player_create(new_videoPath);
    if (player == NULL) {
        xos_player_debug_log("[XOSPLAYER] qua_mm_player_create failed path=%s\n", new_videoPath);
        LV_LOG_ERROR("player: %p\n", player);
        goto fail;
    }
    ret = qua_mm_player_set_data_source(player, new_videoPath);
    if (ret != 0) {
        xos_player_debug_log("[XOSPLAYER] set_data_source failed ret=%d path=%s\n", ret, new_videoPath);
        printf("player: set_data_source fail\n");
        goto fail;
    }
    #if defined(CONFIG_XOS_USE_APP_NATIVE_ESL) || defined(CONFIG_XOS_APP_ESL2)
    #else
    QUA_S32 fps = 0;
    qua_mm_player_get_parameter(player, KEY_PARAMETER_VIDEO_FRAMERATE, (QUA_VOID_PTR)&fps);
    if( fps > 30){
        printf("player: fps =%d\n",fps);
        goto fail;
    }
    #endif

    qua_size_t size;
    qua_rect_t rect, disp_rect, hole_rect;
    int disp_width, disp_height;
    qua_mm_player_get_parameter(player, KEY_PARAMETER_VIDEO_SIZE, (QUA_VOID_PTR)&size);
    if (size.width <= 0 || size.height <= 0) {
        size.width = area.x2 - area.x1 + 1;
        size.height = area.y2 - area.y1 + 1;
        xos_player_debug_log("[XOSPLAYER] video_size unavailable, fallback width=%d height=%d\n",
                             size.width, size.height);
    }
    xos_player_debug_log("[XOSPLAYER] video_size width=%d height=%d\n", size.width, size.height);
    #if defined(CONFIG_XOS_USE_APP_NATIVE_ESL) || defined(CONFIG_XOS_APP_ESL2)
    #else
    if( size.width * size.height > 1920 * 1080){
        printf("player: source size.width = %d,height = %d\n",size.width ,size.height);
        goto fail;
    }
    #endif
    calculate_display_rect(size.width, size.height, &area, &rect,auto_center);
    //LV_LOG_USER("rect.width=%d. height=%d . display_idx = %d\n", rect.width, rect.height,display_idx);
    //LV_LOG_USER("[losplayer]path=%s. (%d,%d),(%d,%d)\n", videoPath, area.x1, area.y1, area.x2, area.y2);
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_DISPLAY_ID, (QUA_VOID_PTR)display_id[display_idx]);
    #endif
    disp_rect.x = 0;
    disp_rect.y = 0;
    disp_rect.width = GET_SCR_WIDTH();
    disp_rect.height = GET_SCR_HEIGHT();
#if defined(CONFIG_CHIPSET_QM10XD) && (!defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0)
    if (g_screen_rotation == 1 || g_screen_rotation == 3) {
        hole_rect.y = rect.y * LV_LOGIC_HOR_SIZE / LV_USE_HOR_SIZE;
        hole_rect.x = rect.x * LV_LOGIC_VER_SIZE / LV_USE_VER_SIZE;
        hole_rect.height = rect.height * LV_LOGIC_HOR_SIZE / LV_USE_HOR_SIZE;
        hole_rect.width = rect.width * LV_LOGIC_VER_SIZE / LV_USE_VER_SIZE;
    } else {
        hole_rect.x = rect.x * LV_LOGIC_HOR_SIZE / LV_USE_HOR_SIZE;
        hole_rect.y = rect.y * LV_LOGIC_VER_SIZE / LV_USE_VER_SIZE;
        hole_rect.width = rect.width * LV_LOGIC_HOR_SIZE / LV_USE_HOR_SIZE;
        hole_rect.height = rect.height * LV_LOGIC_VER_SIZE / LV_USE_VER_SIZE;
    }
#else
    hole_rect = rect;
#endif
    LV_LOG_USER("disp_rect=[%d, %d] hole_rect=[%d, %d]", disp_rect.width, disp_rect.height, hole_rect.width, hole_rect.height);
    qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_DISPLAY_RECT, (QUA_VOID_PTR)&disp_rect);
    qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_CHN_RECT, (QUA_VOID_PTR)&rect);
    #if defined(CONFIG_XOS_USE_APP_FRAMEO)
    LV_LOG_USER("rect(%d,%d,%d,%d),g_screen_rotation=%d\n", rect.x, rect.y, rect.width, rect.height, g_screen_rotation);
    qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_ROTATE, (QUA_VOID_PTR)&g_screen_rotation);
    #else
    LV_LOG_USER("rect(%d,%d,%d,%d),g_screen_rotation=%d\n", rect.x, rect.y, rect.width, rect.height, rotation);
    qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_ROTATE, (QUA_VOID_PTR)&rotation);
    #endif
    // s_qmplayer = player;
    ret = qua_mm_player_prepare(player);
    if (ret != 0) {
        xos_player_debug_log("[XOSPLAYER] prepare failed ret=%d path=%s\n", ret, new_videoPath);
        LV_LOG_ERROR("player: prepare fail. ret=%d\n", ret);
        goto fail;
    }
    ret = qua_mm_player_set_callback(player, my_call_back, player);
    if (ret != 0) {
        xos_player_debug_log("[XOSPLAYER] set_callback failed ret=%d\n", ret);
        LV_LOG_ERROR("player: set_callback fail. ret=%d\n", ret);
        goto fail;
    }
    qua_mm_player_set_parameter(player, KEY_PARAMETER_SET_CURRENT_POSITION_CALLBACK, (QUA_VOID*)&pos_cb);
    qua_mm_player_start(player);

    s_xos_player_volume.player = player;
    s_xos_player_volume.volume = setting_vol;
    xos_player_set_volume(player, setting_vol);
    #if defined(CONFIG_XOS_USE_APP_NATIVE_ESL) || defined(CONFIG_XOS_APP_ESL2)
    extern void fbdev_set_hole(int x, int y, int width, int height);
    extern void fbdev2_set_hole(int x, int y, int width, int height);
    //视频播放时，设置挖洞区域。
    if(display_idx == 0){
        fbdev_set_hole(hole_rect.x, hole_rect.y, hole_rect.width, hole_rect.height);
    }else{
        fbdev2_set_hole(hole_rect.x, hole_rect.y, hole_rect.width, hole_rect.height);
    }
    #endif
    xos_player_debug_log("[XOSPLAYER] start success player=%p display=%d hole=(%d,%d,%d,%d)\n",
                         player, display_idx, hole_rect.x, hole_rect.y, hole_rect.width, hole_rect.height);
    return player;
fail:
    xos_player_debug_log("[XOSPLAYER] fail cleanup player=%p\n", player);
    qua_mm_player_destroy(player);
    // s_qmplayer = NULL;
    return NULL;
#endif //!SIMULATOR
#endif //CONFIG_XOS_FWK_PLAYER
    return NULL;
}

int64_t los_get_duration(void *player)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    #if LV_USE_FFMPEG == 1
    return 0;
    //return los_ffmpeg_player_get_duration((void *)player);
    #endif
#else
    if (player != NULL) {
        return qua_mm_player_get_durationUs(player)/USEC_TO_SECONDS;
    }
#endif
    LV_LOG_WARN("No player.");
#endif
    return 0;
}

void los_set_progress_callback(progress_callback progress_cb)
{
    s_progress_cb = progress_cb;
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    #if LV_USE_FFMPEG == 1
    los_ffmpeg_player_set_callback(my_ffmpeg_progress_callback);
    #endif
#else
    
#endif //!simulator
#endif //CONFIG_XOS_FWK_PLAYER
}

void los_player_play(void *player)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    #if LV_USE_FFMPEG == 1
    lv_ffmpeg_player_set_cmd((void *)player, LV_FFMPEG_PLAYER_CMD_START);
    #endif
#else
    LV_LOG_USER("will play(%p)", player);
    if (player != NULL) {
        int ret = qua_mm_player_start(player);
        if (ret != 0) {
            LV_LOG_ERROR("player: start fail. ret=%d\n", ret);
        }
        LV_LOG_USER("after play");
    }
#endif //!simulator
#endif //CONFIG_XOS_FWK_PLAYER
}

void los_player_resume(void *player)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    #if LV_USE_FFMPEG == 1
    lv_ffmpeg_player_set_cmd((void *)player, LV_FFMPEG_PLAYER_CMD_RESUME);
    #endif
#else
    if (player != NULL) {
        LV_LOG_USER("do player resume >>");
        int ret = qua_mm_player_resume(player);
        if (ret != 0) {
            LV_LOG_ERROR("player: resume fail: %d\n", ret);
        }
    }
#endif //NOT BUILD_SIMULATOR
#endif //CONFIG_XOS_FWK_PLAYER
}

void los_player_pause(void *player)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    #if LV_USE_FFMPEG == 1
    lv_ffmpeg_player_set_cmd((void *)player, LV_FFMPEG_PLAYER_CMD_PAUSE);
    #endif
#else
    if (player != NULL) {
        LV_LOG_USER("do player pause >>");
        int ret = qua_mm_player_pause(player);
        if (ret != 0) {
            LV_LOG_ERROR("player: pause fail: %d\n", ret);
        }
    }
#endif //NOT BUILD_SIMULATOR
#endif //CONFIG_XOS_FWK_PLAYER
}

void los_player_seek(void *player, int64_t miliseconds)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    #if LV_USE_FFMPEG == 1
    lv_ffmpeg_player_set_cmd((void *)player, LV_FFMPEG_PLAYER_CMD_RESUME);
    #endif
#else
    
    if (player != NULL) {
        LV_LOG_USER("do player seek >> %lld",miliseconds);
        int ret = qua_mm_player_seek(player, miliseconds);
        if (ret != 0) {
            LV_LOG_ERROR("player: seek fail: %d\n", ret);
        }
    }
#endif //NOT BUILD_SIMULATOR
#endif //CONFIG_XOS_FWK_PLAYER
}

void xos_player_stop_async(void *player)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    LV_LOG_USER("do player stop async >>");
    int r = qua_mm_player_stop_async(player);
    if (r != 0) {
        LV_LOG_ERROR("player: stop async fail. ret=%d\n", r);
    }
    s_xos_player_volume.player = NULL;
#endif
#endif
}

void los_player_stop(void *player)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    #if LV_USE_FFMPEG == 1
    lv_ffmpeg_player_set_cmd((void *)player, LV_FFMPEG_PLAYER_CMD_STOP);
    #endif
#else
    if (player != NULL) {
        LV_LOG_USER("do player stop >>");
        int r = qua_mm_player_stop(player);
        if (r != 0) {
            LV_LOG_ERROR("player: stop fail. ret=%d\n", r);
        }
        s_xos_player_volume.player = NULL;
    }
#endif //not simulator
#endif //CONFIG_XOS_FWK_PLAYER
}

void xos_player_destroy_async(void *player)
{
    xos_player_destroy_async2(player,0);
}

void xos_player_destroy_async2(void *player,int display_idx)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    #if defined(CONFIG_XOS_USE_APP_NATIVE_ESL) || defined(CONFIG_XOS_APP_ESL2)
        extern void fbdev_set_hole(int x, int y, int width, int height);
        extern void fbdev2_set_hole(int x, int y, int width, int height);
        //视频播放时，设置挖洞区域。
        if(display_idx == 0){
            fbdev_set_hole(0,0,0,0);
        }else{
            fbdev2_set_hole(0,0,0,0);
        }
    #endif
    LV_LOG_USER("do player destroy async >>");
    int r = qua_mm_player_destroy_async(player);
    if (r != 0) {
        LV_LOG_ERROR("player: destroy async fail. ret=%d\n", r);
    }
    s_xos_player_volume.player = NULL;
#endif
#endif
}

void xos_player_set_volume_level(void *player, xos_volume_level level) {
    int volume = 0;
    switch (level)
    {
        case VOLUME_LEVEL_MUTE:
            volume = 0;
            break;

        case VOLUME_LEVEL_LOW:
            volume = 20;
            break;

        case VOLUME_LEVEL_MEDIUM:
            volume = 55;
            break;

        case VOLUME_LEVEL_MAX:
        case VOLUME_LEVEL_HIGH:
            volume = 100;
            break;

        default:
            LV_LOG_ERROR("player: wrong volume levle\n");
            return;
    }
    if (player) {
        xos_player_set_volume(player, volume);
    } else {
        setting_vol = volume;
    }
}

void xos_player_set_volume(void *player, int volume) {
    LV_LOG_USER("player volume is %d\n", volume);
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    qua_mm_player_set_volume(player, volume);
#endif
#endif
    setting_vol = volume;
}

void qua_xos_player_set_volume(int volume) {
    LV_LOG_USER("player volume is %d\n", volume);
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
    if (s_xos_player_volume.player) {
        s_xos_player_volume.volume = volume;
        qua_mm_player_set_volume(s_xos_player_volume.player, volume);
    }
#endif
#endif
    setting_vol = volume;
}

void los_player_destroy(void *player)
{
    los_player_destroy2(player,0);
}

void los_player_destroy2(void *player,int display_idx)
{
#if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
#if defined(BUILD_SIMULATOR) && BUILD_SIMULATOR == 1
    #if LV_USE_FFMPEG == 1
    lv_ffmpeg_player_set_cmd((void *)player, LV_FFMPEG_PLAYER_CMD_STOP);
    #endif
    //
    lv_obj_del((void *)player);
#else
    #if defined(CONFIG_XOS_USE_APP_NATIVE_ESL) || defined(CONFIG_XOS_APP_ESL2)
        extern void fbdev_set_hole(int x, int y, int width, int height);
        extern void fbdev2_set_hole(int x, int y, int width, int height);
        //视频播放时，设置挖洞区域。
        if(display_idx == 0){
            fbdev_set_hole(0,0,0,0);
        }else{
            fbdev2_set_hole(0,0,0,0);
        }
    #endif
    if (player != NULL) {
        LV_LOG_USER("do player destroy");
        int r = qua_mm_player_destroy(player);
        if (r != 0) {
            LV_LOG_ERROR("player: destroy fail. ret=%d\n", r);
        }
        LV_LOG_USER("done player quit..");
    }
#endif //not simulator
#endif //CONFIG_XOS_FWK_PLAYER
}

int xos_player_reset(void *player){
    #if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        if (player != NULL) {
            LV_LOG_USER("do player reset >>");
            int ret = qua_mm_player_reset(player);
            if (ret != 0) {
                LV_LOG_ERROR("player: reset fail: %d\n", ret);
            }
            return ret;
        }
    #endif
    #endif
    return 0;
}

static int drop_system_caches() {
    sync();

    int fd = open("/proc/sys/vm/drop_caches", O_WRONLY);
    if (fd < 0) {
        perror("Failed to open drop_caches");
        return -1;
    }
    int ret = write(fd, "3", 1);
    close(fd);
    return (ret == 1) ? 0 : -1;
}

static pthread_mutex_t g_player_datasrc_mutex = PTHREAD_MUTEX_INITIALIZER;

int xos_player_set_data_source(void *player, const char *path){
    #if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        if (player != NULL) {
            LV_LOG_USER("do player set data source >>");
            //drop_system_caches();
            usleep(50000);
            pthread_mutex_lock(&g_player_datasrc_mutex);
            int ret = qua_mm_player_set_data_source(player, path);
            pthread_mutex_unlock(&g_player_datasrc_mutex);
            if (ret != 0) {
                LV_LOG_ERROR("player: set data source fail: %d\n", ret);
            }
            return ret;
        }
    #endif
    #endif
    return 0;
}

int xos_player_set_data_source_with_update_rect(void *player, const char *path,lv_area_t area){
    #if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        if (player != NULL) {
            LV_LOG_USER("do player set data source >>");
            int ret = qua_mm_player_set_data_source(player, path);
            if (ret != 0) {
                LV_LOG_ERROR("player: set data source fail: %d\n", ret);
            }

            QUA_S32 fps = 0;
            qua_mm_player_get_parameter(player, KEY_PARAMETER_VIDEO_FRAMERATE, (QUA_VOID_PTR)&fps);
            if( fps > 30){
                return -1;
            }

            qua_size_t size;
            qua_rect_t rect, chn_rect;
            int disp_width, disp_height;
            rect.x = area.x1;
            rect.y = area.y1;
            rect.width = area.x2 - area.x1 + 1;
            rect.height = area.y2 - area.y1 + 1;
            qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_DISPLAY_RECT, (QUA_VOID_PTR)&rect);
            qua_mm_player_get_parameter(player, KEY_PARAMETER_VIDEO_SIZE, (QUA_VOID_PTR)&size);
            if( size.width * size.height > 1920 * 1080){
                return -1;
            }

            calculate_display_rect(size.width, size.height, &area, &chn_rect,true);
            
            #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
            qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_DISPLAY_ID, (QUA_VOID_PTR)display_id[0]);
            #endif
            #if defined(CONFIG_XOS_USE_APP_FRAMEO)
            qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_ROTATE, (QUA_VOID_PTR)&g_screen_rotation);
            #endif
            qua_mm_player_set_parameter(player, KEY_PARAMETER_VO_CHN_RECT, (QUA_VOID_PTR)&chn_rect);
            return ret;
        }
    #endif
    #endif
    return 0;
}

int xos_player_prepare(void *player){
    #if defined(CONFIG_XOS_FWK_PLAYER) && CONFIG_XOS_FWK_PLAYER != 0
    #if !defined(BUILD_SIMULATOR) || BUILD_SIMULATOR == 0
        if (player != NULL) {
            LV_LOG_USER("do player prepare >>");
            int ret = qua_mm_player_prepare(player);
            if (ret != 0) {
                LV_LOG_ERROR("player: prepare fail: %d\n", ret);
            }
            return ret;
        }
    #endif
    #endif
    return 0;
}
